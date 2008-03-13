unit ParserUtilsRTL;

// ParserUtilsRTL 3.0.1 -- .NET version
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
// The Original Code is "ParserUtilsRTL.pas".
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
// 2007-12-03 3.0.1 Made .NET compliant.
// 2006-09-24 3.0.0 Completely refactored.
// 2004-09-25 2.0.2 TUtilsUCS4Reader.Create modified, DefaultCodecClass added.
// 2004-08-08 2.0.1 TUtilsNoRefCount moved to separate unit.
// 2004-06-01 2.0.0 Completely refactored.
// 2004-01-04 1.0.2 Small revisions.
// 2003-11-16 1.0.1 Small revisions.
// 2003-08-03 1.0.0


{$INCLUDE cDefines.inc}

interface

uses
  cUnicodeCodecsRTL,
  UnicodeUtils, SysUtils, Classes;

type
  EParserUtilsException = class(Exception);

type
  TUtilsCustomInputStream = class
  private
    FStream: TStream;
    FBuffer: TBytes;
    FInitialStreamPosition: Int64;
    FPosition: Int64;
    FRemaining: Int64;
    FBufSize: Integer;
  protected
    function GetPosition: Int64; virtual;
    function Read(var Buf: array of Byte; const Count: Longint): Boolean; virtual;  
    procedure SetPosition(Value: Int64); virtual;

    property BufSize: Integer read FBufSize;
    property InitialStreamPosition: Int64 read FInitialStreamPosition;
    property Position: Int64 read GetPosition write SetPosition;
  public
    constructor Create(const Stream: TStream; const BufSize: Integer);
    destructor Destroy; override;
    procedure FlushBuffer; virtual;
  end;

  TUtilsCustomOutputStream = class
  private
    FStream: TStream;
    FBuffer: TBytes;
    FInitialStreamPosition: Int64;
    FPosition: Int64;
    FBufSize: Integer;
  protected
    function GetPosition: Int64; virtual;
    procedure SetPosition(Value: Int64); virtual;
    procedure Write(const Buf: TBytes; const Count: Longint); virtual;

    property BufSize: Integer read FBufSize;
    property InitialStreamPosition: Int64 read FInitialStreamPosition;
    property Position: Int64 read GetPosition write SetPosition;
  public
    constructor Create(const Stream: TStream; const BufSize: Integer);
    destructor Destroy; override;
    procedure FlushBuffer; virtual;
  end;

  TUtilsCustomAutodetectInputStream = class(TUtilsCustomInputStream)
  private
    FByteOrderMarkSize: Cardinal;
    FCodec: TUnicodeCodecClass;
    procedure FindBOM(const Stream: TStream; out BOMType: TUnicodeCodecClass;
      out BOMSize: Cardinal);
    procedure Reset;
  protected
    function GetPosition: Int64; override;
    procedure SetCodec(Value: TUnicodeCodecClass); virtual;
    procedure SetPosition(Value: Int64); override;

    property ByteOrderMarkSize: Cardinal read FByteOrderMarkSize;
  public
    constructor Create(const Stream: TStream; const BufSize: Integer;
      const ExpectedEncoding: TUnicodeCodecClass);

    property BufSize;
    property Codec: TUnicodeCodecClass read FCodec;
  end;

  TUtilsAutodetectInputStream = class(TUtilsCustomAutodetectInputStream)
  protected
    function GetHasByteOrderMark: Boolean; virtual;
  public
    property Position;
    property HasByteOrderMark: Boolean read GetHasByteOrderMark;
  end;

  TUtilsUCS4CharData = record
    ByteCount: Int64;
    CharCount: Int64;
    CharsInLine: Int64;
    CodePoint: UCS4Char;
    Line: Int64;
    Size: Cardinal;
    TabsInLine: Int64;
  end;

  TUtilsUCS4Reader = class
  private
    FCodec: TCustomUnicodeCodec;
    FCurrentUCS4Char: TUtilsUCS4CharData;
    FInternalInputStream: TUtilsAutodetectInputStream;
    FInitialUCS4CharData: TUtilsUCS4CharData;
    FNextUCS4Char: TUtilsUCS4CharData;
    FPreviousUCS4Char: TUtilsUCS4CharData;
    FReadLFOption: TCodecReadLFOption;
    FResetPosition: Int64;
    function GetBof: Boolean;
    function GetBufSize: Integer;
    function GetEof: Boolean;
    function GetHasByteOrderMark: Boolean;
    function GetPosition: Int64;
    procedure SetReadLFOption(const Value: TCodecReadLFOption);
    procedure UpdateLocator(var UCS4CharData: TUtilsUCS4CharData);
  protected
    function GetCodecClass: TUnicodeCodecClass; virtual;
    function GetDefaultCodecClass: TUnicodeCodecClass; virtual;
    procedure ReadEventHandler(Sender: TObject; var Buf: array of Byte; Count: Longint; var Ok:  
      Boolean); virtual;
    procedure SetCodecClass(const Value: TUnicodeCodecClass); virtual;
    procedure SetResetPosition(const Value: Int64); virtual;

    property DefaultCodecClass: TUnicodeCodecClass read GetDefaultCodecClass;
    property InternalInputStream: TUtilsAutodetectInputStream read FInternalInputStream;
    property InitialUCS4CharData: TUtilsUCS4CharData read FInitialUCS4CharData
      write FInitialUCS4CharData;
    property Position: Int64 read GetPosition;
    property ReadLFOption: TCodecReadLFOption read FReadLFOption write
      SetReadLFOption default lrNormalize;
    property ResetPosition: Int64 read FResetPosition write SetResetPosition;
  public
    constructor Create(const Stream: TStream; const ABufSize: Integer;
      const ACodecClass: TUnicodeCodecClass; const InitialByteCount,
      InitialCharCount, InitialCharsInLine, InitialTabsInLine,
      InitialLine: Int64);
    destructor Destroy; override;
    function Match(Ucs2Str: WideString): Boolean; virtual;
    procedure Next; virtual;
    procedure Reset; virtual;
    function SkipNext(Ucs2Str: WideString): Integer; virtual;

    property Bof: Boolean read GetBof;
    property BufSize: Integer read GetBufSize;
    property CodecClass: TUnicodeCodecClass read GetCodecClass;
    property CurrentCharInfo: TUtilsUCS4CharData read FCurrentUCS4Char;
    property Eof: Boolean read GetEof;
    property HasByteOrderMark: Boolean read GetHasByteOrderMark;
    property NextCharInfo: TUtilsUCS4CharData read FNextUCS4Char;
    property PreviousCharInfo: TUtilsUCS4CharData read FPreviousUCS4Char;
  end;

  TUtilsLineBreakOpt = (
    lbCRLF,
    lbCR,
    lbLF,
    lbNone);

  TUtilsCustomTranscoder = class
  private
    FInputCodec: TCustomUnicodeCodec;
    FInputEncoding: WideString;
    FLineBreakOpt: TUtilsLineBreakOpt;
    FOutputCodec: TCustomUnicodeCodec;
    FOutputEncoding: WideString;

    FOnProgress: TNotifyEvent;
    procedure SetInputEncoding(const Value: WideString);
    procedure SetLineBreakOpt(const Value: TUtilsLineBreakOpt);
    procedure SetOutputEncoding(const Value: WideString);
    procedure UpdateLineBreakOpt;
  protected
    FBusy: Boolean;
    procedure CodecReadEventHandler(Sender: TObject;
      var Buf: array of Byte;    
      Count: Longint;
      var Ok: Boolean); virtual;
    procedure CodecWriteEventHandler(Sender: TObject;
      const Buf: array of Byte;  
      Count: Longint); virtual;
    procedure DoProgress; virtual;

    property Busy: Boolean read FBusy;
    property InputCodec: TCustomUnicodeCodec read FInputCodec;
    property InputEncoding: WideString read FInputEncoding write
      SetInputEncoding;
    property LineBreakOpt: TUtilsLineBreakOpt read FLineBreakOpt write
      SetLineBreakOpt default lbNone;
    property OutputCodec: TCustomUnicodeCodec read FOutputCodec;
    property OutputEncoding: WideString read FOutputEncoding write
      SetOutputEncoding;

    property OnProgress: TNotifyEvent read FOnProgress write FOnProgress;
  public
    constructor Create;
    destructor Destroy; override;
    procedure Transcode; virtual;
  end;

  TUtilsStandardTranscoder = class(TUtilsCustomTranscoder)
  private
    FOnRead: TCodecReadEvent;
    FOnWrite: TCodecWriteEvent;
  protected
    procedure CodecReadEventHandler(Sender: TObject;
      var Buf: TBytes;
      Count: Longint;
      var Ok: Boolean); override;
    procedure CodecWriteEventHandler(Sender: TObject;
      const Buf: TBytes;
      Count: Longint); override;
  public
    property Busy;
    property InputEncoding;
    property LineBreakOpt;
    property OutputEncoding;

    property OnProgress;
    property OnRead: TCodecReadEvent read FOnRead write FOnRead;
    property OnWrite: TCodecWriteEvent read FOnWrite write FOnWrite;
  end;

  TUtilsStreamTranscoder = class(TUtilsCustomTranscoder)
  private
    FReader: TUtilsCustomInputStream;
    FWriter: TUtilsCustomOutputStream;
  protected
    procedure CodecReadEventHandler(Sender: TObject;
      var Buf: TBytes;
      Count: Longint;
      var Ok: Boolean); override;
    procedure CodecWriteEventHandler(Sender: TObject;
      const Buf: TBytes;
      Count: Longint); override;
  public
    constructor Create(const InputStream, OutputStream: TStream;
      const BufSize: Integer);
    destructor Destroy; override;

    property Busy;
    property InputEncoding;
    property LineBreakOpt;
    property OutputEncoding;

    property OnProgress;
  end;

resourcestring
  SByteOrderMarkMismatch = 'Specified input encoding does not match byte order mark.';
  SEncodingNotSupported = 'Character encoding scheme not supported.';
  SInputEncodingNotSpecified = 'Input Encoding not specified';
  SOutputEncodingNotSpecified = 'Output Encoding not specified';
  SStreamNotSpecified = 'Stream not specified.';
  SStreamPositionOutOfBounds = 'Stream position (%u) out of bounds (maximum: %u)';

implementation

{ TUtilsCustomInputStream }

// TUtilsCustomInputStream was provided by Robert Marquardt.
// Modifications in TdomCustomReader.Read() by Dieter Köhler.

constructor TUtilsCustomInputStream.Create(const Stream: TStream;
  const BufSize: Integer);
begin
  if not Assigned(Stream) then
    raise EParserUtilsException.Create(SStreamNotSpecified);
  inherited Create;
  if Assigned(Stream) then
    FInitialStreamPosition := Stream.Position
  else
    FInitialStreamPosition := 0;
  FStream := Stream;
  FBufSize := BufSize;
  FPosition := 0;
  FRemaining := 0;
  SetLength(FBuffer, BufSize);
end;

destructor TUtilsCustomInputStream.Destroy;
begin
  FlushBuffer;
  inherited;
end;

procedure TUtilsCustomInputStream.FlushBuffer;
begin
  FStream.Position := FStream.Position - FRemaining;
  FRemaining := 0;
  FPosition := 0;
end;

function TUtilsCustomInputStream.GetPosition: Int64;
begin
  Result := FStream.Position - FRemaining - FInitialStreamPosition;
end;

function TUtilsCustomInputStream.Read(var Buf: array of Byte; const Count: Longint): Boolean; 
begin
  Result := True;

  if Count <= FRemaining then
  begin
    System.Array.Copy(FBuffer, FPosition, Buf, 0, Count);
    Inc(FPosition, Count);
    Dec(FRemaining, Count);
    Exit;
  end
  else
    FlushBuffer;

  if Count > FBufSize then
  begin
    if FStream.Read(Buf, Count) = 0 then
      Result := False;
    Exit;
  end;

  if FRemaining = 0 then
  begin
    FRemaining := FStream.Read(FBuffer, FBufSize);
    if FRemaining = 0 then
    begin
      Result := False;
      Exit;
    end;
    FPosition := 0;
  end;

  System.Array.Copy(FBuffer, FPosition, Buf, 0, Count);
  Inc(FPosition, Count);
  Dec(FRemaining, Count);
end;

procedure TUtilsCustomInputStream.SetPosition(Value: Int64);
begin
  FStream.Position := Value + FInitialStreamPosition;
  FRemaining := 0;
  FPosition := 0;
end;

{ TUtilsCustomOutputStream }

// TUtilsCustomOutputStream was provided by Robert Marquardt

constructor TUtilsCustomOutputStream.Create(const Stream: TStream;
  const BufSize: Integer);
begin
  if not Assigned(Stream) then
    raise EParserUtilsException.Create(SStreamNotSpecified);
  inherited Create;
  if Assigned(Stream) then
    FInitialStreamPosition := Stream.Position
  else
    FInitialStreamPosition := 0;
  FStream := Stream;
  SetLength(FBuffer, BufSize);
  FPosition := 0;
  FBufSize := BufSize;
end;

destructor TUtilsCustomOutputStream.Destroy;
begin
  FlushBuffer;
  inherited;
end;

procedure TUtilsCustomOutputStream.FlushBuffer;
begin
  if FPosition > High(Longint) then
    raise EParserUtilsException.CreateFmt(SStreamPositionOutOfBounds, [FPosition, High(Longint)]);
  FStream.Write(FBuffer, Longint(FPosition));
  FPosition := 0;
end;

function TUtilsCustomOutputStream.GetPosition: Int64;
begin
  Result := FStream.Position + FPosition - FInitialStreamPosition;
end;

procedure TUtilsCustomOutputStream.SetPosition(Value: Int64);
begin
  FlushBuffer;
  FStream.Position := Value + FInitialStreamPosition;
end;

procedure TUtilsCustomOutputStream.Write(const Buf: TBytes; const Count: Longint);
begin
  if FPosition + Count <= FBufSize then
  begin
    System.Array.Copy(Buf, 0, FBuffer, FPosition, Count);
    Inc(FPosition, Count);
    Exit;
  end
  else
    FlushBuffer;
  if Count > FBufSize then
    FStream.Write(Buf, Count)
  else
  begin
    System.Array.Copy(Buf, 0, FBuffer, FPosition, Count);
    Inc(FPosition, Count);
  end;
end;

{ TUtilsCustomAutodetectInputStream }

constructor TUtilsCustomAutodetectInputStream.Create(const Stream: TStream;
  const BufSize: Integer; const ExpectedEncoding: TUnicodeCodecClass);
var
  BOMType: TUnicodeCodecClass;
  BOMSize: Cardinal;
begin
  inherited Create(Stream, BufSize);
  FByteOrderMarkSize := 0;
  FCodec := nil;
  try
    FindBOM(Stream, BOMType, BOMSize);

    if  ExpectedEncoding = BOMType then begin

      FByteOrderMarkSize := BOMSize;
      SetCodec(ExpectedEncoding);

    end else if (ExpectedEncoding = TUTF16BECodec) or
                (ExpectedEncoding = TUCS2BECodec) then begin

      if BOMType = TUTF16LECodec then
        raise EConvertError.Create(SByteOrderMarkMismatch);
      if BOMType = TUTF16BECodec then
        FByteOrderMarkSize := BOMSize;

      SetCodec(ExpectedEncoding);

    end else if (ExpectedEncoding = TUTF16LECodec) or
                (ExpectedEncoding = TUCS2LECodec) then begin


      if BOMType = TUTF16BECodec then
        raise EConvertError.Create(SByteOrderMarkMismatch);
      if BOMType = TUTF16LECodec then
        FByteOrderMarkSize := BOMSize;

      SetCodec(ExpectedEncoding);

    end else if Assigned(ExpectedEncoding) then begin

      SetCodec(ExpectedEncoding);

    end else begin

      FByteOrderMarkSize := BOMSize;
      SetCodec(BOMType);

    end;

  finally
    Reset;
  end;
end;

procedure TUtilsCustomAutodetectInputStream.FindBOM(const Stream: TStream;
  out BOMType: TUnicodeCodecClass; out BOMSize: Cardinal);
var
  A: array[0..1] of Byte;
  // Do not use Word, because the byte order of a Word is CPU dependant.
  B: Byte;
  W: Word;
begin
  BOMType := nil;
  BOMSize := 0;
  with Stream do
  begin
    if Read(A, 2) = 2 then
      case A[0] of
        $00: if A[1] = $00 then
            if Read(A, 2) = 2 then
              if (A[0] = $FE) and (A[1] = $FF) then
              begin
                BOMType := TUCS4BECodec;
                BOMSize := 4;
              end
              else if (A[0] = $FF) and (A[1] = $FE) then
              begin
                BOMType := TUCS4_2143Codec;
                BOMSize := 4;
              end;
        $FE: if A[1] = $FF then
            if Read(W) = 2 then
            begin
              if W = $0000 then
              begin
                BOMType := TUCS4_3412Codec;
                BOMSize := 4;
              end
              else
              begin
                BOMType := TUTF16BECodec;
                BOMSize := 2;
              end;
            end
            else
            begin
              BOMType := TUTF16BECodec;
              BOMSize := 2;
            end;
        $FF: if (A[1] = $FE) then
            if Read(W) = 2 then
            begin
              if W = $0000 then
              begin
                BOMType := TUCS4LECodec;
                BOMSize := 4;
              end
              else
              begin
                BOMType := TUTF16LECodec;
                BOMSize := 2;
              end;
            end
            else
            begin
              BOMType := TUTF16LECodec;
              BOMSize := 2;
            end;
        $EF: if (A[1] = $BB) then
            if Read(B) = 1 then
              if B = $BF then
              begin
                BOMType := TUTF8Codec;
                BOMSize := 3;
              end;
      end;
  end;
end;

function TUtilsCustomAutodetectInputStream.GetPosition: Int64;
begin
  Result := inherited GetPosition - ByteOrderMarkSize;
end;

procedure TUtilsCustomAutodetectInputStream.Reset;
begin
  Position := 0;
end;

procedure TUtilsCustomAutodetectInputStream.SetCodec(Value: TUnicodeCodecClass);
begin
  FCodec := Value;
end;

procedure TUtilsCustomAutodetectInputStream.SetPosition(Value: Int64);
begin
  inherited SetPosition(Value + ByteOrderMarkSize);
end;

{ TUtilsAutodetectInputStream }

function TUtilsAutodetectInputStream.GetHasByteOrderMark: Boolean;
begin
  Result := ByteOrderMarkSize <> 0;
end;

{ TUtilsUCS4Reader }

constructor TUtilsUCS4Reader.Create(const Stream: TStream;
  const ABufSize: Integer; const ACodecClass: TUnicodeCodecClass;
  const InitialByteCount, InitialCharCount, InitialCharsInLine,
  InitialTabsInLine, InitialLine: Int64);
// Creation fails if the source stream's first character (if any) cannot be
// converted from the specified encoding to a UCS4 code point, because the
// call to FCodec.ReadUCS4Char() in the Reset procedure raises an EConvertError
// exception.
begin
  inherited Create;
  FCodec := nil;       // Remark: If an exception occurs, the destructor is automatically called.
  FInternalInputStream := nil; // Therefore, we need to initialize critical objects with nil first.
  with FInitialUCS4CharData do
  begin
    ByteCount := InitialByteCount;
    CharCount := InitialCharCount;
    CharsInLine := InitialCharsInLine;
    CodePoint := $98; // START OF STRING
    Line := InitialLine;
    Size := 0;
    TabsInLine := InitialTabsInLine;
  end;
  FResetPosition := 0;
  FReadLFOption := lrNormalize;
  // Remark: "FReadLFOption := lrNormalize" must appear before SetCodecClass!

  FInternalInputStream := TUtilsAutodetectInputStream.Create(Stream, ABufSize, ACodecClass);
  if Assigned(FInternalInputStream.Codec) then
    SetCodecClass(FInternalInputStream.Codec)
  else
    SetCodecClass(DefaultCodecClass);

  Reset;
end;

destructor TUtilsUCS4Reader.Destroy;
begin
  FCodec.Free;
  FInternalInputStream.Free;
  inherited;
end;

function TUtilsUCS4Reader.GetBof: Boolean;
begin
  Result := CurrentCharInfo.CodePoint = $98; // START OF STRING
end;

function TUtilsUCS4Reader.GetBufSize: Integer;
begin
  Result := InternalInputStream.BufSize;
end;

function TUtilsUCS4Reader.GetCodecClass: TUnicodeCodecClass;
begin
  if Assigned(FCodec) then
    Result := TUnicodeCodecClass(FCodec.ClassType)
  else
    Result := nil;
end;

function TUtilsUCS4Reader.GetDefaultCodecClass: TUnicodeCodecClass;
begin
  Result := TUTF8Codec;
end;

function TUtilsUCS4Reader.GetEof: Boolean;
begin
  Result := CurrentCharInfo.CodePoint = $9C; // STRING TERMINATOR
end;

function TUtilsUCS4Reader.GetHasByteOrderMark: Boolean;
begin
  Result := InternalInputStream.HasByteOrderMark;
end;

function TUtilsUCS4Reader.GetPosition: Int64;
begin
  Result := InternalInputStream.Position;
end;

function TUtilsUCS4Reader.Match(Ucs2Str: WideString): Boolean;
// Raises an EConvertError exception via the call to Next(), if the next
// character of the source stream cannot be converted (according to the
// source's character encoding scheme) to a UCS4 code point.
const
  STRING_TERMINATOR = $9C;
var
  I: Integer;
begin
  for I := 1 to Length(Ucs2Str) do
  begin
    Next;
    if CurrentCharInfo.CodePoint <> Ord(Ucs2Str[I]) then
    begin
      Result := False;
      Exit; // Missmatch found.
    end;
  end;
  Result := True;
end;

procedure TUtilsUCS4Reader.Next;
// Raises an EConvertError exception via the call to FCodec.ReadUCS4Char(),
// if the next character of the source stream cannot be converted (according
// to the source's character encoding scheme) to a UCS4 code point.
const
  STRING_TERMINATOR = $9C;
var
  TempNextCP: UCS4Char;
  TempSize: Integer;
begin
  if FCurrentUCS4Char.CodePoint = STRING_TERMINATOR then
    Exit;
  if FNextUCS4Char.CodePoint = STRING_TERMINATOR then
  begin
    FPreviousUCS4Char := FCurrentUCS4Char;
    FCurrentUCS4Char := FNextUCS4Char;
    Exit;
  end;
  FCodec.ReadUCS4Char(TempNextCP, TempSize);
  // We use a temporary code point, so that the properties of TUtilsUCS4Reader
  // remain unchanged if FCodec.ReadUCS4Char() raises an exception.
  FPreviousUCS4Char := FCurrentUCS4Char;
  FCurrentUCS4Char := FNextUCS4Char;
  with FNextUCS4Char do
  begin
    Inc(CharCount, 1);
    CodePoint := TempNextCP;
    Inc(ByteCount, TempSize);
    Size := TempSize;
  end;
  UpdateLocator(FNextUCS4Char);
end;

procedure TUtilsUCS4Reader.ReadEventHandler(Sender: TObject; var Buf: array of Byte; Count:  
  Longint; var Ok: Boolean);
begin
  Ok := InternalInputStream.Read(Buf, Count);
end;

procedure TUtilsUCS4Reader.Reset;
var
  TempNextCP: UCS4Char;
  TempSize: Integer;
begin
  InternalInputStream.Position := ResetPosition;
  FPreviousUCS4Char := InitialUCS4CharData;
  FCurrentUCS4Char := InitialUCS4CharData;
  FNextUCS4Char := InitialUCS4CharData;

  FCodec.ReadUCS4Char(TempNextCP, TempSize);

  with FNextUCS4Char do
  begin
    Inc(CharCount);
    CodePoint := TempNextCP;
    Inc(ByteCount, TempSize);
    Size := TempSize;
  end;
  UpdateLocator(FNextUCS4Char);
end;

procedure TUtilsUCS4Reader.SetCodecClass(const Value: TUnicodeCodecClass);
begin
  if Assigned(FCodec) then
    FCodec.Free;
  if Assigned(Value) then
  begin
    FCodec := Value.Create;
    FCodec.ReadLFOption := FReadLFOption;
    FCodec.OnRead := ReadEventHandler;
  end
  else
    FCodec := nil;
end;

procedure TUtilsUCS4Reader.SetReadLFOption(
  const Value: TCodecReadLFOption);
begin
  FReadLFOption := Value;
  if Assigned(FCodec) then
    FCodec.ReadLFOption := FReadLFOption;
end;

procedure TUtilsUCS4Reader.SetResetPosition(const Value: Int64);
begin
  FResetPosition := Value;
end;

function TUtilsUCS4Reader.SkipNext(Ucs2Str: WideString): Integer;
// Raises an EConvertError exception via the call to Next(), if the next
// character of the source stream cannot be converted (according to the
// source's character encoding scheme) to a UCS4 code point.
const
  STRING_TERMINATOR = $9C;
var
  I: Integer;
begin
  Result := 0;
  Next;
  if CurrentCharInfo.CodePoint = STRING_TERMINATOR then
    Exit;
  for I := 1 to Length(Ucs2Str) do
    if CurrentCharInfo.CodePoint = Ord(Ucs2Str[I]) then
    begin
      Result := SkipNext(Ucs2Str);
      Inc(Result);
      Exit;
    end;
end;

procedure TUtilsUCS4Reader.UpdateLocator(var UCS4CharData: TUtilsUCS4CharData);
begin
  with UCS4CharData do
    case CodePoint of
      $A:
        begin // LF
          Inc(Line);
          CharsInLine := 0;
          TabsInLine := 0;
        end;
      $9:
        begin // TAB
          Inc(CharsInLine);
          Inc(TabsInLine);
        end;
    else
      Inc(CharsInLine);
    end;
end;

{ TUtilsCustomTranscoder }

procedure TUtilsCustomTranscoder.CodecReadEventHandler(Sender: TObject;
  var Buf: array of Byte; Count: Integer; var Ok: Boolean);  
begin
  Ok := False; // By default return False;
end;

procedure TUtilsCustomTranscoder.CodecWriteEventHandler(Sender: TObject;
  const Buf: array of Byte; Count: Integer);  
begin
  // By default do nothing.
end;

constructor TUtilsCustomTranscoder.Create;
begin
  inherited;
  FBusy := False;
  FLineBreakOpt := lbNone;
  UpdateLineBreakOpt;
end;

destructor TUtilsCustomTranscoder.Destroy;
begin
  FInputCodec.Free;
  FOutputCodec.Free;
  inherited Destroy;
end;

procedure TUtilsCustomTranscoder.DoProgress;
begin
  if Assigned(FOnProgress) then
    FOnProgress(Self);
end;

procedure TUtilsCustomTranscoder.SetInputEncoding(const Value: WideString);
var
  NewCodecClass: TUnicodeCodecClass;
begin
  if Value = '' then
  begin
    FInputEncoding := '';
    FInputCodec.Free;
    FInputCodec := nil;
  end
  else
  begin
    NewCodecClass := StrToEncoding(Value);
    if Assigned(NewCodecClass) then
    begin
      FInputCodec.Free;
      FInputCodec := NewCodecClass.Create;
      FInputCodec.OnRead := CodecReadEventHandler;
      FInputEncoding := Value;
      UpdateLineBreakOpt;
    end
    else
      raise EParserUtilsException.create(SEncodingNotSupported);
  end; {if value = '' else ...}
end;

procedure TUtilsCustomTranscoder.SetLineBreakOpt(const Value:
  TUtilsLineBreakOpt);
begin
  if (FLineBreakOpt <> Value) and not FBusy then
  begin
    FLineBreakOpt := Value;
    UpdateLineBreakOpt;
  end;
end;

procedure TUtilsCustomTranscoder.SetOutputEncoding(const Value: WideString);
var
  NewCodecClass: TUnicodeCodecClass;
begin
  if Value = '' then
  begin
    FOutputEncoding := '';
    FOutputCodec.Free;
    FOutputCodec := nil;
  end
  else
  begin
    NewCodecClass := StrToEncoding(Value);
    if Assigned(NewCodecClass) then
    begin
      FOutputCodec.Free;
      FOutputCodec := NewCodecClass.Create;
      FOutputCodec.OnWrite := CodecWriteEventHandler;
      FOutputEncoding := Value;
      UpdateLineBreakOpt;
    end
    else
      raise EParserUtilsException.create(SEncodingNotSupported);
  end; {if value = '' else ...}
end;

procedure TUtilsCustomTranscoder.Transcode;
var
  C: UCS4Char;
  ByteCount: Integer; // Dummy
begin
  if not Assigned(FInputCodec) then
    raise EParserUtilsException.Create(SInputEncodingNotSpecified);
  if not Assigned(FOutputCodec) then
    raise EParserUtilsException.Create(SOutputEncodingNotSpecified);
  FBusy := True;
  try
    while True do
    begin
      FInputCodec.ReadUCS4Char(C, ByteCount);
      if C = $9C then
        Exit; // $9C = STRING TERMINATOR
      FOutputCodec.WriteUCS4Char(C, ByteCount);
      DoProgress;
    end;
  finally
    FBusy := False;
  end;
end;

procedure TUtilsCustomTranscoder.UpdateLineBreakOpt;
begin
  case FLineBreakOpt of
    lbCRLF:
      begin
        if Assigned(FInputCodec) then
          FInputCodec.ReadLFOption := lrNormalize;
        if Assigned(FOutputCodec) then
          FOutputCodec.WriteLFOption := lwCRLF;
      end;
    lbCR:
      begin
        if Assigned(FInputCodec) then
          FInputCodec.ReadLFOption := lrNormalize;
        if Assigned(FOutputCodec) then
          FOutputCodec.WriteLFOption := lwCR;
      end;
    lbLF:
      begin
        if Assigned(FInputCodec) then
          FInputCodec.ReadLFOption := lrNormalize;
        if Assigned(FOutputCodec) then
          FOutputCodec.WriteLFOption := lwLF;
      end;
    lbNone:
      begin
        if Assigned(FInputCodec) then
          FInputCodec.ReadLFOption := lrPass;
        if Assigned(FOutputCodec) then
          FOutputCodec.WriteLFOption := lwLF;
      end;
  end;
end;

{ TUtilsStandardTranscoder }

procedure TUtilsStandardTranscoder.CodecReadEventHandler(Sender: TObject;
  var Buf: TBytes; Count: Integer; var Ok: Boolean);
begin
  if Assigned(FOnRead) then
    FOnRead(Self, Buf, Count, Ok)
  else
    inherited;
end;

procedure TUtilsStandardTranscoder.CodecWriteEventHandler(Sender: TObject;
  const Buf: TBytes; Count: Integer);
begin
  if Assigned(FOnWrite) then
    FOnWrite(Self, Buf, Count);
end;

{ TUtilsStreamTranscoder }

procedure TUtilsStreamTranscoder.CodecReadEventHandler(Sender: TObject;
  var Buf: TBytes; Count: Integer; var Ok: Boolean);
begin
  Ok := FReader.Read(Buf, Count);
end;

procedure TUtilsStreamTranscoder.CodecWriteEventHandler(Sender: TObject;
  const Buf: TBytes; Count: Integer);
begin
  FWriter.Write(Buf, Count);
end;

constructor TUtilsStreamTranscoder.Create(const InputStream,
  OutputStream: TStream; const BufSize: Integer);
begin
  inherited Create;
  FReader := TUtilsCustomInputStream.Create(InputStream, BufSize);
  FWriter := TUtilsCustomOutputStream.Create(OutputStream, BufSize);
end;

destructor TUtilsStreamTranscoder.Destroy;
begin
  FReader.Free;
  FWriter.Free;
  inherited;
end;

end.

