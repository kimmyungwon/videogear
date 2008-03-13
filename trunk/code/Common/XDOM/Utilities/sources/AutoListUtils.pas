unit AutoListUtils;

// AutoListUtils 1.0.2
// Delphi 4 to 2007 and Kylix 3 Implementation
// December 2007
//
//
// LICENSE
//
// The contents of this file are subject to the Mozilla Public License Version
// 1.1 (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at
// "http://www.mozilla.org/MPL/"
//
// Software distributed under the License is distributed on an "AS IS" basis,
// WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
// the specific language governing rights and limitations under the License.
//
// The Original Code is "AutoListUtils.pas".
//
// The Initial Developer of the Original Code is Dieter Köhler (Heidelberg,
// Germany, "http://www.philo.de/"). Portions created by the Initial Developer
// are Copyright (C) 2003-2007 Dieter Köhler. All Rights Reserved.
//
// Alternatively, the contents of this file may be used under the terms of the
// GNU General Public License Version 2 or later (the "GPL"), in which case the
// provisions of the GPL are applicable instead of those above. If you wish to
// allow use of your version of this file only under the terms of the GPL, and
// not to allow others to use your version of this file under the terms of the
// MPL, indicate your decision by deleting the provisions above and replace them
// with the notice and other provisions required by the GPL. If you do not delete
// the provisions above, a recipient may use your version of this file under the
// terms of any one of the MPL or the GPL.

// HISTORY
//
// 2007-12-03 1.0.2 Made .NET compatible. Some functions and parameters renamed.
// 2003-09-20 1.0.1 Minor bug fixes.
// 2003-08-03 1.0.0

{$HINTS OFF}

interface

uses
  Contnrs, SysUtils, Classes;

type

  { Exception classes }

  EAutoListException = class(Exception);

  { Event prototypes}

  TUtilsPositionChangeEvent = procedure(Sender: TObject; OldPos, NewPos:
    Integer) of object;

  { Forward class declarations }

  TUtilsCustomIteratorAliases = class;
  TUtilsCustomIterator = class;
  TUtilsIteratorAdapter = class;

  // ---------- Basic List and Iterator Classes ----------

  { Bookmark classes }

  TUtilsBookmark = class
  private
    FNoChangeSinceMark: Boolean;
    FPosition: Integer;
  public
    constructor Create(APosition: Integer);
    property NoChangeSinceMark: Boolean read FNoChangeSinceMark write
      FNoChangeSinceMark default True;
    property Position: Integer read FPosition;
  end;

  TUtilsBookmarkStack = class(TObjectStack)
  private
    function GetNoChangeSinceMark: Boolean;
  public
    destructor Destroy; override;
    procedure NotiyChange;
    procedure Push(APosition: Integer);
    procedure Pop;
    function PeekPosition: Integer;
    property NoChangeSinceMark: Boolean read GetNoChangeSinceMark;
  end;

  { TUtilsCustomIteratorList }

  TUtilsCustomIteratorList = class(TComponent)
  private
    FAliases: TUtilsCustomIteratorAliases;
    FIterators: TList;
    FEnabled: Boolean;
    function GetHasIterators: Boolean;
    function GetIterators: TList;
    procedure RegisterIterator(Iterator: TUtilsCustomIterator);
    procedure UnregisterIterator(Iterator: TUtilsCustomIterator);
  protected
    procedure DoAliasChanged; virtual;
    procedure DoNotifyAliasChanged;
    procedure DoNotifyStateChanged;
    procedure DoNotifyClear;
    procedure DoNotifyDelete(Index: Integer);
    procedure DoNotifyDisableControls;
    procedure DoNotifyEnableControls;
    procedure DoNotifyExchange(Index1, Index2: Integer);
    procedure DoNotifyInsert(Index: Integer);
    procedure DoNotifyValueChanged(Index: Integer);
    function GetCount: Integer; virtual; abstract;
    function GetIsActive: Boolean; virtual;
    procedure SetAliases(const Value: TUtilsCustomIteratorAliases); virtual;
    procedure SetEnabled(Value: Boolean); virtual;

    property Aliases: TUtilsCustomIteratorAliases read FAliases write
      SetAliases;
    property Count: Integer read GetCount;
    property Enabled: Boolean read FEnabled write SetEnabled default True;
    property HasIterators: Boolean read GetHasIterators;
    property Iterators: TList read GetIterators;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    property IsActive: Boolean read GetIsActive;
  end;

  { TUtilsCustomIterator }

  TUtilsCustomIterator = class(TComponent)
  private
    FAdapters: TList;
    FBookmarkStack: TUtilsBookmarkStack;
    FCount: Integer;
    FFollowItem: Boolean;
    FItemChanged: Boolean;
    FList: TUtilsCustomIteratorList;
    FListModified: Boolean;
    FOldCount: Integer;
    FOldPos: Integer;
    FOnPositionChanged: TUtilsPositionChangeEvent;
    FOnItemChanged: TNotifyEvent;
    FOnListModified: TNotifyEvent;
    FPosition: Integer;
    FUpdateCount: Integer;
    procedure AddAdapter(Adapter: TUtilsIteratorAdapter);
    function GetAdapters: TList;
    function GetHasAdapters: Boolean;
    procedure RemoveAdapter(Adapter: TUtilsIteratorAdapter);
  protected
    procedure DoItemChanged; virtual;
    procedure DoListModified; virtual;
    procedure DoPositionChanged(const OldPos,
      NewPos: Integer); virtual;
    function GetList: TUtilsCustomIteratorList;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation); override;
    procedure NotifyAliasChanged; virtual;
    procedure NotifyStateChanged; virtual;
    procedure NotifyClear; virtual;
    procedure NotifyDelete(const Index: Integer); virtual;
    procedure NotifyDisable; virtual;
    procedure NotifyEnable; virtual;
    procedure NotifyExchange(const Index1, Index2: Integer); virtual;
    procedure NotifyInsert(const Index: Integer); virtual;
    procedure NotifyValueChanged(const Index: Integer); virtual;
    procedure SetList(Value: TUtilsCustomIteratorList);
    procedure SetUpdateState(Updating: Boolean); virtual;
    procedure UpdateIterator(const NewPos: Integer;
      const ItemChanged,
      ListModified: Boolean); virtual;

    property Adapters: TList read GetAdapters;
    property HasAdapters: Boolean read GetHasAdapters;
    property List: TUtilsCustomIteratorList read GetList write SetList;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    // Moving:
    procedure First; virtual;
    procedure Last; virtual;
    procedure Prior; virtual;
    procedure Next; virtual;
    procedure Go(const Index: Integer); virtual;
    // Bookmarking:
    procedure Mark; virtual;
    procedure Return; virtual;
    // Event flow control:
    procedure BeginUpdate; virtual;
    procedure EndUpdate; virtual;
    // Iterator Position Inquiring:
    function Position: Integer; virtual;
    function IsFirst: Boolean; virtual;
    function IsLast: Boolean; virtual;
    function Offleft: Boolean; virtual;
    function Offright: Boolean; virtual;
    function Offlist: Boolean; virtual;
    // Inquiring general list attributes:
    function Count: Integer; virtual;
    function Empty: Boolean; virtual;

    property UpdateCount: Integer read FUpdateCount;
  published
    property FollowItem: Boolean read FFollowItem write FFollowItem default
      True;
    //   If FollowItem is true, operations behave has follows:
    //
    //         Exchange: If the item at the current position is involved,
    //                   the position follows the item.
    //           Delete: If an item before the current position is deleted,
    //                   the position is decremented.
    //                   If the item at the current position is deleted,
    //                   the position is set offleft.
    //     InsertBefore: If an item is inserted somewhere before the current
    //                   position, the position is incremented.
    //
    //   If FollowItem is false, operations behave has follows:
    //
    //         Exchange: If the item at the current position is involved,
    //                   the position nevertheless remains the same.
    //           Delete: If the position is offright, it remains offright
    //                   while the position is decremented.
    //                   If the position is not offright and an item before
    //                   or at the current position is deleted,
    //                   the position nevertheless remains the same.
    //     InsertBefore: If an item is inserted somewhere before the current
    //                   position, the position nevertheless remains the same.

  // Events:
    property OnPositionChanged: TUtilsPositionChangeEvent read FOnPositionChanged
      write FOnPositionChanged;
    property OnItemChanged: TNotifyEvent read FOnItemChanged write
      FOnItemChanged;
    property OnListModified: TNotifyEvent read FOnListModified write
      FOnListModified;
    // The OnPositionChanged event is triggered whenever Position has changed.
    // The OnItemChanged event is triggered whenever the value or the item
    // itself has changed.
    // The OnListModified event is triggered whenever an item in the list was
    // added, deleted or modified.
    // If an operation triggers more than one of this events, they are called
    // in the following sequence:
    //  - OnListModified
    //  - OnPositionChanged
    //  - OnItemChanged
    // Event calls can be suppressed by calling BeginUpdate which increases
    // UpdateCount by one.  As long as UpdateCount is not 0, no event is
    // triggered.  If UpdateCount is set to 0 by calling EndUpdate, an
    // OnListModified event is triggered if an item in the list was added,
    // deleted or modified since the last time UpdateCount was 0.  Likewise,
    // an OnPositionChanged event is triggered if the the Position has
    // changed since the last time UpdateCount was 0.  And an OnItemChanged
    // event is triggered, when the the item was at least changed once since
    // the last time UpdateCount was 0 (no matter whether the item was changed
    // back to its original state if it was repeatedly changed, i.e. an
    // OnItemChanged event is triggered anyway in this case.)
  end;

  // ---------- Object List and Iterator Classes ----------

  { TUtilsCustomObjectIteratorList }

  TUtilsCustomObjectIteratorList = class(TUtilsCustomIteratorList)
  private
    FDuplicates: TDuplicates;
    FReadOnly: Boolean;
    FSorted: Boolean;
    procedure QuickSort(L, R: Integer);
    procedure SetSorted(Value: Boolean);
    function GetCapacity: Integer;
    function GetObject(Index: Integer): TObject;
    procedure PutObject(Index: Integer; const Value: TObject);
    procedure SetCapacity(const Value: Integer);
    procedure SetReadOnly(const Value: Boolean);
  protected
    FItems: TList;
    procedure CheckCanModify;
    function DoCompare(const Object1, Object2: TObject): Integer; virtual;
    function Find(const AObject: TObject;
      var Index: Integer): Boolean; virtual;
    function GetCanModify: Boolean; virtual;
    function GetCount: Integer; override;

    // List mutation:
    function Add(const AObject: TObject): Integer; virtual;
    procedure Clear; virtual;
    procedure Delete(Index: Integer); virtual;
    procedure DeleteAllOccurences(const AObject: TObject); virtual;
    function Insert(Index: Integer; const AObject: TObject): Boolean; virtual;
    // Sorting:
    procedure Exchange(Index1, Index2: Integer); virtual;
    procedure Sort; virtual;
    // Access:
    function HasObject(const AObject: TObject): Boolean; virtual;
    function IndexOf(const AObject: TObject; I: Integer): Integer; virtual;

    property ReadOnly: Boolean read FReadOnly write SetReadOnly;
    property Capacity: Integer read GetCapacity write SetCapacity;
    property Duplicates: TDuplicates read FDuplicates write FDuplicates;
    property Objects[Index: Integer]: TObject read GetObject write PutObject;
    property Sorted: Boolean read FSorted write SetSorted;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    property CanModify: Boolean read GetCanModify;
  end;

  // ---------- String List and Iterator Classes ----------

  TUtilsStringCompare = function(const S1, S2: string): Integer;

  TUtilsAliasOpt = (aoOff, aoFull, aoPrefix);

  { TUtilsCustomStringIteratorList }

  TUtilsCustomStringIteratorList = class(TUtilsCustomIteratorList)
  private
    FCaseSensitive: Boolean;
    FDuplicates: TDuplicates;
    FItems: TStringList;
    FReadOnly: Boolean;
    FSorted: Boolean;
    procedure QuickSort(L, R: Integer; SCompare: TUtilsStringCompare);
    procedure SetSorted(Value: Boolean);
    function GetCapacity: Integer;
    function GetObject(Index: Integer): TObject;
    procedure PutObject(Index: Integer; const Value: TObject);
    procedure SetCapacity(const Value: Integer);
    procedure SetReadOnly(const Value: Boolean);
    procedure SetCaseSensitive(const Value: Boolean);
  protected
    procedure CheckCanModify;
    function CustomFind(const S: WideString; SCompare: TUtilsStringCompare;
      var Index: Integer): Boolean; virtual;
    function CustomIndexOf(const S: string; SCompare: TUtilsStringCompare;
      I: Integer): Integer; virtual;
    function Find(const S: WideString;
      var Index: Integer): Boolean; virtual;
    function Get(Index: Integer): string; virtual;
    function GetCanModify: Boolean; virtual;
    function GetCount: Integer; override;
    procedure Put(Index: Integer; const Value: string); virtual;

    // List mutation:
    function Add(const S: string): Integer; virtual;
    function AddObject(const S: string; AObject: TObject): Integer; virtual;
    procedure Clear; virtual;
    procedure Delete(Index: Integer); virtual;
    procedure DeleteAllOccurences(const S: string); virtual;
    function Insert(Index: Integer; const S: string): Boolean; virtual;
    function InsertObject(Index: Integer; const S: string; AObject: TObject):
      Boolean; virtual;
    // Sorting:
    procedure CustomSort(Compare: TUtilsStringCompare); virtual;
    procedure Exchange(Index1, Index2: Integer); virtual;
    procedure Sort; virtual;
    // Access:
    function HasStr(const S: string): Boolean; virtual;
    function IndexOf(const S: string; I: Integer): Integer; virtual;

    property ReadOnly: Boolean read FReadOnly write SetReadOnly;
    property Capacity: Integer read GetCapacity write SetCapacity;
    property CaseSensitive: Boolean read FCaseSensitive write SetCaseSensitive;
    property Duplicates: TDuplicates read FDuplicates write FDuplicates;
    property Objects[Index: Integer]: TObject read GetObject write PutObject;
    property Sorted: Boolean read FSorted write SetSorted;
    property Strings[Index: Integer]: string read Get write Put; default;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    property CanModify: Boolean read GetCanModify;
  end;

  { TUtilsCustomAliasedStrings }

  TUtilsCustomAliasedStrings = class(TUtilsCustomStringIteratorList)
  private
    FAliasOpt: TUtilsAliasOpt;
  protected
    procedure DoAliasChanged; override;
    function EscapeStr(S: string): string; virtual;
    function Get(Index: Integer): string; override;
    function GetLiteral(Index: Integer): string; virtual;
    function InsertObject(Index: Integer; const S: string; AObject: TObject):
      Boolean; override;
    procedure Put(Index: Integer; const Value: string); override;
    procedure PutLiteral(Index: Integer; const Value: string); virtual;
    procedure SetAliasOpt(const Value: TUtilsAliasOpt); virtual;
    function UnescapeStr(S: string): string; virtual;

    // List mutation via literal value:
    function AddLiteral(const S: string): Integer; virtual;
    function AddLiteralObject(const S: string; AObject: TObject): Integer;
      virtual;
    function AddObject(const S: string; AObject: TObject): Integer; override;
    procedure DeleteAllLiteralOccurences(const S: string); virtual;
    procedure DeleteAllOccurences(const S: string); override;
    function InsertLiteral(Index: Integer; const S: string): Boolean; virtual;
    function InsertLiteralObject(Index: Integer; const S: string; AObject:
      TObject): Boolean; virtual;
    // Access via literal value:
    function HasLiteralStr(const S: string): Boolean; virtual;
    function IndexOfLiteral(const S: string; I: Integer): Integer; virtual;

    property AliasOpt: TUtilsAliasOpt read FAliasOpt write SetAliasOpt default
      aoOff;
    property Literals[Index: Integer]: string read GetLiteral write PutLiteral;
  end;

  { TUtilsAutoStrings }

  TUtilsAutoStrings = class(TUtilsCustomAliasedStrings)
  public
    // List mutation:
    function Add(const S: string): Integer; override;
    function AddObject(const S: string; AObject: TObject): Integer; override;
    procedure Clear; override;
    procedure Delete(Index: Integer); override;
    procedure DeleteAllOccurences(const S: string); override;
    function Insert(Index: Integer; const S: string): Boolean; override;
    function InsertObject(Index: Integer; const S: string; AObject: TObject):
      Boolean; override;
    // List mutation via literal value:
    function AddLiteral(const S: string): Integer; override;
    function AddLiteralObject(const S: string; AObject: TObject): Integer;
      override;
    procedure DeleteAllLiteralOccurences(const S: string); override;
    function InsertLiteral(Index: Integer; const S: string): Boolean; override;
    function InsertLiteralObject(Index: Integer; const S: string; AObject:
      TObject): Boolean; override;
    // Sorting:
    procedure CustomSort(Compare: TUtilsStringCompare); override;
    procedure Exchange(Index1, Index2: Integer); override;
    procedure Sort; override;
    // Access:
    function HasStr(const S: string): Boolean; override;
    function IndexOf(const S: string; I: Integer): Integer; override;
    // Access via literal value:
    function HasLiteralStr(const S: string): Boolean; override;
    function IndexOfLiteral(const S: string; I: Integer): Integer; override;
    // View Control:
    procedure DisableControls; virtual;
    procedure EnableControls; virtual;

    property Capacity;
    property CaseSensitive;
    property Count;
    property Literals;
    property Objects;
    property Strings;
  published
    property AliasOpt;
    property Aliases;
    property Duplicates;
    property Enabled;
    property Sorted;
  end;

  { TUtilsStringIterator }

  TUtilsStringIterator = class(TUtilsCustomIterator)
  private
    FLiteralValue: string;
    FValue: string;
  protected
    function GetCanModify: Boolean; virtual;
    function GetList: TUtilsCustomAliasedStrings;
    function GetLiteralValue: string; virtual;
    function GetValue: string; virtual;
    procedure SetList(Value: TUtilsCustomAliasedStrings);
    procedure UpdateIterator(const NewPos: Integer;
      const ItemChanged,
      ListModified: Boolean); override;
  public
    // Iterator Value Inquiring:
    property LiteralValue: string read GetLiteralValue;
    property Value: string read GetValue;
    property CanModify: Boolean read GetCanModify;
  published
    property List: TUtilsCustomAliasedStrings read GetList write SetList;
  end;

  // ---------- Adapter Classes ----------

  { TUtilsIteratorAdapter }

  TUtilsIteratorAdapter = class(TPersistent)
  private
    FIterator: TUtilsCustomIterator;
    FIteratorFixed: Boolean;
    FOnActiveChanged: TNotifyEvent;
    FOnPositionChanged: TUtilsPositionChangeEvent;
    FOnItemChanged: TNotifyEvent;
    FOnListModified: TNotifyEvent;
    procedure CheckIteratorFixed;
  protected
    procedure DoActiveChanged; virtual;
    function GetIterator: TUtilsCustomIterator;
    procedure HandleItemChanged(Sender: TObject);
    procedure HandleListModified(Sender: TObject);
    procedure HandlePositionChanged(Sender: TObject; OldPos, NewPos: Integer);
    procedure SetIterator(AIterator: TUtilsCustomIterator);
    property Iterator: TUtilsCustomIterator read GetIterator write SetIterator;
  public
    destructor Destroy; override;
    function ExecuteAction(Action: TBasicAction): Boolean; dynamic;
    function UpdateAction(Action: TBasicAction): Boolean; dynamic;

    property IteratorFixed: Boolean read FIteratorFixed write FIteratorFixed;

    property OnActiveChanged: TNotifyEvent read FOnActiveChanged write
      FOnActiveChanged;
    property OnPositionChanged: TUtilsPositionChangeEvent read FOnPositionChanged
      write FOnPositionChanged;
    property OnItemChanged: TNotifyEvent read FOnItemChanged write
      FOnItemChanged;
    property OnListModified: TNotifyEvent read FOnListModified write
      FOnListModified;
  end;

  { TUtilsStandardIteratorAdapter }

  TUtilsStandardIteratorAdapter = class(TUtilsIteratorAdapter)
  public
    property Iterator;
  end;

  { TUtilsStringIteratorAdapter }

  TUtilsStringIteratorAdapter = class(TUtilsIteratorAdapter)
  private
    function GetIterator: TUtilsStringIterator; reintroduce;
    procedure SetIterator(AIterator: TUtilsStringIterator); reintroduce;
  public
    property Iterator: TUtilsStringIterator read GetIterator write SetIterator;
  end;

  // ---------- Alias List Classes ----------

  { TUtilsCustomAliases }

  TUtilsCustomAliases = class(TComponent)
  private
    FAliasList: TStringList;
    FUpdateCount: Integer;
    FOnChange: TNotifyEvent;
    FOnChanging: TNotifyEvent;
    procedure CheckAlias(S: string);
    procedure CheckConsistency;
    procedure CheckKey(S: string);
    function GetAlias(Index: Integer): string;
    function GetCount: Integer;
    function GetKey(Index: Integer): string;
  protected
    procedure DoChange; virtual;
    procedure DoChanging; virtual;
    procedure SetUpdateState(Updating: Boolean); virtual;
    property UpdateCount: Integer read FUpdateCount;

    procedure BeginUpdate; virtual;
    procedure Clear; virtual;
    procedure Define(Key,
      Alias: string); virtual;
    procedure EndUpdate; virtual;
    procedure LoadFromFile(const FileName: string); virtual;
    procedure LoadFromStream(Stream: TStream); virtual;
    function RemoveAlias(S: string): Boolean; virtual;
    function RemoveKey(S: string): Boolean; virtual;
    procedure SaveToFile(const FileName: string); virtual;
    procedure SaveToStream(Stream: TStream); virtual;

    property OnChange: TNotifyEvent read FOnChange write FOnChange;
    property OnChanging: TNotifyEvent read FOnChanging write FOnChanging;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

    // Alias Definition Access:
    function HasAlias(S: string): Boolean; virtual;
    function HasKey(S: string): Boolean; virtual;
    function IndexOfAlias(S: string): Integer; virtual;
    function IndexOfKey(S: string): Integer; virtual;

    // String Escaping:
    function EscapeStr(S: string; Opt: TUtilsAliasOpt): string; virtual;
    function UnescapeStr(S: string; Opt: TUtilsAliasOpt): string; virtual;

    property Count: Integer read GetCount;
    property Alias[Index: Integer]: string read GetAlias;
    property Key[Index: Integer]: string read GetKey;
  end;

  { TUtilsCustomIteratorAliases }

  TUtilsCustomIteratorAliases = class(TUtilsCustomAliases)
  private
    FIteratorLists: TList;
    procedure AddIteratorList(IteratorList: TUtilsCustomIteratorList);
    procedure RemoveIteratorList(IteratorList: TUtilsCustomIteratorList);
  protected
    procedure DoChange; override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
  end;

  { TUtilsAutoAliases }

  TUtilsAutoAliases = class(TUtilsCustomIteratorAliases)
  public
    procedure BeginUpdate; override;
    procedure Clear; override;
    procedure Define(Key,
      Alias: string); override;
    procedure EndUpdate; override;
    procedure LoadFromFile(const FileName: string); override;
    procedure LoadFromStream(Stream: TStream); override;
    function RemoveAlias(S: string): Boolean; override;
    function RemoveKey(S: string): Boolean; override;
    procedure SaveToFile(const FileName: string); override;
    procedure SaveToStream(Stream: TStream); override;
  published
    property OnChange;
    property OnChanging;
  end;

  { Global methods }

procedure AutoListError(Msg: string; Data: Integer);

resourcestring
  SChangeSinceMark = 'Change since last mark';
  SIndexOutOfBounds = 'Index out of bounds (%d)';
  SInvalidAbbr = 'Invalid abbreviation';
  SNoDuplicatesAllowed = 'List does not allow duplicates';
  SNoMarkAvailable = 'No mark available';
  SNoModification = 'No modification allowed';

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('AutoStr', [TUtilsStringIterator,
    TUtilsAutoStrings,
      TUtilsAutoAliases]);
end;

procedure AutoListError(Msg: string; Data: Integer);
begin
  raise EAutoListException.CreateFmt(Msg, [Data]);
end;

{ TUtilsBookmark }

constructor TUtilsBookmark.Create(APosition: Integer);
begin
  inherited Create;
  FPosition := APosition;
  FNoChangeSinceMark := True;
end;

{ TUtilsBookmarkStack }

destructor TUtilsBookmarkStack.Destroy;
begin
  while Count > 0 do
    Pop;
  inherited;
end;

function TUtilsBookmarkStack.GetNoChangeSinceMark: Boolean;
begin
  Result := TUtilsBookmark(inherited Peek).NoChangeSinceMark;
end;

procedure TUtilsBookmarkStack.NotiyChange;
begin
  if Count > 0 then
    TUtilsBookmark(inherited Peek).NoChangeSinceMark := False;
end;

function TUtilsBookmarkStack.PeekPosition: Integer;
begin
  Result := TUtilsBookmark(inherited Peek).Position;
end;

procedure TUtilsBookmarkStack.Pop;
begin
  TUtilsBookmark(inherited Pop).Free
end;

procedure TUtilsBookmarkStack.Push(APosition: Integer);
begin
  inherited Push(TUtilsBookmark.Create(APosition));
end;

{ TUtilsCustomIteratorList }

constructor TUtilsCustomIteratorList.Create(AOwner: TComponent);
begin
  inherited;
  FIterators := TList.Create;
  Enabled := True;
end;

destructor TUtilsCustomIteratorList.Destroy;
begin
  SetAliases(nil);
  with FIterators do
    while Count > 0 do
      UnregisterIterator(TUtilsCustomIterator(Last));
  FIterators.Free;
  inherited;
end;

procedure TUtilsCustomIteratorList.DoAliasChanged;
begin
  DoNotifyAliasChanged;
end;

procedure TUtilsCustomIteratorList.DoNotifyAliasChanged;
var
  I: Integer;
begin
  if HasIterators then
    for I := Pred(Iterators.Count) downto 0 do
      TUtilsCustomIterator(Iterators[I]).NotifyAliasChanged;
end;

procedure TUtilsCustomIteratorList.DoNotifyStateChanged;
var
  I: Integer;
begin
  if HasIterators then
    for I := Pred(Iterators.Count) downto 0 do
      TUtilsCustomIterator(Iterators[I]).NotifyStateChanged;
end;

procedure TUtilsCustomIteratorList.DoNotifyClear;
var
  I: Integer;
begin
  if HasIterators then
    for I := Pred(Iterators.Count) downto 0 do
      TUtilsCustomIterator(Iterators[I]).NotifyClear;
end;

procedure TUtilsCustomIteratorList.DoNotifyDelete(Index: Integer);
var
  I: Integer;
begin
  if HasIterators then
    for I := Pred(Iterators.Count) downto 0 do
      TUtilsCustomIterator(Iterators[I]).NotifyDelete(Index);
end;

procedure TUtilsCustomIteratorList.DoNotifyDisableControls;
var
  I: Integer;
begin
  if HasIterators then
    for I := Pred(Iterators.Count) downto 0 do
      TUtilsCustomIterator(Iterators[I]).NotifyDisable;
end;

procedure TUtilsCustomIteratorList.DoNotifyEnableControls;
var
  I: Integer;
begin
  if HasIterators then
    for I := Pred(Iterators.Count) downto 0 do
      TUtilsCustomIterator(Iterators[I]).NotifyEnable;
end;

procedure TUtilsCustomIteratorList.DoNotifyExchange(Index1, Index2: Integer);
var
  I: Integer;
begin
  if HasIterators then
    for I := Pred(Iterators.Count) downto 0 do
      TUtilsCustomIterator(Iterators[I]).NotifyExchange(Index1, Index2);
end;

procedure TUtilsCustomIteratorList.DoNotifyInsert(Index: Integer);
var
  I: Integer;
begin
  if HasIterators then
    for I := Pred(Iterators.Count) downto 0 do
      TUtilsCustomIterator(Iterators[I]).NotifyInsert(Index);
end;

procedure TUtilsCustomIteratorList.DoNotifyValueChanged(Index: Integer);
var
  I: Integer;
begin
  if HasIterators then
    for I := Pred(Iterators.Count) downto 0 do
      TUtilsCustomIterator(Iterators[I]).NotifyValueChanged(Index);
end;

function TUtilsCustomIteratorList.GetHasIterators: Boolean;
begin
  Result := FIterators.Count > 0;
end;

function TUtilsCustomIteratorList.GetIterators: TList;
begin
  Result := FIterators;
end;

function TUtilsCustomIteratorList.GetIsActive: Boolean;
begin
  Result := Enabled;
end;

procedure TUtilsCustomIteratorList.RegisterIterator(
  Iterator: TUtilsCustomIterator);
begin
  FIterators.Add(Iterator);
  Iterator.FList := Self;
  Iterator.NotifyStateChanged;
end;

procedure TUtilsCustomIteratorList.SetAliases(
  const Value: TUtilsCustomIteratorAliases);
begin
  if FAliases <> Value then
  begin
    if Assigned(FAliases) then
      FAliases.RemoveIteratorList(Self);
    if Assigned(Value) then
      Value.AddIteratorList(Self);
    DoAliasChanged;
  end;
end;

procedure TUtilsCustomIteratorList.SetEnabled(Value: Boolean);
begin
  if FEnabled <> Value then
  begin
    FEnabled := Value;
    DoNotifyStateChanged;
  end;
end;

procedure TUtilsCustomIteratorList.UnregisterIterator(
  Iterator: TUtilsCustomIterator);
begin
  Iterator.FList := nil;
  FIterators.Remove(Iterator);
  Iterator.NotifyStateChanged;
end;

{ TUtilsCustomIterator }

procedure TUtilsCustomIterator.AddAdapter(Adapter: TUtilsIteratorAdapter);
begin
  FAdapters.Add(Adapter);
  Adapter.FIterator := Self;
  Adapter.DoActiveChanged;
end;

procedure TUtilsCustomIterator.BeginUpdate;
begin
  if FUpdateCount = 0 then
    SetUpdateState(True);
  Inc(FUpdateCount);
end;

function TUtilsCustomIterator.Count: Integer;
begin
  Result := FCount;
end;

constructor TUtilsCustomIterator.Create(AOwner: TComponent);
begin
  inherited;
  FBookmarkStack := TUtilsBookmarkStack.Create;
  FFollowItem := True;
  FAdapters := TList.Create;
end;

destructor TUtilsCustomIterator.Destroy;
begin
  FOnPositionChanged := nil;
  FOnItemChanged := nil;
  FOnListModified := nil;
  SetList(nil);
  while FAdapters.Count > 0 do
    RemoveAdapter(TUtilsIteratorAdapter(FAdapters.Last));
  FAdapters.Free;
  FBookmarkStack.Free;
  inherited;
end;

procedure TUtilsCustomIterator.DoItemChanged;
var
  I: Integer;
begin
  if UpdateCount = 0 then
  begin
    if HasAdapters then
      for I := Pred(Adapters.Count) downto 0 do
        TUtilsIteratorAdapter(Adapters[I]).HandleItemChanged(Self);
    if Assigned(FOnItemChanged) then
      FOnItemChanged(Self);
  end
  else
    FItemChanged := True;
end;

procedure TUtilsCustomIterator.DoListModified;
var
  I: Integer;
begin
  if UpdateCount = 0 then
  begin
    if HasAdapters then
      for I := Pred(Adapters.Count) downto 0 do
        TUtilsIteratorAdapter(Adapters[I]).HandleListModified(Self);
    if Assigned(FOnListModified) then
      FOnListModified(Self);
  end
  else
    FListModified := True;
end;

procedure TUtilsCustomIterator.DoPositionChanged(const OldPos, NewPos: Integer);
var
  I: Integer;
begin
  if UpdateCount = 0 then
    if OldPos <> NewPos then
    begin
      if HasAdapters then
        for I := Pred(Adapters.Count) downto 0 do
          TUtilsIteratorAdapter(Adapters[I]).HandlePositionChanged(Self, OldPos,
            NewPos);
      if Assigned(FOnPositionChanged) then
        FOnPositionChanged(Self, OldPos, NewPos);
    end;
end;

function TUtilsCustomIterator.Empty: Boolean;
begin
  Result := Count = 0;
end;

procedure TUtilsCustomIterator.EndUpdate;
begin
  Dec(FUpdateCount);
  if FUpdateCount = 0 then
    SetUpdateState(False);
end;

procedure TUtilsCustomIterator.First;
begin
  if Count > 0 then
    Go(0);
end;

function TUtilsCustomIterator.GetAdapters: TList;
begin
  Result := FAdapters;
end;

function TUtilsCustomIterator.GetHasAdapters: Boolean;
begin
  Result := FAdapters.Count > 0;
end;

function TUtilsCustomIterator.GetList: TUtilsCustomIteratorList;
begin
  Result := FList;
end;

procedure TUtilsCustomIterator.Go(const Index: Integer);
begin
  if (Index < -1) or (Index > Count) then
    AutoListError(SIndexOutOfBounds, Index);
  if FPosition <> Index then
    UpdateIterator(Index, not (Offlist and ((Index = -1) or (Index = Count))),
      False);
end;

function TUtilsCustomIterator.IsFirst: Boolean;
begin
  Result := Position = 0;
end;

function TUtilsCustomIterator.IsLast: Boolean;
begin
  Result := Position = Pred(Count);
end;

procedure TUtilsCustomIterator.Last;
begin
  Go(Pred(Count));
end;

procedure TUtilsCustomIterator.Mark;
begin
  FBookmarkStack.Push(Position);
end;

procedure TUtilsCustomIterator.Next;
begin
  Go(Position + 1);
end;

procedure TUtilsCustomIterator.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (Operation = opRemove) and (AComponent = FList) then
  begin
    FList := nil;
    NotifyStateChanged;
  end;
end;

procedure TUtilsCustomIterator.NotifyAliasChanged;
begin
  UpdateIterator(Position, True, True);
end;

procedure TUtilsCustomIterator.NotifyStateChanged;
var
  NoItems: Boolean;
  NewPos: Integer;
begin
  if Assigned(List) then
  begin
    if List.Enabled then
      NoItems := List.Count = 0
    else
      NoItems := True;
    if List.IsActive then
      NewPos := Position
    else
      NewPos := -1;
  end
  else
  begin
    NoItems := True;
    NewPos := -1;
  end;
  // xxx Iterator currently makes no use of NoItems --> change this in next version!
  UpdateIterator(NewPos, True, True);
end;

procedure TUtilsCustomIterator.NotifyClear;
begin
  UpdateIterator(-1, not Offlist, FCount > 0);
end;

procedure TUtilsCustomIterator.NotifyDelete(const Index: Integer);
begin
  if (Index < 0) or (Index > Pred(Count)) then
    AutoListError(SIndexOutOfBounds, Index);
  if Offright then
  begin
    UpdateIterator(Position - 1, False, True);
  end
  else if Index < Position then
  begin
    if FollowItem then
      UpdateIterator(Position - 1, False, True)
    else
      UpdateIterator(Position, True, True);
  end
  else if Index = Position then
  begin
    if FollowItem then
      UpdateIterator(-1, True, True)
    else
      UpdateIterator(Position, True, True);
  end
  else
    UpdateIterator(Position, False, True);
end;

procedure TUtilsCustomIterator.NotifyDisable;
begin
  BeginUpdate;
end;

procedure TUtilsCustomIterator.NotifyEnable;
begin
  EndUpdate;
end;

procedure TUtilsCustomIterator.NotifyExchange(const Index1, Index2: Integer);
begin
  if (Index1 < 0) or (Index1 > Pred(Count)) then
    AutoListError(SIndexOutOfBounds, Index1);
  if (Index2 < 0) or (Index2 > Pred(Count)) then
    AutoListError(SIndexOutOfBounds, Index2);
  if Index1 = Index2 then
    exit;

  if Index1 = Position then
  begin
    if FollowItem then
      UpdateIterator(Index2, False, True)
    else
      UpdateIterator(Position, True, True);
  end
  else if Index2 = Position then
  begin
    if FollowItem then
      UpdateIterator(Index1, False, True)
    else
      UpdateIterator(Position, True, True);
  end
  else
    UpdateIterator(Position, False, True);
end;

procedure TUtilsCustomIterator.NotifyInsert(const Index: Integer);
begin
  if (Index < 0) or (Index > Count) then
    AutoListError(SIndexOutOfBounds, Index);
  if Index <= FPosition then
  begin
    if FollowItem then
      UpdateIterator(Position + 1, False, True)
    else
      UpdateIterator(Position, True, True);
  end
  else
    UpdateIterator(Position, False, True);
end;

procedure TUtilsCustomIterator.NotifyValueChanged(const Index: Integer);
begin
  UpdateIterator(Position, Index = Position, True);
end;

function TUtilsCustomIterator.Offleft: Boolean;
begin
  Result := Empty or (Position = -1);
end;

function TUtilsCustomIterator.Offlist: Boolean;
begin
  Result := Empty or (Position = -1) or (Position = Count);
end;

function TUtilsCustomIterator.Offright: Boolean;
begin
  Result := Empty or (Position = Count);
end;

function TUtilsCustomIterator.Position: Integer;
begin
  Result := FPosition;
end;

procedure TUtilsCustomIterator.Prior;
begin
  Go(Position - 1);
end;

procedure TUtilsCustomIterator.RemoveAdapter(Adapter: TUtilsIteratorAdapter);
begin
  Adapter.FIterator := nil;
  FAdapters.Remove(Adapter);
  Adapter.DoActiveChanged;
end;

procedure TUtilsCustomIterator.Return;
var
  Temp: Integer;
begin
  if FBookmarkStack.Count = 0 then
    AutoListError(SNoMarkAvailable, -1);
  if not FBookmarkStack.NoChangeSinceMark then
    AutoListError(SChangeSinceMark, -1);
  Temp := FBookmarkStack.PeekPosition;
  FBookmarkStack.Pop;
  Go(Temp); // Use Temp do avoid event calls before Pop.
end;

procedure TUtilsCustomIterator.SetList(Value: TUtilsCustomIteratorList);
begin
  if FList <> Value then
  begin
    if Assigned(FList) then
      FList.UnregisterIterator(Self);
    if Assigned(Value) then
    begin
      Value.RegisterIterator(Self);
      Value.FreeNotification(Self);
    end;
  end;
end;

procedure TUtilsCustomIterator.SetUpdateState(Updating: Boolean);
var
  NewPos: Integer;
begin
  if Updating then
  begin
    FItemChanged := False;
    FListModified := False;
    FOldCount := Count;
    FOldPos := Position;
  end
  else
  begin
    NewPos := Position;
    // Temporally switch to the status quo antea (UpdateIterator will switch back):
    FPosition := FOldPos;
    FCount := FOldCount;
    UpdateIterator(NewPos, FItemChanged, FListModified);
  end;
end;

procedure TUtilsCustomIterator.UpdateIterator(const NewPos: Integer;
  const ItemChanged, ListModified: Boolean);
var
  OldPos, OldCount: Integer;
begin
  OldPos := FPosition;
  FPosition := NewPos;
  OldCount := FCount;
  if Assigned(List) then
  begin
    if List.Enabled then
      FCount := List.Count
    else
      FCount := 0;
  end
  else
    FCount := 0;

  if ListModified and not ((OldCount = 0) and (FCount = 0)) then
    DoListModified;
  if FPosition <> OldPos then
    DoPositionChanged(OldPos, FPosition);
  if ItemChanged then
    DoItemChanged;
end;

{ TUtilsCustomObjectIteratorList }

function TUtilsCustomObjectIteratorList.Add(
  const AObject: TObject): Integer;
begin
  CheckCanModify;
  if not Sorted then
    Result := Count
  else if Find(AObject, Result) then
    case Duplicates of
      dupIgnore: Exit;
      dupError: AutoListError(SNoDuplicatesAllowed, 0);
    end;
  Insert(Result, AObject);
end;

procedure TUtilsCustomObjectIteratorList.CheckCanModify;
begin
  if not CanModify then
    AutoListError(SNoModification, -1);
end;

procedure TUtilsCustomObjectIteratorList.Clear;
begin
  CheckCanModify;
  FItems.Clear;
  DoNotifyClear;
end;

constructor TUtilsCustomObjectIteratorList.Create(AOwner: TComponent);
begin
  inherited;
  FItems := TList.Create;
end;

procedure TUtilsCustomObjectIteratorList.Delete(Index: Integer);
begin
  CheckCanModify;
  FItems.Delete(Index);
  DoNotifyDelete(Index);
end;

procedure TUtilsCustomObjectIteratorList.DeleteAllOccurences(
  const AObject: TObject);
var
  I: Integer;
begin
  CheckCanModify;
  I := Count;
  while I > 0 do
  begin
    Dec(I);
    if Objects[I] = AObject then
      Delete(I);
  end;
end;

destructor TUtilsCustomObjectIteratorList.Destroy;
begin
  FItems.Free;
  inherited;
end;

function TUtilsCustomObjectIteratorList.DoCompare(const Object1,
  Object2: TObject): Integer;
begin
  Result := 0; // By default treat the objects as identical.
end;

procedure TUtilsCustomObjectIteratorList.Exchange(Index1, Index2: Integer);
begin
  CheckCanModify;
  if (Index1 < 0) or (Index1 >= Count) then
    AutoListError(SIndexOutOfBounds, Index1);
  if (Index2 < 0) or (Index2 >= Count) then
    AutoListError(SIndexOutOfBounds, Index2);
  FItems.Exchange(Index1, Index2);
  DoNotifyExchange(Index1, Index2);
end;

function TUtilsCustomObjectIteratorList.Find(const AObject: TObject;
  var Index: Integer): Boolean;
var
  L, H, I, C: Integer;
begin
  Result := False;
  L := 0;
  H := Count - 1;
  while L <= H do
  begin
    I := (L + H) shr 1;
    C := DoCompare(Objects[I], AObject);
    if C < 0 then
      L := I + 1
    else
    begin
      H := I - 1;
      if C = 0 then
      begin
        Result := True;
        if Duplicates <> dupAccept then
          L := I;
      end;
    end;
  end;
  Index := L;
end;

function TUtilsCustomObjectIteratorList.GetCanModify: Boolean;
begin
  Result := not ReadOnly;
end;

function TUtilsCustomObjectIteratorList.GetCapacity: Integer;
begin
  Result := FItems.Capacity;
end;

function TUtilsCustomObjectIteratorList.GetCount: Integer;
begin
  Result := FItems.Count;
end;

function TUtilsCustomObjectIteratorList.GetObject(Index: Integer): TObject;
begin
  Result := TObject(FItems[Index]);
end;

function TUtilsCustomObjectIteratorList.HasObject(
  const AObject: TObject): Boolean;
begin
  Result := IndexOf(AObject, 1) >= 0;
end;

function TUtilsCustomObjectIteratorList.IndexOf(const AObject: TObject;
  I: Integer): Integer;
var
  J, K: Integer;
begin
  if I <= 0 then
    AutoListError(SIndexOutOfBounds, I);
  if Sorted then
  begin
    if Find(AObject, J) then
    begin
      K := 1;
      Inc(J);
      while (J < Count) and (K < I) and (DoCompare(Objects[J], AObject) = 0) do
      begin
        Inc(K);
        Inc(J);
      end;
      if K = I then
        Result := Pred(J)
      else
        Result := -1;
    end
    else
      Result := -1;
  end
  else
  begin
    J := 0;
    K := 0;
    while (J < Count) and (K < I) do
    begin
      if DoCompare(Objects[J], AObject) = 0 then
        Inc(K);
      Inc(J);
    end;
    if K = I then
      Result := Pred(J)
    else
      Result := -1;
  end;
end;

function TUtilsCustomObjectIteratorList.Insert(Index: Integer;
  const AObject: TObject): Boolean;
begin
  CheckCanModify;
  FItems.Insert(Index, AObject);
  Result := True;
  DoNotifyInsert(Index);
end;

procedure TUtilsCustomObjectIteratorList.PutObject(Index: Integer;
  const Value: TObject);
begin
  CheckCanModify;
  FItems[Index] := Value;
  DoNotifyValueChanged(Index);
end;

procedure TUtilsCustomObjectIteratorList.QuickSort(L, R: Integer);
var
  I, J: Integer;
  Obj: TObject;
begin
  repeat
    I := L;
    J := R;
    Obj := Objects[(L + R) shr 1];
    repeat
      while DoCompare(Objects[I], Obj) < 0 do
        Inc(I);
      while DoCompare(Objects[J], Obj) > 0 do
        Dec(J);
      if I <= J then
      begin
        Exchange(I, J);
        Inc(I);
        Dec(J);
      end;
    until I > J;
    if L < J then
      QuickSort(L, J);
    L := I;
  until I >= R;
end;

procedure TUtilsCustomObjectIteratorList.SetCapacity(const Value: Integer);
begin
  FItems.Capacity := Value;
end;

procedure TUtilsCustomObjectIteratorList.SetReadOnly(const Value: Boolean);
begin
  FReadOnly := Value;
end;

procedure TUtilsCustomObjectIteratorList.SetSorted(Value: Boolean);
begin
  CheckCanModify;
  if FSorted <> Value then
  begin
    if Value then
      Sort;
    FSorted := Value;
  end;
end;

procedure TUtilsCustomObjectIteratorList.Sort;
begin
  CheckCanModify;
  if not Sorted and (Count > 1) then
    QuickSort(0, Count - 1);
end;

{ TUtilsCustomStringIteratorList }

// This is a wrapper of Delphi's TStringList class which reimplements
// some methods to allow for a better control over the event flow.

function TUtilsCustomStringIteratorList.Add(const S: string): Integer;
begin
  Result := AddObject(S, nil);
end;

function TUtilsCustomStringIteratorList.AddObject(const S: string;
  AObject: TObject): Integer;
begin
  CheckCanModify;
  if not Sorted then
    Result := Count
  else if Find(S, Result) then
    case Duplicates of
      dupIgnore: Exit;
      dupError: AutoListError(SNoDuplicatesAllowed, 0);
    end;
  InsertObject(Result, S, AObject);
end;

procedure TUtilsCustomStringIteratorList.CheckCanModify;
begin
  if not CanModify then
    AutoListError(SNoModification, -1);
end;

procedure TUtilsCustomStringIteratorList.Clear;
begin
  CheckCanModify;
  FItems.Clear;
  DoNotifyClear;
end;

constructor TUtilsCustomStringIteratorList.Create(AOwner: TComponent);
begin
  inherited;
  FItems := TStringList.Create;
end;

function TUtilsCustomStringIteratorList.CustomFind(const S: WideString;
  SCompare: TUtilsStringCompare; var Index: Integer): Boolean;
var
  L, H, I, C: Integer;
begin
  Result := False;
  L := 0;
  H := Count - 1;
  while L <= H do
  begin
    I := (L + H) shr 1;
    C := SCompare(Strings[I], S);
    if C < 0 then
      L := I + 1
    else
    begin
      H := I - 1;
      if C = 0 then
      begin
        Result := True;
        if Duplicates <> dupAccept then
          L := I;
      end;
    end;
  end;
  Index := L;
end;

function TUtilsCustomStringIteratorList.CustomIndexOf(const S: string;
  SCompare: TUtilsStringCompare; I: Integer): Integer;
var
  J, K: Integer;
begin
  if I <= 0 then
    AutoListError(SIndexOutOfBounds, I);
  if Sorted then
  begin
    if CustomFind(S, SCompare, J) then
    begin
      K := 1;
      Inc(J);
      while (J < Count) and (K < I) and (SCompare(Strings[J], S) = 0) do
      begin
        Inc(K);
        Inc(J);
      end;
      if K = I then
        Result := Pred(J)
      else
        Result := -1;
    end
    else
      Result := -1;
  end
  else
  begin
    J := 0;
    K := 0;
    while (J < Count) and (K < I) do
    begin
      if SCompare(Strings[J], S) = 0 then
        Inc(K);
      Inc(J);
    end;
    if K = I then
      Result := Pred(J)
    else
      Result := -1;
  end;
end;

procedure TUtilsCustomStringIteratorList.CustomSort(
  Compare: TUtilsStringCompare);
begin
  CheckCanModify;
  if not Sorted and (Count > 1) then
    QuickSort(0, Count - 1, Compare);
end;

procedure TUtilsCustomStringIteratorList.Delete(Index: Integer);
begin
  CheckCanModify;
  FItems.Delete(Index);
  DoNotifyDelete(Index);
end;

procedure TUtilsCustomStringIteratorList.DeleteAllOccurences(
  const S: string);
var
  I: Integer;
begin
  CheckCanModify;
  I := Count;
  while I > 0 do
  begin
    Dec(I);
    if Strings[I] = S then
      Delete(I);
  end;
end;

destructor TUtilsCustomStringIteratorList.Destroy;
begin
  FItems.Free;
  inherited;
end;

procedure TUtilsCustomStringIteratorList.Exchange(Index1, Index2: Integer);
begin
  CheckCanModify;
  if (Index1 < 0) or (Index1 >= Count) then
    AutoListError(SIndexOutOfBounds, Index1);
  if (Index2 < 0) or (Index2 >= Count) then
    AutoListError(SIndexOutOfBounds, Index2);
  FItems.Exchange(Index1, Index2);
  DoNotifyExchange(Index1, Index2);
end;

{$IFDEF CLR}

function TUtilsCustomStringIteratorList.Find(const S: WideString;
  var Index: Integer): Boolean;
begin
  if CaseSensitive then
    Result := CustomFind(S, WideCompareStr, Index)
  else
    Result := CustomFind(S, WideCompareText, Index);
end;

{$ELSE}

function TUtilsCustomStringIteratorList.Find(const S: WideString;
  var Index: Integer): Boolean;
begin
  if CaseSensitive then
    Result := CustomFind(S, AnsiCompareStr, Index)
    { TODO 5 -cRefactoring : Change AnsiCompareStr to WideCompareStr? }
  else
    Result := CustomFind(S, AnsiCompareText, Index);
    { TODO 5 -cRefactoring : Change AnsiCompareText to WideCompareText? }
end;

{$ENDIF}

function TUtilsCustomStringIteratorList.Get(Index: Integer): string;
begin
  Result := FItems[Index];
end;

function TUtilsCustomStringIteratorList.GetCanModify: Boolean;
begin
  Result := not ReadOnly;
end;

function TUtilsCustomStringIteratorList.GetCapacity: Integer;
begin
  Result := FItems.Capacity;
end;

function TUtilsCustomStringIteratorList.GetCount: Integer;
begin
  Result := FItems.Count;
end;

function TUtilsCustomStringIteratorList.GetObject(Index: Integer): TObject;
begin
  Result := FItems.Objects[Index];
end;

function TUtilsCustomStringIteratorList.HasStr(const S: string): Boolean;
begin
  Result := IndexOf(S, 1) >= 0;
end;

function TUtilsCustomStringIteratorList.IndexOf(const S: string;
  I: Integer): Integer;
begin
{$IFDEF CLR}
  Result := CustomIndexOf(S, CompareStr, I);
{$ELSE}
  Result := CustomIndexOf(S, AnsiCompareStr, I);
  { TODO 5 -cRefactoring : Change AnsiCompareStr to CompareStr? }
{$ENDIF}
end;

function TUtilsCustomStringIteratorList.Insert(Index: Integer;
  const S: string): Boolean;
begin
  Result := InsertObject(Index, S, nil);
end;

function TUtilsCustomStringIteratorList.InsertObject(Index: Integer;
  const S: string; AObject: TObject): Boolean;
begin
  CheckCanModify;
  FItems.InsertObject(Index, S, AObject);
  Result := True;
  DoNotifyInsert(Index);
end;

procedure TUtilsCustomStringIteratorList.Put(Index: Integer;
  const Value: string);
begin
  CheckCanModify;
  FItems[Index] := Value;
  DoNotifyValueChanged(Index);
end;

procedure TUtilsCustomStringIteratorList.PutObject(Index: Integer;
  const Value: TObject);
begin
  CheckCanModify;
  FItems.Objects[Index] := Value;
  DoNotifyValueChanged(Index);
end;

procedure TUtilsCustomStringIteratorList.QuickSort(L, R: Integer;
  SCompare: TUtilsStringCompare);
var
  I, J: Integer;
  P: string;
begin
  repeat
    I := L;
    J := R;
    P := Strings[(L + R) shr 1];
    repeat
      while SCompare(Strings[I], P) < 0 do
        Inc(I);
      while SCompare(Strings[J], P) > 0 do
        Dec(J);
      if I <= J then
      begin
        Exchange(I, J);
        Inc(I);
        Dec(J);
      end;
    until I > J;
    if L < J then
      QuickSort(L, J, SCompare);
    L := I;
  until I >= R;
end;

procedure TUtilsCustomStringIteratorList.SetCapacity(const Value: Integer);
begin
  FItems.Capacity := Value;
end;

procedure TUtilsCustomStringIteratorList.SetCaseSensitive(
  const Value: Boolean);
begin
  CheckCanModify;
  if Value <> FCaseSensitive then
  begin
    FCaseSensitive := Value;
    if Sorted then
      Sort;
  end;
end;

procedure TUtilsCustomStringIteratorList.SetReadOnly(const Value: Boolean);
begin
  FReadOnly := Value;
end;

procedure TUtilsCustomStringIteratorList.SetSorted(Value: Boolean);
begin
  CheckCanModify;
  if FSorted <> Value then
  begin
    if Value then
      Sort;
    FSorted := Value;
  end;
end;

{$IFDEF CLR}

procedure TUtilsCustomStringIteratorList.Sort;
begin
  if CaseSensitive then
    CustomSort(CompareStr)
  else
    CustomSort(CompareText);
end;

{$ELSE}

procedure TUtilsCustomStringIteratorList.Sort;
begin
  if CaseSensitive then
    CustomSort(AnsiCompareStr)
    { TODO 5 -cRefactoring : Change AnsiCompareStr to CompareStr? }
  else
    CustomSort(AnsiCompareText);
    { TODO 5 -cRefactoring : Change AnsiCompareText to CompareText? }
end;

{$ENDIF}


{ TUtilsCustomAliasedStrings }

function TUtilsCustomAliasedStrings.AddLiteral(const S: string): Integer;
begin
  Result := AddLiteralObject(S, nil);
end;

function TUtilsCustomAliasedStrings.AddLiteralObject(const S: string;
  AObject: TObject): Integer;
begin
  CheckCanModify;
  if not Sorted then
    Result := Count
  else if Find(EscapeStr(S), Result) then
    case Duplicates of
      dupIgnore: Exit;
      dupError: AutoListError(SNoDuplicatesAllowed, 0);
    end;
  InsertLiteralObject(Result, S, AObject);
end;

function TUtilsCustomAliasedStrings.AddObject(const S: string;
  AObject: TObject): Integer;
begin
  Result := inherited AddObject(UnescapeStr(S), AObject);
end;

procedure TUtilsCustomAliasedStrings.DeleteAllLiteralOccurences(
  const S: string);
begin
  inherited DeleteAllOccurences(S);
end;

procedure TUtilsCustomAliasedStrings.DeleteAllOccurences(const S: string);
begin
  inherited DeleteAllOccurences(UnescapeStr(S));
end;

procedure TUtilsCustomAliasedStrings.DoAliasChanged;
begin
  if not (AliasOpt = aoOff) then
  begin
    DoNotifyDisableControls;
    try
      if Sorted then
        Sort;
      DoNotifyStateChanged;
      DoNotifyAliasChanged;
    finally
      DoNotifyEnableControls;
    end;
  end;
end;

function TUtilsCustomAliasedStrings.EscapeStr(S: string): string;
begin
  if Assigned(Aliases) then
    Result := Aliases.EscapeStr(S, AliasOpt)
  else
    Result := S;
end;

function TUtilsCustomAliasedStrings.Get(Index: Integer): string;
begin
  Result := EscapeStr(inherited Get(Index));
end;

function TUtilsCustomAliasedStrings.GetLiteral(
  Index: Integer): string;
begin
  Result := inherited Get(Index);
end;

function TUtilsCustomAliasedStrings.HasLiteralStr(
  const S: string): Boolean;
begin
  Result := IndexOfLiteral(S, 1) >= 0;
end;

function TUtilsCustomAliasedStrings.IndexOfLiteral(const S: string;
  I: Integer): Integer;
begin
  Result := IndexOf(EscapeStr(S), I);
end;

function TUtilsCustomAliasedStrings.InsertLiteral(Index: Integer;
  const S: string): Boolean;
begin
  Result := InsertLiteralObject(Index, S, nil);
end;

function TUtilsCustomAliasedStrings.InsertLiteralObject(Index: Integer;
  const S: string; AObject: TObject): Boolean;
begin
  Result := inherited InsertObject(Index, S, AObject);
end;

function TUtilsCustomAliasedStrings.InsertObject(Index: Integer;
  const S: string; AObject: TObject): Boolean;
begin
  Result := inherited InsertObject(Index, UnescapeStr(S), AObject);
end;

procedure TUtilsCustomAliasedStrings.Put(Index: Integer;
  const Value: string);
begin
  inherited Put(Index, UnescapeStr(Value));
  // Remark: We always store the literal string value in the list and
  //         recalculate the aliased version on retrieval.
end;

procedure TUtilsCustomAliasedStrings.PutLiteral(Index: Integer;
  const Value: string);
begin
  inherited Put(Index, Value);
end;

procedure TUtilsCustomAliasedStrings.SetAliasOpt(
  const Value: TUtilsAliasOpt);
begin
  if FAliasOpt <> Value then
  begin
    FAliasOpt := Value;
    if Assigned(Aliases) then
    begin
      DoNotifyDisableControls;
      try
        if Sorted then
          Sort;
        DoNotifyStateChanged;
      finally
        DoNotifyEnableControls;
      end;
    end;
  end;
end;

function TUtilsCustomAliasedStrings.UnescapeStr(S: string): string;
begin
  if Assigned(Aliases) then
    Result := Aliases.UnescapeStr(S, AliasOpt)
  else
    Result := S;
end;

{ TUtilsAutoStrings }

function TUtilsAutoStrings.Add(const S: string): Integer;
begin
  Result := inherited Add(S);
end;

function TUtilsAutoStrings.AddLiteral(const S: string): Integer;
begin
  Result := inherited AddLiteral(S);
end;

function TUtilsAutoStrings.AddLiteralObject(const S: string;
  AObject: TObject): Integer;
begin
  Result := inherited AddLiteralObject(S, AObject);
end;

function TUtilsAutoStrings.AddObject(const S: string;
  AObject: TObject): Integer;
begin
  Result := inherited AddObject(S, AObject);
end;

procedure TUtilsAutoStrings.Clear;
begin
  inherited;
end;

procedure TUtilsAutoStrings.CustomSort(Compare: TUtilsStringCompare);
begin
  inherited;
end;

procedure TUtilsAutoStrings.Delete(Index: Integer);
begin
  inherited;
end;

procedure TUtilsAutoStrings.DeleteAllLiteralOccurences(const S: string);
begin
  inherited;
end;

procedure TUtilsAutoStrings.DeleteAllOccurences(const S: string);
begin
  inherited;
end;

procedure TUtilsAutoStrings.DisableControls;
begin
  DoNotifyDisableControls;
end;

procedure TUtilsAutoStrings.EnableControls;
begin
  DoNotifyEnableControls;
end;

procedure TUtilsAutoStrings.Exchange(Index1, Index2: Integer);
begin
  inherited;
end;

function TUtilsAutoStrings.HasLiteralStr(const S: string): Boolean;
begin
  Result := inherited HasLiteralStr(S);
end;

function TUtilsAutoStrings.HasStr(const S: string): Boolean;
begin
  Result := inherited HasStr(S);
end;

function TUtilsAutoStrings.IndexOf(const S: string; I: Integer): Integer;
begin
  Result := inherited IndexOf(S, I);
end;

function TUtilsAutoStrings.IndexOfLiteral(const S: string;
  I: Integer): Integer;
begin
  Result := inherited IndexOfLiteral(S, I);
end;

function TUtilsAutoStrings.Insert(Index: Integer; const S: string): Boolean;
begin
  Result := inherited Insert(Index, S);
end;

function TUtilsAutoStrings.InsertLiteral(Index: Integer;
  const S: string): Boolean;
begin
  Result := inherited InsertLiteral(Index, S);
end;

function TUtilsAutoStrings.InsertLiteralObject(Index: Integer;
  const S: string; AObject: TObject): Boolean;
begin
  Result := inherited InsertLiteralObject(Index, S, AObject);
end;

function TUtilsAutoStrings.InsertObject(Index: Integer; const S: string;
  AObject: TObject): Boolean;
begin
  Result := inherited InsertObject(Index, S, AObject);
end;

procedure TUtilsAutoStrings.Sort;
begin
  inherited;
end;

{ TUtilsStringIterator }

function TUtilsStringIterator.GetCanModify: Boolean;
begin
  if Assigned(List) then
    Result := List.CanModify and (UpdateCount = 0)
  else
    Result := False;
end;

function TUtilsStringIterator.GetList: TUtilsCustomAliasedStrings;
begin
  Result := inherited GetList as TUtilsCustomAliasedStrings;
end;

function TUtilsStringIterator.GetLiteralValue: string;
begin
  Result := FLiteralValue;
end;

function TUtilsStringIterator.GetValue: string;
begin
  Result := FValue;
end;

procedure TUtilsStringIterator.SetList(
  Value: TUtilsCustomAliasedStrings);
begin
  inherited SetList(Value);
end;

procedure TUtilsStringIterator.UpdateIterator(const NewPos: Integer;
  const ItemChanged, ListModified: Boolean);
var
  NewCount: Integer;
begin
  if ItemChanged then
  begin
    if Assigned(List) then
    begin
      if List.Enabled then
        NewCount := List.Count
      else
        NewCount := 0;
      if (NewPos = -1) or (NewPos = NewCount) then
      begin
        FLiteralValue := '';
        FValue := '';
      end
      else
      begin
        FLiteralValue := List.Literals[NewPos];
        FValue := List[NewPos];
      end;
    end
    else
    begin
      FLiteralValue := '';
      FValue := '';
    end;
  end;

  inherited;
end;

{ TUtilsIteratorAdapter }

procedure TUtilsIteratorAdapter.CheckIteratorFixed;
begin
  if FIteratorFixed then
    AutoListError(SNoModification, -1);
end;

destructor TUtilsIteratorAdapter.Destroy;
begin
  FIteratorFixed := False;
  FOnActiveChanged := nil;
  FOnPositionChanged := nil;
  FOnItemChanged := nil;
  FOnListModified := nil;
  SetIterator(nil);
  inherited;
end;

procedure TUtilsIteratorAdapter.DoActiveChanged;
begin
  if Assigned(FOnActiveChanged) then
    FOnActiveChanged(Self);
end;

function TUtilsIteratorAdapter.ExecuteAction(
  Action: TBasicAction): Boolean;
begin
  if Action.HandlesTarget(Iterator) then
  begin
    Action.ExecuteTarget(Iterator);
    Result := True;
  end
  else
    Result := False;
end;

function TUtilsIteratorAdapter.GetIterator: TUtilsCustomIterator;
begin
  Result := FIterator;
end;

procedure TUtilsIteratorAdapter.HandleItemChanged(Sender: TObject);
begin
  if Assigned(FOnItemChanged) then
    FOnItemChanged(Self);
end;

procedure TUtilsIteratorAdapter.HandleListModified(Sender: TObject);
begin
  if Assigned(FOnListModified) then
    FOnListModified(Self);
end;

procedure TUtilsIteratorAdapter.HandlePositionChanged(Sender: TObject; OldPos,
  NewPos: Integer);
begin
  if Assigned(FOnPositionChanged) then
    FOnPositionChanged(Self, OldPos, NewPos);
end;

procedure TUtilsIteratorAdapter.SetIterator(
  AIterator: TUtilsCustomIterator);
begin
  if FIterator <> AIterator then
  begin
    CheckIteratorFixed;
    if Assigned(FIterator) then
      FIterator.RemoveAdapter(Self);
    if Assigned(AIterator) then
      AIterator.AddAdapter(Self);
  end;
end;

function TUtilsIteratorAdapter.UpdateAction(Action: TBasicAction): Boolean;
begin
  if Action.HandlesTarget(Iterator) then
  begin
    Action.UpdateTarget(Iterator);
    Result := True;
  end
  else
    Result := False;
end;

{ TUtilsStringIteratorAdapter }

function TUtilsStringIteratorAdapter.GetIterator: TUtilsStringIterator;
begin
  Result := inherited GetIterator as TUtilsStringIterator;
end;

procedure TUtilsStringIteratorAdapter.SetIterator(
  AIterator: TUtilsStringIterator);
begin
  inherited SetIterator(AIterator);
end;

{ TUtilsCustomAliases }

constructor TUtilsCustomAliases.Create(AOwner: TComponent);
begin
  inherited;
  FAliasList := TStringList.Create;
  with FAliasList do
  begin
    Duplicates := dupIgnore;
    Sorted := True;
  end;
end;

destructor TUtilsCustomAliases.Destroy;
begin
  FAliasList.Free;
  inherited;
end;

procedure TUtilsCustomAliases.BeginUpdate;
begin
  if FUpdateCount = 0 then
    SetUpdateState(True);
  Inc(FUpdateCount);
end;

procedure TUtilsCustomAliases.CheckAlias(S: string);
begin
  // By default do nothing.
end;

procedure TUtilsCustomAliases.CheckConsistency;
var
  I: Integer;
begin
  try
    for I := 0 to Pred(Count) do
    begin
      CheckKey(Key[I]);
      CheckAlias(Alias[I]);
    end;
  except
    FAliasList.Clear;
    raise;
  end;
end;

procedure TUtilsCustomAliases.CheckKey(S: string);
begin
  if Length(S) = 0 then
    AutoListError(SInvalidAbbr, -1);
end;

procedure TUtilsCustomAliases.Clear;
var
  TriggerEvents: Boolean;
begin
  TriggerEvents := Count > 0;
  if TriggerEvents then
    DoChanging;
  FAliasList.Clear;
  if TriggerEvents then
    DoChange;
end;

procedure TUtilsCustomAliases.Define(Key, Alias: string);
begin
  CheckKey(Key);
  CheckAlias(Alias);
  DoChanging;
  FAliasList.Add(Key + '@' + Alias);
  // xxx Storing the value of both, the key and the alias, in one string
  //     separated by '@' is slow.  I should work out a better method if time permits.
  DoChange;
end;

procedure TUtilsCustomAliases.DoChange;
begin
  if (FUpdateCount = 0) and Assigned(FOnChange) then
    FOnChange(Self);
end;

procedure TUtilsCustomAliases.DoChanging;
begin
  if (FUpdateCount = 0) and Assigned(FOnChanging) then
    FOnChanging(Self);
end;

procedure TUtilsCustomAliases.EndUpdate;
begin
  Dec(FUpdateCount);
  if FUpdateCount = 0 then
    SetUpdateState(False);
end;

function TUtilsCustomAliases.EscapeStr(S: string;
  Opt: TUtilsAliasOpt): string;
var
  I: Integer;
  N: string;
begin
  case Opt of
    aoOff: Result := S;
    aoFull:
      begin
        I := IndexOfKey(S);
        if I = -1 then
          Result := S
        else
          Result := Alias[I];
      end;
    aoPrefix:
      begin
        Result := S;
        for I := 0 to Pred(Count) do
        begin
          N := Key[I];
          if Copy(S, 1, Length(N)) = N then
          begin
            Result := Alias[I] + Copy(S, Length(N) + 1, Length(S));
            Exit;
          end;
        end;
      end;
  end;
end;

function TUtilsCustomAliases.GetCount: Integer;
begin
  Result := FAliasList.Count;
end;

function TUtilsCustomAliases.GetAlias(Index: Integer): string;
var
  S: string;
  I, P: Integer;
begin
  P := 1;
  S := FAliasList[Index];
  for I := 1 to Length(S) do // Using a loop is faster than Pos()
    if S[I] = '@' then
    begin
      P := I + 1;
      Break;
    end;
  Result := Copy(S, P, Length(S));
end;

function TUtilsCustomAliases.GetKey(Index: Integer): string;
var
  S: string;
  I, P: Integer;
begin
  P := 0;
  S := FAliasList[Index];
  for I := 1 to Length(S) do // Using a loop is faster than Pos()
    if S[I] = '@' then
    begin
      P := I - 1;
      Break;
    end;
  Result :=  Copy(S, 1, P);
end;

function TUtilsCustomAliases.HasAlias(S: string): Boolean;
begin
  Result := not (IndexOfAlias(S) = -1);
end;

function TUtilsCustomAliases.HasKey(S: string): Boolean;
begin
  Result := not (IndexOfKey(S) = -1);
end;

function TUtilsCustomAliases.IndexOfAlias(S: string): Integer;
var
  I, L: Integer;
  E: string;
begin
  E := '@' + S;
  L := Length(E);
  Result := -1;
  for I := 0 to Pred(Count) do
  begin
    if Copy(FAliasList[I], Length(FAliasList[I]) + 1 - L, L) = E then
    begin
      Result := I;
      Exit;
    end;
  end;
end;

function TUtilsCustomAliases.IndexOfKey(S: string): Integer;
var
  I, L: Integer;
  A: string;
begin
  A := S + '@';
  L := Length(A);
  Result := -1;
  for I := 0 to Pred(Count) do
  begin
    if Copy(FAliasList[I], 1, L)= A then
    begin
      Result := I;
      Exit;
    end;
  end;
end;

procedure TUtilsCustomAliases.LoadFromFile(const FileName: string);
var
  Stream: TStream;
begin
  Stream := TFileStream.Create(FileName, fmOpenRead or fmShareDenyWrite);
  try
    LoadFromStream(Stream);
  finally
    Stream.Free;
  end;
end;

procedure TUtilsCustomAliases.LoadFromStream(Stream: TStream);
begin
  BeginUpdate;
  try
    FAliasList.LoadFromStream(Stream);
    CheckConsistency;
  finally
    EndUpdate;
  end;
end;

function TUtilsCustomAliases.RemoveAlias(S: string): Boolean;
var
  Index: Integer;
begin
  Index := IndexOfAlias(S);
  if Index = -1 then
  begin
    Result := False;
  end
  else
  begin
    DoChanging;
    FAliasList.Delete(Index);
    DoChange;
    Result := True;
  end;
end;

function TUtilsCustomAliases.RemoveKey(S: string): Boolean;
var
  Index: Integer;
begin
  Index := IndexOfKey(S);
  if Index = -1 then
  begin
    Result := False;
  end
  else
  begin
    DoChanging;
    FAliasList.Delete(Index);
    DoChange;
    Result := True;
  end;
end;

procedure TUtilsCustomAliases.SaveToFile(const FileName: string);
begin
  FAliasList.SaveToFile(Filename);
end;

procedure TUtilsCustomAliases.SaveToStream(Stream: TStream);
begin
  FAliasList.SaveToStream(Stream);
end;

procedure TUtilsCustomAliases.SetUpdateState(Updating: Boolean);
begin
  if Updating then
    DoChanging
  else
    DoChange;
end;

function TUtilsCustomAliases.UnescapeStr(S: string;
  Opt: TUtilsAliasOpt): string;
var
  I: Integer;
  A: string;
begin
  case Opt of
    aoOff: Result := S;
    aoFull:
      begin
        I := IndexOfAlias(S);
        if I = -1 then
          Result := S
        else
          Result := Key[I];
      end;
    aoPrefix:
      begin
        Result := S;
        for I := 0 to Pred(Count) do
        begin
          A := Alias[I];
          if Copy(S, 1, Length(A)) = A then
          begin
            Result := Key[I] + Copy(S, Length(A) + 1, Length(S));
            Exit;
          end;
        end;
      end;
  end;
end;

{ TUtilsCustomIteratorAliases }

constructor TUtilsCustomIteratorAliases.Create(AOwner: TComponent);
begin
  inherited;
  FIteratorLists := TList.Create;
end;

destructor TUtilsCustomIteratorAliases.Destroy;
begin
  if Assigned(FIteratorLists) then
    while FIteratorLists.Count > 0 do
      RemoveIteratorList(TUtilsCustomIteratorList(FIteratorLists.Last));
  FreeAndNil(FIteratorLists);
  inherited;
end;

procedure TUtilsCustomIteratorAliases.AddIteratorList(
  IteratorList: TUtilsCustomIteratorList);
begin
  FIteratorLists.Add(IteratorList);
  IteratorList.FAliases := Self;
end;

procedure TUtilsCustomIteratorAliases.DoChange;
var
  I: Integer;
begin
  if UpdateCount = 0 then
    for I := 0 to Pred(FIteratorLists.Count) do
      TUtilsCustomAliasedStrings(FIteratorLists[I]).DoAliasChanged;
  inherited;
end;

procedure TUtilsCustomIteratorAliases.RemoveIteratorList(
  IteratorList: TUtilsCustomIteratorList);
begin
  IteratorList.FAliases := nil;
  FIteratorLists.Remove(IteratorList);
end;

{ TUtilsAutoAliases }

procedure TUtilsAutoAliases.BeginUpdate;
begin
  inherited;
end;

procedure TUtilsAutoAliases.Clear;
begin
  inherited;
end;

procedure TUtilsAutoAliases.Define(Key, Alias: string);
begin
  inherited;
end;

procedure TUtilsAutoAliases.EndUpdate;
begin
  inherited;
end;

procedure TUtilsAutoAliases.LoadFromFile(const FileName: string);
begin
  inherited;
end;

procedure TUtilsAutoAliases.LoadFromStream(Stream: TStream);
begin
  inherited;
end;

function TUtilsAutoAliases.RemoveAlias(S: string): Boolean;
begin
  Result := RemoveAlias(S);
end;

function TUtilsAutoAliases.RemoveKey(S: string): Boolean;
begin
  Result := RemoveKey(S);
end;

procedure TUtilsAutoAliases.SaveToFile(const FileName: string);
begin
  inherited;
end;

procedure TUtilsAutoAliases.SaveToStream(Stream: TStream);
begin
  inherited;
end;

end.

