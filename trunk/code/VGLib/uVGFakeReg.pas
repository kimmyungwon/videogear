unit uVGFakeReg;

interface

uses
  Windows, SysUtils, ActiveX, JclPeImage;

type
  TVGFakeRegister = class
  private
    FHooks: TJclPeMapImgHooks;
  public
    constructor Create;
    destructor Destroy; override;

    function LoadLibrary(const ALibFile: WideString): THandle;
    procedure FreeLibrary(AModule: THandle);
  end;

implementation

uses JclDebug;

function MyCoCreateInstance(const clsid: TCLSID; unkOuter: IUnknown;
  dwClsContext: Longint; const iid: TIID; out pv): HResult; stdcall;
begin
  TraceMsg('CoCreateInstance');
  Result := CoCreateInstance(clsid, unkOuter, dwClsContext, iid, pv);
end;

function MyRegOpenKeyA(hKey: HKEY; lpSubKey: PAnsiChar; var phkResult: HKEY): Longint; stdcall;
begin
  TraceMsg('RegOpenKeyA');
  Result := RegOpenKeyA(hKey, lpSubKey, phkResult);
end;

function MyRegOpenKeyW(hKey: HKEY; lpSubKey: PWideChar; var phkResult: HKEY): Longint; stdcall;
begin
  TraceMsg('RegOpenKeyW');
  Result := RegOpenKeyW(hKey, lpSubKey, phkResult);
end;

function MyRegOpenKeyExA(hKey: HKEY; lpSubKey: PAnsiChar;
  ulOptions: DWORD; samDesired: REGSAM; var phkResult: HKEY): Longint; stdcall;
begin
  TraceMsg('RegOpenKeyExA');
  Result := RegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);
end;

function MyRegOpenKeyExW(hKey: HKEY; lpSubKey: PWideChar;
  ulOptions: DWORD; samDesired: REGSAM; var phkResult: HKEY): Longint; stdcall;
begin
  TraceMsg('RegOpenKeyExW');
  Result := RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
end;

function MyRegQueryValueExA(hKey: HKEY; lpValueName: PAnsiChar;
  lpReserved: Pointer; lpType: PDWORD; lpData: PByte; lpcbData: PDWORD): Longint; stdcall;
begin
  TraceMsg('RegQueryValueExA');
  Result := RegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
end;

{ TVGFakeRegister }

constructor TVGFakeRegister.Create;
begin
  FHooks := TJclPeMapImgHooks.Create;
end;

destructor TVGFakeRegister.Destroy;
begin
  FHooks.Free;
  inherited;
end;

procedure TVGFakeRegister.FreeLibrary(AModule: THandle);
var
  Idx: Integer;
begin
  Idx := 0;
  while Idx < FHooks.Count do
  begin
    if Cardinal(FHooks[Idx].BaseAddress) = AModule then
      FHooks[Idx].Unhook
    else
      Inc(Idx);
  end;
  FreeLibrary(AModule);
end;

function TVGFakeRegister.LoadLibrary(const ALibFile: WideString): THandle;
var
  pDummy: Pointer;
begin
  Result := LoadLibraryW(PWideChar(ALibFile));
  if Result = 0 then
    RaiseLastOSError;
  FHooks.HookImport(Pointer(Result), 'ole32.dll', 'CoCreateInstance', @MyCoCreateInstance, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegOpenKeyA', @MyRegOpenKeyA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegOpenKeyW', @MyRegOpenKeyW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegOpenKeyExA', @MyRegOpenKeyExA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegOpenKeyExW', @MyRegOpenKeyExW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegQueryValueExA', @MyRegQueryValueExA, pDummy);
end;

end.
