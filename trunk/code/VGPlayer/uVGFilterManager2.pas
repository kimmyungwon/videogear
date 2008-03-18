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
