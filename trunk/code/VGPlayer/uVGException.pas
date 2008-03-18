unit uVGException;

interface

uses
  Classes, uAriaException, DirectShow9;

type
  EVGError = class(EAriaError)
  protected
    function SysErrorToString(AHR: HRESULT): WideString; override;  
  end;  

implementation

{ EVGError }

function EVGError.SysErrorToString(AHR: HRESULT): WideString;
var
  szMsg: array[0..MAX_ERROR_TEXT_LEN] of WideChar;
begin
  AMGetErrorTextW(AHR, szMsg, MAX_ERROR_TEXT_LEN);
  Result := szMsg;
end;

end.
