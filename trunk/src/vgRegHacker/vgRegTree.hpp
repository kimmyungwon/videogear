#pragma once

#include "vgInclude.hpp"
#include "vgRegValue.hpp"
#include "vgRegNode.hpp"

VG_NAMESPACE_BEGIN

struct RegPath
{
	HKEY m_rootKey;
	vector<wstring> m_segments;

	wstring ToString(void) const
	{
		wstring result;

		switch ((int)m_rootKey)
		{
		case HKEY_CLASSES_ROOT:
			result += L"HKEY_CLASSES_ROOT";
			break;
		case HKEY_CURRENT_USER:
			result += L"HKEY_CURRENT_USER";
			break;
		case HKEY_LOCAL_MACHINE:
			result += L"HKEY_LOCAL_MACHINE";
			break;
		case HKEY_USERS:
			result += L"HKEY_USERS";
			break;
		case HKEY_CURRENT_CONFIG:
			result += L"HKEY_CURRENT_CONFIG";
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
	void InitPrefixs(void);
	bool IsVirtualKey(HKEY key);
	RegNode* OpenKey(const RegPath &path, bool openAlways = false, LPDWORD disposition = NULL);
	bool ResolveKey(HKEY key, wstring &path);
	bool ResolveKey(HKEY key, RegPath &path);
	bool ResolveKey(HKEY key, const wstring &subKey, RegPath &path);
private:
	typedef ptr_map<HKEY, RegNode> RootNodeList;
private:
	wstring m_userSid;
	wstring m_classesRootPrefix;
	wstring m_currentUserPrefix;
	wstring m_currentConfigSoftwarePrefix;
	wstring m_currentConfigSystemPrefix;
	CodeHook *m_hook;
	RootNodeList m_rootNodes;
};

VG_NAMESPACE_END
