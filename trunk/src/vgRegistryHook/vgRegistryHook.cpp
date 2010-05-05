#include "stdafx.h"
#include "vgRegistryHook.h"
#include "vgHook.h"

#define DECLARE_API(name)	BOOST_TYPEOF(&name) Real_##name = name
#define HOOK_API(name)		Real_##name = m_codeHook->Hook(Real_##name, Mine_##name)
#define UNHOOK_API(name)	m_codeHook->Unhook(Mine_##name)

//DECLARE_API(RegCreateKeyA);
//DECLARE_API(RegCreateKeyW);
DECLARE_API(RegCreateKeyExA);
DECLARE_API(RegCreateKeyExW);

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

vgRegistryHook& vgRegistryHook::Instance( void )
{
	static vgRegistryHook instance;
	return instance;
}

vgRegistryHook::~vgRegistryHook( void )
{
	delete m_codeHook;
}

void vgRegistryHook::Enable( void )
{
	HOOK_API(RegCreateKeyExA);
	HOOK_API(RegCreateKeyExW);	
}

void vgRegistryHook::Disable( void )
{
	m_codeHook->UnhookAll();	
}

vgRegistryHook::vgRegistryHook( void )
{
	m_codeHook = new vgHook;
}