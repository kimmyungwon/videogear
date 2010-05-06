#pragma once

#include "vgInclude.h"
#include "vgRegistryKey.h"

struct vgRegistryNode;

struct vgRegistryNodeBase
{
	struct IndexByName {};

	HKEY m_rootKey;
	vgRegistryNode *m_parent;
	wstring m_name;

	vgRegistryNodeBase(void)
	{
		m_rootKey = NULL;
		m_parent = NULL;
	}

	virtual ~vgRegistryNodeBase(void) {}

	wstring GetNameLC(void) const
	{
		return boost::algorithm::to_lower_copy(m_name);
	}
};

typedef boost::multi_index::multi_index_container<
	vgRegistryNode*,
	boost::multi_index::indexed_by<
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique<
			boost::multi_index::tag<vgRegistryNodeBase::IndexByName>, 
			boost::multi_index::const_mem_fun<vgRegistryNodeBase, wstring, &vgRegistryNodeBase::GetNameLC> 
		>
	>
> NodeList;

struct vgRegistryNode : vgRegistryNodeBase
{		
	NodeList m_children;
	// 以下保留给vgRegistry使用
	vgRegistryKey m_realKey;

	static vgRegistryNode* FromKey(HKEY key)
	{
		if ((int)key > 0x40000000 && (int)key < 0x80000000)
			return (vgRegistryNode*)((int)key & ~0x40000000);
		else
			return NULL;
	}

	~vgRegistryNode(void)
	{
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

	HKEY AsKey(void)
	{
		if (m_parent == NULL)
			return m_rootKey;
		else
			return (HKEY)((int)this | 0x40000000);
	}
};

class vgRegistryTree
{
public:
	vgRegistryTree(void);
	virtual ~vgRegistryTree(void) {}

	vgRegistryNode* CreateNode(HKEY rootKey, const wstring &subKey);
	vgRegistryNode* OpenNode(HKEY rootKey, const wstring &subKey);

	void Print(void);
protected:
	virtual vgRegistryNode* InternalCreateNode(vgRegistryNode *parent, const wstring &name);
	virtual vgRegistryNode* InternalCreateRoot(HKEY rootKey, const wstring &name);
	vgRegistryNode* InternalGetChild(vgRegistryNode *node, const wstring &name, bool onlyExists);
	vgRegistryNode* InternalGetNode(HKEY rootKey, const wstring &subKey, bool onlyExists);
	wstring InternalGetPath(vgRegistryNode *node);

	void InternalPrint(const vgRegistryNode &node, int level, bool lastNode);
private:
	typedef boost::ptr_map<HKEY, vgRegistryNode> RootNodes;
private:
	RootNodes m_rootNodes;
};