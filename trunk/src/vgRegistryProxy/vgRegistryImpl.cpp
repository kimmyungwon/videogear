#include "StdAfx.h"
#include "vgRegistryImpl.h"
#include "vgCommon/vgConsole.h"
#include "vgCommon/vgWinDDK.h"

#include "vgRegistryImpl_Impl.inl"

#define DECLARE_ROOT(key) \
	m_rootNodes.insert(boost::shared_ptr<vgRegistryNode>(vgRegistryNode::CreateRoot(key, L#key)))

vgRegistryImpl& vgRegistryImpl::GetInstance( void )
{
	static vgRegistryImpl instance;
	return instance;
}

vgRegistryImpl::~vgRegistryImpl(void)
{
}

void vgRegistryImpl::AddKey( const vgRegistryPath &path, bool asOverride )
{
	CreateKey(path, asOverride);
}

void vgRegistryImpl::Print( void )
{
	for (RootNodeList::const_iterator iter = m_rootNodes.begin(); iter != m_rootNodes.end(); iter++)
		Print(**iter, 0);
}

LSTATUS APIENTRY vgRegistryImpl::RegCloseKey( HKEY hKey )
{
/*	return ERROR_SUCCESS;*/
	return Real_RegCloseKey(hKey);
}

LSTATUS APIENTRY vgRegistryImpl::RegCreateKeyExW( HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition )
{
	vgRegistryPath path;
	if (MapKey(hKey, lpSubKey, path))
	{
		vgRegistryNode *node = CreateKey(path, false);
		*phkResult = node->AsKey();
		if (lpdwDisposition != NULL)
			return REG_OPENED_EXISTING_KEY;
		return ERROR_SUCCESS;
	}
	else
		return Real_RegCreateKeyExW(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}

LSTATUS APIENTRY vgRegistryImpl::RegOpenKeyExW( HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult )
{
	vgRegistryPath path;
	if (MapKey(hKey, lpSubKey, path))
	{
		vgRegistryNode *node = OpenKey(path);
		*phkResult = node->AsKey();
		return ERROR_SUCCESS;
	}
	else
		return Real_RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}

vgRegistryImpl::vgRegistryImpl(void)
{	
	DECLARE_ROOT(HKEY_CLASSES_ROOT);
	DECLARE_ROOT(HKEY_CURRENT_USER);
	DECLARE_ROOT(HKEY_LOCAL_MACHINE);
	DECLARE_ROOT(HKEY_USERS);
	DECLARE_ROOT(HKEY_CURRENT_CONFIG);
}

bool vgRegistryImpl::MapKey( HKEY key, const wstring &subKey, vgRegistryPath &path )
{
	if ((int)key >= 0x80000000)
	{
		path = vgRegistryPath(key, subKey);
		return true;	
	}
	else if ((int)key >= 0x40000000)
	{
		vgRegistryNode *node = (vgRegistryNode*)((int)key & ~0x40000000);
		path = vgRegistryPath(node->m_rootKey, subKey);
		return true;
	}
	else
	{
		BYTE buffer[1024];
		ULONG resultSize;
		if (ZwQueryKey(key, KeyNameInformation, buffer, sizeof(buffer), &resultSize) != STATUS_SUCCESS)
			return false;
		KEY_BASIC_INFORMATION *keyInfo = (KEY_BASIC_INFORMATION*)buffer;
		wstring keyPath(keyInfo->Name, (resultSize - sizeof(KEY_BASIC_INFORMATION)) / sizeof(wchar_t) + 1);

		// 去掉开头的"TRY\"
		keyPath = keyPath.substr(4);
		// 解析Root
		size_t sepPos = keyPath.find(L'\\');
	 	wstring rootName = keyPath.substr(0, sepPos);
		if (rootName == L"USER")
			path.m_rootKey = HKEY_USERS;
		else if (rootName == L"MACHINE")
			path.m_rootKey = HKEY_LOCAL_MACHINE;
		else
			assert(false);
		// 取得SubKey
		path.m_subKey = keyPath.substr(sepPos + 1);

		return true;
	}
}

vgRegistryNode* vgRegistryImpl::MapRoot( HKEY rootKey )
{
	RootNodeList::const_iterator iter = m_rootNodes.find(rootKey);
	assert(iter != m_rootNodes.end());
	return iter->get();
}

vgRegistryNode* vgRegistryImpl::OpenChild( vgRegistryNode *parent, const wstring &childName )
{
	typedef vgRegistryNodeList::index<vgRegistryNodeIndex::Name>::type Index;
	Index &index = parent->m_children.get<vgRegistryNodeIndex::Name>();
	Index::const_iterator iter = index.find(boost::algorithm::to_lower_copy(childName));
	if (iter != index.end())
		return *iter;
	else
		return NULL;
}

vgRegistryNode* vgRegistryImpl::CreateKey( const vgRegistryPath &path, bool createAsOverride )
{
	vgRegistryNode *parent = MapRoot(path.m_rootKey);
	size_t pos = 0;
	while (parent != NULL && pos != wstring::npos)
	{
		int newPos = path.m_subKey.find(L'\\', pos);

		wstring childName;
		if (newPos != wstring::npos)
			childName = path.m_subKey.substr(pos, newPos - pos);
		else
			childName = path.m_subKey.substr(pos);
		wstring childSubKey = path.m_subKey.substr(0, newPos);

		vgRegistryNode *newParent = OpenChild(parent, childName);

		if (newParent != NULL)
		{
			if (newParent->m_type == vgRegistryNode::Type_Real)
				newParent->SetType(vgRegistryNode::Type_Virtual);
		}
		else
		{	
			HKEY realKey = NULL;
			if (parent->m_type != vgRegistryNode::Type_Override)
			{
				if (Real_RegOpenKeyExW(path.m_rootKey, childSubKey.c_str(), 0, KEY_ALL_ACCESS, &realKey) != ERROR_SUCCESS)
					realKey = NULL;
				newParent = vgRegistryNode::CreateVirtual(parent, childName, realKey).release();
			}
			else
				newParent = vgRegistryNode::CreateOverride(parent, childName).release();
			parent->m_children.push_back(newParent);
		}

		parent = newParent;
		pos = (newPos != wstring::npos ? newPos + 1 : wstring::npos);
	}

	if (createAsOverride)
		parent->SetType(vgRegistryNode::Type_Override);
	return parent;
}

vgRegistryNode* vgRegistryImpl::OpenKey( const vgRegistryPath &path )
{
	vgRegistryNode *parent = MapRoot(path.m_rootKey);
	size_t pos = 0;
	while (parent != NULL && pos != wstring::npos)
	{
		int newPos = path.m_subKey.find(L'\\', pos);

		wstring childName;
		if (newPos != wstring::npos)
			childName = path.m_subKey.substr(pos, newPos - pos);
		else
			childName = path.m_subKey.substr(pos);
		wstring childSubKey = path.m_subKey.substr(0, newPos);

		vgRegistryNode *newParent = OpenChild(parent, childName);

		if (newParent == NULL && parent->m_type != vgRegistryNode::Type_Override)
		{	
			HKEY realKey;
			if (Real_RegOpenKeyExW(path.m_rootKey, childSubKey.c_str(), 0, KEY_ALL_ACCESS, &realKey) == ERROR_SUCCESS)
			{
				newParent = vgRegistryNode::CreateReal(parent, childName, realKey).release();
				parent->m_children.push_back(newParent);
			}
		}

		parent = newParent;
		pos = (newPos != wstring::npos ? newPos + 1 : wstring::npos);
	}
	return parent;
}

void vgRegistryImpl::Print( const vgRegistryNode &node, int level )
{
	for (int i = 1; i <= level; i++)
		vgConsole::GetInstance().Print(L"+---");

	wstring nodeType = (
		node.m_type == vgRegistryNode::Type_Real ? L"R" :
		node.m_type == vgRegistryNode::Type_Virtual ? L"V" :
		L"O"
	);
	vgConsole::GetInstance().Print(L"[%s]%s\n", nodeType.c_str(), node.m_name.c_str());

	for (size_t i = 0, childCount = node.m_children.size(); i < childCount; i++)
		Print(*node.m_children[i], level + 1);
}

