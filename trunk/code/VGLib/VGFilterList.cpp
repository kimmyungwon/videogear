#include "StdAfx.h"
#include "VGFilterList.h"

CVGFilterList::CVGFilterList(void)
	:m_cRef(1)
{
}

CVGFilterList::~CVGFilterList(void)
{
	if (m_cRef != 0)
		throw "Invaild pointer";
	InterlockedDecrement(&m_cRef);
}

HRESULT STDMETHODCALLTYPE CVGFilterList::QueryInterface( REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject )
{
	CheckPointer(ppvObject, E_POINTER);
	ValidateReadWritePtr(ppvObject, sizeof(PVOID));

	/* We know only about IUnknown */

	if (riid == IID_IUnknown) {
		GetInterface((LPUNKNOWN)this, ppvObject);
		return S_OK;
	} else {
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
}

ULONG STDMETHODCALLTYPE CVGFilterList::AddRef( void )
{
	LONG lRef = InterlockedIncrement(&m_cRef);
	return lRef;
}

ULONG STDMETHODCALLTYPE CVGFilterList::Release( void )
{
	LONG lRef = InterlockedDecrement(&m_cRef);
	if (lRef == 0)
		delete this;
	return lRef;
}

HRESULT STDMETHODCALLTYPE CVGFilterList::Add( IBaseFilter *pBF )
{
	m_filters.push_back(pBF);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CVGFilterList::Clear( void )
{
	m_filters.clear();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CVGFilterList::Delete( DWORD nIndex )
{
	if (nIndex >= m_filters.size())
		return E_INVALIDARG;

	m_filters.erase(m_filters.begin() + nIndex);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CVGFilterList::Get( DWORD nIndex, IBaseFilter** ppBF )
{
	if (nIndex >= m_filters.size())
	{
		*ppBF = NULL;
		return E_INVALIDARG;
	}

	*ppBF = m_filters[nIndex];
	return S_OK;
}

DWORD STDMETHODCALLTYPE CVGFilterList::GetCount( void )
{
	return m_filters.size();
}

