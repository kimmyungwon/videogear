#pragma once

#include "vgInclude.h"

struct vgRegistryNode;

struct vgRegistryNodeBase : public boost::noncopyable
{
	enum Type
	{
		Type_Real,	  // ��ʵ�ڵ㣺�ӽڵ��������ʵ�ڵ�
		Type_Virtual, // ����ڵ㣺�ӽڵ��������ڵ����ʵ�ڵ�
		Type_Override,// ���ؽڵ㣺�ӽڵ����������ڵ�
	};
	
	Type m_type;
	HKEY m_rootKey;
	vgRegistryNode *m_parent;
	wstring m_name;

	virtual ~vgRegistryNodeBase(void) {}

	wstring GetNameForIndex(void) const
	{
		return boost::algorithm::to_lower_copy(m_name);
	}
protected:
	vgRegistryNodeBase(Type type)
	{
		m_type = type;
		m_rootKey = NULL;
		m_parent = NULL;
	}
};

namespace vgRegistryNodeIndex
{
	struct Name {};
	struct Type {};
}

typedef boost::multi_index::multi_index_container<
	vgRegistryNode*,
	boost::multi_index::indexed_by<
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique<
			boost::multi_index::tag<vgRegistryNodeIndex::Name>,
			boost::multi_index::const_mem_fun<vgRegistryNodeBase, wstring, &vgRegistryNodeBase::GetNameForIndex>
		>,
		boost::multi_index::ordered_non_unique<
			boost::multi_index::tag<vgRegistryNodeIndex::Type>,
			boost::multi_index::member<vgRegistryNodeBase, vgRegistryNodeBase::Type, &vgRegistryNodeBase::m_type>
		>
	>
> vgRegistryNodeList;

struct vgRegistryNode : vgRegistryNodeBase
{
	vgRegistryNodeList m_children;
	HKEY m_realKey;

	static auto_ptr<vgRegistryNode> CreateRoot(HKEY rootKey, const wstring &name);
	static auto_ptr<vgRegistryNode> CreateReal(vgRegistryNode *parent, const wstring &name, HKEY realKey);
	static auto_ptr<vgRegistryNode> CreateVirtual(vgRegistryNode *parent, const wstring &name, HKEY realKey);
	static auto_ptr<vgRegistryNode> CreateOverride(vgRegistryNode *parent, const wstring &name);

	virtual ~vgRegistryNode(void);

	HKEY AsKey(void);
	void SetType(Type newType);
private:
	vgRegistryNode(Type type);
};