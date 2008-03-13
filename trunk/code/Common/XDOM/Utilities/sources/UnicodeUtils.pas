unit UnicodeUtils;

// UnicodeUtilsRTL 4.0.2
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
// The Original Code is "UnicodeUtilsRTL.pas".
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
// 2007-12-03 4.0.2 Made .NET compliant. Conversion stream classes deleted.
// 2004-09-19 4.0.1 StrToEncoding bug fixed.
// 2004-06-01 4.0.0 General reconstruction.

interface

uses
{$IFDEF CLR}
  cUnicodeCodecsRTL,
{$ELSE}
  cUnicodeCodecsWin32,
{$ENDIF}
  SysUtils, Classes;

function EncodingToStr(const CodecClass: TUnicodeCodecClass): string;
function StrToEncoding(const S: string): TUnicodeCodecClass;

function UTF16HighSurrogate(const Value: Integer): WideChar;
function UTF16LowSurrogate(const Value: Integer): WideChar;
function UTF16SurrogateToInt(const HighSurrogate, LowSurrogate: WideChar):
  Integer;
function IsUTF16HighSurrogate(const S: WideChar): Boolean;
function IsUTF16LowSurrogate(const S: WideChar): Boolean;

function UTF8ToUTF16BEStr(const S: string): WideString;

type
  ECSMIBException = Exception;

  TCSMIBChangingEvent = procedure(Sender: TObject;
    NewEnum: Integer;
    var AllowChange: Boolean) of object;

  TCSMIB = class(TComponent)
  protected
    FEnum: Integer;
    FIgnoreInvalidEnum: Boolean;
    FOnChanging: TCSMIBChangingEvent;
    FOnChange: TNotifyEvent;
    procedure DoChange(Sender: TObject); virtual;
    procedure DoChanging(Sender: TObject;
      NewEnum: Integer;
      var AllowChange: Boolean); virtual;
    function GetPrfMIMEName: string; virtual;
    function GetAlias(I: Integer): string; virtual;
    function GetAliasCount: Integer; virtual;
    procedure SetEnum(const Value: Integer); virtual;
    procedure SetOnChange(const Value: TNotifyEvent); virtual;
    procedure SetOnChanging(const Value: TCSMIBChangingEvent); virtual;
  public
    constructor Create(AOwner: TComponent); override;
    function IsValidEnum(const Value: Integer): Boolean; virtual;
    function SetToAlias(const S: string): Boolean; virtual;
    property Alias[I: Integer]: string read GetAlias;
    property AliasCount: Integer read GetAliasCount;
    property PreferredMIMEName: string read GetPrfMIMEName;
  published
    property OnChange: TNotifyEvent read FOnChange write SetOnChange;
    property OnChanging: TCSMIBChangingEvent read FOnChanging write
      SetOnChanging;
    property Enum: Integer read FEnum write SetEnum;
    property IgnoreInvalidEnum: Boolean read FIgnoreInvalidEnum write
      FIgnoreInvalidEnum;
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('Unicode', [TCSMIB]);
end;

// +++++++++++++++++++ Encoding Detection Functions +++++++++++++++++++++

function EncodingToStr(const CodecClass: TUnicodeCodecClass): string;
begin
  Result := GetEncodingName(CodecClass);
  if Result = '' then
    raise EConvertError.Create('Invalid encoding.');
end;

function StrToEncoding(const S: string): TUnicodeCodecClass;
var
  Csmib: TCSMIB;
begin
  Result := GetCodecClassByAlias(S);

  if not Assigned(Result) then
  begin
    Csmib := TCSMIB.Create(nil);
    try
      if Csmib.SetToAlias(S) then
      begin
        case Csmib.Enum of
          3: Result := TUSASCIICodec;
          4: Result := TISO8859_1Codec;
          5: Result := TISO8859_2Codec;
          6: Result := TISO8859_3Codec;
          7: Result := TISO8859_4Codec;
          8: Result := TISO8859_5Codec;
          9: Result := TISO8859_6Codec;
          10: Result := TISO8859_7Codec;
          11: Result := TISO8859_8Codec;
          12: Result := TISO8859_9Codec;
          13: Result := TISO8859_10Codec;
          15: Result := TJIS_X0201Codec;
          106: Result := TUTF8Codec;
          109: Result := TISO8859_13Codec;
          110: Result := TISO8859_14Codec;
          111: Result := TISO8859_15Codec;
          1013: Result := TUTF16BECodec;
          1014: Result := TUTF16LECodec;
          2009: Result := TIBM850Codec;
          2010: Result := TIBM852Codec;
          2011: Result := TIBM437Codec;
          2013: Result := TIBM862Codec;
          2028: Result := TIBM037Codec;
          2043: Result := TIBM424Codec;
          2044: Result := TIBM500Codec;
          2046: Result := TIBM855Codec;
          2047: Result := TIBM857Codec;
          2048: Result := TIBM860Codec;
          2049: Result := TIBM861Codec;
          2050: Result := TIBM863Codec;
          2051: Result := TIBM864Codec;
          2052: Result := TIBM865Codec;
          2054: Result := TIBM869Codec;
          2063: Result := TIBM1026Codec;
          2084: Result := TKOI8_RCodec;
          2086: Result := TIBM866Codec;
          2250: Result := TWindows1250Codec;
          2251: Result := TWindows1251Codec;
          2252: Result := TWindows1252Codec;
          2253: Result := TWindows1253Codec;
          2254: Result := TWindows1254Codec;
          2255: Result := TWindows1255Codec;
          2256: Result := TWindows1256Codec;
          2257: Result := TWindows1257Codec;
          2258: Result := TWindows1258Codec;
        else
          Result := nil
        end;
      end
      else
        Result := nil
    finally
      Csmib.Free;
    end;
  end;
end;

function Utf16HighSurrogate(const Value: Integer): WideChar;
var
  Value2: Word;
begin
  Value2 := $D7C0 + (Value shr 10);
  Result := WideChar(Value2);
end;

function Utf16LowSurrogate(const Value: Integer): WideChar;
var
  Value2: Word;
begin
  Value2 := $DC00 xor (Value and $3FF);
  Result := WideChar(Value2);
end;

function Utf16SurrogateToInt(const HighSurrogate, LowSurrogate: WideChar):
  Integer;
begin
  Result := ((Word(HighSurrogate) - $D7C0) shl 10)
    + (Word(LowSurrogate) xor $DC00);
end;

function IsUtf16HighSurrogate(const S: WideChar): Boolean;
begin
  case Word(S) of
    $D800..$DBFF: Result := True;
  else
    Result := False;
  end;
end;

function IsUtf16LowSurrogate(const S: WideChar): Boolean;
begin
  case Word(S) of
    $DC00..$DFFF: Result := True;
  else
    Result := False;
  end;
end;

function UTF8ToUTF16BEStr(const S: string): WideString;
// Converts an UTF-8 string into an UTF-16 WideString.
// No special conversions (e.g. on line breaks) and
// no XML-char checking are done.
// - This function was provided by Ernst van der Pols -
// - and slightly modified by Dieter Köhler -
const
  MaxCode: array[1..6] of Integer = ($7F, $7FF, $FFFF, $1FFFFF, $3FFFFFF,
    $7FFFFFFF);
var
  i, j, CharSize, mask, ucs4: Integer;
  c, first: Char;
begin
  setLength(Result, Length(S) + 1); // assume no or little above-ASCII-chars
  j := 0; // keep track of actual length
  i := 0;

  // Skip byte order mark:
  if Length(S) >= 3 then
    if S[1] = #$EF then
      if S[2] = #$BB then
        if S[3] = #$BF then
          i := 3;

  while i < Length(S) do
  begin
    Inc(i);
    c := S[i];
    if ord(c) >= $80 then // UTF-8 sequence
    begin
      if (ord(c) and $C0) <> $C0 then
        raise EConvertError.CreateFmt('Invalid UTF-8 sequence %2.2X', [ord(c)]);

      CharSize := 1;
      first := c;
      mask := $40;
      ucs4 := ord(c);
      while (mask and ord(first)) <> 0 do
      begin
        // read next character of stream
        if i = length(S) then
          raise EConvertError.CreateFmt('Aborted UTF-8 sequence "%s"', [Copy(S, i
            - CharSize, CharSize)]);
        Inc(i);
        c := S[i];
        if (ord(c) and $C0 <> $80) then
          raise EConvertError.CreateFmt('Invalid UTF-8 sequence $%2.2X',
            [ord(c)]);
        ucs4 := (ucs4 shl 6) or (ord(c) and $3F); // add bits to Result
        Inc(CharSize); // increase sequence length
        mask := mask shr 1; // adjust mask
      end;
      if (CharSize > 6) then // no 0 bit in sequence header 'first'
        raise EConvertError.CreateFmt('Invalid UTF-8 sequence "%s"', [Copy(S, i
          - CharSize, CharSize)]);
      ucs4 := ucs4 and MaxCode[CharSize]; // dispose of header bits
      // check for invalid sequence as suggested by RFC2279
      if ((CharSize > 1) and (ucs4 <= MaxCode[CharSize - 1])) then
        raise EConvertError.CreateFmt('Invalid UTF-8 encoding "%s"', [Copy(S, i
          - CharSize, CharSize)]);
      // convert non-ASCII UCS-4 to UTF-16 if possible
      case ucs4 of
        $00000080..$0000D7FF, $0000E000..$0000FFFD:
          begin
            Inc(j);
            Result[j] := WideChar(ucs4);
          end;
        $0000D800..$0000DFFF, $0000FFFE, $0000FFFF:
          raise EConvertError.CreateFmt('Invalid UCS-4 character $%8.8X',
            [ucs4]);
        $00010000..$0010FFFF:
          begin
            // add high surrogate to content as if it was processed earlier
            Inc(j);
            Result[j] := Utf16HighSurrogate(ucs4); // assign high surrogate
            Inc(j);
            Result[j] := Utf16LowSurrogate(ucs4); // assign low surrogate
          end;
      else // out of UTF-16 range
        raise EConvertError.CreateFmt('Cannot convert $%8.8X to UTF-16',
          [ucs4]);
      end;
    end
    else // ASCII char
    begin
      Inc(j);
      Result[j] := WideChar(ord(c));
    end;
  end;
  setLength(Result, j); // set to correct length
end;

{ TCSMIB }

constructor TCSMIB.Create(AOwner: TComponent);
begin
  inherited;
  Enum := 3;
end;

procedure TCSMIB.DoChange(Sender: TObject);
begin
  if assigned(FOnChange) then
    FOnChange(Sender);
end;

procedure TCSMIB.DoChanging(Sender: TObject; NewEnum: Integer;
  var AllowChange: Boolean);
begin
  if assigned(FOnChanging) then
    FOnChanging(Sender, NewEnum, AllowChange);
end;

function TCSMIB.GetAlias(I: Integer): string;
begin
  case FEnum of
    3: case i of
        0: Result := 'ANSI_X3.4-1968';
        1: Result := 'iso-ir-6';
        2: Result := 'ANSI_X3.4-1986';
        3: Result := 'ISO_646.irv:1991';
        4: Result := 'ASCII';
        5: Result := 'ISO646-US';
        6: Result := 'US-ASCII';
        7: Result := 'us';
        8: Result := 'IBM367';
        9: Result := 'cp367';
        10: Result := 'csASCII';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    4: case i of
        0: Result := 'ISO_8859-1:1987';
        1: Result := 'iso-ir-100';
        2: Result := 'ISO_8859-1';
        3: Result := 'ISO-8859-1';
        4: Result := 'latin1';
        5: Result := 'l1';
        6: Result := 'IBM819';
        7: Result := 'CP819';
        8: Result := 'csISOLatin1';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    5: case i of
        0: Result := 'ISO_8859-2:1987';
        1: Result := 'iso-ir-101';
        2: Result := 'ISO_8859-2';
        3: Result := 'ISO-8859-2';
        4: Result := 'latin2';
        5: Result := 'l2';
        6: Result := 'csISOLatin2';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    6: case i of
        0: Result := 'ISO_8859-3:1988';
        1: Result := 'iso-ir-109';
        2: Result := 'ISO_8859-3';
        3: Result := 'ISO-8859-3';
        4: Result := 'latin3';
        5: Result := 'l3';
        6: Result := 'csISOLatin3';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    7: case i of
        0: Result := 'ISO_8859-4:1988';
        1: Result := 'iso-ir-110';
        2: Result := 'ISO_8859-4';
        3: Result := 'ISO-8859-4';
        4: Result := 'latin4';
        5: Result := 'l4';
        6: Result := 'csISOLatin4';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    8: case i of
        0: Result := 'ISO_8859-5:1988';
        1: Result := 'iso-ir-144';
        2: Result := 'ISO_8859-5';
        3: Result := 'ISO-8859-5';
        4: Result := 'cyrillic';
        5: Result := 'csISOLatinCyrillic';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    9: case i of
        0: Result := 'ISO_8859-6:1987';
        1: Result := 'iso-ir-127';
        2: Result := 'ISO_8859-6';
        3: Result := 'ISO-8859-6';
        4: Result := 'ECMA-114';
        5: Result := 'ASMO-708';
        6: Result := 'arabic';
        7: Result := 'csISOLatinArabic';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    10: case i of
        0: Result := 'ISO_8859-7:1987';
        1: Result := 'iso-ir-126';
        2: Result := 'ISO_8859-7';
        3: Result := 'ISO-8859-7';
        4: Result := 'ELOT_928';
        5: Result := 'ECMA-118';
        6: Result := 'greek';
        7: Result := 'greek8';
        8: Result := 'csISOLatinGreek';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    11: case i of
        0: Result := 'ISO_8859-8:1988';
        1: Result := 'iso-ir-138';
        2: Result := 'ISO_8859-8';
        3: Result := 'ISO-8859-8';
        4: Result := 'hebrew';
        5: Result := 'csISOLatinHebrew';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    12: case i of
        0: Result := 'ISO_8859-9:1989';
        1: Result := 'iso-ir-148';
        2: Result := 'ISO_8859-9';
        3: Result := 'ISO-8859-9';
        4: Result := 'latin5';
        5: Result := 'l5';
        6: Result := 'csISOLatin5';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    13: case i of
        0: Result := 'ISO_8859-10';
        1: Result := 'iso-ir-157';
        2: Result := 'l6';
        3: Result := 'ISO-8859-10:1992';
        4: Result := 'csISOLatin6';
        5: Result := 'latin6';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    14: case i of
        0: Result := 'ISO_6937-2-add';
        1: Result := 'iso-ir-142';
        2: Result := 'csISOTextComm';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    15: case i of
        0: Result := 'JIS_X0201';
        1: Result := 'X0201';
        2: Result := 'csHalfWidthKatakana';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    16: case i of
        0: Result := 'JIS_Encoding';
        1: Result := 'csJISEncoding';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    17: case i of
        0: Result := 'Shift_JIS';
        1: Result := 'MS_Kanji';
        2: Result := 'csShiftJIS';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    18: case i of
        0: Result := 'Extended_UNIX_Code_Packed_Format_for_Japanese';
        1: Result := 'csEUCPPkdFmtJapanese';
        2: Result := 'EUC-JP';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    19: case i of
        0: Result := 'Extended_UNIX_Code_Fixed_Width_for_Japanese';
        1: Result := 'csEUCFixWidJapanese';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    20: case i of
        0: Result := 'BS_4730';
        1: Result := 'iso-ir-4';
        2: Result := 'ISO646-GB';
        3: Result := 'gb';
        4: Result := 'uk';
        5: Result := 'csISO4UnitedKingdom';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    21: case i of
        0: Result := 'SEN_850200_C';
        1: Result := 'iso-ir-11';
        2: Result := 'ISO646-SE2';
        3: Result := 'se2';
        4: Result := 'csISO11SwedishForNames';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    22: case i of
        0: Result := 'IT';
        1: Result := 'iso-ir-15';
        2: Result := 'ISO646-IT';
        3: Result := 'csISO15Italian';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    23: case i of
        0: Result := 'ES';
        1: Result := 'iso-ir-17';
        2: Result := 'ISO646-ES';
        3: Result := 'csISO17Spanish';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    24: case i of
        0: Result := 'DIN_66003';
        1: Result := 'iso-ir-21';
        2: Result := 'de';
        3: Result := 'ISO646-DE';
        4: Result := 'csISO21German';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    25: case i of
        0: Result := 'NS_4551-1';
        1: Result := 'iso-ir-60';
        2: Result := 'ISO646-NO';
        3: Result := 'no';
        4: Result := 'csISO60Danish-Norwegian';
        5: Result := 'csISO60Norwegian1';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    26: case i of
        0: Result := 'NF_Z_62-010';
        1: Result := 'iso-ir-69';
        2: Result := 'ISO646-FR';
        3: Result := 'fr';
        4: Result := 'csISO69French';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    27: case i of
        0: Result := 'ISO-10646-UTF-1';
        1: Result := 'csISO10646UTF1';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    28: case i of
        0: Result := 'ISO_646.basic:1983';
        1: Result := 'ref';
        2: Result := 'csISO646basic1983';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    29: case i of
        0: Result := 'INVARIANT';
        1: Result := 'csINVARIANT';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    30: case i of
        0: Result := 'ISO_646.irv:1983';
        1: Result := 'iso-ir-2';
        2: Result := 'irv';
        3: Result := 'csISO2Int1RefVersion';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    31: case i of
        0: Result := 'NATS-SEFI';
        1: Result := 'iso-ir-8-1';
        2: Result := 'csNATSSEFI';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    32: case i of
        0: Result := 'NATS-SEFI-ADD';
        1: Result := 'iso-ir-8-2';
        2: Result := 'csNATSSEFIADD';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    33: case i of
        0: Result := 'NATS-DANO';
        1: Result := 'iso-ir-9-1';
        2: Result := 'csNATSDANO';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    34: case i of
        0: Result := 'NATS-DANO-ADD';
        1: Result := 'iso-ir-9-2';
        2: Result := 'csNATSDANOADD';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    35: case i of
        0: Result := 'SEN_850200_B';
        1: Result := 'iso-ir-10';
        2: Result := 'FI';
        3: Result := 'ISO646-FI';
        4: Result := 'ISO646-SE';
        5: Result := 'se';
        6: Result := 'csISO10Swedish';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    36: case i of
        0: Result := 'KS_C_5601-1987';
        1: Result := 'iso-ir-149';
        2: Result := 'KS_C_5601-1989';
        3: Result := 'KSC_5601';
        4: Result := 'korean';
        5: Result := 'csKSC56011987';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    37: case i of
        0: Result := 'ISO-2022-KR';
        1: Result := 'csISO2022KR';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    38: case i of
        0: Result := 'EUC-KR';
        1: Result := 'csEUCKR';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    39: case i of
        0: Result := 'ISO-2022-JP';
        1: Result := 'csISO2022JP';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    40: case i of
        0: Result := 'ISO-2022-JP-2';
        1: Result := 'csISO2022JP2';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    41: case i of
        0: Result := 'JIS_C6220-1969-jp';
        1: Result := 'JIS_C6220-1969';
        2: Result := 'iso-ir-13';
        3: Result := 'katakana';
        4: Result := 'x0201-7';
        5: Result := 'csISO13JISC6220jp';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    42: case i of
        0: Result := 'JIS_C6220-1969-ro';
        1: Result := 'iso-ir-14';
        2: Result := 'jp';
        3: Result := 'ISO646-JP';
        4: Result := 'csISO14JISC6220ro';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    43: case i of
        0: Result := 'PT';
        1: Result := 'iso-ir-16';
        2: Result := 'ISO646-PT';
        3: Result := 'csISO16Portuguese';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    44: case i of
        0: Result := 'greek7-old';
        1: Result := 'iso-ir-18';
        2: Result := 'csISO18Greek7Old';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    45: case i of
        0: Result := 'latin-greek';
        1: Result := 'iso-ir-19';
        2: Result := 'csISO19LatinGreek';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    46: case i of
        0: Result := 'NF_Z_62-010_(1973)';
        1: Result := 'iso-ir-25';
        2: Result := 'ISO646-FR1';
        3: Result := 'csISO25French';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    47: case i of
        0: Result := 'Latin-greek-1';
        1: Result := 'iso-ir-27';
        2: Result := 'csISO27LatinGreek1';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    48: case i of
        0: Result := 'ISO_5427';
        1: Result := 'iso-ir-37';
        2: Result := 'csISO5427Cyrillic';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    49: case i of
        0: Result := 'JIS_C6226-1978';
        1: Result := 'iso-ir-42';
        2: Result := 'csISO42JISC62261978';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    50: case i of
        0: Result := 'BS_viewdata';
        1: Result := 'iso-ir-47';
        2: Result := 'csISO47BSViewdata';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    51: case i of
        0: Result := 'INIS';
        1: Result := 'iso-ir-49';
        2: Result := 'csISO49INIS';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    52: case i of
        0: Result := 'INIS-8';
        1: Result := 'iso-ir-50';
        2: Result := 'csISO50INIS8';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    53: case i of
        0: Result := 'INIS-cyrillic';
        1: Result := 'iso-ir-51';
        2: Result := 'csISO51INISCyrillic';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    54: case i of
        0: Result := 'ISO_5427:1981';
        1: Result := 'iso-ir-54';
        2: Result := 'ISO5427Cyrillic1981';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    55: case i of
        0: Result := 'ISO_5428:1980';
        1: Result := 'iso-ir-55';
        2: Result := 'csISO5428Greek';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    56: case i of
        0: Result := 'GB_1988-80';
        1: Result := 'iso-ir-57';
        2: Result := 'cn';
        3: Result := 'ISO646-CN';
        4: Result := 'csISO57GB1988';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    57: case i of
        0: Result := 'GB_2312-80';
        1: Result := 'iso-ir-58';
        2: Result := 'chinese';
        3: Result := 'csISO58GB231280';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    58: case i of
        0: Result := 'NS_4551-2';
        1: Result := 'ISO646-NO2';
        2: Result := 'iso-ir-61';
        3: Result := 'no2';
        4: Result := 'csISO61Norwegian2';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    59: case i of
        0: Result := 'videotex-suppl';
        1: Result := 'iso-ir-70';
        2: Result := 'csISO70VideotexSupp1';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    60: case i of
        0: Result := 'PT2';
        1: Result := 'iso-ir-84';
        2: Result := 'ISO646-PT2';
        3: Result := 'csISO84Portuguese2';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    61: case i of
        0: Result := 'ES2';
        1: Result := 'iso-ir-85';
        2: Result := 'ISO646-ES2';
        3: Result := 'csISO85Spanish2';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    62: case i of
        0: Result := 'MSZ_7795.3';
        1: Result := 'iso-ir-86';
        2: Result := 'ISO646-HU';
        3: Result := 'hu';
        4: Result := 'csISO86Hungarian';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    63: case i of
        0: Result := 'JIS_C6226-1983';
        1: Result := 'iso-ir-87';
        2: Result := 'x0208';
        3: Result := 'JIS_X0208-1983';
        4: Result := 'csISO87JISX0208';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    64: case i of
        0: Result := 'greek7';
        1: Result := 'iso-ir-88';
        2: Result := 'csISO88Greek7';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    65: case i of
        0: Result := 'ASMO_449';
        1: Result := 'ISO_9036';
        2: Result := 'arabic7';
        3: Result := 'iso-ir-89';
        4: Result := 'csISO89ASMO449';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    66: case i of
        0: Result := 'iso-ir-90';
        1: Result := 'csISO90';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    67: case i of
        0: Result := 'JIS_C6229-1984-a';
        1: Result := 'iso-ir-91';
        2: Result := 'jp-ocr-a';
        3: Result := 'csISO91JISC62291984a';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    68: case i of
        0: Result := 'JIS_C6229-1984-b';
        1: Result := 'iso-ir-92';
        2: Result := 'ISO646-JP-OCR-B';
        3: Result := 'jp-ocr-b';
        4: Result := 'csISO92JISC62291984b';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    69: case i of
        0: Result := 'JIS_C6229-1984-b-add';
        1: Result := 'iso-ir-93';
        2: Result := 'jp-ocr-b-add';
        3: Result := 'csISO93JISC62291984badd';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    70: case i of
        0: Result := 'JIS_C6229-1984-hand';
        1: Result := 'iso-ir-94';
        2: Result := 'jp-ocr-hand';
        3: Result := 'csISO94JISC62291984hand';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    71: case i of
        0: Result := 'JIS_C6229-1984-hand-add';
        1: Result := 'iso-ir-95';
        2: Result := 'jp-ocr-hand-add';
        3: Result := 'csISO95JISC62291984handadd';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    72: case i of
        0: Result := 'JIS_C6229-1984-kana';
        1: Result := 'iso-ir-96';
        2: Result := 'jp-ocr-hand';
        3: Result := 'csISO96JISC62291984kana';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    73: case i of
        0: Result := 'ISO_2033-1983';
        1: Result := 'iso-ir-98';
        2: Result := 'e13b';
        3: Result := 'csISO2033';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    74: case i of
        0: Result := 'ANSI_X3.110-1983';
        1: Result := 'iso-ir-99';
        2: Result := 'CSA_T500-1983';
        3: Result := 'NAPLPS';
        4: Result := 'csISO99NAPLPS';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    75: case i of
        0: Result := 'T.61-7bit';
        1: Result := 'iso-ir-102';
        2: Result := 'csISO102T617bit';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    76: case i of
        0: Result := 'T.61-8bit';
        1: Result := 'T.61';
        2: Result := 'iso-ir-103';
        3: Result := 'csISO103T618bit';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    77: case i of
        0: Result := 'ECMA-cyrillic';
        1: Result := 'iso-ir-111';
        2: Result := 'csISO111ECMACyrillic';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    78: case i of
        0: Result := 'CSA_Z243.4-1985-1';
        1: Result := 'iso-ir-121';
        2: Result := 'ISO646-CA';
        3: Result := 'csa7-1';
        4: Result := 'ca';
        5: Result := 'csISO121Canadian1';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    79: case i of
        0: Result := 'CSA_Z243.4-1985-2';
        1: Result := 'iso-ir-122';
        2: Result := 'ISO646-CA2';
        3: Result := 'csa7-2';
        4: Result := 'csISO122Canadian2';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    80: case i of
        0: Result := 'CSA_Z243.4-1985-gr';
        1: Result := 'iso-ir-123';
        2: Result := 'csISO123CSAZ24341985gr';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    81: case i of
        0: Result := 'ISO_8859-6-E';
        1: Result := 'csISO88596E';
        2: Result := 'ISO-8859-6-E';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    82: case i of
        0: Result := 'ISO_8859-6-I';
        1: Result := 'csISO88596I';
        2: Result := 'ISO-8859-6-I';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    83: case i of
        0: Result := 'T.101-G2';
        1: Result := 'iso-ir-128';
        2: Result := 'csISO128T101G2';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    84: case i of
        0: Result := 'ISO_8859-8-E';
        1: Result := 'csISO88598E';
        2: Result := 'ISO-8859-8-E';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    85: case i of
        0: Result := 'ISO_8859-8-I';
        1: Result := 'csISO88598I';
        2: Result := 'ISO-8859-8-I';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    86: case i of
        0: Result := 'CSN_369103';
        1: Result := 'iso-ir-139';
        2: Result := 'csISO139CSN369103';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    87: case i of
        0: Result := 'JUS_I.B1.002';
        1: Result := 'iso-ir-141';
        2: Result := 'ISO646-YU';
        3: Result := 'js';
        4: Result := 'yu';
        5: Result := 'csISO141JUSIB1002';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    88: case i of
        0: Result := 'IEC_P27-1';
        1: Result := 'iso-ir-143';
        2: Result := 'csISO143IECP271';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    89: case i of
        0: Result := 'JUS_I.B1.003-serb';
        1: Result := 'iso-ir-146';
        2: Result := 'serbian';
        3: Result := 'csISO146Serbian';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    90: case i of
        0: Result := 'JUS_I.B1.003-mac';
        1: Result := 'macedonian';
        2: Result := 'iso-ir-147';
        3: Result := 'csISO147Macedonian';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    91: case i of
        0: Result := 'greek-ccitt';
        1: Result := 'iso-ir-150';
        2: Result := 'csISO150';
        3: Result := 'csISO150GreekCCITT';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    92: case i of
        0: Result := 'NC_NC00-10:81';
        1: Result := 'cuba';
        2: Result := 'iso-ir-151';
        3: Result := 'ISO646-CU';
        4: Result := 'csISO151Cuba';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    93: case i of
        0: Result := 'ISO_6937-2-25';
        1: Result := 'iso-ir-152';
        2: Result := 'csISO6937Add';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    94: case i of
        0: Result := 'GOST_19768-74';
        1: Result := 'ST_SEV_358-88';
        2: Result := 'iso-ir-153';
        3: Result := 'csISO153GOST1976874';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    95: case i of
        0: Result := 'ISO_8859-supp';
        1: Result := 'iso-ir-154';
        2: Result := 'latin1-2-5';
        3: Result := 'csISO8859Supp';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    96: case i of
        0: Result := 'ISO_10367-box';
        1: Result := 'iso-ir-155';
        2: Result := 'csISO10367Box';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    97: case i of
        0: Result := 'latin-lap';
        1: Result := 'lap';
        2: Result := 'iso-ir-158';
        3: Result := 'csISO158Lap';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    98: case i of
        0: Result := 'JIS_X0212-1990';
        1: Result := 'x0212';
        2: Result := 'iso-ir-159';
        3: Result := 'csISO159JISX02121990';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    99: case i of
        0: Result := 'DS_2089';
        1: Result := 'DS2089';
        2: Result := 'ISO646-DK';
        3: Result := 'dk';
        4: Result := 'csISO646Danish';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    100: case i of
        0: Result := 'us-dk';
        1: Result := 'csUSDK';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    101: case i of
        0: Result := 'dk-us';
        1: Result := 'csDKUS';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    102: case i of
        0: Result := 'KSC5636';
        1: Result := 'ISO646-KR';
        2: Result := 'csKSC5636';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    103: case i of
        0: Result := 'UNICODE-1-1-UTF-7';
        1: Result := 'csUnicode11UTF7';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    104: case i of
        0: Result := 'ISO-2022-CN';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    105: case i of
        0: Result := 'ISO-2022-CN-EXT';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    106: case i of
        0: Result := 'UTF-8';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    109: case i of
        0: Result := 'ISO-8859-13';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    110: case i of
        0: Result := 'ISO-8859-14';
        1: Result := 'iso-ir-199';
        2: Result := 'ISO_8859-14:1998';
        3: Result := 'ISO_8859-14';
        4: Result := 'latin8';
        5: Result := 'iso-celtic';
        6: Result := 'l8';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    111: case i of
        0: Result := 'ISO-8859-15';
        1: Result := 'ISO_8869-15';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    112: case i of
        0: Result := 'ISO-8859-16';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1000: case i of
        0: Result := 'ISO-10646-UCS-2';
        1: Result := 'csUnicode';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1001: case i of
        0: Result := 'ISO-10646-UCS-4';
        1: Result := 'csUCS4';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1002: case i of
        0: Result := 'ISO-10646-UCS-Basic';
        1: Result := 'csUnicodeASCII';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1003: case i of
        0: Result := 'ISO-10646-Unicode-Latin1';
        1: Result := 'csUnicodeLatin1';
        2: Result := 'ISO-10646';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1004: case i of
        0: Result := 'ISO-10646-J-1';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1005: case i of
        0: Result := 'ISO-Unicode-IBM-1261';
        1: Result := 'csUnicodeIBM1261';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1006: case i of
        0: Result := 'ISO-Unicode-IBM-1268';
        1: Result := 'csUnicodeIBM1268';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1007: case i of
        0: Result := 'ISO-Unicode-IBM-1276';
        1: Result := 'csUnicodeIBM1276';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1008: case i of
        0: Result := 'ISO-Unicode-IBM-1264';
        1: Result := 'csUnicodeIBM1264';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1009: case i of
        0: Result := 'ISO-Unicode-IBM-1265';
        1: Result := 'csUnicodeIBM1265';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1010: case i of
        0: Result := 'UNICODE-1-1';
        1: Result := 'csUnicode11';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1011: case i of
        0: Result := 'SCSU';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1012: case i of
        0: Result := 'UTF-7';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1013: case i of
        0: Result := 'UTF-16BE';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1014: case i of
        0: Result := 'UTF-16LE';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    1015: case i of
        0: Result := 'UTF-16';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2000: case i of
        0: Result := 'ISO-8859-1-Windows-3.0-Latin-1';
        1: Result := 'csWindows30Latin1';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2001: case i of
        0: Result := 'ISO-8859-1-Windows-3.1-Latin-1';
        1: Result := 'csWindows31Latin1';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2002: case i of
        0: Result := 'ISO-8859-2-Windows-Latin-2';
        1: Result := 'csWindows31Latin2';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2003: case i of
        0: Result := 'ISO-8859-9-Windows-Latin-5';
        1: Result := 'csWindows31Latin5';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2004: case i of
        0: Result := 'hp-roman8';
        1: Result := 'roman8';
        2: Result := 'r8';
        3: Result := 'csHPRoman8';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2005: case i of
        0: Result := 'Adobe-Standard-Encoding';
        1: Result := 'csAdobeStandardEncoding';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2006: case i of
        0: Result := 'Ventura-US';
        1: Result := 'csVenturaUS';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2007: case i of
        0: Result := 'Ventura-International';
        1: Result := 'csVenturaInternational';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2008: case i of
        0: Result := 'DEC-MCS';
        1: Result := 'dec';
        2: Result := 'csDECMCS';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2009: case i of
        0: Result := 'IBM850';
        1: Result := 'cp850';
        2: Result := '850';
        3: Result := 'csPC850Multilingual';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2010: case i of
        0: Result := 'IBM852';
        1: Result := 'cp852';
        2: Result := '852';
        3: Result := 'csPCp852';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2011: case i of
        0: Result := 'IBM437';
        1: Result := 'cp437';
        2: Result := '437';
        3: Result := 'csPC8CodePage437';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2012: case i of
        0: Result := 'PC8-Danish-Norwegian';
        1: Result := 'csPC8DanishNorwegian';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2013: case i of
        0: Result := 'IBM862';
        1: Result := 'cp862';
        2: Result := '862';
        3: Result := 'csPC862LatinHebrew';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2014: case i of
        0: Result := 'PC8-Turkish';
        1: Result := 'csPC8Turkish';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2015: case i of
        0: Result := 'IBM-Symbols';
        1: Result := 'csIBMSymbols';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2016: case i of
        0: Result := 'IBM-Thai';
        1: Result := 'csIBMThai';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2017: case i of
        0: Result := 'HP-Legal';
        1: Result := 'csHPLegal';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2018: case i of
        0: Result := 'HP-Pi-font';
        1: Result := 'csHPPiFont';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2019: case i of
        0: Result := 'HP-Math8';
        1: Result := 'csHPMath8';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2020: case i of
        0: Result := 'Adobe-Symbol-Encoding';
        1: Result := 'csHPPSMath';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2021: case i of
        0: Result := 'HP-DeskTop';
        1: Result := 'csHPDesktop';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2022: case i of
        0: Result := 'Ventura-Math';
        1: Result := 'csVenturaMath';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2023: case i of
        0: Result := 'Microsoft-Publishing';
        1: Result := 'csMicrosoftPublishing';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2024: case i of
        0: Result := 'Windows-31J';
        1: Result := 'csWindows31J';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2025: case i of
        0: Result := 'GB2312';
        1: Result := 'csGB2312';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2026: case i of
        0: Result := 'Big5';
        1: Result := 'csBig5';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2027: case i of
        0: Result := 'macintosh';
        1: Result := 'mac';
        2: Result := 'csMacintosh';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2028: case i of
        0: Result := 'IBM037';
        1: Result := 'cp037';
        2: Result := 'ebcdic-cp-us';
        3: Result := 'ebcdic-cp-ca';
        4: Result := 'ebcdic-cp-wt';
        5: Result := 'ebcdic-cp-nl';
        6: Result := 'csIBM037';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2029: case i of
        0: Result := 'IBM038';
        1: Result := 'EBCDIC-INT';
        2: Result := 'cp038';
        3: Result := 'csIBM038';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2030: case i of
        0: Result := 'IBM273';
        1: Result := 'CP273';
        2: Result := 'csIBM273';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2031: case i of
        0: Result := 'IBM274';
        1: Result := 'EBCDIC-BE';
        2: Result := 'CP274';
        3: Result := 'csIBM274';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2032: case i of
        0: Result := 'IBM275';
        1: Result := 'EBCDIC-BR';
        2: Result := 'cp275';
        3: Result := 'csIBM275';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2033: case i of
        0: Result := 'IBM277';
        1: Result := 'EBCDIC-CP-DK';
        2: Result := 'EBCDIC-CP-NO';
        3: Result := 'csIBM277';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2034: case i of
        0: Result := 'IBM278';
        1: Result := 'CP278';
        2: Result := 'ebcdic-cp-fi';
        3: Result := 'ebcdic-cp-se';
        4: Result := 'csIBM278';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2035: case i of
        0: Result := 'IBM280';
        1: Result := 'CP280';
        2: Result := 'ebcdic-cp-it';
        3: Result := 'csIBM280';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2036: case i of
        0: Result := 'IBM281';
        1: Result := 'EBCDIC-JP-E';
        2: Result := 'cp281';
        3: Result := 'csIBM281';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2037: case i of
        0: Result := 'IBM284';
        1: Result := 'CP284';
        2: Result := 'ebcdic-cp-es';
        3: Result := 'csIBM284';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2038: case i of
        0: Result := 'IBM285';
        1: Result := 'CP285';
        2: Result := 'ebcdic-cp-gb';
        3: Result := 'csIBM285';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2039: case i of
        0: Result := 'IBM290';
        1: Result := 'cp290';
        2: Result := 'EBCDIC-JP-kana';
        3: Result := 'csIBM290';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2040: case i of
        0: Result := 'IBM297';
        1: Result := 'cp297';
        2: Result := 'ebcdic-cp-fr';
        3: Result := 'csIBM297';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2041: case i of
        0: Result := 'IBM420';
        1: Result := 'cp420';
        2: Result := 'ebcdic-cp-ar1';
        3: Result := 'csIBM420';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2042: case i of
        0: Result := 'IBM423';
        1: Result := 'cp423';
        2: Result := 'ebcdic-cp-gr';
        3: Result := 'csIBM423';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2043: case i of
        0: Result := 'IBM424';
        1: Result := 'cp424';
        2: Result := 'ebcdic-cp-he';
        3: Result := 'csIBM424';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2044: case i of
        0: Result := 'IBM500';
        1: Result := 'CP500';
        2: Result := 'ebcdic-cp-be';
        3: Result := 'ebcdic-cp-ch';
        4: Result := 'csIBM500';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2045: case i of
        0: Result := 'IBM851';
        1: Result := 'cp851';
        2: Result := '851';
        3: Result := 'csIBM851';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2046: case i of
        0: Result := 'IBM855';
        1: Result := 'cp855';
        2: Result := '855';
        3: Result := 'csIBM855';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2047: case i of
        0: Result := 'IBM857';
        1: Result := 'cp857';
        2: Result := '857';
        3: Result := 'csIBM857';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2048: case i of
        0: Result := 'IBM860';
        1: Result := 'cp860';
        2: Result := '860';
        3: Result := 'csIBM860';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2049: case i of
        0: Result := 'IBM861';
        1: Result := 'cp861';
        2: Result := '861';
        3: Result := 'cp-is';
        4: Result := 'csIBM861';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2050: case i of
        0: Result := 'IBM863';
        1: Result := 'cp863';
        2: Result := '863';
        3: Result := 'csIBM863';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2051: case i of
        0: Result := 'IBM864';
        1: Result := 'cp864';
        2: Result := 'csIBM864';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2052: case i of
        0: Result := 'IBM865';
        1: Result := 'cp865';
        2: Result := '865';
        3: Result := 'csIBM865';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2053: case i of
        0: Result := 'IBM868';
        1: Result := 'CP868';
        2: Result := 'cp-ar';
        3: Result := 'csIBM868';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2054: case i of
        0: Result := 'IBM869';
        1: Result := 'cp869';
        2: Result := '869';
        3: Result := 'cp-gr';
        4: Result := 'csIBM869';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2055: case i of
        0: Result := 'IBM870';
        1: Result := 'CP870';
        2: Result := 'ebcdic-cp-roece';
        3: Result := 'ebcdic-cp-yu';
        4: Result := 'csIBM870';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2056: case i of
        0: Result := 'IBM871';
        1: Result := 'CP871';
        2: Result := 'ebcdic-cp-is';
        3: Result := 'csIBM871';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2057: case i of
        0: Result := 'IBM880';
        1: Result := 'cp880';
        2: Result := 'EBCDIC-Cyrillic';
        3: Result := 'csIBM880';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2058: case i of
        0: Result := 'IBM891';
        1: Result := 'cp891';
        2: Result := 'csIBM891';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2059: case i of
        0: Result := 'IBM903';
        1: Result := 'cp903';
        2: Result := 'csIBM903';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2060: case i of
        0: Result := 'IBM904';
        1: Result := 'cp904';
        2: Result := '904';
        3: Result := 'csIBM904';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2061: case i of
        0: Result := 'IBM905';
        1: Result := 'CP905';
        2: Result := 'ebcdic-cp-tr';
        3: Result := 'csIBM905';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2062: case i of
        0: Result := 'IBM918';
        1: Result := 'CP918';
        2: Result := 'ebcdic-cp-ar2';
        3: Result := 'csIBM918';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2063: case i of
        0: Result := 'IBM1026';
        1: Result := 'CP1026';
        2: Result := 'csIBM1026';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2064: case i of
        0: Result := 'EBCDIC-AT-DE';
        1: Result := 'csIBMEBCDICATDE';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2065: case i of
        0: Result := 'EBCDIC-AT-DE-A';
        1: Result := 'csIBMEBCDICATDEA';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2066: case i of
        0: Result := 'EBCDIC-CA-FR';
        1: Result := 'csIBMEBCDICCAFR';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2067: case i of
        0: Result := 'EBCDIC-DK-NO';
        1: Result := 'csIBMEBCDICDKNO';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2068: case i of
        0: Result := 'EBCDIC-DK-NO-A';
        1: Result := 'csIBMEBCDICDKNOA';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2069: case i of
        0: Result := 'EBCDIC-FI-SE';
        1: Result := 'csIBMEBCDICFISE';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2070: case i of
        0: Result := 'EBCDIC-FI-SE-A';
        1: Result := 'csIBMEBCDICFISEA';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2071: case i of
        0: Result := 'EBCDIC-FR';
        1: Result := 'csIBMEBCDICFR';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2072: case i of
        0: Result := 'EBCDIC-IT';
        1: Result := 'csIBMEBCDICIT';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2073: case i of
        0: Result := 'EBCDIC-PT';
        1: Result := 'csIBMEBCDICPT';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2074: case i of
        0: Result := 'EBCDIC-ES';
        1: Result := 'csIBMEBCDICES';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2075: case i of
        0: Result := 'EBCDIC-ES-A';
        1: Result := 'csIBMEBCDICESA';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2076: case i of
        0: Result := 'EBCDIC-ES-S';
        1: Result := 'csIBMEBCDICESS';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2077: case i of
        0: Result := 'EBCDIC-UK';
        1: Result := 'csIBMEBCDICUK';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2078: case i of
        0: Result := 'EBCDIC-US';
        1: Result := 'csIBMEBCDICUS';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2079: case i of
        0: Result := 'UNKNOWN-8BIT';
        1: Result := 'csUnkown8Bit';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2080: case i of
        0: Result := 'MNEMONIC';
        1: Result := 'csMnemonic';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2081: case i of
        0: Result := 'MNEM';
        1: Result := 'csMnem';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2082: case i of
        0: Result := 'VISCII';
        1: Result := 'csVISCII';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2083: case i of
        0: Result := 'VIQR';
        1: Result := 'csVIQR';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2084: case i of
        0: Result := 'KOI8-R';
        1: Result := 'csKOI8R';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2085: case i of
        0: Result := 'HZ-GB-2312';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2086: case i of
        0: Result := 'IBM866';
        1: Result := 'cp866';
        2: Result := '866';
        3: Result := 'csIBM866';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2087: case i of
        0: Result := 'IBM775';
        1: Result := 'cp775';
        2: Result := 'csPC775Baltic';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2088: case i of
        0: Result := 'KOI8-U';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2089: case i of
        0: Result := 'IBM00858';
        1: Result := 'CCSID00858';
        2: Result := 'CP00858';
        3: Result := 'PC-Multilingual-850+euro';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2090: case i of
        0: Result := 'IBM00924';
        1: Result := 'CCSID00924';
        2: Result := 'CP00924';
        3: Result := 'ebcdic-Latin9--euro';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2091: case i of
        0: Result := 'IBM01140';
        1: Result := 'CCSID01140';
        2: Result := 'CP01140';
        3: Result := 'ebcdic-us-37+euro';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2092: case i of
        0: Result := 'IBM01141';
        1: Result := 'CCSID01141';
        2: Result := 'CP01141';
        3: Result := 'ebcdic-de-273+euro';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2093: case i of
        0: Result := 'IBM01142';
        1: Result := 'CCSID01142';
        2: Result := 'CP01142';
        3: Result := 'ebcdic-dk-277+euro';
        4: Result := 'ebcdic-no-277+euro';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2094: case i of
        0: Result := 'IBM01143';
        1: Result := 'CCSID01143';
        2: Result := 'CP01143';
        3: Result := 'ebcdic-fi-278+euro';
        4: Result := 'ebcdic-se-278+euro';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2095: case i of
        0: Result := 'IBM01144';
        1: Result := 'CCSID01144';
        2: Result := 'CP01144';
        3: Result := 'ebcdic-it-280+euro';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2096: case i of
        0: Result := 'IBM01145';
        1: Result := 'CCSID01145';
        2: Result := 'CP01145';
        3: Result := 'ebcdic-es-284+euro';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2097: case i of
        0: Result := 'IBM01146';
        1: Result := 'CCSID01146';
        2: Result := 'CP01146';
        3: Result := 'ebcdic-gb-285+euro';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2098: case i of
        0: Result := 'IBM01147';
        1: Result := 'CCSID01147';
        2: Result := 'CP01147';
        3: Result := 'ebcdic-fr-297+euro';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2099: case i of
        0: Result := 'IBM01148';
        1: Result := 'CCSID01148';
        2: Result := 'CP01148';
        3: Result := 'ebcdic-international-500+euro';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2100: case i of
        0: Result := 'IBM01149';
        1: Result := 'CCSID01149';
        2: Result := 'CP01149';
        3: Result := 'ebcdic-is-871+euro';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2101: case i of
        0: Result := 'Big5-HKSCS';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2250: case i of
        0: Result := 'windows-1250';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2251: case i of
        0: Result := 'windows-1251';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2252: case i of
        0: Result := 'windows-1252';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2253: case i of
        0: Result := 'windows-1253';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2254: case i of
        0: Result := 'windows-1254';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2255: case i of
        0: Result := 'windows-1255';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2256: case i of
        0: Result := 'windows-1256';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2257: case i of
        0: Result := 'windows-1257';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2258: case i of
        0: Result := 'windows-1258';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
    2259: case i of
        0: Result := 'TIS-620';
      else
        raise ECSMIBException.Create('Invalid MIB number');
      end;
  else
    raise ECSMIBException.Create('Invalid MIB number');
  end;
end;

function TCSMIB.GetAliasCount: Integer;
begin
  case FEnum of
    104..106, 109, 112, 1004, 1011..1015, 2085, 2088, 2101, 2250..2259: Result
      := 1;
    16, 19, 27, 29, 37..40, 66, 100..101, 103, 111, 1000..1002, 1005..1010,
      2000..2003, 2005..2007, 2012, 2014..2026, 2064..2084: Result := 2;
    14..15, 17..18, 28, 31..34, 44..45, 47..55, 59, 64, 72, 75, 77, 80..86, 88,
      93, 96, 102, 1003, 2008, 2027, 2030, 2051, 2058..2059, 2063, 2087: Result :=
      3;
    22..23, 30, 43, 46, 57, 60..61, 67, 69..71, 73, 76, 89..91, 94..95, 97..98,
      2004, 2009..2011, 2013, 2029, 2031..2033, 2035..2043, 2045..2048, 2050,
      2052..2053, 2056..2057, 2060..2062, 2086, 2089..2092, 2095..2100: Result :=
      4;
    21, 24, 26, 42, 56, 58, 62..63, 65, 68, 74, 79, 92, 99, 2034, 2044, 2049,
      2054..2055, 2093..2094: Result := 5;
    8, 11, 13, 20, 25, 36, 41, 78, 87: Result := 6;
    5..7, 12, 35, 110, 2028: Result := 7;
    9: Result := 8;
    4, 10: Result := 9;
    3: Result := 11;
  else
    raise ECSMIBException.Create('Invalid MIB number');
  end;
end;

function TCSMIB.GetPrfMIMEName: string;
begin
  case FEnum of
    13, 17, 37..40, 2025..2026, 2084: Result := Alias[0];
    18, 82, 84..85: Result := Alias[2];
    4..12: Result := Alias[3];
    3: Result := Alias[6];
  else
    Result := '';
  end;
end;

function TCSMIB.IsValidEnum(const Value: Integer): Boolean;
begin
  case Value of
    3..106, 109..112, 1000..1015, 2000..2101, 2250..2259:
      Result := true;
  else
    Result := false;
  end;
end;

procedure TCSMIB.SetEnum(const Value: Integer);
var
  AllowChange: Boolean;
begin
  if IsValidEnum(Value) then
  begin
    if FEnum = Value then
      exit;
    AllowChange := True;
    DoChanging(self, Value, AllowChange);
    if AllowChange then
    begin
      FEnum := Value;
      DoChange(self);
    end;
  end
  else if not IgnoreInvalidEnum then
  begin
    raise ECSMIBException.Create('Invalid MIB number');
  end;
end;

procedure TCSMIB.SetOnChange(const Value: TNotifyEvent);
begin
  FOnChange := Value;
end;

procedure TCSMIB.SetOnChanging(const Value: TCSMIBChangingEvent);
begin
  FOnChanging := Value;
end;

function TCSMIB.SetToAlias(const S: string): Boolean;
var
  i, j, oldEnum: Integer;
begin
  Result := true;
  oldEnum := Enum;
  for i := 3 to 106 do
  begin
    Enum := i;
    for j := 0 to pred(AliasCount) do
    begin
      if CompareText(Alias[j], S) = 0 then
        exit;
    end;
  end;
  for i := 109 to 112 do
  begin
    Enum := i;
    for j := 0 to pred(AliasCount) do
    begin
      if CompareText(Alias[j], S) = 0 then
        exit;
    end;
  end;
  for i := 1000 to 1015 do
  begin
    Enum := i;
    for j := 0 to pred(AliasCount) do
    begin
      if CompareText(Alias[j], S) = 0 then
        exit;
    end;
  end;
  for i := 2000 to 2101 do
  begin
    Enum := i;
    for j := 0 to pred(AliasCount) do
    begin
      if CompareText(Alias[j], S) = 0 then
        exit;
    end;
  end;
  for i := 2250 to 2259 do
  begin
    Enum := i;
    for j := 0 to pred(AliasCount) do
    begin
      if CompareText(Alias[j], S) = 0 then
        exit;
    end;
  end;
  Result := false;
  Enum := oldEnum;
end;

end.

