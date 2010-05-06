#pragma once

#include "vgInclude.h"
#include "vgRegistry.h"

class vgHook;

class vgRegistryProxy : public vgRegistry
{
public:
	static vgRegistryProxy& GetInstance(void);

	~vgRegistryProxy(void);

	void Disable(void);
	void Enable(void);
protected:
	vgRegistryProxy(void);
private:
	static LSTATUS APIENTRY Mine_RegCloseKey(HKEY hKey);
	static LSTATUS APIENTRY Mine_RegCreateKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, 
		REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition);
	static LSTATUS APIENTRY Mine_RegCreateKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, 
		REGSAM samDesired, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition);
	static LSTATUS APIENTRY Mine_RegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
	static LSTATUS APIENTRY Mine_RegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
private:
	vgHook *m_codeHook;
};
