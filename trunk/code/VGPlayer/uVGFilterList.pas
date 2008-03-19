unit uVGFilterList;

{$WARN SYMBOL_PLATFORM OFF}

interface

uses
  Windows, Classes, SysUtils, Contnrs, ActiveX, WideStrings,
  DirectShow9, uVGBase;

type
  TVGFilterList = class
  private
    FItems: TObjectList;
    FPos: Integer;
    function GetCount: Integer;
  public
    constructor Create(AOwnsObjects: Boolean = False);
    destructor Destroy; override;
    function Add(AFilter: TVGFilter): Integer;
    procedure Clear;
    function Get(AIndex: Integer): TVGFilter;
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

procedure TVGFilterList.Clear;
begin
  FItems.Clear;
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

end.
