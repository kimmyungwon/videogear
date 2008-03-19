unit uVGException;

interface

uses
  Classes, DirectShow9, TntSysUtils;

type
  EVGError = class(WideException)
  protected
    function SysErrorToString(hr: HRESULT): WideString;
  public
    constructor CreateOSError(AErrorCode: Integer);
  end;  

implementation

{ EVGError }

constructor EVGError.CreateOSError(AErrorCode: Integer);
begin
  inherited Create(SysErrorToString(AErrorCode));
end;

function EVGError.SysErrorToString(hr: HRESULT): WideString;
var
  szMsg: array[0..MAX_ERROR_TEXT_LEN] of WideChar;
begin
  AMGetErrorTextW(hr, szMsg, MAX_ERROR_TEXT_LEN);
  Result := szMsg;
end;

end.
