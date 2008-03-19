#include "StdAfx.h"
#include "VGFilterList.h"

CVGFilterList::CVGFilterList(void)
{
}

CVGFilterList::~CVGFilterList(void)
{
	Clear();
}

HRESULT STDMETHODCALLTYPE CVGFilterList::Add( IBaseFilter *pBF, LPCWSTR pName )
{
	CheckPointer(pBF, E_POINTER);

	pBF->AddRef();
	m_filters.push_back(pBF);
	m_name.push_back(pName);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CVGFilterList::Clear( void )
{
	while (m_filters.size() > 0)
	{
		Delete(0);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CVGFilterList::Delete( DWORD nIndex )
{
	if (nIndex >= m_filters.size())
		return E_INVALIDARG;

	m_filters[nIndex]->Release();
	m_filters.erase(m_filters.begin() + nIndex);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CVGFilterList::Get( DWORD nIndex, IBaseFilter** ppBF, LPCWSTR *ppName )
{
	if (m_filters.size() != m_name.size() || nIndex >= m_filters.size())
	{
		*ppBF = NULL;
		return E_INVALIDARG;
	}

	*ppBF = m_filters[nIndex];
	(*ppBF)->AddRef();
	*ppName = m_name[nIndex];
	return S_OK;
}

DWORD STDMETHODCALLTYPE CVGFilterList::GetCount( void )
{
	return m_filters.size();
}

