unit uVGPlayer;

interface

uses
  Windows, Messages, Classes, Controls, DirectShow9, uVGFilterManager;

type
  TVGPlayer = class(TVGFilterManager)
  private
    FVideoWnd: TWinControl;
    FVidWndProc: TWndMethod;
    FMC: IMediaControl;
    FVW: IVideoWindow;
    FBV: IBasicVideo;
  protected
    procedure AdjustVideo;
    procedure VidWndProc(var Message: TMessage);
  public
    destructor Destroy; override;
    function Init(AVideoWnd: TWinControl): HRESULT;
    function Play: HRESULT;
    function RenderFile(const AFileName: WideString): HRESULT; override;
    function Stop: HRESULT;
    function ZoomVideo();
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

destructor TVGPlayer.Destroy;
begin
  Stop;
  inherited;
end;

function TVGPlayer.Init(AVideoWnd: TWinControl): HRESULT;
begin
  FVideoWnd := AVideoWnd;
  FVidWndProc := FVideoWnd.WindowProc;
  FVideoWnd.WindowProc := VidWndProc; 
  Result := S_OK;
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
end;

function TVGPlayer.RenderFile(const AFileName: WideString): HRESULT;
begin
  Result := inherited RenderFile(AFileName);
  if Succeeded(Result) then
  begin
    FMC := FGB as IMediaControl;
    FVW := FGB as IVideoWindow;
    FBV := FGB as IBasicVideo;
  end;
end;

function TVGPlayer.Stop: HRESULT;
begin
  If FMC = nil then
  begin
    Result := E_POINTER;
    Exit;
  end;

  Result := FMC.Stop;
  if FVW <> nil then
  begin
    FVW.put_Visible(False);
    FVW.put_Owner(0);
    FVW := nil;
  end;
  FMC := nil;
  Clear;
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
