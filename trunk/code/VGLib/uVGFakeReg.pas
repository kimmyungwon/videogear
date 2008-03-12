unit uVGFakeReg;

interface

uses
  Windows, SysUtils, ActiveX, JclPeImage, uAriaTree;

type
  TVGFakeRegister = class(TAriaTree)
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

const
  HKEY_FAKEREG    = Cardinal($C0000000);

function HKEYToStr(hKey: HKEY): WideString; inline;
begin
  case hKey of
    HKEY_CLASSES_ROOT: Result := 'HKEY_CLASSES_ROOT';
    HKEY_CURRENT_USER: Result := 'HKEY_CURRENT_USER';
    HKEY_LOCAL_MACHINE: Result := 'HKEY_LOCAL_MACHINE';
    HKEY_USERS: Result := 'HKEY_USERS';
    HKEY_PERFORMANCE_DATA: Result := 'HKEY_PERFORMANCE_DATA';
    HKEY_CURRENT_CONFIG: Result := 'HKEY_CURRENT_CONFIG';
    HKEY_DYN_DATA: Result := 'HKEY_DYN_DATA';
  else
    Result := WideFormat('%.8X', [hKey]);
  end;
end;

function IsFakeNode(hKey: HKEY): Boolean; inline;
begin
  Result := (hKey and HKEY_FAKEREG) = HKEY_FAKEREG;
end;

function HKEYToNode(hKey: HKEY): TAriaNode; inline;
begin
  if IsFakeNode(hKey) then
    Result := TAriaNode(hKey and not HKEY_FAKEREG)
  else
    Result := nil;
end;

function NodeToHKEY(ANode: TAriaNode): HKEY; inline;
begin
  Result := (Cardinal(ANode) or HKEY_FAKEREG);
end;

{ HOOKs }

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

function MyRegCloseKey(hKey: HKEY): Longint; stdcall;
begin
  TraceMsg('RegCloseKey');
  Result := RegCloseKey(hKey);
end;

function MyRegCreateKeyW(hKey: HKEY; lpSubKey: PWideChar;
  var phkResult: HKEY): Longint; stdcall;
var
  Node: TAriaNode;
begin
  TraceMsg('RegCreateKeyW');
  if (hKey and $80000000) <> 0 then
  begin
    if not IsFakeNode(hKey) then  // 打开的是根节点
    begin
      Node := gFakeReg.Children.FindSibling(HKEYToStr(hKey));
      if Node = nil then
      begin
        Result := ERROR_INTERNAL_ERROR;
        Exit;
      end;
    end
    else // 打开的是HOOK过后的子节点
    begin
      Node := HKEYToNode(hKey); 
    end;
    // 按lpSubKey建立路径
    Node := Node.CreatePath(lpSubKey);
    // 返回
    phkResult := (Cardinal(Node) or HKEY_FAKEREG);
    Result := S_OK;
  end
  else
    Result := RegCreateKeyW(hKey, lpSubKey, phkResult);
end;

function MyRegCreateKeyA(hKey: HKEY; lpSubKey: PAnsiChar;
  var phkResult: HKEY): Longint; stdcall;
begin
  TraceMsg('RegCreateKeyA');
  Result := MyRegCreatekeyW(hKey, PWideChar(WideString(lpSubKey)), phkResult);
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

function MyRegDeleteKeyA(hKey: HKEY; lpSubKey: PAnsiChar): Longint; stdcall;
begin
  TraceMsg('RegDeleteKeyA');
  Result := RegDeleteKeyA(hKey, lpSubKey);
end;

function MyRegDeleteKeyW(hKey: HKEY; lpSubKey: PWideChar): Longint; stdcall;
begin
  TraceMsg('RegDeleteKeyW');
  Result := RegDeleteKeyW(hKey, lpSubKey);
end;

function MyRegDeleteValueA(hKey: HKEY; lpValueName: PAnsiChar): Longint; stdcall;
begin
  TraceMsg('RegDeleteValueA');
  Result := RegDeleteValueA(hKey, lpValueName);
end;

function MyRegDeleteValueW(hKey: HKEY; lpValueName: PWideChar): Longint; stdcall;
begin
  TraceMsg('RegDeleteValueW');
  Result := RegDeleteValueW(hKey, lpValueName);
end;

function MyRegEnumKeyA(hKey: HKEY; dwIndex: DWORD; lpName: PAnsiChar; cbName: DWORD): Longint; stdcall;
begin
  TraceMsg('MyRegEnumKeyA');
  Result := MyRegEnumKeyA(hKey, dwIndex, lpName, cbName);
end;

function MyRegEnumKeyW(hKey: HKEY; dwIndex: DWORD; lpName: PWideChar; cbName: DWORD): Longint; stdcall;
begin
  TraceMsg('MyRegEnumKeyW');
  Result := MyRegEnumKeyW(hKey, dwIndex, lpName, cbName);
end;

function MyRegEnumKeyExA(hKey: HKEY; dwIndex: DWORD; lpName: PAnsiChar;
  var lpcbName: DWORD; lpReserved: Pointer; lpClass: PAnsiChar;
  lpcbClass: PDWORD; lpftLastWriteTime: PFileTime): Longint; stdcall;
begin
  TraceMsg('RegEnumKeyExA');
  Result := RegEnumKeyExA(hKey, dwIndex, lpName, lpcbName, lpReserved,
    lpClass, lpcbClass, lpftLastWriteTime);
end;

function MyRegEnumKeyExW(hKey: HKEY; dwIndex: DWORD; lpName: PWideChar;
  var lpcbName: DWORD; lpReserved: Pointer; lpClass: PWideChar;
  lpcbClass: PDWORD; lpftLastWriteTime: PFileTime): Longint; stdcall;
begin
  TraceMsg('RegEnumKeyExW');
  Result := RegEnumKeyExW(hKey, dwIndex, lpName, lpcbName, lpReserved,
    lpClass, lpcbClass, lpftLastWriteTime);
end;

function MyRegEnumValueA(hKey: HKEY; dwIndex: DWORD; lpValueName: PAnsiChar;
  var lpcbValueName: DWORD; lpReserved: Pointer; lpType: PDWORD;
  lpData: PByte; lpcbData: PDWORD): Longint; stdcall;
begin
  TraceMsg('RegEnumValueA');
  Result := RegEnumValueA(hKey, dwIndex, lpValueName, lpcbValueName, lpReserved,
    lpType, lpData, lpcbData);
end;

function MyRegEnumValueW(hKey: HKEY; dwIndex: DWORD; lpValueName: PWideChar;
  var lpcbValueName: DWORD; lpReserved: Pointer; lpType: PDWORD;
  lpData: PByte; lpcbData: PDWORD): Longint; stdcall;
begin
  TraceMsg('RegEnumValueW');
  Result := RegEnumValueW(hKey, dwIndex, lpValueName, lpcbValueName, lpReserved,
    lpType, lpData, lpcbData);
end;

function MyRegFlushKey(hKey: HKEY): Longint; stdcall;
begin
  TraceMsg('RegFlushKey');
  Result := RegFlushKey(hKey);
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

function MyRegQueryValueA(hKey: HKEY; lpSubKey: PAnsiChar;
  lpValue: PAnsiChar; var lpcbValue: Longint): Longint; stdcall;
begin
  TraceMsg('RegQueryValueA');
  Result := RegQueryValueA(hKey, lpSubKey, lpValue, lpcbValue);
end;

function MyRegQueryValueW(hKey: HKEY; lpSubKey: PWideChar;
  lpValue: PWideChar; var lpcbValue: Longint): Longint; stdcall;
begin
  TraceMsg('RegQueryValueW');
  Result := RegQueryValueW(hKey, lpSubKey, lpValue, lpcbValue);
end;

function MyRegQueryMultipleValuesA(hKey: HKEY; var ValList;
  NumVals: DWORD; lpValueBuf: PAnsiChar; var ldwTotsize: DWORD): Longint; stdcall;
begin
  TraceMsg('RegQueryMultipleValuesA');
  Result := RegQueryMultipleValuesA(hKey, ValList, NumVals, lpValueBuf, ldwTotsize);
end;

function MyRegQueryMultipleValuesW(hKey: HKEY; var ValList;
  NumVals: DWORD; lpValueBuf: PWideChar; var ldwTotsize: DWORD): Longint; stdcall;
begin
  TraceMsg('RegQueryMultipleValuesW');
  Result := RegQueryMultipleValuesW(hKey, ValList, NumVals, lpValueBuf, ldwTotsize);
end;

function MyRegQueryValueExA(hKey: HKEY; lpValueName: PAnsiChar;
  lpReserved: Pointer; lpType: PDWORD; lpData: PByte; lpcbData: PDWORD): Longint; stdcall;
begin
  TraceMsg('RegQueryValueExA');
  Result := RegQueryValueExA(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
end;

function MyRegQueryValueExW(hKey: HKEY; lpValueName: PWideChar;
  lpReserved: Pointer; lpType: PDWORD; lpData: PByte; lpcbData: PDWORD): Longint; stdcall;
begin
  TraceMsg('RegQueryValueExW');
  Result := RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
end;

function MyRegSetValueW(hKey: HKEY; lpSubKey: PWideChar;
  dwType: DWORD; lpData: PWideChar; cbData: DWORD): Longint; stdcall;
var
  Node: TAriaNode;
begin
  TraceMsg('RegSetValueW');
  if IsFakeNode(hKey) then
  begin
    Node := HKEYToNode(hKey);
    Node.Value := WideString(lpData);
    Result := S_OK;
  end
  else
    Result := RegSetValueW(hKey, lpSubKey, dwType, lpData, cbData);
end;

function MyRegSetValueA(hKey: HKEY; lpSubKey: PAnsiChar;
  dwType: DWORD; lpData: PAnsiChar; cbData: DWORD): Longint; stdcall;
begin
  TraceMsg('RegSetValueA');
  Result := MyRegSetValueW(hKey, PWideChar(WideString(lpSubKey)), dwType,
    PWideChar(WideString(lpData)), cbData);
end;

function MyRegSetValueExA(hKey: HKEY; lpValueName: PAnsiChar;
  Reserved: DWORD; dwType: DWORD; lpData: Pointer; cbData: DWORD): Longint; stdcall;
begin
  TraceMsg('RegSetValueExA');
  Result := RegSetValueExA(hKey, lpValueName, Reserved, dwType, lpData, cbData);
end;

function MyRegSetValueExW(hKey: HKEY; lpValueName: PWideChar;
  Reserved: DWORD; dwType: DWORD; lpData: Pointer; cbData: DWORD): Longint; stdcall;
begin
  TraceMsg('RegSetValueExW');
  Result := RegSetValueExW(hKey, lpValueName, Reserved, dwType, lpData, cbData);
end;

{ TVGFakeRegister }

constructor TVGFakeRegister.Create;
var
  hKey: Windows.HKEY;
begin
  inherited Create;
  FHooks := TJclPeMapImgHooks.Create;
  // 初始化根节点
  for hKey := HKEY_CLASSES_ROOT to HKEY_DYN_DATA do
    Children.Add(HKEYToStr(hKey));
end;

destructor TVGFakeRegister.Destroy;
begin
  FreeAndNil(FHooks);
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
  //FHooks.HookImport(Pointer(Result), kernel32, 'LoadLibraryA', @MyLoadLibraryA, pDummy);
  //FHooks.HookImport(Pointer(Result), kernel32, 'LoadLibraryW', @MyLoadLibraryW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegCloseKey', @MyRegCloseKey, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegCreateKeyA', @MyRegCreateKeyA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegCreateKeyW', @MyRegCreateKeyW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegCreateKeyExA', @MyRegCreateKeyExA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegCreateKeyExW', @MyRegCreateKeyExW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegDeleteKeyA', @MyRegDeleteKeyA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegDeleteKeyW', @MyRegDeleteKeyW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegDeleteValueA', @MyRegDeleteValueA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegDeleteValueW', @MyRegDeleteValueW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegEnumKeyA', @MyRegEnumKeyA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegEnumKeyW', @MyRegEnumKeyW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegEnumKeyExA', @MyRegEnumKeyExA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegEnumKeyExW', @MyRegEnumKeyExW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegEnumValueA', @MyRegEnumValueA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegEnumValueW', @MyRegEnumValueW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegFlushKey', @MyRegFlushKey, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegOpenKeyA', @MyRegOpenKeyW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegOpenKeyW', @MyRegOpenKeyW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegOpenKeyExA', @MyRegOpenKeyExA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegOpenKeyExW', @MyRegOpenKeyExW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegQueryValueA', @MyRegQueryValueA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegQueryValueW', @MyRegQueryValueW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegQueryMultipleValuesA', @MyRegQueryMultipleValuesA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegQueryMultipleValuesW', @MyRegQueryMultipleValuesW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegQueryValueExA', @MyRegQueryValueExA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegQueryValueExW', @MyRegQueryValueExW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegSetValueA', @MyRegSetValueA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegSetValueW', @MyRegSetValueW, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegSetValueExA', @MyRegSetValueExA, pDummy);
  FHooks.HookImport(Pointer(Result), advapi32, 'RegSetValueExW', @MyRegSetValueExW, pDummy);
end;

initialization
  gFakeReg := TVGFakeRegister.Create;

finalization
  FreeAndNil(gFakeReg);

end.
