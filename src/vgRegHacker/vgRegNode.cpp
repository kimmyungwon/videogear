#include "vgRegNode.hpp"

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

auto_ptr<RegNode> RegNode::CreateRoot( HKEY rootKey )
{
	auto_ptr<RegNode> node(new RegNode(RegNodeType_Real, rootKey));
	return node;
}

auto_ptr<RegNode> RegNode::CreateNormal( HKEY rootKey, const wstring &name, HKEY realKey )
{
	auto_ptr<RegNode> node(new RegNode(RegNodeType_Normal, rootKey));
	node->m_name = name;
	node->m_realKey = realKey;
	return node;
}

auto_ptr<RegNode> RegNode::CreateReal( HKEY rootKey, const wstring &name, HKEY realKey )
{
	auto_ptr<RegNode> node(new RegNode(RegNodeType_Real, rootKey));
	node->m_name = name;
	node->m_realKey = realKey;
	return node;
}

RegNode::~RegNode( void )
{
	if (m_parent != NULL)
	{
		m_parent->m_children.remove(this);
		m_parent = NULL;
	}
	while (!m_children.empty())
	{
		RegNode *node = *m_children.begin();
		m_children.erase(m_children.begin());
		delete node;
	}
}

HKEY RegNode::AsKey( void )
{
	if (m_parent == NULL)
		return m_rootKey;
	else if (m_type != RegNodeType_Real)
		return (HKEY)((int)this | 0x40000000);
	else
		return m_realKey;
}

void RegNode::GetSubKey( vector<wstring> &segments )
{
	if (m_parent != NULL)
	{
		for (RegNode *node = m_parent; node != NULL && node->m_parent != NULL; node = node->m_parent)
			segments.insert(segments.begin(), node->m_name);
		segments.push_back(m_name);
	}
}

RegNode::RegNode( RegNodeType type, HKEY rootKey )
{
	m_type = type;
	m_rootKey = rootKey;
	m_parent = NULL;
	m_realKey = NULL;
}

VG_NAMESPACE_END