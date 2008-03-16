unit uVGPlayer;

interface

uses
  Windows, DirectShow9, uVGFilterManager;

type
  TVGPlayer = class(TVGFilterManager)
  private
    FMC: IMediaControl;
    FVW: IVideoWindow;
  public
    function Play: HRESULT;
    function RenderFile(const AFileName: WideString): HRESULT; override;
  end;

implementation

{ TVGPlayer }

function TVGPlayer.Play: HRESULT;
begin
  If FMC = nil then
  begin
    Result := E_POINTER;
    Exit;
  end;

  Result := FMC.Run;
  if FVW <> nil then
  begin
    FVW.put_Visible(True);
  end;
end;

function TVGPlayer.RenderFile(const AFileName: WideString): HRESULT;
begin
  Result := inherited RenderFile(AFileName);
  if Succeeded(Result) then
  begin
    FMC := FGB as IMediaControl;
    FVW := FGB as IVideoWindow;
  end;
end;

end.
