program VGPlayer;

uses
  TntSystem,
  Forms,
  ufrmMain in 'ufrmMain.pas' {frmMain},
  uVGLib in 'uVGLib.pas',
  uVGFilterManager in 'uVGFilterManager.pas',
  uVGPlayer in 'uVGPlayer.pas',
  uVGException in 'uVGException.pas',
  uVGFilterList in 'uVGFilterList.pas',
  uVGMTLookup in 'uVGMTLookup.pas',
  uVGBase in 'uVGBase.pas',
  uAriaJclExt in '..\common\uAriaJclExt.pas';

{$R *.res}

begin
  //InstallTntSystemUpdates;
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TfrmMain, frmMain);
  Application.Run;
end.
