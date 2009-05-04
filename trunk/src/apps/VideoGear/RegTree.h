#pragma once

class CRegTreeNode
{
	friend class CRegTree;
public:
	virtual ~CRegTreeNode(void);
	const CStringW& GetName(void)	{ return m_strName; }
	CRegTreeNode* GetParent(void)	{ return m_pParent; }
	
	CRegTreeNode* GetChild(CStringW strName, bool bExistsOnly = false);
	void RemoveChild(CStringW strName);
	UINT GetChildCount(void)	{ return m_children.size(); }
	CRegTreeNode* OpenNode(CStringW strPath, bool bExistsOnly = false);
	void CloseNode(CStringW strPath);
	CStringW GetPath(void);

	bool GetValue(CStringW strKey, DWORD* lpType, BYTE* lpData, DWORD* lpcbData);
	void SetValue(CStringW strKey, DWORD dwType, const BYTE* lpData, DWORD cbData);
	UINT GetValueCount(void)	{ return m_values.size(); }
protected:
	CRegTreeNode(bool bIsRoot, const CStringW& strName, CRegTreeNode* pParent);
	void Save(FILE* fp);
private:
	struct value_t
	{
		CStringW key;
		DWORD type;
		CAutoVectorPtr<BYTE> data;
		DWORD size;
	};
	typedef boost::ptr_map<CStringW, value_t> values_t;
	typedef boost::ptr_map<CStringW, CRegTreeNode> children_t;

	bool		m_bIsRoot;
	CStringW	m_strName;
	CRegTreeNode*	m_pParent;
	values_t	m_values;
	children_t	m_children;
};

class CRegTree : public CRegTreeNode
{
public:
	CRegTree(void);
	void LoadFromFile(LPCWSTR lpszFileName);
	void SaveToFile(LPCWSTR lpszFileName);
};
