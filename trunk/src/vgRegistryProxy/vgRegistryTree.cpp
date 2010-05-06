#include "stdafx.h"
#include "vgRegistryTree.h"
#include "vgConsole.h"

#define DECLARE_ROOT(key) m_rootNodes.insert(key, auto_ptr<vgRegistryNode>(InternalCreateRoot(key, L#key)))

vgRegistryTree::vgRegistryTree(void)
{
	DECLARE_ROOT(HKEY_CLASSES_ROOT);
	DECLARE_ROOT(HKEY_CURRENT_USER);
	DECLARE_ROOT(HKEY_LOCAL_MACHINE);
	DECLARE_ROOT(HKEY_USERS);
	DECLARE_ROOT(HKEY_CURRENT_CONFIG);
}

vgRegistryNode* vgRegistryTree::CreateNode(HKEY rootKey, const wstring &subKey)
{
	return InternalGetNode(rootKey, subKey, false);
}

vgRegistryNode* vgRegistryTree::OpenNode(HKEY rootKey, const wstring &subKey)
{
	return InternalGetNode(rootKey, subKey, true);
}

void vgRegistryTree::Print( void )
{
	for (RootNodes::const_iterator iter = m_rootNodes.begin(); iter != m_rootNodes.end(); iter++)
		InternalPrint(*iter->second, 0, false);
}

vgRegistryNode* vgRegistryTree::InternalCreateNode(vgRegistryNode *parent, const wstring &name)
{
	vgRegistryNode *node = new vgRegistryNode;
	node->m_rootKey = parent->m_rootKey;
	node->m_parent = parent;
	node->m_name = name;
	return node;
}

vgRegistryNode* vgRegistryTree::InternalCreateRoot(HKEY rootKey, const wstring &name)
{
	vgRegistryNode *node = new vgRegistryNode;
	node->m_rootKey = rootKey;
	node->m_name = name;
	return node;
}

vgRegistryNode* vgRegistryTree::InternalGetChild(vgRegistryNode *node, const wstring &name, bool onlyExists)
{
	typedef NodeList::index<vgRegistryNode::IndexByName>::type Index;
	Index &index = node->m_children.get<vgRegistryNode::IndexByName>();
	Index::const_iterator iter = index.find(boost::algorithm::to_lower_copy(name));
	if (iter != index.end())
		return *iter;
	else if (!onlyExists)
	{
		vgRegistryNode *newNode = InternalCreateNode(node, name);
		node->m_children.push_back(newNode);
		return newNode;
	}
	else
		return NULL;
}

vgRegistryNode* vgRegistryTree::InternalGetNode(HKEY rootKey, const wstring &subKey, bool onlyExists)
{
	int sepPos = subKey.rfind(L'\\');
	if (sepPos == wstring::npos)
	{
		vgRegistryNode *parent = &m_rootNodes[rootKey];
		return InternalGetChild(parent, subKey, onlyExists);
	}
	else
	{
		vgRegistryNode *parent = InternalGetNode(rootKey, subKey.substr(0, sepPos), onlyExists);
		if (parent != NULL)
			return InternalGetChild(parent, subKey.substr(sepPos + 1), onlyExists);
		else
			return NULL;
	}
}

wstring vgRegistryTree::InternalGetPath(vgRegistryNode *node)
{
	if (node->m_parent != NULL)
	{
		wstring parentPath = InternalGetPath(node->m_parent);
		if (!parentPath.empty())
			return parentPath + L"\\" + node->m_name;
	}
	return node->m_name;
}

void vgRegistryTree::InternalPrint( const vgRegistryNode &node, int level, bool lastNode )
{
	static vgConsole console;
	
	if (level >= 1)
	{
		for (int i = 1; i <= level - 1; i++)
			console.Print(L"©¦  ");
		if (!lastNode)
			console.Print(L"©À©¤");
		else
			console.Print(L"©¸©¤");
	}	
	console.Print(L"%s\n", node.m_name.c_str());

	for (size_t i = 0, childCount = node.m_children.size(); i < childCount; i++)
		InternalPrint(*node.m_children[i], level + 1, i == childCount - 1);
}
