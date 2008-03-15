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

  function VGEnumMatchingFilters(out pList: IVGFilterList; dwMerit: DWORD; bInputNeeded: BOOL;
    clsInMaj, clsInSub: TCLSID; bRender, bOutputNeeded: BOOL;
    clsOutMaj, clsOutSub: TCLSID): HResult; stdcall; external 'VGLib.dll';

implementation

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

end.
