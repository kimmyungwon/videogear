unit ufrmMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, DirectShow9;

type
  TfrmMain = class(TForm)
    procedure FormDblClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

uses uVGLib;

{$R *.dfm}

procedure TfrmMain.FormDblClick(Sender: TObject);
var
  hr: HRESULT;
  pList: IVGFilterList;
  InputTypes, OutputTypes: array[0..1] of TGUID;
begin
  InputTypes[0] := MEDIATYPE_Video;
  InputTypes[1] := MEDIASUBTYPE_NULL;
  OutputTypes[0] := MEDIATYPE_Video;
  OutputTypes[1] := MEDIASUBTYPE_RGB24;
  hr := VGEnumMatchingFilters(pList, 0, False, MERIT_DO_NOT_USE, True, Length(InputTypes),
    @InputTypes[0], nil, nil, False, True, Length(OutputTypes), @OutputTypes[0],
    nil, nil);
  if Failed(hr) then
    Application.MessageBox(PChar(Format('Ê§°Ü£º%s', [SysErrorMessage(HResultCode(hr))])), '´íÎó', MB_OK + 
      MB_ICONSTOP);
end;

end.
