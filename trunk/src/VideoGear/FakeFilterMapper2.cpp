#include "StdAfx.h"
#include "FakeFilterMapper2.h"
#include "APIHook.h"

HRESULT 
(STDMETHODCALLTYPE *Real_CoCreateInstance)(
   __in REFCLSID rclsid, 
   __in_opt LPUNKNOWN pUnkOuter,
   __in DWORD dwClsContext, 
   __in REFIID riid, 
   __deref_out LPVOID FAR* ppv
   ) = CoCreateInstance ;

LSTATUS
(APIENTRY *Real_RegCloseKey )(
    __in HKEY hKey
    ) = RegCloseKey ;

LSTATUS
(APIENTRY *Real_RegOverridePredefKey )(
    __in HKEY hKey,
    __in_opt HKEY hNewHKey
    ) = RegOverridePredefKey ;

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
(APIENTRY *Real_RegConnectRegistryA )(
    __in_opt LPCSTR lpMachineName,
    __in HKEY hKey,
    __out PHKEY phkResult
    ) = RegConnectRegistryA ;
LSTATUS
(APIENTRY *Real_RegConnectRegistryW )(
    __in_opt LPCWSTR lpMachineName,
    __in HKEY hKey,
    __out PHKEY phkResult
    ) = RegConnectRegistryW ;

LSTATUS
(APIENTRY *Real_RegCreateKeyA )(
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __out PHKEY phkResult
    ) = RegCreateKeyA ;
LSTATUS
(APIENTRY *Real_RegCreateKeyW )(
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __out PHKEY phkResult
    ) = RegCreateKeyW ;

LSTATUS
(APIENTRY *Real_RegCreateKeyExA )(
    __in HKEY hKey,
    __in LPCSTR lpSubKey,
    __reserved DWORD Reserved,
    __in_opt LPSTR lpClass,
    __in DWORD dwOptions,
    __in REGSAM samDesired,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __out PHKEY phkResult,
    __out_opt LPDWORD lpdwDisposition
    ) = RegCreateKeyExA ;
LSTATUS
(APIENTRY *Real_RegCreateKeyExW )(
    __in HKEY hKey,
    __in LPCWSTR lpSubKey,
    __reserved DWORD Reserved,
    __in_opt LPWSTR lpClass,
    __in DWORD dwOptions,
    __in REGSAM samDesired,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __out PHKEY phkResult,
    __out_opt LPDWORD lpdwDisposition
    ) = RegCreateKeyExW ;

LSTATUS
(APIENTRY *Real_RegDeleteKeyA )(
    __in HKEY hKey,
    __in LPCSTR lpSubKey
    ) = RegDeleteKeyA ;
LSTATUS
(APIENTRY *Real_RegDeleteKeyW )(
    __in HKEY hKey,
    __in LPCWSTR lpSubKey
    ) = RegDeleteKeyW ;
    
LSTATUS
(APIENTRY *Real_RegDeleteValueA )(
    __in HKEY hKey,
    __in_opt LPCSTR lpValueName
    ) = RegDeleteValueA ;
LSTATUS
(APIENTRY *Real_RegDeleteValueW )(
    __in HKEY hKey,
    __in_opt LPCWSTR lpValueName
    ) = RegDeleteValueW ;

LSTATUS
(APIENTRY *Real_RegEnumKeyA )(
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(cchName,cchName + 1) LPSTR lpName,
    __in DWORD cchName
    ) = RegEnumKeyA ;
LSTATUS
(APIENTRY *Real_RegEnumKeyW )(
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(cchName,cchName + 1) LPWSTR lpName,
    __in DWORD cchName
    ) = RegEnumKeyW ;

LSTATUS
(APIENTRY *Real_RegEnumKeyExA )(
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(*lpcchName, *lpcchName + 1) LPSTR lpName,
    __inout LPDWORD lpcchName,
    __reserved LPDWORD lpReserved,
    __out_ecount_part_opt(*lpcchClass,*lpcchClass + 1) LPSTR lpClass,
    __inout_opt LPDWORD lpcchClass,
    __out_opt PFILETIME lpftLastWriteTime
    ) = RegEnumKeyExA ;
LSTATUS
(APIENTRY *Real_RegEnumKeyExW )(
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(*lpcchName, *lpcchName + 1) LPWSTR lpName,
    __inout LPDWORD lpcchName,
    __reserved LPDWORD lpReserved,
    __out_ecount_part_opt(*lpcchClass,*lpcchClass + 1) LPWSTR lpClass,
    __inout_opt LPDWORD lpcchClass,
    __out_opt PFILETIME lpftLastWriteTime
    ) = RegEnumKeyExW ;

LSTATUS
(APIENTRY *Real_RegEnumValueA )(
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(*lpcchValueName, *lpcchValueName + 1) LPSTR lpValueName,
    __inout LPDWORD lpcchValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    ) = RegEnumValueA ;
LSTATUS
(APIENTRY *Real_RegEnumValueW )(
    __in HKEY hKey,
    __in DWORD dwIndex,
    __out_ecount_part_opt(*lpcchValueName, *lpcchValueName + 1) LPWSTR lpValueName,
    __inout LPDWORD lpcchValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    ) = RegEnumValueW ;

LSTATUS
(APIENTRY *Real_RegFlushKey )(
    __in HKEY hKey
    ) = RegFlushKey ;

LSTATUS
(APIENTRY *Real_RegGetKeySecurity )(
    __in HKEY hKey,
    __in SECURITY_INFORMATION SecurityInformation,
    __out_bcount_opt(*lpcbSecurityDescriptor) PSECURITY_DESCRIPTOR pSecurityDescriptor,
    __inout LPDWORD lpcbSecurityDescriptor
    ) = RegGetKeySecurity ;

LSTATUS
(APIENTRY *Real_RegLoadKeyA )(
    __in HKEY    hKey,
    __in_opt LPCSTR  lpSubKey,
    __in LPCSTR  lpFile
    ) = RegLoadKeyA ;
LSTATUS
(APIENTRY *Real_RegLoadKeyW )(
    __in HKEY    hKey,
    __in_opt LPCWSTR  lpSubKey,
    __in LPCWSTR  lpFile
    ) = RegLoadKeyW ;

LSTATUS
(APIENTRY *Real_RegNotifyChangeKeyValue )(
    __in HKEY hKey,
    __in BOOL bWatchSubtree,
    __in DWORD dwNotifyFilter,
    __in_opt HANDLE hEvent,
    __in BOOL fAsynchronous
    ) = RegNotifyChangeKeyValue ;

LSTATUS
(APIENTRY *Real_RegOpenKeyA )(
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __out PHKEY phkResult
    ) = RegOpenKeyA ;
LSTATUS
(APIENTRY *Real_RegOpenKeyW )(
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __out PHKEY phkResult
    ) = RegOpenKeyW ;

LSTATUS
(APIENTRY *Real_RegOpenKeyExA )(
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __reserved DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult
    ) = RegOpenKeyExA ;
LSTATUS
(APIENTRY *Real_RegOpenKeyExW )(
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __reserved DWORD ulOptions,
    __in REGSAM samDesired,
    __out PHKEY phkResult
    ) = RegOpenKeyExW ;

LSTATUS
(APIENTRY *Real_RegQueryInfoKeyA )(
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
    ) = RegQueryInfoKeyA ;
LSTATUS
(APIENTRY *Real_RegQueryInfoKeyW )(
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
    ) = RegQueryInfoKeyW ;

LSTATUS
(APIENTRY *Real_RegQueryValueA )(
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPSTR lpData,
    __inout_opt PLONG lpcbData
    ) = RegQueryValueA ;
LSTATUS
(APIENTRY *Real_RegQueryValueW )(
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPWSTR lpData,
    __inout_opt PLONG lpcbData
    ) = RegQueryValueW ;

#if(WINVER >= 0x0400)

LSTATUS
(APIENTRY *Real_RegQueryMultipleValuesA )(
    __in HKEY hKey,
    __out_ecount(num_vals) PVALENTA val_list,
    __in DWORD num_vals,
    __out_bcount_part_opt(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPSTR lpValueBuf,
    __inout_opt LPDWORD ldwTotsize
    ) = RegQueryMultipleValuesA ;
LSTATUS
(APIENTRY *Real_RegQueryMultipleValuesW )(
    __in HKEY hKey,
    __out_ecount(num_vals) PVALENTW val_list,
    __in DWORD num_vals,
    __out_bcount_part_opt(*ldwTotsize, *ldwTotsize) __out_data_source(REGISTRY) LPWSTR lpValueBuf,
    __inout_opt LPDWORD ldwTotsize
    ) = RegQueryMultipleValuesW ;
#endif /* WINVER >= 0x0400 */

LSTATUS
(APIENTRY *Real_RegQueryValueExA )(
    __in HKEY hKey,
    __in_opt LPCSTR lpValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    ) = RegQueryValueExA ;
LSTATUS
(APIENTRY *Real_RegQueryValueExW )(
    __in HKEY hKey,
    __in_opt LPCWSTR lpValueName,
    __reserved LPDWORD lpReserved,
    __out_opt LPDWORD lpType,
    __out_bcount_part_opt(*lpcbData, *lpcbData) __out_data_source(REGISTRY) LPBYTE lpData,
    __inout_opt LPDWORD lpcbData
    ) = RegQueryValueExW ;

LSTATUS
(APIENTRY *Real_RegReplaceKeyA )(
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __in LPCSTR lpNewFile,
    __in LPCSTR lpOldFile
    ) = RegReplaceKeyA ;
LSTATUS
(APIENTRY *Real_RegReplaceKeyW )(
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __in LPCWSTR lpNewFile,
    __in LPCWSTR lpOldFile
    ) = RegReplaceKeyW ;

LSTATUS
(APIENTRY *Real_RegRestoreKeyA )(
    __in HKEY hKey,
    __in LPCSTR lpFile,
    __in DWORD dwFlags
    ) = RegRestoreKeyA ;
LSTATUS
(APIENTRY *Real_RegRestoreKeyW )(
    __in HKEY hKey,
    __in LPCWSTR lpFile,
    __in DWORD dwFlags
    ) = RegRestoreKeyW ;

LSTATUS
(APIENTRY *Real_RegSaveKeyA )(
    __in HKEY hKey,
    __in LPCSTR lpFile,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes
    ) = RegSaveKeyA ;
LSTATUS
(APIENTRY *Real_RegSaveKeyW )(
    __in HKEY hKey,
    __in LPCWSTR lpFile,
    __in_opt CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes
    ) = RegSaveKeyW ;

LSTATUS
(APIENTRY *Real_RegSetKeySecurity )(
    __in HKEY hKey,
    __in SECURITY_INFORMATION SecurityInformation,
    __in PSECURITY_DESCRIPTOR pSecurityDescriptor
    ) = RegSetKeySecurity ;

LSTATUS
(APIENTRY *Real_RegSetValueA )(
    __in HKEY hKey,
    __in_opt LPCSTR lpSubKey,
    __in DWORD dwType,
    __in_bcount_opt(cbData) LPCSTR lpData,
    __in DWORD cbData
    ) = RegSetValueA ;
LSTATUS
(APIENTRY *Real_RegSetValueW )(
    __in HKEY hKey,
    __in_opt LPCWSTR lpSubKey,
    __in DWORD dwType,
    __in_bcount_opt(cbData) LPCWSTR lpData,
    __in DWORD cbData
    ) = RegSetValueW ;


LSTATUS
(APIENTRY *Real_RegSetValueExA )(
    __in HKEY hKey,
    __in_opt LPCSTR lpValueName,
    __reserved DWORD Reserved,
    __in DWORD dwType,
    __in_bcount_opt(cbData) CONST BYTE* lpData,
    __in DWORD cbData
    ) = RegSetValueExA ;
LSTATUS
(APIENTRY *Real_RegSetValueExW )(
    __in HKEY hKey,
    __in_opt LPCWSTR lpValueName,
    __reserved DWORD Reserved,
    __in DWORD dwType,
    __in_bcount_opt(cbData) CONST BYTE* lpData,
    __in DWORD cbData
    ) = RegSetValueExW ;

LSTATUS
(APIENTRY *Real_RegUnLoadKeyA )(
    __in HKEY    hKey,
    __in_opt LPCSTR lpSubKey
    ) = RegUnLoadKeyA ;
LSTATUS
(APIENTRY *Real_RegUnLoadKeyW )(
    __in HKEY    hKey,
    __in_opt LPCWSTR lpSubKey
    ) = RegUnLoadKeyW ;

//////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE Mine_CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, 
	LPVOID* ppv)
{
	if (CFakeFilterMapper2::m_pFilterMapper != NULL)
	{
		CheckPointer(ppv, E_POINTER);
		
		if (rclsid == CLSID_FilterMapper)
		{
			*ppv = NULL;
			return REGDB_E_CLASSNOTREG;
		}
		else if (rclsid == CLSID_FilterMapper2)
		{
			if (pUnkOuter != NULL)
				return CLASS_E_NOAGGREGATION;

			if (riid == IID_IUnknown)
			{
				CFakeFilterMapper2::m_pFilterMapper->AddRef();
				*ppv = (IUnknown*)CFakeFilterMapper2::m_pFilterMapper;
				return S_OK;
			}
			else if (riid == IID_IFilterMapper2)
			{
				CFakeFilterMapper2::m_pFilterMapper->AddRef();
				*ppv = (IFilterMapper2*)CFakeFilterMapper2::m_pFilterMapper;
				return S_OK;
			}
			else
				return E_NOINTERFACE;
		}
	}

	return Real_CoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
}

IFilterMapper2* CFakeFilterMapper2::m_pFilterMapper = NULL;

void CFakeFilterMapper2::Initialize( void )
{
	static bool bInitialized = false;
	
	if (!bInitialized)
	{
		HookAPI(Real_CoCreateInstance, Mine_CoCreateInstance);
		bInitialized = true;
	}	
}

CFakeFilterMapper2::CFakeFilterMapper2( LPUNKNOWN pUnk )
: CUnknown(_T("CFakeFilterMapper2"), pUnk)
{
	Real_CoCreateInstance(CLSID_FilterMapper2, NULL, CLSCTX_INPROC_SERVER, IID_IFilterMapper2, (LPVOID*)&m_pFM2);
}

CFakeFilterMapper2::~CFakeFilterMapper2( void )
{
	m_pFM2 = NULL;
}

HRESULT CFakeFilterMapper2::Register( LPCTSTR lpszFile )
{
	HMODULE hDLL;
	typedef HRESULT (STDMETHODCALLTYPE *DllRegSvr)(void);
	DllRegSvr pfnDllRegSvr;
	
	hDLL = LoadLibrary(lpszFile);
	if (hDLL == NULL)
		return HRESULT_FROM_WIN32(GetLastError());
	pfnDllRegSvr = (DllRegSvr)GetProcAddress(hDLL, "DllRegisterServer");
	if (pfnDllRegSvr == NULL)
		return E_FAIL;
	m_pFilterMapper = (IFilterMapper2*)this;
	pfnDllRegSvr();
	m_pFilterMapper = NULL;
	FreeLibrary(hDLL);
	return S_OK;
}

STDMETHODIMP CFakeFilterMapper2::CreateCategory( REFCLSID clsidCategory, DWORD dwCategoryMerit, LPCWSTR Description )
{
	return m_pFM2->CreateCategory(clsidCategory, dwCategoryMerit, Description);
}

STDMETHODIMP CFakeFilterMapper2::UnregisterFilter( const CLSID *pclsidCategory, LPCOLESTR szInstance, REFCLSID Filter )
{
	return m_pFM2->UnregisterFilter(pclsidCategory, szInstance, Filter);
}

STDMETHODIMP CFakeFilterMapper2::RegisterFilter( REFCLSID clsidFilter, LPCWSTR Name, IMoniker **ppMoniker, const CLSID *pclsidCategory, LPCOLESTR szInstance, const REGFILTER2 *prf2 )
{
	return m_pFM2->RegisterFilter(clsidFilter, Name, ppMoniker, pclsidCategory, szInstance, prf2);
}

STDMETHODIMP CFakeFilterMapper2::EnumMatchingFilters( IEnumMoniker **ppEnum, DWORD dwFlags, BOOL bExactMatch, DWORD dwMerit, BOOL bInputNeeded, DWORD cInputTypes, const GUID *pInputTypes, const REGPINMEDIUM *pMedIn, const CLSID *pPinCategoryIn, BOOL bRender, BOOL bOutputNeeded, DWORD cOutputTypes, const GUID *pOutputTypes, const REGPINMEDIUM *pMedOut, const CLSID *pPinCategoryOut )
{
	return m_pFM2->EnumMatchingFilters(ppEnum, dwFlags, bExactMatch, dwMerit, bInputNeeded, cInputTypes, pInputTypes,
		pMedIn, pPinCategoryIn, bRender, bOutputNeeded, cOutputTypes, pOutputTypes, pMedOut, pPinCategoryOut);
}

