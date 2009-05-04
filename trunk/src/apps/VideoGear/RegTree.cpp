#include "stdafx.h"
#include "RegTree.h"

CRegTreeNode::CRegTreeNode( bool bIsRoot, const CStringW& strName, CRegTreeNode* pParent )
: m_bIsRoot(bIsRoot), m_strName(strName), m_pParent(pParent)
{

}

CRegTreeNode::~CRegTreeNode( void )
{
	m_values.clear();
	m_children.clear();
}

CRegTreeNode* CRegTreeNode::GetChild( CStringW strName, bool bExistsOnly )
{
	CAutoPtr<CRegTreeNode> child(new CRegTreeNode(false, strName, this));

	strName.MakeLower();
	children_t::iterator it = m_children.find(strName);
	if (it != m_children.end())
		return it->second;
	else if (!bExistsOnly)
	{
		CRegTreeNode* pChild = child.Detach();
		m_children.insert(strName, pChild);
		return pChild;
	}
	else
		return NULL;
}

void CRegTreeNode::RemoveChild( CStringW strName )
{
	strName.MakeLower();
	children_t::iterator it = m_children.find(strName);
	if (it != m_children.end())
		m_children.erase(it);
}

CRegTreeNode* CRegTreeNode::OpenNode( CStringW strPath, bool bExistsOnly )
{
	int nPos;
	CStringW strChild, strSubPath;
	CRegTreeNode* pChild;
		
	strPath = strPath.Trim();
	strPath = strPath.TrimRight(L'\\');
	if (!strPath.IsEmpty())
	{
		nPos = strPath.Find(L'\\');
		if (nPos >= 0)
		{
			strChild = strPath.Left(nPos);
			strSubPath = strPath.Right(strPath.GetLength() - nPos - 1);
			pChild = GetChild(strChild, bExistsOnly);
			if (pChild != NULL)
				return pChild->OpenNode(strSubPath, bExistsOnly);
			else
				return NULL;			
		}
		else
			return GetChild(strPath, bExistsOnly);
	}
	else
		return this;
}

void CRegTreeNode::CloseNode( CStringW strPath )
{
	CStringW strSubPath, strChild;
	int nPos = strPath.ReverseFind(L'\\');
	if (nPos >= 0)
	{
		strSubPath = strPath.Left(nPos);
		strChild = strPath.Right(strPath.GetLength() - nPos - 1);
		CRegTreeNode* pNode = OpenNode(strSubPath, true);
		if (pNode != NULL)
			pNode->RemoveChild(strChild);
	}
	else
		RemoveChild(strPath);
}

CStringW CRegTreeNode::GetPath( void )
{
	if (m_pParent != NULL && !m_pParent->m_bIsRoot)
		return m_pParent->GetPath() + L"\\" + m_strName;
	else
		return m_strName;
}

bool CRegTreeNode::GetValue( CStringW strKey, DWORD* lpType, BYTE* lpData, DWORD* lpcbData )
{
	values_t::iterator it = m_values.find(strKey.MakeLower());
	if (it != m_values.end())
	{	
		if (lpType != NULL)
			*lpType = it->second->type;
		if (lpData != NULL)
			memcpy(lpData, it->second->data.m_p, it->second->size);
		if (lpcbData != NULL)
			*lpcbData = it->second->size;
		return true;
	}
	else
		return false;
}

void CRegTreeNode::SetValue( CStringW strKey, DWORD dwType, const BYTE* lpData, DWORD cbData )
{
	value_t* pValue = new value_t;
	pValue->key = strKey;
	pValue->type = dwType;
	pValue->data.Allocate(cbData);
	pValue->size = cbData;
	memcpy(pValue->data.m_p, lpData, cbData);
	m_values.insert(strKey.MakeLower(), pValue);
}

void CRegTreeNode::Save( FILE* fp )
{
	fwprintf_s(fp, L"[%s]\r\n", (LPCWSTR)GetPath());
	for (values_t::iterator it = m_values.begin(); it != m_values.end(); it++)
	{
		if (it->second->key.IsEmpty())
			fwprintf_s(fp, L"@=");
		else
			fwprintf_s(fp, L"\"%s\"=", (LPCWSTR)it->second->key);

		switch (it->second->type)
		{
		case REG_SZ:
			fwprintf_s(fp, L"\"%s\"", (LPCWSTR)it->second->data.m_p);
			break;
		}

		fwprintf_s(fp, L"\r\n");
	}
	fwprintf_s(fp, L"\r\n");
	for (children_t::iterator it = m_children.begin(); it != m_children.end(); it++)
		it->second->Save(fp);
}
//////////////////////////////////////////////////////////////////////////

CRegTree::CRegTree( void )
: CRegTreeNode(true, L"Computer", NULL)
{
}

void CRegTree::LoadFromFile( LPCWSTR lpszFileName )
{
	FILE* fp;

	if (_wfopen_s(&fp, lpszFileName, L"r, ccs=UTF-8") == 0)
	{
		fclose(fp);
	}
}

void CRegTree::SaveToFile( LPCWSTR lpszFileName )
{
	FILE* fp;
	
	if (_wfopen_s(&fp, lpszFileName, L"w, ccs=UTF-8") == 0)
	{
		for (children_t::iterator it = m_children.begin(); it != m_children.end(); it++)
			it->second->Save(fp);
		fclose(fp);
	}
}