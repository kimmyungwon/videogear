unit uAriaTree;

interface

uses
  Classes, SysUtils, Contnrs;

type
  EAriaTreeError = class(Exception);

  TAriaNodeList = class;

  TAriaNode = class
  private
    FName: WideString;
    FValue: Variant;
    FChildren: TAriaNodeList;
    FSiblings: TAriaNodeList;
  public
    constructor Create(const AName: WideString; ASiblings: TAriaNodeList); virtual;
    destructor Destroy; override;

    property Name: WideString read FName;
    property Value: Variant read FValue write FValue;
    property Children: TAriaNodeList read FChildren;
    property Siblings: TAriaNodeList read FSiblings;
  end;

  TAriaNodeList = class
  private
    FParent: TAriaNode;
    FItems: TObjectList;
  public
    constructor Create(AParent: TAriaNode);
    destructor Destroy; override;
    function Add(const AName: WideString; AErrorIfExists: Boolean = True): TAriaNode;
    function Extract(ANode: TAriaNode): TAriaNode; overload;
    function FindSibling(const AName: WideString): TAriaNode;
    function Get(AIndex: Integer): TAriaNode; overload;
    function Get(const AName: WideString): TAriaNode; overload;
    function GetCount: Integer;
    function IndexOf(const AName: WideString): Integer; overload;
    function IndexOf(ANode: TAriaNode): Integer; overload;
    procedure Remove(AIndex: Integer); overload;
    procedure Remove(const AName: WideString); overload;
    procedure Remove(ANode: TAriaNode); overload;

    property Count: Integer read GetCount;
    property Items[AIndex: Integer]: TAriaNode read Get; default;
    property ItemByName[const AName: WideString]: TAriaNode read Get;
    property Parent: TAriaNode read FParent;
  end;

  TAriaTree = class(TAriaNode)
  
  end;

implementation

resourcestring
  RsENodeAlreadyExists    = 'Node "%s" is already exists!';
  RsEParentIsNull         = 'Parent can not be NULL!';

{ TAriaNode }

constructor TAriaNode.Create(const AName: WideString; ASiblings: TAriaNodeList);
begin
  FName := AName;
  FSiblings := ASiblings;
  FChildren := TAriaNodeList.Create(Self);
end;

destructor TAriaNode.Destroy;
begin
  if FSiblings <> nil then
    FSiblings.Extract(Self);
  FreeAndNil(FChildren);
  inherited;
end;

{ TAriaNodeList }

function TAriaNodeList.Add(const AName: WideString; AErrorIfExists: Boolean): TAriaNode;
begin
  Result := FindSibling(AName);
  if Result = nil then
  begin
    Result := TAriaNode.Create(AName, Self);
  end
  else
  begin
    if AErrorIfExists then
      raise EAriaTreeError.CreateResFmt(@RsENodeAlreadyExists, [AName]);
  end;
end;

constructor TAriaNodeList.Create(AParent: TAriaNode);
begin
  FParent := AParent;
  if FParent = nil then
    raise EAriaTreeError.CreateRes(@RsEParentIsNull);
  FItems := TObjectList.Create;
end;

destructor TAriaNodeList.Destroy;
begin
  FreeAndNil(FItems);
  inherited;
end;

function TAriaNodeList.Extract(ANode: TAriaNode): TAriaNode;
begin
  Result := FItems.Extract(ANode) as TAriaNode;
end;

function TAriaNodeList.FindSibling(const AName: WideString): TAriaNode;
var
  nIdx: Integer;
begin
  nIdx := IndexOf(AName);
  if nIdx >= 0 then
    Result := Get(nIdx)
  else
    Result := nil;
end;

function TAriaNodeList.Get(AIndex: Integer): TAriaNode;
begin
  Result := FItems[AIndex] as TAriaNode;
end;

function TAriaNodeList.Get(const AName: WideString): TAriaNode;
begin
  Result := FindSibling(AName);
end;

function TAriaNodeList.GetCount: Integer;
begin
  Result := FItems.Count;
end;

function TAriaNodeList.IndexOf(ANode: TAriaNode): Integer;
begin
  Result := FItems.IndexOf(ANode);
end;

procedure TAriaNodeList.Remove(ANode: TAriaNode);
begin
  FItems.Remove(ANode);
end;

procedure TAriaNodeList.Remove(AIndex: Integer);
begin
  FItems.Delete(AIndex);
end;

procedure TAriaNodeList.Remove(const AName: WideString);
var
  nIdx: Integer;
begin
  nIdx := IndexOf(AName);
  if nIdx >= 0 then
    Remove(nIdx);
end;

function TAriaNodeList.IndexOf(const AName: WideString): Integer;
var
  I: Integer;
begin
  for I := 0 to GetCount - 1 do
  begin
    Result := I;
    if WideSameText(AName, Get(Result).Name) then
      Exit;
  end;
  Result := -1;
end;

end.
