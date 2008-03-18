unit uVGFilterManager2;

{$DEFINE __ARIA_LIST_TEMPLATE__}

interface

uses
  Windows, Classes, Contnrs, SysUtils, ActiveX, DirectShow9, uVGLib;

type
  TVGMediaType = record
    clsMajorType: TGUID;
    clsMinorType: TGUID;
  end;

  TVGPin = class
  private
    FName         : WideString;
    FRendered     : Boolean;
    FOutput       : Boolean;
    FZero         : Boolean;
    FMany         : Boolean;
    FMediaTypes   : array of TVGMediaType;
  public
    constructor Create(const lpPin: PRegFilterPins);
    destructor Destroy; override;
  end;

  TVGFilter = class
  private
    FCLSID        : TCLSID;
    FName         : WideString;
    FMerit        : Cardinal;
    FPins         : array of TVGPin;
  public
    constructor Create(const clsID: PCLSID; lpszName: PWideChar;
      dwMerit: Cardinal; nPins: Cardinal; const lpPin: PRegFilterPins);
    destructor Destroy; override;
  end;

  TVGSource = class(TVGFilter)
  private
    FChkBytes     : WideString;
    FExtensions   : array of WideString;
  public
    constructor Create(const clsID: PCLSID; lpszName: PWideChar;
      dwMerit: Cardinal; nPins: Cardinal; const lpPin: PRegFilterPins;
      lpszChkBytes: PWideChar; nExts: Cardinal; ppszExts: PPWideChar);
    destructor Destroy; override;
  end;

  _ITEM_TYPE_ = TVGFilter;
  {$INCLUDE tAriaList.tpl}

  TVGFilterList = class(_ARIA_LIST_TEMPLATE_);

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

{$INCLUDE tAriaList.tpl}

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
  Result := False;
  for I := 0 to FInternalFilters.Count - 1 do
  begin
    if not (FInternalFilters[I] is TVGSource) then
      Continue;
    Filter := FInternalFilters[I] as TVGSource;
    
  end;
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

{ TVGPin }

constructor TVGPin.Create(const lpPin: PRegFilterPins);
var
  I: Integer;
  pPtr: PRegPinTypes;
begin
  FName := lpPin^.strName;
  FRendered := lpPin^.bRendered;
  FOutput := lpPin^.bOutput;
  FZero := lpPin^.bZero;
  FMany := lpPin^.bMany;
  SetLength(FMediaTypes, lpPin^.nMediaTypes);
  pPtr := lpPin^.lpMediaType;
  for I := 0 to lpPin^.nMediaTypes - 1 do
  begin
    FMediaTypes[I].clsMajorType := pPtr^.clsMajorType^;
    FMediaTypes[I].clsMinorType := pPtr^.clsMinorType^;
    Inc(pPtr);
  end;
end;

destructor TVGPin.Destroy;
begin
  SetLength(FMediaTypes, 0);
  inherited;
end;

{ TVGFilter }

constructor TVGFilter.Create(const clsID: PCLSID; lpszName: PWideChar; dwMerit,
  nPins: Cardinal; const lpPin: PRegFilterPins);
var
  I: Integer;
  pPtr: PRegFilterPins;
begin
  FCLSID := clsID^;
  FName := lpszName;
  FMerit := dwMerit;
  SetLength(FPins, nPins);
  pPtr := lpPin;
  for I := 0 to nPins - 1 do
  begin
    FPins[I] := TVGPin.Create(pPtr);
    Inc(pPtr);
  end;
  Trace('Register filter "%s" successfully!', [FName]);
end;

destructor TVGFilter.Destroy;
var
  I: Integer;
begin
  for I := Low(FPins) to High(FPins) do
    FreeAndNil(FPins[I]);
  SetLength(FPins, 0);
  inherited;
end;

{ TVGSource }

constructor TVGSource.Create(const clsID: PCLSID; lpszName: PWideChar; dwMerit,
  nPins: Cardinal; const lpPin: PRegFilterPins; lpszChkBytes: PWideChar;
  nExts: Cardinal; ppszExts: PPWideChar);
var
  I: Integer;
  pPtr: PPWideChar;
begin
  inherited Create(clsID, lpszName, dwMerit, nPins, lpPin);
  FChkBytes := lpszChkBytes;
  SetLength(FExtensions, nExts);
  pPtr := ppszExts;
  for I := 0 to nExts - 1 do
  begin
    FExtensions[I] := pPtr^;
    Inc(pPtr);
  end;
end;

destructor TVGSource.Destroy;
begin
  SetLength(FExtensions, 0);
  inherited;
end;

initialization
  CoInitialize(nil);
  gFilterMan := TVGFilterManager.Create;

finalization
  FreeAndNil(gFilterMan);

end.
