program VGPlayer;

uses
  Forms,
  ufrmMain in 'ufrmMain.pas' {frmMain},
  uVGFakeReg in '..\VGLib\uVGFakeReg.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TfrmMain, frmMain);
  Application.Run;
end.
