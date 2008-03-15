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

  TFakeFilterMapper2 = class(TInterfacedObject, IFilterMapper2)
  private
    FRealFM2: IFilterMapper2;
  public
    constructor Create;
    { IFilterMapper2 }
    function CreateCategory(const clsidCategory: TGUID; dwCategoryMerit: DWORD;
        Description: PWideChar): HResult; stdcall;
    function UnregisterFilter(const pclsidCategory: TGUID;
        szInstance: PWideChar; const Filter: TGUID): HResult; stdcall;
    function RegisterFilter(const clsidFilter: TGUID; Name: PWideChar;
        ppMoniker: IMoniker; pclsidCategory: PGUID;
        szInstance: PWideChar; const prf2: TRegFilter2): HResult; stdcall;
    function EnumMatchingFilters(out ppEnum: IEnumMoniker; dwFlags: DWORD; bExactMatch: BOOL;
        dwMerit: DWORD; bInputNeeded: BOOL; cInputTypes: DWORD; pInputTypes: PGUID;
        pMedIn: PREGPINMEDIUM; pPinCategoryIn: PGUID; bRender, bOutputNeeded: BOOL;
        cOutputTypes: DWORD; pOutputTypes: PGUID; pMedOut: PRegPinMedium;
        pPinCategoryOut: PGUID): HResult; stdcall;
  end;

  function VGEnumMatchingFilters(out pList: IVGFilterList; dwFlags: DWORD; bExactMatch: BOOL;
        dwMerit: DWORD; bInputNeeded: BOOL; cInputTypes: DWORD; pInputTypes: PGUID;
        pMedIn: PREGPINMEDIUM; pPinCategoryIn: PGUID; bRender, bOutputNeeded: BOOL;
        cOutputTypes: DWORD; pOutputTypes: PGUID; pMedOut: PRegPinMedium;
        pPinCategoryOut: PGUID): HResult; stdcall; external 'VGLib.dll';

var
  gHooks: TJclPeMapImgHooks;
  gRealCoCreateInstance: function (const clsid: TCLSID; unkOuter: IUnknown;
    dwClsContext: Longint; const iid: TIID; out pv): HResult; stdcall;

implementation

function MyCoCreateInstance(const clsid: TCLSID; unkOuter: IUnknown;
  dwClsContext: Longint; const iid: TIID; out pv): HResult; stdcall;
begin
  if IsEqualIID(IID_IFilterMapper2, iid) then
  begin
    IFilterMapper2(pv) := TFakeFilterMapper2.Create;
    Result := S_OK;
  end
  else
    Result := gRealCoCreateInstance(clsid, unkOuter, dwClsContext, iid, pv);
end;

{ TVGFilterListAdapter }

function TVGFilterListAdapter.Clone(out enm: IEnumMoniker): HResult;
begin
  enm := TVGFilterListAdapter.Create(FList);
  enm.Skip(FPos);
  Result := S_OK;
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
var
  pelf: ^IBaseFilter;
  I: Integer;
begin
  pceltFetched^ := FList.GetCount - FPos;
  if pceltFetched^ > celt then
    pceltFetched^ := celt;
    
  pelf := @elt;
  for I := 0 to pceltFetched^ - 1 do
  begin
    FList.Get(FPos + Cardinal(I), pelf^);
    Inc(pelf);
    Inc(FPos);
  end;

  if celt = pceltFetched^ then
    Result := S_OK
  else
    Result := S_FALSE;
end;

function TVGFilterListAdapter.Reset: HResult;
begin
  FPos := 0;
  Result := S_OK;
end;

function TVGFilterListAdapter.Skip(celt: Integer): HResult;
begin
  if Int64(FList.GetCount - FPos) < Int64(celt) then
  begin
    FPos := FList.GetCount;
    Result := S_FALSE;
  end
  else
  begin
    Inc(FPos, celt);
    Result := S_OK; 
  end;
end;

{ CFakeFilterMapper2 }

constructor TFakeFilterMapper2.Create;
begin
  gRealCoCreateInstance(CLSID_FilterMapper2, nil, CLSCTX_INPROC_SERVER, IID_IFilterMapper2, FRealFM2);
end;

function TFakeFilterMapper2.CreateCategory(const clsidCategory: TGUID; dwCategoryMerit: DWORD; Description: PWideChar): HResult;
begin
  Result := FRealFM2.CreateCategory(clsidCategory, dwCategoryMerit, Description);
end;

function TFakeFilterMapper2.EnumMatchingFilters(out ppEnum: IEnumMoniker; dwFlags: DWORD; bExactMatch: BOOL; dwMerit: DWORD;
  bInputNeeded: BOOL; cInputTypes: DWORD; pInputTypes: PGUID; pMedIn: PREGPINMEDIUM; pPinCategoryIn: PGUID; bRender, bOutputNeeded: BOOL;
  cOutputTypes: DWORD; pOutputTypes: PGUID; pMedOut: PRegPinMedium; pPinCategoryOut: PGUID): HResult;
var
  pList: IVGFilterList;
begin
  Result := VGEnumMatchingFilters(pList, dwFlags, bExactMatch, dwMerit, bInputNeeded, cInputTypes, pInputTypes,
    pMedIn, pPinCategoryIn, bRender, bOutputNeeded, cOutputTypes, pOutputTypes, pMedOut, pPinCategoryOut);
  if Failed(Result) then
    Result := FRealFM2.EnumMatchingFilters(ppEnum, dwFlags, bExactMatch, dwMerit, bInputNeeded, cInputTypes, pInputTypes,
      pMedIn, pPinCategoryIn, bRender, bOutputNeeded, cOutputTypes, pOutputTypes, pMedOut, pPinCategoryOut)
  else
  begin
    ppEnum := TVGFilterListAdapter.Create(pList);
  end;
end;

function TFakeFilterMapper2.RegisterFilter(const clsidFilter: TGUID; Name: PWideChar; ppMoniker: IMoniker; pclsidCategory: PGUID;
  szInstance: PWideChar; const prf2: TRegFilter2): HResult;
begin
  Result := FRealFM2.RegisterFilter(clsidFilter, Name, ppMoniker, pclsidCategory, szInstance, prf2);
end;

function TFakeFilterMapper2.UnregisterFilter(const pclsidCategory: TGUID; szInstance: PWideChar; const Filter: TGUID): HResult;
begin
  Result := FRealFM2.UnregisterFilter(pclsidCategory, szInstance, Filter);
end;

initialization
  //CoInitialize(nil);
  //gHooks := TJclPeMapImgHooks.Create;
  //gHooks.HookImport(Pointer(HInstance), 'ole32.dll', 'CoCreateInstance', @MyCoCreateInstance, @gRealCoCreateInstance);

finalization
  //FreeAndNil(gHooks);

end.
