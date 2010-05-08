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

	wstring ToString(void) const
	{
		wstring result;

		switch ((int)m_rootKey)
		{
		case HKEY_LOCAL_MACHINE:
			result += L"HKEY_LOCAL_MACHINE";
			break;
		case HKEY_USERS:
			result += L"HKEY_USERS";
			break;
		}
		
		for (size_t i = 0; i < m_segments.size(); i++)
			result += L"\\" + m_segments[i];

		return result;
	}
};

class CodeHook;

class RegTree : public noncopyable
{

#include "vgRegTree_Decl.inl"

public:
	static RegTree& GetInstance(void);

	~RegTree(void);

	void Hook(void);
	void Unhook(void);
private:
	RegTree(void);

	bool ResolveKEY(HKEY key, RegPath &path);
	bool ResolveKEY(HKEY key, const wstring &subKey, RegPath &path);
private:
	CodeHook *m_hook;
};

VG_NAMESPACE_END
