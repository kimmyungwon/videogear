unit uVGLib;

interface

uses
  Windows, SysUtils, ActiveX, DirectShow9, JclPeImage;

type
  IVGFilterList =  interface(IUnknown)
  ['{CA6D4AE8-CFF5-4479-8FE0-11810F2FBFA5}']
    function Add(pBF: IBaseFilter): HRESULT; stdcall;
    function Clear: HRESULT; stdcall;
    function Delete(nIndex: Cardinal): HRESULT; stdcall;
		function Get(nIndex: Cardinal; out pBF: IBaseFilter): HRESULT; stdcall;
		function GetCount: Cardinal; stdcall;
  end;

  function VGEnumMatchingFilters(out pList: IVGFilterList; dwMerit: DWORD; bInputNeeded: BOOL;
    clsInMaj, clsInSub: TCLSID; bRender, bOutputNeeded: BOOL;
    clsOutMaj, clsOutSub: TCLSID): HResult; stdcall; external 'VGLib.dll';

  function VGEnumMatchingSource(lpszFile: PWideChar; out pBF: IBaseFilter): HRESULT; stdcall; external 'VGLib.dll';

implementation

end.
