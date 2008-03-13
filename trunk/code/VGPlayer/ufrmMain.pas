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
  hDLL: THandle;
  pRegSvr: function: HRESULT; stdcall;
  //pConfig: procedure(hwnd: HWND; hInst: THandle; lpCmdLine: PChar; nCmdShow: Integer); stdcall;
begin
  gFakeReg.ImportReg('..\..\doc\filters.reg');
  hDLL := gFakeReg.LoadLibrary('ffdshow\ffdshow.ax');
  pRegSvr := GetProcAddress(hDLL, 'DllRegisterServer');
  pRegSvr;
  //pConfig := GetProcAddress(hDLL, 'configure');
  //pConfig(Handle, hDLL, nil, SW_SHOWNORMAL);
end;

end.
