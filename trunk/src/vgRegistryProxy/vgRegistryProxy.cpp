#include "StdAfx.h"
#include "vgRegistryProxy.h"
#include "vgHook.h"

#define HOOK_API(name)		Real_##name = m_codeHook->Hook(Real_##name, Mine_##name)
#define UNHOOK_API(name)	m_codeHook->Unhook(Real_##name, Mine_##name)

vgRegistryProxy& vgRegistryProxy::GetInstance( void )
{
	static vgRegistryProxy instance;
	return instance;
}

vgRegistryProxy::~vgRegistryProxy(void)
{
	Disable();
	delete m_codeHook;
	m_codeHook = NULL;
}

void vgRegistryProxy::Disable( void )
{
	UNHOOK_API(RegCloseKey);
	UNHOOK_API(RegCreateKeyExW);
	UNHOOK_API(RegOpenKeyExW);
}

void vgRegistryProxy::Enable( void )
{
	HOOK_API(RegCloseKey);
	HOOK_API(RegCreateKeyExW);
	HOOK_API(RegOpenKeyExW);
}

vgRegistryProxy::vgRegistryProxy(void)
{
	m_codeHook = new vgHook;
}

LSTATUS APIENTRY vgRegistryProxy::Mine_RegCloseKey( HKEY hKey )
{
	vgRegistryNode *node = vgRegistryNode::FromKey(hKey);
	if (node != NULL) 
		return ERROR_SUCCESS;
	else
		return Real_RegCloseKey(hKey);
}

LSTATUS APIENTRY vgRegistryProxy::Mine_RegCreateKeyExW( HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition )
{
	vgRegistryNode *node = GetInstance().CreateNode(hKey, lpSubKey);
	*phkResult = node->AsKey();
	return ERROR_SUCCESS;
}

LSTATUS APIENTRY vgRegistryProxy::Mine_RegOpenKeyExW( HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult )
{
	return Real_RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}