#pragma once

#include "vgInclude.hpp"
#include "vgRegValue.hpp"

VG_NAMESPACE_BEGIN

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
	RegValueList m_values;
	HKEY m_realKey;
	
	static auto_ptr<RegNode> CreateRoot(HKEY rootKey);
	static auto_ptr<RegNode> CreateNormal(HKEY rootKey, const wstring &name, HKEY realKey);
	static auto_ptr<RegNode> CreateReal(HKEY rootKey, const wstring &name, HKEY realKey);

	~RegNode(void);

	HKEY AsKey(void);
	void GetSubKey(vector<wstring> &segments);
private:
	RegNode(RegNodeType type, HKEY rootKey);
};

VG_NAMESPACE_END