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

    property Value: Variant read FValue write FValue;
  end;

  TAriaNodeList = class
  private
    FParent: TAriaNode;
    FItems: TObjectList;
  public
    constructor Create(AParent: TAriaNode);
    destructor Destroy; override;
    function Append(const AName: WideString; ARaiseErrorIfExists: Boolean = True): TAriaNode;
    function FindSibling(const AName: WideString): TAriaNode;

    property Parent: TAriaNode read FParent;
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
  FreeAndNil(FChildren);
  inherited;
end;

{ TAriaNodeList }

function TAriaNodeList.Append(const AName: WideString; ARaiseErrorIfExists: Boolean): TAriaNode;
begin
  Result := FindSibling(AName);
  if Result = nil then
  begin
    Result := TAriaNode.Create(AName, Self);
  end
  else
  begin
    if ARaiseErrorIfExists then
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

function TAriaNodeList.FindSibling(const AName: WideString): TAriaNode;
begin
  Result := nil;
end;

end.
