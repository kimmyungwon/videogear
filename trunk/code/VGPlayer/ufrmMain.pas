unit ufrmMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, uVGPlayer;

type
  TfrmMain = class(TForm)
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormDblClick(Sender: TObject);
  private
    FPlayer: TVGPlayer;
  public
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

uses uVGLib;

{$R *.dfm}

procedure TfrmMain.FormCreate(Sender: TObject);
begin
  FPlayer := TVGPlayer.Create;
end;

procedure TfrmMain.FormDblClick(Sender: TObject);
begin
  if Succeeded(FPlayer.RenderFile('d:\test.rmvb')) then
    FPlayer.Play;
end;

procedure TfrmMain.FormDestroy(Sender: TObject);
begin
  FreeAndNil(FPlayer);
end;

end.
