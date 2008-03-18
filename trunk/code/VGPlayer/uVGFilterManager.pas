unit uVGFilterManager;

interface

uses
  Windows, ActiveX, DirectShow9, uVGLib, uAriaDebug, WideStrings;

type
  TVGFilterManager = class
  protected
    FGB: IGraphBuilder;
    FAudioSwitcher: IBaseFilter;
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
    procedure Clear;
    function GetFilterList: TWideStringList;
    function RenderFile(const AFileName: WideString): HRESULT; virtual; 
  end;

implementation

uses DSUtil, uVGFilterManager2, uVGFilterList, JclSysUtils;

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
  Filter := gFilterMan.Get(IID_IAudioSwitcher);
  FGB.AddFilter(Filter.CreateInstance, PWideChar(Filter.Name));
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
begin
  CoCreateInstance(CLSID_FilterGraph, nil, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, FGB);
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

function TVGFilterManager.RenderFile(const AFileName: WideString): HRESULT;
var
  pSource: IBaseFilter;
begin
  Clear;
  // 优先使用内部源滤镜
  if not gFilterMan.FindMatchingSource(AFileName, pSource) then
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
    if not gFilterMan.FindMatchingFilters(lstMatched, MERIT_NORMAL, True,
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
