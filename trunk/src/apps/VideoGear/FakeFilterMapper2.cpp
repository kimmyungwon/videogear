#include "stdafx.h"
#include "FakeFilterMapper2.h"
#include <APIHook.h>

HRESULT 
(STDAPICALLTYPE *Real_CoCreateInstance) (
	__in     REFCLSID rclsid, 
   __in_opt LPUNKNOWN pUnkOuter,
   __in     DWORD dwClsContext, 
   __in     REFIID riid, 
   __deref_out LPVOID FAR* ppv
   ) = CoCreateInstance;

LSTATUS
(APIENTRY *Real_RegCloseKey) (
    __in HKEY hKey
    ) = RegCloseKey;

LSTATUS
(APIENTRY *Real_RegOverridePredefKey) (
    __in HKEY hKey,
    __in_opt HKEY hNewHKey
    ) = RegOverridePredefKey;

LSTATUS
(APIENTRY *Real_RegOpenUserClassesRoot)(
    __in HANDLE hToken,
    __reserved DWORD dwOptions,
    __in REGSAM samDesired,
    __out PHKEY  phkResult
    ) = RegOpenUserClassesRoot;

LSTATUS
(APIENTRY *Real_RegOpenCurrentUser)(
    __in REGSAM samDesired,
    __out PHKEY phkResult
    ) = RegOpenCurrentUser;

LSTATUS
(APIENTRY *Real_RegDisablePredefinedCache)(
    VOID
    ) = RegDisablePredefinedCache;

LSTATUS
(APIENTRY *Real_RegDisablePredefinedCacheEx)(
    VOID
    ) = RegDisablePredefinedCacheEx;

LSTATUS
(APIENTRY *Real_RegConnectRegistryA) (
    __in_opt LPCSTR lpMachineName,
    __in HKEY hKey,
    __out PHKEY phkResult
    ) = RegConnectRegistryA;
LSTATUS
(APIENTRY *Real_RegConnectRegistryW) (
    __in_opt LPCWSTR lpMachineName,
    __in HKEY hKey,
    __out PHKEY phkResult
    ) = RegConnectRegistryW;

#if _WIN32_WINNT >= 0x0600

LSTATUS
(APIENTRY *Real_RegConnectRegistryExA) (
    __in_opt LPCSTR lpMachineName,
    __in HKEY hKey,
    __in ULONG Flags,
    __out PHKEY phkResult
    ) = RegConnectRegistryExA;
LSTATUS
(APIENTRY *Real_RegConnectRegistryExW) (
    __in_opt LPCWSTR lpMachineName,
    __in HKEY hKey,
    __in ULONG Flags,
    __out PHKEY phkResult
    ) = RegConnectRegistryExW;

#endif // _WIN32_WINNT >= 0x0600

LSTATUS
(APIENTRY *Real_RegCreateKeyA) (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __out PHKEY phkResult
    ) = RegCreateKeyA;
LSTATUS
(APIENTRY *Real_RegCreateKeyW) (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __out PHKEY phkResult
    ) = RegCreateKeyW;

LSTATUS
(APIENTRY *Real_RegCreateKeyExA) (
    __in HKEY hKey,
    __in LPCSTR lpSubKey,
    __reserved DWORD Reserved,
    __in_opt LPSTR lpClass,
    __in DWORD dwOptions,
    __in REGSAM samDesired,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __out PHKEY phkResult,
    __out_opt LPDWORD lpdwDisposition
    ) = RegCreateKeyExA;
LSTATUS
(APIENTRY *Real_RegCreateKeyExW) (
    __in HKEY hKey,
    __in LPCWSTR lpSubKey,
    __reserved DWORD Reserved,
    __in_opt LPWSTR lpClass,
    __in DWORD dwOptions,
    __in REGSAM samDesired,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __out PHKEY phkResult,
    __out_opt LPDWORD lpdwDisposition
    ) = RegCreateKeyExW;

#if _WIN32_WINNT >= 0x0600

LSTATUS
(APIENTRY *Real_RegCreateKeyTransactedA) (
    __in HKEY hKey,
    __in LPCSTR lpSubKey,
    __reserved DWORD Reserved,
    __in_opt LPSTR lpClass,
    __in DWORD dwOptions,
    __in REGSAM samDesired,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __out PHKEY phkResult,
    __out_opt LPDWORD lpdwDisposition,
    __in        HANDLE hTransaction,
    __reserved PVOID  pExtendedParemeter
    ) = RegCreateKeyTransactedA;
LSTATUS
(APIENTRY *Real_RegCreateKeyTransactedW) (
    __in HKEY hKey,
    __in LPCWSTR lpSubKey,
    __reserved DWORD Reserved,
    __in_opt LPWSTR lpClass,
    __in DWORD dwOptions,
    __in REGSAM samDesired,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __out PHKEY phkResult,
    __out_opt LPDWORD lpdwDisposition,
    __in        HANDLE hTransaction,
    __reserved PVOID  pExtendedParemeter
    ) = RegCreateKeyTransactedW;

#endif // _WIN32_WINNT >= 0x0600

LSTATUS
(APIENTRY *Real_RegDeleteKeyA) (
    __in HKEY hKey,
    __in LPCSTR lpSubKey
    ) = RegDeleteKeyA;
LSTATUS
(APIENTRY *Real_RegDeleteKeyW) (
    __in HKEY hKey,
    __in LPCWSTR lpSubKey
    ) = RegDeleteKeyW;

#if _WIN32_WINNT >= 0x0600

LSTATUS
(APIENTRY *Real_RegDeleteKeyExA) (
    __in HKEY hKey,
    __in LPCSTR lpSubKey,
    __in REGSAM samDesired,
    __reserved DWORD Reserved
    ) = RegDeleteKeyExA;
LSTATUS
(APIENTRY *Real_RegDeleteKeyExW) (
    __in HKEY hKey,
    __in LPCWSTR lpSubKey,
    __in REGSAM samDesired,
    __reserved DWORD Reserved
    ) = RegDeleteKeyExW;

LSTATUS
(APIENTRY *Real_RegDeleteKeyTransactedA) (
    __in HKEY hKey,
    __in LPCSTR lpSubKey,
    __in REGSAM samDesired,
    __reserved DWORD Reserved,
    __in        HANDLE hTransaction,
    __reserved PVOID  pExtendedParameter
    ) = RegDeleteKeyTransactedA;
LSTATUS
(APIENTRY *Real_RegDeleteKeyTransactedW) (
    __in HKEY hKey,
    __in LPCWSTR lpSubKey,
    __in REGSAM samDesired,
    __reserved DWORD Reserved,
    __in        HANDLE hTransaction,
    __reserved PVOID  pExtendedParameter
    ) = RegDeleteKeyTransactedW;

LONG
(APIENTRY *Real_RegDisableReflectionKey) (
    __in HKEY hBase
    ) = RegDisableReflectionKey;    

LONG
(APIENTRY *Real_RegEnableReflectionKey) (
    __in HKEY hBase
    ) = RegEnableReflectionKey;    

LONG
(APIENTRY *Real_RegQueryReflectionKey) (
    __in HKEY hBase,
    __out BOOL *bIsReflectionDisabled
    ) = RegQueryReflectionKey;    

#endif // _WIN32_WINNT >= 0x0600
    
LSTATUS
(APIENTRY *Real_RegDeleteValueA) (
    __in HKEY hKey,
    __in_opt LPCSTR lpValueName
    ) = RegDeleteValueA;
LSTATUS
(APIENTRY *Real_RegDeleteValueW) (
    __in HKEY hKey,
    __in_opt LPCWSTR lpValueName
    ) = RegDeleteValueW;

LSTATUS
(APIENTRY *Real_RegEnumKeyA) (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(cchName,cchName + 1) LPSTR lpName,
    __in DWORD cchName
    ) = RegEnumKeyA;
LSTATUS
(APIENTRY *Real_RegEnumKeyW) (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(cchName,cchName + 1) LPWSTR lpName,
    __in DWORD cchName
    ) = RegEnumKeyW;

LSTATUS
(APIENTRY *Real_RegEnumKeyExA) (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(*lpcchName, *lpcchName + 1) LPSTR lpName,
    __inout LPDWORD lpcchName,
    __reserved LPDWORD lpReserved,
    __out_ecount_part_opt(*lpcchClass,*lpcchClass + 1) LPSTR lpClass,
    __inout_opt LPDWORD lpcchClass,
    __out_opt PFILETIME lpftLastWriteTime
    ) = RegEnumKeyExA;
LSTATUS
(APIENTRY *Real_RegEnumKeyExW) (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(*lpcchName, *lpcchName + 1) LPWSTR lpName,
    __inout LPDWORD lpcchName,
    __reserved LPDWORD lpReserved,
    __out_ecount_part_opt(*lpcchClass,*lpcchClass + 1) LPWSTR lpClass,
    __inout_opt LPDWORD lpcchClass,
    __out_opt PFILETIME lpftLastWriteTime
    ) = RegEnumKeyExW;

LSTATUS
(APIENTRY *Real_RegEnumValueA) (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(*lpcchValueName, *lpcchValueName + 1) LPSTR lpValueName,
    __inout LPDWORD lpcchValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    ) = RegEnumValueA;
LSTATUS
(APIENTRY *Real_RegEnumValueW) (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(*lpcchValueName, *lpcchValueName + 1) LPWSTR lpValueName,
    __inout LPDWORD lpcchValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    ) = RegEnumValueW;

LSTATUS
(APIENTRY *Real_RegFlushKey) (
    __in HKEY hKey
    ) = RegFlushKey;

LSTATUS
(APIENTRY *Real_RegGetKeySecurity) (
    __in HKEY hKey,
    __in SECURITY_INFORMATION SecurityInformation,
    __out_bcount_opt(*lpcbSecurityDescriptor) PSECURITY_DESCRIPTOR pSecurityDescriptor,
    __inout LPDWORD lpcbSecurityDescriptor
    ) = RegGetKeySecurity;

LSTATUS
(APIENTRY *Real_RegLoadKeyA) (
    __in HKEY    hKey,
    __in_opt LPCSTR  lpSubKey,
    __in LPCSTR  lpFile
    ) = RegLoadKeyA;
LSTATUS
(APIENTRY *Real_RegLoadKeyW) (
    __in HKEY    hKey,
    __in_opt LPCWSTR  lpSubKey,
    __in LPCWSTR  lpFile
    ) = RegLoadKeyW;

LSTATUS
(APIENTRY *Real_RegNotifyChangeKeyValue) (
    __in HKEY hKey,
    __in BOOL bWatchSubtree,
    __in DWORD dwNotifyFilter,
    __in_opt HANDLE hEvent,
    __in BOOL fAsynchronous
    ) = RegNotifyChangeKeyValue;

LSTATUS
(APIENTRY *Real_RegOpenKeyA) (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __out PHKEY phkResult
    ) = RegOpenKeyA;
LSTATUS
(APIENTRY *Real_RegOpenKeyW) (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __out PHKEY phkResult
    ) = RegOpenKeyW;

LSTATUS
(APIENTRY *Real_RegOpenKeyExA) (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __reserved DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult
    ) = RegOpenKeyExA;
LSTATUS
(APIENTRY *Real_RegOpenKeyExW) (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __reserved DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult
    ) = RegOpenKeyExW;

#if _WIN32_WINNT >= 0x0600

LSTATUS
(APIENTRY *Real_RegOpenKeyTransactedA) (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __in DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult,
    __in        HANDLE hTransaction,
    __reserved PVOID  pExtendedParemeter
    ) = RegOpenKeyTransactedA;
LSTATUS
(APIENTRY *Real_RegOpenKeyTransactedW) (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __in DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult,
    __in        HANDLE hTransaction,
    __reserved PVOID  pExtendedParemeter
    ) = RegOpenKeyTransactedW;

#endif // _WIN32_WINNT >= 0x0600

LSTATUS
(APIENTRY *Real_RegQueryInfoKeyA) (
    __in HKEY hKey,
    __out_ecount_part_opt(*lpcchClass, *lpcchClass + 1) LPSTR lpClass,
    __inout_opt LPDWORD lpcchClass,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpcSubKeys,
    __out_opt LPDWORD lpcbMaxSubKeyLen,
    __out_opt LPDWORD lpcbMaxClassLen,
    __out_opt LPDWORD lpcValues,
    __out_opt LPDWORD lpcbMaxValueNameLen,
    __out_opt LPDWORD lpcbMaxValueLen,
    __out_opt LPDWORD lpcbSecurityDescriptor,
    __out_opt PFILETIME lpftLastWriteTime
    ) = RegQueryInfoKeyA;
LSTATUS
(APIENTRY *Real_RegQueryInfoKeyW) (
    __in HKEY hKey,
    __out_ecount_part_opt(*lpcchClass, *lpcchClass + 1) LPWSTR lpClass,
    __inout_opt LPDWORD lpcchClass,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpcSubKeys,
    __out_opt LPDWORD lpcbMaxSubKeyLen,
    __out_opt LPDWORD lpcbMaxClassLen,
    __out_opt LPDWORD lpcValues,
    __out_opt LPDWORD lpcbMaxValueNameLen,
    __out_opt LPDWORD lpcbMaxValueLen,
    __out_opt LPDWORD lpcbSecurityDescriptor,
    __out_opt PFILETIME lpftLastWriteTime
    ) = RegQueryInfoKeyW;

LSTATUS
(APIENTRY *Real_RegQueryValueA) (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPSTR lpData,
    __inout_opt PLONG lpcbData
    ) = RegQueryValueA;
LSTATUS
(APIENTRY *Real_RegQueryValueW) (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPWSTR lpData,
    __inout_opt PLONG lpcbData
    ) = RegQueryValueW;

#if(WINVER >= 0x0400)

LSTATUS
(APIENTRY *Real_RegQueryMultipleValuesA) (
    __in HKEY hKey,
    __out_ecount(num_vals) PVALENTA val_list,
    __in DWORD num_vals,
    __out_bcount_part_opt(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPSTR lpValueBuf,
    __inout_opt LPDWORD ldwTotsize
    ) = RegQueryMultipleValuesA;
LSTATUS
(APIENTRY *Real_RegQueryMultipleValuesW) (
    __in HKEY hKey,
    __out_ecount(num_vals) PVALENTW val_list,
    __in DWORD num_vals,
    __out_bcount_part_opt(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPWSTR lpValueBuf,
    __inout_opt LPDWORD ldwTotsize
    ) = RegQueryMultipleValuesW;
#endif /* WINVER >= 0x0400 */

LSTATUS
(APIENTRY *Real_RegQueryValueExA) (
    __in HKEY hKey,
    __in_opt LPCSTR lpValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    ) = RegQueryValueExA;
LSTATUS
(APIENTRY *Real_RegQueryValueExW) (
    __in HKEY hKey,
    __in_opt LPCWSTR lpValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    ) = RegQueryValueExW;

LSTATUS
(APIENTRY *Real_RegReplaceKeyA) (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __in LPCSTR lpNewFile,
    __in LPCSTR lpOldFile
    ) = RegReplaceKeyA;
LSTATUS
(APIENTRY *Real_RegReplaceKeyW) (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __in LPCWSTR lpNewFile,
    __in LPCWSTR lpOldFile
    ) = RegReplaceKeyW;

LSTATUS
(APIENTRY *Real_RegRestoreKeyA) (
    __in HKEY hKey,
    __in LPCSTR lpFile,
    __in DWORD dwFlags
    ) = RegRestoreKeyA;
LSTATUS
(APIENTRY *Real_RegRestoreKeyW) (
    __in HKEY hKey,
    __in LPCWSTR lpFile,
    __in DWORD dwFlags
    ) = RegRestoreKeyW;

LSTATUS
(APIENTRY *Real_RegSaveKeyA) (
    __in HKEY hKey,
    __in LPCSTR lpFile,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes
    ) = RegSaveKeyA;
LSTATUS
(APIENTRY *Real_RegSaveKeyW) (
    __in HKEY hKey,
    __in LPCWSTR lpFile,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes
    ) = RegSaveKeyW;

LSTATUS
(APIENTRY *Real_RegSetKeySecurity) (
    __in HKEY hKey,
    __in SECURITY_INFORMATION SecurityInformation,
    __in PSECURITY_DESCRIPTOR pSecurityDescriptor
    ) = RegSetKeySecurity;

LSTATUS
(APIENTRY *Real_RegSetValueA) (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __in DWORD dwType,
    __in_bcount_opt(cbData) LPCSTR lpData,
    __in DWORD cbData
    ) = RegSetValueA;
LSTATUS
(APIENTRY *Real_RegSetValueW) (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __in DWORD dwType,
    __in_bcount_opt(cbData) LPCWSTR lpData,
    __in DWORD cbData
    ) = RegSetValueW;


LSTATUS
(APIENTRY *Real_RegSetValueExA) (
    __in HKEY hKey,
    __in_opt LPCSTR lpValueName,
    __reserved DWORD Reserved,
    __in DWORD dwType,
    __in_bcount_opt(cbData) CONST BYTE* lpData,
    __in DWORD cbData
    ) = RegSetValueExA;
LSTATUS
(APIENTRY *Real_RegSetValueExW) (
    __in HKEY hKey,
    __in_opt LPCWSTR lpValueName,
    __reserved DWORD Reserved,
    __in DWORD dwType,
    __in_bcount_opt(cbData) CONST BYTE* lpData,
    __in DWORD cbData
    ) = RegSetValueExW;

LSTATUS
(APIENTRY *Real_RegUnLoadKeyA) (
    __in HKEY    hKey,
    __in_opt LPCSTR lpSubKey
    ) = RegUnLoadKeyA;
LSTATUS
(APIENTRY *Real_RegUnLoadKeyW) (
    __in HKEY    hKey,
    __in_opt LPCWSTR lpSubKey
    ) = RegUnLoadKeyW;

//
// Utils wrappers
//
#if _WIN32_WINNT >= 0x0600

LSTATUS
(APIENTRY *Real_RegDeleteKeyValueA) (
    __in      HKEY     hKey,
    __in_opt  LPCSTR lpSubKey,
    __in_opt  LPCSTR lpValueName
    ) = RegDeleteKeyValueA;
LSTATUS
(APIENTRY *Real_RegDeleteKeyValueW) (
    __in      HKEY     hKey,
    __in_opt  LPCWSTR lpSubKey,
    __in_opt  LPCWSTR lpValueName
    ) = RegDeleteKeyValueW;

LSTATUS
(APIENTRY *Real_RegSetKeyValueA) (
    __in        HKEY     hKey,
    __in_opt    LPCSTR  lpSubKey,
    __in_opt    LPCSTR  lpValueName,
    __in        DWORD    dwType,
    __in_bcount_opt(cbData) LPCVOID  lpData,
    __in        DWORD    cbData
    ) = RegSetKeyValueA;
LSTATUS
(APIENTRY *Real_RegSetKeyValueW) (
    __in        HKEY     hKey,
    __in_opt    LPCWSTR  lpSubKey,
    __in_opt    LPCWSTR  lpValueName,
    __in        DWORD    dwType,
    __in_bcount_opt(cbData) LPCVOID  lpData,
    __in        DWORD    cbData
    ) = RegSetKeyValueW;

LSTATUS
(APIENTRY *Real_RegDeleteTreeA) (
    __in        HKEY     hKey,
    __in_opt    LPCSTR  lpSubKey
    ) = RegDeleteTreeA;
LSTATUS
(APIENTRY *Real_RegDeleteTreeW) (
    __in        HKEY     hKey,
    __in_opt    LPCWSTR  lpSubKey
    ) = RegDeleteTreeW;

LSTATUS
(APIENTRY *Real_RegCopyTreeA) (
    __in        HKEY     hKeySrc,
    __in_opt    LPCSTR  lpSubKey,
    __in        HKEY     hKeyDest
    ) = RegCopyTreeA;
LSTATUS
(APIENTRY *Real_RegCopyTreeW) (
    __in        HKEY     hKeySrc,
    __in_opt    LPCWSTR  lpSubKey,
    __in        HKEY     hKeyDest
    ) = RegCopyTreeW;

LSTATUS
(APIENTRY *Real_RegGetValueA) (
    __in HKEY    hkey,
    __in_opt LPCSTR  lpSubKey,
    __in_opt LPCSTR  lpValue,
    __in_opt DWORD    dwFlags,
    __out_opt LPDWORD pdwType,
    __out_bcount_part_opt(*pcbData,*pcbData) PVOID   pvData,
    __inout_opt LPDWORD pcbData
    ) = RegGetValueA;
LSTATUS
(APIENTRY *Real_RegGetValueW) (
    __in HKEY    hkey,
    __in_opt LPCWSTR  lpSubKey,
    __in_opt LPCWSTR  lpValue,
    __in_opt DWORD    dwFlags,
    __out_opt LPDWORD pdwType,
    __out_bcount_part_opt(*pcbData,*pcbData) PVOID   pvData,
    __inout_opt LPDWORD pcbData
    ) = RegGetValueW;

LSTATUS
(APIENTRY *Real_RegLoadMUIStringA) (
    __in                    HKEY        hKey,
    __in_opt                LPCSTR    pszValue,
    __out_bcount_opt(cbOutBuf)  LPSTR     pszOutBuf,
    __in                    DWORD       cbOutBuf,
    __out_opt               LPDWORD     pcbData,
    __in                    DWORD       Flags,   
    __in_opt                LPCSTR    pszDirectory
    ) = RegLoadMUIStringA;
LSTATUS
(APIENTRY *Real_RegLoadMUIStringW) (
	__in                    HKEY        hKey,
	__in_opt                LPCWSTR    pszValue,
	__out_bcount_opt(cbOutBuf)  LPWSTR     pszOutBuf,
	__in                    DWORD       cbOutBuf,
	__out_opt               LPDWORD     pcbData,
	__in                    DWORD       Flags,   
	__in_opt                LPCWSTR    pszDirectory
	) = RegLoadMUIStringW;


LSTATUS
(APIENTRY *Real_RegLoadAppKeyA) (
        __in        LPCSTR    lpFile,
        __out       PHKEY       phkResult,
        __in        REGSAM      samDesired, 
        __in        DWORD       dwOptions,
        __reserved  DWORD       Reserved
    ) = RegLoadAppKeyA;
LSTATUS
(APIENTRY *Real_RegLoadAppKeyW) (
        __in        LPCWSTR    lpFile,
        __out       PHKEY       phkResult,
        __in        REGSAM      samDesired, 
        __in        DWORD       dwOptions,
        __reserved  DWORD       Reserved
    ) = RegLoadAppKeyW;

#endif // _WIN32_WINNT >= 0x0600

//////////////////////////////////////////////////////////////////////////

HRESULT 
STDAPICALLTYPE Mine_CoCreateInstance (
	__in     REFCLSID rclsid, 
	__in_opt LPUNKNOWN pUnkOuter,
	__in     DWORD dwClsContext, 
	__in     REFIID riid, 
	__deref_out LPVOID FAR* ppv
	);

LSTATUS
APIENTRY Mine_RegCloseKey (
    __in HKEY hKey
    );

LSTATUS
APIENTRY Mine_RegOverridePredefKey (
    __in HKEY hKey,
    __in_opt HKEY hNewHKey
    );

LSTATUS
APIENTRY Mine_RegOpenUserClassesRoot(
    __in HANDLE hToken,
    __reserved DWORD dwOptions,
    __in REGSAM samDesired,
    __out PHKEY  phkResult
    );

LSTATUS
APIENTRY Mine_RegOpenCurrentUser(
    __in REGSAM samDesired,
    __out PHKEY phkResult
    );

LSTATUS
APIENTRY Mine_RegDisablePredefinedCache(
    VOID
    );

LSTATUS
APIENTRY Mine_RegDisablePredefinedCacheEx(
    VOID
    );

LSTATUS
APIENTRY Mine_RegConnectRegistryA (
    __in_opt LPCSTR lpMachineName,
    __in HKEY hKey,
    __out PHKEY phkResult
    );
LSTATUS
APIENTRY Mine_RegConnectRegistryW (
    __in_opt LPCWSTR lpMachineName,
    __in HKEY hKey,
    __out PHKEY phkResult
    );

#if _WIN32_WINNT >= 0x0600

LSTATUS
APIENTRY Mine_RegConnectRegistryExA (
    __in_opt LPCSTR lpMachineName,
    __in HKEY hKey,
    __in ULONG Flags,
    __out PHKEY phkResult
    );
LSTATUS
APIENTRY Mine_RegConnectRegistryExW (
    __in_opt LPCWSTR lpMachineName,
    __in HKEY hKey,
    __in ULONG Flags,
    __out PHKEY phkResult
    );

#endif // _WIN32_WINNT >= 0x0600

LSTATUS
APIENTRY Mine_RegCreateKeyA (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __out PHKEY phkResult
    );
LSTATUS
APIENTRY Mine_RegCreateKeyW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __out PHKEY phkResult
    );

LSTATUS
APIENTRY Mine_RegCreateKeyExA (
    __in HKEY hKey,
    __in LPCSTR lpSubKey,
    __reserved DWORD Reserved,
    __in_opt LPSTR lpClass,
    __in DWORD dwOptions,
    __in REGSAM samDesired,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __out PHKEY phkResult,
    __out_opt LPDWORD lpdwDisposition
    );
LSTATUS
APIENTRY Mine_RegCreateKeyExW (
    __in HKEY hKey,
    __in LPCWSTR lpSubKey,
    __reserved DWORD Reserved,
    __in_opt LPWSTR lpClass,
    __in DWORD dwOptions,
    __in REGSAM samDesired,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __out PHKEY phkResult,
    __out_opt LPDWORD lpdwDisposition
    );

#if _WIN32_WINNT >= 0x0600

LSTATUS
APIENTRY Mine_RegCreateKeyTransactedA (
    __in HKEY hKey,
    __in LPCSTR lpSubKey,
    __reserved DWORD Reserved,
    __in_opt LPSTR lpClass,
    __in DWORD dwOptions,
    __in REGSAM samDesired,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __out PHKEY phkResult,
    __out_opt LPDWORD lpdwDisposition,
    __in        HANDLE hTransaction,
    __reserved PVOID  pExtendedParemeter
    );
LSTATUS
APIENTRY Mine_RegCreateKeyTransactedW (
    __in HKEY hKey,
    __in LPCWSTR lpSubKey,
    __reserved DWORD Reserved,
    __in_opt LPWSTR lpClass,
    __in DWORD dwOptions,
    __in REGSAM samDesired,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __out PHKEY phkResult,
    __out_opt LPDWORD lpdwDisposition,
    __in        HANDLE hTransaction,
    __reserved PVOID  pExtendedParemeter
    );

#endif // _WIN32_WINNT >= 0x0600

LSTATUS
APIENTRY Mine_RegDeleteKeyA (
    __in HKEY hKey,
    __in LPCSTR lpSubKey
    );
LSTATUS
APIENTRY Mine_RegDeleteKeyW (
    __in HKEY hKey,
    __in LPCWSTR lpSubKey
    );

#if _WIN32_WINNT >= 0x0600

LSTATUS
APIENTRY Mine_RegDeleteKeyExA (
    __in HKEY hKey,
    __in LPCSTR lpSubKey,
    __in REGSAM samDesired,
    __reserved DWORD Reserved
    );
LSTATUS
APIENTRY Mine_RegDeleteKeyExW (
    __in HKEY hKey,
    __in LPCWSTR lpSubKey,
    __in REGSAM samDesired,
    __reserved DWORD Reserved
    );

LSTATUS
APIENTRY Mine_RegDeleteKeyTransactedA (
    __in HKEY hKey,
    __in LPCSTR lpSubKey,
    __in REGSAM samDesired,
    __reserved DWORD Reserved,
    __in        HANDLE hTransaction,
    __reserved PVOID  pExtendedParameter
    );
LSTATUS
APIENTRY Mine_RegDeleteKeyTransactedW (
    __in HKEY hKey,
    __in LPCWSTR lpSubKey,
    __in REGSAM samDesired,
    __reserved DWORD Reserved,
    __in        HANDLE hTransaction,
    __reserved PVOID  pExtendedParameter
    );

LONG
APIENTRY Mine_RegDisableReflectionKey (
    __in HKEY hBase
    );    

LONG
APIENTRY Mine_RegEnableReflectionKey (
    __in HKEY hBase
    );    

LONG
APIENTRY Mine_RegQueryReflectionKey (
    __in HKEY hBase,
    __out BOOL *bIsReflectionDisabled
    );    

#endif // _WIN32_WINNT >= 0x0600
    
LSTATUS
APIENTRY Mine_RegDeleteValueA (
    __in HKEY hKey,
    __in_opt LPCSTR lpValueName
    );
LSTATUS
APIENTRY Mine_RegDeleteValueW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpValueName
    );

LSTATUS
APIENTRY Mine_RegEnumKeyA (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(cchName,cchName + 1) LPSTR lpName,
    __in DWORD cchName
    );
LSTATUS
APIENTRY Mine_RegEnumKeyW (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(cchName,cchName + 1) LPWSTR lpName,
    __in DWORD cchName
    );

LSTATUS
APIENTRY Mine_RegEnumKeyExA (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(*lpcchName, *lpcchName + 1) LPSTR lpName,
    __inout LPDWORD lpcchName,
    __reserved LPDWORD lpReserved,
    __out_ecount_part_opt(*lpcchClass,*lpcchClass + 1) LPSTR lpClass,
    __inout_opt LPDWORD lpcchClass,
    __out_opt PFILETIME lpftLastWriteTime
    );
LSTATUS
APIENTRY Mine_RegEnumKeyExW (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(*lpcchName, *lpcchName + 1) LPWSTR lpName,
    __inout LPDWORD lpcchName,
    __reserved LPDWORD lpReserved,
    __out_ecount_part_opt(*lpcchClass,*lpcchClass + 1) LPWSTR lpClass,
    __inout_opt LPDWORD lpcchClass,
    __out_opt PFILETIME lpftLastWriteTime
    );

LSTATUS
APIENTRY Mine_RegEnumValueA (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(*lpcchValueName, *lpcchValueName + 1) LPSTR lpValueName,
    __inout LPDWORD lpcchValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    );
LSTATUS
APIENTRY Mine_RegEnumValueW (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(*lpcchValueName, *lpcchValueName + 1) LPWSTR lpValueName,
    __inout LPDWORD lpcchValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    );

LSTATUS
APIENTRY Mine_RegFlushKey (
    __in HKEY hKey
    );

LSTATUS
APIENTRY Mine_RegGetKeySecurity (
    __in HKEY hKey,
    __in SECURITY_INFORMATION SecurityInformation,
    __out_bcount_opt(*lpcbSecurityDescriptor) PSECURITY_DESCRIPTOR pSecurityDescriptor,
    __inout LPDWORD lpcbSecurityDescriptor
    );

LSTATUS
APIENTRY Mine_RegLoadKeyA (
    __in HKEY    hKey,
    __in_opt LPCSTR  lpSubKey,
    __in LPCSTR  lpFile
    );
LSTATUS
APIENTRY Mine_RegLoadKeyW (
    __in HKEY    hKey,
    __in_opt LPCWSTR  lpSubKey,
    __in LPCWSTR  lpFile
    );

LSTATUS
APIENTRY Mine_RegNotifyChangeKeyValue (
    __in HKEY hKey,
    __in BOOL bWatchSubtree,
    __in DWORD dwNotifyFilter,
    __in_opt HANDLE hEvent,
    __in BOOL fAsynchronous
    );

LSTATUS
APIENTRY Mine_RegOpenKeyA (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __out PHKEY phkResult
    );
LSTATUS
APIENTRY Mine_RegOpenKeyW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __out PHKEY phkResult
    );

LSTATUS
APIENTRY Mine_RegOpenKeyExA (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __reserved DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult
    );
LSTATUS
APIENTRY Mine_RegOpenKeyExW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __reserved DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult
    );

#if _WIN32_WINNT >= 0x0600

LSTATUS
APIENTRY Mine_RegOpenKeyTransactedA (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __in DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult,
    __in        HANDLE hTransaction,
    __reserved PVOID  pExtendedParemeter
    );
LSTATUS
APIENTRY Mine_RegOpenKeyTransactedW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __in DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult,
    __in        HANDLE hTransaction,
    __reserved PVOID  pExtendedParemeter
    );

#endif // _WIN32_WINNT >= 0x0600

LSTATUS
APIENTRY Mine_RegQueryInfoKeyA (
    __in HKEY hKey,
    __out_ecount_part_opt(*lpcchClass, *lpcchClass + 1) LPSTR lpClass,
    __inout_opt LPDWORD lpcchClass,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpcSubKeys,
    __out_opt LPDWORD lpcbMaxSubKeyLen,
    __out_opt LPDWORD lpcbMaxClassLen,
    __out_opt LPDWORD lpcValues,
    __out_opt LPDWORD lpcbMaxValueNameLen,
    __out_opt LPDWORD lpcbMaxValueLen,
    __out_opt LPDWORD lpcbSecurityDescriptor,
    __out_opt PFILETIME lpftLastWriteTime
    );
LSTATUS
APIENTRY Mine_RegQueryInfoKeyW (
    __in HKEY hKey,
    __out_ecount_part_opt(*lpcchClass, *lpcchClass + 1) LPWSTR lpClass,
    __inout_opt LPDWORD lpcchClass,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpcSubKeys,
    __out_opt LPDWORD lpcbMaxSubKeyLen,
    __out_opt LPDWORD lpcbMaxClassLen,
    __out_opt LPDWORD lpcValues,
    __out_opt LPDWORD lpcbMaxValueNameLen,
    __out_opt LPDWORD lpcbMaxValueLen,
    __out_opt LPDWORD lpcbSecurityDescriptor,
    __out_opt PFILETIME lpftLastWriteTime
    );

LSTATUS
APIENTRY Mine_RegQueryValueA (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPSTR lpData,
    __inout_opt PLONG lpcbData
    );
LSTATUS
APIENTRY Mine_RegQueryValueW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPWSTR lpData,
    __inout_opt PLONG lpcbData
    );

#if(WINVER >= 0x0400)

LSTATUS
APIENTRY Mine_RegQueryMultipleValuesA (
    __in HKEY hKey,
    __out_ecount(num_vals) PVALENTA val_list,
    __in DWORD num_vals,
    __out_bcount_part_opt(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPSTR lpValueBuf,
    __inout_opt LPDWORD ldwTotsize
    );
LSTATUS
APIENTRY Mine_RegQueryMultipleValuesW (
    __in HKEY hKey,
    __out_ecount(num_vals) PVALENTW val_list,
    __in DWORD num_vals,
    __out_bcount_part_opt(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPWSTR lpValueBuf,
    __inout_opt LPDWORD ldwTotsize
    );
#endif /* WINVER >= 0x0400 */

LSTATUS
APIENTRY Mine_RegQueryValueExA (
    __in HKEY hKey,
    __in_opt LPCSTR lpValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    );
LSTATUS
APIENTRY Mine_RegQueryValueExW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    );

LSTATUS
APIENTRY Mine_RegReplaceKeyA (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __in LPCSTR lpNewFile,
    __in LPCSTR lpOldFile
    );
LSTATUS
APIENTRY Mine_RegReplaceKeyW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __in LPCWSTR lpNewFile,
    __in LPCWSTR lpOldFile
    );

LSTATUS
APIENTRY Mine_RegRestoreKeyA (
    __in HKEY hKey,
    __in LPCSTR lpFile,
    __in DWORD dwFlags
    );
LSTATUS
APIENTRY Mine_RegRestoreKeyW (
    __in HKEY hKey,
    __in LPCWSTR lpFile,
    __in DWORD dwFlags
    );

LSTATUS
APIENTRY Mine_RegSaveKeyA (
    __in HKEY hKey,
    __in LPCSTR lpFile,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );
LSTATUS
APIENTRY Mine_RegSaveKeyW (
    __in HKEY hKey,
    __in LPCWSTR lpFile,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes
    );

LSTATUS
APIENTRY Mine_RegSetKeySecurity (
    __in HKEY hKey,
    __in SECURITY_INFORMATION SecurityInformation,
    __in PSECURITY_DESCRIPTOR pSecurityDescriptor
    );

LSTATUS
APIENTRY Mine_RegSetValueA (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __in DWORD dwType,
    __in_bcount_opt(cbData) LPCSTR lpData,
    __in DWORD cbData
    );
LSTATUS
APIENTRY Mine_RegSetValueW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __in DWORD dwType,
    __in_bcount_opt(cbData) LPCWSTR lpData,
    __in DWORD cbData
    );


LSTATUS
APIENTRY Mine_RegSetValueExA (
    __in HKEY hKey,
    __in_opt LPCSTR lpValueName,
    __reserved DWORD Reserved,
    __in DWORD dwType,
    __in_bcount_opt(cbData) CONST BYTE* lpData,
    __in DWORD cbData
    );
LSTATUS
APIENTRY Mine_RegSetValueExW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpValueName,
    __reserved DWORD Reserved,
    __in DWORD dwType,
    __in_bcount_opt(cbData) CONST BYTE* lpData,
    __in DWORD cbData
    );

LSTATUS
APIENTRY Mine_RegUnLoadKeyA (
    __in HKEY    hKey,
    __in_opt LPCSTR lpSubKey
    );
LSTATUS
APIENTRY Mine_RegUnLoadKeyW (
    __in HKEY    hKey,
    __in_opt LPCWSTR lpSubKey
    );

//
// Utils wrappers
//
#if _WIN32_WINNT >= 0x0600

LSTATUS
APIENTRY Mine_RegDeleteKeyValueA (
    __in      HKEY     hKey,
    __in_opt  LPCSTR lpSubKey,
    __in_opt  LPCSTR lpValueName
    );
LSTATUS
APIENTRY Mine_RegDeleteKeyValueW (
    __in      HKEY     hKey,
    __in_opt  LPCWSTR lpSubKey,
    __in_opt  LPCWSTR lpValueName
    );

LSTATUS
APIENTRY Mine_RegSetKeyValueA (
    __in        HKEY     hKey,
    __in_opt    LPCSTR  lpSubKey,
    __in_opt    LPCSTR  lpValueName,
    __in        DWORD    dwType,
    __in_bcount_opt(cbData) LPCVOID  lpData,
    __in        DWORD    cbData
    );
LSTATUS
APIENTRY Mine_RegSetKeyValueW (
    __in        HKEY     hKey,
    __in_opt    LPCWSTR  lpSubKey,
    __in_opt    LPCWSTR  lpValueName,
    __in        DWORD    dwType,
    __in_bcount_opt(cbData) LPCVOID  lpData,
    __in        DWORD    cbData
    );

LSTATUS
APIENTRY Mine_RegDeleteTreeA (
    __in        HKEY     hKey,
    __in_opt    LPCSTR  lpSubKey
    );
LSTATUS
APIENTRY Mine_RegDeleteTreeW (
    __in        HKEY     hKey,
    __in_opt    LPCWSTR  lpSubKey
    );

LSTATUS
APIENTRY Mine_RegCopyTreeA (
    __in        HKEY     hKeySrc,
    __in_opt    LPCSTR  lpSubKey,
    __in        HKEY     hKeyDest
    );
LSTATUS
APIENTRY Mine_RegCopyTreeW (
    __in        HKEY     hKeySrc,
    __in_opt    LPCWSTR  lpSubKey,
    __in        HKEY     hKeyDest
    );

LSTATUS
APIENTRY Mine_RegGetValueA (
    __in HKEY    hkey,
    __in_opt LPCSTR  lpSubKey,
    __in_opt LPCSTR  lpValue,
    __in_opt DWORD    dwFlags,
    __out_opt LPDWORD pdwType,
    __out_bcount_part_opt(*pcbData,*pcbData) PVOID   pvData,
    __inout_opt LPDWORD pcbData
    );
LSTATUS
APIENTRY Mine_RegGetValueW (
    __in HKEY    hkey,
    __in_opt LPCWSTR  lpSubKey,
    __in_opt LPCWSTR  lpValue,
    __in_opt DWORD    dwFlags,
    __out_opt LPDWORD pdwType,
    __out_bcount_part_opt(*pcbData,*pcbData) PVOID   pvData,
    __inout_opt LPDWORD pcbData
    );

LSTATUS
APIENTRY Mine_RegLoadMUIStringA (
    __in                    HKEY        hKey,
    __in_opt                LPCSTR    pszValue,
    __out_bcount_opt(cbOutBuf)  LPSTR     pszOutBuf,
    __in                    DWORD       cbOutBuf,
    __out_opt               LPDWORD     pcbData,
    __in                    DWORD       Flags,   
    __in_opt                LPCSTR    pszDirectory
    );
LSTATUS
APIENTRY Mine_RegLoadMUIStringW (
	__in                    HKEY        hKey,
	__in_opt                LPCWSTR    pszValue,
	__out_bcount_opt(cbOutBuf)  LPWSTR     pszOutBuf,
	__in                    DWORD       cbOutBuf,
	__out_opt               LPDWORD     pcbData,
	__in                    DWORD       Flags,   
	__in_opt                LPCWSTR    pszDirectory
	);


LSTATUS
APIENTRY Mine_RegLoadAppKeyA (
        __in        LPCSTR    lpFile,
        __out       PHKEY       phkResult,
        __in        REGSAM      samDesired, 
        __in        DWORD       dwOptions,
        __reserved  DWORD       Reserved
    );
LSTATUS
APIENTRY Mine_RegLoadAppKeyW (
        __in        LPCWSTR    lpFile,
        __out       PHKEY       phkResult,
        __in        REGSAM      samDesired, 
        __in        DWORD       dwOptions,
        __reserved  DWORD       Reserved
    );

#endif // _WIN32_WINNT >= 0x0600

//////////////////////////////////////////////////////////////////////////

bool CFakeFilterMapper2::ms_bInitialized;

CFakeFilterMapper2::CFakeFilterMapper2( void )
{
	if (SUCCEEDED(Real_CoCreateInstance(CLSID_FilterMapper2, NULL, CLSCTX_INPROC_SERVER, IID_IFilterMapper2, (LPVOID*)&m_pFM2))
		&& !ms_bInitialized)
	{
		HookAPI(Real_CoCreateInstance, Mine_CoCreateInstance);
		HookAPI(Real_RegCloseKey, Mine_RegCloseKey);
		HookAPI(Real_RegOverridePredefKey, Mine_RegOverridePredefKey);
		HookAPI(Real_RegOpenUserClassesRoot, Mine_RegOpenUserClassesRoot);
		HookAPI(Real_RegOpenCurrentUser, Mine_RegOpenCurrentUser);
		HookAPI(Real_RegDisablePredefinedCache, Mine_RegDisablePredefinedCache);
		HookAPI(Real_RegDisablePredefinedCacheEx, Mine_RegDisablePredefinedCacheEx);
		HookAPI(Real_RegConnectRegistryA, Mine_RegConnectRegistryA);
		HookAPI(Real_RegConnectRegistryW, Mine_RegConnectRegistryW);
#if _WIN32_WINNT >= 0x0600
		HookAPI(Real_RegConnectRegistryExA, Mine_RegConnectRegistryExA);
		HookAPI(Real_RegConnectRegistryExW, Mine_RegConnectRegistryExW);
#endif // _WIN32_WINNT >= 0x0600
		HookAPI(Real_RegCreateKeyA, Mine_RegCreateKeyA);
		HookAPI(Real_RegCreateKeyW, Mine_RegCreateKeyW);
		HookAPI(Real_RegCreateKeyExA, Mine_RegCreateKeyExA);
		HookAPI(Real_RegCreateKeyExW, Mine_RegCreateKeyExW);
#if _WIN32_WINNT >= 0x0600
		HookAPI(Real_RegCreateKeyTransactedA, Mine_RegCreateKeyTransactedA);
		HookAPI(Real_RegCreateKeyTransactedW, Mine_RegCreateKeyTransactedW);
#endif // _WIN32_WINNT >= 0x0600
		HookAPI(Real_RegDeleteKeyA, Mine_RegDeleteKeyA);
		HookAPI(Real_RegDeleteKeyW, Mine_RegDeleteKeyW);
#if _WIN32_WINNT >= 0x0600
		HookAPI(Real_RegDeleteKeyExA, Mine_RegDeleteKeyExA);
		HookAPI(Real_RegDeleteKeyExW, Mine_RegDeleteKeyExW);
		HookAPI(Real_RegDeleteKeyTransactedA, Mine_RegDeleteKeyTransactedA);
		HookAPI(Real_RegDeleteKeyTransactedW, Mine_RegDeleteKeyTransactedW);
		HookAPI(Real_RegDisableReflectionKey, Mine_RegDisableReflectionKey);    
		HookAPI(Real_RegEnableReflectionKey, Mine_RegEnableReflectionKey);    
		HookAPI(Real_RegQueryReflectionKey, Mine_RegQueryReflectionKey);    
#endif // _WIN32_WINNT >= 0x0600   
		HookAPI(Real_RegDeleteValueA, Mine_RegDeleteValueA);
		HookAPI(Real_RegDeleteValueW, Mine_RegDeleteValueW);
		HookAPI(Real_RegEnumKeyA, Mine_RegEnumKeyA);
		HookAPI(Real_RegEnumKeyW, Mine_RegEnumKeyW);
		HookAPI(Real_RegEnumKeyExA, Mine_RegEnumKeyExA);
		HookAPI(Real_RegEnumKeyExW, Mine_RegEnumKeyExW);
		HookAPI(Real_RegEnumValueA, Mine_RegEnumValueA);
		HookAPI(Real_RegEnumValueW, Mine_RegEnumValueW);
		HookAPI(Real_RegFlushKey, Mine_RegFlushKey);
		HookAPI(Real_RegGetKeySecurity, Mine_RegGetKeySecurity);
		HookAPI(Real_RegLoadKeyA, Mine_RegLoadKeyA);
		HookAPI(Real_RegLoadKeyW, Mine_RegLoadKeyW);
		HookAPI(Real_RegNotifyChangeKeyValue, Mine_RegNotifyChangeKeyValue);
		HookAPI(Real_RegOpenKeyA, Mine_RegOpenKeyA);
		HookAPI(Real_RegOpenKeyW, Mine_RegOpenKeyW);
		HookAPI(Real_RegOpenKeyExA, Mine_RegOpenKeyExA);
		HookAPI(Real_RegOpenKeyExW, Mine_RegOpenKeyExW);
#if _WIN32_WINNT >= 0x0600
		HookAPI(Real_RegOpenKeyTransactedA, Mine_RegOpenKeyTransactedA);
		HookAPI(Real_RegOpenKeyTransactedW, Mine_RegOpenKeyTransactedW);
#endif // _WIN32_WINNT >= 0x0600
		HookAPI(Real_RegQueryInfoKeyA, Mine_RegQueryInfoKeyA);
		HookAPI(Real_RegQueryInfoKeyW, Mine_RegQueryInfoKeyW);
		HookAPI(Real_RegQueryValueA, Mine_RegQueryValueA);
		HookAPI(Real_RegQueryValueW, Mine_RegQueryValueW);
#if(WINVER >= 0x0400)
		HookAPI(Real_RegQueryMultipleValuesA, Mine_RegQueryMultipleValuesA);
		HookAPI(Real_RegQueryMultipleValuesW, Mine_RegQueryMultipleValuesW);
#endif /* WINVER >= 0x0400 */
		HookAPI(Real_RegQueryValueExA, Mine_RegQueryValueExA);
		HookAPI(Real_RegQueryValueExW, Mine_RegQueryValueExW);
		HookAPI(Real_RegReplaceKeyA, Mine_RegReplaceKeyA);
		HookAPI(Real_RegReplaceKeyW, Mine_RegReplaceKeyW);
		HookAPI(Real_RegRestoreKeyA, Mine_RegRestoreKeyA);
		HookAPI(Real_RegRestoreKeyW, Mine_RegRestoreKeyW);
		HookAPI(Real_RegSaveKeyA, Mine_RegSaveKeyA);
		HookAPI(Real_RegSaveKeyW, Mine_RegSaveKeyW);
		HookAPI(Real_RegSetKeySecurity, Mine_RegSetKeySecurity);
		HookAPI(Real_RegSetValueA, Mine_RegSetValueA);
		HookAPI(Real_RegSetValueW, Mine_RegSetValueW);
		HookAPI(Real_RegSetValueExA, Mine_RegSetValueExA);
		HookAPI(Real_RegSetValueExW, Mine_RegSetValueExW);
		HookAPI(Real_RegUnLoadKeyA, Mine_RegUnLoadKeyA);
		HookAPI(Real_RegUnLoadKeyW, Mine_RegUnLoadKeyW);
#if _WIN32_WINNT >= 0x0600
		HookAPI(Real_RegDeleteKeyValueA, Mine_RegDeleteKeyValueA);
		HookAPI(Real_RegDeleteKeyValueW, Mine_RegDeleteKeyValueW);
		HookAPI(Real_RegSetKeyValueA, Mine_RegSetKeyValueA);
		HookAPI(Real_RegSetKeyValueW, Mine_RegSetKeyValueW);
		HookAPI(Real_RegDeleteTreeA, Mine_RegDeleteTreeA);
		HookAPI(Real_RegDeleteTreeW, Mine_RegDeleteTreeW);
		HookAPI(Real_RegCopyTreeA, Mine_RegCopyTreeA);
		HookAPI(Real_RegCopyTreeW, Mine_RegCopyTreeW);
		HookAPI(Real_RegGetValueA, Mine_RegGetValueA);
		HookAPI(Real_RegGetValueW, Mine_RegGetValueW);
		HookAPI(Real_RegLoadMUIStringA, Mine_RegLoadMUIStringA);
		HookAPI(Real_RegLoadMUIStringW, Mine_RegLoadMUIStringW);
		HookAPI(Real_RegLoadAppKeyA, Mine_RegLoadAppKeyA);
		HookAPI(Real_RegLoadAppKeyW, Mine_RegLoadAppKeyW);
#endif // _WIN32_WINNT >= 0x0600
		ms_bInitialized = true;
	}
}

CFakeFilterMapper2::~CFakeFilterMapper2( void )
{
	m_pFM2 = NULL;
}

HRESULT STDMETHODCALLTYPE CFakeFilterMapper2::CreateCategory( REFCLSID clsidCategory, DWORD dwCategoryMerit, LPCWSTR Description )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CFakeFilterMapper2::UnregisterFilter( const CLSID *pclsidCategory, LPCOLESTR szInstance, REFCLSID Filter )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CFakeFilterMapper2::RegisterFilter( REFCLSID clsidFilter, LPCWSTR Name, IMoniker **ppMoniker, const CLSID *pclsidCategory, LPCOLESTR szInstance, const REGFILTER2 *prf2 )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CFakeFilterMapper2::EnumMatchingFilters( IEnumMoniker **ppEnum, DWORD dwFlags, BOOL bExactMatch, DWORD dwMerit, BOOL bInputNeeded, DWORD cInputTypes, const GUID *pInputTypes, const REGPINMEDIUM *pMedIn, const CLSID *pPinCategoryIn, BOOL bRender, BOOL bOutputNeeded, DWORD cOutputTypes, const GUID *pOutputTypes, const REGPINMEDIUM *pMedOut, const CLSID *pPinCategoryOut )
{
	return E_NOTIMPL;
}