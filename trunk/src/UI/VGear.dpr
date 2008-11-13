program VGear;

uses
  Forms,
  FormMain in 'FormMain.pas' {frmMain},
  PlayerIntf in 'PlayerIntf.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TfrmMain, frmMain);
  Application.Run;
end.
