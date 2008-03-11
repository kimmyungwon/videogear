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

var
  gFakeReg: TVGFakeRegister;

implementation

uses JclDebug;

function MyCoCreateInstance(const clsid: TCLSID; unkOuter: IUnknown;
  dwClsContext: Longint; const iid: TIID; out pv): HResult; stdcall;
begin
  TraceMsg('CoCreateInstance');
  Result := CoCreateInstance(clsid, unkOuter, dwClsContext, iid, pv);
end;

function MyLoadLibraryA(lpLibFileName: PAnsiChar): HMODULE; stdcall;
begin
  TraceMsg('LoadLibraryA');
  Result := LoadLibraryA(lpLibFileName);
end;

function MyLoadLibraryW(lpLibFileName: PWideChar): HMODULE; stdcall;
begin
  TraceMsg('LoadLibraryW');
  Result := LoadLibraryW(lpLibFileName);
end;

function MyRegCreateKeyA(hKey: HKEY; lpSubKey: PAnsiChar;
  var phkResult: HKEY): Longint; stdcall;
begin
  TraceMsg('RegCreateKeyA');
  Result := RegCreateKeyA(hKey, lpSubKey, phkResult);
end;

function MyRegCreateKeyW(hKey: HKEY; lpSubKey: PWideChar;
  var phkResult: HKEY): Longint; stdcall;
begin
  TraceMsg('RegCreateKeyW');
  Result := RegCreateKeyW(hKey, lpSubKey, phkResult);
end;

function MyRegCreateKeyExA(hKey: HKEY; lpSubKey: PAnsiChar;
  Reserved: DWORD; lpClass: PAnsiChar; dwOptions: DWORD; samDesired: REGSAM;
  lpSecurityAttributes: PSecurityAttributes; var phkResult: HKEY;
  lpdwDisposition: PDWORD): Longint; stdcall;
begin
  TraceMsg('RegCreateKeyExA');
  Result := RegCreateKeyExA(hKey, lpSubKey, Reserved, lpClass, dwOptions,
    samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
end;

function MyRegCreateKeyExW(hKey: HKEY; lpSubKey: PWideChar;
  Reserved: DWORD; lpClass: PWideChar; dwOptions: DWORD; samDesired: REGSAM;
  lpSecurityAttributes: PSecurityAttributes; var phkResult: HKEY;
  lpdwDisposition: PDWORD): Longint; stdcall;
begin
  TraceMsg('RegCreateKeyExW');
  Result := RegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions,
    samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
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

  //FHooks.HookImport(Pointer(Result), 'ole32.dll', 'CoCreateInstance', @MyCoCreateInstance, pDummy);
  FHooks.HookImport(Pointer(Result), kernel32, 'LoadLibraryA', @MyLoadLibraryA, pDummy);
  FHooks.HookImport(Pointer(Result), kernel32, 'LoadLibraryW', @MyLoadLibraryW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegCreateKeyA', @MyRegCreateKeyA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegCreateKeyW', @MyRegCreateKeyW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegCreateKeyExA', @MyRegCreateKeyExA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegCreateKeyExW', @MyRegCreateKeyExW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegOpenKeyA', @MyRegOpenKeyA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegOpenKeyW', @MyRegOpenKeyW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegOpenKeyExA', @MyRegOpenKeyExA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegOpenKeyExW', @MyRegOpenKeyExW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegQueryValueExA', @MyRegQueryValueExA, pDummy);
end;

initialization
  gFakeReg := TVGFakeRegister.Create;

finalization
  FreeAndNil(gFakeReg);

end.
