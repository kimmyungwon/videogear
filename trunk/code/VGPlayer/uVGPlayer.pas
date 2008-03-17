unit uVGPlayer;

interface

uses
  Windows, Messages, Classes, SysUtils, Controls, WideStrings, 
  DirectShow9, uVGFilterManager;

type
  TVGPlayer = class;
  TVGPlayerNotifyEvent = procedure(APlayer: TVGPlayer) of object;

  TVGPlayerStatus = (vpsUninitialized, vpsInitialized, vpsStopped,
                    vpsPlaying, vpsPaused);

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
    FOnRenderComplete: TVGPlayerNotifyEvent;
    function GetVideoWidth: Integer;
    function GetVideoHeight: Integer;
  protected
    procedure AdjustVideo;
    procedure SetStatus(const Value: TVGPlayerStatus);
    procedure VidWndProc(var Message: TMessage);
  public
    constructor Create;
    destructor Destroy; override;
    function GetFilterList: TWideStringList;
    function Init(AVideoWnd: TWinControl): HRESULT;
    function Pause: HRESULT;
    function Play: HRESULT;
    function PlayOrPause: HRESULT;
    function RenderFile(const AFileName: WideString): HRESULT; override;
    function Stop: HRESULT;
  public
    property Status: TVGPlayerStatus read FStatus;
    property VideoHeight: Integer read GetVideoHeight;
    property VideoWidth: Integer read GetVideoWidth;
    // 事件
    property OnRenderComplete: TVGPlayerNotifyEvent read FOnRenderComplete
      write FOnRenderComplete;
  end;

implementation

uses uAriaGraphicUtils, uAriaDebug;

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

function TVGPlayer.GetFilterList: TWideStringList;
var
  pEnum: IEnumFilters;
  pBF: IBaseFilter;
  Info: TFilterInfo;
begin
  Result := nil;
  if (FGB = nil) then
    Exit;
  if Failed(FGB.EnumFilters(pEnum)) then
    Exit;

  Result := TWideStringList.Create;
  while pEnum.Next(1, pBF, nil) = S_OK do
  begin
    pBF.QueryFilterInfo(Info);
    Result.Add(Info.achName);
  end;
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
  Result := inherited RenderFile(AFileName);
  if Succeeded(Result) then
  begin
    FMC := FGB as IMediaControl;
    FMS := FGB as IMediaSeeking;
    FVW := FGB as IVideoWindow;
    FBV := FGB as IBasicVideo;
    SetStatus(vpsStopped);
    if Assigned(FOnRenderComplete) then
      FOnRenderComplete(Self);
  end;
end;

procedure TVGPlayer.SetStatus(const Value: TVGPlayerStatus);
begin
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
    Clear;
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
