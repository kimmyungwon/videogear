// Delphi 5/6/7/2005/2006/2007 for Win32 and Kylix Implementation
// November 2007
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
// The Original Code is "XdomErrorReporting_4_1.pas".
//
// The Initial Developer of the Original Code is Dieter K�hler (Heidelberg,
// Germany, "http://www.philo.de/"). Portions created by the Initial Developer
// are Copyright (C) 1999-2007 Dieter K�hler. All Rights Reserved.
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

unit XdomErrorReporting_4_1;

interface

uses
  XdomCore, LangUtils, cUnicodeCodecsWin32, AutoListUtils, SysUtils, Classes;

type
  TDomCustomErrorTranslator = class(TComponent)
  private
    FLanguage: TIso639LanguageCode;
    FTabWidth: Integer;
  protected
    function GetSupportedLanguages: TIso639LanguageCodeSet; virtual; abstract;
    procedure SetLanguage(const Value: TIso639LanguageCode); virtual;
    procedure SetTabWidth(const Value: Integer); virtual;
  public
    function Translate(const Error: TdomError): WideString; virtual; abstract;
    property Language: TIso639LanguageCode read FLanguage write SetLanguage;
    property SupportedLanguages: TIso639LanguageCodeSet read
      GetSupportedLanguages;
    property TabWidth: Integer read FTabWidth write SetTabWidth default 4;
  end;

  TDomDutchErrorTranslator = class(TDomCustomErrorTranslator)
  protected
    function GetSupportedLanguages: TIso639LanguageCodeSet; override;
  public
    constructor Create(AOwner: TComponent); override;
    function Translate(const Error: TdomError): WideString; override;
  end;

  TDomEnglishErrorTranslator = class(TDomCustomErrorTranslator)
  protected
    function GetSupportedLanguages: TIso639LanguageCodeSet; override;
  public
    constructor Create(AOwner: TComponent); override;
    function Translate(const Error: TdomError): WideString; override;
  end;

  TDomFrenchErrorTranslator = class(TDomCustomErrorTranslator)
  protected
    function GetSupportedLanguages: TIso639LanguageCodeSet; override;
  public
    constructor Create(AOwner: TComponent); override;
    function Translate(const Error: TdomError): WideString; override;
  end;

  TDomGermanErrorTranslator = class(TDomCustomErrorTranslator)
  protected
    function GetSupportedLanguages: TIso639LanguageCodeSet; override;
  public
    constructor Create(AOwner: TComponent); override;
    function Translate(const Error: TdomError): WideString; override;
  end;

  TDomItalianErrorTranslator = class(TDomCustomErrorTranslator)
  protected
    function GetSupportedLanguages: TIso639LanguageCodeSet; override;
  public
    constructor Create(AOwner: TComponent); override;
    function Translate(const Error: TdomError): WideString; override;
  end;

  TDomPolishErrorTranslator = class(TDomCustomErrorTranslator)
  protected
    function GetSupportedLanguages: TIso639LanguageCodeSet; override;
  public
    constructor Create(AOwner: TComponent); override;
    function Translate(const Error: TdomError): WideString; override;
  end;

  TDomPortugueseErrorTranslator = class(TDomCustomErrorTranslator)
  protected
    function GetSupportedLanguages: TIso639LanguageCodeSet; override;
  public
    constructor Create(AOwner: TComponent); override;
    function Translate(const Error: TdomError): WideString; override;
  end;

  TDomSpanishErrorTranslator = class(TDomCustomErrorTranslator)
  protected
    function GetSupportedLanguages: TIso639LanguageCodeSet; override;
  public
    constructor Create(AOwner: TComponent); override;
    function Translate(const Error: TdomError): WideString; override;
  end;

  TDomErrorAutoStrings = class(TUtilsCustomAliasedStrings)
  private
    FErrorTranslator: TDomCustomErrorTranslator;
  protected
    function Get(Index: Integer): string; override;
    function GetError(Index: Integer): TDomError; virtual;
    procedure SetErrorTranslator(const AErrorTranslator:
      TDomCustomErrorTranslator); virtual;
    procedure Notification(AComponent: TComponent; Operation: TOperation);
      override;
  public
    // List mutation:
    function AddError(AError: TDomError): Integer; overload; virtual;
    function AddError(const ErrorType: TXmlErrorType;
      const StartByteNumber, StartCharNumber, StartColumnNumber,
      StartLineNumber, StartTabsInLine, EndByteNumber, EndCharNumber,
      EndColumnNumber, EndLineNumber, EndTabsInLine: Int64;
      const UriStr: WideString; const RASObject: TDtdObject;
      const RNode: TdomNode; const Code, Clue: WideString): Integer; overload;
      virtual;
    procedure Assign(Source: TPersistent); override;
    procedure Clear; override;
    procedure Delete(Index: Integer); override;
    function InsertError(Index: Integer; AError: TDomError): Boolean; overload;
      virtual;
    function InsertError(Index: Integer; const ErrorType: TXmlErrorType;
      const StartByteNumber, StartCharNumber, StartColumnNumber,
      StartLineNumber, StartTabsInLine, EndByteNumber, EndCharNumber,
      EndColumnNumber, EndLineNumber, EndTabsInLine: Int64;
      const UriStr: WideString; const RASObject: TDtdObject;
      const RNode: TdomNode; const Code, Clue: WideString): Boolean; overload;
      virtual;
    // View Control:
    procedure DisableControls; virtual;
    procedure EnableControls; virtual;

    property Capacity;
    property Count;
    property Errors[Index: Integer]: TdomError read GetError;
    property Strings;
  published
    property Enabled;
    property ErrorTranslator: TDomCustomErrorTranslator read FErrorTranslator
      write SetErrorTranslator;
  end;

procedure Register;

implementation

procedure Register;
begin
  RegisterComponents('XDOM 4.1 Add-Ons', [
    TDomErrorAutoStrings,
    TDomDutchErrorTranslator,
    TDomEnglishErrorTranslator,
    TDomFrenchErrorTranslator,
    TDomGermanErrorTranslator,
    TDomItalianErrorTranslator,
    TDomPolishErrorTranslator,
    TDomPortugueseErrorTranslator,
    TDomSpanishErrorTranslator ]);
end;

{Local methods}

function ExtractFileNameFromUri(const Uri: WideString): WideString;
var
  I: Integer;
begin
  if Uri = '' then
    Result := ''
  else
  begin
    I := LastDelimiter('/', Uri);
    Result := Copy(Uri, I + 1, MaxInt);
  end;
end;

{ TDomCustomErrorTranslator }

procedure TDomCustomErrorTranslator.SetLanguage(
  const Value: TIso639LanguageCode);
begin
  if not (Value in SupportedLanguages) then
    raise ENot_Supported_Err.create('Not supported error.');
  FLanguage := Value;
end;

procedure TDomCustomErrorTranslator.SetTabWidth(const Value: Integer);
begin
  FTabWidth := Value;
end;

{ TDomDutchErrorTranslator }

constructor TDomDutchErrorTranslator.Create(AOwner: TComponent);
begin
  inherited;
  FLanguage := iso639_nl;
end;

function TDomDutchErrorTranslator.GetSupportedLanguages: TIso639LanguageCodeSet;
begin
  Result := [iso639_nl];
end;

function TDomDutchErrorTranslator.Translate(
  const Error: TdomError): WideString;
// This function was provided by Erik van der Poll,
// Locus Warehouse Management Systems b.v., Almere
var
  ErrorStr, FileNameStr, PosStr, SeverityStr, S: string;
begin
  with Error do
  begin
    case Severity of
      DOM_SEVERITY_FATAL_ERROR: SeverityStr := 'Fatal Fout';
      DOM_SEVERITY_ERROR: SeverityStr := 'Fout';
      DOM_SEVERITY_WARNING: SeverityStr := 'Waarschuwing';
    end;

    FileNameStr := ExtractFileNameFromUri(Uri);
    if EndLineNumber < 1 then
      PosStr := ''
    else if StartLineNumber = EndLineNumber then
    begin
      if StartColumnNumber = EndColumnNumber then
        FmtStr(PosStr, '%d:%d', [EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1) ])
      else
        FmtStr(PosStr, '%d:%d-%d', [EndLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
          EndColumnNumber]);
    end
    else
    begin
      FmtStr(PosStr, '%d:%d-%d:%d', [StartLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
        EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1)]);
    end;

    case RelatedException of
      ET_ATTLIST_DECL_START_EXPECTED:
        ErrorStr := 'ET_ATTLIST_DECL_START_EXPECTED';
      ET_BYTE_ORDER_MARK_ENCODING_MISMATCH:
        ErrorStr := 'ET_BYTE_ORDER_MARK_ENCODING_MISMATCH';
      ET_CDATA_START_EXPECTED:
        ErrorStr := 'ET_CDATA_START_EXPECTED';
      ET_COMMENT_START_EXPECTED:
        ErrorStr := 'ET_COMMENT_START_EXPECTED';
      ET_CONDITIONAL_SECTION_NOT_ALLOWED:
        ErrorStr := 'ET_CONDITIONAL_SECTION_NOT_ALLOWED';
      ET_DOCTYPE_START_EXPECTED:
        ErrorStr := 'ET_DOCTYPE_START_EXPECTED';
      ET_ELEMENT_DECL_START_EXPECTED:
        ErrorStr := 'ET_ELEMENT_DECL_START_EXPECTED';
      ET_ENTITY_DECL_START_EXPECTED:
        ErrorStr := 'ET_ENTITY_DECL_START_EXPECTED';
      ET_INVALID_ELEMENT_NAME:
        ErrorStr := 'Ongeldige element naam';
      ET_DOUBLE_HYPHEN_IN_COMMENT:
        ErrorStr := 'ET_DOUBLE_HYPHEN_IN_COMMENT';
      ET_DOUBLE_ROOT_ELEMENT:
        ErrorStr := 'Dubbel begin element';
      ET_DOUBLE_DOCTYPE:
        ErrorStr := 'Dubbele document type declaratie (DTD)';
      ET_HYPHEN_AT_COMMENT_END:
        ErrorStr := 'ET_HYPHEN_AT_COMMENT_END';
      ET_INVALID_ATTRIBUTE_NAME:
        ErrorStr := 'Ongeldige attribuut naam';
      ET_INVALID_ATTRIBUTE_VALUE:
        ErrorStr := 'Ongeldige attribuut waarde';
      ET_DOUBLE_ATTRIBUTE_NAME:
        ErrorStr := 'Dubbele attribuut naam in een element';
      ET_INVALID_ENCODING_NAME:
        ErrorStr := 'ET_INVALID_ENCODING_NAME';
      ET_INVALID_ENTITY_NAME:
        ErrorStr := 'Ongeldige entiteit naam';
      ET_INVALID_PARAMETER_ENTITY_NAME:
        ErrorStr := 'Ongeldige parameter entiteit naam';
      ET_INVALID_PROCESSING_INSTRUCTION:
        ErrorStr := 'Ongeldige verwerkings instruktie';
      ET_INVALID_VERSION_NUMBER:
        ErrorStr := 'ET_INVALID_VERSION_NUMBER';
      ET_INVALID_XML_DECL:
        ErrorStr := 'Ongeldige XML declaratie';
      ET_INVALID_CHAR_REF:
        ErrorStr := 'Ongeldige karakter referentie';
      ET_MISSING_QUOTATION_MARK:
        ErrorStr := 'Aanhalingstekens ontbreken';
      ET_MISSING_EQUALITY_SIGN:
        ErrorStr := 'Is-teken ontbreekt';
      ET_DOUBLE_EQUALITY_SIGN:
        ErrorStr := 'Dubbel is-teken';
      ET_MISSING_WHITE_SPACE:
        ErrorStr := 'Ontbrekende witruimte';
      ET_MISSING_START_TAG:
        ErrorStr := 'Eind-tag zonder begin-tag';
      ET_MISSING_ELEMENT_NAME:
        ErrorStr := 'ET_MISSING_ELEMENT_NAME';
      ET_MISSING_ENTITY_NAME:
        ErrorStr := 'ET_MISSING_ENTITY_NAME';
      ET_MISSING_END_TAG:
        ErrorStr := 'Eind-tag ontbreekt';
      ET_INVALID_CHARACTER:
        ErrorStr := 'Ongeldig karakter';
      ET_NOT_IN_ROOT_ELEMENT:
        ErrorStr := 'Tekst buiten het begin element';
      ET_KEYWORD_ANY_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_ANY_NOT_ALLOWED';
      ET_KEYWORD_EMPTY_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_EMPTY_NOT_ALLOWED';
      ET_KEYWORD_PCDATA_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_PCDATA_NOT_ALLOWED';
      ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL:
        ErrorStr := 'ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL';
      ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL';
      ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL';
      ET_NOTATION_DECL_START_EXPECTED:
        ErrorStr := 'ET_NOTATION_DECL_START_EXPECTED';
      ET_PARAMETER_ENTITY_REF_NOT_ALLOWED:
        ErrorStr := 'ET_PARAMETER_ENTITY_REF_NOT_ALLOWED';
      ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL:
        ErrorStr := 'ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL';
      ET_UNCLOSED_ATTLIST_DECL:
        ErrorStr := 'ET_UNCLOSED_ATTLIST_DECL';
      ET_SYSTEM_KEYWORD_EXPECTED:
        ErrorStr := 'ET_SYSTEM_KEYWORD_EXPECTED';
      ET_UNCLOSED_CDATA_SECTION:
        ErrorStr := 'ET_UNCLOSED_CDATA_SECTION';
      ET_UNCLOSED_CHAR_REF:
        ErrorStr := 'ET_UNCLOSED_CHAR_REF';
      ET_UNCLOSED_COMMENT:
        ErrorStr := 'ET_UNCLOSED_COMMENT';
      ET_UNCLOSED_CONDITIONAL_SECTION:
        ErrorStr := 'ET_UNCLOSED_CONDITIONAL_SECTION';
      ET_UNCLOSED_DOCTYPE:
        ErrorStr := 'ET_UNCLOSED_DOCTYPE';
      ET_UNCLOSED_ELEMENT:
        ErrorStr := 'ET_UNCLOSED_ELEMENT';
      ET_UNCLOSED_ELEMENT_DECL:
        ErrorStr := 'ET_UNCLOSED_ELEMENT_DECL';
      ET_UNCLOSED_ENTITY_DECL:
        ErrorStr := 'ET_UNCLOSED_ENTITY_DECL';
      ET_UNCLOSED_ENTITY_REF:
        ErrorStr := 'ET_UNCLOSED_ENTITY_REF';
      ET_UNCLOSED_NOTATION_DECL:
        ErrorStr := 'ET_UNCLOSED_NOTATION_DECL';
      ET_UNCLOSED_PARAMETER_ENTITY_REF:
        ErrorStr := 'ET_UNCLOSED_PARAMETER_ENTITY_REF';
      ET_UNCLOSED_PROCESSING_INSTRUCTION:
        ErrorStr := 'ET_UNCLOSED_PROCESSING_INSTRUCTION';
      ET_WRONG_ORDER:
        ErrorStr := 'Volgorde fout';
      ET_UNKNOWN_DECL_TYPE:
        ErrorStr := 'Onbekend declaratie type';
      ET_INVALID_ATTRIBUTE_DECL:
        ErrorStr := 'Ongeldige attribuut declaratie';
      ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL';
      ET_INVALID_ATTLIST_DECL_NAME:
        ErrorStr := 'ET_INVALID_ATTLIST_DECL_NAME';
      ET_INVALID_CONDITIONAL_SECTION:
        ErrorStr := 'Ongeldige conditionele sectie';
      ET_INVALID_ELEMENT_DECL:
        ErrorStr := 'Ongeldige element declaratie';
      ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL';
      ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL:
        ErrorStr := 'ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL';
      ET_INVALID_ENTITY_DECL:
        ErrorStr := 'Ongeldige entiteits declaratie';
      ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL';
      ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL';
      ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL';
      ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL';
      ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL';
      ET_INVALID_MARKUP_DECL:
        ErrorStr := 'Ongeldige markup declaratie';
      ET_INVALID_NOTATION_DECL:
        ErrorStr := 'Ongeldige notatie declaratie';
      ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL';
      ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL';
      ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL';
      ET_INVALID_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Ongeldige parameter entiteit declaratie';
      ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL:
        ErrorStr := 'ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL';
      ET_INVALID_TEXT_DECL:
        ErrorStr := 'Ongeldige tekst declaratie';
      ET_LEFT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := 'ET_LEFT_SQUARE_BRACKET_EXPECTED';
      ET_LT_IN_ATTRIBUTE_VALUE:
        ErrorStr := '"<" in de vervangingstekst van een attribuut waarde';
      ET_ATTRIBUTE_VALUE_REFERS_TO_EXTERNAL_ENTITY:
        ErrorStr := 'Attribuut waarde verwijst naar externe entiteit';
      ET_PE_BETWEEN_DECLARATIONS:
        ErrorStr := 'ET_PE_BETWEEN_DECLARATIONS';
      ET_PUBLIC_KEYWORD_EXPECTED:
        ErrorStr := 'ET_PUBLIC_KEYWORD_EXPECTED';
      ET_QUOTATION_MARK_EXPECTED:
        ErrorStr := 'ET_QUOTATION_MARK_EXPECTED';
      ET_RECURSIVE_REFERENCE:
        ErrorStr := 'Recursieve referentie gevonden';
      ET_REFERS_TO_UNPARSED_ENTITY:
        ErrorStr := 'Referentie naar een niet verwerkte entiteit';
      ET_RIGHT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := 'ET_RIGHT_SQUARE_BRACKET_EXPECTED';
      ET_NO_PROPER_MARKUP_REFERENCED:
        ErrorStr := 'Referentie naar een entiteit zonder juiste opmaak';
      ET_INVALID_COMMENT:
        ErrorStr := 'Ongeldig commentaar';
      ET_INVALID_CDATA_SECTION:
        ErrorStr := 'Ongeldige CDATA sectie';
      ET_INVALID_SYSTEM_LITERAL:
        ErrorStr := 'Ongeldige systeem tekstconstante';
      ET_INVALID_PUBID_LITERAL:
        ErrorStr := 'Ongeldige publieke tekstconstante';
      ET_INVALID_QUALIFIED_NAME:
        ErrorStr := 'Ongeldige namespace naamgeving';
      ET_INVALID_PREFIX:
        ErrorStr := 'Ongeldig namespace voorvoegsel';
      ET_INVALID_NAMESPACE_URI:
        ErrorStr := 'Ongeldig namespace adres';
      ET_NAMESPACE_URI_NOT_FOUND:
        ErrorStr := 'Geen namespace adres gevonden';
      ET_WRONG_PREFIX_MAPPING_NESTING:
        ErrorStr := 'Foute nesting van namespace voorvoegsels';
      ET_XML_VERSION_NOT_SUPPORTED:
        ErrorStr := 'ET_XML_VERSION_NOT_SUPPORTED';
      ET_ENCODING_NOT_SUPPORTED:
        ErrorStr := 'Codering niet ondersteund';
      ET_INVALID_CHARACTER_IN_EXT_ENTITY:
        ErrorStr := 'ET_INVALID_CHARACTER_IN_EXT_ENTITY';
      ET_EXT_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'ET_EXT_ENTITY_RESOURCE_NOT_FOUND';
      ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND';
      ET_DOUBLE_ATTLISTDECL:
        ErrorStr := 'Dubbele attribuut declaratie';
      ET_DOUBLE_ATTDEF:
        ErrorStr := 'Dubbele attribuut definitie';
      ET_DOUBLE_ENTITY_DECL:
        ErrorStr := 'Dubbele entiteit declaratie';
      ET_DOUBLE_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Dubbele parameter entiteit declaratie';
      ET_UNDEFINED_ENTITY_VC:
        ErrorStr := 'Ontbrekende entiteit declaratie';
      ET_UNDEFINED_ENTITY_WFC:
        ErrorStr := 'Ontbrekende entiteit declaratie';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC:
        ErrorStr := 'ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC:
        ErrorStr := 'ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC';
      ET_DOCTYPE_NOT_FOUND:
        ErrorStr := 'ET_DOCTYPE_NOT_FOUND';
      ET_DUPLICATE_ELEMENT_TYPE_DECL:
        ErrorStr := 'Dubbele element type declaratie';
      ET_DUPLICATE_NAME_IN_MIXED_CONTENT:
        ErrorStr := 'Naam komt meer keren voor in gemengde inhoud';
      ET_DUPLICATE_ID_ON_ELEMENT_TYPE:
        ErrorStr := 'Dubbel ID attribuut binnen hetzelfde element type';
      ET_UNDEFINED_NOTATION:
        ErrorStr := 'Niet gedefinieerde notatie naam';
      ET_DUPLICATE_NOTATION_ON_ELEMENT_TYPE:
        ErrorStr := 'Dubbel notatie attribuut binnen hetzelfde element type';
      ET_DUPLICATE_NOTATION_TOKEN:
        ErrorStr := 'Dubbel notatie kenmerk';
      ET_NOTATION_ON_EMPTY_ELEMENT:
        ErrorStr := 'Notatie attribuut definitie voor een leeg element type';
      ET_DUPLICATE_ENUMERATION_TOKEN:
        ErrorStr := 'Dubbele enumeratie waarde';
      ET_ATTRIBUTE_TYPE_MISMATCH:
        ErrorStr := 'Attribuut type en attribuut waarde komen niet overeen';
      ET_DUPLICATE_TOKENS:
        ErrorStr := 'Tokens meermalen gedefinieerd';
      ET_ID_NEITHER_IMPLIED_NOR_REQUIRED:
        ErrorStr :=
          'ID attribuut is niet gedefinieerd als #IMPLIED of #REQUIRED';
      ET_WRONG_ROOT_ELEMENT_TYPE:
        ErrorStr :=
          'Type van begin element komt niet overeen met de naam van de document type declaratie';
      ET_UNDEFINED_ELEMENT_TYPE:
        ErrorStr := 'Element type declaratie niet gevonden';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY';
      ET_ELEMENT_DECLARED_EMPTY_HAS_CONTENT:
        ErrorStr := 'Element met definitie EMPTY is niet leeg';
      ET_NONDETERMINISTIC_ELEMENT_CONTENT_MODEL:
        ErrorStr := 'Inhoud element model is niet deterministisch';
      ET_DUPLICATE_NOTATION_DECL:
        ErrorStr := 'Dubbele notatie declaratie';
      ET_UNDEFINED_ATTRIBUTE:
        ErrorStr := 'Attribuut definitie niet gevonden';
      ET_REQUIRED_ATTRIBUTE_NOT_SPECIFIED:
        ErrorStr := 'Verplicht attribuut niet gevonden';
      ET_FIXED_ATTRIBUTE_MISMATCH:
        ErrorStr :=
          'Gedefinieerde waarde en werkelijke waarde van een vast attribuut komen niet overeen';
      ET_ATTRIBUTE_DEFAULT_TYPE_MISMATCH:
        ErrorStr :=
          'Attribuut type en attribuut default waarde komen niet overeen';
      ET_DUPLICATE_ID_VALUE:
        ErrorStr := 'Dubbele ID waarde';
      ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC';
      ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC';
      ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC:
        ErrorStr := 'ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC';
      ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC:
        ErrorStr := 'ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC';
      ET_TARGET_ID_VALUE_NOT_FOUND:
        ErrorStr :=
          'IDREF of IDREFS waarde verwijst naar een niet bestaande ID waarde';
      ET_UNDEFINED_TARGET_UNPARSED_ENTITY:
        ErrorStr :=
          'ENTITY of ENTITIES waarde verwijst naar een niet bestaande entiteit';
      ET_PREDEFINED_ENTITY_INCORRECTLY_REDECLARED:
        ErrorStr := 'Foute declaratie voor een voorgedefinieerde entiteit';
      ET_UNRESOLVABLE_EXTERNAL_SUBSET:
        ErrorStr := 'Externe subset van de DTD niet gevonden';
      ET_UNDEFINED_PARAMETER_ENTITY:
        ErrorStr := 'Parameter entiteit declaratie niet gevonden';
      ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION';
      ET_PE_NOT_PROPERLY_NESTED_WITH_DECL:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_DECL';
      ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP';
    else
      ErrorStr := 'Fout in broncode';
    end; {case ...}
  end; {with ...}

  if PosStr <> '' then
    S := Format('[%s] %s(%s): %s', [SeverityStr, FileNameStr, PosStr, ErrorStr])
  else
    S := Format('[%s] %s: %s', [SeverityStr, FileNameStr, ErrorStr]);

  Result := EncodingToUTF16(TISO8859_1Codec, S);
end;

{ TDomEnglishErrorTranslator }

constructor TDomEnglishErrorTranslator.Create(AOwner: TComponent);
begin
  inherited;
  FLanguage := iso639_en;
end;

function TDomEnglishErrorTranslator.GetSupportedLanguages:
  TIso639LanguageCodeSet;
begin
  Result := [iso639_en];
end;

function TDomEnglishErrorTranslator.Translate(
  const Error: TdomError): WideString;
var
  ErrorStr, FileNameStr, PosStr, SeverityStr, S: string;
begin
  with Error do
  begin
    case Severity of
      DOM_SEVERITY_FATAL_ERROR: SeverityStr := 'Fatal Error';
      DOM_SEVERITY_ERROR: SeverityStr := 'Error';
      DOM_SEVERITY_WARNING: SeverityStr := 'Warning';
    end;

    FileNameStr := ExtractFileNameFromUri(Uri);
    if EndLineNumber < 1 then
      PosStr := ''
    else if StartLineNumber = EndLineNumber then
    begin
      if StartColumnNumber = EndColumnNumber then
        FmtStr(PosStr, '%d:%d', [EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1) ])
      else
        FmtStr(PosStr, '%d:%d-%d', [EndLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
          EndColumnNumber]);
    end
    else
    begin
      FmtStr(PosStr, '%d:%d-%d:%d', [StartLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
        EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1)]);
    end;

    case RelatedException of
      ET_ATTLIST_DECL_START_EXPECTED:
        ErrorStr := 'Start of attribute list declaration expected';
      ET_BYTE_ORDER_MARK_ENCODING_MISMATCH:
        ErrorStr := 'Byte order mark does not match the declared encoding';
      ET_CDATA_START_EXPECTED:
        ErrorStr := 'Start of CDATA section expected';
      ET_COMMENT_START_EXPECTED:
        ErrorStr := 'Start of comment expected';
      ET_CONDITIONAL_SECTION_NOT_ALLOWED:
        ErrorStr := 'Conditional section not allowed in internal subset';
      ET_DOCTYPE_START_EXPECTED:
        ErrorStr := 'Start of document type declaration (DTD) expected';
      ET_ELEMENT_DECL_START_EXPECTED:
        ErrorStr := 'Start of element declaration expected';
      ET_ENTITY_DECL_START_EXPECTED:
        ErrorStr := 'Start of entity declaration expected';
      ET_INVALID_ELEMENT_NAME:
        ErrorStr := 'Invalid element name';
      ET_DOUBLE_HYPHEN_IN_COMMENT:
        ErrorStr := 'Double hyphen in comment';
      ET_DOUBLE_ROOT_ELEMENT:
        ErrorStr := 'Double root element';
      ET_DOUBLE_DOCTYPE:
        ErrorStr := 'Double document type declaration (DTD)';
      ET_HYPHEN_AT_COMMENT_END:
        ErrorStr := 'Comment text ends with a hyphen';
      ET_INVALID_ATTRIBUTE_NAME:
        ErrorStr := 'Invalid attribute name';
      ET_INVALID_ATTRIBUTE_VALUE:
        ErrorStr := 'Invalid attribute value';
      ET_DOUBLE_ATTRIBUTE_NAME:
        ErrorStr := 'Double attribute name in one element';
      ET_INVALID_ENCODING_NAME:
        ErrorStr := 'Invalid encoding name';
      ET_INVALID_ENTITY_NAME:
        ErrorStr := 'Invalid entity name';
      ET_INVALID_PARAMETER_ENTITY_NAME:
        ErrorStr := 'Invalid parameter entity name';
      ET_INVALID_PROCESSING_INSTRUCTION:
        ErrorStr := 'Invalid processing instruction';
      ET_INVALID_VERSION_NUMBER:
        ErrorStr := 'Invalid version number';
      ET_INVALID_XML_DECL:
        ErrorStr := 'Invalid XML declaration';
      ET_INVALID_CHAR_REF:
        ErrorStr := 'Invalid character reference';
      ET_MISSING_QUOTATION_MARK:
        ErrorStr := 'Missing quotation marks';
      ET_MISSING_EQUALITY_SIGN:
        ErrorStr := 'Missing equality sign';
      ET_DOUBLE_EQUALITY_SIGN:
        ErrorStr := 'Double equality sign';
      ET_MISSING_WHITE_SPACE:
        ErrorStr := 'Missing white-space';
      ET_MISSING_START_TAG:
        ErrorStr := 'End-tag without start-tag';
      ET_MISSING_ELEMENT_NAME:
        ErrorStr := 'Missing element name';
      ET_MISSING_ENTITY_NAME:
        ErrorStr := 'Missing entity name';
      ET_MISSING_END_TAG:
        ErrorStr := 'Missing end-tag';
      ET_INVALID_CHARACTER:
        ErrorStr := 'Invalid character';
      ET_NOT_IN_ROOT_ELEMENT:
        ErrorStr := 'Character(s) outside the root element';
      ET_ROOT_ELEMENT_NOT_FOUND:
        ErrorStr := 'Root element not found';
      ET_KEYWORD_ANY_NOT_ALLOWED:
        ErrorStr := '"ANY" keyword not allowed here';
      ET_KEYWORD_EMPTY_NOT_ALLOWED:
        ErrorStr := '"EMPTY" keyword not allowed here';
      ET_KEYWORD_PCDATA_NOT_ALLOWED:
        ErrorStr := '"#PCDATA" keyword not allowed here';
      ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL:
        ErrorStr :=  'Malformed content model in element declaration';
      ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'Missing entity value in entity declaration';
      ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Missing entity value in parameter entity declaration';
      ET_NOTATION_DECL_START_EXPECTED:
        ErrorStr := 'Start of notation declaration expected';
      ET_PARAMETER_ENTITY_REF_NOT_ALLOWED:
        ErrorStr := 'Parameter entity reference not allowed here';
      ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL:
        ErrorStr := 'Invalid content model token in element declaration';
      ET_UNCLOSED_ATTLIST_DECL:
        ErrorStr := 'Unclosed attribute-list declaration';
      ET_SYSTEM_KEYWORD_EXPECTED:
        ErrorStr := '"SYSTEM" keyword expected';
      ET_UNCLOSED_CDATA_SECTION:
        ErrorStr := 'Unclosed CDATA section';
      ET_UNCLOSED_CHAR_REF:
        ErrorStr := 'Unclosed character reference';
      ET_UNCLOSED_COMMENT:
        ErrorStr := 'Unclosed comment';
      ET_UNCLOSED_CONDITIONAL_SECTION:
        ErrorStr := 'Unclosed conditional section';
      ET_UNCLOSED_DOCTYPE:
        ErrorStr := 'Unclosed document type declaration (DTD)';
      ET_UNCLOSED_ELEMENT:
        ErrorStr := 'Unclosed element';
      ET_UNCLOSED_ELEMENT_DECL:
        ErrorStr := 'Unclosed element declaration';
      ET_UNCLOSED_ENTITY_DECL:
        ErrorStr := 'Unclosed entity declaration';
      ET_UNCLOSED_ENTITY_REF:
        ErrorStr := 'Unclosed entity reference';
      ET_UNCLOSED_NOTATION_DECL:
        ErrorStr := 'Unclosed notation declaration';
      ET_UNCLOSED_PARAMETER_ENTITY_REF:
        ErrorStr := 'Unclosed parameter entity reference';
      ET_UNCLOSED_PROCESSING_INSTRUCTION:
        ErrorStr := 'Unclosed processsing instruction';
      ET_WRONG_ORDER:
        ErrorStr := 'Wrong order';
      ET_UNKNOWN_DECL_TYPE:
        ErrorStr := 'Unknown declaration type';
      ET_INVALID_ATTRIBUTE_DECL:
        ErrorStr := 'Invalid attribute declaration';
      ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'Invalid attribute name in attribute declaration';
      ET_INVALID_ATTLIST_DECL_NAME:
        ErrorStr := 'Invalid name of attribute declaration';
      ET_INVALID_CONDITIONAL_SECTION:
        ErrorStr := 'Invalid conditional section';
      ET_INVALID_ELEMENT_DECL:
        ErrorStr := 'Invalid element declaration';
      ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'Invalid element name in attribute declaration';
      ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL:
        ErrorStr := 'Invalid element name in element declaration';
      ET_INVALID_ENTITY_DECL:
        ErrorStr := 'Invalid entity declaration';
      ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL:
        ErrorStr := 'Invalid entity name in entity declaration';
      ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Invalid entity name in parameter entity declaration';
      ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Invalid entity value in parameter entity declaration';
      ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'Invalid entity value in entity declaration';
      ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'Invalid enumeration token in attribute declaration';
      ET_INVALID_MARKUP_DECL:
        ErrorStr := 'Invalid markup declaration';
      ET_INVALID_NOTATION_DECL:
        ErrorStr := 'Invalid notation declaration';
      ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL:
        ErrorStr := 'Invalid notation name in entity declaration';
      ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL:
        ErrorStr := 'Invalid notation name in notation declaration';
      ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'Invalid notation token in attribute declaration';
      ET_INVALID_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Invalid parameter entity declaration';
      ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL:
        ErrorStr := 'Invalid root element name in document type declaration';
      ET_INVALID_TEXT_DECL:
        ErrorStr := 'Invalid text declaration';
      ET_LEFT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := '"[" expected';
      ET_LT_IN_ATTRIBUTE_VALUE:
        ErrorStr :=
          '"<" in the replacement text of an entity referred to directly or indirectly in an attribute value';
      ET_ATTRIBUTE_VALUE_REFERS_TO_EXTERNAL_ENTITY:
        ErrorStr := 'Attribute value refers to external entity';
      ET_PE_BETWEEN_DECLARATIONS:
        ErrorStr := 'Incomplete markup declaration generated by parameter entity reference between declarations';
      ET_PUBLIC_KEYWORD_EXPECTED:
        ErrorStr := '"PUBLIC" keyword expected';
      ET_QUOTATION_MARK_EXPECTED:
        ErrorStr := 'Quotation mark expected';
      ET_RECURSIVE_REFERENCE:
        ErrorStr := 'Recursive reference found';
      ET_REFERS_TO_UNPARSED_ENTITY:
        ErrorStr := 'Reference to an unparsed entity';
      ET_RIGHT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := '"]" expected';
      ET_NO_PROPER_MARKUP_REFERENCED:
        ErrorStr := 'Reference to an entity containing no proper markup';
      ET_INVALID_COMMENT:
        ErrorStr := 'Invalid comment';
      ET_INVALID_CDATA_SECTION:
        ErrorStr := 'Invalid CDATA section';
      ET_INVALID_SYSTEM_LITERAL:
        ErrorStr := 'Invalid system literal';
      ET_INVALID_PUBID_LITERAL:
        ErrorStr := 'Invalid pubid literal';
      ET_INVALID_QUALIFIED_NAME:
        ErrorStr := 'Invalid qualified name';
      ET_INVALID_PREFIX:
        ErrorStr := 'Invalid prefix';
      ET_INVALID_NAMESPACE_URI:
        ErrorStr := 'Invalid namespace URI';
      ET_NAMESPACE_URI_NOT_FOUND:
        ErrorStr := 'Namespace URI not found';
      ET_WRONG_PREFIX_MAPPING_NESTING:
        ErrorStr := 'Wrong prefix mapping nesting';
      ET_XML_VERSION_NOT_SUPPORTED:
        ErrorStr := 'XML version not supported';
      ET_ENCODING_NOT_SUPPORTED:
        ErrorStr := 'Encoding not supported';
      ET_INVALID_CHARACTER_IN_EXT_ENTITY:
        ErrorStr := 'Invalid character in external parsed entity';
      ET_EXT_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'External entity resource not found';
      ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'External parameter entity resource not found';
      ET_DOUBLE_ATTLISTDECL:
        ErrorStr := 'Double attribute-list declaration';
      ET_DOUBLE_ATTDEF:
        ErrorStr := 'Double attribute defintion';
      ET_DOUBLE_ENTITY_DECL:
        ErrorStr := 'Double entity declaration';
      ET_DOUBLE_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Double parameter entity declaration';
      ET_UNDEFINED_ENTITY_VC:
        ErrorStr := 'Undefined Entity';
      ET_UNDEFINED_ENTITY_WFC:
        ErrorStr := 'Undefined Entity';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC:
        ErrorStr := 'Entity referenced before defined';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC:
        ErrorStr := 'Entity referenced before defined';
      ET_DOCTYPE_NOT_FOUND:
        ErrorStr := 'Missing document type declaration (DTD)';
      ET_DUPLICATE_ELEMENT_TYPE_DECL:
        ErrorStr := 'Double element type declaration';
      ET_DUPLICATE_NAME_IN_MIXED_CONTENT:
        ErrorStr := 'Duplicate name in mixed content';
      ET_DUPLICATE_ID_ON_ELEMENT_TYPE:
        ErrorStr := 'Duplicate ID attribute on the same element type declared';
      ET_UNDEFINED_NOTATION:
        ErrorStr := 'Undefined notation';
      ET_DUPLICATE_NOTATION_ON_ELEMENT_TYPE:
        ErrorStr :=
          'Duplicate notation attribute on the same element type declared';
      ET_DUPLICATE_NOTATION_TOKEN:
        ErrorStr := 'Duplicate notation token';
      ET_NOTATION_ON_EMPTY_ELEMENT:
        ErrorStr := 'Notation attribute on an empty element type declared';
      ET_DUPLICATE_ENUMERATION_TOKEN:
        ErrorStr := 'Duplicate enumeration token';
      ET_ATTRIBUTE_TYPE_MISMATCH:
        ErrorStr := 'Attribute type and attribute value do not match';
      ET_DUPLICATE_TOKENS:
        ErrorStr := 'Duplicate tokens declared';
      ET_ID_NEITHER_IMPLIED_NOR_REQUIRED:
        ErrorStr :=
          'ID attribute is neither declared as #IMPLIED nor as #REQUIRED';
      ET_WRONG_ROOT_ELEMENT_TYPE:
        ErrorStr :=
          'Type of root element does not match name of document type declaration';
      ET_UNDEFINED_ELEMENT_TYPE:
        ErrorStr := 'Undefined element type';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY:
        ErrorStr := 'Element contains a CDATA section where only child elements are allowed';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE:
        ErrorStr := 'Element contains another element which does not match the content model';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY:
        ErrorStr := 'Element contains another element where only child PCDATA (text) is allowed';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY:
        ErrorStr := 'Element contains a PCDATA (text) where only child elements are allowed';
      ET_ELEMENT_DECLARED_EMPTY_HAS_CONTENT:
        ErrorStr := 'Element declared EMPTY is not empty';
      ET_NONDETERMINISTIC_ELEMENT_CONTENT_MODEL:
        ErrorStr := 'Content model of an element is not deterministic';
      ET_DUPLICATE_NOTATION_DECL:
        ErrorStr := 'Duplicate notation declaration';
      ET_UNDEFINED_ATTRIBUTE:
        ErrorStr := 'Attribute undefined';
      ET_REQUIRED_ATTRIBUTE_NOT_SPECIFIED:
        ErrorStr := 'Required attribute not specified';
      ET_FIXED_ATTRIBUTE_MISMATCH:
        ErrorStr :=
          'Declared value and actual value of a fixed attribute do not match';
      ET_ATTRIBUTE_DEFAULT_TYPE_MISMATCH:
        ErrorStr := 'Attribute type and attribute default do not match';
      ET_DUPLICATE_ID_VALUE:
        ErrorStr := 'Duplicate ID value';
      ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'Externally declared attribute not normalized in standalone document';
      ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'Externally declared default attribute not specified in standalone document';
      ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC:
        ErrorStr := 'Externally declared entity referenced in standalone document';
      ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC:
        ErrorStr := 'Whitespace in externally declared element content of standalone document';
      ET_TARGET_ID_VALUE_NOT_FOUND:
        ErrorStr :=
          'IDREF or IDREFS value refers to a non-existent target ID value';
      ET_UNDEFINED_TARGET_UNPARSED_ENTITY:
        ErrorStr :=
          'ENTITY or ENTITIES value refers to an undefined target unparsed entity';
      ET_PREDEFINED_ENTITY_INCORRECTLY_REDECLARED:
        ErrorStr := 'Predefined entity incorrectly redeclared';
      ET_UNRESOLVABLE_EXTERNAL_SUBSET:
        ErrorStr := 'External subset of the DTD not found';
      ET_UNDEFINED_PARAMETER_ENTITY:
        ErrorStr := 'Undefined parameter entity';
      ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION:
        ErrorStr := 'Parameter entity replacement text not properly nested with conditional section';
      ET_PE_NOT_PROPERLY_NESTED_WITH_DECL:
        ErrorStr := 'Parameter entity replacement text not properly nested with markup declarations';
      ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP:
        ErrorStr := 'Parameter entity replacement text not properly nested with parenthesized group';
    else
      ErrorStr := 'Invalid source-code';
    end; {case ...}
  end; {with ...}

  if PosStr <> '' then
    S := Format('[%s] %s(%s): %s', [SeverityStr, FileNameStr, PosStr, ErrorStr])
  else
    S := Format('[%s] %s: %s', [SeverityStr, FileNameStr, ErrorStr]);

  Result := EncodingToUTF16(TISO8859_1Codec, S);
end;

{ TDomFrenchErrorTranslator }

constructor TDomFrenchErrorTranslator.Create(AOwner: TComponent);
begin
  inherited;
  FLanguage := iso639_fr;
end;

function TDomFrenchErrorTranslator.GetSupportedLanguages:
  TIso639LanguageCodeSet;
begin
  Result := [iso639_fr];
end;

function TDomFrenchErrorTranslator.Translate(
  const Error: TdomError): WideString;
// - This function was provided by Bernard Eblin -
var
  ErrorStr, FileNameStr, PosStr, SeverityStr, S: string;
begin
  with Error do
  begin
    case Severity of
      DOM_SEVERITY_FATAL_ERROR: SeverityStr := 'Erreur Fatale';
      DOM_SEVERITY_ERROR: SeverityStr := 'Erreur';
      DOM_SEVERITY_WARNING: SeverityStr := 'Attention';
    end;

    FileNameStr := ExtractFileNameFromUri(Uri);
    if EndLineNumber < 1 then
      PosStr := ''
    else if StartLineNumber = EndLineNumber then
    begin
      if StartColumnNumber = EndColumnNumber then
        FmtStr(PosStr, '%d:%d', [EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1) ])
      else
        FmtStr(PosStr, '%d:%d-%d', [EndLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
          EndColumnNumber]);
    end
    else
    begin
      FmtStr(PosStr, '%d:%d-%d:%d', [StartLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
        EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1)]);
    end;

    case RelatedException of
      ET_ATTLIST_DECL_START_EXPECTED:
        ErrorStr := 'ET_ATTLIST_DECL_START_EXPECTED';
      ET_BYTE_ORDER_MARK_ENCODING_MISMATCH:
        ErrorStr := 'ET_BYTE_ORDER_MARK_ENCODING_MISMATCH';
      ET_CDATA_START_EXPECTED:
        ErrorStr := 'ET_CDATA_START_EXPECTED';
      ET_COMMENT_START_EXPECTED:
        ErrorStr := 'ET_COMMENT_START_EXPECTED';
      ET_CONDITIONAL_SECTION_NOT_ALLOWED:
        ErrorStr := 'ET_CONDITIONAL_SECTION_NOT_ALLOWED';
      ET_DOCTYPE_START_EXPECTED:
        ErrorStr := 'ET_DOCTYPE_START_EXPECTED';
      ET_ELEMENT_DECL_START_EXPECTED:
        ErrorStr := 'ET_ELEMENT_DECL_START_EXPECTED';
      ET_ENTITY_DECL_START_EXPECTED:
        ErrorStr := 'ET_ENTITY_DECL_START_EXPECTED';
      ET_INVALID_ELEMENT_NAME:
        ErrorStr := 'Nom d'';�l�ment invalide';
      ET_DOUBLE_HYPHEN_IN_COMMENT:
        ErrorStr := 'ET_DOUBLE_HYPHEN_IN_COMMENT';
      ET_DOUBLE_ROOT_ELEMENT:
        ErrorStr := 'Double �l�ment racine';
      ET_DOUBLE_DOCTYPE:
        ErrorStr := 'Double d�claration de type de document (DTD)';
      ET_HYPHEN_AT_COMMENT_END:
        ErrorStr := 'ET_HYPHEN_AT_COMMENT_END';
      ET_INVALID_ATTRIBUTE_NAME:
        ErrorStr := 'Nom d'';attribut invalide';
      ET_INVALID_ATTRIBUTE_VALUE:
        ErrorStr := 'Valeur d'';attribut invalide';
      ET_DOUBLE_ATTRIBUTE_NAME:
        ErrorStr := 'Double nom d'';attribut dans un �l�ment';
      ET_INVALID_ENCODING_NAME:
        ErrorStr := 'ET_INVALID_ENCODING_NAME';
      ET_INVALID_ENTITY_NAME:
        ErrorStr := 'Nom d'';entit� invalide';
      ET_INVALID_PARAMETER_ENTITY_NAME:
        ErrorStr := 'Nom d'';entit� param�tre invalide';
      ET_INVALID_PROCESSING_INSTRUCTION:
        ErrorStr := 'Instruction de commande invalide';
      ET_INVALID_VERSION_NUMBER:
        ErrorStr := 'ET_INVALID_VERSION_NUMBER';
      ET_INVALID_XML_DECL:
        ErrorStr := 'D�claration XML invalide';
      ET_INVALID_CHAR_REF:
        ErrorStr := 'R�f�rence de caract�re invalide';
      ET_MISSING_QUOTATION_MARK:
        ErrorStr := 'Apostrophe (''): manquant';
      ET_MISSING_EQUALITY_SIGN:
        ErrorStr := 'Signe �gale manquant';
      ET_DOUBLE_EQUALITY_SIGN:
        ErrorStr := 'Double signe �gale';
      ET_MISSING_WHITE_SPACE:
        ErrorStr := 'Espace manquant';
      ET_MISSING_START_TAG:
        ErrorStr := 'Tag de fin sans tag de d�but';
      ET_MISSING_ELEMENT_NAME:
        ErrorStr := 'ET_MISSING_ELEMENT_NAME';
      ET_MISSING_ENTITY_NAME:
        ErrorStr := 'ET_MISSING_ENTITY_NAME';
      ET_MISSING_END_TAG:
        ErrorStr := 'Tag de fin manquant';
      ET_INVALID_CHARACTER:
        ErrorStr := 'Caract�re invalide';
      ET_NOT_IN_ROOT_ELEMENT:
        ErrorStr := 'Caract�re(s) en dehors de l'';�l�ment racine';
      ET_ROOT_ELEMENT_NOT_FOUND:
        ErrorStr := 'El�ment racine non trouv�';
      ET_KEYWORD_ANY_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_ANY_NOT_ALLOWED';
      ET_KEYWORD_EMPTY_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_EMPTY_NOT_ALLOWED';
      ET_KEYWORD_PCDATA_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_PCDATA_NOT_ALLOWED';
      ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL:
        ErrorStr := 'ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL';
      ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL';
      ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL';
      ET_NOTATION_DECL_START_EXPECTED:
        ErrorStr := 'ET_NOTATION_DECL_START_EXPECTED';
      ET_PARAMETER_ENTITY_REF_NOT_ALLOWED:
        ErrorStr := 'ET_PARAMETER_ENTITY_REF_NOT_ALLOWED';
      ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL:
        ErrorStr := 'ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL';
      ET_UNCLOSED_ATTLIST_DECL:
        ErrorStr := 'ET_UNCLOSED_ATTLIST_DECL';
      ET_SYSTEM_KEYWORD_EXPECTED:
        ErrorStr := 'ET_SYSTEM_KEYWORD_EXPECTED';
      ET_UNCLOSED_CDATA_SECTION:
        ErrorStr := 'ET_UNCLOSED_CDATA_SECTION';
      ET_UNCLOSED_CHAR_REF:
        ErrorStr := 'ET_UNCLOSED_CHAR_REF';
      ET_UNCLOSED_COMMENT:
        ErrorStr := 'ET_UNCLOSED_COMMENT';
      ET_UNCLOSED_CONDITIONAL_SECTION:
        ErrorStr := 'ET_UNCLOSED_CONDITIONAL_SECTION';
      ET_UNCLOSED_DOCTYPE:
        ErrorStr := 'ET_UNCLOSED_DOCTYPE';
      ET_UNCLOSED_ELEMENT:
        ErrorStr := 'ET_UNCLOSED_ELEMENT';
      ET_UNCLOSED_ELEMENT_DECL:
        ErrorStr := 'ET_UNCLOSED_ELEMENT_DECL';
      ET_UNCLOSED_ENTITY_DECL:
        ErrorStr := 'ET_UNCLOSED_ENTITY_DECL';
      ET_UNCLOSED_ENTITY_REF:
        ErrorStr := 'ET_UNCLOSED_ENTITY_REF';
      ET_UNCLOSED_NOTATION_DECL:
        ErrorStr := 'ET_UNCLOSED_NOTATION_DECL';
      ET_UNCLOSED_PARAMETER_ENTITY_REF:
        ErrorStr := 'ET_UNCLOSED_PARAMETER_ENTITY_REF';
      ET_UNCLOSED_PROCESSING_INSTRUCTION:
        ErrorStr := 'ET_UNCLOSED_PROCESSING_INSTRUCTION';
      ET_WRONG_ORDER:
        ErrorStr := 'Mauvais ordre';
      ET_UNKNOWN_DECL_TYPE:
        ErrorStr := 'D�claration de type inconnue';
      ET_INVALID_ATTRIBUTE_DECL:
        ErrorStr := 'D�claration d'';attribut invalide';
      ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL';
      ET_INVALID_ATTLIST_DECL_NAME:
        ErrorStr := 'ET_INVALID_ATTLIST_DECL_NAME';
      ET_INVALID_CONDITIONAL_SECTION:
        ErrorStr := 'Section conditionnelle invalide';
      ET_INVALID_ELEMENT_DECL:
        ErrorStr := 'D�claration d'';�l�ment invalide';
      ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL';
      ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL:
        ErrorStr := 'ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL';
      ET_INVALID_ENTITY_DECL:
        ErrorStr := 'D�claration d'';entit� invalide';
      ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL';
      ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL';
      ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL';
      ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL';
      ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL';
      ET_INVALID_MARKUP_DECL:
        ErrorStr := 'ET_INVALID_MARKUP_DECL';
      ET_INVALID_NOTATION_DECL:
        ErrorStr := 'D�claration de notation invalide';
      ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL';
      ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL';
      ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL';
      ET_INVALID_PARAMETER_ENTITY_DECL:
        ErrorStr := 'D�claration d'';entit� param�tre invalide';
      ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL:
        ErrorStr := 'ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL';
      ET_INVALID_TEXT_DECL:
        ErrorStr := 'D�claration de texte invalide';
      ET_LEFT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := 'ET_LEFT_SQUARE_BRACKET_EXPECTED';
      ET_LT_IN_ATTRIBUTE_VALUE:
        ErrorStr :=
          '"<" dans le texte de remplacement d'';une entit� se r�f�re directement ou indirectement � la valeur d'';un attribut';
      ET_ATTRIBUTE_VALUE_REFERS_TO_EXTERNAL_ENTITY:
        ErrorStr := 'La valeur d'';attribut se r�f�re � une entit� externe';
      ET_PE_BETWEEN_DECLARATIONS:
        ErrorStr := 'ET_PE_BETWEEN_DECLARATIONS';
      ET_PUBLIC_KEYWORD_EXPECTED:
        ErrorStr := 'ET_PUBLIC_KEYWORD_EXPECTED';
      ET_QUOTATION_MARK_EXPECTED:
        ErrorStr := 'ET_QUOTATION_MARK_EXPECTED';
      ET_RECURSIVE_REFERENCE:
        ErrorStr := 'R�f�rence r�cursive trouv�e';
      ET_REFERS_TO_UNPARSED_ENTITY:
        ErrorStr := 'R�f�rence � une entit�e non pars�e';
      ET_RIGHT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := 'ET_RIGHT_SQUARE_BRACKET_EXPECTED';
      ET_NO_PROPER_MARKUP_REFERENCED:
        ErrorStr :=
          'R�f�rence � une entit� ne contenant pas de marque correcte';
      ET_INVALID_COMMENT:
        ErrorStr := 'Commentaire invalide';
      ET_INVALID_CDATA_SECTION:
        ErrorStr := 'Section CDATA invalide';
      ET_INVALID_SYSTEM_LITERAL:
        ErrorStr := 'Syst�me lit�ral invalide';
      ET_INVALID_PUBID_LITERAL:
        ErrorStr := 'Cha�ne pubid invalide';
      ET_INVALID_QUALIFIED_NAME:
        ErrorStr := 'Non qualifi� invalide';
      ET_INVALID_PREFIX:
        ErrorStr := 'Pr�fixe invalide';
      ET_INVALID_NAMESPACE_URI:
        ErrorStr := 'Espace de nom URI invalide';
      ET_NAMESPACE_URI_NOT_FOUND:
        ErrorStr := 'Espace de nom URI non trouv�';
      ET_WRONG_PREFIX_MAPPING_NESTING:
        ErrorStr := 'Mauvais emboitement du pr�fixe mapp�';
      ET_XML_VERSION_NOT_SUPPORTED:
        ErrorStr := 'ET_XML_VERSION_NOT_SUPPORTED';
      ET_ENCODING_NOT_SUPPORTED:
        ErrorStr := 'Encodage non support�';
      ET_INVALID_CHARACTER_IN_EXT_ENTITY:
        ErrorStr := 'ET_INVALID_CHARACTER_IN_EXT_ENTITY';
      ET_EXT_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'ET_EXT_ENTITY_RESOURCE_NOT_FOUND';
      ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND';
      ET_DOUBLE_ATTLISTDECL:
        ErrorStr := 'ET_DOUBLE_ATTLISTDECL';
      ET_DOUBLE_ATTDEF:
        ErrorStr := 'Double d�finition d'';attribut';
      ET_DOUBLE_ENTITY_DECL:
        ErrorStr := 'Double d�claration d'';entit�';
      ET_DOUBLE_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Double d�claration d'';entit� param�tre';
      ET_UNDEFINED_ENTITY_VC:
        ErrorStr := 'Entit�e d�clar�e non trouv�e';
      ET_UNDEFINED_ENTITY_WFC:
        ErrorStr := 'Entit�e d�clar�e non trouv�e';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC:
        ErrorStr := 'ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC:
        ErrorStr := 'ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC';
      ET_DOCTYPE_NOT_FOUND:
        ErrorStr := 'ET_DOCTYPE_NOT_FOUND';
      ET_DUPLICATE_ELEMENT_TYPE_DECL:
        ErrorStr := 'Double d�claration de type �l�ment';
      ET_DUPLICATE_NAME_IN_MIXED_CONTENT:
        ErrorStr := 'Nom dupliqu� dans un contenu mixte';
      ET_DUPLICATE_ID_ON_ELEMENT_TYPE:
        ErrorStr :=
          'ID d'';attribut dupliqu� dans un �l�ment d�clar� de m�me type';
      ET_UNDEFINED_NOTATION:
        ErrorStr := 'Notation du nom non d�clar�';
      ET_DUPLICATE_NOTATION_ON_ELEMENT_TYPE:
        ErrorStr :=
          'Notation d'';attribut dupliqu� dans un �l�ment d�clar� de m�me type';
      ET_DUPLICATE_NOTATION_TOKEN:
        ErrorStr := 'Notation de token dupliqu�e';
      ET_NOTATION_ON_EMPTY_ELEMENT:
        ErrorStr := 'Notation d'';attribut sur un type �l�ment vide d�clar�';
      ET_DUPLICATE_ENUMERATION_TOKEN:
        ErrorStr := 'Token �num�r� dupliqu�';
      ET_ATTRIBUTE_TYPE_MISMATCH:
        ErrorStr := 'La valeur d''atribut ne correspond pas � son type';
      ET_DUPLICATE_TOKENS:
        ErrorStr := 'Token d�clar� dupliqu�';
      ET_ID_NEITHER_IMPLIED_NOR_REQUIRED:
        ErrorStr := 'ID attribut jamais d�clar� ni en #IMPLIED ni en #REQUIRED';
      ET_WRONG_ROOT_ELEMENT_TYPE:
        ErrorStr :=
          'Le type de l'';�l�ment racine ne correspond pas � la d�claration du nom du document';
      ET_UNDEFINED_ELEMENT_TYPE:
        ErrorStr := 'D�claration du type d'';�l�ment non trouv�e';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY';
      ET_ELEMENT_DECLARED_EMPTY_HAS_CONTENT:
        ErrorStr := 'El�ment d�clar� EMPTY non vide';
      ET_NONDETERMINISTIC_ELEMENT_CONTENT_MODEL:
        ErrorStr := 'Model de contenu d'';un �l�ment non d�terministe';
      ET_DUPLICATE_NOTATION_DECL:
        ErrorStr := 'D�claration de notation dupliqu�e';
      ET_UNDEFINED_ATTRIBUTE:
        ErrorStr := 'D�finition d'';attribut non trouv�e';
      ET_REQUIRED_ATTRIBUTE_NOT_SPECIFIED:
        ErrorStr := 'Attribut requis non trouv�';
      ET_FIXED_ATTRIBUTE_MISMATCH:
        ErrorStr :=
          'La valeur d�clar�e et la valeur actuelle d'',un attribut fixe ne correspondent pas';
      ET_ATTRIBUTE_DEFAULT_TYPE_MISMATCH:
        ErrorStr :=
          'La valeur par d�faut d'';un attribut ne correspond pas � son type';
      ET_DUPLICATE_ID_VALUE:
        ErrorStr := 'valeur ID dupliqu�e';
      ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC';
      ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC';
      ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC:
        ErrorStr := 'ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC';
      ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC:
        ErrorStr := 'ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC';
      ET_TARGET_ID_VALUE_NOT_FOUND:
        ErrorStr :=
          'La valeur IDREF ou IDREFS pointe sur une valeur ID inexistante';
      ET_UNDEFINED_TARGET_UNPARSED_ENTITY:
        ErrorStr :=
          'La valeur ENTITY ou ENTITIES pointe sur une entit�e non pars�e inexistante';
      ET_PREDEFINED_ENTITY_INCORRECTLY_REDECLARED:
        ErrorStr := 'Mauvaise d�claration d'';entit�e pr�d�finie';
      ET_UNRESOLVABLE_EXTERNAL_SUBSET:
        ErrorStr := 'Sous ensemble externe de la DTD non trouv�e';
      ET_UNDEFINED_PARAMETER_ENTITY:
        ErrorStr := 'D�claration entit� param�tre non trouv�e';
      ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION';
      ET_PE_NOT_PROPERLY_NESTED_WITH_DECL:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_DECL';
      ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP';
    else
      ErrorStr := 'Code source invalide';
    end; {case ...}
  end; {with ...}

  if PosStr <> '' then
    S := Format('[%s] %s(%s): %s', [SeverityStr, FileNameStr, PosStr, ErrorStr])
  else
    S := Format('[%s] %s: %s', [SeverityStr, FileNameStr, ErrorStr]);

  Result := EncodingToUTF16(TISO8859_1Codec, S);
end;

{ TDomGermanErrorTranslator }

constructor TDomGermanErrorTranslator.Create(AOwner: TComponent);
begin
  inherited;
  FLanguage := iso639_de;
end;

function TDomGermanErrorTranslator.GetSupportedLanguages:
  TIso639LanguageCodeSet;
begin
  Result := [iso639_de];
end;

function TDomGermanErrorTranslator.Translate(
  const Error: TdomError): WideString;
var
  ErrorStr, FileNameStr, PosStr, SeverityStr, S: string;
begin
  with Error do
  begin
    case Severity of
      DOM_SEVERITY_FATAL_ERROR: SeverityStr := 'Kritischer Fehler';
      DOM_SEVERITY_ERROR: SeverityStr := 'Fehler';
      DOM_SEVERITY_WARNING: SeverityStr := 'Warnung';
    end;

    FileNameStr := ExtractFileNameFromUri(Uri);
    if EndLineNumber < 1 then
      PosStr := ''
    else if StartLineNumber = EndLineNumber then
    begin
      if StartColumnNumber = EndColumnNumber then
        FmtStr(PosStr, '%d:%d', [EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1) ])
      else
        FmtStr(PosStr, '%d:%d-%d', [EndLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
          EndColumnNumber]);
    end
    else
    begin
      FmtStr(PosStr, '%d:%d-%d:%d', [StartLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
        EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1)]);
    end;

    case RelatedException of
      ET_ATTLIST_DECL_START_EXPECTED:
        ErrorStr := 'Beginn einer Attribute-List-Deklaration erwartet';
      ET_BYTE_ORDER_MARK_ENCODING_MISMATCH:
        ErrorStr := 'Byte-Reihenfolgen-Markierung pa�t nicht zur deklarierten Zeichencodierung';
      ET_CDATA_START_EXPECTED:
        ErrorStr := 'Beginn eines CDATA-Abschnitts erwartet';
      ET_COMMENT_START_EXPECTED:
        ErrorStr := 'Beginn eines Kommentars erwartet';
      ET_CONDITIONAL_SECTION_NOT_ALLOWED:
        ErrorStr :=
          'Bedingter Abschnitt nicht erlaubt im internen Teil einer DTD';
      ET_DOCTYPE_START_EXPECTED:
        ErrorStr := 'Beginn einer Dokument-Typ-Deklaration (DTD) erwartet';
      ET_ELEMENT_DECL_START_EXPECTED:
        ErrorStr := 'Beginn einer Element-Deklaration erwartet';
      ET_ENTITY_DECL_START_EXPECTED:
        ErrorStr := 'Beginn einer Entit�t-Deklaration erwartet';
      ET_INVALID_ELEMENT_NAME:
        ErrorStr := 'Ung�ltiger Element-Name';
      ET_DOUBLE_HYPHEN_IN_COMMENT:
        ErrorStr := 'Doppelter Bindestrich in einem Kommentar';
      ET_DOUBLE_ROOT_ELEMENT:
        ErrorStr := 'Doppeltes Wurzel-Element';
      ET_DOUBLE_DOCTYPE:
        ErrorStr := 'Doppelte Dokument-Typ-Deklaration (DTD)';
      ET_HYPHEN_AT_COMMENT_END:
        ErrorStr := 'Kommentartext endet mit einem Bindestrich';
      ET_INVALID_ATTRIBUTE_NAME:
        ErrorStr := 'Ung�ltiger Attribut-Name';
      ET_INVALID_ATTRIBUTE_VALUE:
        ErrorStr := 'Ung�ltiger Attribut-Wert';
      ET_DOUBLE_ATTRIBUTE_NAME:
        ErrorStr := 'Doppelter Attribut-Name in einem Element';
      ET_INVALID_ENCODING_NAME:
        ErrorStr := 'Ung�ltiger Zeichencodierungsname';
      ET_INVALID_ENTITY_NAME:
        ErrorStr := 'Ung�ltiger Entit�ten-Name';
      ET_INVALID_PARAMETER_ENTITY_NAME:
        ErrorStr := 'Ung�ltiger Parameter-Entit�ten-Name';
      ET_INVALID_PROCESSING_INSTRUCTION:
        ErrorStr := 'Ung�ltige Verarbeitungsanweisung';
      ET_INVALID_VERSION_NUMBER:
        ErrorStr := 'Ung�ltige Versionszahl';
      ET_INVALID_XML_DECL:
        ErrorStr := 'Ung�ltige XML-Deklaration';
      ET_INVALID_CHAR_REF:
        ErrorStr := 'Ung�ltiger Zeichenverweis';
      ET_MISSING_QUOTATION_MARK:
        ErrorStr := 'Fehlende Anf�hrungszeichen';
      ET_MISSING_EQUALITY_SIGN:
        ErrorStr := 'Fehlendes Gleichheitszeichen';
      ET_DOUBLE_EQUALITY_SIGN:
        ErrorStr := 'Doppeltes Gleichheitszeichen';
      ET_MISSING_WHITE_SPACE:
        ErrorStr := 'Fehlender Leerraum';
      ET_MISSING_START_TAG:
        ErrorStr := 'End-Tag ohne Start-Tag';
      ET_MISSING_ELEMENT_NAME:
        ErrorStr := 'Fehlender Elementname';
      ET_MISSING_ENTITY_NAME:
        ErrorStr := 'Fehlender Entit�tenname';
      ET_MISSING_END_TAG:
        ErrorStr := 'Fehlendes End-Tag';
      ET_INVALID_CHARACTER:
        ErrorStr := 'Ung�ltiges Zeichen';
      ET_NOT_IN_ROOT_ELEMENT:
        ErrorStr := 'Zeichen au�erhalb des Wurzel-Elements';
      ET_ROOT_ELEMENT_NOT_FOUND:
        ErrorStr := 'Wurzel-Element nicht gefunden';
      ET_KEYWORD_ANY_NOT_ALLOWED:
        ErrorStr := 'Schl�sselwort "ANY" ist hier nicht erlaubt';
      ET_KEYWORD_EMPTY_NOT_ALLOWED:
        ErrorStr := 'Schl�sselwort "EMPTY" ist hier nicht erlaubt';
      ET_KEYWORD_PCDATA_NOT_ALLOWED:
        ErrorStr := 'Schl�sselwort "#PCDATA" ist hier nicht erlaubt';
      ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL:
        ErrorStr := 'Inhaltsmodell einer Element-Deklaration nicht wohlgeformt';
      ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'Fehlender Entit�ten-Wert in Entit�ten-Deklaration';
      ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Fehlender Entit�ten-Wert in Parameter-Entit�ten-Deklaration';
      ET_NOTATION_DECL_START_EXPECTED:
        ErrorStr := 'Beginn einer Notationsdeklaration erwartet';
      ET_PARAMETER_ENTITY_REF_NOT_ALLOWED:
        ErrorStr := 'Parameter-Entit�ten-Verweis hier nicht erlaubt';
      ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL:
        ErrorStr := 'Ung�ltiges Token im Inhaltsmodell einer Element-Deklaration';
      ET_UNCLOSED_ATTLIST_DECL:
        ErrorStr := 'Attributlisten-Deklaration nicht geschlossen';
      ET_SYSTEM_KEYWORD_EXPECTED:
        ErrorStr := 'Schl�sselwort "SYSTEM" erwartet';
      ET_UNCLOSED_CDATA_SECTION:
        ErrorStr := 'CDATA-Abschnitt nicht geschlossen';
      ET_UNCLOSED_CHAR_REF:
        ErrorStr := 'Zeichenverweis nicht geschlossen';
      ET_UNCLOSED_COMMENT:
        ErrorStr := 'Kommentar nicht geschlossen';
      ET_UNCLOSED_CONDITIONAL_SECTION:
        ErrorStr := 'Bedingter Abschnitt nicht geschlossen';
      ET_UNCLOSED_DOCTYPE:
        ErrorStr := 'Dokument-Typ-Deklaration (DTD) nicht abgeschlossen';
      ET_UNCLOSED_ELEMENT:
        ErrorStr := 'Element nicht geschlossen';
      ET_UNCLOSED_ELEMENT_DECL:
        ErrorStr := 'Element-Deklaration nicht geschlossen';
      ET_UNCLOSED_ENTITY_DECL:
        ErrorStr := 'Entit�ten-Deklaration nicht geschlossen';
      ET_UNCLOSED_ENTITY_REF:
        ErrorStr := 'Entit�ten-Verweis nicht geschlossen';
      ET_UNCLOSED_NOTATION_DECL:
        ErrorStr := 'Notationsdeklaration nicht geschlossen';
      ET_UNCLOSED_PARAMETER_ENTITY_REF:
        ErrorStr := 'Parameter-Entit�ten-Verweis nicht geschlossen';
      ET_UNCLOSED_PROCESSING_INSTRUCTION:
        ErrorStr := 'Verarbeitungsanweisung nicht geschlossen';
      ET_WRONG_ORDER:
        ErrorStr := 'Falsche Reihenfolge';
      ET_UNKNOWN_DECL_TYPE:
        ErrorStr := 'Unbekannter Deklarationstyp';
      ET_INVALID_ATTRIBUTE_DECL:
        ErrorStr := 'Ung�ltige Attribut-Deklaration';
      ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'Ung�ltiger Attribut-Name in Attribut-Deklaration';
      ET_INVALID_ATTLIST_DECL_NAME:
        ErrorStr := 'Name einer Attribut-Deklaration ung�ltig';
      ET_INVALID_CONDITIONAL_SECTION:
        ErrorStr := 'Ung�ltiger bedingter Abschnitt';
      ET_INVALID_ELEMENT_DECL:
        ErrorStr := 'Ung�ltige Element-Deklaration';
      ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'Ung�ltiger Element-Name in Attribut-Deklaration';
      ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL:
        ErrorStr := 'Ung�ltiger Element-Name in Element-Deklaration';
      ET_INVALID_ENTITY_DECL:
        ErrorStr := 'Ung�ltige Entit�t-Deklaration';
      ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL:
        ErrorStr := 'Ung�ltiger Entit�ten-Name in Entit�ten-Deklaration';
      ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Ung�ltiger Entit�ten-Name in Parameter-Entit�ten-Deklaration';
      ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'Ung�ltiger Entit�ten-Wert in Entit�ten-Deklaration';
      ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Ung�ltiger Entit�ten-Wert in Parameter-Entit�ten-Deklaration';
      ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'Ung�ltiger Aufz�hlungstoken in Attribut-Deklaration';
      ET_INVALID_MARKUP_DECL:
        ErrorStr := 'Ung�ltige Markup-Deklaration';
      ET_INVALID_NOTATION_DECL:
        ErrorStr := 'Ung�ltige Notations-Deklaration';
      ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL:
        ErrorStr := 'Ung�ltiger Notations-Name in Entit�ten-Deklaration';
      ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL:
        ErrorStr := 'Ung�ltiger Notations-Name in Notations-Deklaration';
      ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'Ung�ltiger Notationstoken in Attribut-Deklaration';
      ET_INVALID_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Ung�ltige Parameter-Entit�ten-Deklaration';
      ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL:
        ErrorStr := 'Ung�ltiger Name f�r das Wurzel-Element einer Dokument-Typ-Deklaration (DTD)';
      ET_INVALID_TEXT_DECL:
        ErrorStr := 'Ung�ltige Text-Deklaration';
      ET_LEFT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := '"[" erwartet';
      ET_LT_IN_ATTRIBUTE_VALUE:
        ErrorStr :=
          '"<" im Ersetzungstext einer Entit�t, die direkt oder indirekt in einem Attributwertes referenziert wird.';
      ET_ATTRIBUTE_VALUE_REFERS_TO_EXTERNAL_ENTITY:
        ErrorStr := 'Attribut-Wert verweist auf eine externe Entit�t';
      ET_PE_BETWEEN_DECLARATIONS:
        ErrorStr := 'Parameter-Entit�ten-Verweis zwischen Deklarationen erzeugt unvollst�ndige Markup-Deklaration';
      ET_PUBLIC_KEYWORD_EXPECTED:
        ErrorStr := 'Schl�sselwort "PUBLIC" erwartet';
      ET_QUOTATION_MARK_EXPECTED:
        ErrorStr := 'Anf�hrungszeichen erwartet';
      ET_RECURSIVE_REFERENCE:
        ErrorStr := 'Rekursiver Verweis gefunden';
      ET_REFERS_TO_UNPARSED_ENTITY:
        ErrorStr := 'Verweis auf eine nicht analysierbare Entit�t';
      ET_RIGHT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := '"]" erwartet';
      ET_NO_PROPER_MARKUP_REFERENCED:
        ErrorStr :=
          'Verweis auf eine Entit�t mit ung�ltigen Markierungszeichen';
      ET_INVALID_COMMENT:
        ErrorStr := 'Ung�ltiger Kommentar';
      ET_INVALID_CDATA_SECTION:
        ErrorStr := 'Ung�ltiger CDATA-Abschnitt';
      ET_INVALID_SYSTEM_LITERAL:
        ErrorStr := 'Ung�ltiges System-Literal';
      ET_INVALID_PUBID_LITERAL:
        ErrorStr := 'Ung�ltiges Pubid-Literal';
      ET_INVALID_QUALIFIED_NAME:
        ErrorStr := 'Ung�ltiger qualifizierter Name';
      ET_INVALID_PREFIX:
        ErrorStr := 'Ung�ltiges Prefix';
      ET_INVALID_NAMESPACE_URI:
        ErrorStr := 'Ung�ltiger Namensraum-URI';
      ET_NAMESPACE_URI_NOT_FOUND:
        ErrorStr := 'Namensraum-URI nicht gefunden';
      ET_WRONG_PREFIX_MAPPING_NESTING:
        ErrorStr := 'Falsche Prefix-Verschachtelung';
      ET_XML_VERSION_NOT_SUPPORTED:
        ErrorStr := 'Nicht unterst�tzte XML-Version';
      ET_ENCODING_NOT_SUPPORTED:
        ErrorStr := 'Nicht unterst�tzte Zeichencodierung';
      ET_INVALID_CHARACTER_IN_EXT_ENTITY:
        ErrorStr := 'Ung�ltiges Zeichen in einer externen analysierten Entit�t';
      ET_EXT_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'Ressource einer externen Entit�t nicht gefunden';
      ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'Ressource einer externen Parameter-Entit�t nicht gefunden';
      ET_DOUBLE_ATTLISTDECL:
        ErrorStr := 'Doppelte Attributlisten-Deklaration';
      ET_DOUBLE_ATTDEF:
        ErrorStr := 'Doppelte Attribut-Defintion';
      ET_DOUBLE_ENTITY_DECL:
        ErrorStr := 'Doppelte Entit�t-Deklaration';
      ET_DOUBLE_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Doppelte Parameter-Entit�t-Deklaration';
      ET_UNDEFINED_ENTITY_VC:
        ErrorStr := 'Entit�t nicht definiert';
      ET_UNDEFINED_ENTITY_WFC:
        ErrorStr := 'Entit�t nicht definiert';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC:
        ErrorStr := 'Verweis auf eine Entit�t vor deren Definition';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC:
        ErrorStr := 'Verweis auf eine Entit�t vor deren Definition';
      ET_DOCTYPE_NOT_FOUND:
        ErrorStr := 'Dokument-Typ-Deklaration (DTD) fehlt';
      ET_DUPLICATE_ELEMENT_TYPE_DECL:
        ErrorStr := 'Doppelte Element-Typ-Deklaration';
      ET_DUPLICATE_NAME_IN_MIXED_CONTENT:
        ErrorStr := 'Doppelter Name in gemischtem Inhalt';
      ET_DUPLICATE_ID_ON_ELEMENT_TYPE:
        ErrorStr := 'ID-Attribut mehrfach f�r denselben Element-Typ deklariert';
      ET_UNDEFINED_NOTATION:
        ErrorStr := 'Notation nicht definiert';
      ET_DUPLICATE_NOTATION_ON_ELEMENT_TYPE:
        ErrorStr :=
          'Notationsattribut mehrfach f�r denselben Element-Typ deklariert';
      ET_DUPLICATE_NOTATION_TOKEN:
        ErrorStr := 'Doppelte Notationstokens';
      ET_NOTATION_ON_EMPTY_ELEMENT:
        ErrorStr := 'Notationsattribut f�r einen leeren Element-Typ deklariert';
      ET_DUPLICATE_ENUMERATION_TOKEN:
        ErrorStr := 'Doppeltes Aufz�hlungstoken';
      ET_ATTRIBUTE_TYPE_MISMATCH:
        ErrorStr := 'Attribut-Typ und Attribut-Wert stimmen nicht �berein';
      ET_DUPLICATE_TOKENS:
        ErrorStr := 'Token mehrfach deklariert';
      ET_ID_NEITHER_IMPLIED_NOR_REQUIRED:
        ErrorStr :=
          'ID-Attribut ist weder als #IMPLIED noch als #REQUIRED deklariert';
      ET_WRONG_ROOT_ELEMENT_TYPE:
        ErrorStr :=
          'Typ des Wurzelelements entspricht nicht der Angabe in der Dokumenttyp-Deklaration';
      ET_UNDEFINED_ELEMENT_TYPE:
        ErrorStr := 'Elementtyp nicht definiert';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY:
        ErrorStr := 'Element enth�lt einen CDATA-Abschnitt, wo nur Kind-Elemente zul�ssig sind';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE:
        ErrorStr := 'Element enth�lt ein anderes Element, das nicht dem Inhaltsmodell entspricht';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY:
        ErrorStr := 'Element enth�lt ein anderes Element, wo nur PCDATA (Text) zul�ssig ist';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY:
        ErrorStr := 'Element enth�lt PCDATA (Text), wo nur Kind-Elemente zul�ssig sind';
      ET_ELEMENT_DECLARED_EMPTY_HAS_CONTENT:
        ErrorStr := 'Als EMPTY deklariertes Element ist nicht leer';
      ET_NONDETERMINISTIC_ELEMENT_CONTENT_MODEL:
        ErrorStr := 'Inhaltsmodell eines Elements ist nicht deterministisch';
      ET_DUPLICATE_NOTATION_DECL:
        ErrorStr := 'Notation mehrfach deklariert';
      ET_UNDEFINED_ATTRIBUTE:
        ErrorStr := 'Attribut nicht definiert';
      ET_REQUIRED_ATTRIBUTE_NOT_SPECIFIED:
        ErrorStr := 'Als "#REQUIRED" deklariertes Attribut fehlt';
      ET_FIXED_ATTRIBUTE_MISMATCH:
        ErrorStr :=
          'Werte eines als "#FIXED" deklarierten Attributs entspricht nicht der Deklaration';
      ET_ATTRIBUTE_DEFAULT_TYPE_MISMATCH:
        ErrorStr := 'Attribut-Typ und Attribut-Vorgabe stimmen nicht �berein';
      ET_DUPLICATE_ID_VALUE:
        ErrorStr := 'ID-Wert mehrfach verwendet';
      ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'Extern deklariertes Attribut in einem "Standalone"-Dokument nicht normalisiert';
      ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'Extern deklariertes Vorgabe-Attribut in einem "Standalone"-Dokument fehlt';
      ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC:
        ErrorStr := 'Extern deklarierte Entit�ten-Verweis in einem "Standalone"-Dokument';
      ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC:
        ErrorStr := 'Leerraum im Inhalt eines extern deklarierten Elements eines "Standalone"-Dokuments';
      ET_TARGET_ID_VALUE_NOT_FOUND:
        ErrorStr :=
          'Ein IDREF- bzw. IDREFS-Attribute verweist auf ein nicht vorhandenes Ziel';
      ET_UNDEFINED_TARGET_UNPARSED_ENTITY:
        ErrorStr :=
          'Ein ENTITY- bzw. ENTITIES-Attribute verweist auf eine nicht analysierbare Entit�t, die nicht definiert wurde';
      ET_PREDEFINED_ENTITY_INCORRECTLY_REDECLARED:
        ErrorStr := 'Vordefinierte Entit�t falsch erneut deklariert';
      ET_UNRESOLVABLE_EXTERNAL_SUBSET:
        ErrorStr := 'Externe Teilmenge der DTD nicht gefunden';
      ET_UNDEFINED_PARAMETER_ENTITY:
        ErrorStr := 'Parameter-Entit�t nicht definiert';
      ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION:
        ErrorStr := 'Ersetzungstext einer Parameter-Entit�t nicht sauber mit bedingtem Abschnitt verschachtelt';
      ET_PE_NOT_PROPERLY_NESTED_WITH_DECL:
        ErrorStr := 'Ersetzungstext einer Parameter-Entit�t nicht sauber mit Markup-Deklarationen verschachtelt';
      ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP:
        ErrorStr := 'Ersetzungstext einer Parameter-Entit�t nicht sauber mit Klammerausdr�cken verschachtelt';
    else
      ErrorStr := 'Ung�ltiger Quellcode';
    end; {case ...}
  end; {with ...}

  if PosStr <> '' then
    S := Format('[%s] %s(%s): %s', [SeverityStr, FileNameStr, PosStr, ErrorStr])
  else
    S := Format('[%s] %s: %s', [SeverityStr, FileNameStr, ErrorStr]);

  Result := EncodingToUTF16(TISO8859_1Codec, S);
end;

{ TDomItalianErrorTranslator }

constructor TDomItalianErrorTranslator.Create(AOwner: TComponent);
begin
  inherited;
  FLanguage := iso639_it;
end;

function TDomItalianErrorTranslator.GetSupportedLanguages:
  TIso639LanguageCodeSet;
begin
  Result := [iso639_it];
end;

function TDomItalianErrorTranslator.Translate(
  const Error: TdomError): WideString;
// - This function was provided by Massimo Maria Ghisalberti -
var
  ErrorStr, FileNameStr, PosStr, SeverityStr, S: string;
begin
  with Error do
  begin
    case Severity of
      DOM_SEVERITY_FATAL_ERROR: SeverityStr := 'Errore Critico';
      DOM_SEVERITY_ERROR: SeverityStr := 'Errore';
      DOM_SEVERITY_WARNING: SeverityStr := 'Attenzione';
    end;

    FileNameStr := ExtractFileNameFromUri(Uri);
    if EndLineNumber < 1 then
      PosStr := ''
    else if StartLineNumber = EndLineNumber then
    begin
      if StartColumnNumber = EndColumnNumber then
        FmtStr(PosStr, '%d:%d', [EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1) ])
      else
        FmtStr(PosStr, '%d:%d-%d', [EndLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
          EndColumnNumber]);
    end
    else
    begin
      FmtStr(PosStr, '%d:%d-%d:%d', [StartLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
        EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1)]);
    end;

    case RelatedException of
      ET_ATTLIST_DECL_START_EXPECTED:
        ErrorStr := 'ET_ATTLIST_DECL_START_EXPECTED';
      ET_BYTE_ORDER_MARK_ENCODING_MISMATCH:
        ErrorStr := 'ET_BYTE_ORDER_MARK_ENCODING_MISMATCH';
      ET_CDATA_START_EXPECTED:
        ErrorStr := 'ET_CDATA_START_EXPECTED';
      ET_COMMENT_START_EXPECTED:
        ErrorStr := 'ET_COMMENT_START_EXPECTED';
      ET_CONDITIONAL_SECTION_NOT_ALLOWED:
        ErrorStr := 'ET_CONDITIONAL_SECTION_NOT_ALLOWED';
      ET_DOCTYPE_START_EXPECTED:
        ErrorStr := 'ET_DOCTYPE_START_EXPECTED';
      ET_ELEMENT_DECL_START_EXPECTED:
        ErrorStr := 'ET_ELEMENT_DECL_START_EXPECTED';
      ET_ENTITY_DECL_START_EXPECTED:
        ErrorStr := 'ET_ENTITY_DECL_START_EXPECTED';
      ET_INVALID_ELEMENT_NAME:
        ErrorStr := 'Nome elemento non valido';
      ET_DOUBLE_HYPHEN_IN_COMMENT:
        ErrorStr := 'ET_DOUBLE_HYPHEN_IN_COMMENT';
      ET_DOUBLE_ROOT_ELEMENT:
        ErrorStr := 'Elemento radice duplicato';
      ET_DOUBLE_DOCTYPE:
        ErrorStr := 'Dichiarazione del tipo di documento (DTD) duplicata';
      ET_HYPHEN_AT_COMMENT_END:
        ErrorStr := 'ET_HYPHEN_AT_COMMENT_END';
      ET_INVALID_ATTRIBUTE_NAME:
        ErrorStr := 'Nome attributo non valido';
      ET_INVALID_ATTRIBUTE_VALUE:
        ErrorStr := 'Valore attributo non valido';
      ET_DOUBLE_ATTRIBUTE_NAME:
        ErrorStr := 'Nome dell''attributo duplicato in un elemento';
      ET_INVALID_ENCODING_NAME:
        ErrorStr := 'ET_INVALID_ENCODING_NAME';
      ET_INVALID_ENTITY_NAME:
        ErrorStr := 'Nome entit� non valido';
      ET_INVALID_PARAMETER_ENTITY_NAME:
        ErrorStr := 'Nome entit� con parametri non valido';
      ET_INVALID_PROCESSING_INSTRUCTION:
        ErrorStr := 'Istruzione di processo non valida';
      ET_INVALID_VERSION_NUMBER:
        ErrorStr := 'ET_INVALID_VERSION_NUMBER';
      ET_INVALID_XML_DECL:
        ErrorStr := 'Dichiarazione XML non valida';
      ET_INVALID_CHAR_REF:
        ErrorStr := 'Riferimento a carattere non valida';
      ET_MISSING_QUOTATION_MARK:
        ErrorStr := 'Virgolette mancanti';
      ET_MISSING_EQUALITY_SIGN:
        ErrorStr := 'Segno di uguale mancante';
      ET_DOUBLE_EQUALITY_SIGN:
        ErrorStr := 'Segno di uguale duplicato';
      ET_MISSING_WHITE_SPACE:
        ErrorStr := 'Spazio mancante';
      ET_MISSING_START_TAG:
        ErrorStr := 'Tag finale senza tag iniziale';
      ET_MISSING_ELEMENT_NAME:
        ErrorStr := 'ET_MISSING_ELEMENT_NAME';
      ET_MISSING_ENTITY_NAME:
        ErrorStr := 'ET_MISSING_ENTITY_NAME';
      ET_MISSING_END_TAG:
        ErrorStr := 'Tag finale mancante';
      ET_INVALID_CHARACTER:
        ErrorStr := 'Carattere non valido';
      ET_NOT_IN_ROOT_ELEMENT:
        ErrorStr := 'Carattere(i) oltre l''elemento radice';
      ET_ROOT_ELEMENT_NOT_FOUND:
        ErrorStr := 'Elemento radice non trovato';
      ET_KEYWORD_ANY_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_ANY_NOT_ALLOWED';
      ET_KEYWORD_EMPTY_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_EMPTY_NOT_ALLOWED';
      ET_KEYWORD_PCDATA_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_PCDATA_NOT_ALLOWED';
      ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL:
        ErrorStr := 'ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL';
      ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL';
      ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL';
      ET_NOTATION_DECL_START_EXPECTED:
        ErrorStr := 'ET_NOTATION_DECL_START_EXPECTED';
      ET_PARAMETER_ENTITY_REF_NOT_ALLOWED:
        ErrorStr := 'ET_PARAMETER_ENTITY_REF_NOT_ALLOWED';
      ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL:
        ErrorStr := 'ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL';
      ET_UNCLOSED_ATTLIST_DECL:
        ErrorStr := 'ET_UNCLOSED_ATTLIST_DECL';
      ET_SYSTEM_KEYWORD_EXPECTED:
        ErrorStr := 'ET_SYSTEM_KEYWORD_EXPECTED';
      ET_UNCLOSED_CDATA_SECTION:
        ErrorStr := 'ET_UNCLOSED_CDATA_SECTION';
      ET_UNCLOSED_CHAR_REF:
        ErrorStr := 'ET_UNCLOSED_CHAR_REF';
      ET_UNCLOSED_COMMENT:
        ErrorStr := 'ET_UNCLOSED_COMMENT';
      ET_UNCLOSED_CONDITIONAL_SECTION:
        ErrorStr := 'ET_UNCLOSED_CONDITIONAL_SECTION';
      ET_UNCLOSED_DOCTYPE:
        ErrorStr := 'ET_UNCLOSED_DOCTYPE';
      ET_UNCLOSED_ELEMENT:
        ErrorStr := 'ET_UNCLOSED_ELEMENT';
      ET_UNCLOSED_ELEMENT_DECL:
        ErrorStr := 'ET_UNCLOSED_ELEMENT_DECL';
      ET_UNCLOSED_ENTITY_DECL:
        ErrorStr := 'ET_UNCLOSED_ENTITY_DECL';
      ET_UNCLOSED_ENTITY_REF:
        ErrorStr := 'ET_UNCLOSED_ENTITY_REF';
      ET_UNCLOSED_NOTATION_DECL:
        ErrorStr := 'ET_UNCLOSED_NOTATION_DECL';
      ET_UNCLOSED_PARAMETER_ENTITY_REF:
        ErrorStr := 'ET_UNCLOSED_PARAMETER_ENTITY_REF';
      ET_UNCLOSED_PROCESSING_INSTRUCTION:
        ErrorStr := 'ET_UNCLOSED_PROCESSING_INSTRUCTION';
      ET_WRONG_ORDER:
        ErrorStr := 'Ordine errato';
      ET_UNKNOWN_DECL_TYPE:
        ErrorStr := 'Dichiarazione di tipo sconosciuta';
      ET_INVALID_ATTRIBUTE_DECL:
        ErrorStr := 'Dichiarazione di attributo non valida';
      ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL';
      ET_INVALID_ATTLIST_DECL_NAME:
        ErrorStr := 'ET_INVALID_ATTLIST_DECL_NAME';
      ET_INVALID_CONDITIONAL_SECTION:
        ErrorStr := 'Sezione condizionale non valida';
      ET_INVALID_ELEMENT_DECL:
        ErrorStr := 'Dichiarazione di elemento non valida';
      ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL';
      ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL:
        ErrorStr := 'ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL';
      ET_INVALID_ENTITY_DECL:
        ErrorStr := 'Dichiarazione di entit� non valida';
      ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL';
      ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL';
      ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL';
      ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL';
      ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL';
      ET_INVALID_MARKUP_DECL:
        ErrorStr := 'Dichiarazione di markup non valida';
      ET_INVALID_NOTATION_DECL:
        ErrorStr := 'Dichiarazione di notazione non valida';
      ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL';
      ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL';
      ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL';
      ET_INVALID_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Dichiarazione di entit� con parametri non valida';
      ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL:
        ErrorStr := 'ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL';
      ET_INVALID_TEXT_DECL:
        ErrorStr := 'Dichiarazione di testo non valida';
      ET_LEFT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := 'ET_LEFT_SQUARE_BRACKET_EXPECTED';
      ET_LT_IN_ATTRIBUTE_VALUE:
        ErrorStr :=
          '"<" nel testo di sostituzione di una entit� riferita direttamente o indirettamente al valore di un attributo';
      ET_ATTRIBUTE_VALUE_REFERS_TO_EXTERNAL_ENTITY:
        ErrorStr :=
          'Il valore dell''attributo si riferisce ad un''entit� esterna';
      ET_PE_BETWEEN_DECLARATIONS:
        ErrorStr := 'ET_PE_BETWEEN_DECLARATIONS';
      ET_PUBLIC_KEYWORD_EXPECTED:
        ErrorStr := 'ET_PUBLIC_KEYWORD_EXPECTED';
      ET_QUOTATION_MARK_EXPECTED:
        ErrorStr := 'ET_QUOTATION_MARK_EXPECTED';
      ET_RECURSIVE_REFERENCE:
        ErrorStr := 'Trovato riferimento ricorsivo';
      ET_REFERS_TO_UNPARSED_ENTITY:
        ErrorStr := 'Riferimento ad una entit� non analizzata';
      ET_RIGHT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := 'ET_RIGHT_SQUARE_BRACKET_EXPECTED';
      ET_NO_PROPER_MARKUP_REFERENCED:
        ErrorStr := 'Riferimento ad una entit� senza un marcatore adeguato';
      ET_INVALID_COMMENT:
        ErrorStr := 'Commento non valido';
      ET_INVALID_CDATA_SECTION:
        ErrorStr := 'Sezione CDATA non valida';
      ET_INVALID_SYSTEM_LITERAL:
        ErrorStr := 'Stringa di sistema non valida';
      ET_INVALID_PUBID_LITERAL:
        ErrorStr := 'Stringa PUBID non valida';
      ET_INVALID_QUALIFIED_NAME:
        ErrorStr := 'Nome qualificano non valido';
      ET_INVALID_PREFIX:
        ErrorStr := 'Prefisso non valido';
      ET_INVALID_NAMESPACE_URI:
        ErrorStr := 'namespace URI non valido';
      ET_NAMESPACE_URI_NOT_FOUND:
        ErrorStr := 'namespace URI non trovato';
      ET_WRONG_PREFIX_MAPPING_NESTING:
        ErrorStr := 'Incapsulamento errato nel mapping del presisso';
      ET_XML_VERSION_NOT_SUPPORTED:
        ErrorStr := 'ET_XML_VERSION_NOT_SUPPORTED';
      ET_ENCODING_NOT_SUPPORTED:
        ErrorStr := 'Codifica non supportata';
      ET_INVALID_CHARACTER_IN_EXT_ENTITY:
        ErrorStr := 'ET_INVALID_CHARACTER_IN_EXT_ENTITY';
      ET_EXT_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'ET_EXT_ENTITY_RESOURCE_NOT_FOUND';
      ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND';
      ET_DOUBLE_ATTLISTDECL:
        ErrorStr := 'ET_DOUBLE_ATTLISTDECL';
      ET_DOUBLE_ATTDEF:
        ErrorStr := 'Definizione di attibuto duplicata';
      ET_DOUBLE_ENTITY_DECL:
        ErrorStr := 'Dichiarazione di entit� duplicata';
      ET_DOUBLE_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Dichiarazione di entit� con parametri duplicati';
      ET_UNDEFINED_ENTITY_VC:
        ErrorStr := 'Dichiarazione di entit� non trovata';
      ET_UNDEFINED_ENTITY_WFC:
        ErrorStr := 'Dichiarazione di entit� non trovata';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC:
        ErrorStr := 'ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC:
        ErrorStr := 'ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC';
      ET_DOCTYPE_NOT_FOUND:
        ErrorStr := 'ET_DOCTYPE_NOT_FOUND';
      ET_DUPLICATE_ELEMENT_TYPE_DECL:
        ErrorStr := 'Dichiarazione di tipo dell''elemento duplicata';
      ET_DUPLICATE_NAME_IN_MIXED_CONTENT:
        ErrorStr := 'Nome duplicato in un contenuto misto';
      ET_DUPLICATE_ID_ON_ELEMENT_TYPE:
        ErrorStr :=
          'Attributo ID duplicato nella stessa dichiarazione di tipo di elemento';
      ET_UNDEFINED_NOTATION:
        ErrorStr := 'Nome di notazione non dichiarato';
      ET_DUPLICATE_NOTATION_ON_ELEMENT_TYPE:
        ErrorStr :=
          'Attributo di notazione duplicato nello stessa dichiarazione di tipo di elemento';
      ET_DUPLICATE_NOTATION_TOKEN:
        ErrorStr := 'Notazione del token duplicata';
      ET_NOTATION_ON_EMPTY_ELEMENT:
        ErrorStr :=
          'Attributo di notazione nella dichiarazione di tipo di elemento vuoto';
      ET_DUPLICATE_ENUMERATION_TOKEN:
        ErrorStr := 'Enumerazione del token duplicata';
      ET_ATTRIBUTE_TYPE_MISMATCH:
        ErrorStr := 'L''attributo di tipo e il valore non coincidono';
      ET_DUPLICATE_TOKENS:
        ErrorStr := 'Dichiarazione duplicata di token';
      ET_ID_NEITHER_IMPLIED_NOR_REQUIRED:
        ErrorStr :=
          'L''attributo ID non � stato dichiarato come #IMPLIED n� come #REQUIRED';
      ET_WRONG_ROOT_ELEMENT_TYPE:
        ErrorStr :=
          'Il tipo dell''elemento radice non coincide con il nome della dichiarazione di tipo del documento';
      ET_UNDEFINED_ELEMENT_TYPE:
        ErrorStr := 'Dichiarazione del tipo dell''elemento non trovata';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY';
      ET_ELEMENT_DECLARED_EMPTY_HAS_CONTENT:
        ErrorStr := 'L''elemento dichiarato vuoto non lo �';
      ET_NONDETERMINISTIC_ELEMENT_CONTENT_MODEL:
        ErrorStr :=
          'Il modello di contenuto di un elemento non � deterministico';
      ET_DUPLICATE_NOTATION_DECL:
        ErrorStr := 'Dichiarazione di notazione duplicata';
      ET_UNDEFINED_ATTRIBUTE:
        ErrorStr := 'Definizione di attributo non trovata';
      ET_REQUIRED_ATTRIBUTE_NOT_SPECIFIED:
        ErrorStr := 'Attributo richiesto non trovato';
      ET_FIXED_ATTRIBUTE_MISMATCH:
        ErrorStr :=
          'Il valore dichiarato e quello attuale di un attributo fisso non coincidono';
      ET_ATTRIBUTE_DEFAULT_TYPE_MISMATCH:
        ErrorStr :=
          'Il tipo di un attributo ed il suo predefinito non coincidono';
      ET_DUPLICATE_ID_VALUE:
        ErrorStr := 'Valore ID duplicato';
      ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC';
      ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC';
      ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC:
        ErrorStr := 'ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC';
      ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC:
        ErrorStr := 'ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC';
      ET_TARGET_ID_VALUE_NOT_FOUND:
        ErrorStr :=
          'Il valore IDREF o IDREFS si riferisce ad un valore ID inesistente';
      ET_UNDEFINED_TARGET_UNPARSED_ENTITY:
        ErrorStr :=
          'Il valore ENTITY o ENTITIES si riferisce ad una entit� non analizzata non esistente';
      ET_PREDEFINED_ENTITY_INCORRECTLY_REDECLARED:
        ErrorStr := 'Dichiarazione di entit� predefinita errata';
      ET_UNRESOLVABLE_EXTERNAL_SUBSET:
        ErrorStr := 'Sottoinsieme esterno del DTD non trovato';
      ET_UNDEFINED_PARAMETER_ENTITY:
        ErrorStr := 'Dichiarazione del parametro di entit� non trovata';
      ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION';
      ET_PE_NOT_PROPERLY_NESTED_WITH_DECL:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_DECL';
      ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP';
    else
      ErrorStr := 'Codice sorgente non valido';
    end; {case ...}
  end; {with ...}

  if PosStr <> '' then
    S := Format('[%s] %s(%s): %s', [SeverityStr, FileNameStr, PosStr, ErrorStr])
  else
    S := Format('[%s] %s: %s', [SeverityStr, FileNameStr, ErrorStr]);

  Result := EncodingToUTF16(TISO8859_1Codec, S);
end;

{ TDomPolishErrorTranslator }

constructor TDomPolishErrorTranslator.Create(AOwner: TComponent);
begin
  inherited;
  FLanguage := iso639_pl;
end;

function TDomPolishErrorTranslator.GetSupportedLanguages:
  TIso639LanguageCodeSet;
begin
  Result := [iso639_pl];
end;

function TDomPolishErrorTranslator.Translate(
  const Error: TdomError): WideString;
// - This function was provided by Piotr Kuczynski -
var
  ErrorStr, FileNameStr, PosStr, SeverityStr, S: string;
begin
  with Error do
  begin
    case Severity of
      DOM_SEVERITY_FATAL_ERROR: SeverityStr := 'FATALNY B��D';
      DOM_SEVERITY_ERROR: SeverityStr := 'B��D';
      DOM_SEVERITY_WARNING: SeverityStr := 'OSTRZE�ENIE';
    end;

    FileNameStr := ExtractFileNameFromUri(Uri);
    if EndLineNumber < 1 then
      PosStr := ''
    else if StartLineNumber = EndLineNumber then
    begin
      if StartColumnNumber = EndColumnNumber then
        FmtStr(PosStr, '%d:%d', [EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1) ])
      else
        FmtStr(PosStr, '%d:%d-%d', [EndLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
          EndColumnNumber]);
    end
    else
    begin
      FmtStr(PosStr, '%d:%d-%d:%d', [StartLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
        EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1)]);
    end;

    case RelatedException of
      ET_ATTLIST_DECL_START_EXPECTED:
        ErrorStr := 'ET_ATTLIST_DECL_START_EXPECTED';
      ET_BYTE_ORDER_MARK_ENCODING_MISMATCH:
        ErrorStr := 'ET_BYTE_ORDER_MARK_ENCODING_MISMATCH';
      ET_CDATA_START_EXPECTED:
        ErrorStr := 'ET_CDATA_START_EXPECTED';
      ET_COMMENT_START_EXPECTED:
        ErrorStr := 'ET_COMMENT_START_EXPECTED';
      ET_CONDITIONAL_SECTION_NOT_ALLOWED:
        ErrorStr := 'ET_CONDITIONAL_SECTION_NOT_ALLOWED';
      ET_DOCTYPE_START_EXPECTED:
        ErrorStr := 'ET_DOCTYPE_START_EXPECTED';
      ET_ELEMENT_DECL_START_EXPECTED:
        ErrorStr := 'ET_ELEMENT_DECL_START_EXPECTED';
      ET_ENTITY_DECL_START_EXPECTED:
        ErrorStr := 'ET_ENTITY_DECL_START_EXPECTED';
      ET_INVALID_ELEMENT_NAME:
        ErrorStr := 'Niew�a�ciwa nazwa elementu';
      ET_DOUBLE_HYPHEN_IN_COMMENT:
        ErrorStr := 'ET_DOUBLE_HYPHEN_IN_COMMENT';
      ET_DOUBLE_ROOT_ELEMENT:
        ErrorStr := 'Podw�jnie zdefiniowany korze�';
      ET_DOUBLE_DOCTYPE:
        ErrorStr := 'Podw�jna deklaracja typu dokumentu (DTD)';
      ET_HYPHEN_AT_COMMENT_END:
        ErrorStr := 'ET_HYPHEN_AT_COMMENT_END';
      ET_INVALID_ATTRIBUTE_NAME:
        ErrorStr := 'Niew�a�ciwa nazwa atrybutu';
      ET_INVALID_ATTRIBUTE_VALUE:
        ErrorStr := 'Niew�a�ciwa warto�� atrybutu';
      ET_DOUBLE_ATTRIBUTE_NAME:
        ErrorStr := 'Powtarzaj�ca si� nazwa atrybutu';
      ET_INVALID_ENCODING_NAME:
        ErrorStr := 'ET_INVALID_ENCODING_NAME';
      ET_INVALID_ENTITY_NAME:
        ErrorStr := 'Niew�a�ciwa nazwa encji';
      ET_INVALID_PARAMETER_ENTITY_NAME:
        ErrorStr := 'Niew�a�ciwa nazwa encji parametrycznej';
      ET_INVALID_PROCESSING_INSTRUCTION:
        ErrorStr := 'Niew�a�ciwa instrukcja przetwarzania';
      ET_INVALID_VERSION_NUMBER:
        ErrorStr := 'ET_INVALID_VERSION_NUMBER';
      ET_INVALID_XML_DECL:
        ErrorStr := 'Niew�a�ciwa deklaracja XML';
      ET_INVALID_CHAR_REF:
        ErrorStr := 'Niew�a�ciwy znak w odwo�aniu';
      ET_MISSING_QUOTATION_MARK:
        ErrorStr := 'Brakuj�cy cudzys��w';
      ET_MISSING_EQUALITY_SIGN:
        ErrorStr := 'Brakuj�cy znak r�wno�ci';
      ET_DOUBLE_EQUALITY_SIGN:
        ErrorStr := 'Podw�jny znak r�wno�ci';
      ET_MISSING_WHITE_SPACE:
        ErrorStr := 'Brakuj�cy znak spacji';
      ET_MISSING_START_TAG:
        ErrorStr := 'Znacznik zamykaj�cy bez znacznika otwieraj�cego';
      ET_MISSING_ELEMENT_NAME:
        ErrorStr := 'ET_MISSING_ELEMENT_NAME';
      ET_MISSING_ENTITY_NAME:
        ErrorStr := 'ET_MISSING_ENTITY_NAME';
      ET_MISSING_END_TAG:
        ErrorStr := 'Brakuj�cy znacznik zamykaj�cy';
      ET_INVALID_CHARACTER:
        ErrorStr := 'Niew�a�ciwy znak';
      ET_NOT_IN_ROOT_ELEMENT:
        ErrorStr := 'Znak(i) umieszczone poza zasi�giem korzenia';
      ET_ROOT_ELEMENT_NOT_FOUND:
        ErrorStr := 'ET_ROOT_NOT_FOUND';
      ET_KEYWORD_ANY_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_ANY_NOT_ALLOWED';
      ET_KEYWORD_EMPTY_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_EMPTY_NOT_ALLOWED';
      ET_KEYWORD_PCDATA_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_PCDATA_NOT_ALLOWED';
      ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL:
        ErrorStr := 'ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL';
      ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL';
      ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL';
      ET_NOTATION_DECL_START_EXPECTED:
        ErrorStr := 'ET_NOTATION_DECL_START_EXPECTED';
      ET_PARAMETER_ENTITY_REF_NOT_ALLOWED:
        ErrorStr := 'ET_PARAMETER_ENTITY_REF_NOT_ALLOWED';
      ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL:
        ErrorStr := 'ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL';
      ET_UNCLOSED_ATTLIST_DECL:
        ErrorStr := 'ET_UNCLOSED_ATTLIST_DECL';
      ET_SYSTEM_KEYWORD_EXPECTED:
        ErrorStr := 'ET_SYSTEM_KEYWORD_EXPECTED';
      ET_UNCLOSED_CDATA_SECTION:
        ErrorStr := 'ET_UNCLOSED_CDATA_SECTION';
      ET_UNCLOSED_CHAR_REF:
        ErrorStr := 'ET_UNCLOSED_CHAR_REF';
      ET_UNCLOSED_COMMENT:
        ErrorStr := 'ET_UNCLOSED_COMMENT';
      ET_UNCLOSED_CONDITIONAL_SECTION:
        ErrorStr := 'ET_UNCLOSED_CONDITIONAL_SECTION';
      ET_UNCLOSED_DOCTYPE:
        ErrorStr := 'ET_UNCLOSED_DOCTYPE';
      ET_UNCLOSED_ELEMENT:
        ErrorStr := 'ET_UNCLOSED_ELEMENT';
      ET_UNCLOSED_ELEMENT_DECL:
        ErrorStr := 'ET_UNCLOSED_ELEMENT_DECL';
      ET_UNCLOSED_ENTITY_DECL:
        ErrorStr := 'ET_UNCLOSED_ENTITY_DECL';
      ET_UNCLOSED_ENTITY_REF:
        ErrorStr := 'ET_UNCLOSED_ENTITY_REF';
      ET_UNCLOSED_NOTATION_DECL:
        ErrorStr := 'ET_UNCLOSED_NOTATION_DECL';
      ET_UNCLOSED_PARAMETER_ENTITY_REF:
        ErrorStr := 'ET_UNCLOSED_PARAMETER_ENTITY_REF';
      ET_UNCLOSED_PROCESSING_INSTRUCTION:
        ErrorStr := 'ET_UNCLOSED_PROCESSING_INSTRUCTION';
      ET_WRONG_ORDER:
        ErrorStr := 'Niew�a�ciwa kolejno��';
      ET_UNKNOWN_DECL_TYPE:
        ErrorStr := 'Nieznany typ deklaracji';
      ET_INVALID_ATTRIBUTE_DECL:
        ErrorStr := 'Niew�a�ciwa deklaracja atrybutu';
      ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL';
      ET_INVALID_ATTLIST_DECL_NAME:
        ErrorStr := 'ET_INVALID_ATTLIST_DECL_NAME';
      ET_INVALID_CONDITIONAL_SECTION:
        ErrorStr := 'Niew�a�ciwa sekcja warunkowa';
      ET_INVALID_ELEMENT_DECL:
        ErrorStr := 'Niew�a�ciwa deklaracja elementu';
      ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL';
      ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL:
        ErrorStr := 'ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL';
      ET_INVALID_ENTITY_DECL:
        ErrorStr := 'Niew�a�ciwa deklaracja encji';
      ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL';
      ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL';
      ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL';
      ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL';
      ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL';
      ET_INVALID_MARKUP_DECL:
        ErrorStr := 'ET_INVALID_MARKUP_DECL';
      ET_INVALID_NOTATION_DECL:
        ErrorStr := 'Niew�a�ciwa deklaracja notacji';
      ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL';
      ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL';
      ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL';
      ET_INVALID_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Niew�a�ciwa deklaracja encji parametrycznej';
      ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL:
        ErrorStr := 'ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL';
      ET_INVALID_TEXT_DECL:
        ErrorStr := 'Niew�a�ciwa deklaracja tekstu';
      ET_LEFT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := 'ET_LEFT_SQUARE_BRACKET_EXPECTED';
      ET_LT_IN_ATTRIBUTE_VALUE:
        ErrorStr := 'ET_LT_IN_ATTRIBUTE_VALUE';
      ET_ATTRIBUTE_VALUE_REFERS_TO_EXTERNAL_ENTITY:
        ErrorStr := 'Warto�� atrybutu odwo�uje si� do zewn�trznej encji';
      ET_PE_BETWEEN_DECLARATIONS:
        ErrorStr := 'ET_PE_BETWEEN_DECLARATIONS';
      ET_PUBLIC_KEYWORD_EXPECTED:
        ErrorStr := 'ET_PUBLIC_KEYWORD_EXPECTED';
      ET_QUOTATION_MARK_EXPECTED:
        ErrorStr := 'ET_QUOTATION_MARK_EXPECTED';
      ET_RECURSIVE_REFERENCE:
        ErrorStr := 'ET_RECURSIVE_REFERENCE';
      ET_REFERS_TO_UNPARSED_ENTITY:
        ErrorStr := 'Odwo�anie do encji, kt�ra nie by�a jeszcze parsowana';
      ET_RIGHT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := 'ET_RIGHT_SQUARE_BRACKET_EXPECTED';
      ET_NO_PROPER_MARKUP_REFERENCED:
        ErrorStr := 'Odwo�anie do encji zawieraj�cej niew�a�ciwy znacznik';
      ET_INVALID_COMMENT:
        ErrorStr := 'ET_INVALID_COMMENT';
      ET_INVALID_CDATA_SECTION:
        ErrorStr := 'ET_INVALID_CDATA_SECTION';
      ET_INVALID_SYSTEM_LITERAL:
        ErrorStr := 'ET_INVALID_SYSTEM_LITERAL';
      ET_INVALID_PUBID_LITERAL:
        ErrorStr := 'ET_INVALID_PUBID_LITERAL';
      ET_INVALID_QUALIFIED_NAME:
        ErrorStr := 'ET_INVALID_QUALIFIED_NAME';
      ET_INVALID_PREFIX:
        ErrorStr := 'ET_INVALID_PREFIX';
      ET_INVALID_NAMESPACE_URI:
        ErrorStr := 'ET_INVALID_NAMESPACE_URI';
      ET_NAMESPACE_URI_NOT_FOUND:
        ErrorStr := 'ET_NAMESPACE_URI_NOT_FOUND';
      ET_WRONG_PREFIX_MAPPING_NESTING:
        ErrorStr := 'ET_WRONG_PREFIX_MAPPING_NESTING';
      ET_XML_VERSION_NOT_SUPPORTED:
        ErrorStr := 'ET_XML_VERSION_NOT_SUPPORTED';
      ET_ENCODING_NOT_SUPPORTED:
        ErrorStr := 'ET_ENCODING_NOT_SUPPORTED';
      ET_INVALID_CHARACTER_IN_EXT_ENTITY:
        ErrorStr := 'ET_INVALID_CHARACTER_IN_EXT_ENTITY';
      ET_EXT_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'ET_EXT_ENTITY_RESOURCE_NOT_FOUND';
      ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND';
      ET_DOUBLE_ATTLISTDECL:
        ErrorStr := 'ET_DOUBLE_ATTLISTDECL';
      ET_DOUBLE_ATTDEF:
        ErrorStr := 'Podw�jna definicja atrybutu';
      ET_DOUBLE_ENTITY_DECL:
        ErrorStr := 'Podw�jna deklaracja encji';
      ET_DOUBLE_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Podw�jna deklaracja encji parametrycznej';
      ET_UNDEFINED_ENTITY_VC:
        ErrorStr := 'Brakuj�ca deklaracja encji';
      ET_UNDEFINED_ENTITY_WFC:
        ErrorStr := 'Brakuj�ca deklaracja encji';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC:
        ErrorStr := 'ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC:
        ErrorStr := 'ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC';
      ET_DOCTYPE_NOT_FOUND:
        ErrorStr := 'ET_DOCTYPE_NOT_FOUND';
      ET_DUPLICATE_ELEMENT_TYPE_DECL:
        ErrorStr := 'Podw�jna deklaracja typu elementu';
      ET_DUPLICATE_NAME_IN_MIXED_CONTENT:
        ErrorStr := 'Podw�jna nazwa w mieszanej zawarto�ci';
      ET_DUPLICATE_ID_ON_ELEMENT_TYPE:
        ErrorStr :=
          'Zadeklarowany podw�jny atrybut ID dla element�w tego samego typu';
      ET_UNDEFINED_NOTATION:
        ErrorStr := 'Nie zadeklarowana nazwa notacji';
      ET_DUPLICATE_NOTATION_ON_ELEMENT_TYPE:
        ErrorStr :=
          'Zadeklarowany podw�jny atrybut notacji dla element�w tego samego typu';
      ET_DUPLICATE_NOTATION_TOKEN:
        ErrorStr := 'ET_DUPLICATE_NOTATION_TOKEN';
      ET_NOTATION_ON_EMPTY_ELEMENT:
        ErrorStr := 'Atrybut notacji zadeklarowany dla elementu typu pusty';
      ET_DUPLICATE_ENUMERATION_TOKEN:
        ErrorStr := 'ET_DUPLICATE_ENUMERATION_TOKEN';
      ET_ATTRIBUTE_TYPE_MISMATCH:
        ErrorStr := 'Typ atrybutu i warto�� atrybutu nie pasuj� do siebie';
      ET_DUPLICATE_TOKENS:
        ErrorStr := 'Zadeklarowane dwa identyczne tokeny';
      ET_ID_NEITHER_IMPLIED_NOR_REQUIRED:
        ErrorStr :=
          'Atrybut ID nie jest zadeklarowany ani jako #IMPLIED ani #REQUIRED';
      ET_WRONG_ROOT_ELEMENT_TYPE:
        ErrorStr :=
          'Typ korzenia nie pasuje do nazwy z deklaracji typu dokumentu';
      ET_UNDEFINED_ELEMENT_TYPE:
        ErrorStr := 'Nie znaleziona deklaracja typu elementu';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY';
      ET_ELEMENT_DECLARED_EMPTY_HAS_CONTENT:
        ErrorStr := 'Element zadeklarowany jako EMPTY nie jest pusty';
      ET_NONDETERMINISTIC_ELEMENT_CONTENT_MODEL:
        ErrorStr := 'Model zawarto�ci elementu jest niedeterministyczny';
      ET_DUPLICATE_NOTATION_DECL:
        ErrorStr := 'Podw�jna deklaracja notacji';
      ET_UNDEFINED_ATTRIBUTE:
        ErrorStr := 'Definicja atrybutu nie zosta�a znaleziona';
      ET_REQUIRED_ATTRIBUTE_NOT_SPECIFIED:
        ErrorStr := 'Wymagany atrybut nie zosta� znaleziony';
      ET_FIXED_ATTRIBUTE_MISMATCH:
        ErrorStr :=
          'Zadeklarowana i bie��ca warto�� sta�ego atrybutu nie pasuja do siebie';
      ET_ATTRIBUTE_DEFAULT_TYPE_MISMATCH:
        ErrorStr := 'Typ atrybutu i jego warto�� domy�lna nie pasuj� do siebie';
      ET_DUPLICATE_ID_VALUE:
        ErrorStr := 'Podw�jna warto�� ID';
      ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC';
      ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC';
      ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC:
        ErrorStr := 'ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC';
      ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC:
        ErrorStr := 'ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC';
      ET_TARGET_ID_VALUE_NOT_FOUND:
        ErrorStr :=
          'Warto�� IDREF lub IDREFS odwo�uje si� do warto�ci ID nieistniej�cego elementu';
      ET_UNDEFINED_TARGET_UNPARSED_ENTITY:
        ErrorStr :=
          'Warto�� ENTITY lub ENTITIES odwo�uje si� do niesparsowanej encji, kt�ra nie istnieje';
      ET_PREDEFINED_ENTITY_INCORRECTLY_REDECLARED:
        ErrorStr := 'Niew�a�ciwa deklaracja predefiniowanej encji';
      ET_UNRESOLVABLE_EXTERNAL_SUBSET:
        ErrorStr := 'Zewn�trzny podzbi�r DTD nie zosta� znaleziony';
      ET_UNDEFINED_PARAMETER_ENTITY:
        ErrorStr := 'ET_UNDEFINED_PARAMETER_ENTITY';
      ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION';
      ET_PE_NOT_PROPERLY_NESTED_WITH_DECL:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_DECL';
      ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP';
    else
      ErrorStr := 'Niew�a�ciwy kod �r�d�owy';
    end; {case ...}
  end; {with ...}

  if PosStr <> '' then
    S := Format('[%s] %s(%s): %s', [SeverityStr, FileNameStr, PosStr, ErrorStr])
  else
    S := Format('[%s] %s: %s', [SeverityStr, FileNameStr, ErrorStr]);

  Result := EncodingToUTF16(TISO8859_2Codec, S);
end;

{ TDomPortugueseErrorTranslator }

constructor TDomPortugueseErrorTranslator.Create(AOwner: TComponent);
begin
  inherited;
  FLanguage := iso639_pt;
end;

function TDomPortugueseErrorTranslator.GetSupportedLanguages:
  TIso639LanguageCodeSet;
begin
  Result := [iso639_pt];
end;

function TDomPortugueseErrorTranslator.Translate(
  const Error: TdomError): WideString;
// - This function was provided by Ricardo Albuquerque (Recife, Brazil) -
var
  ErrorStr, FileNameStr, PosStr, SeverityStr, S: string;
begin
  with Error do
  begin
    case Severity of
      DOM_SEVERITY_FATAL_ERROR: SeverityStr := 'Erro Fatal';
      DOM_SEVERITY_ERROR: SeverityStr := 'Erro';
      DOM_SEVERITY_WARNING: SeverityStr := 'Aten��o';
    end;

    FileNameStr := ExtractFileNameFromUri(Uri);
    if EndLineNumber < 1 then
      PosStr := ''
    else if StartLineNumber = EndLineNumber then
    begin
      if StartColumnNumber = EndColumnNumber then
        FmtStr(PosStr, '%d:%d', [EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1) ])
      else
        FmtStr(PosStr, '%d:%d-%d', [EndLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
          EndColumnNumber]);
    end
    else
    begin
      FmtStr(PosStr, '%d:%d-%d:%d', [StartLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
        EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1)]);
    end;

    case RelatedException of
      ET_ATTLIST_DECL_START_EXPECTED:
        ErrorStr := 'ET_ATTLIST_DECL_START_EXPECTED';
      ET_BYTE_ORDER_MARK_ENCODING_MISMATCH:
        ErrorStr := 'ET_BYTE_ORDER_MARK_ENCODING_MISMATCH';
      ET_CDATA_START_EXPECTED:
        ErrorStr := 'ET_CDATA_START_EXPECTED';
      ET_COMMENT_START_EXPECTED:
        ErrorStr := 'ET_COMMENT_START_EXPECTED';
      ET_CONDITIONAL_SECTION_NOT_ALLOWED:
        ErrorStr := 'ET_CONDITIONAL_SECTION_NOT_ALLOWED';
      ET_DOCTYPE_START_EXPECTED:
        ErrorStr := 'ET_DOCTYPE_START_EXPECTED';
      ET_ELEMENT_DECL_START_EXPECTED:
        ErrorStr := 'ET_ELEMENT_DECL_START_EXPECTED';
      ET_ENTITY_DECL_START_EXPECTED:
        ErrorStr := 'ET_ENTITY_DECL_START_EXPECTED';
      ET_INVALID_ELEMENT_NAME:
        ErrorStr := 'Nome de elemento inv�lido';
      ET_DOUBLE_HYPHEN_IN_COMMENT:
        ErrorStr := 'ET_DOUBLE_HYPHEN_IN_COMMENT';
      ET_DOUBLE_ROOT_ELEMENT:
        ErrorStr := 'Elemento raiz duplicado';
      ET_DOUBLE_DOCTYPE:
        ErrorStr := 'Defini��o de tipo de documento(DTD) duplicado';
      ET_HYPHEN_AT_COMMENT_END:
        ErrorStr := 'ET_HYPHEN_AT_COMMENT_END';
      ET_INVALID_ATTRIBUTE_NAME:
        ErrorStr := 'Nome de atributo inv�lido';
      ET_INVALID_ATTRIBUTE_VALUE:
        ErrorStr := 'valor de atributo inv�lido';
      ET_DOUBLE_ATTRIBUTE_NAME:
        ErrorStr := 'Nome de atributo duplicado em um elemento';
      ET_INVALID_ENCODING_NAME:
        ErrorStr := 'ET_INVALID_ENCODING_NAME';
      ET_INVALID_ENTITY_NAME:
        ErrorStr := 'Nome de entidade inv�lida';
      ET_INVALID_PARAMETER_ENTITY_NAME:
        ErrorStr := 'Nome de entidade param�trica inv�lida';
      ET_INVALID_PROCESSING_INSTRUCTION:
        ErrorStr := 'Instru��o de processamento inv�lida';
      ET_INVALID_VERSION_NUMBER:
        ErrorStr := 'ET_INVALID_VERSION_NUMBER';
      ET_INVALID_XML_DECL:
        ErrorStr := 'Declara��o XML inv�lida';
      ET_INVALID_CHAR_REF:
        ErrorStr := 'Refer�ncia a caracter invalida';
      ET_MISSING_QUOTATION_MARK:
        ErrorStr := 'Aspas faltando';
      ET_MISSING_EQUALITY_SIGN:
        ErrorStr := 'Sinal de igualdade faltando';
      ET_DOUBLE_EQUALITY_SIGN:
        ErrorStr := 'Sinal de igualdade duplicado';
      ET_MISSING_WHITE_SPACE:
        ErrorStr := 'Espa�o em branco faltando';
      ET_MISSING_START_TAG:
        ErrorStr := 'Marca de fim sem marca de in�cio ';
      ET_MISSING_ELEMENT_NAME:
        ErrorStr := 'ET_MISSING_ELEMENT_NAME';
      ET_MISSING_ENTITY_NAME:
        ErrorStr := 'ET_MISSING_ENTITY_NAME';
      ET_MISSING_END_TAG:
        ErrorStr := 'Marca de fim faltando';
      ET_INVALID_CHARACTER:
        ErrorStr := 'Caracter inv�lido';
      ET_NOT_IN_ROOT_ELEMENT:
        ErrorStr := 'Caracter(es) fora do elemento raiz';
      ET_ROOT_ELEMENT_NOT_FOUND:
        ErrorStr := 'Elemento raiz n�o encontrado';
      ET_KEYWORD_ANY_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_ANY_NOT_ALLOWED';
      ET_KEYWORD_EMPTY_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_EMPTY_NOT_ALLOWED';
      ET_KEYWORD_PCDATA_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_PCDATA_NOT_ALLOWED';
      ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL:
        ErrorStr := 'ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL';
      ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL';
      ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL';
      ET_NOTATION_DECL_START_EXPECTED:
        ErrorStr := 'ET_NOTATION_DECL_START_EXPECTED';
      ET_PARAMETER_ENTITY_REF_NOT_ALLOWED:
        ErrorStr := 'ET_PARAMETER_ENTITY_REF_NOT_ALLOWED';
      ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL:
        ErrorStr := 'ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL';
      ET_UNCLOSED_ATTLIST_DECL:
        ErrorStr := 'ET_UNCLOSED_ATTLIST_DECL';
      ET_SYSTEM_KEYWORD_EXPECTED:
        ErrorStr := 'ET_SYSTEM_KEYWORD_EXPECTED';
      ET_UNCLOSED_CDATA_SECTION:
        ErrorStr := 'ET_UNCLOSED_CDATA_SECTION';
      ET_UNCLOSED_CHAR_REF:
        ErrorStr := 'ET_UNCLOSED_CHAR_REF';
      ET_UNCLOSED_COMMENT:
        ErrorStr := 'ET_UNCLOSED_COMMENT';
      ET_UNCLOSED_CONDITIONAL_SECTION:
        ErrorStr := 'ET_UNCLOSED_CONDITIONAL_SECTION';
      ET_UNCLOSED_DOCTYPE:
        ErrorStr := 'ET_UNCLOSED_DOCTYPE';
      ET_UNCLOSED_ELEMENT:
        ErrorStr := 'ET_UNCLOSED_ELEMENT';
      ET_UNCLOSED_ELEMENT_DECL:
        ErrorStr := 'ET_UNCLOSED_ELEMENT_DECL';
      ET_UNCLOSED_ENTITY_DECL:
        ErrorStr := 'ET_UNCLOSED_ENTITY_DECL';
      ET_UNCLOSED_ENTITY_REF:
        ErrorStr := 'ET_UNCLOSED_ENTITY_REF';
      ET_UNCLOSED_NOTATION_DECL:
        ErrorStr := 'ET_UNCLOSED_NOTATION_DECL';
      ET_UNCLOSED_PARAMETER_ENTITY_REF:
        ErrorStr := 'ET_UNCLOSED_PARAMETER_ENTITY_REF';
      ET_UNCLOSED_PROCESSING_INSTRUCTION:
        ErrorStr := 'ET_UNCLOSED_PROCESSING_INSTRUCTION';
      ET_WRONG_ORDER:
        ErrorStr := 'Ordem errada';
      ET_UNKNOWN_DECL_TYPE:
        ErrorStr := 'Tipo de declara��o inv�lida';
      ET_INVALID_ATTRIBUTE_DECL:
        ErrorStr := 'Declara��o de atributo inv�lido';
      ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL';
      ET_INVALID_ATTLIST_DECL_NAME:
        ErrorStr := 'ET_INVALID_ATTLIST_DECL_NAME';
      ET_INVALID_CONDITIONAL_SECTION:
        ErrorStr := 'Se��o condicional inv�lida';
      ET_INVALID_ELEMENT_DECL:
        ErrorStr := 'Declara��o de elemento inv�lido';
      ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL';
      ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL:
        ErrorStr := 'ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL';
      ET_INVALID_ENTITY_DECL:
        ErrorStr := 'Declara��o de entidade inv�lida';
      ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL';
      ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL';
      ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL';
      ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL';
      ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL';
      ET_INVALID_MARKUP_DECL:
        ErrorStr := 'Declara��o de markup inv�lido';
      ET_INVALID_NOTATION_DECL:
        ErrorStr := 'Declara��o de nota��o inv�lida';
      ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL';
      ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL';
      ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL';
      ET_INVALID_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Declara��o de entidade param�trica inv�lida';
      ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL:
        ErrorStr := 'ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL';
      ET_INVALID_TEXT_DECL:
        ErrorStr := 'Declara��o de texto inv�lido';
      ET_LEFT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := 'ET_LEFT_SQUARE_BRACKET_EXPECTED';
      ET_LT_IN_ATTRIBUTE_VALUE:
        ErrorStr :=
          '"<" no texto de substitui��o de uma entidade refere-se diretamente ou indiretamente a um  valor de atributo';
      ET_ATTRIBUTE_VALUE_REFERS_TO_EXTERNAL_ENTITY:
        ErrorStr := 'O valor do atributo se refere a uma entidade externa';
      ET_PE_BETWEEN_DECLARATIONS:
        ErrorStr := 'ET_PE_BETWEEN_DECLARATIONS';
      ET_PUBLIC_KEYWORD_EXPECTED:
        ErrorStr := 'ET_PUBLIC_KEYWORD_EXPECTED';
      ET_QUOTATION_MARK_EXPECTED:
        ErrorStr := 'ET_QUOTATION_MARK_EXPECTED';
      ET_RECURSIVE_REFERENCE:
        ErrorStr := 'Encontrada Refer�ncia recursiva';
      ET_REFERS_TO_UNPARSED_ENTITY:
        ErrorStr := 'Refer�ncia a uma entidade n�o analisada';
      ET_RIGHT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := 'ET_RIGHT_SQUARE_BRACKET_EXPECTED';
      ET_NO_PROPER_MARKUP_REFERENCED:
        ErrorStr := 'Refer�ncia a uma entidade contendo marca��o impr�pria';
      ET_INVALID_COMMENT:
        ErrorStr := 'Coment�rio Inv�lido';
      ET_INVALID_CDATA_SECTION:
        ErrorStr := 'Sess�o CDATA inv�lida';
      ET_INVALID_SYSTEM_LITERAL:
        ErrorStr := 'Literal de sistema inv�lido';
      ET_INVALID_PUBID_LITERAL:
        ErrorStr := 'Literal pubid inv�lido';
      ET_INVALID_QUALIFIED_NAME:
        ErrorStr := 'Nome qualificado inv�lido';
      ET_INVALID_PREFIX:
        ErrorStr := 'Prefixo inv�lido ';
      ET_INVALID_NAMESPACE_URI:
        ErrorStr := 'NameSpace URI inv�lido';
      ET_NAMESPACE_URI_NOT_FOUND:
        ErrorStr := 'NameSpace URI n�o encontrado';
      ET_WRONG_PREFIX_MAPPING_NESTING:
        ErrorStr := 'Aninhamento de mapeamento de prefixo inv�lido';
      ET_XML_VERSION_NOT_SUPPORTED:
        ErrorStr := 'ET_XML_VERSION_NOT_SUPPORTED';
      ET_ENCODING_NOT_SUPPORTED:
        ErrorStr := 'Codifica��o n�o suportada';
      ET_INVALID_CHARACTER_IN_EXT_ENTITY:
        ErrorStr := 'ET_INVALID_CHARACTER_IN_EXT_ENTITY';
      ET_EXT_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'ET_EXT_ENTITY_RESOURCE_NOT_FOUND';
      ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND';
      ET_DOUBLE_ATTLISTDECL:
        ErrorStr := 'ET_DOUBLE_ATTLISTDECL';
      ET_DOUBLE_ATTDEF:
        ErrorStr := 'Defini��o de atributo duplicada';
      ET_DOUBLE_ENTITY_DECL:
        ErrorStr := 'Declara��o de entidade duplicada';
      ET_DOUBLE_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Declara��o de entidade param�trica duplicada';
      ET_UNDEFINED_ENTITY_VC:
        ErrorStr := 'Faltando declara��o de entidade';
      ET_UNDEFINED_ENTITY_WFC:
        ErrorStr := 'Faltando declara��o de entidade';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC:
        ErrorStr := 'ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC:
        ErrorStr := 'ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC';
      ET_DOCTYPE_NOT_FOUND:
        ErrorStr := 'ET_DOCTYPE_NOT_FOUND';
      ET_DUPLICATE_ELEMENT_TYPE_DECL:
        ErrorStr := 'Declara��o do tipo do elemento duplicada';
      ET_DUPLICATE_NAME_IN_MIXED_CONTENT:
        ErrorStr := 'Nome duplicado no conte�do misto';
      ET_DUPLICATE_ID_ON_ELEMENT_TYPE:
        ErrorStr :=
          'Atributo ID duplicado na mesma declara��o de tipo de elemento';
      ET_UNDEFINED_NOTATION:
        ErrorStr := 'Nome de anota��o n�o declarado';
      ET_DUPLICATE_NOTATION_ON_ELEMENT_TYPE:
        ErrorStr :=
          'Atributo de anota��o duplicado na mesma declara��o de tipo de elemento';
      ET_DUPLICATE_NOTATION_TOKEN:
        ErrorStr := ' "Token"  de nota��o duplicada';
      ET_NOTATION_ON_EMPTY_ELEMENT:
        ErrorStr :=
          'Atributo de anota��o em uma declara��o de tipo de elemento vazia';
      ET_DUPLICATE_ENUMERATION_TOKEN:
        ErrorStr := ' "Token"  de enumera��o duplicada';
      ET_ATTRIBUTE_TYPE_MISMATCH:
        ErrorStr := 'Tipo de atributo e valor de atributo n�o coincidem';
      ET_DUPLICATE_TOKENS:
        ErrorStr := 'Declara��o de "tokens" duplicada';
      ET_ID_NEITHER_IMPLIED_NOR_REQUIRED:
        ErrorStr :=
          'Atributo ID n�o foi declarado como #IMPLIED nem como #REQUIRED';
      ET_WRONG_ROOT_ELEMENT_TYPE:
        ErrorStr :=
          'O tipo de elemento raiz n�o tem o mesmo nome que a declara��o do tipo de documento';
      ET_UNDEFINED_ELEMENT_TYPE:
        ErrorStr := 'Declara��o de tipo de elemento n�o encontrada';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY';
      ET_ELEMENT_DECLARED_EMPTY_HAS_CONTENT:
        ErrorStr := 'Elemento declarado como "EMPTY" n�o est� vazio';
      ET_NONDETERMINISTIC_ELEMENT_CONTENT_MODEL:
        ErrorStr := 'O modelo de conte�do de um elemento n�o � determinante';
      ET_DUPLICATE_NOTATION_DECL:
        ErrorStr := 'Declara��o de anota��o duplicada';
      ET_UNDEFINED_ATTRIBUTE:
        ErrorStr := 'Defini��o de atributo n�o encontrada';
      ET_REQUIRED_ATTRIBUTE_NOT_SPECIFIED:
        ErrorStr := 'Atributo requerido n�o encontrado';
      ET_FIXED_ATTRIBUTE_MISMATCH:
        ErrorStr :=
          'O valor declarado e o valor atual de um atributo fixo n�o coincidem';
      ET_ATTRIBUTE_DEFAULT_TYPE_MISMATCH:
        ErrorStr :=
          'O tipo de atributo e o atributo por defini��o n�o coincidem';
      ET_DUPLICATE_ID_VALUE:
        ErrorStr := 'Valor de ID duplicado';
      ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC';
      ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC';
      ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC:
        ErrorStr := 'ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC';
      ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC:
        ErrorStr := 'ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC';
      ET_TARGET_ID_VALUE_NOT_FOUND:
        ErrorStr :=
          'O valor de IDREF ou IDREFS se refere a um valor de ID n�o existente';
      ET_UNDEFINED_TARGET_UNPARSED_ENTITY:
        ErrorStr :=
          'O valor de ENTITY ou ENTITIES se refere a uma entidade n�o existente';
      ET_PREDEFINED_ENTITY_INCORRECTLY_REDECLARED:
        ErrorStr := 'Declara��o err�nea de uma entidade pr�-definida';
      ET_UNRESOLVABLE_EXTERNAL_SUBSET:
        ErrorStr := 'Subgrupo externo do DTD n�o encontrado';
      ET_UNDEFINED_PARAMETER_ENTITY:
        ErrorStr := 'Declara��o de entidade de par�metro n�o encontrada';
      ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION';
      ET_PE_NOT_PROPERLY_NESTED_WITH_DECL:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_DECL';
      ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP';
    else
      ErrorStr := 'C�digo fonte inv�lido';
    end; {case ...}
  end; {with ...}

  if PosStr <> '' then
    S := Format('[%s] %s(%s): %s', [SeverityStr, FileNameStr, PosStr, ErrorStr])
  else
    S := Format('[%s] %s: %s', [SeverityStr, FileNameStr, ErrorStr]);

  Result := EncodingToUTF16(TISO8859_1Codec, S);
end;

{ TDomSpanishErrorTranslator }

constructor TDomSpanishErrorTranslator.Create(AOwner: TComponent);
begin
  inherited;
  FLanguage := iso639_es;
end;

function TDomSpanishErrorTranslator.GetSupportedLanguages:
  TIso639LanguageCodeSet;
begin
  Result := [iso639_es];
end;

function TDomSpanishErrorTranslator.Translate(
  const Error: TdomError): WideString;
// - This function was provided by Pedro de Paz -
var
  ErrorStr, FileNameStr, PosStr, SeverityStr, S: string;
begin
  with Error do
  begin
    case Severity of
      DOM_SEVERITY_FATAL_ERROR: SeverityStr := 'Error Fatal';
      DOM_SEVERITY_ERROR: SeverityStr := 'Error';
      DOM_SEVERITY_WARNING: SeverityStr := 'Aviso';
    end;

    FileNameStr := ExtractFileNameFromUri(Uri);
    if EndLineNumber < 1 then
      PosStr := ''
    else if StartLineNumber = EndLineNumber then
    begin
      if StartColumnNumber = EndColumnNumber then
        FmtStr(PosStr, '%d:%d', [EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1) ])
      else
        FmtStr(PosStr, '%d:%d-%d', [EndLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
          EndColumnNumber]);
    end
    else
    begin
      FmtStr(PosStr, '%d:%d-%d:%d', [StartLineNumber, StartColumnNumber + StartTabsInLine * (TabWidth - 1),
        EndLineNumber, EndColumnNumber + EndTabsInLine * (TabWidth - 1)]);
    end;

    case RelatedException of
      ET_ATTLIST_DECL_START_EXPECTED:
        ErrorStr := 'ET_ATTLIST_DECL_START_EXPECTED';
      ET_BYTE_ORDER_MARK_ENCODING_MISMATCH:
        ErrorStr := 'ET_BYTE_ORDER_MARK_ENCODING_MISMATCH';
      ET_CDATA_START_EXPECTED:
        ErrorStr := 'ET_CDATA_START_EXPECTED';
      ET_COMMENT_START_EXPECTED:
        ErrorStr := 'ET_COMMENT_START_EXPECTED';
      ET_CONDITIONAL_SECTION_NOT_ALLOWED:
        ErrorStr := 'ET_CONDITIONAL_SECTION_NOT_ALLOWED';
      ET_DOCTYPE_START_EXPECTED:
        ErrorStr := 'ET_DOCTYPE_START_EXPECTED';
      ET_ELEMENT_DECL_START_EXPECTED:
        ErrorStr := 'ET_ELEMENT_DECL_START_EXPECTED';
      ET_ENTITY_DECL_START_EXPECTED:
        ErrorStr := 'ET_ENTITY_DECL_START_EXPECTED';
      ET_INVALID_ELEMENT_NAME:
        ErrorStr := 'Nombre de elemento no v�lido';
      ET_DOUBLE_HYPHEN_IN_COMMENT:
        ErrorStr := 'ET_DOUBLE_HYPHEN_IN_COMMENT';
      ET_DOUBLE_ROOT_ELEMENT:
        ErrorStr := 'Elemento raiz duplicado';
      ET_DOUBLE_DOCTYPE:
        ErrorStr := 'Declaraci�n de tipo de documento (DTD) duplicada';
      ET_HYPHEN_AT_COMMENT_END:
        ErrorStr := 'ET_HYPHEN_AT_COMMENT_END';
      ET_INVALID_ATTRIBUTE_NAME:
        ErrorStr := 'Nombre de atributo no v�lido';
      ET_INVALID_ATTRIBUTE_VALUE:
        ErrorStr := 'Valor de atributo no v�lido';
      ET_DOUBLE_ATTRIBUTE_NAME:
        ErrorStr := 'Atributo duplicado en un elemento';
      ET_INVALID_ENCODING_NAME:
        ErrorStr := 'ET_INVALID_ENCODING_NAME';
      ET_INVALID_ENTITY_NAME:
        ErrorStr := 'Nombre de entidad no v�lida';
      ET_INVALID_PARAMETER_ENTITY_NAME:
        ErrorStr := 'Nombre de entidad con parametros no v�lida';
      ET_INVALID_PROCESSING_INSTRUCTION:
        ErrorStr := 'Instrucci�n de proceso no v�lida';
      ET_INVALID_VERSION_NUMBER:
        ErrorStr := 'ET_INVALID_VERSION_NUMBER';
      ET_INVALID_XML_DECL:
        ErrorStr := 'Declaraci�n XML no v�lida';
      ET_INVALID_CHAR_REF:
        ErrorStr := 'Referencia a caracter no v�lida';
      ET_MISSING_QUOTATION_MARK:
        ErrorStr := 'No se encontraron comillas';
      ET_MISSING_EQUALITY_SIGN:
        ErrorStr := 'No se encontr� simbolo de igualdad';
      ET_DOUBLE_EQUALITY_SIGN:
        ErrorStr := 'Signo de igualdad duplicado';
      ET_MISSING_WHITE_SPACE:
        ErrorStr := 'No se encontr� espacio en blanco';
      ET_MISSING_START_TAG:
        ErrorStr := 'Tag final sin tag de inicio';
      ET_MISSING_ELEMENT_NAME:
        ErrorStr := 'ET_MISSING_ELEMENT_NAME';
      ET_MISSING_ENTITY_NAME:
        ErrorStr := 'ET_MISSING_ENTITY_NAME';
      ET_MISSING_END_TAG:
        ErrorStr := 'Tag de inicio sin tag final';
      ET_INVALID_CHARACTER:
        ErrorStr := 'Caracter no v�lido';
      ET_NOT_IN_ROOT_ELEMENT:
        ErrorStr := 'Caracter(es) fuera del elemento raiz';
      ET_ROOT_ELEMENT_NOT_FOUND:
        ErrorStr := 'Elemento raiz no encontrado';
      ET_KEYWORD_ANY_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_ANY_NOT_ALLOWED';
      ET_KEYWORD_EMPTY_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_EMPTY_NOT_ALLOWED';
      ET_KEYWORD_PCDATA_NOT_ALLOWED:
        ErrorStr := 'ET_KEYWORD_PCDATA_NOT_ALLOWED';
      ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL:
        ErrorStr := 'ET_MALFORMED_CONTENT_MODEL_IN_ELEMENT_DECL';
      ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'ET_MISSING_ENTITY_VALUE_IN_ENTITY_DECL';
      ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_MISSING_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL';
      ET_NOTATION_DECL_START_EXPECTED:
        ErrorStr := 'ET_NOTATION_DECL_START_EXPECTED';
      ET_PARAMETER_ENTITY_REF_NOT_ALLOWED:
        ErrorStr := 'ET_PARAMETER_ENTITY_REF_NOT_ALLOWED';
      ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL:
        ErrorStr := 'ET_INVALID_CONTENT_MODEL_TOKEN_IN_ELEMENT_DECL';
      ET_UNCLOSED_ATTLIST_DECL:
        ErrorStr := 'ET_UNCLOSED_ATTLIST_DECL';
      ET_SYSTEM_KEYWORD_EXPECTED:
        ErrorStr := 'ET_SYSTEM_KEYWORD_EXPECTED';
      ET_UNCLOSED_CDATA_SECTION:
        ErrorStr := 'ET_UNCLOSED_CDATA_SECTION';
      ET_UNCLOSED_CHAR_REF:
        ErrorStr := 'ET_UNCLOSED_CHAR_REF';
      ET_UNCLOSED_COMMENT:
        ErrorStr := 'ET_UNCLOSED_COMMENT';
      ET_UNCLOSED_CONDITIONAL_SECTION:
        ErrorStr := 'ET_UNCLOSED_CONDITIONAL_SECTION';
      ET_UNCLOSED_DOCTYPE:
        ErrorStr := 'ET_UNCLOSED_DOCTYPE';
      ET_UNCLOSED_ELEMENT:
        ErrorStr := 'ET_UNCLOSED_ELEMENT';
      ET_UNCLOSED_ELEMENT_DECL:
        ErrorStr := 'ET_UNCLOSED_ELEMENT_DECL';
      ET_UNCLOSED_ENTITY_DECL:
        ErrorStr := 'ET_UNCLOSED_ENTITY_DECL';
      ET_UNCLOSED_ENTITY_REF:
        ErrorStr := 'ET_UNCLOSED_ENTITY_REF';
      ET_UNCLOSED_NOTATION_DECL:
        ErrorStr := 'ET_UNCLOSED_NOTATION_DECL';
      ET_UNCLOSED_PARAMETER_ENTITY_REF:
        ErrorStr := 'ET_UNCLOSED_PARAMETER_ENTITY_REF';
      ET_UNCLOSED_PROCESSING_INSTRUCTION:
        ErrorStr := 'ET_UNCLOSED_PROCESSING_INSTRUCTION';
      ET_WRONG_ORDER:
        ErrorStr := 'Orden equivocado';
      ET_UNKNOWN_DECL_TYPE:
        ErrorStr := 'Tipo de declaraci�n desconocida';
      ET_INVALID_ATTRIBUTE_DECL:
        ErrorStr := 'Declaraci�n de atributo no v�lida';
      ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ATTRIBUTE_NAME_IN_ATTRIBUTE_DECL';
      ET_INVALID_ATTLIST_DECL_NAME:
        ErrorStr := 'ET_INVALID_ATTLIST_DECL_NAME';
      ET_INVALID_CONDITIONAL_SECTION:
        ErrorStr := 'Secci�n condicional no v�lida';
      ET_INVALID_ELEMENT_DECL:
        ErrorStr := 'Declaraci�n de elemento no v�lida';
      ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ELEMENT_NAME_IN_ATTRIBUTE_DECL';
      ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL:
        ErrorStr := 'ET_INVALID_ELEMENT_NAME_IN_ELEMENT_DECL';
      ET_INVALID_ENTITY_DECL:
        ErrorStr := 'Declaraci�n de entidad no v�lida';
      ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_NAME_IN_ENTITY_DECL';
      ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_NAME_IN_PARAMETER_ENTITY_DECL';
      ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_VALUE_IN_ENTITY_DECL';
      ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_ENTITY_VALUE_IN_PARAMETER_ENTITY_DECL';
      ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_ENUMERATION_TOKEN_IN_ATTRIBUTE_DECL';
      ET_INVALID_MARKUP_DECL:
        ErrorStr := 'Declaraci�n de markup no v�lida';
      ET_INVALID_NOTATION_DECL:
        ErrorStr := 'Declaraci�n de anotaci�n no v�lida';
      ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_NAME_IN_ENTITY_DECL';
      ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_NAME_IN_NOTATION_DECL';
      ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL:
        ErrorStr := 'ET_INVALID_NOTATION_TOKEN_IN_ATTRIBUTE_DECL';
      ET_INVALID_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Declaraci�n de entidad con parametros no v�lida';
      ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL:
        ErrorStr := 'ET_INVALID_ROOT_ELEMENT_NAME_IN_DOCTYPE_DECL';
      ET_INVALID_TEXT_DECL:
        ErrorStr := 'Declaraci�n de texto no v�lida';
      ET_LEFT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := 'ET_LEFT_SQUARE_BRACKET_EXPECTED';
      ET_LT_IN_ATTRIBUTE_VALUE:
        ErrorStr :=
          'Encontrado el caracter "<" en el texto de sustituci�n de una entidad que refiere directa o indirectamente a un valor de atributo';
      ET_ATTRIBUTE_VALUE_REFERS_TO_EXTERNAL_ENTITY:
        ErrorStr := 'Valor de atributo apuntando a una entidad externa';
      ET_PE_BETWEEN_DECLARATIONS:
        ErrorStr := 'ET_PE_BETWEEN_DECLARATIONS';
      ET_PUBLIC_KEYWORD_EXPECTED:
        ErrorStr := 'ET_PUBLIC_KEYWORD_EXPECTED';
      ET_QUOTATION_MARK_EXPECTED:
        ErrorStr := 'ET_QUOTATION_MARK_EXPECTED';
      ET_RECURSIVE_REFERENCE:
        ErrorStr := 'Encontrada referencia recursiva';
      ET_REFERS_TO_UNPARSED_ENTITY:
        ErrorStr := 'Referencia a una entidad no analizada';
      ET_RIGHT_SQUARE_BRACKET_EXPECTED:
        ErrorStr := 'ET_RIGHT_SQUARE_BRACKET_EXPECTED';
      ET_NO_PROPER_MARKUP_REFERENCED:
        ErrorStr :=
          'Referencia a una entidad que contiene un "markup" no adecuado';
      ET_INVALID_COMMENT:
        ErrorStr := 'Comentario no v�lido';
      ET_INVALID_CDATA_SECTION:
        ErrorStr := 'Secci�n CDATA no v�lida';
      ET_INVALID_SYSTEM_LITERAL:
        ErrorStr := 'Literal SYSTEM no v�lido';
      ET_INVALID_PUBID_LITERAL:
        ErrorStr := 'Literal PUBID no v�lido';
      ET_INVALID_QUALIFIED_NAME:
        ErrorStr := 'Nombre cualificado no v�lido';
      ET_INVALID_PREFIX:
        ErrorStr := 'Prefijo no v�lido';
      ET_INVALID_NAMESPACE_URI:
        ErrorStr := 'NAMESPACE URI no v�lido';
      ET_NAMESPACE_URI_NOT_FOUND:
        ErrorStr := 'NAMESPACE URI no encontrado';
      ET_WRONG_PREFIX_MAPPING_NESTING:
        ErrorStr := 'Anidamiento de mapeo de prefijos incorrecto';
      ET_XML_VERSION_NOT_SUPPORTED:
        ErrorStr := 'ET_XML_VERSION_NOT_SUPPORTED';
      ET_ENCODING_NOT_SUPPORTED:
        ErrorStr := 'Tipo de ENCODING no soportado';
      ET_INVALID_CHARACTER_IN_EXT_ENTITY:
        ErrorStr := 'ET_INVALID_CHARACTER_IN_EXT_ENTITY';
      ET_EXT_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'ET_EXT_ENTITY_RESOURCE_NOT_FOUND';
      ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND:
        ErrorStr := 'ET_EXT_PARAMETER_ENTITY_RESOURCE_NOT_FOUND';
      ET_DOUBLE_ATTLISTDECL:
        ErrorStr := 'ET_DOUBLE_ATTLISTDECL';
      ET_DOUBLE_ATTDEF:
        ErrorStr := 'Definici�n de atributo duplicada';
      ET_DOUBLE_ENTITY_DECL:
        ErrorStr := 'Declaraci�n de entidad duplicada';
      ET_DOUBLE_PARAMETER_ENTITY_DECL:
        ErrorStr := 'Declaraci�n de entidad con parametros duplicados';
      ET_UNDEFINED_ENTITY_VC:
        ErrorStr := 'No existe declaraci�n de entidad';
      ET_UNDEFINED_ENTITY_WFC:
        ErrorStr := 'No existe declaraci�n de entidad';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC:
        ErrorStr := 'ET_ENTITY_REFERENCED_BEFORE_DECLARED_VC';
      ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC:
        ErrorStr := 'ET_ENTITY_REFERENCED_BEFORE_DECLARED_WFC';
      ET_DOCTYPE_NOT_FOUND:
        ErrorStr := 'ET_DOCTYPE_NOT_FOUND';
      ET_DUPLICATE_ELEMENT_TYPE_DECL:
        ErrorStr := 'Duplicada declaraci�n de tipo de elemento';
      ET_DUPLICATE_NAME_IN_MIXED_CONTENT:
        ErrorStr := 'Nombre duplicado en contenido mezclado';
      ET_DUPLICATE_ID_ON_ELEMENT_TYPE:
        ErrorStr :=
          'Atributo ID duplicado en la misma declaraci�n de tipo de elemento';
      ET_UNDEFINED_NOTATION:
        ErrorStr := 'Nombre de anotaci�n no declarado';
      ET_DUPLICATE_NOTATION_ON_ELEMENT_TYPE:
        ErrorStr :=
          'Atributo de anotaci�n duplicado en la misma declaraci�n de tipo de elemento';
      ET_DUPLICATE_NOTATION_TOKEN:
        ErrorStr := '"token" de notaci�n duplicado';
      ET_NOTATION_ON_EMPTY_ELEMENT:
        ErrorStr :=
          'Atributo de anotaci�n en una declaraci�n de tipo de elemento vacia';
      ET_DUPLICATE_ENUMERATION_TOKEN:
        ErrorStr := '"token" de enumeraci�n duplicado';
      ET_ATTRIBUTE_TYPE_MISMATCH:
        ErrorStr := 'Tipo de atributo y valor de atributo no coinciden';
      ET_DUPLICATE_TOKENS:
        ErrorStr := 'Declaraci�n de "tokens" duplicada';
      ET_ID_NEITHER_IMPLIED_NOR_REQUIRED:
        ErrorStr :=
          'Atributo ID no ha sido declarado como #IMPLIED ni como #REQUIRED';
      ET_WRONG_ROOT_ELEMENT_TYPE:
        ErrorStr :=
          'El tipo del elemento raiz no tiene el mismo nombre que la declaraci�n del tipo de documento';
      ET_UNDEFINED_ELEMENT_TYPE:
        ErrorStr := 'Declaraci�n de tipo de elemento no encontrada';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CDATA_SECTION_WHERE_ELEMENT_ONLY';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_CHILD_ELEMENT_TYPE';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_ELEMENT_WHERE_PCDATA_ONLY';
      ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY:
        ErrorStr := 'ET_ELEMENT_CONTENT_MODEL_MISMATCH_WITH_PCDATA_WHERE_ELEMENT_ONLY';
      ET_ELEMENT_DECLARED_EMPTY_HAS_CONTENT:
        ErrorStr := 'Elemento declarado como "EMPTY" no esta vacio';
      ET_NONDETERMINISTIC_ELEMENT_CONTENT_MODEL:
        ErrorStr := 'El modelo de contenido de un elemento no es determinante';
      ET_DUPLICATE_NOTATION_DECL:
        ErrorStr := 'Declaraci�n de anotaci�n duplicada';
      ET_UNDEFINED_ATTRIBUTE:
        ErrorStr := 'Definici�n de atributo no encontrada';
      ET_REQUIRED_ATTRIBUTE_NOT_SPECIFIED:
        ErrorStr := 'Atributo requerido no encontrado';
      ET_FIXED_ATTRIBUTE_MISMATCH:
        ErrorStr :=
          'El valor declarado y el valor actual de un atributo fijo no coincide';
      ET_ATTRIBUTE_DEFAULT_TYPE_MISMATCH:
        ErrorStr := 'El tipo de atributo y el atributo por defecto no coincide';
      ET_DUPLICATE_ID_VALUE:
        ErrorStr := 'Valor de ID duplicado';
      ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'ET_UNNORMALIZED_EXT_ATTR_IN_STANDALONE_DOC';
      ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC:
        ErrorStr := 'ET_UNSPECIFIED_EXT_ATTR_IN_STANDALONE_DOC';
      ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC:
        ErrorStr := 'ET_EXT_DECL_ENTITY_REFERENCED_IN_STANDALONE_DOC';
      ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC:
        ErrorStr := 'ET_WHITESPACE_IN_EXT_ELEMENT_CONTENT_IN_STANDALONE_DOC';
      ET_TARGET_ID_VALUE_NOT_FOUND:
        ErrorStr :=
          'El valor de IDREF o IDREFS apunta a un valor de ID no existente';
      ET_UNDEFINED_TARGET_UNPARSED_ENTITY:
        ErrorStr :=
          'El valor de ENTITY o ENTITIES apunta a una entidad no existente';
      ET_PREDEFINED_ENTITY_INCORRECTLY_REDECLARED:
        ErrorStr := 'Declaraci�n erronea de una entidad predefinida';
      ET_UNRESOLVABLE_EXTERNAL_SUBSET:
        ErrorStr := '"Subset" externo del DTD no encontrado';
      ET_UNDEFINED_PARAMETER_ENTITY:
        ErrorStr := 'Declaraci�n de entidad de par�metro no encontrada';
      ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_CONDITIONAL_SECTION';
      ET_PE_NOT_PROPERLY_NESTED_WITH_DECL:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_DECL';
      ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP:
        ErrorStr := 'ET_PE_NOT_PROPERLY_NESTED_WITH_PARENTHESIZED_GROUP';
    else
      ErrorStr := 'Codigo fuente no v�lido';
    end; {case ...}
  end; {with ...}

  if PosStr <> '' then
    S := Format('[%s] %s(%s): %s', [SeverityStr, FileNameStr, PosStr, ErrorStr])
  else
    S := Format('[%s] %s: %s', [SeverityStr, FileNameStr, ErrorStr]);

  Result := EncodingToUTF16(TISO8859_1Codec, S);
end;

{ TDomErrorAutoStrings }

function TDomErrorAutoStrings.AddError(AError: TDomError): Integer;
begin
  Result := AddObject('', AError.CloneError);
end;

function TDomErrorAutoStrings.AddError(const ErrorType: TXmlErrorType;
  const StartByteNumber, StartCharNumber, StartColumnNumber,
  StartLineNumber, StartTabsInLine, EndByteNumber, EndCharNumber,
  EndColumnNumber, EndLineNumber, EndTabsInLine: Int64;
  const UriStr: WideString; const RASObject: TDtdObject;
  const RNode: TdomNode; const Code, Clue: WideString): Integer;
begin
  Result := AddObject('', TDomError.Create(ErrorType, StartByteNumber,
  StartCharNumber, StartColumnNumber, StartLineNumber, StartTabsInLine,
  EndByteNumber, EndCharNumber, EndColumnNumber, EndLineNumber, EndTabsInLine,
  UriStr, RASObject, RNode, Code, Clue));
end;

procedure TDomErrorAutoStrings.Assign(Source: TPersistent);
var
  I: Integer;
begin
  if Source is TDomErrorAutoStrings then
  begin
    if Source = Self then
      Exit;
    DisableControls;
    try
      Clear;
      for I := 0 to Pred(TDomErrorAutoStrings(Source).Count) do
        AddError(TDomErrorAutoStrings(Source).Errors[I]);
    finally
      EnableControls;
    end;
    Exit;
  end;
  inherited Assign(Source);
end;

procedure TDomErrorAutoStrings.Clear;
var
  I: Integer;
begin
  for I := 0 to Pred(Count) do
    Errors[I].Free;
  inherited;
end;

procedure TDomErrorAutoStrings.Delete(Index: Integer);
begin
  Errors[Index].Free;
  inherited;
end;

procedure TDomErrorAutoStrings.DisableControls;
begin
  DoNotifyDisableControls;
end;

procedure TDomErrorAutoStrings.EnableControls;
begin
  DoNotifyEnableControls;
end;

function TDomErrorAutoStrings.Get(Index: Integer): string;
begin
  if Assigned(FErrorTranslator) then
  begin
{$IFDEF LINUX}
    Result := UTF16BEDecode(TISO8859_1Codec,
      FErrorTranslator.Translate(Errors[Index]));
{$ELSE}
    try
      Result := UTF16ToEncoding(GetSystemEncodingCodecClass,
        FErrorTranslator.Translate(Errors[Index]));
    except
      Result := Errors[Index].ClassName;
    end;
{$ENDIF}
  end
  else
    Result := Errors[Index].ClassName;
end;

function TDomErrorAutoStrings.GetError(Index: Integer): TDomError;
begin
  Result := Objects[Index] as TDomError;
end;

function TDomErrorAutoStrings.InsertError(Index: Integer;
  AError: TDomError): Boolean;
begin
  Result := InsertObject(Index, '', AError.CloneError);
end;

function TDomErrorAutoStrings.InsertError(Index: Integer;
  const ErrorType: TXmlErrorType; const StartByteNumber, StartCharNumber,
  StartColumnNumber, StartLineNumber, StartTabsInLine, EndByteNumber,
  EndCharNumber, EndColumnNumber, EndLineNumber, EndTabsInLine: Int64;
  const UriStr: WideString; const RASObject: TDtdObject;
  const RNode: TdomNode; const Code, Clue: WideString): Boolean;
begin
  Result := InsertObject(Index, '', TDomError.Create(ErrorType, StartByteNumber,
  StartCharNumber, StartColumnNumber, StartLineNumber, StartTabsInLine,
  EndByteNumber, EndCharNumber, EndColumnNumber, EndLineNumber, EndTabsInLine,
  UriStr, RASObject, RNode, Code, Clue));
end;

procedure TDomErrorAutoStrings.Notification(AComponent: TComponent;
  Operation: TOperation);
begin
  inherited;
  if (Operation = opRemove) and (AComponent = FErrorTranslator) then
    FErrorTranslator := nil;
end;

procedure TDomErrorAutoStrings.SetErrorTranslator(
  const AErrorTranslator: TDomCustomErrorTranslator);
begin
  if FErrorTranslator = AErrorTranslator then
    exit;
{$IFDEF VER140+}
  if assigned(FErrorTranslator) then
    FErrorTranslator.RemoveFreeNotification(Self);
{$ENDIF}
{$IFDEF LINUX}
  if assigned(FErrorTranslator) then
    FErrorTranslator.RemoveFreeNotification(Self);
{$ENDIF}
  FErrorTranslator := AErrorTranslator;
  if assigned(AErrorTranslator) then
    AErrorTranslator.FreeNotification(Self);
end;

end.

