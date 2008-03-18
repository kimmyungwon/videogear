unit uVGFilterList;

{$WARN SYMBOL_PLATFORM OFF}

interface

uses
  Windows, Classes, SysUtils, Contnrs, ActiveX, WideStrings, 
  DirectShow9, uVGFilterManager;

type
  TVGMediaType = record
    clsMajorType: TGUID;
    clsMinorType: TGUID;
  end;

  TVGPin = class(TPersistent)
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
    function GetMediaTypeCount: Integer;
    function GetMediaType(AIndex: Integer): TVGMediaType;
  public
    property Output: Boolean read FOutput;
    property MediaTypeCount: Integer read GetMediaTypeCount;
    property MediaTypes[AIndex: Integer]: TVGMediaType read GetMediaType;
  end;

  TVGFilter = class(TPersistent)
  private
    FCLSID        : TCLSID;
    FName         : WideString;
    FMerit        : Cardinal;
    FInPins, FOutPins: Cardinal;
    FPins         : array of TVGPin;
  public
    constructor Create(const clsID: PCLSID; lpszName: PWideChar;
      dwMerit: Cardinal; nPins: Cardinal; const lpPin: PRegFilterPins);
    destructor Destroy; override;
    function CreateInstance: IBaseFilter;
    function GetPinCount: Integer;
    function GetPin(AIndex: Integer): TVGPin;
  public
    property Name: WideString read FName;
    property Merit: Cardinal read FMerit;
    property InPins: Cardinal read FInPins;
    property OutPins: Cardinal read FOutPins;
    property PinCount: Integer read GetPinCount;
    property Pins[AIndex: Integer]: TVGPin read GetPin;
  end;

  TVGSource = class(TVGFilter)
  private
    FChkBytesStart, FChkBytesCount: Integer;
    FChkBytes     : WideString;
    FExtensions   : array of WideString;
  public
    constructor Create(const clsID: PCLSID; lpszName: PWideChar;
      dwMerit: Cardinal; nPins: Cardinal; const lpPin: PRegFilterPins;
      lpszChkBytes: PWideChar; nExts: Cardinal; ppszExts: PPWideChar);
    destructor Destroy; override;
    function QueryAccept(const AFile: WideString; AIgnoreExt: Boolean = False): Boolean;
  end;

  TVGFilterList = class(TInterfacedObject, IEnumMoniker)
  private
    FItems: TObjectList;
    FPos: Integer;
    function GetCount: Integer;
  public
    constructor Create(AOwnsObjects: Boolean = True);
    destructor Destroy; override;
    function Add(AFilter: TVGFilter): Integer;
    function Get(AIndex: Integer): TVGFilter;
    { IEnumMoniker }
    function Next(celt: Longint; out elt; pceltFetched: PLongint): HResult; stdcall;
    function Skip(celt: Longint): HResult; stdcall;
    function Reset: HResult; stdcall;
    function Clone(out enm: IEnumMoniker): HResult; stdcall;
  public
    property Count: Integer read GetCount;
    property Items[AIndex: Integer]: TVGFilter read Get; default;
  end;

implementation

uses uAriaDebug, TntClasses, TntSysUtils, uVGLib, uVGException, RTLConsts;

function CompareFilter(AFilter1, AFilter2: TVGFilter): Integer; inline;
begin
  Result := AFilter1.Merit - AFilter2.Merit;
end;

function BytesToStr(ABytes: array of Byte): WideString;
var
  I: Integer;
begin
  Result := '';
  for I := Low(ABytes) to High(ABytes) do
    Result := Result + WideFormat('%.2X', [ABytes[I]]); 
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

function TVGPin.GetMediaType(AIndex: Integer): TVGMediaType;
begin
  if (AIndex < Low(FMediaTypes)) or (AIndex > High(FMediaTypes)) then
    raise EVGError.CreateResFmt(@SListIndexError, [AIndex]);
  Result := FMediaTypes[Low(FMediaTypes) + AIndex];
end;

function TVGPin.GetMediaTypeCount: Integer;
begin
  Result := Length(FMediaTypes);
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
  FInPins := 0;
  FOutPins := 0;
  SetLength(FPins, nPins);
  pPtr := lpPin;
  for I := 0 to nPins - 1 do
  begin
    FPins[I] := TVGPin.Create(pPtr);
    if FPins[I].Output then
      Inc(FOutPins)
    else
      Inc(FInPins);
    Inc(pPtr);
  end;
  Trace('Register filter successfully: "%s", Merit:%.8X', [FName, FMerit]);
end;

function TVGFilter.CreateInstance: IBaseFilter;
begin
  if Failed(VGCreateInstance(@FCLSID, Result)) then
    Result := nil;
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

function TVGFilter.GetPin(AIndex: Integer): TVGPin;
begin
  Result := nil;
  if (AIndex < Low(FPins)) or (AIndex > High(FPins)) then
    Exit;
  Result := FPins[Low(FPins) + AIndex];
end;

function TVGFilter.GetPinCount: Integer;
begin
  Result := Length(FPins);
end;

{ TVGSource }

constructor TVGSource.Create(const clsID: PCLSID; lpszName: PWideChar; dwMerit,
  nPins: Cardinal; const lpPin: PRegFilterPins; lpszChkBytes: PWideChar;
  nExts: Cardinal; ppszExts: PPWideChar);
var
  I: Integer;
  lst: TWideStringList;
  pPtr: PPWideChar;
begin
  inherited Create(clsID, lpszName, dwMerit, nPins, lpPin);
  // 解析ChkBytes
  FChkBytes := lpszChkBytes;
  if FChkBytes <> '' then
  begin
    lst := TWideStringList.Create;
    try
      lst.Delimiter := ',';
      lst.DelimitedText := FChkBytes;
      if lst.Count = 4 then
      begin
        FChkBytesStart := StrToIntDef(lst[0], 0);
        FChkBytesCount := StrToIntDef(lst[1], 0);
        FChkBytes := lst[3];
      end;
    finally
      lst.Free;
    end;
  end;
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

function TVGSource.QueryAccept(const AFile: WideString; AIgnoreExt: Boolean): Boolean;
var
  I: Integer;
  strExt: WideString;
  strm: TTntFileStream;
  bytes: array of Byte;
begin
  Result := False;

  Trace('Query if "%s" accept "%s"', [FName, AFile]);
  strExt := WideExtractFileExt(AFile);
  if (not AIgnoreExt) and (strExt <> '') then
  begin
    for I := Low(FExtensions) to High(FExtensions) do
    begin
      if WideSameText(FExtensions[I], strExt) then
      begin
        Result := True;
        Trace('"%s" accept "%s" by extension', [FName, AFile]);
        Break;
      end;
    end;
  end;
  // 如果没有指定AIgnoreExt则当扩展名检查失败时返回失败
  if (not AIgnoreExt) and (not Result) then
  begin
    Trace('"%s" don''t accept "%s" because of extension', [FName, AFile]);
    Exit;
  end;
  // 匹配文件头
  if FChkBytesCount > 0 then
  begin
    try
      try
        strm := TTntFileStream.Create(AFile, fmOpenRead or fmShareCompat);
        SetLength(bytes, FChkBytesCount);
        if strm.Read(bytes[0], FChkBytesCount) <> FChkBytesCount then
          Exit;
        if not WideSameText(BytesToStr(bytes), FChkBytes) then
          Exit;
        Trace('"%s" accept "%s"', [FName, AFile]);
        Result := True;
      except
        Exit;
      end;
    finally
      if not Result then
        Trace('"%s" don''t accept "%s" because of chkbytes', [FName, AFile]);
      if strm <> nil then
        FreeAndNil(strm);
    end;
  end
  else
    Result := True;
end;

{ TVGFilterList }

function TVGFilterList.Add(AFilter: TVGFilter): Integer;
var
  I: Integer;
begin
  Result := -1;
  if AFilter = nil then
    Exit;

  for I := 0 to FItems.Count - 1 do
  begin
    if CompareFilter(AFilter, Get(I)) > 0 then
    begin
      FItems.Insert(I, AFilter);
      Result := I;
      Exit;
    end;
  end;
  FItems.Add(AFilter);
end;

function TVGFilterList.Clone(out enm: IEnumMoniker): HResult;
var
  NewList: TVGFilterList;
  I: Integer;
begin
  NewList := TVGFilterList.Create(False);
  for I := 0 to FItems.Count - 1 do
    NewList.Add(Get(I));
  NewList.FPos := FPos;
  enm := NewList;
  Result := S_OK;
end;

constructor TVGFilterList.Create(AOwnsObjects: Boolean);
begin
  FItems := TObjectList.Create(AOwnsObjects);
  FPos := 0;
end;

destructor TVGFilterList.Destroy;
begin
  FreeAndNil(FItems);
  inherited;
end;

function TVGFilterList.Get(AIndex: Integer): TVGFilter;
begin
  Result := nil;
  if (AIndex < 0) or (AIndex >= FItems.Count) then
    Exit;
  Result := TVGFilter(FItems[AIndex]);
end;

function TVGFilterList.GetCount: Integer;
begin
  Result := FItems.Count;
end;

function TVGFilterList.Next(celt: Integer; out elt;
  pceltFetched: PLongint): HResult;
var
  nToDo, I: Integer;
  pPtr: PIMoniker;
begin
  nToDo := FItems.Count - FPos;
  if nToDo = 0 then
  begin
    Result := E_FAIL;
    Exit;
  end;
  
  if nToDo > celt then
    nToDo := celt;
  if pceltFetched <> nil then
    pceltFetched^ := nToDo;
  pPtr := @elt;
  for I := 0 to nToDo - 1 do
  begin
    pPtr^ := Get(FPos + I).CreateInstance as IMoniker;
    Inc(FPos);
  end;
  if nToDo = celt then
    Result := S_OK
  else
    Result := S_FALSE;
end;

function TVGFilterList.Reset: HResult;
begin
  FPos := 0;
  Result := S_OK;
end;

function TVGFilterList.Skip(celt: Integer): HResult;
var
  nToDo: Integer;
begin
  nToDo := FItems.Count - FPos;
  if nToDo = 0 then
  begin
    Result := E_FAIL;
    Exit;
  end;

  Inc(FPos, nToDo);
  if nToDo = celt then
    Result := S_OK
  else
    Result := S_FALSE;
end;

end.
