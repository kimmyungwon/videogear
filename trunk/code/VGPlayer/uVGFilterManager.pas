unit uVGFilterManager;

interface

uses
  Windows, ActiveX, WideStrings, DirectShow9,
  uAriaDebug,
  uVGBase, uVGLib, uVGFilterList, uVGException, uVGMTLookup;

type
  TVGFilterManager = class
  protected
    FFM2: IFilterMapper2;
    FGB: IGraphBuilder;
    FInternalFilters: TVGFilterList;
    FInternalFiltersLookupByMT: TVGFilterLookupByMediaType;
    FAudioSwitcher: IBaseFilter;
    FAudioSwitcherCtrl: IAudioSwitcherFilter;
  private
    procedure RegisterFilter(const clsID: PCLSID; lpszName: PWideChar;
      dwMerit: Cardinal; nPins: Cardinal; const lpPin: PRegFilterPins);
    procedure RegisterSource(const clsID: PCLSID; lpszName: PWideChar;
      dwMerit: Cardinal; nPins: Cardinal; const lpPin: PRegFilterPins;
      lpszChkBytes: PWideChar; nExts: Cardinal; ppszExts: PPWideChar);
  protected
    function ConnectDirect(AOutPin: IPin; AFilter: IBaseFilter; AMT: PAMMediaType): HRESULT;
    procedure DisconnectFilters;
    function GetPinDir(APin: IPin): PIN_DIRECTION;
    function GetPinName(APin: IPin): WideString;
    function IsPinConnected(APin: IPin): Boolean;
    function RenderFilter(AFilter: IBaseFilter): HRESULT;
    function RenderPin(APin: IPin): HRESULT;
    function RenderPinUsingExists(APIn: IPin; AMT: PAMMediaType): HRESULT;
    function RenderPinUsingInternal(APIn: IPin; AMT: PAMMediaType): HRESULT;
    function RenderPinUsingSystem(APIn: IPin): HRESULT;
  public
    constructor Create;
    destructor Destroy; override;
    procedure Clear;
    function GetFilterList: TWideStringList;
    function FindMatchingSource(const AFile: WideString;
      out AFilter: IBaseFilter; AIgnoreExt: Boolean = False): Boolean;
    function FindMatchingFilters(out AList: TVGFilterList; AMerit: DWORD;
      AInputNeeded: BOOL; AClsInMaj, AClsInSub: TCLSID; ARender, AOutputNeeded: BOOL;
      AClsOutMaj, AClsOutSub: TCLSID): Boolean; overload;
    function FindMatchingFilters(out AList: TVGFilterList; AMerit: DWORD;
      AClsInMaj, AClsInSub: TCLSID): Boolean; overload;
    function Get(const clsID: TCLSID): TVGFilter;
    function RenderFile(const AFileName: WideString): HRESULT; virtual;
  end;

implementation

uses DSUtil, JclSysUtils;

function MatchGUID(const AGUID1, AGUID2: TGUID): Boolean; inline;
begin
  if IsEqualGUID(AGUID1, AGUID2) or
    IsEqualGUID(AGUID1, GUID_NULL) or
    IsEqualGUID(AGUID2, GUID_NULL) then
    Result := True
  else
    Result := False;
end;

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

procedure TVGFilterManager.Clear;
var
  I: integer;
  FilterList: TFilterList;
  Filter: TVGFilter;
begin
  if Assigned(FGB) then
  begin
    DisconnectFilters;
    FilterList:= TFilterList.Create(FGB);

    if FilterList.count > 0 then
    for i := 0 to FilterList.Count - 1 do
      CheckDSError(FGB.RemoveFilter(FilterList.Items[i]));
    FilterList.Free;
  end;
  // 启用AudioSwitcher
  Filter := Get(IID_IAudioSwitcher);
  FAudioSwitcher := Filter.CreateInstance;
  FGB.AddFilter(FAudioSwitcher, PWideChar(Filter.Name));
  FAudioSwitcherCtrl := FAudioSwitcher as IAudioSwitcherFilter;
  FAudioSwitcherCtrl.SetNormalizeBoost(True, True, 0);
end;

function TVGFilterManager.ConnectDirect(AOutPin: IPin; AFilter: IBaseFilter; AMT: PAMMediaType): HRESULT;
var
  pEnumPin: IEnumPins;
  pPinIn: IPin;
begin
  AFilter.EnumPins(pEnumPin);
  while pEnumPin.Next(1, pPinIn, nil) = S_OK do
  begin
    if IsPinConnected(pPinIn) or (GetPinDir(pPinIn) = PINDIR_OUTPUT) then
      Continue;
    
    Result := FGB.ConnectDirect(AOutPin, pPinIn, AMT);
    if Succeeded(Result) then
      Exit;
  end;
  Result := E_FAIL;
end;

constructor TVGFilterManager.Create;
var
  hr: HRESULT;
begin
  FInternalFilters := TVGFilterList.Create(True);
  FInternalFiltersLookupByMT := TVGFilterLookupByMediaType.Create;

  hr := CoCreateInstance(CLSID_FilterMapper2, nil, CLSCTX_INPROC_SERVER,
    IID_IFilterMapper2, FFM2);
  if Failed(hr) then
    raise EVGError.CreateOSError(hr);
  hr := CoCreateInstance(CLSID_FilterGraph, nil, CLSCTX_INPROC_SERVER,
    IID_IGraphBuilder, FGB);
  if Failed(hr) then
    raise EVGError.CreateOSError(hr);
  // 初始化内部滤镜列表
  VGEnumInternalFilters(EnumFilterProc, Self);
  VGEnumInternalSources(EnumSourceProc, Self);
end;

destructor TVGFilterManager.Destroy;
begin
  FFM2 := nil;
  FreeAndNil(FInternalFiltersLookupByMT);
  FreeAndNil(FInternalFilters);
  inherited;
end;

procedure TVGFilterManager.DisconnectFilters;
var
  FilterList: TFilterList;
  PinList: TPinList;
  BaseFilter: IBaseFilter;
  i, j: integer;
begin
  if Assigned(FGB) then
  begin
    FilterList:= TFilterList.Create(FGB);
    if FilterList.Count > 0 then
      for i := 0 to FilterList.Count - 1 do
        begin
          BaseFilter := FilterList.Items[i] as IBaseFilter;
          PinList := TPinList.Create(BaseFilter);
          if PinList.Count > 0 then
          for j := 0 to PinList.Count - 1 do
            CheckDSError(IPin(PinList.Items[j]).Disconnect);
          PinList.Free;
          BaseFilter := nil;
        end;
    FilterList.Free;
  end;
end;

function TVGFilterManager.FindMatchingFilters(out AList: TVGFilterList;
  AMerit: DWORD; AInputNeeded: BOOL; AClsInMaj, AClsInSub: TCLSID; ARender,
  AOutputNeeded: BOOL; AClsOutMaj, AClsOutSub: TCLSID): Boolean;
var
  I, J, K: Integer;
  Filter: TVGFilter;
  Pin: TVGPin;
  MT: TVGMediaType;
  bInMatched, bOutMatched: Boolean;
begin
  AList.Clear;
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
    // 判断结果
    if ((Filter.InPins > 0) and (not bInMatched)) or
      ((Filter.OutPins > 0) and (not bOutMatched)) then
      Continue;
    AList.Add(Filter);
  end;
  Result := AList.Count > 0;
end;

function TVGFilterManager.FindMatchingFilters(out AList: TVGFilterList;
  AMerit: DWORD; AClsInMaj, AClsInSub: TCLSID): Boolean;
var
  mtIn: TVGMediaType;
begin
  mtIn := TVGMediaType.Create(AClsInMaj, AClsInSub);
  
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

function TVGFilterManager.Get(const clsID: TCLSID): TVGFilter;
var
  I: Integer;
begin
  Result := nil;
  for I := 0 to FInternalFilters.Count - 1 do
  begin
    if IsEqualCLSID(clsID, FInternalFilters[I].CLSID) then
    begin
      Result := FInternalFilters[I];
      Exit;
    end;
  end;
end;

function TVGFilterManager.GetFilterList: TWideStringList;
var
  pEnum: IEnumFilters;
  pBF: IBaseFilter;
  Info: TFilterInfo;
begin
  Result := nil;
  if (FGB = nil) then
    Exit;
  if Failed(FGB.EnumFilters(pEnum)) then
    Exit;

  Result := TWideStringList.Create;
  while pEnum.Next(1, pBF, nil) = S_OK do
  begin
    pBF.QueryFilterInfo(Info);
    Result.Add(Info.achName);
  end;
end;

function TVGFilterManager.GetPinDir(APin: IPin): PIN_DIRECTION;
begin
  APin.QueryDirection(Result);
end;

function TVGFilterManager.GetPinName(APin: IPin): WideString;
var
  pi: TPinInfo;
begin
  APin.QueryPinInfo(pi);
  Result := pi.achName;
end;

function TVGFilterManager.IsPinConnected(APin: IPin): Boolean;
var
  pmt: TAMMediaType;
begin
  Result := Succeeded(APin.ConnectionMediaType(pmt));
end;

procedure TVGFilterManager.RegisterFilter(const clsID: PCLSID;
  lpszName: PWideChar; dwMerit, nPins: Cardinal; const lpPin: PRegFilterPins);
var
  newFilter: TVGFilter;
  I, J: Integer;
begin
  newFilter := TVGFilter.Create(clsID, lpszName, dwMerit, nPins, lpPin);
  FInternalFilters.Add(newFilter);
  // 注册输入Pin的MediaType到Lookup
  for I := 0 to newFilter.PinCount - 1 do
  begin
    if newFilter.Pins[I].Output then
      Continue;
    for J := 0 to newFilter.Pins[I].MediaTypeCount - 1 do
    begin
      //FInternalFiltersLookupByMT[newFilter.Pins[I].MediaTypes[J]] := newFilter;
    end;
  end;
end;

procedure TVGFilterManager.RegisterSource(const clsID: PCLSID;
  lpszName: PWideChar; dwMerit, nPins: Cardinal; const lpPin: PRegFilterPins;
  lpszChkBytes: PWideChar; nExts: Cardinal; ppszExts: PPWideChar);
begin
  FInternalFilters.Add(TVGSource.Create(clsID, lpszName, dwMerit, nPins, lpPin,
    lpszChkBytes, nExts, ppszExts));
end;

function TVGFilterManager.RenderFile(const AFileName: WideString): HRESULT;
var
  pSource: IBaseFilter;
begin
  Clear;
  // 优先使用内部源滤镜
  if not FindMatchingSource(AFileName, pSource) then
  begin
    Result := VFW_E_CANNOT_RENDER;
    Exit;
  end;
  Result := (pSource as IFileSourceFilter).Load(PWideChar(AFileName), nil);
  if Failed(Result) then
    Exit;
  Result := FGB.AddFilter(pSource, PWideChar(AFileName));
  if Failed(Result) then
    Exit;
  Result := RenderFilter(pSource);
end;

function TVGFilterManager.RenderFilter(AFilter: IBaseFilter): HRESULT;
var
  pEnumPin: IEnumPins;
  pPinOut: IPin;
  nRendered: Integer;
begin
  nRendered := 0;

  AFilter.EnumPins(pEnumPin);
  while pEnumPin.Next(1, pPinOut, nil) = S_OK do
  begin
    if IsPinConnected(pPinOut) or (GetPinDir(pPinOut) = PINDIR_INPUT) then
      Continue;
    Result := RenderPin(pPinOut);
    if Succeeded(Result) then
      Inc(nRendered);
  end;

  if nRendered > 0 then
    Result := S_OK
  else
    Result := VFW_E_CANNOT_RENDER;
end;

function TVGFilterManager.RenderPin(APin: IPin): HRESULT;
var
  pEnumMT: IEnumMediaTypes;
  pmt: PAMMediaType;
begin
  Trace('开始渲染"%s"...', [GetPinName(APin)]);
  APin.EnumMediaTypes(pEnumMT);
  while pEnumMT.Next(1, pmt, nil) = S_OK do
  begin
    Trace('尝试使用MediaType："%s"...', [GetMediaTypeDescription(pmt)]);
    Trace('尝试使用FilterGraph中已有的滤镜...');
    if Succeeded(RenderPinUsingExists(APin, pmt)) then
    begin
      Trace('"%s"被成功渲染', [GetPinName(APin)]);
      DeleteMediaType(pmt);
      Result := S_OK;
      Exit;
    end;
    // 尝试内部滤镜
    Trace('尝试使用内部滤镜...');
    if Succeeded(RenderPinUsingInternal(APin, pmt)) then
    begin
      Trace('"%s"被成功渲染', [GetPinName(APin)]);
      DeleteMediaType(pmt);
      Result := S_OK;
      Exit;
    end;
    DeleteMediaType(pmt);
  end; // end while
  // 内置滤镜尝试失败，使用系统滤镜
  Trace('尝试使用系统滤镜...', [GetPinName(APin)]);
  if Succeeded(RenderPinUsingSystem(APin)) then
  begin
    Trace('"%s"被成功渲染', [GetPinName(APin)]);
    Result := S_OK;
    Exit;
  end;
  // 所有尝试失败，无法渲染
  Result := VFW_E_CANNOT_RENDER;
end;

function TVGFilterManager.RenderPinUsingExists(APIn: IPin;
  AMT: PAMMediaType): HRESULT;
var
  pEnum: IEnumFilters;
  pBF: IBaseFilter;
begin
  if Succeeded(FGB.EnumFilters(pEnum)) then
  begin
    while pEnum.Next(1, pBF, nil) = S_OK do
    begin
      if Succeeded(ConnectDirect(APin, pBF, AMT)) then
      begin
        Result := RenderFilter(pBF);
        if Succeeded(Result) then
        begin
          Result := S_OK;
          Exit;
        end;
      end;
    end;
  end;
  Result := VFW_E_CANNOT_RENDER;
end;

function TVGFilterManager.RenderPinUsingInternal(APIn: IPin;
  AMT: PAMMediaType): HRESULT;
var
  pMatched: TVGFilter;
  pBF: IBaseFilter;
  lstMatched: TVGFilterList;
  I: Integer;
begin
  lstMatched := TVGFilterList.Create(False);
  try
    if not FindMatchingFilters(lstMatched, MERIT_NORMAL, True,
      AMT^.MajorType, AMT^.SubType, False, False, GUID_NULL, GUID_NULL) then
    begin
      Result := VFW_E_CANNOT_RENDER;
      Exit;
    end;
    
    for I := 0 to lstMatched.Count - 1 do
    begin
      pMatched := lstMatched[I];
      pBF := pMatched.CreateInstance;
      if pBF = nil then
        Continue;
      FGB.AddFilter(pBF, PWideChar(pMatched.Name));
      if Succeeded(ConnectDirect(APin, pBF, AMT)) then
      begin
        Result := RenderFilter(pBF);
        if Succeeded(Result) then
        begin
          Result := S_OK;
          Exit;
        end;
      end;
    end;

    Result := VFW_E_CANNOT_RENDER;
  finally
    lstMatched.Free;
  end;
end;

function TVGFilterManager.RenderPinUsingSystem(APIn: IPin): HRESULT;
begin
  Result := FGB.Render(APin);
end;

end.
