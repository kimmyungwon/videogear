unit uVGMTLookup;

{$DEFINE __ARIA_HASHSET_TEMPLATE__}

interface

uses
  SysUtils, ActiveX, uVGBase;

type
  _ITEM_TYPE_ = TVGMediaType;
  _DATA_TYPE_ = TVGFilter;

  {$INCLUDE 'tAriaHashSet.tpl'}
  TAriaMediaTypeLookup = class(_ARIA_HASHSET_TEMPLATE_)
  protected
    function HashItem(const AItem: _ITEM_TYPE_): Cardinal; override;
    function IsNull(const AItem: _ITEM_TYPE_): Boolean; override;
    function IsSame(const AItem1, AItem2: _ITEM_TYPE_): Boolean; override;
  end;

implementation

{$INCLUDE 'tAriaHashSet.tpl'}

{ TAriaMTHashSet }

function TAriaMediaTypeLookup.HashItem(const AItem: _ITEM_TYPE_): Cardinal;
var
  pPtr: PByteArray;
  I: Integer;
begin
  Result := 0;
  pPtr := PByteArray(@AItem);
  for I := 0 to SizeOf(AItem) - 1 do
  begin
    Result := (Result shl 4) + pPtr[I];
  end;
end;

function TAriaMediaTypeLookup.IsNull(const AItem: _ITEM_TYPE_): Boolean;
begin
  Result := IsEqualCLSID(AItem.clsMajorType, GUID_NULL) and
    IsEqualCLSID(AItem.clsMinorType, GUID_NULL);
end;

function TAriaMediaTypeLookup.IsSame(const AItem1, AItem2: _ITEM_TYPE_): Boolean;
begin
  Result := IsEqualCLSID(AItem1.clsMajorType, AItem2.clsMajorType) and
    IsEqualCLSID(AItem1.clsMinorType, AItem2.clsMinorType);
end;

end.
