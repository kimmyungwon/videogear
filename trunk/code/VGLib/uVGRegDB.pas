unit uVGRegDB;

interface

uses
  Windows, SysUtils, XMLIntf, XMLDoc, MD5, TntSysUtils;

type
  TVGRegRoots = array[HKEY_CLASSES_ROOT..HKEY_DYN_DATA] of IXMLNode;

  TVGRegDB = class
  private
    FXMLDoc: TXMLDocument;
    FRoots: TVGRegRoots;
  protected
    property XMLDoc: TXMLDocument read FXMLDoc;
  public
    constructor Create;
    destructor Destroy; override;
    function CreateNode(AParent: IXMLNode; const AName: WideString): IXMLNode;
    function CreatePath(ANode: IXMLNode; ASubPath: WideString): IXMLNode;
    procedure SetValue(ANode: IXMLNode; AValName: WideString; AValType: DWORD;
      AValData: Pointer; AValDataSize: DWORD);
    procedure SaveToFile(const AFileName: WideString);
  public
    property Roots: TVGRegRoots read FRoots;
  end;

implementation

function MakeHash(const AStr: WideString): WideString; inline;
begin
  Result := MD5DigestToString(MD5Buf(PWideChar(AStr)^, Length(AStr) * SizeOf(AStr)));
end;

function HKEYToStr(hKey: HKEY): WideString; inline;
begin
  case hKey of
    HKEY_CLASSES_ROOT: Result := 'HKEY_CLASSES_ROOT';
    HKEY_CURRENT_USER: Result := 'HKEY_CURRENT_USER';
    HKEY_LOCAL_MACHINE: Result := 'HKEY_LOCAL_MACHINE';
    HKEY_USERS: Result := 'HKEY_USERS';
    HKEY_PERFORMANCE_DATA: Result := 'HKEY_PERFORMANCE_DATA';
    HKEY_CURRENT_CONFIG: Result := 'HKEY_CURRENT_CONFIG';
    HKEY_DYN_DATA: Result := 'HKEY_DYN_DATA';
  else
    Result := WideFormat('%.8X', [hKey]);
  end;
end;

{ TVGRegDB }

constructor TVGRegDB.Create;
var
  hKey: Cardinal;
begin
  FXMLDoc := TXMLDocument.Create(nil);
  FXMLDoc.Active := True;
  FXMLDoc.DocumentElement := FXMLDoc.CreateNode('VGRegDB');
  for hKey := Low(FRoots) to High(FRoots) do
  begin
    FRoots[hKey] := FXMLDoc.DocumentElement.AddChild(HKEYToStr(hKey));
  end;
end;

function TVGRegDB.CreateNode(AParent: IXMLNode; const AName: WideString): IXMLNode;
begin
  Result := AParent.AddChild('N' + MakeHash(AName));
  Result.Attributes['Name'] := AName;
end;

function TVGRegDB.CreatePath(ANode: IXMLNode; ASubPath: WideString): IXMLNode;
var
  strPath, strNode: WideString;
  ndParent: IXMLNode;
begin
  strPath := WideExcludeTrailingPathDelimiter(WideExtractFilePath(ASubPath));
  if strPath = '' then
  begin
    Result := CreateNode(ANode, ASubPath);
  end
  else
  begin
    strNode := WideExtractFileName(ASubPath);
    ndParent := CreatePath(ANode, strPath);
    Result := ndParent.ChildNodes.FindNode(strNode);
    if Result = nil then
      Result := CreateNode(ndParent, strNode);
  end;
end;

destructor TVGRegDB.Destroy;
var
  hKey: Cardinal;
begin
  for hKey := Low(FRoots) to High(FRoots) do
    FRoots[hKey] := nil;  
  FreeAndNil(FXMLDoc);
  inherited;
end;

procedure TVGRegDB.SaveToFile(const AFileName: WideString);
begin
  FXMLDoc.SaveToFile(AFileName);
end;

procedure TVGRegDB.SetValue(ANode: IXMLNode; AValName: WideString;
  AValType: DWORD; AValData: Pointer; AValDataSize: DWORD);
var
  Node: IXMLNode;
begin
  if ANode = nil then
    Exit;
  Node := ANode.ChildNodes['A' + MakeHash(AValName)];
  Node.Attributes['Name'] := AValName;
  Node.Attributes['Type'] := AValType;
end;

end.
