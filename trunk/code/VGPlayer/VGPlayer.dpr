program VGPlayer;

uses
  Forms,
  ufrmMain in 'ufrmMain.pas' {frmMain},
  uVGFilterManager in '..\VGLib\uVGFilterManager.pas',
  uBaseVideoFilter in '..\VGLib\uBaseVideoFilter.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TfrmMain, frmMain);
  Application.Run;
end.
