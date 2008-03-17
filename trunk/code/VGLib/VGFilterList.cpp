#include "StdAfx.h"
#include "VGFilterList.h"

CVGFilter::CVGFilter( const CFactoryTemplate &Templ )
	: m_cRef(0)
{
	m_Templ = Templ;
}

CVGFilter::~CVGFilter( void )
{
	if (m_cRef != 0)
		throw "Invaild pointer";
}

HRESULT STDMETHODCALLTYPE CVGFilter::QueryInterface( REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject )
{
	CheckPointer(ppvObject, E_POINTER);
	ValidateReadWritePtr(ppvObject, sizeof(PVOID));

	/* We know only about IUnknown */

	if (riid == IID_IVGFilter)
	{
		GetInterface((IVGFilter*)this, ppvObject);
		return S_OK;
	} else if (riid == IID_IUnknown) {
		GetInterface((LPUNKNOWN)this, ppvObject);
		return S_OK;
	} else {
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
}

ULONG STDMETHODCALLTYPE CVGFilter::AddRef( void )
{
	LONG lRef = InterlockedIncrement(&m_cRef);
	return lRef;
}

ULONG STDMETHODCALLTYPE CVGFilter::Release( void )
{
	LONG lRef = InterlockedDecrement(&m_cRef);
	if (lRef == 0)
		delete this;
	return lRef;
}

HRESULT STDMETHODCALLTYPE CVGFilter::CreateInstance( IBaseFilter **ppvObj )
{
	CheckPointer(ppvObj, E_POINTER);
	ValidateReadWritePtr(ppvObj, sizeof(IBaseFilter*));
	
	HRESULT hr;
	
	*ppvObj = (CBaseFilter*)m_Templ.CreateInstance(NULL, &hr);
	if (FAILED(hr))
	{
		*ppvObj = NULL;
		return hr;
	}
	return S_OK;
}

CLSID STDMETHODCALLTYPE CVGFilter::GetCLSID( void )
{
	return *m_Templ.m_ClsID;
}

DWORD STDMETHODCALLTYPE CVGFilter::GetMerit( void )
{
	return m_Templ.m_pAMovieSetup_Filter->dwMerit;
}

LPCWSTR STDMETHODCALLTYPE CVGFilter::GetName( void )
{
	return m_Templ.m_Name;
}

DWORD STDMETHODCALLTYPE CVGFilter::GetPinCount( void )
{
	return m_Templ.m_pAMovieSetup_Filter->nPins;
}

HRESULT STDMETHODCALLTYPE CVGFilter::GetPinInfo( const REGFILTERPINS *pInfo )
{
	CheckPointer(pInfo, E_POINTER);

	pInfo = (REGFILTERPINS*)m_Templ.m_pAMovieSetup_Filter;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

CVGFilterList::CVGFilterList(void)
	:m_cRef(0), m_nPos(0)
{
}

CVGFilterList::~CVGFilterList(void)
{
	if (m_cRef != 0)
		throw "Invalid pointer";
}

HRESULT STDMETHODCALLTYPE CVGFilterList::QueryInterface( REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject )
{
	CheckPointer(ppvObject, E_POINTER);
	ValidateReadWritePtr(ppvObject, sizeof(PVOID));

	/* We know only about IUnknown */

	if (riid == IID_IEnumUnknown)
	{
		GetInterface((IEnumUnknown*)this, ppvObject);
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

void CVGFilterList::Add( CFactoryTemplate *pTemplates, UINT nCount )
{
	ValidateReadPtr(pTemplates, sizeof(CFactoryTemplate) * nCount);

	CVGFilter *pFilter = NULL;

	for (UINT i = 0; i < nCount; i++)
	{
		pFilter = new CVGFilter(pTemplates[i]);
		m_filters.push_back(pFilter);
	}
}

void CVGFilterList::Clear( void )
{
	m_filters.clear();
}

HRESULT STDMETHODCALLTYPE CVGFilterList::Next( ULONG celt, IUnknown **rgelt, ULONG *pceltFetched )
{
	CheckPointer(rgelt, E_POINTER);
	ValidateReadWritePtr(rgelt, sizeof(IUnknown*) * celt);	

	ULONG nToRead = m_filters.size() - m_nPos;
	if (nToRead > celt)
		nToRead = celt;
	for (ULONG i = 0; i < nToRead; i++)
	{
		rgelt[i] = m_filters[m_nPos + i];
		m_nPos++;
	}
	if (nToRead > 0)
		return nToRead == celt ? S_OK : S_FALSE;
	else
		return E_FAIL;
}

HRESULT STDMETHODCALLTYPE CVGFilterList::Skip( ULONG celt )
{
	ULONG nRemain = m_filters.size() - m_nPos;
	if (nRemain > 0)
	{
		if (nRemain >= celt)
		{
			m_nPos += celt;
			return S_OK;
		}
		else
		{
			m_nPos += nRemain;
			return S_FALSE;
		}
	}
	else
		return E_FAIL;
}

HRESULT STDMETHODCALLTYPE CVGFilterList::Reset( void )
{
	m_nPos = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CVGFilterList::Clone( __RPC__deref_out_opt IEnumUnknown **ppenum )
{
	CheckPointer(ppenum, E_POINTER);
	ValidateReadWritePtr(ppenum, sizeof(IEnumUnknown*));		

	CVGFilterList *pList = new CVGFilterList;
	pList->AddRef();
	pList->m_filters = m_filters;
	pList->m_nPos = m_nPos;
	*ppenum = pList;
	return S_OK;
}