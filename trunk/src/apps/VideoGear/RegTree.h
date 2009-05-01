#pragma once

class CRegTreeNode
{
	friend class CRegTree;
public:
	CRegTreeNode(const CStringW& strName, CRegTreeNode* pParent);
	virtual ~CRegTreeNode(void);
	const CStringW& GetName(void)	{ return m_strName; }
	CRegTreeNode* GetParent(void)	{ return m_pParent; }
	CRegTreeNode* GetChild(CStringW strName, bool bExistsOnly = false);
	void RemoveChild(CStringW strName);
	CRegTreeNode* OpenNode(CStringW strPath, bool bExistsOnly = false);
	void CloseNode(CStringW strPath);
	bool GetValue(CStringW strKey, DWORD* lpType, BYTE* lpData, DWORD* lpcbData);
	void SetValue(CStringW strKey, DWORD dwType, const BYTE* lpData, DWORD cbData);
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

	CStringW	m_strName;
	CRegTreeNode*	m_pParent;
	values_t	m_values;
	children_t	m_children;
};

class CRegTree : public CRegTreeNode
{
public:
	CRegTree(void);
	void DumpToFile(LPCWSTR lpszFileName);
protected:
	void Dump(FILE* fp, CRegTreeNode* pNode, uint32_t nLevel);
};