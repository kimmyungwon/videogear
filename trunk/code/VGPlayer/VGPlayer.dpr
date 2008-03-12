program VGPlayer;

uses
  Forms,
  ufrmMain in 'ufrmMain.pas' {frmMain},
  uVGFakeReg in '..\VGLib\uVGFakeReg.pas',
  uAriaTree in '..\Common\uAriaTree.pas',
  uVGRegDB in '..\VGLib\uVGRegDB.pas',
  MD5 in '..\Common\MD5.pas';

{$R *.res}

begin
  ReportMemoryLeaksOnShutdown := True;
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TfrmMain, frmMain);
  Application.Run;
end.
