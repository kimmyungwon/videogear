#include "vgRegTree.hpp"
#include "vgUtil/vgConsole.h"
#include "vgUtil/vgHook.hpp"
#include "vgUtil/vgString.hpp"
#include "vgUtil/vgWinDDK.hpp"

#define HOOK(api)	m_hook->Hook(Real_##api, Mine_##api);
#define UNHOOK(api)	m_hook->Unhook(Real_##api, Mine_##api);	

VG_NAMESPACE_BEGIN

#include "vgRegTree_Impl.inl"

RegTree& RegTree::GetInstance( void )
{
	static RegTree instance;
	return instance;
}

RegTree::~RegTree( void )
{
	delete m_hook;
	m_hook = NULL;
}

void RegTree::Hook( void )
{	
	HOOK(RegCloseKey);	
	HOOK(RegCreateKeyExW);
	HOOK(RegEnumKeyExW);
	HOOK(RegEnumValueW);
	HOOK(RegOpenKeyExW);
	HOOK(RegQueryInfoKeyW);
	HOOK(RegQueryValueExW);
}

void RegTree::Unhook( void )
{
	UNHOOK(RegCloseKey);	
	UNHOOK(RegCreateKeyExW);
	UNHOOK(RegEnumKeyExW);
	UNHOOK(RegEnumValueW);
	UNHOOK(RegOpenKeyExW);
	UNHOOK(RegQueryInfoKeyW);
	UNHOOK(RegQueryValueExW);
}

LSTATUS APIENTRY RegTree::RegCloseKey(HKEY hKey)
{
	RegPath path;
	ResolveKEY(hKey, path);
	Console::GetInstance().Print(L"CloseKey: %s\n", path.ToString().c_str());
	
	return Real_RegCloseKey(hKey);
}

LSTATUS APIENTRY RegTree::RegCreateKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition)
{
	RegPath path;
	ResolveKEY(hKey, lpSubKey, path);
	Console::GetInstance().Print(L"CreateKey: %s\n", path.ToString().c_str());
	
	return Real_RegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}

LSTATUS APIENTRY RegTree::RegEnumKeyExW(HKEY hKey, DWORD dwIndex, LPWSTR lpName, LPDWORD lpcchName, LPDWORD lpReserved, LPWSTR lpClass, LPDWORD lpcchClass, PFILETIME lpftLastWriteTime)
{
	RegPath path;
	ResolveKEY(hKey, path);
	Console::GetInstance().Print(L"EnumKey: %s\n", path.ToString().c_str());
	
	return Real_RegEnumKeyExW(hKey, dwIndex, lpName, lpcchName, lpReserved, lpClass, lpcchClass, lpftLastWriteTime);
}

LSTATUS APIENTRY RegTree::RegEnumValueW(HKEY hKey, DWORD dwIndex, LPWSTR lpValueName, LPDWORD lpcchValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	RegPath path;
	ResolveKEY(hKey, path);
	Console::GetInstance().Print(L"EnumValue: %s\n", path.ToString().c_str());
	
	return Real_RegEnumValueW(hKey, dwIndex, lpValueName, lpcchValueName, lpReserved, lpType, lpData, lpcbData);
}

LSTATUS APIENTRY RegTree::RegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
	RegPath path;
	ResolveKEY(hKey, lpSubKey, path);
	Console::GetInstance().Print(L"OpenKey: %s\n", path.ToString().c_str());
	
	return Real_RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

LSTATUS APIENTRY RegTree::RegQueryInfoKeyW(HKEY hKey, LPWSTR lpClass, LPDWORD lpcchClass, LPDWORD lpReserved, LPDWORD lpcSubKeys, LPDWORD lpcbMaxSubKeyLen, LPDWORD lpcbMaxClassLen, LPDWORD lpcValues, LPDWORD lpcbMaxValueNameLen, LPDWORD lpcbMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime)
{
	RegPath path;
	ResolveKEY(hKey, path);
	Console::GetInstance().Print(L"QueryInfoKey: %s\n", path.ToString().c_str());
	
	return Real_RegQueryInfoKeyW(hKey, lpClass, lpcchClass, lpReserved, lpcSubKeys, lpcbMaxSubKeyLen, lpcbMaxClassLen, lpcValues, lpcbMaxValueNameLen, lpcbMaxValueLen, lpcbSecurityDescriptor, lpftLastWriteTime);
}

LSTATUS APIENTRY RegTree::RegQueryValueExW(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	RegPath path;
	ResolveKEY(hKey, path);
	Console::GetInstance().Print(L"QueryValue: %s\n", path.ToString().c_str());
	
	return Real_RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}

RegTree::RegTree( void )
{
	m_hook = new CodeHook;
	
	HKEY key;
	Real_RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &key);
	RegPath path;
	ResolveKEY(key, path);
	Real_RegCloseKey(key);

	RegNode node1; node1.m_rootKey = HKEY_LOCAL_MACHINE;
	RegNode node2; node2.m_rootKey = HKEY_LOCAL_MACHINE; node2.m_parent = &node1; node2.m_name = L"SOFTWARE";
	RegNode node3; node3.m_rootKey = HKEY_LOCAL_MACHINE; node3.m_parent = &node2; node3.m_name = L"Microsoft";
	ResolveKEY((HKEY)((int)&node3 | 0x40000000), path);
}

bool RegTree::ResolveKEY( HKEY key, RegPath &path )
{
	if ((int)key >= 0x80000000)
	{
		path.m_rootKey = key;
		path.m_segments.clear();
	}
	else if ((int)key >= 0x40000000)
	{
		RegNode *node = (RegNode*)((int)key & ~0x40000000);
		path.m_rootKey = node->m_rootKey;
		path.m_segments.clear();
		node->GetSubKey(path.m_segments);
	}
	else
	{
		BYTE result[1024];
		DWORD resultSize;
		if (WinDDK::GetInstance().ZwQueryKey(key, KeyNameInformation, result, sizeof(result), &resultSize) != STATUS_SUCCESS)
			return false;

		KEY_BASIC_INFORMATION &info = *(KEY_BASIC_INFORMATION*)result;
		wstring keyPath(info.Name, (resultSize - sizeof(KEY_BASIC_INFORMATION)) / sizeof(WCHAR) + 1);

		// 去掉开头的"TRY\"
		if (!starts_with(keyPath, L"TRY\\"))
			return false;
		keyPath = keyPath.substr(4);
		// 解析Root
		size_t sepPos = keyPath.find(L'\\');
		wstring rootName = keyPath.substr(0, sepPos);
		if (rootName == L"USER")
			path.m_rootKey = HKEY_USERS;
		else if (rootName == L"MACHINE")
			path.m_rootKey = HKEY_LOCAL_MACHINE;
		else
			return false;
		// 取得SubKey
		wstring subKey = keyPath.substr(sepPos + 1);
		// 分割SubKey
		path.m_segments.clear();
		String::Split(subKey, L"\\", path.m_segments);
	}

	return true;
}

bool RegTree::ResolveKEY( HKEY key, const wstring &subKey, RegPath &path )
{
	RegPath result;
	if (!ResolveKEY(key, result))
		return false;
	path = result;
	String::Split(subKey, L"\\", path.m_segments);
	return true;
}

VG_NAMESPACE_END

