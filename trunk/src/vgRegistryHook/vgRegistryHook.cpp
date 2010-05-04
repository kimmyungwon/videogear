#include "stdafx.h"
#include <NCodeHookInstantiation.h>

#define DECLARE_API(name)	BOOST_TYPEOF(&name) Real_##name = name
#define HOOK_API(name)		Real_##name = g_codeHook.createHook(Real_##name, Mine_##name)
#define UNHOOK_API(name)	g_codeHook.removeHook(Mine_##name)

//DECLARE_API(RegCreateKeyA);
//DECLARE_API(RegCreateKeyW);
DECLARE_API(RegCreateKeyExA);
DECLARE_API(RegCreateKeyExW);

#ifdef WIN64
static NCodeHookX64 g_codeHook;
#else
static NCodeHookIA32 g_codeHook;
#endif

LSTATUS APIENTRY Mine_RegCreateKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, 
									  CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition)
{
	LSTATUS ret = Real_RegCreateKeyExA(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, 
		lpdwDisposition);
	if (ret == ERROR_SUCCESS)
	{
		
	}
	return ret;
}

LSTATUS APIENTRY Mine_RegCreateKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, 
									  CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition)
{
	LSTATUS ret = Real_RegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, 
		lpdwDisposition);
	if (ret == ERROR_SUCCESS)
	{

	}
	return ret;
}

void vgRHStartup(void)
{
	//HOOK_API(RegCreateKeyA);
	//HOOK_API(RegCreateKeyW);
	HOOK_API(RegCreateKeyExA);
	HOOK_API(RegCreateKeyExW);
}

void vgRHShutdown(void)
{
	
}