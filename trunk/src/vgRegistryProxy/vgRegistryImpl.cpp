#include "StdAfx.h"
#include "vgRegistryImpl.h"
#include "vgCommon/vgConsole.h"

#include "vgRegistryImpl_Impl.inl"

#define DECLARE_ROOT(key) \
	m_rootNodes.insert(boost::shared_ptr<vgRegistryNode>(vgRegistryNode::CreateRoot(key, L#key))); \
	m_keyToPathMap.insert(make_pair(key, vgRegistryPath(key, L"")))

vgRegistryImpl& vgRegistryImpl::GetInstance( void )
{
	static vgRegistryImpl instance;
	return instance;
}

vgRegistryImpl::~vgRegistryImpl(void)
{
}

void vgRegistryImpl::Print( void )
{
	for (RootNodeList::const_iterator iter = m_rootNodes.begin(); iter != m_rootNodes.end(); iter++)
		Print(**iter, 0);
}

LSTATUS APIENTRY vgRegistryImpl::RegCloseKey( HKEY hKey )
{
	return ERROR_NOT_FOUND;
}

LSTATUS APIENTRY vgRegistryImpl::RegCreateKeyExW( HKEY hKey, LPCWSTR lpSubKey, DWORD Reserved, LPWSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition )
{
	vgRegistryPath path = MapKey(hKey, lpSubKey);
	vgRegistryNode *node = CreateKey(path);
	return ERROR_NOT_FOUND;
}

LSTATUS APIENTRY vgRegistryImpl::RegOpenKeyExW( HKEY hKey, LPCWSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult )
{
	vgRegistryPath path = MapKey(hKey, lpSubKey);
	vgRegistryNode *node = OpenKey(path);
	return ERROR_NOT_FOUND;
}

vgRegistryImpl::vgRegistryImpl(void)
{
	DECLARE_ROOT(HKEY_CLASSES_ROOT);
	DECLARE_ROOT(HKEY_CURRENT_USER);
	DECLARE_ROOT(HKEY_LOCAL_MACHINE);
	DECLARE_ROOT(HKEY_USERS);
	DECLARE_ROOT(HKEY_CURRENT_CONFIG);
}

vgRegistryPath vgRegistryImpl::MapKey( HKEY key, const wstring &subKey )
{
	KeyToPathMap::const_iterator iter = m_keyToPathMap.find(key);
	assert(iter != m_keyToPathMap.end());
	vgRegistryPath path(iter->second.m_rootKey, vgRegistryPath::Combine(iter->second.m_subKey, subKey));
	return path;
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

vgRegistryNode* vgRegistryImpl::CreateKey( const vgRegistryPath &path )
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
				newParent->m_type = vgRegistryNode::Type_Virtual;
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

			if (realKey != NULL)
				m_keyToPathMap.insert(make_pair(realKey, vgRegistryPath(newParent->m_rootKey, childSubKey)));
		}

		parent = newParent;
		pos = (newPos != wstring::npos ? newPos + 1 : wstring::npos);
	}
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

				m_keyToPathMap.insert(make_pair(realKey, vgRegistryPath(newParent->m_rootKey, childSubKey)));
			}
		}

		parent = newParent;
		pos = (newPos != wstring::npos ? newPos + 1 : wstring::npos);
	}
	return parent;
}

void vgRegistryImpl::Print( const vgRegistryNode &node, int level )
{
	for (int i = 1; i <= level - 1; i++)
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
