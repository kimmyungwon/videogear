unit uAriaJclExt;

interface

uses
  Classes, SysUtils, ActiveX, JclContainerIntf, JclAbstractContainers, JclHashMaps;

type
  TGUIDCompare = function(const Obj1, Obj2: TGUID): Integer;
  TGUIDEqualityCompare = function(const Obj1, Obj2: TGUID): Boolean;
  TGUIDHashConvert = function(const AValue: TGUID): Integer;

  IJclGUIDEqualityComparer = interface
    ['{F3E4F637-0025-426d-AB4E-1E00F60D312E}']
    function GetEqualityCompare: TGUIDEqualityCompare;
    procedure SetEqualityCompare(Value: TGUIDEqualityCompare);
    function ItemsEqual(const A, B: TGUID): Boolean;
    property EqualityCompare: TGUIDEqualityCompare read GetEqualityCompare write SetEqualityCompare;
  end;

  IJclGUIDComparer = interface
    ['{E1A2F3EB-3A81-4011-A0A0-17AC1041C6C1}']
    function GetCompare: TGUIDCompare;
    procedure SetCompare(Value: TGUIDCompare);
    function ItemsCompare(const A, B: TGUID): Integer;
    property Compare: TGUIDCompare read GetCompare write SetCompare;
  end;

  IJclGUIDHashConverter = interface
    ['{FC2613D3-8E39-4f87-94EB-C6A4AB331F0E}']
    function GetHashConvert: TGUIDHashConvert;
    procedure SetHashConvert(Value: TGUIDHashConvert);
    function Hash(const AGUID: TGUID): Integer;
    property HashConvert: TGUIDHashConvert read GetHashConvert write SetHashConvert;
  end;

  IJclGUIDIterator = interface(IJclAbstractIterator)
    ['{0EA5A34C-410F-4246-9CFD-9586CF0ED442}']
    function Add(const AValue: TGUID): Boolean;
    function Equals(const AIterator: IJclSingleIterator): Boolean;
    function GetGUID: TGUID;
    function HasNext: Boolean;
    function HasPrevious: Boolean;
    function Insert(const AValue: TGUID): Boolean;
    function Next: TGUID;
    function NextIndex: Integer;
    function Previous: TGUID;
    function PreviousIndex: Integer;
    procedure Remove;
    procedure Reset;
    procedure SetValue(const AValue: TGUID);
    {$IFDEF SUPPORTS_FOR_IN}
    function MoveNext: Boolean;
    property Current: TGUID read GetValue;
    {$ENDIF SUPPORTS_FOR_IN}
  end;

  IJclGUIDCollection = interface(IJclContainer)
    ['{21D67B30-6980-47c2-A169-2A50C47A4A86}']
    function Add(const AGUID: TGUID): Boolean;
    function AddAll(const ACollection: IJclGUIDCollection): Boolean;
    procedure Clear;
    function Contains(const AGUID: TGUID): Boolean;
    function ContainsAll(const ACollection: IJclGUIDCollection): Boolean;
    function Equals(const ACollection: IJclGUIDCollection): Boolean;
    function First: IJclGUIDIterator;
    function IsEmpty: Boolean;
    function Last: IJclGUIDIterator;
    function Remove(const AGUID: TGUID): Boolean;
    function RemoveAll(const ACollection: IJclGUIDCollection): Boolean;
    function RetainAll(const ACollection: IJclGUIDCollection): Boolean;
    function Size: Integer;
    {$IFDEF SUPPORTS_FOR_IN}
    function GetEnumerator: IJclGUIDIterator;
    {$ENDIF SUPPORTS_FOR_IN}
  end;

  IJclGUIDSet = interface(IJclGUIDCollection)
    ['{134B8587-A9CC-4900-B286-78861937A665}']
    procedure Intersect(const ACollection: IJclGUIDCollection);
    procedure Subtract(const ACollection: IJclGUIDCollection);
    procedure Union(const ACollection: IJclGUIDCollection);
  end;

  IJclGUIDMap = interface(IJclContainer)
    ['{22E84CF2-5D28-48f9-BDCC-4ACD5EB5B92C}']
    procedure Clear;
    function ContainsKey(const Key: TGUID): Boolean;
    function ContainsValue(Value: TObject): Boolean;
    function Equals(const AMap: IJclGUIDMap): Boolean;
    function GetValue(const Key: TGUID): TObject;
    function IsEmpty: Boolean;
    function KeyOfValue(Value: TObject): TGUID;
    //function KeySet: IJclGUIDSet;
    //procedure PutAll(const AMap: IJclGUIDMap);
    procedure PutValue(const Key: TGUID; Value: TObject);
    function Remove(const Key: TGUID): TObject;
    function Size: Integer;
    function Values: IJclCollection;
    property Items[const Key: TGUID]: TObject read GetValue write PutValue;
      {$IFNDEF BUGGY_DEFAULT_INDEXED_PROP} default; {$ENDIF ~BUGGY_DEFAULT_INDEXED_PROP}
  end;

  TJclGUIDAbstractContainer = class(TJclAbstractContainerBase, {$IFDEF THREADSAFE} IJclLockable, {$ENDIF THREADSAFE}
    IJclCloneable, IJclIntfCloneable, IJclContainer,
    IJclGUIDEqualityComparer, IJclGUIDComparer, IJclGUIDHashConverter)
  protected
    FEqualityCompare: TGUIDEqualityCompare;
    FCompare: TGUIDCompare;
    FHashConvert: TGUIDHashConvert;
    procedure AssignPropertiesTo(Dest: TJclAbstractContainerBase); override;
    function FreeGUID(var AGUID: TGUID): TGUID;
    { IJclIntfCloneable }
    function IJclIntfCloneable.Clone = IntfClone;
    { IJclGUIDEqualityComparer }
    function GetEqualityCompare: TGUIDEqualityCompare; virtual;
    procedure SetEqualityCompare(Value: TGUIDEqualityCompare); virtual;
    function ItemsEqual(const A, B: TGUID): Boolean; virtual;
    { IJclGUIDComparer }
    function GetCompare: TGUIDCompare; virtual;
    procedure SetCompare(Value: TGUIDCompare); virtual;
    function ItemsCompare(const A, B: TGUID): Integer; virtual;
    { IJclGUIDHashConverter }
    function GetHashConvert: TGUIDHashConvert; virtual;
    procedure SetHashConvert(Value: TGUIDHashConvert); virtual;
    function Hash(const AGUID: TGUID): Integer; virtual;
  public
    property EqualityCompare: TGUIDEqualityCompare read GetEqualityCompare write SetEqualityCompare;
    property Compare: TGUIDCompare read GetCompare write SetCompare;
    property HashConvert: TGUIDHashConvert read GetHashConvert write SetHashConvert;
  end;

  TJclGUIDHashEntry = record
    Key: TGUID;
    Value: TObject;
  end;

  TJclGUIDHashEntryArray = array of TJclGUIDHashEntry;

  TJclGUIDBucket = class
  public
    Size: Integer;
    Entries: TJclGUIDHashEntryArray;
  end;

  TJclGUIDBucketArray = array of TJclGUIDBucket;

  TJclGUIDHashMap = class(TJclGUIDAbstractContainer, {$IFDEF THREADSAFE} IJclLockable, {$ENDIF THREADSAFE}
    IJclIntfCloneable, IJclCloneable, IJclGrowable, IJclPackable, IJclContainer, IJclValueOwner, 
    IJclGUIDMap)
  private
    FBuckets: TJclGUIDBucketArray;
    FHashFunction: TJclHashFunction;
    FOwnsValues: Boolean;
  protected
    //procedure AssignDataTo(Dest: TJclAbstractContainerBase); override;
    procedure AssignPropertiesTo(Dest: TJclAbstractContainerBase); override;
    procedure MoveArray(var List: TJclGUIDHashEntryArray; FromIndex, ToIndex, Count: Integer);
    { IJclPackable }
    procedure Pack; override;
    procedure SetCapacity(Value: Integer); override;
    { IJclSingleMap }
    procedure Clear;
    function ContainsKey(const Key: TGUID): Boolean;
    function ContainsValue(Value: TObject): Boolean;
    function Equals(const AMap: IJclGUIDMap): Boolean;
    function GetValue(const Key: TGUID): TObject;
    function IsEmpty: Boolean;
    function KeyOfValue(Value: TObject): TGUID;
    //function KeySet: IJclGUIDSet;
    //procedure PutAll(const AMap: IJclGUIDMap);
    procedure PutValue(const Key: TGUID; Value: TObject);
    function Remove(const Key: TGUID): TObject;
    function Size: Integer;
    function Values: IJclCollection;
    { IJclIntfCloneable }
    function IJclIntfCloneable.Clone = IntfClone;
    { IJclValueOwner }
    function FreeValue(var Value: TObject): TObject;
    function GetOwnsValues: Boolean;
    function CreateEmptyContainer: TJclAbstractContainerBase; override;
    function FreeKey(var Key: TGUID): TGUID;
    function KeysEqual(const A, B: TGUID): Boolean;
    function ValuesEqual(A, B: TObject): Boolean;
  public
    constructor Create(ACapacity: Integer; AOwnsValues: Boolean);
    destructor Destroy; override;
    property HashFunction: TJclHashFunction read FHashFunction write FHashFunction;
    property OwnsValues: Boolean read FOwnsValues;
  end;

implementation

uses JclArrayLists, JclArraySets;

{ TJclGUIDAbstractContainer }

procedure TJclGUIDAbstractContainer.AssignPropertiesTo(
  Dest: TJclAbstractContainerBase);
var
  ADest: TJclGUIDAbstractContainer;
begin
  inherited AssignPropertiesTo(Dest);
  if Dest is TJclGUIDAbstractContainer then
  begin
    ADest := TJclGUIDAbstractContainer(Dest);
    ADest.EqualityCompare := EqualityCompare;
    ADest.Compare := Compare;
    ADest.HashConvert := HashConvert;
  end;
end;

function TJclGUIDAbstractContainer.FreeGUID(var AGUID: TGUID): TGUID;
begin
  Result := AGUID;
  AGUID := GUID_NULL;
end;

function TJclGUIDAbstractContainer.GetCompare: TGUIDCompare;
begin
  Result := FCompare;
end;

function TJclGUIDAbstractContainer.GetEqualityCompare: TGUIDEqualityCompare;
begin
  Result := FEqualityCompare;
end;

function TJclGUIDAbstractContainer.GetHashConvert: TGUIDHashConvert;
begin
  Result := FHashConvert;
end;

function TJclGUIDAbstractContainer.Hash(const AGUID: TGUID): Integer;
var
  pPtr: PByteArray;
  I: Integer;
begin
  if Assigned(FHashConvert) then
    Result := FHashConvert(AGUID)
  else
  begin
    Result := 0;
    pPtr := PByteArray(@AGUID);
    for I := 0 to SizeOf(AGUID) - 1 do
      Inc(Result, pPtr^[I] * I * 256);
  end;
end;

function TJclGUIDAbstractContainer.ItemsCompare(const A, B: TGUID): Integer;
begin
  if Assigned(FCompare) then
    Result := FCompare(A, B)
  else
  begin
    Result := A.D1 - B.D1;
    if Result <> 0 then Exit;
    Result := A.D2 - B.D2;
    if Result <> 0 then Exit;
    Result := A.D3 - B.D3;
    if Result <> 0 then Exit;
    Result := PCardinal(@A.D4)^ - PCardinal(@B.D4)^;
  end;
end;

function TJclGUIDAbstractContainer.ItemsEqual(const A, B: TGUID): Boolean;
begin
  if Assigned(FEqualityCompare) then
    Result := FEqualityCompare(A, B)
  else
  if Assigned(FCompare) then
    Result := FCompare(A, B) = 0
  else
    Result := IsEqualGUID(A, B);
end;

procedure TJclGUIDAbstractContainer.SetCompare(Value: TGUIDCompare);
begin
  FCompare := Value;
end;

procedure TJclGUIDAbstractContainer.SetEqualityCompare(
  Value: TGUIDEqualityCompare);
begin
  FEqualityCompare := Value;
end;

procedure TJclGUIDAbstractContainer.SetHashConvert(Value: TGUIDHashConvert);
begin
  FHashConvert := Value;
end;

{ TJclGUIDHashMap }

//procedure TJclGUIDHashMap.AssignDataTo(Dest: TJclAbstractContainerBase);
//var
//  I, J: Integer;
//  SelfBucket, NewBucket: TJclGUIDBucket;
//  ADest: TJclGUIDHashMap;
//  AMap: IJclGUIDMap;
//begin
//  {$IFDEF THREADSAFE}
//  if FThreadSafe then
//    SyncReaderWriter.BeginRead;
//  try
//  {$ENDIF THREADSAFE}
//    inherited AssignDataTo(Dest);
//    if Dest is TJclGUIDHashMap then
//    begin
//      ADest := TJclGUIDHashMap(Dest);
//      ADest.Clear;
//      for I := 0 to FCapacity - 1 do
//      begin
//        SelfBucket := FBuckets[I];
//        if SelfBucket <> nil then
//        begin
//          NewBucket := TJclGUIDBucket.Create;
//          SetLength(NewBucket.Entries, SelfBucket.Size);
//          for J := 0 to SelfBucket.Size - 1 do
//          begin
//            NewBucket.Entries[J].Key := SelfBucket.Entries[J].Key;
//            NewBucket.Entries[J].Value := SelfBucket.Entries[J].Value;
//          end;
//          NewBucket.Size := SelfBucket.Size;
//          ADest.FBuckets[I] := NewBucket;
//        end;
//      end;
//    end
//    else
//    if Supports(IInterface(Dest), IJclGUIDMap, AMap) then
//    begin
//      AMap.Clear;
//      AMap.PutAll(Self);
//    end;
//  {$IFDEF THREADSAFE}
//  finally
//    if FThreadSafe then
//      SyncReaderWriter.EndRead;
//  end;
//  {$ENDIF THREADSAFE}
//end;

procedure TJclGUIDHashMap.AssignPropertiesTo(Dest: TJclAbstractContainerBase);
begin
  inherited AssignPropertiesto(Dest);
  if Dest is TJclGUIDHashMap then
    TJclGUIDHashMap(Dest).HashFunction := HashFunction;
end;

procedure TJclGUIDHashMap.Clear;
var
  I, J: Integer;
  Bucket: TJclGUIDBucket;
begin
  if ReadOnly then
    raise EJclReadOnlyError.Create;
  {$IFDEF THREADSAFE}
  if FThreadSafe then
    SyncReaderWriter.BeginWrite;
  try
  {$ENDIF THREADSAFE}
    for I := 0 to FCapacity - 1 do
    begin
      Bucket := FBuckets[I];
      if Bucket <> nil then
      begin
        for J := 0 to Bucket.Size - 1 do
        begin
          FreeKey(Bucket.Entries[J].Key);
          FreeValue(Bucket.Entries[J].Value);
        end;
        FreeAndNil(FBuckets[I]);
      end;
    end;
    FSize := 0;
  {$IFDEF THREADSAFE}
  finally
    if FThreadSafe then
      SyncReaderWriter.EndWrite;
  end;
  {$ENDIF THREADSAFE}
end;

function TJclGUIDHashMap.ContainsKey(const Key: TGUID): Boolean;
var
  I: Integer;
  Bucket: TJclGUIDBucket;
begin
  {$IFDEF THREADSAFE}
  if FThreadSafe then
    SyncReaderWriter.BeginRead;
  try
  {$ENDIF THREADSAFE}
    Result := False;
    Bucket := FBuckets[FHashFunction(Hash(Key), FCapacity)];
    if Bucket <> nil then
      for I := 0 to Bucket.Size - 1 do
        if KeysEqual(Bucket.Entries[I].Key, Key) then
        begin
          Result := True;
          Break;
        end;
  {$IFDEF THREADSAFE}
  finally
    if FThreadSafe then
      SyncReaderWriter.EndRead;
  end;
  {$ENDIF THREADSAFE}
end;

function TJclGUIDHashMap.ContainsValue(Value: TObject): Boolean;
var
  I, J: Integer;
  Bucket: TJclGUIDBucket;
begin
  {$IFDEF THREADSAFE}
  if FThreadSafe then
    SyncReaderWriter.BeginRead;
  try
  {$ENDIF THREADSAFE}
    Result := False;
    for J := 0 to FCapacity - 1 do
    begin
      Bucket := FBuckets[J];
      if Bucket <> nil then
        for I := 0 to Bucket.Size - 1 do
          if ValuesEqual(Bucket.Entries[I].Value, Value) then
          begin
            Result := True;
            Break;
          end;
    end;
  {$IFDEF THREADSAFE}
  finally
    if FThreadSafe then
      SyncReaderWriter.EndRead;
  end;
  {$ENDIF THREADSAFE}
end;

constructor TJclGUIDHashMap.Create(ACapacity: Integer; AOwnsValues: Boolean);
begin
  inherited Create;
  FOwnsValues := AOwnsValues;
  SetCapacity(ACapacity);
  FHashFunction := HashMul;
end;

function TJclGUIDHashMap.CreateEmptyContainer: TJclAbstractContainerBase;
begin
  Result := TJclGUIDHashMap.Create(FCapacity, False);
  AssignPropertiesTo(Result);
end;

destructor TJclGUIDHashMap.Destroy;
begin
  FReadOnly := False;
  Clear;
  inherited;
end;

function TJclGUIDHashMap.Equals(const AMap: IJclGUIDMap): Boolean;
var
  I, J: Integer;
  Bucket: TJclGUIDBucket;
begin
  {$IFDEF THREADSAFE}
  if FThreadSafe then
    SyncReaderWriter.BeginRead;
  try
  {$ENDIF THREADSAFE}
    Result := False;
    if AMap = nil then
      Exit;
    if FSize <> AMap.Size then
      Exit;
    for I := 0 to FCapacity - 1 do
    begin
      Bucket := FBuckets[I];
      if Bucket <> nil then
        for J := 0 to Bucket.Size - 1 do
          if AMap.ContainsKey(Bucket.Entries[J].Key) then
          begin
            if not ValuesEqual(AMap.GetValue(Bucket.Entries[J].Key), Bucket.Entries[J].Value) then
              Exit;
          end
          else
            Exit;
    end;
    Result := True;
  {$IFDEF THREADSAFE}
  finally
    if FThreadSafe then
      SyncReaderWriter.EndRead;
  end;
  {$ENDIF THREADSAFE}
end;

function TJclGUIDHashMap.FreeKey(var Key: TGUID): TGUID;
begin
  Result := Key;
  Key := GUID_NULL;
end;

function TJclGUIDHashMap.FreeValue(var Value: TObject): TObject;
begin
  if FOwnsValues then
  begin
    Result := nil;
    FreeAndNil(Value);
  end
  else
  begin
    Result := Value;
    Value := nil;
  end;
end;

function TJclGUIDHashMap.GetOwnsValues: Boolean;
begin
  Result := FOwnsValues;
end;

function TJclGUIDHashMap.GetValue(const Key: TGUID): TObject;
var
  I: Integer;
  Bucket: TJclGUIDBucket;
  Found: Boolean;
begin
  {$IFDEF THREADSAFE}
  if FThreadSafe then
    SyncReaderWriter.BeginRead;
  try
  {$ENDIF THREADSAFE}
    Found := False;
    Result := nil;
    Bucket := FBuckets[FHashFunction(Hash(Key), FCapacity)];
    if Bucket <> nil then
      for I := 0 to Bucket.Size - 1 do
        if KeysEqual(Bucket.Entries[I].Key, Key) then
        begin
          Result := Bucket.Entries[I].Value;
          Found := True;
          Break;
        end;
    if (not Found) and (not FReturnDefaultElements) then
      raise EJclNoSuchElementError.Create('');
  {$IFDEF THREADSAFE}
  finally
    if FThreadSafe then
      SyncReaderWriter.EndRead;
  end;
  {$ENDIF THREADSAFE}
end;

function TJclGUIDHashMap.IsEmpty: Boolean;
begin
  Result := FSize = 0;
end;

function TJclGUIDHashMap.KeyOfValue(Value: TObject): TGUID;
var
  I, J: Integer;
  Bucket: TJclGUIDBucket;
  Found: Boolean;
begin
  {$IFDEF THREADSAFE}
  if FThreadSafe then
    SyncReaderWriter.BeginRead;
  try
  {$ENDIF THREADSAFE}
    Found := False;
    Result := GUID_NULL;
    for J := 0 to FCapacity - 1 do
    begin
      Bucket := FBuckets[J];
      if Bucket <> nil then
        for I := 0 to Bucket.Size - 1 do
          if ValuesEqual(Bucket.Entries[I].Value, Value) then
          begin
            Result := Bucket.Entries[I].Key;
            Found := True;
            Break;
          end;
    end;
    if (not Found) and (not FReturnDefaultElements) then
      raise EJclNoSuchElementError.Create('');
  {$IFDEF THREADSAFE}
  finally
    if FThreadSafe then
      SyncReaderWriter.EndRead;
  end;
  {$ENDIF THREADSAFE}
end;

function TJclGUIDHashMap.KeysEqual(const A, B: TGUID): Boolean;
begin
  Result := ItemsEqual(A, B);
end;

//function TJclGUIDHashMap.KeySet: IJclGUIDSet;
//var
//  I, J: Integer;
//  Bucket: TJclGUIDBucket;
//begin
//  {$IFDEF THREADSAFE}
//  if FThreadSafe then
//    SyncReaderWriter.BeginRead;
//  try
//  {$ENDIF THREADSAFE}
//    Result := TJclGUIDArraySet.Create(FSize);
//    for I := 0 to FCapacity - 1 do
//    begin
//      Bucket := FBuckets[I];
//      if Bucket <> nil then
//        for J := 0 to Bucket.Size - 1 do
//          Result.Add(Bucket.Entries[J].Key);
//    end;
//  {$IFDEF THREADSAFE}
//  finally
//    if FThreadSafe then
//      SyncReaderWriter.EndRead;
//  end;
//  {$ENDIF THREADSAFE}
//end;

procedure TJclGUIDHashMap.MoveArray(var List: TJclGUIDHashEntryArray; FromIndex,
  ToIndex, Count: Integer);
{$IFDEF CLR}
var
  I: Integer;
begin
  if FromIndex < ToIndex then
  begin
    for I := Count - 1 downto 0 do
      List[ToIndex + I] := List[FromIndex + I];
    if (ToIndex - FromIndex) < Count then
      // overlapped source and target
      for I := 0 to ToIndex - FromIndex - 1 do
      begin
        List[FromIndex + I].Key := 0.0;
        List[FromIndex + I].Value := nil;
      end
    else
      // independant
      for I := 0 to Count - 1 do
      begin
        List[FromIndex + I].Key := 0.0;
        List[FromIndex + I].Value := nil;
      end;
  end
  else
  if FromIndex > ToIndex then
  begin
    for I := 0 to Count - 1 do
      List[ToIndex + I] := List[FromIndex + I];
    if (FromIndex - ToIndex) < Count then
      // overlapped source and target
      for I := Count - FromIndex + ToIndex to Count - 1 do
      begin
        List[FromIndex + I].Key := 0.0;
        List[FromIndex + I].Value := nil;
      end
    else
      // independant
      for I := 0 to Count - 1 do
      begin
        List[FromIndex + I].Key := 0.0;
        List[FromIndex + I].Value := nil;
      end;
  end;
end;
{$ELSE}
begin
  if Count > 0 then
  begin
    Move(List[FromIndex], List[ToIndex], Count * SizeOf(List[0]));
    { Keep reference counting working }
    if FromIndex < ToIndex then
    begin
      if (ToIndex - FromIndex) < Count then
        FillChar(List[FromIndex], (ToIndex - FromIndex) * SizeOf(List[0]), 0)
      else
        FillChar(List[FromIndex], Count * SizeOf(List[0]), 0);
    end
    else
    if FromIndex > ToIndex then
    begin
      if (FromIndex - ToIndex) < Count then
        FillChar(List[ToIndex + Count], (FromIndex - ToIndex) * SizeOf(List[0]), 0)
      else
        FillChar(List[FromIndex], Count * SizeOf(List[0]), 0);
    end;
  end;
end;
{$ENDIF CLR}

procedure TJclGUIDHashMap.Pack;
var
  I: Integer;
  Bucket: TJclGUIDBucket;
begin
  if ReadOnly then
    raise EJclReadOnlyError.Create;
  {$IFDEF THREADSAFE}
  if FThreadSafe then
    SyncReaderWriter.BeginWrite;
  try
  {$ENDIF THREADSAFE}
    for I := 0 to FCapacity - 1 do
    begin
      Bucket := FBuckets[I];
      if Bucket <> nil then
      begin
        if Bucket.Size > 0 then
          SetLength(Bucket.Entries, Bucket.Size)
        else
          FreeAndNil(FBuckets[I]);
      end;
    end;
  {$IFDEF THREADSAFE}
  finally
    if FThreadSafe then
      SyncReaderWriter.EndWrite;
  end;
  {$ENDIF THREADSAFE}
end;

//procedure TJclGUIDHashMap.PutAll(const AMap: IJclGUIDMap);
//var
//  It: IJclGUIDIterator;
//  Key: TGUID;
//begin
//  if ReadOnly then
//    raise EJclReadOnlyError.Create;
//  {$IFDEF THREADSAFE}
//  if FThreadSafe then
//    SyncReaderWriter.BeginWrite;
//  try
//  {$ENDIF THREADSAFE}
//    if AMap = nil then
//      Exit;
//    It := AMap.KeySet.First;
//    while It.HasNext do
//    begin
//      Key := It.Next;
//      PutValue(Key, AMap.GetValue(Key));
//    end;
//  {$IFDEF THREADSAFE}
//  finally
//    if FThreadSafe then
//      SyncReaderWriter.EndWrite;
//  end;
//  {$ENDIF THREADSAFE}
//end;

procedure TJclGUIDHashMap.PutValue(const Key: TGUID; Value: TObject);
var
  Index: Integer;
  Bucket: TJclGUIDBucket;
  I: Integer;
begin
  if ReadOnly then
    raise EJclReadOnlyError.Create;
  {$IFDEF THREADSAFE}
  if FThreadSafe then
    SyncReaderWriter.BeginWrite;
  try
  {$ENDIF THREADSAFE}
    if FAllowDefaultElements or (not KeysEqual(Key, GUID_NULL) and not ValuesEqual(Value, nil)) then
    begin
      Index := FHashFunction(Hash(Key), FCapacity);
      Bucket := FBuckets[Index];
      if Bucket <> nil then
      begin
        for I := 0 to Bucket.Size - 1 do
          if KeysEqual(Bucket.Entries[I].Key, Key) then
          begin
            FreeValue(Bucket.Entries[I].Value);
            Bucket.Entries[I].Value := Value;
            Exit;
          end;
      end
      else
      begin
        Bucket := TJclGUIDBucket.Create;
        SetLength(Bucket.Entries, 1);
        FBuckets[Index] := Bucket;
      end;

      if Bucket.Size = Length(Bucket.Entries) then
        SetLength(Bucket.Entries, CalcGrowCapacity(Bucket.Size, Bucket.Size));

      if Bucket.Size < Length(Bucket.Entries) then
      begin
        Bucket.Entries[Bucket.Size].Key := Key;
        Bucket.Entries[Bucket.Size].Value := Value;
        Inc(Bucket.Size);
        Inc(FSize);
      end;
    end;
  {$IFDEF THREADSAFE}
  finally
    if FThreadSafe then
      SyncReaderWriter.EndWrite;
  end;
  {$ENDIF THREADSAFE}
end;

function TJclGUIDHashMap.Remove(const Key: TGUID): TObject;
var
  Bucket: TJclGUIDBucket;
  I, NewCapacity: Integer;
begin
  if ReadOnly then
    raise EJclReadOnlyError.Create;
  {$IFDEF THREADSAFE}
  if FThreadSafe then
    SyncReaderWriter.BeginWrite;
  try
  {$ENDIF THREADSAFE}
    Result := nil;
    Bucket := FBuckets[FHashFunction(Hash(Key), FCapacity)];
    if Bucket <> nil then
    begin
      for I := 0 to Bucket.Size - 1 do
        if KeysEqual(Bucket.Entries[I].Key, Key) then
        begin
          Result := FreeValue(Bucket.Entries[I].Value);
          FreeKey(Bucket.Entries[I].Key);
          if I < Length(Bucket.Entries) - 1 then
            MoveArray(Bucket.Entries, I + 1, I, Bucket.Size - I - 1);
          Dec(Bucket.Size);
          Dec(FSize);
          Break;
        end;

      NewCapacity := CalcPackCapacity(Length(Bucket.Entries), Bucket.Size);
      if NewCapacity < Length(Bucket.Entries) then
        SetLength(Bucket.Entries, NewCapacity);
    end;
  {$IFDEF THREADSAFE}
  finally
    if FThreadSafe then
      SyncReaderWriter.EndWrite;
  end;
  {$ENDIF THREADSAFE}
end;

procedure TJclGUIDHashMap.SetCapacity(Value: Integer);
begin
  if ReadOnly then
    raise EJclReadOnlyError.Create;
  {$IFDEF THREADSAFE}
  if FThreadSafe then
    SyncReaderWriter.BeginWrite;
  try
  {$ENDIF THREADSAFE}
    if FSize = 0 then
    begin
      SetLength(FBuckets, Value);
      inherited SetCapacity(Value);
    end
    else
      raise EJclOperationNotSupportedError.Create;
  {$IFDEF THREADSAFE}
  finally
    if FThreadSafe then
      SyncReaderWriter.EndWrite;
  end;
  {$ENDIF THREADSAFE}
end;

function TJclGUIDHashMap.Size: Integer;
begin
  Result := FSize;
end;

function TJclGUIDHashMap.Values: IJclCollection;
var
  I, J: Integer;
  Bucket: TJclGUIDBucket;
begin
  {$IFDEF THREADSAFE}
  if FThreadSafe then
    SyncReaderWriter.BeginRead;
  try
  {$ENDIF THREADSAFE}
    Result := TJclArrayList.Create(FSize, False);
    for I := 0 to FCapacity - 1 do
    begin
      Bucket := FBuckets[I];
      if Bucket <> nil then
        for J := 0 to Bucket.Size - 1 do
          Result.Add(Bucket.Entries[J].Value);
    end;
  {$IFDEF THREADSAFE}
  finally
    if FThreadSafe then
      SyncReaderWriter.EndRead;
  end;
  {$ENDIF THREADSAFE}
end;

function TJclGUIDHashMap.ValuesEqual(A, B: TObject): Boolean;
begin
  Result := Integer(A) = Integer(B);
end;

end.
