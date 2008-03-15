#include "StdAfx.h"
#include "VGFilterList.h"

CVGFilterList::CVGFilterList(void)
	:m_cRef(0)
{
}

CVGFilterList::~CVGFilterList(void)
{
	if (m_cRef != 0)
		throw "Invaild pointer";

	Clear();
}

HRESULT STDMETHODCALLTYPE CVGFilterList::QueryInterface( REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject )
{
	CheckPointer(ppvObject, E_POINTER);
	ValidateReadWritePtr(ppvObject, sizeof(PVOID));

	/* We know only about IUnknown */

	if (riid == IID_IVGFilterList)
	{
		GetInterface((IVGFilterList*)this, ppvObject);
		return S_OK;
	} else if (riid == IID_IUnknown) {
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
	CheckPointer(pBF, E_POINTER);

	pBF->AddRef();
	m_filters.push_back(pBF);
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

HRESULT STDMETHODCALLTYPE CVGFilterList::Get( DWORD nIndex, IBaseFilter** ppBF )
{
	if (nIndex >= m_filters.size())
	{
		*ppBF = NULL;
		return E_INVALIDARG;
	}

	*ppBF = m_filters[nIndex];
	(*ppBF)->AddRef();
	return S_OK;
}

DWORD STDMETHODCALLTYPE CVGFilterList::GetCount( void )
{
	return m_filters.size();
}

