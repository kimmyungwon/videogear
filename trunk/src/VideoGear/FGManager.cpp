#include "StdAfx.h"
#include "FGManager.h"

CFilterManager::CFilterManager(HRESULT *hr)
: CUnknown(L"CFilterManager", NULL)
{
	if (hr != NULL)
		*hr = m_pFG.CoCreateInstance(CLSID_FilterGraph);
	else
		m_pFG.CoCreateInstance(CLSID_FilterGraph);
}

CFilterManager::~CFilterManager(void)
{
	m_pFG = NULL;
}

/* IGraphBuilder2 */
STDMETHODIMP CFilterManager::IsPinConnected(IPin *ppin)
{
	CheckPointer(ppin, E_POINTER);

	CComPtr<IPin> ppinTo;

	return SUCCEEDED(ppin->ConnectedTo(&ppinTo)) && (ppinTo != NULL) ? S_OK : S_FALSE;
}

/* IFilterGraph2 */
STDMETHODIMP CFilterManager::AddSourceFilterForMoniker(IMoniker *pMoniker, IBindCtx *pCtx, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter) 
{
	return m_pFG->AddSourceFilterForMoniker(pMoniker, pCtx, lpcwstrFilterName, ppFilter);
}

STDMETHODIMP CFilterManager::ReconnectEx(IPin *ppin, const AM_MEDIA_TYPE *pmt) 
{
	return m_pFG->ReconnectEx(ppin, pmt);
}

STDMETHODIMP CFilterManager::RenderEx(IPin *pPinOut, DWORD dwFlags, DWORD *pvContext) 
{
	return m_pFG->RenderEx(pPinOut, dwFlags, pvContext);
}

/* IGraphBuilder */
STDMETHODIMP CFilterManager::Connect(IPin *ppinOut, IPin *ppinIn) 
{
	return m_pFG->Connect(ppinOut, ppinIn);
}

STDMETHODIMP CFilterManager::Render(IPin *ppinOut) 
{
	return m_pFG->Render(ppinOut);
}

STDMETHODIMP CFilterManager::RenderFile(LPCWSTR lpcwstrFile, LPCWSTR lpcwstrPlayList) 
{
	return m_pFG->RenderFile(lpcwstrFile, lpcwstrPlayList);
}

STDMETHODIMP CFilterManager::AddSourceFilter(LPCWSTR lpcwstrFileName, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter) 
{
	return m_pFG->AddSourceFilter(lpcwstrFileName, lpcwstrFilterName, ppFilter);
}

STDMETHODIMP CFilterManager::SetLogFile(DWORD_PTR hFile) 
{
	return m_pFG->SetLogFile(hFile);
}

STDMETHODIMP CFilterManager::Abort(void) 
{
	return m_pFG->Abort();
}

STDMETHODIMP CFilterManager::ShouldOperationContinue(void) 
{
	return m_pFG->ShouldOperationContinue();
}

/* IFilterGraph */
STDMETHODIMP CFilterManager::AddFilter(IBaseFilter *pFilter, LPCWSTR pName) 
{
	return m_pFG->AddFilter(pFilter, pName);
}

STDMETHODIMP CFilterManager::RemoveFilter(IBaseFilter *pFilter) 
{
	return m_pFG->RemoveFilter(pFilter);
}

STDMETHODIMP CFilterManager::EnumFilters(IEnumFilters **ppEnum) 
{
	return m_pFG->EnumFilters(ppEnum);
}

STDMETHODIMP CFilterManager::FindFilterByName(LPCWSTR pName, IBaseFilter **ppFilter) 
{
	return m_pFG->FindFilterByName(pName, ppFilter);
}

STDMETHODIMP CFilterManager::ConnectDirect(IPin *ppinOut, IPin *ppinIn, const AM_MEDIA_TYPE *pmt) 
{
	return m_pFG->ConnectDirect(ppinOut, ppinIn, pmt);
}

STDMETHODIMP CFilterManager::Reconnect(IPin *ppin) 
{
	return m_pFG->Reconnect(ppin);
}

STDMETHODIMP CFilterManager::Disconnect(IPin *ppin) 
{
	return m_pFG->Disconnect(ppin);
}

STDMETHODIMP CFilterManager::SetDefaultSyncSource(void) 
{
	return m_pFG->SetDefaultSyncSource();
}

STDMETHODIMP CFilterManager::NonDelegatingQueryInterface( REFIID riid, void ** ppv )
{
	if (riid == IID_IGraphBuilder2)
	{
		GetInterface((IGraphBuilder2*)this, ppv);
		return S_OK;
	}
	else
	{
		if (m_pFG->QueryInterface(riid, ppv) == S_OK)
			return S_OK;
		else
			return E_NOINTERFACE;
	}
}