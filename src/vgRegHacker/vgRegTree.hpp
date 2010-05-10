#pragma once

#include "vgInclude.hpp"
#include "vgRegValue.hpp"

VG_NAMESPACE_BEGIN

using namespace algorithm;
using namespace multi_index;

enum RegNodeType
{
	RegNodeType_Normal,
	RegNodeType_Real,
	RegNodeType_Override,
};

struct RegNode;

namespace RegNodeIndex
{
	struct Type {};
	struct Name {};
}

struct RegNodeTypeGetter
{
	typedef RegNodeType result_type;

	RegNodeType operator ()(RegNode *node) const;
};

struct RegNodeNameGetter
{
	typedef wstring result_type;
	
	wstring operator ()(RegNode *node) const;
};

typedef multi_index_container<
	RegNode*,
	indexed_by<
		random_access<>,
		ordered_non_unique<tag<RegNodeIndex::Type>, RegNodeTypeGetter>,
		ordered_unique<tag<RegNodeIndex::Name>, RegNodeNameGetter>
	>
> RegNodeList;

struct RegNode
{
	RegNodeType m_type;
	HKEY m_rootKey;
	RegNode *m_parent;
	wstring m_name;
	RegNodeList m_children;
	HKEY m_realKey;
	
	static auto_ptr<RegNode> CreateRoot(HKEY rootKey)
	{
		auto_ptr<RegNode> node(new RegNode(RegNodeType_Real, rootKey));
		return node;
	}

	static auto_ptr<RegNode> CreateNormal(HKEY rootKey, const wstring &name, HKEY realKey)
	{
		auto_ptr<RegNode> node(new RegNode(RegNodeType_Normal, rootKey));
		node->m_name = name;
		node->m_realKey = realKey;
		return node;
	}

	static auto_ptr<RegNode> CreateReal(HKEY rootKey, const wstring &name, HKEY realKey)
	{
		auto_ptr<RegNode> node(new RegNode(RegNodeType_Real, rootKey));
		node->m_name = name;
		node->m_realKey = realKey;
		return node;
	}

	~RegNode(void)
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

	HKEY AsKey(void)
	{
		if (m_parent != NULL)
			return (HKEY)((int)this | 0x40000000);
		else
			return m_rootKey;
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
private:
	RegNode(RegNodeType type, HKEY rootKey)
	{
		m_type = type;
		m_rootKey = rootKey;
		m_parent = NULL;
		m_realKey = NULL;
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

	void ClearRealChildren(RegNode *node);
	RegNode* CreateKey(const RegPath &path, LPDWORD disposition);
	RegNode* GetRoot(HKEY rootKey);
	void LoadRealChildren(RegNode *node);
	void InitUserSid(void);
	bool IsVirtualKey(HKEY key);
	RegNode* OpenKey(const RegPath &path, bool openAlways = false, LPDWORD disposition = NULL);
	bool ResolveKey(HKEY key, RegPath &path);
	bool ResolveKey(HKEY key, const wstring &subKey, RegPath &path);
private:
	typedef ptr_map<HKEY, RegNode> RootNodeList;
private:
	wstring m_userSid;
	CodeHook *m_hook;
	RootNodeList m_rootNodes;
};

VG_NAMESPACE_END
