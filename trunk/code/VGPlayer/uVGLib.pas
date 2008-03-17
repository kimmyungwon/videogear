unit uVGLib;

{$WARN SYMBOL_PLATFORM OFF}

interface

uses
  Windows, Classes, SysUtils, ActiveX, DirectShow9, JclPeImage;

type
  IVGFilter =  interface(IUnknown)
  ['{6145535B-A312-438c-860C-7558ED1D6EB7}']
    function CreateInstance(out ppvObj: IBaseFilter): HRESULT; stdcall;
    function GetCLSID: TCLSID; stdcall;
		function GetMerit: DWORD; stdcall;
		function GetName: PWideChar; stdcall;
    function GetPinCount: DWORD; stdcall;
    function GetPinInfo(out pInfo: TRegFilterPins): HRESULT; stdcall;
  end;

  function GetInternalFilters(out ppvObj: IEnumUnknown): HRESULT; stdcall; external 'VGLib.dll' index 0;
  function GetMatchingInternalFilters(AMerit: Cardinal; AInputNeeded: Boolean; AClsInMaj, AClsInSub: TCLSID;
    ARender, AOutputNeeded: Boolean; AClsOutMaj, AClsOutSub: TCLSID): TList;

var
  gInternalFilters: IEnumUnknown;

implementation

function GetMatchingInternalFilters(AMerit: Cardinal; AInputNeeded: Boolean; AClsInMaj, AClsInSub: TCLSID;
  ARender, AOutputNeeded: Boolean; AClsOutMaj, AClsOutSub: TCLSID): TList;
begin
  Result := nil;
end;

initialization
  GetInternalFilters(gInternalFilters);

finalization
  gInternalFilters := nil;

end.
