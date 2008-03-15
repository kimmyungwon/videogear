unit uVGLib;

interface

uses
  Windows, ActiveX, DirectShow9;

type
  IVGFilterList =  interface(IUnknown)
  ['{CA6D4AE8-CFF5-4479-8FE0-11810F2FBFA5}']
    function Add(pBF: IBaseFilter): HRESULT; stdcall;
    function Clear: HRESULT; stdcall;
    function Delete(nIndex: Cardinal): HRESULT; stdcall;
		function Get(nIndex: Cardinal; out pBF: IBaseFilter): HRESULT; stdcall;
		function GetCount: Cardinal; stdcall;
  end;

  TVGFilterListAdapter = class(TInterfacedObject, IEnumMoniker)
  private
    FList: IVGFilterList;
    FPos: Cardinal;
  public
    constructor Create(AList: IVGFilterList);
    destructor Destroy; override;
    { IEnumMoniker }
    function Next(celt: Longint; out elt; pceltFetched: PLongint): HResult; stdcall;
    function Skip(celt: Longint): HResult; stdcall;
    function Reset: HResult; stdcall;
    function Clone(out enm: IEnumMoniker): HResult; stdcall;
  end;

  function VGEnumMatchingFilters(out pList: IVGFilterList; dwFlags: DWORD; bExactMatch: BOOL;
        dwMerit: DWORD; bInputNeeded: BOOL; cInputTypes: DWORD; pInputTypes: PGUID;
        pMedIn: PREGPINMEDIUM; pPinCategoryIn: PGUID; bRender, bOutputNeeded: BOOL;
        cOutputTypes: DWORD; pOutputTypes: PGUID; pMedOut: PRegPinMedium;
        pPinCategoryOut: PGUID): HResult; stdcall; external 'VGLib.dll';

implementation

{ TVGFilterListAdapter }

function TVGFilterListAdapter.Clone(out enm: IEnumMoniker): HResult;
begin

end;

constructor TVGFilterListAdapter.Create(AList: IVGFilterList);
begin
  FList := AList;
  FPos := 0;
end;

destructor TVGFilterListAdapter.Destroy;
begin
  FList := nil;
  inherited;
end;

function TVGFilterListAdapter.Next(celt: Integer; out elt; pceltFetched: PLongint): HResult;
begin

end;

function TVGFilterListAdapter.Reset: HResult;
begin
  FPos := 0;
end;

function TVGFilterListAdapter.Skip(celt: Integer): HResult;
begin

end;

end.
