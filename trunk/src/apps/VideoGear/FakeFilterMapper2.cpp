#include "stdafx.h"
#include "VideoGear.h"
#include "FakeFilterMapper2.h"
#include <APIHook.h>

#pragma region "注册表API"

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

#pragma endregion 

//////////////////////////////////////////////////////////////////////////

#define FAKEHKEY			((HKEY)0x40000000)
#define ToHKEY(ptr)			((HKEY)((uint32_t)(ptr) | 0x40000000))

inline bool IsFakeHKEY(HKEY hKey)
{
	return ((uint32_t)hKey & 0x40000000) != 0;
}

inline bool IsRootHKEY(HKEY hKey)
{
	return ((uint32_t)hKey >= 0x80000000);
}

HRESULT 
STDAPICALLTYPE Mine_CoCreateInstance (
	__in     REFCLSID rclsid, 
	__in_opt LPUNKNOWN pUnkOuter,
	__in     DWORD dwClsContext, 
	__in     REFIID riid, 
	__deref_out LPVOID FAR* ppv
	)
{
	if (AfxGetFakeFM2()->m_bHooking)
	{
		if (ppv == NULL)
			return E_POINTER;

		if (IsEqualCLSID(rclsid, CLSID_FilterMapper))
			return REGDB_E_CLASSNOTREG;
		else if (IsEqualCLSID(rclsid, CLSID_FilterMapper2))
		{
			if (pUnkOuter != NULL)
				return CLASS_E_NOAGGREGATION;

			if (IsEqualIID(riid, IID_IFilterMapper2))
			{
				*ppv = (IFilterMapper2*)AfxGetFakeFM2();
				return S_OK;
			}
			else if (IsEqualIID(riid, IID_IUnknown))
			{
				*ppv = (IUnknown*)AfxGetFakeFM2();
				return S_OK;
			}
			else
				return E_NOINTERFACE;
		}
	}
	// 请求的类不是FilterMapper2相关，让系统处理
	return Real_CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
}

LSTATUS
APIENTRY Mine_RegCloseKey (
    __in HKEY hKey
    )
{
	if (IsFakeHKEY(hKey))
		return S_OK;
	else
		return Real_RegCloseKey(hKey);
}

LSTATUS
APIENTRY Mine_RegOverridePredefKey (
    __in HKEY hKey,
    __in_opt HKEY hNewHKey
    )
{
	TRACE("RegOverridePredefKey\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegOpenUserClassesRoot(
    __in HANDLE hToken,
    __reserved DWORD dwOptions,
    __in REGSAM samDesired,
    __out PHKEY  phkResult
    )
{
	TRACE("RegOpenUserClassesRoot\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegOpenCurrentUser(
    __in REGSAM samDesired,
    __out PHKEY phkResult
    )
{
	TRACE("RegOpenCurrentUser\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegDisablePredefinedCache(
    VOID
    )
{
	TRACE("RegDisablePredefinedCache\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegDisablePredefinedCacheEx(
    VOID
    )
{
	TRACE("RegDisablePredefinedCacheEx\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegConnectRegistryA (
    __in_opt LPCSTR lpMachineName,
    __in HKEY hKey,
    __out PHKEY phkResult
    )
{
	TRACE("RegConnectRegistryA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegConnectRegistryW (
    __in_opt LPCWSTR lpMachineName,
    __in HKEY hKey,
    __out PHKEY phkResult
    )
{
	TRACE("RegConnectRegistryW\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegCreateKeyA (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __out PHKEY phkResult
    )
{
	TRACE("RegCreateKeyA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegCreateKeyW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __out PHKEY phkResult
    )
{
	return RegCreateKeyExW(hKey, lpSubKey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, phkResult, NULL);
}

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
    )
{
	TRACE("RegCreateKeyExA\n");
	return E_NOTIMPL;
}
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
    )
{
	if (AfxGetFakeFM2()->m_bHooking)
	{
		if (phkResult == NULL)
			return E_POINTER;

		CRegTreeNode* pNode = AfxGetFakeFM2()->RegNodeFromHKEY(hKey);
		ASSERT(pNode != NULL);
		CRegTreeNode* pResult = pNode->OpenNode(lpSubKey);
		*phkResult = ToHKEY(pResult);
		return S_OK;
	}
	else
		return Real_RegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}

LSTATUS
APIENTRY Mine_RegDeleteKeyA (
    __in HKEY hKey,
    __in LPCSTR lpSubKey
    )
{
	TRACE("RegDeleteKeyA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegDeleteKeyW (
    __in HKEY hKey,
    __in LPCWSTR lpSubKey
    )
{
	if (AfxGetFakeFM2()->m_bHooking)
	{
		CRegTreeNode* pNode = AfxGetFakeFM2()->RegNodeFromHKEY(hKey);
		ASSERT(pNode != NULL);
		pNode->CloseNode(lpSubKey);
		return S_OK;
	}
	else
		return Real_RegDeleteKeyW(hKey, lpSubKey);
}
    
LSTATUS
APIENTRY Mine_RegDeleteValueA (
    __in HKEY hKey,
    __in_opt LPCSTR lpValueName
    )
{
	TRACE("RegDeleteValueA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegDeleteValueW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpValueName
    )
{
	TRACE("RegDeleteValueW\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegEnumKeyA (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(cchName,cchName + 1) LPSTR lpName,
    __in DWORD cchName
    )
{
	TRACE("RegEnumKeyA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegEnumKeyW (
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(cchName,cchName + 1) LPWSTR lpName,
    __in DWORD cchName
    )
{
	TRACE("RegEnumKeyW\n");
	return E_NOTIMPL;
}

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
    )
{
	TRACE("RegEnumKeyExA\n");
	return E_NOTIMPL;
}
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
    )
{
	TRACE("RegEnumKeyExW\n");
	return E_NOTIMPL;
}

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
    )
{
	TRACE("RegEnumValueA\n");
	return E_NOTIMPL;
}
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
    )
{
	TRACE("RegEnumValueW\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegFlushKey (
    __in HKEY hKey
    )
{
	TRACE("RegFlushKey\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegGetKeySecurity (
    __in HKEY hKey,
    __in SECURITY_INFORMATION SecurityInformation,
    __out_bcount_opt(*lpcbSecurityDescriptor) PSECURITY_DESCRIPTOR pSecurityDescriptor,
    __inout LPDWORD lpcbSecurityDescriptor
    )
{
	TRACE("RegGetKeySecurity\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegLoadKeyA (
    __in HKEY    hKey,
    __in_opt LPCSTR  lpSubKey,
    __in LPCSTR  lpFile
    )
{
	TRACE("RegLoadKeyA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegLoadKeyW (
    __in HKEY    hKey,
    __in_opt LPCWSTR  lpSubKey,
    __in LPCWSTR  lpFile
    )
{
	TRACE("RegLoadKeyW\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegNotifyChangeKeyValue (
    __in HKEY hKey,
    __in BOOL bWatchSubtree,
    __in DWORD dwNotifyFilter,
    __in_opt HANDLE hEvent,
    __in BOOL fAsynchronous
    )
{
	TRACE("RegNotifyChangeKeyValue\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegOpenKeyA (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __out PHKEY phkResult
    )
{
	TRACE("RegOpenKeyA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegOpenKeyW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __out PHKEY phkResult
    )
{
	if (AfxGetFakeFM2()->m_bHooking)
	{
		return RegOpenKeyExW(hKey, lpSubKey, 0, KEY_ALL_ACCESS, phkResult);
	}
	else
		return Real_RegOpenKeyW(hKey, lpSubKey, phkResult);
}

LSTATUS
APIENTRY Mine_RegOpenKeyExA (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __reserved DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult
    )
{
	USES_CONVERSION;
	
	if (AfxGetFakeFM2()->m_bHooking)
	{
		return RegOpenKeyExW(hKey, A2W(lpSubKey), ulOptions, samDesired, phkResult);
	}
	else
		return Real_RegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}
LSTATUS
APIENTRY Mine_RegOpenKeyExW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __reserved DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult
    )
{
	if (AfxGetFakeFM2()->m_bHooking)
	{
		if (phkResult == NULL)
			return E_POINTER;

		CRegTreeNode* pNode = AfxGetFakeFM2()->RegNodeFromHKEY(hKey);
		ASSERT(pNode != NULL);
		CRegTreeNode* pResult = pNode->OpenNode(lpSubKey);
		*phkResult = ToHKEY(pResult);
		return S_OK;
	}
	else
		return Real_RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

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
    )
{
	TRACE("RegQueryInfoKeyA\n");
	return E_NOTIMPL;
}
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
    )
{
	if (AfxGetFakeFM2()->m_bHooking)
	{
		CRegTreeNode* pNode = AfxGetFakeFM2()->RegNodeFromHKEY(hKey);
		ASSERT(pNode != NULL);
		if (lpcSubKeys != NULL)
			*lpcSubKeys = pNode->GetChildCount();
		if (lpcValues != NULL)
			*lpcValues = pNode->GetValueCount();
		return S_OK;
	}
	else
		return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegQueryValueA (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPSTR lpData,
    __inout_opt PLONG lpcbData
    )
{
	TRACE("RegQueryValueA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegQueryValueW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPWSTR lpData,
    __inout_opt PLONG lpcbData
    )
{
	TRACE("RegQueryValueW\n");
	return E_NOTIMPL;
}

#if(WINVER >= 0x0400)

LSTATUS
APIENTRY Mine_RegQueryMultipleValuesA (
    __in HKEY hKey,
    __out_ecount(num_vals) PVALENTA val_list,
    __in DWORD num_vals,
    __out_bcount_part_opt(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPSTR lpValueBuf,
    __inout_opt LPDWORD ldwTotsize
    )
{
	TRACE("RegQueryMultipleValuesA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegQueryMultipleValuesW (
    __in HKEY hKey,
    __out_ecount(num_vals) PVALENTW val_list,
    __in DWORD num_vals,
    __out_bcount_part_opt(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPWSTR lpValueBuf,
    __inout_opt LPDWORD ldwTotsize
    )
{
	TRACE("RegQueryMultipleValuesW\n");
	return E_NOTIMPL;
}
#endif /* WINVER >= 0x0400 */

LSTATUS
APIENTRY Mine_RegQueryValueExA (
    __in HKEY hKey,
    __in_opt LPCSTR lpValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    )
{
	TRACE("RegQueryValueExA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegQueryValueExW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    )
{
	if (AfxGetFakeFM2()->m_bHooking)
	{
		CRegTreeNode* pNode = AfxGetFakeFM2()->RegNodeFromHKEY(hKey);
		ASSERT(pNode != NULL);
		if (pNode->GetValue(lpValueName, lpType, lpData, lpcbData))
			return S_OK;
		else
			return REGDB_E_READREGDB;
	}
	else
		return Real_RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}

LSTATUS
APIENTRY Mine_RegReplaceKeyA (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __in LPCSTR lpNewFile,
    __in LPCSTR lpOldFile
    )
{
	TRACE("RegReplaceKeyA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegReplaceKeyW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __in LPCWSTR lpNewFile,
    __in LPCWSTR lpOldFile
    )
{
	TRACE("RegReplaceKeyW\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegRestoreKeyA (
    __in HKEY hKey,
    __in LPCSTR lpFile,
    __in DWORD dwFlags
    )
{
	TRACE("RegRestoreKeyA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegRestoreKeyW (
    __in HKEY hKey,
    __in LPCWSTR lpFile,
    __in DWORD dwFlags
    )
{
	TRACE("RegRestoreKeyW\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegSaveKeyA (
    __in HKEY hKey,
    __in LPCSTR lpFile,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes
    )
{
	TRACE("RegSaveKeyA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegSaveKeyW (
    __in HKEY hKey,
    __in LPCWSTR lpFile,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes
    )
{
	TRACE("RegSaveKeyW\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegSetKeySecurity (
    __in HKEY hKey,
    __in SECURITY_INFORMATION SecurityInformation,
    __in PSECURITY_DESCRIPTOR pSecurityDescriptor
    )
{
	TRACE("RegSetKeySecurity\n");
	return E_NOTIMPL;
}

LSTATUS
APIENTRY Mine_RegSetValueA (
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __in DWORD dwType,
    __in_bcount_opt(cbData) LPCSTR lpData,
    __in DWORD cbData
    )
{
	TRACE("RegSetValueA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegSetValueW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __in DWORD dwType,
    __in_bcount_opt(cbData) LPCWSTR lpData,
    __in DWORD cbData
    )
{
	HKEY hSubKey;
	
	if (SUCCEEDED(RegOpenKeyExW(hKey, lpSubKey, 0, KEY_SET_VALUE, &hSubKey)) && hSubKey != NULL)
		return RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE*)lpData, cbData);
	else
		return REGDB_E_KEYMISSING;
}


LSTATUS
APIENTRY Mine_RegSetValueExA (
    __in HKEY hKey,
    __in_opt LPCSTR lpValueName,
    __reserved DWORD Reserved,
    __in DWORD dwType,
    __in_bcount_opt(cbData) CONST BYTE* lpData,
    __in DWORD cbData
    )
{
	TRACE("RegSetValueExA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegSetValueExW (
    __in HKEY hKey,
    __in_opt LPCWSTR lpValueName,
    __reserved DWORD Reserved,
    __in DWORD dwType,
    __in_bcount_opt(cbData) CONST BYTE* lpData,
    __in DWORD cbData
    )
{
	if (AfxGetFakeFM2()->m_bHooking)
	{
		CRegTreeNode* pNode = AfxGetFakeFM2()->RegNodeFromHKEY(hKey);
		ASSERT(pNode != NULL);
		pNode->SetValue(lpValueName, dwType, lpData, cbData);
		return S_OK;
	}
	else
		return Real_RegSetValueExW(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}

LSTATUS
APIENTRY Mine_RegUnLoadKeyA (
    __in HKEY    hKey,
    __in_opt LPCSTR lpSubKey
    )
{
	TRACE("RegUnLoadKeyA\n");
	return E_NOTIMPL;
}
LSTATUS
APIENTRY Mine_RegUnLoadKeyW (
    __in HKEY    hKey,
    __in_opt LPCWSTR lpSubKey
    )
{
	TRACE("RegUnLoadKeyW\n");
	return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////

CFakeFilterMapper2* AfxGetFakeFM2(void)
{
	static CFakeFilterMapper2 fakeFM2;
	
	return &fakeFM2;
}

CFakeFilterMapper2::CFakeFilterMapper2( void )
: m_bHooking(false)
{
	m_rootKeys.insert(std::make_pair(HKEY_CLASSES_ROOT, L"HKEY_CLASSES_ROOT"));
	m_rootKeys.insert(std::make_pair(HKEY_CURRENT_USER, L"HKEY_CURRENT_USER"));
	m_rootKeys.insert(std::make_pair(HKEY_LOCAL_MACHINE, L"HKEY_LOCAL_MACHINE"));
	m_regDB.LoadFromFile(AfxGetExePath() + L"reg.db");
}

void CFakeFilterMapper2::Initialize( void )
{
	if (SUCCEEDED(Real_CoCreateInstance(CLSID_FilterMapper2, NULL, CLSCTX_INPROC_SERVER, IID_IFilterMapper2, (LPVOID*)&m_pFM2)))
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
		HookAPI(Real_RegCreateKeyA, Mine_RegCreateKeyA);
		HookAPI(Real_RegCreateKeyW, Mine_RegCreateKeyW);
		HookAPI(Real_RegCreateKeyExA, Mine_RegCreateKeyExA);
		HookAPI(Real_RegCreateKeyExW, Mine_RegCreateKeyExW);
		HookAPI(Real_RegDeleteKeyA, Mine_RegDeleteKeyA);
		HookAPI(Real_RegDeleteKeyW, Mine_RegDeleteKeyW);
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
		HookAPI(Real_RegQueryInfoKeyA, Mine_RegQueryInfoKeyA);
		HookAPI(Real_RegQueryInfoKeyW, Mine_RegQueryInfoKeyW);
		HookAPI(Real_RegQueryValueA, Mine_RegQueryValueA);
		HookAPI(Real_RegQueryValueW, Mine_RegQueryValueW);
#if(WINVER >= 0x0400)
		HookAPI(Real_RegQueryMultipleValuesA, Mine_RegQueryMultipleValuesA);
		HookAPI(Real_RegQueryMultipleValuesW, Mine_RegQueryMultipleValuesW);
#endif // WINVER >= 0x0400
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
	}
}

void CFakeFilterMapper2::Uninitialize( void )
{
	m_pFM2 = NULL;

	m_regDB.SaveToFile(AfxGetExePath() + L"reg.db");
}

HRESULT CFakeFilterMapper2::Register( LPCTSTR lpszFileName )
{
	HMODULE hDLL;
	HRESULT hr = E_FAIL;
	
	if ((hDLL = LoadLibrary(lpszFileName)) != NULL)
	{
		typedef HRESULT (__stdcall *DllRegisterServerFunc)(void);
		DllRegisterServerFunc pfnRegSvr;
		
		pfnRegSvr = (DllRegisterServerFunc)GetProcAddress(hDLL, "DllRegisterServer");
		if (pfnRegSvr != NULL)
		{
			m_lockHooking.Lock();
			m_bHooking = true;
			pfnRegSvr();
			m_bHooking = false;
			m_lockHooking.Unlock();
			hr = S_OK;
		}
		FreeLibrary(hDLL);
	}
	return hr;
}

CRegTreeNode* CFakeFilterMapper2::RegNodeFromHKEY( HKEY hKey )
{
	if (IsRootHKEY(hKey))
	{
		std::map<HKEY, CStringW>::const_iterator it = m_rootKeys.find(hKey);
		if (it != m_rootKeys.end())
			return m_regDB.GetChild(it->second);
		else
			return NULL;
	}
	else if (IsFakeHKEY(hKey))
	{
		return (CRegTreeNode*)((uint32_t)hKey & 0x3FFFFFFF);
	}
	else
		return NULL;	
}

HRESULT STDMETHODCALLTYPE CFakeFilterMapper2::CreateCategory( REFCLSID clsidCategory, DWORD dwCategoryMerit, LPCWSTR Description )
{
	TRACE("CFilterMapper2::CreateCategory\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CFakeFilterMapper2::UnregisterFilter( const CLSID *pclsidCategory, LPCOLESTR szInstance, REFCLSID Filter )
{
	TRACE("CFilterMapper2::UnregisterFilter\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CFakeFilterMapper2::RegisterFilter( REFCLSID clsidFilter, LPCWSTR Name, IMoniker **ppMoniker, const CLSID *pclsidCategory, LPCOLESTR szInstance, const REGFILTER2 *prf2 )
{
	TRACE("CFilterMapper2::RegisterFilter\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CFakeFilterMapper2::EnumMatchingFilters( IEnumMoniker **ppEnum, DWORD dwFlags, BOOL bExactMatch, DWORD dwMerit, BOOL bInputNeeded, DWORD cInputTypes, const GUID *pInputTypes, const REGPINMEDIUM *pMedIn, const CLSID *pPinCategoryIn, BOOL bRender, BOOL bOutputNeeded, DWORD cOutputTypes, const GUID *pOutputTypes, const REGPINMEDIUM *pMedOut, const CLSID *pPinCategoryOut )
{
	TRACE("CFilterMapper2::EnumMatchingFilters\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CFakeFilterMapper2::QueryInterface( REFIID riid, void** ppvObject )
{
	if (ppvObject == NULL)
		return E_POINTER;

	if (IsEqualIID(riid, IID_IFilterMapper2))
	{
		AddRef();
		*ppvObject = (IFilterMapper2*)this;
		return S_OK;
	}
	else if (IsEqualIID(riid, IID_IUnknown))
	{
		AddRef();
		*ppvObject = (IUnknown*)this;
		return S_OK;
	}
	else
		return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CFakeFilterMapper2::AddRef( void )
{
	return 1ul;
}

ULONG STDMETHODCALLTYPE CFakeFilterMapper2::Release( void )
{
	return 1ul;
}

