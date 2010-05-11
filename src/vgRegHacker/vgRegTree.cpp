#include "vgRegTree.hpp"
#include "vgUtil/vgConsole.h"
#include "vgUtil/vgHook.hpp"
#include "vgUtil/vgString.hpp"
#include "vgUtil/vgWinDDK.hpp"

#define MAX_KEY_LENGTH 255
#define HOOK(api)	m_hook->Hook(Real_##api, Mine_##api);
#define UNHOOK(api)	m_hook->Unhook(Real_##api, Mine_##api);	

VG_NAMESPACE_BEGIN

RegNodeType RegNodeTypeGetter::operator()( RegNode *node ) const
{
	return node->m_type;
}

wstring RegNodeNameGetter::operator()( RegNode *node ) const
{
	return to_lower_copy(node->m_name);
}

//////////////////////////////////////////////////////////////////////////

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
	HOOK(RegDeleteKeyW);
	HOOK(RegDeleteValueW);
	HOOK(RegEnumKeyW);
	HOOK(RegEnumKeyExW);
	HOOK(RegEnumValueW);
	HOOK(RegFlushKey);
	HOOK(RegGetKeySecurity);
	HOOK(RegLoadKeyW);
	HOOK(RegNotifyChangeKeyValue);
	HOOK(RegOpenKeyExW);
	HOOK(RegOverridePredefKey);
	HOOK(RegQueryInfoKeyW);
	HOOK(RegQueryMultipleValuesW);
	HOOK(RegQueryValueExW);
	HOOK(RegReplaceKeyW);
	HOOK(RegRestoreKeyW);
	HOOK(RegSaveKeyW);
	HOOK(RegSaveKeyExW);
	HOOK(RegSetKeySecurity);
	HOOK(RegSetValueExW);
	HOOK(RegUnLoadKeyW);
}

void RegTree::Unhook( void )
{
	UNHOOK(RegCloseKey);
	UNHOOK(RegCreateKeyExW);
	UNHOOK(RegDeleteKeyW);
	UNHOOK(RegDeleteValueW);
	UNHOOK(RegEnumKeyW);
	UNHOOK(RegEnumKeyExW);
	UNHOOK(RegEnumValueW);
	UNHOOK(RegFlushKey);
	UNHOOK(RegGetKeySecurity);
	UNHOOK(RegLoadKeyW);
	UNHOOK(RegNotifyChangeKeyValue);
	UNHOOK(RegOpenKeyExW);
	UNHOOK(RegOverridePredefKey);
	UNHOOK(RegQueryInfoKeyW);
	UNHOOK(RegQueryMultipleValuesW);
	UNHOOK(RegQueryValueExW);
	UNHOOK(RegReplaceKeyW);
	UNHOOK(RegRestoreKeyW);
	UNHOOK(RegSaveKeyW);
	UNHOOK(RegSaveKeyExW);
	UNHOOK(RegSetKeySecurity);
	UNHOOK(RegSetValueExW);
	UNHOOK(RegUnLoadKeyW);
}

LSTATUS APIENTRY RegTree::RegCloseKey(HKEY hKey)
{
	if (IsVirtualKey(hKey))
		return ERROR_SUCCESS;	
	else
		return Real_RegCloseKey(hKey);
}

LSTATUS APIENTRY RegTree::RegCreateKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition)
{
	RegPath path;
	if (ResolveKey(hKey, lpSubKey, path))
	{
		RegNode *node = CreateKey(path, lpdwDisposition);
		if (node != NULL)
		{
			*phkResult = node->AsKey();
			return ERROR_SUCCESS;
		}
		else
			return ERROR_ACCESS_DENIED;
	}
	else
		return Real_RegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}

LSTATUS APIENTRY RegTree::RegDeleteKeyW(HKEY hKey, LPCWSTR lpSubKey)
{
	RegPath path;
	ResolveKey(hKey, lpSubKey, path);
	Console::GetInstance().Print(L"DeleteKey: %s\n", path.ToString().c_str());
	
	return Real_RegDeleteKeyW(hKey, lpSubKey);
}

LSTATUS APIENTRY RegTree::RegDeleteValueW(HKEY hKey, LPCWSTR lpValueName)
{
	RegPath path;
	ResolveKey(hKey, path);
	Console::GetInstance().Print(L"DeleteValue: %s\n", path.ToString().c_str());
	
	return Real_RegDeleteValueW(hKey, lpValueName);
}

LSTATUS APIENTRY RegTree::RegEnumKeyW( HKEY hKey, DWORD dwIndex, LPWSTR lpName, DWORD cchName )
{
	if (IsVirtualKey(hKey))
	{
		RegNode *node = (RegNode*)((int)hKey & ~0x40000000);
		if (dwIndex == 0)
			LoadRealChildren(node);

		if (dwIndex < node->m_children.size())
		{
			RegNode *childNode = node->m_children[dwIndex];
			wcscpy_s(lpName, cchName, childNode->m_name.c_str());
		
			return ERROR_SUCCESS;
		}
		else
			return ERROR_NO_MORE_ITEMS;
	}
	else
		return Real_RegEnumKeyW(hKey, dwIndex, lpName, cchName);
}

LSTATUS APIENTRY RegTree::RegEnumKeyExW(HKEY hKey, DWORD dwIndex, LPWSTR lpName, LPDWORD lpcchName, LPDWORD lpReserved, LPWSTR lpClass, LPDWORD lpcchClass, PFILETIME lpftLastWriteTime)
{
	if (IsVirtualKey(hKey))
	{
		RegNode *node = (RegNode*)((int)hKey & ~0x40000000);
		LoadRealChildren(node);

		if (dwIndex < node->m_children.size())
		{
			wcscpy_s(lpName, *lpcchName, node->m_name.c_str());
			*lpcchName = node->m_name.length();

			return ERROR_SUCCESS;
		}
		else
			return ERROR_NO_MORE_ITEMS;
	}
	else
		return Real_RegEnumKeyExW(hKey, dwIndex, lpName, lpcchName, lpReserved, lpClass, lpcchClass, lpftLastWriteTime);
}

LSTATUS APIENTRY RegTree::RegEnumValueW(HKEY hKey, DWORD dwIndex, LPWSTR lpValueName, LPDWORD lpcchValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	RegPath path;
	ResolveKey(hKey, path);
	Console::GetInstance().Print(L"EnumValue: %s\n", path.ToString().c_str());
	
	//return Real_RegEnumValueW(hKey, dwIndex, lpValueName, lpcchValueName, lpReserved, lpType, lpData, lpcbData);
	return ERROR_NO_MORE_ITEMS;
}

LSTATUS APIENTRY RegTree::RegFlushKey(HKEY hKey)
{
	RegPath path;
	ResolveKey(hKey, path);
	Console::GetInstance().Print(L"FlushKey: %s\n", path.ToString().c_str());

	return Real_RegFlushKey(hKey);
}

LSTATUS APIENTRY RegTree::RegGetKeySecurity(HKEY hKey, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor, LPDWORD lpcbSecurityDescriptor)
{
	RegPath path;
	ResolveKey(hKey, path);
	Console::GetInstance().Print(L"GetKeySecurity: %s\n", path.ToString().c_str());

	return Real_RegGetKeySecurity(hKey, SecurityInformation, pSecurityDescriptor, lpcbSecurityDescriptor);
}

LSTATUS APIENTRY RegTree::RegLoadKeyW(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpFile)
{
	return Real_RegLoadKeyW(hKey, lpSubKey, lpFile);
}

LSTATUS APIENTRY RegTree::RegNotifyChangeKeyValue(HKEY hKey, BOOL bWatchSubtree, DWORD dwNotifyFilter, HANDLE hEvent, BOOL fAsynchronous)
{
	RegPath path;
	ResolveKey(hKey, path);
	Console::GetInstance().Print(L"NotifyChangeKeyValue: %s\n", path.ToString().c_str());
	
	return Real_RegNotifyChangeKeyValue(hKey, bWatchSubtree, dwNotifyFilter, hEvent, fAsynchronous);
}

LSTATUS APIENTRY RegTree::RegOpenKeyExW(HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{	
	RegPath path;
	if (ResolveKey(hKey, lpSubKey, path))
	{
		RegNode *node = OpenKey(path);
		if (node != NULL)
		{
			*phkResult = node->AsKey();
			return ERROR_SUCCESS;
		}
		else
			return ERROR_NOT_FOUND;
	}
	else
		return Real_RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

LSTATUS APIENTRY RegTree::RegOverridePredefKey(HKEY hKey, HKEY hNewHKey)
{
	RegPath path;
	ResolveKey(hKey, path);
	Console::GetInstance().Print(L"OverridePredefKey: %s\n", path.ToString().c_str());
	
	return Real_RegOverridePredefKey(hKey, hNewHKey);
}

LSTATUS APIENTRY RegTree::RegQueryInfoKeyW(HKEY hKey, LPWSTR lpClass, LPDWORD lpcchClass, LPDWORD lpReserved, LPDWORD lpcSubKeys, LPDWORD lpcbMaxSubKeyLen, LPDWORD lpcbMaxClassLen, LPDWORD lpcValues, LPDWORD lpcbMaxValueNameLen, LPDWORD lpcbMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime)
{
	RegPath path;
	if (IsVirtualKey(hKey))
	{
		RegNode *node = (RegNode*)((int)hKey & ~0x40000000);
		LoadRealChildren(node);
		
		if (lpClass != NULL)
			lpClass[0] = 0;
		if (lpcchClass != NULL)
			*lpcchClass = 0;
		if (lpcSubKeys != NULL)
			*lpcSubKeys = node->m_children.size();
		if (lpcbMaxSubKeyLen != NULL)
			*lpcbMaxSubKeyLen = 255;
		if (lpcbMaxClassLen)
			*lpcbMaxClassLen = MAX_PATH;
		if (lpcValues != NULL)
			*lpcValues = 0;
		if (lpcbMaxValueNameLen != NULL)
			*lpcbMaxValueNameLen = 16383;
		if (lpcbMaxValueLen != NULL)
			*lpcbMaxValueLen = 0xFFFFFFFF;

		return ERROR_SUCCESS;
	}
	else
		return Real_RegQueryInfoKeyW(hKey, lpClass, lpcchClass, lpReserved, lpcSubKeys, lpcbMaxSubKeyLen, lpcbMaxClassLen, lpcValues, lpcbMaxValueNameLen, lpcbMaxValueLen, lpcbSecurityDescriptor, lpftLastWriteTime);
}

LSTATUS APIENTRY RegTree::RegQueryMultipleValuesW(HKEY hKey, PVALENTW val_list, DWORD num_vals, LPWSTR lpValueBuf, LPDWORD ldwTotsize)
{
	RegPath path;
	ResolveKey(hKey, path);
	Console::GetInstance().Print(L"QueryMultipleValues: %s\n", path.ToString().c_str());
	
	return Real_RegQueryMultipleValuesW(hKey, val_list, num_vals, lpValueBuf, ldwTotsize);
}

LSTATUS APIENTRY RegTree::RegQueryValueExW(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	RegPath path;
	ResolveKey(hKey, path);
	Console::GetInstance().Print(L"QueryValueEx: %s, %s\n", path.ToString().c_str(), lpValueName);
	
	return Real_RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}

LSTATUS APIENTRY RegTree::RegReplaceKeyW(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpNewFile, LPCWSTR lpOldFile)
{
	RegPath path;
	ResolveKey(hKey, lpSubKey, path);
	Console::GetInstance().Print(L"ReplaceKey: %s\n", path.ToString().c_str());
	
	return Real_RegReplaceKeyW(hKey, lpSubKey, lpNewFile, lpOldFile);
}

LSTATUS APIENTRY RegTree::RegRestoreKeyW(HKEY hKey, LPCWSTR lpFile, DWORD dwFlags)
{
	RegPath path;
	ResolveKey(hKey,  path);
	Console::GetInstance().Print(L"RestoreKey: %s\n", path.ToString().c_str());
	
	return Real_RegRestoreKeyW(hKey, lpFile, dwFlags);
}

LSTATUS APIENTRY RegTree::RegSaveKeyW(HKEY hKey, LPCWSTR lpFile, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	RegPath path;
	ResolveKey(hKey, path);
	Console::GetInstance().Print(L"SaveKey: %s\n", path.ToString().c_str());
	
	return Real_RegSaveKeyW(hKey, lpFile, lpSecurityAttributes);
}

LSTATUS APIENTRY RegTree::RegSaveKeyExW(HKEY hKey, LPCWSTR lpFile, CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD Flags)
{
	RegPath path;
	ResolveKey(hKey, path);
	Console::GetInstance().Print(L"SaveKeyEx: %s\n", path.ToString().c_str());
	
	return Real_RegSaveKeyExW(hKey, lpFile, lpSecurityAttributes, Flags);
}

LSTATUS APIENTRY RegTree::RegSetKeySecurity(HKEY hKey, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	RegPath path;
	ResolveKey(hKey, path);
	Console::GetInstance().Print(L"SetKeySecurity: %s\n", path.ToString().c_str());
	
	return Real_RegSetKeySecurity(hKey, SecurityInformation, pSecurityDescriptor);
}

LSTATUS APIENTRY RegTree::RegSetValueExW(HKEY hKey, LPCWSTR lpValueName, DWORD Reserved, DWORD dwType, CONST BYTE* lpData, DWORD cbData)
{
	RegPath path;
	ResolveKey(hKey,  path);
	Console::GetInstance().Print(L"SetValueEx: %s, %s\n", path.ToString().c_str(), lpValueName);
	
	return Real_RegSetValueExW(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}

LSTATUS APIENTRY RegTree::RegUnLoadKeyW(HKEY hKey, LPCWSTR lpSubKey)
{
	RegPath path;
	ResolveKey(hKey, lpSubKey, path);
	Console::GetInstance().Print(L"UnLoadKey: %s\n", path.ToString().c_str());
	
	return Real_RegUnLoadKeyW(hKey, lpSubKey);
}

RegTree::RegTree( void )
{
	InitPrefixs();
	m_hook = new CodeHook;

	m_rootNodes.insert(HKEY_CLASSES_ROOT, RegNode::CreateRoot(HKEY_CLASSES_ROOT));
	m_rootNodes.insert(HKEY_CURRENT_USER, RegNode::CreateRoot(HKEY_CURRENT_USER));
	m_rootNodes.insert(HKEY_LOCAL_MACHINE, RegNode::CreateRoot(HKEY_LOCAL_MACHINE));
	m_rootNodes.insert(HKEY_USERS, RegNode::CreateRoot(HKEY_USERS));
	m_rootNodes.insert(HKEY_CURRENT_CONFIG, RegNode::CreateRoot(HKEY_CURRENT_CONFIG));

	HKEY key;
	RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"Software\\iLuE\\VideoGear", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, NULL);
	RegCloseKey(key);
}

void RegTree::ClearRealChildren( RegNode *node )
{
	typedef RegNodeList::index<RegNodeIndex::Type>::type Index;
	Index &index = node->m_children.get<RegNodeIndex::Type>();
	pair<Index::iterator, Index::iterator> range = index.equal_range(RegNodeType_Real);
	for (Index::iterator iter = range.first; iter != range.second; )
	{
		RegNode *child = *iter;
		index.erase(iter++);
		delete child;
	}
}

RegNode* RegTree::CreateKey( const RegPath &path, LPDWORD disposition )
{
	return OpenKey(path, true, disposition);	
}

RegNode* RegTree::GetRoot( HKEY rootKey )
{
	RootNodeList::iterator iter = m_rootNodes.find(rootKey);
	if (iter != m_rootNodes.end())
		return iter->second;
	else
		throw "Invalid root";
}

void RegTree::LoadRealChildren( RegNode *node )
{
	if (node->m_realKey != NULL)
	{
		ClearRealChildren(node);

		for (DWORD index = 0; ; index++)
		{
			WCHAR keyName[MAX_KEY_LENGTH + 1];
			DWORD keyNameLen = _countof(keyName);
			WCHAR className[MAX_PATH + 1];
			DWORD classNameLen = _countof(className);
			if (Real_RegEnumKeyExW(node->m_realKey, index, keyName, &keyNameLen, NULL, className, &classNameLen, NULL) != ERROR_SUCCESS)
				break;

			HKEY newKey;
			if (Real_RegOpenKeyExW(node->m_realKey, keyName, 0, KEY_ALL_ACCESS, &newKey) != ERROR_SUCCESS)
				continue;

			typedef RegNodeList::index<RegNodeIndex::Name>::type Index;
			Index &childIndex = node->m_children.get<RegNodeIndex::Name>();
			Index::iterator iter = childIndex.find(to_lower_copy(wstring(keyName)));
			if (iter != childIndex.end())
				continue;

			auto_ptr<RegNode> newNode = RegNode::CreateReal(node->m_rootKey, keyName, newKey);
			newNode->m_parent = node;
			node->m_children.push_back(newNode.release());
		}
	}
}

void RegTree::InitPrefixs( void )
{
	HKEY key;
	wstring path;

	RegOpenCurrentUser(KEY_READ, &key);
	ResolveKey(key, path);
	m_currentUserPrefix = path;

	Real_RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Classes", 0, KEY_READ, &key);
	ResolveKey(key, path);
	m_classesRootPrefix = path;

	Real_RegOpenKeyExW(HKEY_CURRENT_CONFIG, L"Software", 0, KEY_READ, &key);
	ResolveKey(key, path);
	m_currentConfigSoftwarePrefix = path;

	Real_RegOpenKeyExW(HKEY_CURRENT_CONFIG, L"System", 0, KEY_READ, &key);
	ResolveKey(key, path);
	m_currentConfigSystemPrefix = path;
}

bool RegTree::IsVirtualKey( HKEY key )
{
	return ((int)key & 0x40000000) != 0 && ((int)key & 0x80000000) == 0;
}

RegNode* RegTree::OpenKey( const RegPath &path, bool openAlways, LPDWORD disposition )
{
	RegNode *root = GetRoot(path.m_rootKey);
	if (path.m_segments.empty())
		return root;
 
	RegNode *parent = root;
	wstring subKey;
	BOOST_FOREACH(const wstring &segment, path.m_segments)
	{
		if (!subKey.empty())
			subKey += L"\\";
		subKey += segment;

		typedef RegNodeList::index<RegNodeIndex::Name>::type Index;
		Index &index = parent->m_children.get<RegNodeIndex::Name>();
		Index::iterator iter = index.find(to_lower_copy(segment));
		if (iter != index.end())
		{
			parent = *iter;
			if (openAlways && parent->m_type == RegNodeType_Real)
			{
				ClearRealChildren(parent);
				parent->m_type = RegNodeType_Normal;
			}
		}
		else
		{
			HKEY newKey;
			if (Real_RegOpenKeyExW(path.m_rootKey, subKey.c_str(), 0, KEY_ALL_ACCESS, &newKey) == ERROR_SUCCESS)
			{
				RegNode *newNode = RegNode::CreateReal(path.m_rootKey, segment, newKey).release();
				newNode->m_parent = parent;
				parent->m_children.push_back(newNode);
				parent = newNode;
				if (openAlways && disposition != NULL)
					*disposition = REG_OPENED_EXISTING_KEY;
			}
			else if (openAlways)
			{
				RegNode *newNode = RegNode::CreateNormal(path.m_rootKey, segment, NULL).release();
				newNode->m_parent = parent;
				parent->m_children.push_back(newNode);
				parent = newNode;
				if (disposition != NULL)
					*disposition = REG_CREATED_NEW_KEY;
			}
			else
			{
				parent = NULL;
				break;
			}
		}
	}

	return parent;
}

bool RegTree::ResolveKey( HKEY key, wstring &path )
{
	BYTE result[1024];
	DWORD resultSize;
	if (WinDDK::GetInstance().ZwQueryKey(key, KeyNameInformation, result, sizeof(result), &resultSize) == STATUS_SUCCESS)
	{
		KEY_BASIC_INFORMATION &info = *(KEY_BASIC_INFORMATION*)result;
		path = wstring(info.Name, (resultSize - sizeof(KEY_BASIC_INFORMATION)) / sizeof(WCHAR) + 1);
		if (!starts_with(path, L"TRY\\"))
			return false;
		path = path.substr(4);
		return true;
	}
	else
		return false;
}

bool RegTree::ResolveKey( HKEY key, RegPath &path )
{
	if ((int)key & 0x80000000)
	{
		path.m_rootKey = key;
		path.m_segments.clear();
	}
	else if ((int)key & 0x40000000)
	{
		RegNode *node = (RegNode*)((int)key & ~0x40000000);
		path.m_rootKey = node->m_rootKey;
		path.m_segments.clear();
		node->GetSubKey(path.m_segments);
	}
	else 
	{
		wstring keyPath;
		if (!ResolveKey(key, keyPath))
			return false;
		
		// Ω‚ŒˆRoot
		size_t sepPos = keyPath.find(L'\\');
		wstring rootName = keyPath.substr(0, sepPos);
		wstring subKey;
		if (rootName == L"USER")
		{
			if (istarts_with(keyPath, m_classesRootPrefix))
			{
				path.m_rootKey = HKEY_CLASSES_ROOT;
				subKey = keyPath.substr(m_classesRootPrefix.length());
				if (starts_with(subKey, L"\\"))
					subKey = subKey.substr(1);
			}
			else if (istarts_with(keyPath, m_currentUserPrefix))
			{
				path.m_rootKey = HKEY_CURRENT_USER;
				subKey = keyPath.substr(m_currentUserPrefix.length());
				if (starts_with(subKey, L"\\"))
					subKey = subKey.substr(1);
			}
			else
			{
				path.m_rootKey = HKEY_USERS;
				subKey = keyPath.substr(sepPos + 1);
			}
		}
		else if (rootName == L"MACHINE")
		{
			path.m_rootKey = HKEY_LOCAL_MACHINE;
			if (istarts_with(keyPath, m_currentConfigSoftwarePrefix))
			{
				subKey = keyPath.substr(m_currentConfigSoftwarePrefix.length());
				if (starts_with(subKey, L"\\"))
					subKey = subKey.substr(1);
			}
			else if (istarts_with(keyPath, m_currentConfigSystemPrefix))
			{
				subKey = keyPath.substr(m_currentConfigSystemPrefix.length());
				if (starts_with(subKey, L"\\"))
					subKey = subKey.substr(1);
			}
			else
				subKey = keyPath.substr(sepPos + 1);
		}
		else
			return false;
		// ∑÷∏ÓSubKey
		path.m_segments.clear();
		String::Split(subKey, L"\\", path.m_segments);
	}

	return true;
}

bool RegTree::ResolveKey( HKEY key, const wstring &subKey, RegPath &path )
{
	RegPath result;
	if (!ResolveKey(key, result))
		return false;
	path = result;
	String::Split(subKey, L"\\", path.m_segments);
	return true;
}

VG_NAMESPACE_END

