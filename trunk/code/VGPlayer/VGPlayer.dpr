program VGPlayer;

uses
  Forms,
  ufrmMain in 'ufrmMain.pas' {frmMain},
  uVGLib in 'uVGLib.pas',
  uVGFilterManager in 'uVGFilterManager.pas',
  uVGPlayer in 'uVGPlayer.pas',
  uVGException in 'uVGException.pas',
  uVGFilterManager2 in 'uVGFilterManager2.pas',
  uVGFilterList in 'uVGFilterList.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TfrmMain, frmMain);
  Application.Run;
end.
