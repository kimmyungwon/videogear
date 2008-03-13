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
    function CreateNode(AParent: TDomElement; const AName: WideString;
      AEncodeName: Boolean = True): TDomElement;
    function CreatePath(ANode: TDomElement; ASubPath: WideString): TDomElement;
    function FindNode(AParent: TDomElement; const AName: WideString): TDomElement;
    function OpenPath(ANode: TDomElement; ASubPath: WideString): TDomElement;
    procedure SetValue(ANode: TDomElement; AValName: WideString; AValType: DWORD;
      AValData: Pointer; AValDataSize: DWORD);
    procedure SaveToFile(const AFileName: WideString);
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
  // Ñ°ÕÒ¸ù½Úµã
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
