unit AutoStrCtrls;

// AutoStrCtrls 1.0.1
// Delphi 6 to 2007 and Kylix 3 Implementation
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
// The Original Code is "AutoStrCtrls.pas".
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
// 2007-12-03 1.0.1 Made .NET compatible.
// 2003-08-03 1.0.0

{$IFDEF VER120}
  {$DEFINE D4_TO_D2005}
{$ENDIF}
{$IFDEF VER12%}
  {$DEFINE D4_TO_D2005}
{$ENDIF}
{$IFDEF VER130}
  {$DEFINE D4_TO_D2005}
{$ENDIF}
{$IFDEF VER140}
  {$DEFINE D4_TO_D2005}
{$ENDIF}
{$IFDEF VER150}
  {$DEFINE D4_TO_D2005}
{$ENDIF}
{$IFDEF VER160}
  {$DEFINE D4_TO_D2005}
{$ENDIF}
{$IFDEF VER170}
  {$DEFINE D4_TO_D2005}
{$ENDIF}

{$WARNINGS OFF}
{$HINTS OFF}

interface

uses
{$IFDEF CLR}
  System.Reflection,
{$ENDIF}
  AutoListUtils, Buttons, ComCtrls, ExtCtrls, Controls, StdCtrls,
  SysUtils, Classes, Math, Dialogs, Messages, Windows;
type

  { Event prototypes}

  TAutoStrUserTextEvent = procedure(Sender: TObject; Iterator:
    TUtilsStringIterator; var S: string) of object;
  TAutoStrUserTextFilterEvent = procedure(Sender: TObject; Iterator:
    TUtilsStringIterator; var S: string;
    var Accept: Boolean) of object;

  { TAutoStrGroupBox }

  TAutoStrGroupBox = class(TCustomGroupBox)
  private
    FAdapter: TUtilsStringIteratorAdapter;
    FCaptionDefault: string;
    FCaptionSuffix: string;
    FCaptionPrefix: string;
    FOnGetUserText: TAutoStrUserTextEvent;
    function GetIterator: TUtilsStringIterator;
    function GetStatementText: string;
    procedure SetIterator(Value: TUtilsStringIterator);
  protected
    procedure DoOnGetUserText(var S: string); virtual;
    procedure ItemChangedEventHandler(Sender: TObject); virtual;
    procedure Loaded; override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation); override;
    procedure SetCaptionDefault(const Value: string); virtual;
    procedure SetCaptionPrefix(const Value: string); virtual;
    procedure SetCaptionSuffix(const Value: string); virtual;
    procedure UpdateCaption; virtual;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure ForceUpdate; virtual;
  published
    property Align;
    property Anchors;
    property BiDiMode;
    property CaptionDefault: string read FCaptionDefault write
      SetCaptionDefault;
    property CaptionPrefix: string read FCaptionPrefix write SetCaptionPrefix;
    property CaptionSuffix: string read FCaptionSuffix write SetCaptionSuffix;
    property Color;
    property Constraints;
    property Ctl3D;
    property DockSite;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Font;
    property Iterator: TUtilsStringIterator read GetIterator write SetIterator;
{$IFNDEF VER140}
    property ParentBackground default True;
      // ParentBackground does not exist in Delphi 6.
{$ENDIF}
    property ParentBiDiMode;
    property ParentColor;
    property ParentCtl3D;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Visible;
    property OnClick;
    property OnContextPopup;
    property OnDblClick;
    property OnDragDrop;
    property OnDockDrop;
    property OnDockOver;
    property OnDragOver;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnGetSiteInfo;
    property OnGetUserText: TAutoStrUserTextEvent read FOnGetUserText write
      FOnGetUserText;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnStartDock;
    property OnStartDrag;
    property OnUnDock;
  end;

  { TAutoStrLabel }

  TAutoStrLabel = class(TCustomLabel)
  private
    FAdapter: TUtilsStringIteratorAdapter;
    FCaptionDefault: string;
    FCaptionSuffix: string;
    FCaptionPrefix: string;
    FOnGetUserText: TAutoStrUserTextEvent;
    function GetIterator: TUtilsStringIterator;
    function GetStatementText: string;
    procedure SetIterator(Value: TUtilsStringIterator);
  protected
    procedure DoOnGetUserText(var S: string); virtual;
    function GetLabelText: string; override;
    procedure ItemChangedEventHandler(Sender: TObject); virtual;
    procedure Loaded; override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation); override;
    procedure SetCaptionDefault(const Value: string); virtual;
    procedure SetCaptionPrefix(const Value: string); virtual;
    procedure SetCaptionSuffix(const Value: string); virtual;
    procedure UpdateCaption; virtual;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    function ExecuteAction(Action: TBasicAction): Boolean; override;
    procedure ForceUpdate; virtual;
    function UpdateAction(Action: TBasicAction): Boolean; override;
    function UseRightToLeftAlignment: Boolean; override;
  published
    property Align;
    property Alignment;
    property Anchors;
    property AutoSize default False;
    property BiDiMode;
    property CaptionDefault: string read FCaptionDefault write
      SetCaptionDefault;
    property CaptionPrefix: string read FCaptionPrefix write SetCaptionPrefix;
    property CaptionSuffix: string read FCaptionSuffix write SetCaptionSuffix;
    property Color;
    property Constraints;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Font;
    property Iterator: TUtilsStringIterator read GetIterator write SetIterator;
    property ParentBiDiMode;
    property ParentColor;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property Transparent;
    property Visible;
    property WordWrap;
    property OnClick;
    property OnContextPopup;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDock;
    property OnEndDrag;
    property OnGetUserText: TAutoStrUserTextEvent read FOnGetUserText write
      FOnGetUserText;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnStartDock;
    property OnStartDrag;
  end;

  { TAutoStrButton }

  TAutoStrButton = class(TButton)
  private
    FAdapter: TUtilsStringIteratorAdapter;
    FCaptionDefault: string;
    FCaptionSuffix: string;
    FCaptionPrefix: string;
    FOnGetUserText: TAutoStrUserTextEvent;
    function GetIterator: TUtilsStringIterator;
    function GetStatementText: string;
    procedure SetIterator(Value: TUtilsStringIterator);
  protected
    procedure DoOnGetUserText(var S: string); virtual;
    procedure ItemChangedEventHandler(Sender: TObject); virtual;
    procedure Loaded; override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation); override;
    procedure SetCaptionDefault(const Value: string); virtual;
    procedure SetCaptionPrefix(const Value: string); virtual;
    procedure SetCaptionSuffix(const Value: string); virtual;
    procedure UpdateCaption; virtual;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    property Caption;
    procedure ForceUpdate; virtual;
    function UseRightToLeftAlignment: Boolean; override;
  published
    property CaptionDefault: string read FCaptionDefault write
      SetCaptionDefault;
    property CaptionPrefix: string read FCaptionPrefix write SetCaptionPrefix;
    property CaptionSuffix: string read FCaptionSuffix write SetCaptionSuffix;
    property Iterator: TUtilsStringIterator read GetIterator write SetIterator;
    property OnGetUserText: TAutoStrUserTextEvent read FOnGetUserText write
      FOnGetUserText;
  end;

  { TAutoStrMemo }

  TAutoStrMemo = class(TCustomMemo)
  private
    FAdapter: TUtilsStringIteratorAdapter;
    FTextDefault: string;
    FOnGetUserText: TAutoStrUserTextEvent;
    function GetIterator: TUtilsStringIterator;
    function GetStatementText: string;
    procedure SetIterator(Value: TUtilsStringIterator);
  protected
    procedure DoOnGetUserText(var S: string); virtual;
    procedure ItemChangedEventHandler(Sender: TObject); virtual;
    procedure Loaded; override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation); override;
    procedure SetTextDefault(const Value: string); virtual;
    procedure UpdateCaption; virtual;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    function ExecuteAction(Action: TBasicAction): Boolean; override;
    procedure ForceUpdate; virtual;
    function UpdateAction(Action: TBasicAction): Boolean; override;
    function UseRightToLeftAlignment: Boolean; override;
  published
    property Align;
    property Alignment;
    property Anchors;
    property BevelEdges;
    property BevelInner;
    property BevelKind default bkNone;
    property BevelOuter;
    property BiDiMode;
    property BorderStyle;
    property Color;
    property Constraints;
    property Ctl3D;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Font;
    property HideSelection;
    property ImeMode;
    property ImeName;
    property Iterator: TUtilsStringIterator read GetIterator write SetIterator;
    property Lines;
    property MaxLength;
    property OEMConvert;
    property ParentBiDiMode;
    property ParentColor;
    property ParentCtl3D;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ScrollBars;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property TextDefault: string read FTextDefault write SetTextDefault;
    property Visible;
    property WantReturns;
    property WantTabs;
    property WordWrap;
    property OnChange;
    property OnClick;
    property OnContextPopup;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnGetUserText: TAutoStrUserTextEvent read FOnGetUserText write
      FOnGetUserText;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnStartDock;
    property OnStartDrag;
  end;

  { TAutoStrListBox }

  TAutoStrListBox = class(TCustomListBox)
  private
    FAdapter: TUtilsStringIteratorAdapter;
    FCanNavigate: Boolean;
    FFocusUpdating: Boolean;
    FOnGetUserText: TAutoStrUserTextFilterEvent;
    function GetIterator: TUtilsStringIterator;
    procedure SetCanNavigate(const Value: Boolean);
    procedure SetIterator(Value: TUtilsStringIterator);
  protected
    procedure Click; override;
    procedure DoOnGetUserText(Iterator: TUtilsStringIterator;
      var S: string;
      var Accept: Boolean); virtual;
    procedure ItemChangedEventHandler(Sender: TObject); virtual;
    procedure ListModifiedEventHandler(Sender: TObject); virtual;
    procedure Loaded; override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation); override;
    procedure PositionChangedEventHandler(Sender: TObject;
      OldPos,
      NewPos: Integer); virtual;
    procedure SetItemIndex(const Value: Integer); override;
    procedure UpdateFocus; virtual;
    procedure UpdateListBox; virtual;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    function ExecuteAction(Action: TBasicAction): Boolean; override;
    procedure ForceUpdate; virtual;
    function UpdateAction(Action: TBasicAction): Boolean; override;
    function UseRightToLeftAlignment: Boolean; override;
  published
    property Align;
    property Anchors;
    property AutoComplete;
    property BevelEdges;
    property BevelInner;
    property BevelOuter;
    property BevelKind;
    property BevelWidth;
    property BiDiMode;
    property BorderStyle;
    property CanNavigate: Boolean read FCanNavigate write SetCanNavigate default
      False;
    property Color;
    property Constraints;
    property Ctl3D;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Font;
    property ImeMode;
    property ImeName;
    property IntegralHeight;
    property ItemHeight;
    property Items;
    property Iterator: TUtilsStringIterator read GetIterator write SetIterator;
    property ParentBiDiMode;
    property ParentColor;
    property ParentCtl3D;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property Sorted;
    property Style;
    property TabOrder;
    property TabStop;
    property Visible;
    property OnClick;
    property OnContextPopup;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnDrawItem;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnGetUserText: TAutoStrUserTextFilterEvent read FOnGetUserText write
      FOnGetUserText;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMeasureItem;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnStartDock;
    property OnStartDrag;
  end;

  { TAutoStrComboBox }

  TAutoStrComboBox = class(TCustomComboBox)
  private
    FAdapter: TUtilsStringIteratorAdapter;
    FCanNavigate: Boolean;
    FFocusUpdating: Boolean;
    FOnGetUserText: TAutoStrUserTextFilterEvent;
    function GetIterator: TUtilsStringIterator;
    procedure SetCanNavigate(const Value: Boolean);
    procedure SetIterator(Value: TUtilsStringIterator);
  protected
    procedure Click; override;
    procedure DoOnGetUserText(Iterator: TUtilsStringIterator;
      var S: string;
      var Accept: Boolean); virtual;
    procedure ItemChangedEventHandler(Sender: TObject); virtual;
    procedure ListModifiedEventHandler(Sender: TObject); virtual;
    procedure Loaded; override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation); override;
    procedure PositionChangedEventHandler(Sender: TObject;
      OldPos,
      NewPos: Integer); virtual;
    procedure SetItemIndex(const Value: Integer); override;
    procedure UpdateComboBox; virtual;
    procedure UpdateFocus; virtual;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure ForceUpdate; virtual;
    function UseRightToLeftAlignment: Boolean; override;
  published
    property Anchors;
    property AutoComplete default True;
    property AutoDropDown default False;
{$IFNDEF VER140}
    property AutoCloseUp default False;
      // AutoCloseUp does not exist in Delphi 6.
{$ENDIF}
    property BevelEdges;
    property BevelInner;
    property BevelKind default bkNone;
    property BevelOuter;
    property BiDiMode;
    property CanNavigate: Boolean read FCanNavigate write SetCanNavigate default
      False;
    property Color;
    property Constraints;
    property Ctl3D;
    property DragCursor;
    property DragKind;
    property DragMode;
    property DropDownCount;
    property Enabled;
    property Font;
    property ImeMode;
    property ImeName;
    property ItemHeight;
    property ItemIndex default -1;
    property Iterator: TUtilsStringIterator read GetIterator write SetIterator;
    property MaxLength;
    property ParentBiDiMode;
    property ParentColor;
    property ParentCtl3D;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property Sorted;
    property TabOrder;
    property TabStop;
    property Text;
    property Visible;
    property OnChange;
    property OnClick;
    property OnCloseUp;
    property OnContextPopup;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnDrawItem;
    property OnDropDown;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnGetUserText: TAutoStrUserTextFilterEvent read FOnGetUserText write
      FOnGetUserText;
    property OnKeyDown;
    property OnKeyPress;
    property OnKeyUp;
    property OnMeasureItem;
    property OnSelect;
    property OnStartDock;
    property OnStartDrag;
    property Items; { Must be published after OnMeasureItem }
  end;

{$IFNDEF VER140}

  // In Delphi 6 TCustomTabControl has no protected SetTabIndex procedure.
  // So we cannot use it to define TAutoStrTabControl.

  { TAutoStrTabControl }

  TAutoStrTabControl = class(TCustomTabControl)
  private
    FAdapter: TUtilsStringIteratorAdapter;
    FCanNavigate: Boolean;
    FFocusUpdating: Boolean;
    FOnGetUserText: TAutoStrUserTextFilterEvent;
    FSorted: Boolean;
    function GetIterator: TUtilsStringIterator;
    procedure SetCanNavigate(const Value: Boolean);
    procedure SetIterator(Value: TUtilsStringIterator);
    procedure SetSorted(const Value: Boolean);
    function GetTabs: TStrings;
  protected
    procedure Change; override;
    procedure DoOnGetUserText(Iterator: TUtilsStringIterator;
      var S: string;
      var Accept: Boolean); virtual;
    procedure ItemChangedEventHandler(Sender: TObject); virtual;
    procedure ListModifiedEventHandler(Sender: TObject); virtual;
    procedure Loaded; override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation); override;
    procedure PositionChangedEventHandler(Sender: TObject;
      OldPos,
      NewPos: Integer); virtual;
    procedure SetTabIndex(Value: Integer); override;
    procedure UpdateFocus; virtual;
    procedure UpdateTabs; virtual;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    property DisplayRect;
    procedure ForceUpdate; virtual;
    function UseRightToLeftAlignment: Boolean; override;
    property Tabs: TStrings read GetTabs;
    property TabIndex; // must be after Tabs
  published
    property Align;
    property Anchors;
    property BiDiMode;
    property CanNavigate: Boolean read FCanNavigate write SetCanNavigate default
      False;
    property Constraints;
    property DockSite;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Font;
    property HotTrack;
    property Images;
    property Iterator: TUtilsStringIterator read GetIterator write SetIterator;
    property MultiLine;
    property MultiSelect;
    property OwnerDraw;
    property ParentBiDiMode;
    property ParentFont;
    property ParentShowHint;
    property PopupMenu;
    property RaggedRight;
    property ScrollOpposite;
    property ShowHint;
    property Sorted: Boolean read FSorted write SetSorted default False;
    property Style;
    property TabHeight;
    property TabOrder;
    property TabPosition;
    property TabStop;
    property TabWidth;
    property Visible;
    property OnChange;
    property OnChanging;
    property OnContextPopup;
    property OnDockDrop;
    property OnDockOver;
    property OnDragDrop;
    property OnDragOver;
    property OnDrawTab;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnGetImageIndex;
    property OnGetSiteInfo;
    property OnGetUserText: TAutoStrUserTextFilterEvent read FOnGetUserText write
      FOnGetUserText;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    property OnResize;
    property OnStartDock;
    property OnStartDrag;
    property OnUnDock;
  end;
{$ENDIF}

  { TAutoStrTrackBar }

  TAutoStrTrackBar = class(TTrackBar)
  private
    FAdapter: TUtilsStandardIteratorAdapter;
    FCanNavigate: Boolean;
    FPositionUpdating: Boolean;
    function GetIterator: TUtilsCustomIterator;
    procedure SetCanNavigate(const Value: Boolean);
    procedure SetIterator(Value: TUtilsCustomIterator);
  protected
    procedure ActiveChangedEventHandler(Sender: TObject); virtual;
    procedure Changed; override;
    procedure Loaded; override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation); override;
    procedure PositionChangedEventHandler(Sender: TObject;
      OldPos,
      NewPos: Integer); virtual;
    procedure UpdateTrackBar; virtual;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    property Max;
    property Min;
  published
    property CanNavigate: Boolean read FCanNavigate write SetCanNavigate default
      False;
    property Iterator: TUtilsCustomIterator read GetIterator write SetIterator;
  end;

  { TRdfNavigator }

  TAutoStrNavAction = (naOK, naCancel, naRetry);
  TAutoStrNavBtnType = (nbtFirst, nbtPrior, nbtNext, nbtLast, nbtAdd, nbtDelete,
    nbtEdit);
  TAutoStrNavBtnTypes = set of TAutoStrNavBtnType;

  TAutoStrNavClickEvent = procedure(Sender: TObject;
    BtnType: TAutoStrNavBtnType) of object;
  TAutoStrNavModifyEvent = procedure(Sender: TObject;
    var S: string;
    var Action: TAutoStrNavAction) of object;

  TAutoStrNavigator = class;

  TAutoStrNavBtn = class(TSpeedButton)
  private
    FBtnType: TAutoStrNavBtnType;
  public
    property BtnType: TAutoStrNavBtnType read FBtnType write FBtnType;
  end;

  TAutoStrNavigator = class(TCustomPanel)
  private
    FAdapter: TUtilsStandardIteratorAdapter;
    FConfirmDelete: Boolean;
    FConfirmDeleteStr: string;
    FButtonWidth: Integer;
    FDefHints: TStrings;
    FFlat: Boolean;
    FFocusedBtn: TAutoStrNavBtnType;
    FHints: TStrings;
    FMinBtnSize: Integer;
    FVisibleButtons: TAutoStrNavBtnTypes;
    FBeforeAction: TAutoStrNavClickEvent;
    FOnAdd: TAutoStrNavModifyEvent;
    FOnClick: TAutoStrNavClickEvent;
    FOnEdit: TAutoStrNavModifyEvent;
    procedure CalcMinSize(var W, H: Integer);
    procedure CMEnabledChanged(var Message: TMessage); message
      CM_ENABLEDCHANGED;
    function GetConfirmDeleteStr: string;
    function GetHints: TStrings;
    function GetIterator: TUtilsCustomIterator;
    procedure InitButtons;
    procedure InitHints(Sender: TObject);
    procedure NavBtnOnClickHandler(Sender: TObject);
    procedure NavBtnOnMouseDownHandler(Sender: TObject;
      Button: TMouseButton;
      Shift: TShiftState;
      X,
      Y: Integer);
    procedure SetConfirmDeleteStr(const Value: string);
    procedure SetFlat(Value: Boolean);
    procedure SetHints(Value: TStrings);
    procedure SetIterator(Value: TUtilsCustomIterator);
    procedure SetSize(var W: Integer;
      var H: Integer);
    procedure SetVisible(Value: TAutoStrNavBtnTypes);
    procedure WMSize(var Message: TWMSize); message WM_SIZE;
    procedure WMSetFocus(var Message: TWMSetFocus); message WM_SETFOCUS;
    procedure WMKillFocus(var Message: TWMKillFocus); message WM_KILLFOCUS;
    procedure WMGetDlgCode(var Message: TWMGetDlgCode); message WM_GETDLGCODE;
    procedure WMWindowPosChanging(var Message: TWMWindowPosChanging); message
      WM_WINDOWPOSCHANGING;
  protected
    Buttons: array[TAutoStrNavBtnType] of TAutoStrNavBtn;
    procedure ActiveChanged; virtual;
    procedure ActiveChangedEventHandler(Sender: TObject); virtual;
    procedure DoAdd; virtual;
    procedure DoDelete; virtual;
    procedure DoEdit; virtual;
{$IFDEF D4_TO_D2005}
  {$IFNDEF CLR}
    // Up to Delphi 2005 for Win32 GetChildren() is declared as protected
    // in the TCustomPanel ancestor class.
    procedure GetChildren(Proc: TGetChildProc;
      Root: TComponent); override;
  {$ENDIF}
{$ENDIF}
    procedure KeyDown(var Key: Word;
      Shift: TShiftState); override;
    procedure Loaded; override;
    procedure Notification(AComponent: TComponent;
      Operation: TOperation); override;
    procedure PositionChangedEventHandler(Sender: TObject;
      OldPos,
      NewPos: Integer); virtual;
    procedure SourceChanged; virtual;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
{$IFDEF CLR}
    // From Delphi 2005 for .NEt on GetChildren() is declared as public
    // in the TCustomPanel ancestor class.
    // Don't know what's the case for Delphi 8.  You may adjust the
    // compiler directives accordingly if you get a compiler error here.
    procedure GetChildren(Proc: TGetChildProc;
      Root: TComponent); override;
{$ELSE}
  {$IFNDEF D4_TO_D2005}
    // From Delphi 2006 on GetChildren() is declared as public
    // in the TCustomPanel ancestor class.
    procedure GetChildren(Proc: TGetChildProc;
      Root: TComponent); override;
  {$ENDIF}
{$ENDIF}
    procedure SetBounds(ALeft,
      ATop,
      AWidth,
      AHeight: Integer); override;
    procedure BtnClick(Index: TAutoStrNavBtnType); virtual;
  published
    property Align;
    property Anchors;
    property ConfirmDelete: Boolean read FConfirmDelete write FConfirmDelete
      default True;
    property ConfirmDeleteStr: string read FConfirmDeleteStr write
      SetConfirmDeleteStr;
    property Constraints;
    property Ctl3D;
    property DragCursor;
    property DragKind;
    property DragMode;
    property Enabled;
    property Flat: Boolean read FFlat write SetFlat default False;
    property Hints: TStrings read GetHints write SetHints;
    property Iterator: TUtilsCustomIterator read GetIterator write SetIterator;
    property ParentCtl3D;
    property ParentShowHint;
    property PopupMenu;
    property ShowHint;
    property TabOrder;
    property TabStop;
    property Visible;
    property VisibleButtons: TAutoStrNavBtnTypes read FVisibleButtons write
      SetVisible
      default [nbtFirst, nbtPrior, nbtNext, nbtLast, nbtAdd, nbtDelete,
        nbtEdit];

    property BeforeAction: TAutoStrNavClickEvent read FBeforeAction write
      FBeforeAction;
    property OnAdd: TAutoStrNavModifyEvent read FOnAdd write FOnAdd;
    property OnClick: TAutoStrNavClickEvent read FOnClick write FOnClick;
    property OnContextPopup;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEdit: TAutoStrNavModifyEvent read FOnEdit write FOnEdit;
    property OnEndDock;
    property OnEndDrag;
    property OnEnter;
    property OnExit;
    property OnResize;
    property OnStartDock;
    property OnStartDrag;
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('AutoStr', [TAutoStrNavigator,
    TAutoStrTrackBar,
    TAutoStrLabel,
    TAutoStrMemo,
    TAutoStrButton,
    TAutoStrListBox,
    TAutoStrComboBox,
    TAutoStrGroupBox
{$IFNDEF VER140}
    , TAutoStrTabControl
{$ENDIF}
    ]);
end;

{ TAutoStrGroupBox }

constructor TAutoStrGroupBox.Create(AOwner: TComponent);
begin
  inherited;
  ControlStyle := ControlStyle + [csReplicatable];
  AutoSize := False;
  FAdapter := TUtilsStringIteratorAdapter.Create;
  with FAdapter do
  begin
    OnActiveChanged := ItemChangedEventHandler;
    OnItemChanged := ItemChangedEventHandler;
  end;
end;

destructor TAutoStrGroupBox.Destroy;
begin
  FAdapter.Free;
  FAdapter := nil;
  inherited;
end;

procedure TAutoStrGroupBox.DoOnGetUserText(var S: string);
begin
  if Assigned(FOnGetUserText) then
    FOnGetUserText(Self, Iterator, S);
end;

procedure TAutoStrGroupBox.ForceUpdate;
begin
  UpdateCaption;
end;

function TAutoStrGroupBox.GetIterator: TUtilsStringIterator;
begin
  Result := FAdapter.Iterator;
end;

function TAutoStrGroupBox.GetStatementText: string;
var
  S: string;
begin
  if Assigned(Iterator) then
  begin
    with Iterator do
      if not Offlist then
      begin
        S := Value;
        DoOnGetUserText(S);
        Result := CaptionPrefix + S + CaptionSuffix;
        Exit;
      end
      else
        S := '';
  end
  else
    S := '';
  if csDesigning in ComponentState then
    Result := Name
  else
    Result := CaptionDefault;
end;

procedure TAutoStrGroupBox.ItemChangedEventHandler(Sender: TObject);
begin
  UpdateCaption;
end;

procedure TAutoStrGroupBox.Loaded;
begin
  inherited;
  if (csDesigning in ComponentState) then
    UpdateCaption;
end;

procedure TAutoStrGroupBox.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (Operation = opRemove) and Assigned(FAdapter) and (AComponent = Iterator)
    then
    Iterator := nil;
end;

procedure TAutoStrGroupBox.SetCaptionDefault(const Value: string);
begin
  FCaptionDefault := Value;
end;

procedure TAutoStrGroupBox.SetCaptionPrefix(const Value: string);
begin
  FCaptionPrefix := Value;
end;

procedure TAutoStrGroupBox.SetCaptionSuffix(const Value: string);
begin
  FCaptionSuffix := Value;
end;

procedure TAutoStrGroupBox.SetIterator(Value: TUtilsStringIterator);
begin
  if not (FAdapter.IteratorFixed and (csLoading in ComponentState)) then
    FAdapter.Iterator := Value;
  if Assigned(Value) then
    Value.FreeNotification(Self);
end;

procedure TAutoStrGroupBox.UpdateCaption;
begin
  Caption := GetStatementText;
end;

{ TAutoStrLabel }

constructor TAutoStrLabel.Create(AOwner: TComponent);
begin
  inherited;
  ControlStyle := ControlStyle + [csReplicatable];
  AutoSize := False;
  ShowAccelChar := False;
  FAdapter := TUtilsStringIteratorAdapter.Create;
  with FAdapter do
  begin
    OnActiveChanged := ItemChangedEventHandler;
    OnItemChanged := ItemChangedEventHandler;
  end;
end;

destructor TAutoStrLabel.Destroy;
begin
  FAdapter.Free;
  FAdapter := nil;
  inherited;
end;

procedure TAutoStrLabel.DoOnGetUserText(var S: string);
begin
  if Assigned(FOnGetUserText) then
    FOnGetUserText(Self, Iterator, S);
end;

function TAutoStrLabel.ExecuteAction(Action: TBasicAction): Boolean;
begin
  Result := inherited ExecuteAction(Action) or Assigned(FAdapter) and
    FAdapter.ExecuteAction(Action);
end;

procedure TAutoStrLabel.ForceUpdate;
begin
  UpdateCaption;
end;

function TAutoStrLabel.GetIterator: TUtilsStringIterator;
begin
  Result := FAdapter.Iterator;
end;

function TAutoStrLabel.GetLabelText: string;
begin
  if csPaintCopy in ControlState then
    Result := GetStatementText
  else
    Result := Caption;
end;

function TAutoStrLabel.GetStatementText: string;
var
  S: string;
begin
  if Assigned(Iterator) then
  begin
    with Iterator do
      if not Offlist then
      begin
        S := Value;
        DoOnGetUserText(S);
        Result := CaptionPrefix + S + CaptionSuffix;
        Exit;
      end
      else
        S := '';
  end
  else
    S := '';
  if csDesigning in ComponentState then
    Result := Name
  else
    Result := CaptionDefault;
end;

procedure TAutoStrLabel.ItemChangedEventHandler(Sender: TObject);
begin
  UpdateCaption;
end;

procedure TAutoStrLabel.Loaded;
begin
  inherited;
  if (csDesigning in ComponentState) then
    UpdateCaption;
end;

procedure TAutoStrLabel.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (Operation = opRemove) and Assigned(FAdapter) and (AComponent = Iterator)
    then
    Iterator := nil;
end;

procedure TAutoStrLabel.SetCaptionDefault(const Value: string);
begin
  FCaptionDefault := Value;
end;

procedure TAutoStrLabel.SetCaptionPrefix(const Value: string);
begin
  FCaptionPrefix := Value;
end;

procedure TAutoStrLabel.SetCaptionSuffix(const Value: string);
begin
  FCaptionPrefix := Value;
end;

procedure TAutoStrLabel.SetIterator(Value: TUtilsStringIterator);
begin
  if not (FAdapter.IteratorFixed and (csLoading in ComponentState)) then
    FAdapter.Iterator := Value;
  if Assigned(Value) then
    Value.FreeNotification(Self);
end;

function TAutoStrLabel.UpdateAction(Action: TBasicAction): Boolean;
begin
  Result := inherited UpdateAction(Action) or Assigned(FAdapter) and
    FAdapter.UpdateAction(Action);
end;

procedure TAutoStrLabel.UpdateCaption;
begin
  Caption := GetStatementText;
end;

function TAutoStrLabel.UseRightToLeftAlignment: Boolean;
begin
  Result := SysLocale.MiddleEast and (BiDiMode = bdRightToLeft);
end;

{ TAutoStrButton }

constructor TAutoStrButton.Create(AOwner: TComponent);
begin
  inherited;
  ControlStyle := ControlStyle + [csReplicatable];
  AutoSize := False;
  FAdapter := TUtilsStringIteratorAdapter.Create;
  with FAdapter do
  begin
    OnActiveChanged := ItemChangedEventHandler;
    OnItemChanged := ItemChangedEventHandler;
  end;
end;

destructor TAutoStrButton.Destroy;
begin
  FAdapter.Free;
  FAdapter := nil;
  inherited;
end;

procedure TAutoStrButton.DoOnGetUserText(var S: string);
begin
  if Assigned(FOnGetUserText) then
    FOnGetUserText(Self, Iterator, S);
end;

procedure TAutoStrButton.ForceUpdate;
begin
  UpdateCaption;
end;

procedure TAutoStrButton.ItemChangedEventHandler(Sender: TObject);
begin
  UpdateCaption;
end;

function TAutoStrButton.GetIterator: TUtilsStringIterator;
begin
  Result := FAdapter.Iterator;
end;

function TAutoStrButton.GetStatementText: string;
var
  S: string;
begin
  if Assigned(Iterator) then
  begin
    with Iterator do
      if not Offlist then
      begin
        S := Value;
        DoOnGetUserText(S);
        Result := CaptionPrefix + S + CaptionSuffix;
        Exit;
      end
      else
        S := '';
  end
  else
    S := '';
  if csDesigning in ComponentState then
    Result := Name
  else
    Result := CaptionDefault;
end;

procedure TAutoStrButton.Loaded;
begin
  inherited;
  if (csDesigning in ComponentState) then
    UpdateCaption;
end;

procedure TAutoStrButton.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (Operation = opRemove) and Assigned(FAdapter) and (AComponent = Iterator)
    then
    Iterator := nil;
end;

procedure TAutoStrButton.SetCaptionDefault(const Value: string);
begin
  FCaptionDefault := Value;
end;

procedure TAutoStrButton.SetCaptionPrefix(const Value: string);
begin
  FCaptionPrefix := Value;
end;

procedure TAutoStrButton.SetCaptionSuffix(const Value: string);
begin
  FCaptionSuffix := Value;
end;

procedure TAutoStrButton.SetIterator(Value: TUtilsStringIterator);
begin
  if not (FAdapter.IteratorFixed and (csLoading in ComponentState)) then
    FAdapter.Iterator := Value;
  if Assigned(Value) then
    Value.FreeNotification(Self);
end;

procedure TAutoStrButton.UpdateCaption;
begin
  Caption := GetStatementText;
end;

function TAutoStrButton.UseRightToLeftAlignment: Boolean;
begin
  Result := SysLocale.MiddleEast and (BiDiMode = bdRightToLeft);
end;

{ TAutoStrMemo }

constructor TAutoStrMemo.Create(AOwner: TComponent);
begin
  inherited;
  ControlStyle := ControlStyle + [csReplicatable];
  AutoSize := False;
  ReadOnly := True;
  FAdapter := TUtilsStringIteratorAdapter.Create;
  with FAdapter do
  begin
    OnActiveChanged := ItemChangedEventHandler;
    OnItemChanged := ItemChangedEventHandler;
  end;
end;

destructor TAutoStrMemo.Destroy;
begin
  FAdapter.Free;
  FAdapter := nil;
  inherited;
end;

procedure TAutoStrMemo.DoOnGetUserText(var S: string);
begin
  if Assigned(FOnGetUserText) then
    FOnGetUserText(Self, Iterator, S);
end;

function TAutoStrMemo.ExecuteAction(Action: TBasicAction): Boolean;
begin
  Result := inherited ExecuteAction(Action) or Assigned(FAdapter) and
    FAdapter.ExecuteAction(Action);
end;

procedure TAutoStrMemo.ForceUpdate;
begin
  UpdateCaption;
end;

procedure TAutoStrMemo.ItemChangedEventHandler(Sender: TObject);
begin
  UpdateCaption;
end;

function TAutoStrMemo.GetIterator: TUtilsStringIterator;
begin
  Result := FAdapter.Iterator;
end;

function TAutoStrMemo.GetStatementText: string;
begin
  if Assigned(Iterator) then
  begin
    with Iterator do
      if not Offlist then
      begin
        Result := Value;
        DoOnGetUserText(Result);
        Exit;
      end
      else
        Result := '';
  end
  else
    Result := '';
  if csDesigning in ComponentState then
    Result := Name
  else
    Result := TextDefault;
end;

procedure TAutoStrMemo.Loaded;
begin
  inherited;
  if (csDesigning in ComponentState) then
    UpdateCaption;
end;

procedure TAutoStrMemo.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (Operation = opRemove) and Assigned(FAdapter) and (AComponent = Iterator)
    then
    Iterator := nil;
end;

procedure TAutoStrMemo.SetIterator(Value: TUtilsStringIterator);
begin
  if not (FAdapter.IteratorFixed and (csLoading in ComponentState)) then
    FAdapter.Iterator := Value;
  if Assigned(Value) then
    Value.FreeNotification(Self);
end;

procedure TAutoStrMemo.SetTextDefault(const Value: string);
begin
  FTextDefault := Value;
end;

function TAutoStrMemo.UpdateAction(Action: TBasicAction): Boolean;
begin
  Result := inherited UpdateAction(Action) or Assigned(FAdapter) and
    FAdapter.UpdateAction(Action);
end;

procedure TAutoStrMemo.UpdateCaption;
begin
  Caption := GetStatementText;
end;

function TAutoStrMemo.UseRightToLeftAlignment: Boolean;
begin
  Result := SysLocale.MiddleEast and (BiDiMode = bdRightToLeft);
end;

{ TAutoStrListBox }

constructor TAutoStrListBox.Create(AOwner: TComponent);
begin
  inherited;
  FAdapter := TUtilsStringIteratorAdapter.Create;
  with FAdapter do
  begin
    OnActiveChanged := ListModifiedEventHandler;
    OnItemChanged := ItemChangedEventHandler;
    OnListModified := ListModifiedEventHandler;
    OnPositionChanged := PositionChangedEventHandler;
  end;
end;

destructor TAutoStrListBox.Destroy;
begin
  FAdapter.Free;
  FAdapter := nil;
  inherited;
end;

procedure TAutoStrListBox.Click;
begin
  inherited;
  if CanNavigate and (ItemIndex <> -1) and Assigned(Iterator) then
  begin
    FFocusUpdating := True;
    try
      Iterator.Go(Integer(Items.Objects[ItemIndex]));
    finally
      FFocusUpdating := False;
    end;
  end;
end;

procedure TAutoStrListBox.DoOnGetUserText(Iterator: TUtilsStringIterator;
  var S: string; var Accept: Boolean);
begin
  if Assigned(FOnGetUserText) then
    FOnGetUserText(Self, Iterator, S, Accept);
end;

function TAutoStrListBox.ExecuteAction(Action: TBasicAction): Boolean;
begin
  Result := inherited ExecuteAction(Action) or Assigned(FAdapter) and
    FAdapter.ExecuteAction(Action);
end;

procedure TAutoStrListBox.ForceUpdate;
begin
  UpdateListBox;
end;

function TAutoStrListBox.GetIterator: TUtilsStringIterator;
begin
  Result := FAdapter.Iterator;
end;

procedure TAutoStrListBox.ItemChangedEventHandler(Sender: TObject);
begin
  UpdateFocus;
end;

procedure TAutoStrListBox.ListModifiedEventHandler(Sender: TObject);
begin
  UpdateListBox;
end;

procedure TAutoStrListBox.Loaded;
begin
  inherited;
  if (csDesigning in ComponentState) then
    ForceUpdate;
end;

procedure TAutoStrListBox.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (Operation = opRemove) and Assigned(FAdapter) and (AComponent = Iterator)
    then
    Iterator := nil;
end;

procedure TAutoStrListBox.PositionChangedEventHandler(Sender: TObject; OldPos,
  NewPos: Integer);
begin
  if FFocusUpdating then
    Exit;
  UpdateFocus;
end;

procedure TAutoStrListBox.SetCanNavigate(const Value: Boolean);
begin
  FCanNavigate := Value;
end;

procedure TAutoStrListBox.SetItemIndex(const Value: Integer);
begin
  if (GetItemIndex <> Value) or (csLoading in ComponentState) then
  begin
    inherited;
    Click;
  end
  else
    inherited;
end;

procedure TAutoStrListBox.SetIterator(Value: TUtilsStringIterator);
begin
  if not (FAdapter.IteratorFixed and (csLoading in ComponentState)) then
    FAdapter.Iterator := Value;
  if Assigned(Value) then
    Value.FreeNotification(Self);
end;

function TAutoStrListBox.UpdateAction(Action: TBasicAction): Boolean;
begin
  Result := inherited UpdateAction(Action) or Assigned(FAdapter) and
    FAdapter.UpdateAction(Action);
end;

procedure TAutoStrListBox.UpdateFocus;
var
  I: Integer;
  P: Integer;
begin
  if CanNavigate and Assigned(Iterator) then
  begin
    P := Iterator.Position;
    if P > -1 then
    begin
      // First, we guess that the items in the TRdfListBox have the same
      // order than in the Source:
      if P < Items.Count then
        if Integer(Items.Objects[P]) = P then
        begin
          ItemIndex := P;
          Exit;
        end;
      // Guessing failed, so we will have to look through all entries:
      for I := 0 to Pred(Items.Count) do
        if Integer(Items.Objects[I]) = P then
        begin
          ItemIndex := I;
          Exit;
        end;
    end;
    ItemIndex := -1;
  end
  else
    ItemIndex := -1;
end;

procedure TAutoStrListBox.UpdateListBox;
var
  Accept: Boolean;
  S: string;
  TempIterator: TUtilsStringIterator;
begin
  Items.BeginUpdate;
  try
    Items.Clear;
    if Assigned(Iterator) then
    begin
      TempIterator := TUtilsStringIterator.Create(nil);
      try
        with TempIterator do
        begin
          List := Iterator.List;
          First;
          while not Offright do
          begin
            Accept := True;
            S := Value;
            DoOnGetUserText(TempIterator, S, Accept);
            if Accept then
              if CanNavigate and (Position = Iterator.Position) then
{$IFDEF CLR}
                ItemIndex := Items.AddObject(S, TObject(Position))
              else
                Items.AddObject(S, TObject(Position));
{$ELSE}
                ItemIndex := Items.AddObject(S, Pointer(Position))
              else
                Items.AddObject(S, Pointer(Position));
{$ENDIF}
            Next;
          end;
        end;
      finally
        TempIterator.Free;
      end;
    end;
  finally
    Items.EndUpdate
  end;
end;

function TAutoStrListBox.UseRightToLeftAlignment: Boolean;
begin
  Result := SysLocale.MiddleEast and (BiDiMode = bdRightToLeft);
end;

{ TAutoStrComboBox }

constructor TAutoStrComboBox.Create(AOwner: TComponent);
begin
  inherited;
  Style := csDropDownList;
  FAdapter := TUtilsStringIteratorAdapter.Create;
  with FAdapter do
  begin
    OnActiveChanged := ListModifiedEventHandler;
    OnItemChanged := ItemChangedEventHandler;
    OnListModified := ListModifiedEventHandler;
    OnPositionChanged := PositionChangedEventHandler;
  end;
end;

destructor TAutoStrComboBox.Destroy;
begin
  FAdapter.Free;
  FAdapter := nil;
  inherited;
end;

procedure TAutoStrComboBox.Click;
begin
  inherited;
  if CanNavigate and (ItemIndex <> -1) and Assigned(Iterator) then
  begin
    FFocusUpdating := True;
    try
      Iterator.Go(Integer(Items.Objects[ItemIndex]));
    finally
      FFocusUpdating := False;
    end;
  end;
end;

procedure TAutoStrComboBox.DoOnGetUserText(Iterator: TUtilsStringIterator;
  var S: string; var Accept: Boolean);
begin
  if Assigned(FOnGetUserText) then
    FOnGetUserText(Self, Iterator, S, Accept);
end;

procedure TAutoStrComboBox.ForceUpdate;
begin
  UpdateComboBox;
end;

function TAutoStrComboBox.GetIterator: TUtilsStringIterator;
begin
  Result := FAdapter.Iterator;
end;

procedure TAutoStrComboBox.ItemChangedEventHandler(Sender: TObject);
begin
  UpdateFocus;
end;

procedure TAutoStrComboBox.ListModifiedEventHandler(Sender: TObject);
begin
  UpdateComboBox;
end;

procedure TAutoStrComboBox.Loaded;
begin
  inherited;
  if (csDesigning in ComponentState) then
    UpdateComboBox;
end;

procedure TAutoStrComboBox.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (Operation = opRemove) and Assigned(FAdapter) and (AComponent = Iterator)
    then
    Iterator := nil;
end;

procedure TAutoStrComboBox.PositionChangedEventHandler(Sender: TObject; OldPos,
  NewPos: Integer);
begin
  if FFocusUpdating then
    Exit;
  UpdateFocus;
end;

procedure TAutoStrComboBox.SetCanNavigate(const Value: Boolean);
begin
  FCanNavigate := Value;
end;

procedure TAutoStrComboBox.SetIterator(Value: TUtilsStringIterator);
begin
  if not (FAdapter.IteratorFixed and (csLoading in ComponentState)) then
    FAdapter.Iterator := Value;
  if Assigned(Value) then
    Value.FreeNotification(Self);
end;

procedure TAutoStrComboBox.UpdateComboBox;
var
  Accept: Boolean;
  S: string;
  TempIterator: TUtilsStringIterator;
begin
  Items.BeginUpdate;
  try
    Items.Clear;
    if Assigned(Iterator) then
    begin
      TempIterator := TUtilsStringIterator.Create(nil);
      try
        with TempIterator do
        begin
          List := Iterator.List;
          First;
          while not Offright do
          begin
            Accept := True;
            S := Value;
            DoOnGetUserText(TempIterator, S, Accept);
            if Accept then
              if CanNavigate and (Position = Iterator.Position) then
{$IFDEF CLR}
                ItemIndex := Items.AddObject(S, TObject(Position))
              else
                Items.AddObject(S, TObject(Position));
{$ELSE}
                ItemIndex := Items.AddObject(S, Pointer(Position))
              else
                Items.AddObject(S, Pointer(Position));
{$ENDIF}
            Next;
          end;
        end;
      finally
        TempIterator.Free;
      end;
    end;
  finally
    Items.EndUpdate
  end;
end;

procedure TAutoStrComboBox.UpdateFocus;
var
  I: Integer;
  P: Integer;
begin
  if CanNavigate and Assigned(Iterator) then
  begin
    P := Iterator.Position;
    if P > -1 then
    begin
      // First, we guess that the items in the TRdfListBox have the same
      // order than in the Source:
      if P < Items.Count then
        if Integer(Items.Objects[P]) = P then
        begin
          ItemIndex := P;
          Exit;
        end;
      // Guessing failed, so we will have to look through all entries:
      for I := 0 to Pred(Items.Count) do
        if Integer(Items.Objects[I]) = P then
        begin
          ItemIndex := I;
          Exit;
        end;
    end;
    ItemIndex := -1;
  end
  else
    ItemIndex := -1;
end;

function TAutoStrComboBox.UseRightToLeftAlignment: Boolean;
begin
  Result := SysLocale.MiddleEast and (BiDiMode = bdRightToLeft);
end;

procedure TAutoStrComboBox.SetItemIndex(const Value: Integer);
begin
  if (GetItemIndex <> Value) or (csLoading in ComponentState) then
  begin
    inherited;
    Click;
  end
  else
    inherited;
end;

{$IFNDEF VER140}
{ TAutoStrTabControl }

constructor TAutoStrTabControl.Create(AOwner: TComponent);
begin
  inherited;
  FAdapter := TUtilsStringIteratorAdapter.Create;
  with FAdapter do
  begin
    OnActiveChanged := ListModifiedEventHandler;
    OnItemChanged := ItemChangedEventHandler;
    OnListModified := ListModifiedEventHandler;
    OnPositionChanged := PositionChangedEventHandler;
  end;
end;

destructor TAutoStrTabControl.Destroy;
begin
  FAdapter.Free;
  FAdapter := nil;
  inherited;
end;

procedure TAutoStrTabControl.Change;
begin
  inherited;
  if CanNavigate and (TabIndex <> -1) and Assigned(Iterator) then
  begin
    FFocusUpdating := True;
    try
      Iterator.Go(Integer(Tabs.Objects[TabIndex]));
    finally
      FFocusUpdating := False;
    end;
  end;
end;

procedure TAutoStrTabControl.DoOnGetUserText(Iterator: TUtilsStringIterator;
  var S: string; var Accept: Boolean);
begin
  if Assigned(FOnGetUserText) then
    FOnGetUserText(Self, Iterator, S, Accept);
end;

procedure TAutoStrTabControl.ForceUpdate;
begin
  UpdateTabs;
end;

function TAutoStrTabControl.GetIterator: TUtilsStringIterator;
begin
  Result := FAdapter.Iterator;
end;

function TAutoStrTabControl.GetTabs: TStrings;
begin
  Result := inherited Tabs;
end;

procedure TAutoStrTabControl.ItemChangedEventHandler(Sender: TObject);
begin
  UpdateFocus;
end;

procedure TAutoStrTabControl.ListModifiedEventHandler(Sender: TObject);
begin
  UpdateTabs;
end;

procedure TAutoStrTabControl.Loaded;
begin
  inherited;
  if (csDesigning in ComponentState) then
    UpdateTabs;
end;

procedure TAutoStrTabControl.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (Operation = opRemove) and Assigned(FAdapter) and (AComponent = Iterator)
    then
    Iterator := nil;
end;

procedure TAutoStrTabControl.PositionChangedEventHandler(Sender: TObject;
  OldPos,
  NewPos: Integer);
begin
  if FFocusUpdating then
    Exit;
  UpdateFocus;
end;

procedure TAutoStrTabControl.SetCanNavigate(const Value: Boolean);
begin
  FCanNavigate := Value;
end;

procedure TAutoStrTabControl.SetIterator(Value: TUtilsStringIterator);
begin
  if not (FAdapter.IteratorFixed and (csLoading in ComponentState)) then
    FAdapter.Iterator := Value;
  if Assigned(Value) then
    Value.FreeNotification(Self);
end;

procedure TAutoStrTabControl.SetSorted(const Value: Boolean);
begin
  if FSorted <> Value then
  begin
    FSorted := Value;
    UpdateTabs;
  end;
end;

procedure TAutoStrTabControl.SetTabIndex(Value: Integer);
begin
  if (TabIndex <> Value) or (csLoading in ComponentState) then
  begin
    inherited;
    Change;
  end
  else
    inherited;
end;

procedure TAutoStrTabControl.UpdateFocus;
var
  I: Integer;
  P: Integer;
begin
  if CanNavigate and Assigned(Iterator) then
  begin
    P := Iterator.Position;
    if P > -1 then
    begin
      // First, we guess that the tabs in the TRdfListBox have the same
      // order than in the Iterator:
      if P < Tabs.Count then
        if Integer(Tabs.Objects[P]) = P then
        begin
          TabIndex := P;
          Exit;
        end;
      // Guessing failed, so we will have to look through all entries:
      for I := 0 to Pred(Tabs.Count) do
        if Integer(Tabs.Objects[I]) = P then
        begin
          TabIndex := I;
          Exit;
        end;
    end;
    TabIndex := -1;
  end
  else
    TabIndex := -1;
end;

procedure TAutoStrTabControl.UpdateTabs;

  procedure UpdateStrings(SL: TStrings);
  var
    Accept: Boolean;
    S: string;
    TempIterator: TUtilsStringIterator;
  begin
    if Assigned(Iterator) then
    begin
      TempIterator := TUtilsStringIterator.Create(nil);
      try
        with TempIterator do
        begin
          List := Iterator.List;
          First;
          while not Offright do
          begin
            Accept := True;
            S := Value;
            DoOnGetUserText(TempIterator, S, Accept);
            if Accept then
{$IFDEF CLR}
              SL.AddObject(S, TObject(Position));
{$ELSE}
              SL.AddObject(S, Pointer(Position));
{$ENDIF}
            Next;
          end;
        end;
      finally
        TempIterator.Free;
      end;
    end;
  end;

var
  SL: TStringList;
begin
  Tabs.BeginUpdate;
  try
    Tabs.Clear;
    if Assigned(Iterator) then
    begin
      if Sorted then
      begin
        SL := TStringList.Create;
        try
          SL.Sorted := True;
          SL.Duplicates := dupAccept;
          UpdateStrings(SL);
          Tabs.Assign(SL);
        finally
          SL.Free;
        end;
      end
      else
        UpdateStrings(Tabs);
      UpdateFocus;
    end;
  finally
    Tabs.EndUpdate
  end;
end;

function TAutoStrTabControl.UseRightToLeftAlignment: Boolean;
begin
  Result := SysLocale.MiddleEast and (BiDiMode = bdRightToLeft);
end;
{$ENDIF}

{ TAutoStrTrackBar }

constructor TAutoStrTrackBar.Create(AOwner: TComponent);
begin
  inherited;
  FAdapter := TUtilsStandardIteratorAdapter.Create;
  with FAdapter do
  begin
    OnActiveChanged := ActiveChangedEventHandler;
    OnListModified := ActiveChangedEventHandler;
    OnPositionChanged := PositionChangedEventHandler;
  end;
end;

destructor TAutoStrTrackBar.Destroy;
begin
  FAdapter.Free;
  FAdapter := nil;
  inherited;
end;

procedure TAutoStrTrackBar.ActiveChangedEventHandler(Sender: TObject);
begin
  UpdateTrackBar;
end;

procedure TAutoStrTrackBar.Changed;
begin
  inherited;
  if Assigned(Iterator) then
  begin
    if CanNavigate and (Position < Iterator.Count) then
    begin
      FPositionUpdating := True;
      try
        Iterator.Go(Position);
      finally
        FPositionUpdating := False;
      end;
    end
    else
      Position := Iterator.Position;
  end;
end;

function TAutoStrTrackBar.GetIterator: TUtilsCustomIterator;
begin
  Result := FAdapter.Iterator;
end;

procedure TAutoStrTrackBar.Loaded;
begin
  inherited;
  if (csDesigning in ComponentState) then
    UpdateTrackBar;
end;

procedure TAutoStrTrackBar.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (Operation = opRemove) and Assigned(FAdapter) and (AComponent = Iterator)
    then
    Iterator := nil;
end;

procedure TAutoStrTrackBar.PositionChangedEventHandler(Sender: TObject; OldPos,
  NewPos: Integer);
begin
  if FPositionUpdating then
    Exit;
  Position := NewPos;
end;

procedure TAutoStrTrackBar.SetCanNavigate(const Value: Boolean);
begin
  FCanNavigate := Value;
end;

procedure TAutoStrTrackBar.SetIterator(Value: TUtilsCustomIterator);
begin
  if not (FAdapter.IteratorFixed and (csLoading in ComponentState)) then
    FAdapter.Iterator := Value;
  if Assigned(Value) then
    Value.FreeNotification(Self);
end;

procedure TAutoStrTrackBar.UpdateTrackBar;
begin
  if Assigned(Iterator) then
  begin
    Min := 0;
    Max := Pred(Iterator.Count);
    Position := Iterator.Position;
  end
  else
  begin
    Min := 0;
    Max := 0;
    Position := 0;
  end;
end;

{ TAutoStrNavigator }

{$R AutoStrCtrls.res} // Include resource file containing the navigator's button glyphs.

resourcestring

  SAddStatement = 'Add statement';
  SConfirmDelete = 'Delete statement?';
  SDeleteStatement = 'Delete statement';
  SEditStatement = 'Edit statement';
  SFirstStatement = 'First statement';
  SLastStatement = 'Last statement';
  SNextStatement = 'Next statement';
  SPriorStatement = 'Prior statement';

var
{$IFDEF CLR}
  BtnTypeName: array[TAutoStrNavBtnType] of string = ('FIRST', 'PRIOR', 'NEXT',
    'LAST', 'ADD', 'DELETE', 'EDIT');
  BtnHintId: array[TAutoStrNavBtnType] of string = (SFirstStatement,
    SPriorStatement, SNextStatement, SLastStatement, SAddStatement,
    SDeleteStatement, SEditStatement);
{$ELSE}
  BtnTypeName: array[TAutoStrNavBtnType] of PChar = ('FIRST', 'PRIOR', 'NEXT',
    'LAST', 'ADD', 'DELETE', 'EDIT');
  { TODO 5 -cRefactoring : Change to array of string? }
  BtnHintId: array[TAutoStrNavBtnType] of Pointer = (@SFirstStatement,
    @SPriorStatement,
    @SNextStatement, @SLastStatement, @SAddStatement, @SDeleteStatement,
      @SEditStatement);
  { TODO 5 -cRefactoring : Change to array of string? }
{$ENDIF}

constructor TAutoStrNavigator.Create(AOwner: TComponent);
begin
  inherited;
  ControlStyle := ControlStyle - [csAcceptsControls, csSetCaption] + [csOpaque];
  if not NewStyleControls then
    ControlStyle := ControlStyle + [csFramed];
  FAdapter := TUtilsStandardIteratorAdapter.Create;
  with FAdapter do
  begin
    OnActiveChanged := ActiveChangedEventHandler;
    OnListModified := ActiveChangedEventHandler;
    OnPositionChanged := PositionChangedEventHandler;
  end;
  FVisibleButtons := [nbtFirst, nbtPrior, nbtNext, nbtLast, nbtAdd, nbtDelete,
    nbtEdit];
  FHints := TStringList.Create;
  TStringList(FHints).OnChange := InitHints;
  InitButtons;
  InitHints(Self);
  BevelOuter := bvNone;
  BevelInner := bvNone;
  Width := 241;
  Height := 25;
  FButtonWidth := 0;
  FFocusedBtn := nbtFirst;
  FConfirmDelete := True;
  FullRepaint := False;
end;

destructor TAutoStrNavigator.Destroy;
begin
  FAdapter.Free;
  FAdapter := nil;
  FHints.Free;
  FDefHints.Free;
  inherited;
end;

procedure TAutoStrNavigator.ActiveChanged;
var
  Btn: TAutoStrNavBtnType;
begin
  if Assigned(Iterator) then
    if Iterator.UpdateCount = 0 then
    begin
      SourceChanged;
      Exit;
    end;
  for Btn := Low(Buttons) to High(Buttons) do
    Buttons[Btn].Enabled := False;
end;

procedure TAutoStrNavigator.ActiveChangedEventHandler(Sender: TObject);
begin
  ActiveChanged;
end;

procedure TAutoStrNavigator.BtnClick(Index: TAutoStrNavBtnType);
begin
  if Assigned(Iterator) then
  begin
    if Iterator.UpdateCount = 0 then
    begin
      if not (csDesigning in ComponentState) and Assigned(FBeforeAction) then
        FBeforeAction(Self, Index);
      case Index of
        nbtPrior: Iterator.Prior;
        nbtNext: Iterator.Next;
        nbtFirst: Iterator.First;
        nbtLast: Iterator.Last;
        nbtAdd: DoAdd;
        nbtEdit: DoEdit;
        nbtDelete: DoDelete;
      end;
    end;
  end;
  if not (csDesigning in ComponentState) and Assigned(FOnClick) then
    FOnClick(Self, Index);
end;

procedure TAutoStrNavigator.CalcMinSize(var W, H: Integer);
var
  Count: Integer;
  NBT: TAutoStrNavBtnType;
begin
  if (csLoading in ComponentState) then
    Exit;
  if not Assigned(Buttons[nbtFirst]) then
    Exit;

  Count := 0;
  for NBT := Low(Buttons) to High(Buttons) do
    if Buttons[NBT].Visible then
      Inc(Count);
  if Count = 0 then
    Inc(Count);

  W := Max(W, Count * FMinBtnSize);
  H := Max(H, FMinBtnSize);

  if Align = alNone then
    W := (W div Count) * Count;
end;

procedure TAutoStrNavigator.CMEnabledChanged(var Message: TMessage);
begin
  inherited;
  if not (csLoading in ComponentState) then
    ActiveChanged;
end;

procedure TAutoStrNavigator.DoAdd;
var
  S: string;
  Action: TAutoStrNavAction;
begin
  (* xxx ooo
  if Assigned(Iterator) then
    if Iterator.CanModify then begin
      Action := naCancel;
      if Assigned(FOnAdd) then
        repeat
          S := '';
          FOnAdd(Self, S, Action);
        until Action <> naRetry;
      if Action = naOK then
        Source.AutoStrings.InsertRight(S);
    end;
    *)
end;

procedure TAutoStrNavigator.DoDelete;
begin
  (* xxx ooo
  if FConfirmDelete and
    ( MessageDlg(GetConfirmDeleteStr, mtConfirmation, mbOKCancel, 0) <> mrOK ) then
    exit;
  if Assigned(Iterator) then
    if Iterator.CanModify then Source.AutoStrings.Delete;
  *)
end;

procedure TAutoStrNavigator.DoEdit;
var
  Action: TAutoStrNavAction;
  S: string;
begin
  (* xxx ooo
  if Assigned(Iterator) then
    with Iterator do
      if CanModify and not Offlist then begin
        Action := naCancel;
        if Assigned(FOnEdit) then
          repeat
            S := Value;
            FOnEdit(Self, S, Action);
          until Action <> naRetry;
        if Action = naOK then
          ChangeValue(S);
      end;
      *)
end;

procedure TAutoStrNavigator.GetChildren(Proc: TGetChildProc;
  Root: TComponent);
begin
  // Hide the navigator buttons.
end;

function TAutoStrNavigator.GetConfirmDeleteStr: string;
begin
  if FConfirmDeleteStr = '' then
    Result := SConfirmDelete
  else
    Result := FConfirmDeleteStr;
end;

function TAutoStrNavigator.GetHints: TStrings;
begin
  if (csDesigning in ComponentState) and not (csWriting in ComponentState) and
    not (csReading in ComponentState) and (FHints.Count = 0) then
    Result := FDefHints
  else
    Result := FHints;
end;

function TAutoStrNavigator.GetIterator: TUtilsCustomIterator;
begin
  Result := FAdapter.Iterator;
end;

procedure TAutoStrNavigator.InitButtons;
var
  BtnType: TAutoStrNavBtnType;
  Btn: TAutoStrNavBtn;
  Offset: Integer;
  ResName: string;
{$IFDEF CLR}
  BaseAssembly: System.Reflection.Assembly;
{$ENDIF}
begin
  FMinBtnSize := 20;
  Offset := 0;
  for BtnType := Low(Buttons) to High(Buttons) do
  begin
    Btn := TAutoStrNavBtn.Create(Self);
    Btn.BtnType := BtnType;
    Btn.Flat := Flat;
    Btn.Visible := BtnType in FVisibleButtons;
    Btn.Enabled := True;
    Btn.SetBounds(Offset, 0, FMinBtnSize, FMinBtnSize);
    FmtStr(ResName, 'N_1_%s', [BtnTypeName[BtnType]]);
{$IFDEF CLR}
    BaseAssembly := Assembly.GetExecutingAssembly;
    Btn.Glyph.LoadFromResourceName(ResName, 'AutoStrCtrls', BaseAssembly);
{$ELSE}
    Btn.Glyph.LoadFromResourceName(HInstance, ResName);
{$ENDIF}
    Btn.NumGlyphs := 2;
    Btn.Enabled := True;
    Btn.OnClick := NavBtnOnClickHandler;
    Btn.OnMouseDown := NavBtnOnMouseDownHandler;
    Btn.Parent := Self;
    Buttons[BtnType] := Btn;
    Offset := Offset + FMinBtnSize;
  end;
end;

procedure TAutoStrNavigator.InitHints(Sender: TObject);
var
  I: Integer;
  NBT: TAutoStrNavBtnType;
begin
  if not Assigned(FDefHints) then
  begin
    FDefHints := TStringList.Create;
    for NBT := Low(Buttons) to High(Buttons) do
{$IFDEF CLR}
      FDefHints.Add(BtnHintId[NBT]);
{$ELSE}
      FDefHints.Add(LoadResString(BtnHintId[NBT]));
{$ENDIF}
  end;
  for NBT := Low(Buttons) to High(Buttons) do
    Buttons[NBT].Hint := FDefHints[Ord(NBT)];
  NBT := Low(Buttons);
  for I := 0 to Pred(FHints.Count) do
  begin
    if FHints.Strings[I] <> '' then
      Buttons[NBT].Hint := FHints.Strings[I];
    if NBT = High(Buttons) then
      Exit;
    Inc(NBT);
  end;
end;

procedure TAutoStrNavigator.KeyDown(var Key: Word; Shift: TShiftState);
var
  NewFocus: TAutoStrNavBtnType;
  OldFocus: TAutoStrNavBtnType;
begin
  OldFocus := FFocusedBtn;
  case Key of
    VK_RIGHT:
      begin
        if OldFocus < High(Buttons) then
        begin
          NewFocus := OldFocus;
          repeat
            NewFocus := Succ(NewFocus);
          until (NewFocus = High(Buttons)) or (Buttons[NewFocus].Visible);
          if Buttons[NewFocus].Visible then
          begin
            FFocusedBtn := NewFocus;
            Buttons[OldFocus].Invalidate;
            Buttons[NewFocus].Invalidate;
          end;
        end;
      end;
    VK_LEFT:
      begin
        NewFocus := FFocusedBtn;
        repeat
          if NewFocus > Low(Buttons) then
            NewFocus := Pred(NewFocus);
        until (NewFocus = Low(Buttons)) or (Buttons[NewFocus].Visible);
        if NewFocus <> FFocusedBtn then
        begin
          FFocusedBtn := NewFocus;
          Buttons[OldFocus].Invalidate;
          Buttons[FFocusedBtn].Invalidate;
        end;
      end;
    VK_SPACE, VK_RETURN:
      begin
        if Buttons[FFocusedBtn].Enabled then
          Buttons[FFocusedBtn].Click;
      end;
  end;
end;

procedure TAutoStrNavigator.Loaded;
var
  W, H: Integer;
begin
  inherited Loaded;
  W := Width;
  H := Height;
  SetSize(W, H);
  if (W <> Width) or (H <> Height) then
    inherited SetBounds(Left, Top, W, H);
  InitHints(Self);
  ActiveChanged;
end;

procedure TAutoStrNavigator.NavBtnOnClickHandler(Sender: TObject);
begin
  BtnClick(TAutoStrNavBtn(Sender).BtnType);
end;

procedure TAutoStrNavigator.NavBtnOnMouseDownHandler(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
var
  OldFocus: TAutoStrNavBtnType;
begin
  OldFocus := FFocusedBtn;
  FFocusedBtn := TAutoStrNavBtn(Sender).BtnType;
  if TabStop and (GetFocus <> Handle) and CanFocus then
  begin
    SetFocus;
    if (GetFocus <> Handle) then
      Exit;
  end
  else if TabStop and (GetFocus = Handle) and (OldFocus <> FFocusedBtn) then
  begin
    Buttons[OldFocus].Invalidate;
    Buttons[FFocusedBtn].Invalidate;
  end;
end;

procedure TAutoStrNavigator.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (Operation = opRemove) and Assigned(FAdapter) and (AComponent = Iterator)
    then
    Iterator := nil;
end;

procedure TAutoStrNavigator.PositionChangedEventHandler(Sender: TObject; OldPos,
  NewPos: Integer);
begin
  SourceChanged;
end;

procedure TAutoStrNavigator.SetBounds(ALeft, ATop, AWidth,
  AHeight: Integer);
var
  W, H: Integer;
begin
  W := AWidth;
  H := AHeight;
  if not HandleAllocated then
    SetSize(W, H);
  inherited SetBounds(ALeft, ATop, W, H);
end;

procedure TAutoStrNavigator.SetConfirmDeleteStr(const Value: string);
begin
  FConfirmDeleteStr := Trim(Value);
end;

procedure TAutoStrNavigator.SetFlat(Value: Boolean);
var
  I: TAutoStrNavBtnType;
begin
  if FFlat <> Value then
  begin
    FFlat := Value;
    for I := Low(Buttons) to High(Buttons) do
      Buttons[I].Flat := Value;
  end;
end;

procedure TAutoStrNavigator.SetHints(Value: TStrings);
begin
  if Value.Text = FDefHints.Text then
    FHints.Clear
  else
    FHints.Assign(Value);
end;

procedure TAutoStrNavigator.SetSize(var W, H: Integer);
var
  Count: Integer;
  NBT: TAutoStrNavBtnType;
  Space, Temp, Remain: Integer;
  Offset: Integer;
begin
  if (csLoading in ComponentState) then
    Exit;
  if not Assigned(Buttons[nbtFirst]) then
    Exit;

  CalcMinSize(W, H);

  Count := 0;
  for NBT := Low(Buttons) to High(Buttons) do
    if Buttons[NBT].Visible then
      Inc(Count);
  if Count = 0 then
    Inc(Count);

  FButtonWidth := W div Count;
  Temp := Count * FButtonWidth;
  if Align = alNone then
    W := Temp;

  Offset := 0;
  Remain := W - Temp;
  Temp := Count div 2;
  for NBT := Low(Buttons) to High(Buttons) do
  begin
    if Buttons[NBT].Visible then
    begin
      Space := 0;
      if Remain <> 0 then
      begin
        Dec(Temp, Remain);
        if Temp < 0 then
        begin
          Inc(Temp, Count);
          Space := 1;
        end;
      end;
      Buttons[NBT].SetBounds(Offset, 0, FButtonWidth + Space, Height);
      Inc(Offset, FButtonWidth + Space);
    end
    else
      Buttons[NBT].SetBounds(Width + 1, 0, FButtonWidth, Height);
  end;
end;

procedure TAutoStrNavigator.SetIterator(Value: TUtilsCustomIterator);
begin
  if not (FAdapter.IteratorFixed and (csLoading in ComponentState)) then
    FAdapter.Iterator := Value;
  if Assigned(Value) then
    Value.FreeNotification(Self);
end;

procedure TAutoStrNavigator.SetVisible(Value: TAutoStrNavBtnTypes);
var
  NBT: TAutoStrNavBtnType;
  W, H: Integer;
begin
  W := Width;
  H := Height;
  FVisibleButtons := Value;
  for NBT := Low(Buttons) to High(Buttons) do
    Buttons[NBT].Visible := NBT in FVisibleButtons;
  SetSize(W, H);
  if (W <> Width) or (H <> Height) then
    inherited SetBounds(Left, Top, W, H);
  Invalidate;
end;

procedure TAutoStrNavigator.SourceChanged;
var
  ScrlBwd, ScrlFwd, Mdfy, Add: Boolean;
begin
  if Assigned(Iterator) then
  begin
    with Iterator do
    begin
      ScrlBwd := not (Offleft or IsFirst);
      ScrlFwd := not (Offright or IsLast);
      // xxx ooo      Mdfy    := Iterator.CanModify and not (Offleft or Offright);
      // xxx ooo      Add     := Iterator.CanModify;
    end;
  end
  else
  begin
    ScrlBwd := False;
    ScrlFwd := False;
    // xxx ooo    Mdfy := False;
    // xxx ooo    Add := False;
  end;
  Buttons[nbtFirst].Enabled := ScrlBwd;
  Buttons[nbtPrior].Enabled := ScrlBwd;
  Buttons[nbtNext].Enabled := ScrlFwd;
  Buttons[nbtLast].Enabled := ScrlFwd;
  // xxx ooo  Buttons[nbtDelete].Enabled := Mdfy;
  // xxx ooo  Buttons[nbtAdd].Enabled    := Add;
  // xxx ooo  Buttons[nbtEdit].Enabled   := Mdfy;
end;

procedure TAutoStrNavigator.WMGetDlgCode(var Message: TWMGetDlgCode);
begin
  Message.Result := DLGC_WANTARROWS;
end;

procedure TAutoStrNavigator.WMKillFocus(var Message: TWMKillFocus);
begin
  Buttons[FFocusedBtn].Invalidate;
end;

procedure TAutoStrNavigator.WMSetFocus(var Message: TWMSetFocus);
begin
  Buttons[FFocusedBtn].Invalidate;
end;

procedure TAutoStrNavigator.WMSize(var Message: TWMSize);
var
  W, H: Integer;
begin
  inherited;
  W := Width;
  H := Height;
  SetSize(W, H);
end;

{$IFDEF CLR}

procedure TAutoStrNavigator.WMWindowPosChanging(
  var Message: TWMWindowPosChanging);
var
  WPos: TWindowPos;
begin
  inherited;
  if (SWP_NOSIZE and Message.WindowPos.Flags) = 0 then
  begin
    WPos := Message.WindowPos;
    CalcMinSize(WPos.cx, WPos.cy);
    Message.WindowPos := WPos;
  end;
end;

{$ELSE}

procedure TAutoStrNavigator.WMWindowPosChanging(
  var Message: TWMWindowPosChanging);
begin
  inherited;
  if (SWP_NOSIZE and Message.WindowPos.Flags) = 0 then
    CalcMinSize(Message.WindowPos.cx, Message.WindowPos.cy);
end;

{$ENDIF}

end.

