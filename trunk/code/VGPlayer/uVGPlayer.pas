unit uVGPlayer;

interface

uses
  Windows, Messages, Classes, SysUtils, Controls, WideStrings, 
  DirectShow9, uVGFilterManager;

type
  TVGPlayerStatus = (vpsUninitialized, vpsInitialized, vpsStopped,
                    vpsPlaying, vpsPaused);
  TVGPlayer = class;
  TVGPlayerNotifyEvent = procedure(APlayer: TVGPlayer) of object;
  TVGPlayerStatusChanged = procedure(APlayer: TVGPlayer; AOldStatus, ANewStatus: TVGPlayerStatus) of object;

  TVGPlayer = class(TVGFilterManager)
  private
    FStatus: TVGPlayerStatus;
    FVideoWnd: TWinControl;
    FVidWndProc: TWndMethod;
    FMC: IMediaControl;
    FMS: IMediaSeeking;
    FVW: IVideoWindow;
    FBV: IBasicVideo;
    // 事件
    FOnStatusChanged: TVGPlayerStatusChanged;
  protected
    procedure AdjustVideo;
    function GetDuration: Cardinal;
    function GetPosition: Cardinal;
    function GetVideoWidth: Integer;
    function GetVideoHeight: Integer;
    procedure SetStatus(const Value: TVGPlayerStatus);
    procedure VidWndProc(var Message: TMessage);
  public
    constructor Create;
    destructor Destroy; override;
    function Init(AVideoWnd: TWinControl): HRESULT;
    function Pause: HRESULT;
    function Play: HRESULT;
    function PlayOrPause: HRESULT;
    function RenderFile(const AFileName: WideString): HRESULT; override;
    procedure SetPosition(const Value: Cardinal);
    function Stop: HRESULT;
  public
    property Duration: Cardinal read GetDuration;
    property Position: Cardinal read GetPosition write SetPosition;
    property Status: TVGPlayerStatus read FStatus;
    property VideoHeight: Integer read GetVideoHeight;
    property VideoWidth: Integer read GetVideoWidth;
    // 事件
    property OnStatusChanged: TVGPlayerStatusChanged read FOnStatusChanged
      write FOnStatusChanged;
  end;

implementation

uses uAriaGraphicUtils, uAriaDebug, DSUtil;

{ TVGPlayer }

procedure TVGPlayer.AdjustVideo;
var
  nVidW, nVidH: Integer;
begin
  if (FVW <> nil) and (FBV <> nil) then
  begin
    FBV.get_VideoWidth(nVidW);
    FBV.get_VideoHeight(nVidH);
    if (nVidW <= 0) or (nVidH <= 0) then
      Exit;
    with FitRectToRect(Rect(0, 0, nVidW, nVidH), FVideoWnd.ClientRect) do
    begin
      FVW.put_Left(Left);
      FVW.put_Top(Top);
      FVW.put_Width(Right - Left);
      FVW.put_Height(Bottom - Top);
    end;
  end;
end;

constructor TVGPlayer.Create;
begin
  inherited Create;
  FStatus := vpsUninitialized;
end;

destructor TVGPlayer.Destroy;
begin
  Stop;
  inherited;
end;

function TVGPlayer.GetDuration: Cardinal;
var
  nDuration: Int64;
begin
  Result := 0;
  if FMS = nil then
    Exit;

  FMS.GetDuration(nDuration);
  Result := RefTimeToMiliSec(nDuration);
end;

function TVGPlayer.GetPosition: Cardinal;
var
  nCurPos: Int64;
begin
  Result := 0;
  if FMS = nil then
    Exit;

  FMS.GetCurrentPosition(nCurPos);
  Result := RefTimeToMiliSec(nCurPos);
end;

function TVGPlayer.GetVideoHeight: Integer;
begin
  if FBV <> nil then
    FBV.get_VideoHeight(Result)
  else
    Result := 0;
end;

function TVGPlayer.GetVideoWidth: Integer;
begin
  if FBV <> nil then
    FBV.get_VideoWidth(Result)
  else
    Result := 0;
end;

function TVGPlayer.Init(AVideoWnd: TWinControl): HRESULT;
begin
  FVideoWnd := AVideoWnd;
  FVidWndProc := FVideoWnd.WindowProc;
  FVideoWnd.WindowProc := VidWndProc;
  SetStatus(vpsInitialized); 
  Result := S_OK;
end;

function TVGPlayer.Pause: HRESULT;
begin
  If FMC = nil then
  begin
    Result := E_POINTER;
    Exit;
  end;

  Result := FMC.Pause;
  if Succeeded(Result) then
    SetStatus(vpsPaused);
end;

function TVGPlayer.Play: HRESULT;
begin
  If FMC = nil then
  begin
    Result := E_POINTER;
    Exit;
  end;

  if (FVW <> nil) and (FBV <> nil) then
  begin
    AdjustVideo;
    FVW.put_Owner(OAHWND(FVideoWnd.Handle));
    FVW.put_WindowStyle(WS_CHILD or WS_CLIPSIBLINGS);
    FVW.put_Visible(True);
  end;
  Result := FMC.Run;
  if Succeeded(Result) then
    SetStatus(vpsPlaying);
  SetPosition(0);
end;

function TVGPlayer.PlayOrPause: HRESULT;
begin
  if Status in [vpsStopped, vpsPaused] then
  begin
    Result := Play;   
  end
  else if Status = vpsPlaying then
  begin
    Result := Pause;
  end
  else
    Result := E_FAIL;
end;

function TVGPlayer.RenderFile(const AFileName: WideString): HRESULT;
begin
  Stop;
  Result := inherited RenderFile(AFileName);
  if Succeeded(Result) then
  begin
    FGB.QueryInterface(IID_IMediaControl, FMC);
    FGB.QueryInterface(IID_IMediaSeeking, FMS);
    FGB.QueryInterface(IID_IVideoWindow, FVW);
    FGB.QueryInterface(IID_IBasicVideo, FBV);
    SetStatus(vpsStopped);
    {$IFDEF DEBUG}
    SaveGraphFile(FGB, 'd:\test.grf');
    {$ENDIF}
  end;
end;

procedure TVGPlayer.SetPosition(const Value: Cardinal);
var
  nCurPos, nStopPos: Int64;
begin
  if FMS = nil then
    Exit;

  nCurPos := MiliSecToRefTime(Value);
  FMS.SetPositions(nCurPos, AM_SEEKING_AbsolutePositioning, nStopPos, AM_SEEKING_NoPositioning);
end;

procedure TVGPlayer.SetStatus(const Value: TVGPlayerStatus);
begin
  if Assigned(FOnStatusChanged) then
    FOnStatusChanged(Self, FStatus, Value);
  FStatus := Value;
end;

function TVGPlayer.Stop: HRESULT;
begin
  If FMC = nil then
  begin
    Result := E_POINTER;
    Exit;
  end;

  Result := FMC.Stop;
  if Succeeded(Result) then
  begin
    SetStatus(vpsInitialized);
    if FVW <> nil then
    begin
      FVW.put_Visible(False);
      FVW.put_Owner(0);
    end;
    FMC := nil;
    FMS := nil;
    FVW := nil;
    FBV := nil;
  end;
end;

procedure TVGPlayer.VidWndProc(var Message: TMessage);
begin
  FVidWndProc(Message);
  case Message.Msg of
    WM_MOVE: begin
      if FVW <> nil then
        FVW.NotifyOwnerMessage(FVideoWnd.Handle, Message.Msg, Message.WParam,
          Message.LParam);
    end;
    WM_SIZE: begin
      AdjustVideo;
    end;
  end;
end;

end.
