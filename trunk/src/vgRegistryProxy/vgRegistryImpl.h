#pragma once

#include "vgInclude.h"
#include "vgRegistryNode.h"

struct vgRegistryPath
{
	HKEY m_rootKey;
	wstring m_subKey;

	static wstring Combine(const wstring &path, const wstring &more)
	{
		wchar_t result[MAX_PATH];
		PathCombineW(result, path.c_str(), more.c_str());
		return result;
	}

	vgRegistryPath(HKEY rootKey, const wstring &subKey)
	{
		m_rootKey = rootKey;
		m_subKey = subKey;
	}
};

class vgRegistryImpl : public boost::noncopyable
{

#include "vgRegistryImpl_Decl.inl"

public:
	static vgRegistryImpl& GetInstance(void);

	~vgRegistryImpl(void);

	void Print(void);
private:
	vgRegistryImpl(void);
private:
	typedef boost::multi_index::multi_index_container<
		boost::shared_ptr<vgRegistryNode>,
		boost::multi_index::indexed_by<
			boost::multi_index::ordered_unique<
				boost::multi_index::member<vgRegistryNodeBase, HKEY, &vgRegistryNodeBase::m_rootKey>
			>
		>
	> RootNodeList;

	typedef map<HKEY, vgRegistryPath> KeyToPathMap;
private:
	vgRegistryPath MapKey(HKEY key, const wstring &subKey);
	vgRegistryNode* MapRoot(HKEY rootKey);
	vgRegistryNode* OpenChild(vgRegistryNode *parent, const wstring &childName);

	vgRegistryNode* CreateKey(const vgRegistryPath &path);
	vgRegistryNode* OpenKey(const vgRegistryPath &path);
	
	void Print(const vgRegistryNode &node, int level);
private:
	RootNodeList m_rootNodes;
	KeyToPathMap m_keyToPathMap;
};
