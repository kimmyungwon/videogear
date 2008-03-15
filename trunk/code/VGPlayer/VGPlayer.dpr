program VGPlayer;

uses
  Forms,
  ufrmMain in 'ufrmMain.pas' {frmMain},
  uVGLib in 'uVGLib.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TfrmMain, frmMain);
  Application.Run;
end.
