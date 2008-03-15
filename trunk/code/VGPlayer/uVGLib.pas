unit uVGLib;

interface

uses
  Windows, DirectShow9;

type
  IVGFilterList =  interface(IUnknown)
  ['{CA6D4AE8-CFF5-4479-8FE0-11810F2FBFA5}']
    function Add(pBF: IBaseFilter): HRESULT; stdcall;
    function Clear: HRESULT; stdcall;
    function Delete(nIndex: Cardinal): HRESULT; stdcall;
		function Get(nIndex: Cardinal; out pBF: IBaseFilter): HRESULT; stdcall;
		function GetCount: Cardinal; stdcall;
  end;

  function VGEnumMatchingFilters(out pList: IVGFilterList; dwFlags: DWORD; bExactMatch: BOOL;
        dwMerit: DWORD; bInputNeeded: BOOL; cInputTypes: DWORD; pInputTypes: PGUID;
        pMedIn: PREGPINMEDIUM; pPinCategoryIn: PGUID; bRender, bOutputNeeded: BOOL;
        cOutputTypes: DWORD; pOutputTypes: PGUID; pMedOut: PRegPinMedium;
        pPinCategoryOut: PGUID): HResult; stdcall; external 'VGLib.dll';

implementation

end.
