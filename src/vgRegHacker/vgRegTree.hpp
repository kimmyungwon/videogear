#pragma once

#include "vgInclude.hpp"

VG_NAMESPACE_BEGIN

using namespace algorithm;
using namespace multi_index;

struct RegValue
{
	wstring m_name;
	any m_value;

	wstring GetKey(void) const
	{
		return to_lower_copy(m_name);
	}
};

typedef multi_index_container<
	RegValue*,
	indexed_by<
		random_access<>,
		ordered_unique<const_mem_fun<RegValue, wstring, &RegValue::GetKey> >
	>
> RegValueList;

struct RegNode;

struct RegNodeKeyGetter
{
	typedef wstring result_type;
	
	wstring operator ()(const RegNode &node) const;
};

typedef multi_index_container<
	RegNode*,
	indexed_by<
		random_access<>,
		ordered_unique<RegNodeKeyGetter>
	>
> RegNodeList;

struct RegNode
{
	HKEY m_rootKey;
	RegNode *m_parent;
	wstring m_name;
	RegNodeList m_children;

	RegNode(void)
	{
		m_rootKey = NULL;
		m_parent = NULL;
	}

	void GetSubKey(vector<wstring> segments)
	{
		if (m_parent != NULL)
		{
			for (RegNode *node = m_parent; node != NULL && node->m_parent != NULL; node = node->m_parent)
				segments.insert(segments.begin(), node->m_name);
			segments.push_back(m_name);
		}	
	}
};

struct RegPath
{
	HKEY m_rootKey;
	vector<wstring> m_segments;
};

class RegTree : public noncopyable
{

#include "vgRegTree_Decl.inl"

public:
	static RegTree& GetInstance(void);
private:
	RegTree(void);

	bool ResolveKEY(HKEY key, RegPath &path);
};

VG_NAMESPACE_END
