#include "stdafx.h"
#include "vgRegistryNode.h"
#include "vgRegistryImpl.h"

auto_ptr<vgRegistryNode> vgRegistryNode::CreateRoot( HKEY rootKey, const wstring &name )
{
	auto_ptr<vgRegistryNode> newNode(new vgRegistryNode(Type_Virtual));
	newNode->m_rootKey = rootKey;
	newNode->m_name = name;
	newNode->m_realKey = rootKey;
	return newNode;
}

auto_ptr<vgRegistryNode> vgRegistryNode::CreateReal( vgRegistryNode *parent, const wstring &name, HKEY realKey )
{
	auto_ptr<vgRegistryNode> newNode(new vgRegistryNode(Type_Real));
	newNode->m_rootKey = parent->m_rootKey;
	newNode->m_parent = parent;
	newNode->m_name = name;
	newNode->m_realKey = realKey;
	return newNode;
}

auto_ptr<vgRegistryNode> vgRegistryNode::CreateVirtual( vgRegistryNode *parent, const wstring &name, HKEY realKey )
{
	auto_ptr<vgRegistryNode> newNode(new vgRegistryNode(Type_Virtual));
	newNode->m_rootKey = parent->m_rootKey;
	newNode->m_parent = parent;
	newNode->m_name = name;
	newNode->m_realKey = realKey;
	return newNode;
}

auto_ptr<vgRegistryNode> vgRegistryNode::CreateOverride( vgRegistryNode *parent, const wstring &name )
{
	auto_ptr<vgRegistryNode> newNode(new vgRegistryNode(Type_Override));
	newNode->m_rootKey = parent->m_rootKey;
	newNode->m_name = name;
	return newNode;
}

vgRegistryNode::~vgRegistryNode( void )
{
	if (m_realKey != NULL)
	{
		vgRegistryImpl::Real_RegCloseKey(m_realKey);
		m_realKey = NULL;
	}
	if (m_parent != NULL)
	{
		m_parent->m_children.remove(this);
		m_parent = NULL;
	}
	while (!m_children.empty())
	{
		vgRegistryNode *node = *m_children.begin();
		m_children.erase(m_children.begin());
		delete node;
	}
}

vgRegistryNode::vgRegistryNode( Type type ) 
: vgRegistryNodeBase(type)
{
	m_realKey = NULL;
}


