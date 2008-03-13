unit ufrmMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, TntSysUtils, TntForms;

type
  TfrmMain = class(TTntForm)
    procedure FormDblClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

uses uVGFakeReg;

{$R *.dfm}

procedure TfrmMain.FormDblClick(Sender: TObject);
var
  fr: TVGFakeRegister;
  hDLL: THandle;
  pRegSvr: function: HRESULT; stdcall;
begin
  fr := TVGFakeRegister.Create;
  try
    hDLL := fr.LoadLibrary('ffdshow\ffdshow.ax');
    pRegSvr := GetProcAddress(hDLL, 'DllRegisterServer');
    pRegSvr;
  finally
    fr.Free;
  end;
end;

end.
