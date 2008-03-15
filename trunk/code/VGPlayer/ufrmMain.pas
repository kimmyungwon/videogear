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
  pSource, pRMSp, pRMVDec: IBaseFilter;
begin
  CoCreateInstance(CLSID_FilterGraph, nil, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, pGB);
  if Succeeded(VGEnumMatchingSource(pList, 'd:\test.rmvb')) then
  begin
    pList.Get(0, pSource);
    if Succeeded(VGEnumMatchingFilters(pList, MERIT_NORMAL, True, MEDIATYPE_Stream, MEDIASUBTYPE_NULL, False, False,
      MEDIATYPE_NULL, MEDIASUBTYPE_NULL)) then
    begin
      pList.Get(0, pRMSp);
      (pSource as IFileSourceFilter).Load('d:\test.rmvb', nil);
      pGB.AddFilter(pSource, nil);
      if Succeeded(VGEnumMatchingFilters(pList, MERIT_NORMAL, True, MEDIATYPE_Video, MEDIASUBTYPE_NULL, False, False,
        MEDIATYPE_NULL, MEDIASUBTYPE_NULL)) then
      begin
        pList.Get(0, pRMVDec);
        pGB.AddFilter(pRMVDec, nil);
        pGB.Connect(GetOutPin(pRMSp, 1), GetInPin(pRMVDec, 0));
        pGB.Render(GetOutPin(pRMVDec, 0));
        (pGB as IMediaControl).Run;
      end;
    end;
  end;
end;

end.
