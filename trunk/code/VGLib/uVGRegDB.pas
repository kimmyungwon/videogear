unit uVGRegDB;

interface

uses
  Windows, Classes, SysUtils, MD5, StrUtils, WideStrUtils,
  TntSysUtils, TntClasses, XdomCore;

type
  TVGRegRoots = array[HKEY_CLASSES_ROOT..HKEY_DYN_DATA] of TDomElement;

  TVGRegDB = class
  private
    FXMLImpl: TDomImplementation;
    FXMLDoc: TDomDocument;
    FRoots: TVGRegRoots;
  protected
    procedure LoadFromFile(const AFileName: WideString);

    property XMLDoc: TDomDocument read FXMLDoc;
  public
    constructor Create; overload;
    constructor Create(const AFileName: WideString); overload;
    destructor Destroy; override;
    // 创建节点：如果指定节点存在则创建，否则直接返回
    function CreateNode(AParent: TDomElement; const AName: WideString;
      AEncodeName: Boolean = True): TDomElement;
    // 创建路径：路径上如果遇到节点不存在则创建
    function CreatePath(ANode: TDomElement; ASubPath: WideString): TDomElement;
    // 查找节点：不存在返回nil
    function FindNode(AParent: TDomElement; const AName: WideString): TDomElement;
    // 打开路径：如果路径上有某一节点不存在则返回nil
    function OpenPath(ANode: TDomElement; ASubPath: WideString): TDomElement;
    // 设置值：
    procedure SetValue(ANode: TDomElement; AValName: WideString; AValType: DWORD;
      AValData: Pointer; AValDataSize: DWORD);
    // 保存到文件
    procedure SaveToFile(const AFileName: WideString);
    // 导入.reg文件
    function ImportReg(const AFileName: WideString): Boolean;
  public
    property Roots: TVGRegRoots read FRoots;
  end;

implementation

uses JclSysUtils;

const
  NODE_DOCROOT    = 'VGRegDB';
  NODE_NAME       = 'Name';
  NODE_ATTR       = 'Attributes';
  NODE_TYPE       = 'Type';
  NODE_DATA       = 'Data';

  ROOTHKEYs: array[HKEY_CLASSES_ROOT..HKEY_DYN_DATA] of WideString = (
    'HKEY_CLASSES_ROOT',
    'HKEY_CURRENT_USER',
    'HKEY_LOCAL_MACHINE',
    'HKEY_USERS',
    'HKEY_PERFORMANCE_DATA',
    'HKEY_CURRENT_CONFIG',
    'HKEY_DYN_DATA'
  );

resourcestring
  RsEBadFileFormat    = 'Unknown file format: "%s"';

function GetNodeName(AStr: WideString): WideString; //inline;
begin
  AStr := WideUpperCase(AStr);
  Result := 'N' + MD5DigestToString(MD5Buf(PWideChar(AStr)^,
    Length(AStr) * SizeOf(WideChar)));
end;

function StrToHKEY(const AStr: WideString): HKEY; inline;
var
  I: HKEY;
begin
  for I := Low(ROOTHKEYs) to High(ROOTHKEYs) do
  begin
    if WideSameText(AStr, ROOTHKEYs[I]) then
    begin
      Result := I;
      Exit;
    end;
  end;
  Result := 0;
end;

function HKEYToStr(hKey: HKEY): WideString; inline;
begin
  if (hKey >= HKEY_CLASSES_ROOT) and (hKey <= HKEY_DYN_DATA) then
    Result := ROOTHKEYs[hKey]
  else
    Result := WideFormat('HKEY_%.8X', [hKey]);
end;

function EncodeData(AData: PByte; ADataSize: Cardinal): WideString;
var
  pPtr: PByte;
begin
  pPtr := AData;
  while Cardinal(pPtr) - Cardinal(AData) >= ADataSize do
  begin
    Result := Result + WideFormat('%.2X', [pPtr^]);
    Inc(pPtr);
  end; 
end;

function WideStartsText(const ASubText, AText: WideString): Boolean;
var
{$IFDEF MSWINDOWS}
  P: PWideChar;
{$ENDIF}
  L, L2: Integer;
begin
{$IFDEF MSWINDOWS}
  P := PWideChar(AText);
{$ENDIF}
  L := Length(ASubText);
  L2 := Length(AText);
  if L > L2 then
    Result := False
  else
{$IFDEF MSWINDOWS}
    Result := CompareStringW(LOCALE_USER_DEFAULT, NORM_IGNORECASE,
      P, L, PWideChar(ASubText), L) = CSTR_EQUAL;
{$ENDIF}
{$IFDEF LINUX}
    Result := WideSameText(ASubText, Copy(AText, 1, L));
{$ENDIF}
end;

{ TVGRegDB }

constructor TVGRegDB.Create;
var
  hKey: Cardinal;
begin
  FXMLImpl := TDomImplementation.Create(nil);
  FXMLDoc := TDomDocument.Create(FXMLImpl);
  FXMLDoc.XmlVersion := '1.0';
  FXMLDoc.XmlEncoding := 'UTF-8';
  FXMLDoc.AppendChild(TDomElement.Create(FXMLDoc, NODE_DOCROOT));
  for hKey := Low(FRoots) to High(FRoots) do
  begin
    FRoots[hKey] := TDomElement.Create(FXMLDoc, HKEYToStr(hKey));
    FXMLDoc.DocumentElement.AppendChild(FRoots[hKey]);
  end;
end;

constructor TVGRegDB.Create(const AFileName: WideString);
begin
  FXMLImpl := TDomImplementation.Create(nil);
  LoadFromFile(AFileName);
end;

function TVGRegDB.CreateNode(AParent: TDomElement; const AName: WideString;
  AEncodeName: Boolean): TDomElement;
begin
  if AEncodeName then
  begin
    Result := AParent.GetFirstChildElement(GetNodeName(AName));
    if Result = nil then
    begin
      Result := TDomElement.Create(FXMLDoc, GetNodeName(AName));
      Result.SetAttribute(NODE_NAME, AName);
      AParent.AppendChild(Result);
    end;
  end
  else
  begin
    Result := AParent.GetFirstChildElement(AName);
    if Result = nil then
    begin
      Result := TDomElement.Create(FXMLDoc, AName);
      AParent.AppendChild(Result);
    end;
  end;
end;

function TVGRegDB.CreatePath(ANode: TDomElement; ASubPath: WideString): TDomElement;
var
  strPath, strNode: WideString;
  ndParent: TDomElement;
begin
  strPath := WideExcludeTrailingPathDelimiter(WideExtractFilePath(ASubPath));
  if strPath = '' then
  begin
    if ASubPath[1] = PathDelim then
      ASubPath := RightStr(ASubPath, Length(ASubPath) - 1);
    Result := CreateNode(ANode, ASubPath);
  end
  else
  begin
    strNode := WideExtractFileName(ASubPath);
    ndParent := CreatePath(ANode, strPath);
    Result := CreateNode(ndParent, strNode);
  end;
end;

destructor TVGRegDB.Destroy;
begin
  FreeAndNil(FXMLDoc);
  FreeAndNil(FXMLImpl);
  inherited;
end;

function TVGRegDB.FindNode(AParent: TDomElement;
  const AName: WideString): TDomElement;
begin
  Result := AParent.GetFirstChildElement(GetNodeName(AName));
end;

function TVGRegDB.ImportReg(const AFileName: WideString): Boolean;
var
  sg: ISafeGuard;
  ini: TTntStringList;
  I: Integer;
  strLine, strNode, strHKEY, strSubPath, strVal, strData: WideString;
  nData: Cardinal;
  Node: TDomElement;
begin
  Result := False;

  ini := TTntStringList(Guard(TTntStringList.Create, sg));
  try
    ini.LoadFromFile(AFileName);
    if (ini.Count < 1) then
      Exit;
    if (not WideSameStr(ini[0], 'Windows Registry Editor Version 5.00')) and
      (not WideSameStr(ini[0], 'REGEDIT4')) then
      Exit;

    Node := nil;
    for I := 1 to ini.Count - 1 do
    begin
      strLine := Trim(ini[I]);
      if strLine = '' then
        Continue;
      if strLine[1] = '[' then // 该行为节
      begin
        strNode := MidStr(strLine, 2, Length(strLine) - 2);
        strHKEY := LeftStr(strNode, Pos(PathDelim, strNode) - 1);
        strSubPath := RightStr(strNode, Length(strNode) - Length(strHKEY));
        Node := CreatePath(FRoots[StrToHKEY(strHKEY)], strSubPath);
      end
      else
      begin
        if Node = nil then
          Continue;
        strVal := ini.Names[I];
        strVal := MidStr(strVal, 2, Length(strVal) - 2);  // 去掉双引号
        strData := ini.ValueFromIndex[I];
        if strData[1] = '"' then  // 值的类型是字符串
        begin
          strData := MidStr(strData, 2, Length(strData) - 2);  // 去掉双引号
          strData := WideReplaceStr(strData, '\\', '\');
          SetValue(Node, strVal, REG_SZ, PWideChar(strData), 0);
        end
        else if WideStartsText('dword:', strData) then
        begin
          strData := RightStr(strData, Length(strData) - Length('dword:'));
          nData := StrToInt64('$' + strData);
          SetValue(Node, strVal, REG_DWORD, @nData, SizeOf(nData));
        end;
      end;
    end;

    Result := True;
  except
    Exit;
  end;
end;

procedure TVGRegDB.LoadFromFile(const AFileName: WideString);
var
  sg: ISafeGuard;
  XmlToDom: TXmlToDomParser;
  Node: TDomNode;
  nRootKeyFound: Integer;
  hKey: Windows.HKEY;
begin
  XmlToDom := TXmlToDomParser(Guard(TXmlToDomParser.Create(nil), sg));
  XmlToDom.DOMImpl := FXMLImpl;
  FXMLDoc := XmlToDom.ParseFile(AFileName, True);
  if (FXMLDoc.DocumentElement = nil) or
    (FXMLDoc.DocumentElement.NodeName <> NODE_DOCROOT) then
    raise Exception.CreateResFmt(@RsEBadFileFormat, [AFileName]);
  // 寻找根节点
  nRootKeyFound := 0;
  Node := FXMLDoc.DocumentElement.FirstChild;
  while Assigned(Node) do
  begin
    hKey := StrToHKEY(Node.NodeName);
    if (hKey <> 0) and (Node.NodeType = ntElement_Node) then
    begin
      FRoots[hKey] := Node as TDomElement;
      Inc(nRootKeyFound);
    end;
    Node := Node.NextSibling;
  end;
  if nRootKeyFound <> Length(FRoots) then
    raise Exception.CreateResFmt(@RsEBadFileFormat, [AFileName]);
end;

function TVGRegDB.OpenPath(ANode: TDomElement; ASubPath: WideString): TDomElement;
var
  strPath, strNode: WideString;
  ndParent: TDomElement;
begin
  strPath := WideExcludeTrailingPathDelimiter(WideExtractFilePath(ASubPath));
  if strPath = '' then
  begin
    if ASubPath[1] = PathDelim then
      ASubPath := RightStr(ASubPath, Length(ASubPath) - 1);
    Result := FindNode(ANode, ASubPath);
  end
  else
  begin
    strNode := WideExtractFileName(ASubPath);
    ndParent := OpenPath(ANode, strPath);
    if ndParent = nil then
    begin
      Result := nil;
      Exit;
    end;
    Result := FindNode(ndParent, strNode);
  end;
end;

procedure TVGRegDB.SaveToFile(const AFileName: WideString);
var
  sg: IMultiSafeGuard;
  DomToXml: TDomToXmlParser;
  F: TTntFileStream;
begin
  F := TTntFileStream(Guard(TTntFileStream.Create(AFileName, fmCreate), sg));
  DomToXml := TDomToXmlParser(Guard(TDomToXmlParser.Create(nil), sg));
  DomToXml.DOMImpl := FXMLImpl;
  DomToXml.WriteToStream(FXMLDoc, 'UTF-8', F);
end;

procedure TVGRegDB.SetValue(ANode: TDomElement; AValName: WideString;
  AValType: DWORD; AValData: Pointer; AValDataSize: DWORD);
var
  Node: TDomElement;
begin
  if ANode = nil then
    Exit;
  Node := CreateNode(CreateNode(ANode, NODE_ATTR, False), AValName);
  Node.SetAttribute(NODE_NAME, AValName);
  Node.SetAttribute(NODE_TYPE, WideFormat('%d', [AValType]));
  case AValType of
    REG_SZ, REG_EXPAND_SZ: begin
      Node.SetAttribute(NODE_DATA, WideString(PWideChar(AValData)));
    end;
    REG_DWORD, REG_DWORD_BIG_ENDIAN: begin
      Node.SetAttribute(NODE_DATA, WideFormat('%d', [PDWORD(AValData)^]));
    end;
  else
    Node.SetAttribute(NODE_DATA, EncodeData(AValData, AValDataSize));  
  end;
end;

end.
