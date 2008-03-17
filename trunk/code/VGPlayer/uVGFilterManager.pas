unit uVGFilterManager;

interface

uses
  Windows, ActiveX, DirectShow9, uVGLib, uAriaDebug, WideStrings;

type
  TVGFilterManager = class
  protected
    FGB: IGraphBuilder;
  protected
    function ConnectDirect(AOutPin: IPin; AFilter: IBaseFilter; AMT: PAMMediaType): HRESULT;
    procedure DisconnectFilters;
    function GetPinDir(APin: IPin): PIN_DIRECTION;
    function GetPinName(APin: IPin): WideString;
    function IsPinConnected(APin: IPin): Boolean;
    function RenderFilter(AFilter: IBaseFilter): HRESULT;
  public
    constructor Create;
    procedure Clear;
    function GetFilterList: TWideStringList;
    function RenderFile(const AFileName: WideString): HRESULT; virtual; 
  end;

implementation

uses DSUtil;

{ TVGFilterManager }

procedure TVGFilterManager.Clear;
var
  I: integer;
  FilterList: TFilterList;
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
end;

function TVGFilterManager.ConnectDirect(AOutPin: IPin; AFilter: IBaseFilter; AMT: PAMMediaType): HRESULT;
var
  pEnumPin: IEnumPins;
  pPinIn: IPin;
begin
  AFilter.EnumPins(pEnumPin);
  while pEnumPin.Next(1, pPinIn, nil) = S_OK do
  begin
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
  Result := VGEnumMatchingSource(PWideChar(AFileName), pSource);
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
  pEnumMT: IEnumMediaTypes;
  pPinOut: IPin;
  pmt: PAMMediaType;
  pMatched: IBaseFilter;
  lstMatched: IVGFilterList;
  pMatchedName: PWideChar;
  K, nRendered: Integer;
  bRendered: Boolean;
begin
  nRendered := 0;

  AFilter.EnumPins(pEnumPin);
  while pEnumPin.Next(1, pPinOut, nil) = S_OK do
  begin
    if IsPinConnected(pPinOut) or (GetPinDir(pPinOut) = PINDIR_INPUT) then
      Continue;

    bRendered := False;
    Trace('Trying to render "%s"...', [GetPinName(pPinOut)]);
    pPinOut.EnumMediaTypes(pEnumMT);
    while pEnumMT.Next(1, pmt, nil) = S_OK do
    begin
      Trace('Trying MediaType "%s"...', [GetMediaTypeDescription(pmt)]);
      if Failed(VGEnumMatchingFilters(lstMatched, MERIT_DO_NOT_USE, True, pmt^.MajorType, pmt^.SubType,
        False, False, GUID_NULL, GUID_NULL)) then
      begin
        DeleteMediaType(pmt);
        Continue;
      end;

      for K := 0 to lstMatched.GetCount - 1 do
      begin
        lstMatched.Get(K, pMatched, pMatchedName);
        FGB.AddFilter(pMatched, pMatchedName);
        if Succeeded(ConnectDirect(pPinOut, pMatched, pmt)) then
        begin
          Result := RenderFilter(pMatched);
          if Succeeded(Result) then
            bRendered := True;
          DeleteMediaType(pmt);
          Break;
        end
        else
        begin
          FGB.RemoveFilter(pMatched);
          DeleteMediaType(pmt);
          Continue;
        end;
      end;

      if bRendered then
        Break;
    end; // end while
    // 内置滤镜尝试失败，使用系统滤镜
    if not bRendered then
    begin
      Trace('Internal filter can''t render "%s", try system filters...', [GetPinName(pPinOut)]);
      Result := FGB.Render(pPinOut);
      if Succeeded(Result) then
        bRendered := True
      else
        Trace('"%s" can''t be rendered!', [GetPinName(pPinOut)]);
    end;

    if bRendered then
      Inc(nRendered);
  end;

  if nRendered > 0 then
    Result := S_OK
  else
    Result := VFW_E_CANNOT_RENDER;
end;

end.
