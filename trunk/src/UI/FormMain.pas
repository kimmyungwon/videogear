unit FormMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, PlayerIntf;

type
  TfrmMain = class(TForm)
    procedure FormDestroy(Sender: TObject);
    procedure FormDblClick(Sender: TObject);
  private
    FPlayer: IPlayer;
  protected
    procedure CreateWindowHandle(const Params: TCreateParams); override;
  public
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

{$R *.dfm}

procedure TfrmMain.CreateWindowHandle(const Params: TCreateParams);
begin
  inherited;
  VGCreatePlayer(FPlayer);
  FPlayer.Initialize(WindowHandle, WindowHandle);
end;

procedure TfrmMain.FormDblClick(Sender: TObject);
begin
  FPlayer.OpenUrl('E:\test.rmvb');
  FPlayer.Play;
  RaiseLastOSError;
end;

procedure TfrmMain.FormDestroy(Sender: TObject);
begin
  FPlayer := nil;
end;

end.
