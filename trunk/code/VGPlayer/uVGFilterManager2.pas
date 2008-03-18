unit uVGFilterManager2;

{$DEFINE __ARIA_LIST_TEMPLATE__}

interface

uses
  Windows, Classes, Contnrs, SysUtils, ActiveX, DirectShow9, uVGLib, uVGFilterList;

type
  TVGFilterManager = class
  strict private
    FFM2: IFilterMapper2;
    FInternalFilters: TVGFilterList;
  private
    procedure RegisterFilter(const clsID: PCLSID; lpszName: PWideChar;
      dwMerit: Cardinal; nPins: Cardinal; const lpPin: PRegFilterPins);
    procedure RegisterSource(const clsID: PCLSID; lpszName: PWideChar;
      dwMerit: Cardinal; nPins: Cardinal; const lpPin: PRegFilterPins;
      lpszChkBytes: PWideChar; nExts: Cardinal; ppszExts: PPWideChar);
  public
    constructor Create;
    destructor Destroy; override;
    function FindMatchingSource(const AFile: WideString;
      out AFilter: IBaseFilter; AIgnoreExt: Boolean = False): Boolean;
    function FindMatchingFilters(out AList: IEnumMoniker; AMerit: DWORD;
      AInputNeeded: BOOL; AClsInMaj, AClsInSub: TCLSID; ARender, AOutputNeeded: BOOL;
      AClsOutMaj, AClsOutSub: TCLSID): Boolean;
  end;

var
  gFilterMan: TVGFilterManager;

implementation

uses uVGException, uAriaDebug;

procedure EnumFilterProc(pUser: Pointer; const clsID: PCLSID; lpszName: PWideChar;
  dwMerit: Cardinal; nPins: Cardinal; const lpPin: PRegFilterPins); stdcall;
var
  FM: TVGFilterManager; 
begin
  if pUser = nil then
    Exit;
  FM := TVGFilterManager(pUser);
  FM.RegisterFilter(clsID, lpszName, dwMerit, nPins, lpPin);
end;

procedure EnumSourceProc(pUser: Pointer; const clsID: PCLSID; lpszName: PWideChar;
  dwMerit: Cardinal; nPins: Cardinal; const lpPin: PRegFilterPins; lpszChkBytes: PWideChar;
  nExts: Cardinal; ppszExts: PPWideChar); stdcall;
var
  FM: TVGFilterManager; 
begin
  if pUser = nil then
    Exit;
  FM := TVGFilterManager(pUser);
  FM.RegisterSource(clsID, lpszName, dwMerit, nPins, lpPin, lpszChkBytes,
    nExts, ppszExts);
end;

function MatchGUID(const AGUID1, AGUID2: TGUID): Boolean; inline;
begin
  if IsEqualGUID(AGUID1, AGUID2) or
    IsEqualGUID(AGUID1, GUID_NULL) or
    IsEqualGUID(AGUID2, GUID_NULL) then
    Result := True
  else
    Result := False;
end;

{ TVGFilterManager }

constructor TVGFilterManager.Create;
var
  hr: HRESULT;
begin
  FInternalFilters := TVGFilterList.Create(True);

  hr := CoCreateInstance(CLSID_FilterMapper2, nil, CLSCTX_INPROC_SERVER,
    IID_IFilterMapper2, FFM2);
  if Failed(hr) then
    raise EVGError.CreateFromLastOSError(hr);
  // 初始化内部滤镜列表
  VGEnumInternalFilters(EnumFilterProc, Self);
  VGEnumInternalSources(EnumSourceProc, Self);
end;

destructor TVGFilterManager.Destroy;
begin
  FFM2 := nil;
  FreeAndNil(FInternalFilters);
  inherited;
end;

function TVGFilterManager.FindMatchingFilters(out AList: IEnumMoniker;
  AMerit: DWORD; AInputNeeded: BOOL; AClsInMaj, AClsInSub: TCLSID; ARender,
  AOutputNeeded: BOOL; AClsOutMaj, AClsOutSub: TCLSID): Boolean;
var
  NewList: TVGFilterList;
  I, J, K: Integer;
  Filter: TVGFilter;
  Pin: TVGPin;
  MT: TVGMediaType;
  bInMatched, bOutMatched: Boolean;
begin
  NewList := TVGFilterList.Create(False);
  for I := 0 to FInternalFilters.Count - 1 do
  begin
    Filter := FInternalFilters[I];
    bInMatched := False;
    bOutMatched := False;
    // 检查Merit
    if Filter.Merit < AMerit then
      Continue;
    // 检查In、Out插针个数
    if (AInputNeeded and (Filter.InPins = 0)) or
      (AOutputNeeded and (Filter.OutPins = 0)) then
      Continue;
    // 检查每一个Pin
    for J := 0 to Filter.PinCount - 1 do
    begin
      Pin := Filter.Pins[J];
      if Pin.Output then
      begin
        // 检查每一个MediaType
        for K := 0 to Pin.MediaTypeCount - 1 do
        begin
          MT := Pin.MediaTypes[K];
          bOutMatched := MatchGUID(MT.clsMajorType, AClsOutMaj) and
            MatchGUID(MT.clsMinorType, AClsOutSub);
          if bOutMatched then
            Break;
        end;
        if not bOutMatched then
          Break;
      end
      else
      begin
        // 检查每一个MediaType
        for K := 0 to Pin.MediaTypeCount - 1 do
        begin
          MT := Pin.MediaTypes[K];
          bInMatched := MatchGUID(MT.clsMajorType, AClsInMaj) and
            MatchGUID(MT.clsMinorType, AClsInSub);
          if bInMatched then
            Break;
        end;
        if not bInMatched then
          Break;
      end;
    end;
  end;
end;

function TVGFilterManager.FindMatchingSource(const AFile: WideString;
  out AFilter: IBaseFilter; AIgnoreExt: Boolean): Boolean;
var
  I: Integer;
  Filter: TVGSource;
begin
  for I := 0 to FInternalFilters.Count - 1 do
  begin
    if not (FInternalFilters[I] is TVGSource) then
      Continue;
    Filter := FInternalFilters[I] as TVGSource;
    if Filter.QueryAccept(AFile, AIgnoreExt) then
    begin
      AFilter := Filter.CreateInstance;
      Result := True;
      Exit;
    end;
  end;
  // 内置Source无法解析文件，使用默认系统滤镜
  Result := Succeeded(CoCreateInstance(CLSID_AsyncReader, nil, CLSCTX_INPROC_SERVER,
    IID_IBaseFilter, AFilter));
end;

procedure TVGFilterManager.RegisterFilter(const clsID: PCLSID;
  lpszName: PWideChar; dwMerit, nPins: Cardinal; const lpPin: PRegFilterPins);
begin
  FInternalFilters.Add(TVGFilter.Create(clsID, lpszName, dwMerit, nPins, lpPin));
end;

procedure TVGFilterManager.RegisterSource(const clsID: PCLSID;
  lpszName: PWideChar; dwMerit, nPins: Cardinal; const lpPin: PRegFilterPins;
  lpszChkBytes: PWideChar; nExts: Cardinal; ppszExts: PPWideChar);
begin
  FInternalFilters.Add(TVGSource.Create(clsID, lpszName, dwMerit, nPins, lpPin,
    lpszChkBytes, nExts, ppszExts));
end;

initialization
  CoInitialize(nil);
  gFilterMan := TVGFilterManager.Create;

finalization
  FreeAndNil(gFilterMan);

end.
