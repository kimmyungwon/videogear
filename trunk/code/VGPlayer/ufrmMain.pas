unit ufrmMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ActiveX, DirectShow9, DSPack, DSUtil;

type
  TfrmMain = class(TForm)
    procedure FormDblClick(Sender: TObject);
  private
    pGB: IGraphBuilder;
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
  InTypes: array[0..1] of TGUID;
  pSource, pRMVDec: IBaseFilter;
  szMsg: array[0..1024] of Char; 
begin
  CoCreateInstance(CLSID_FilterGraph, nil, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, pGB);
  CoCreateInstance(CLSID_AsyncReader, nil, CLSCTX_INPROC_SERVER, IID_IBaseFilter, pSource);
  InTypes[0] := MEDIATYPE_Stream;
  InTypes[1] := MEDIASUBTYPE_NULL;
  if Succeeded(VGEnumMatchingFilters(pList, 0, False, MERIT_NORMAL, True, Length(InTypes), @InTypes[0], nil, nil,
    False, False, 0, nil, nil, nil)) then
  begin
    pList.Get(0, pRMVDec);
    (pSource as IFileSourceFilter).Load('d:\test.rmvb', nil);
    pGB.AddFilter(pSource, nil);
    pGB.AddFilter(pRMVDec, nil);
    pGB.Connect(GetOutPin(pSource, 0), GetInPin(pRMVDec, 0));
    pGB.Render(GetOutPin(pRMVDec, 0));
    (pGB as IMediaControl).Run;
  end;
end;

end.
