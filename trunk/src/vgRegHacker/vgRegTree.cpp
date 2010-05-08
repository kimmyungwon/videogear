#include "vgRegTree.hpp"
#include "vgUtil/vgWinDDK.hpp"

VG_NAMESPACE_BEGIN

#include "vgRegTree_Impl.inl"

RegTree& RegTree::GetInstance( void )
{
	static RegTree instance;
	return instance;
}

// LSTATUS APIENTRY RegTree::RegCloseKey(HKEY hKey);
// LSTATUS APIENTRY RegTree::RegCreateKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition);
// LSTATUS APIENTRY RegTree::RegEnumKeyExW(HKEY hKey, DWORD dwIndex, LPWSTR lpName, LPDWORD lpcchName, LPDWORD lpReserved, LPWSTR lpClass, LPDWORD lpcchClass, PFILETIME lpftLastWriteTime);
// LSTATUS APIENTRY RegTree::RegEnumValueW(HKEY hKey, DWORD dwIndex, LPWSTR lpValueName, LPDWORD lpcchValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
// LSTATUS APIENTRY RegTree::RegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
// LSTATUS APIENTRY RegTree::RegQueryInfoKeyW(HKEY hKey, LPWSTR lpClass, LPDWORD lpcchClass, LPDWORD lpReserved, LPDWORD lpcSubKeys, LPDWORD lpcbMaxSubKeyLen, LPDWORD lpcbMaxClassLen, LPDWORD lpcValues, LPDWORD lpcbMaxValueNameLen, LPDWORD lpcbMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime);

RegTree::RegTree( void )
{
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
		typedef tokenizer<char_separator<wchar_t>, wstring::const_iterator, wstring> tokenizer;
		char_separator<wchar_t> sep(L"\\");
		tokenizer tokens(subKey, sep);
		path.m_segments.clear();
		for (tokenizer::const_iterator iter = tokens.begin(); iter != tokens.end(); iter++)
			path.m_segments.push_back(*iter);
	}

	return true;
}


VG_NAMESPACE_END

