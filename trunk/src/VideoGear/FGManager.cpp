#include "StdAfx.h"
#include "FGManager.h"
#include "VideoGear.h"

CFGManager::CFGManager( LPUNKNOWN pUnk, HRESULT *hr )
: CUnknown(L"CFilterManager", pUnk)
{
	if (hr != NULL)
		*hr = m_pFG.CoCreateInstance(CLSID_FilterGraph);
	else
		m_pFG.CoCreateInstance(CLSID_FilterGraph);
}

CFGManager::~CFGManager(void)
{
	m_pFG = NULL;
}

/* IGraphBuilder2 */
STDMETHODIMP CFGManager::IsPinConnected(IPin *ppin)
{
	CheckPointer(ppin, E_POINTER);

	CComPtr<IPin> ppinTo;

	return SUCCEEDED(ppin->ConnectedTo(&ppinTo)) && (ppinTo != NULL) ? S_OK : S_FALSE;
}

STDMETHODIMP CFGManager::IsPinDirection( IPin *ppin, PIN_DIRECTION dir )
{
	CheckPointer(ppin, E_POINTER);

	HRESULT hr;
	PIN_DIRECTION pindir;

	JIF(ppin->QueryDirection(&pindir));
	return pindir == dir ? S_OK : S_FALSE;
}

/* IFilterGraph2 */
STDMETHODIMP CFGManager::AddSourceFilterForMoniker(IMoniker *pMoniker, IBindCtx *pCtx, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter) 
{
	return m_pFG->AddSourceFilterForMoniker(pMoniker, pCtx, lpcwstrFilterName, ppFilter);
}

STDMETHODIMP CFGManager::ReconnectEx(IPin *ppin, const AM_MEDIA_TYPE *pmt) 
{
	return m_pFG->ReconnectEx(ppin, pmt);
}

STDMETHODIMP CFGManager::RenderEx(IPin *pPinOut, DWORD dwFlags, DWORD *pvContext) 
{
	return m_pFG->RenderEx(pPinOut, dwFlags, pvContext);
}

/* IGraphBuilder */
STDMETHODIMP CFGManager::Connect(IPin *ppinOut, IPin *ppinIn) 
{
	return m_pFG->Connect(ppinOut, ppinIn);
}

STDMETHODIMP CFGManager::Render(IPin *ppinOut) 
{
	return m_pFG->Render(ppinOut);
}

STDMETHODIMP CFGManager::RenderFile(LPCWSTR lpcwstrFile, LPCWSTR lpcwstrPlayList) 
{
	return m_pFG->RenderFile(lpcwstrFile, lpcwstrPlayList);
}

STDMETHODIMP CFGManager::AddSourceFilter(LPCWSTR lpcwstrFileName, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter) 
{
	return m_pFG->AddSourceFilter(lpcwstrFileName, lpcwstrFilterName, ppFilter);
}

STDMETHODIMP CFGManager::SetLogFile(DWORD_PTR hFile) 
{
	return m_pFG->SetLogFile(hFile);
}

STDMETHODIMP CFGManager::Abort(void) 
{
	return m_pFG->Abort();
}

STDMETHODIMP CFGManager::ShouldOperationContinue(void) 
{
	return m_pFG->ShouldOperationContinue();
}

/* IFilterGraph */
STDMETHODIMP CFGManager::AddFilter(IBaseFilter *pFilter, LPCWSTR pName) 
{
	return m_pFG->AddFilter(pFilter, pName);
}

STDMETHODIMP CFGManager::RemoveFilter(IBaseFilter *pFilter) 
{
	return m_pFG->RemoveFilter(pFilter);
}

STDMETHODIMP CFGManager::EnumFilters(IEnumFilters **ppEnum) 
{
	return m_pFG->EnumFilters(ppEnum);
}

STDMETHODIMP CFGManager::FindFilterByName(LPCWSTR pName, IBaseFilter **ppFilter) 
{
	return m_pFG->FindFilterByName(pName, ppFilter);
}

STDMETHODIMP CFGManager::ConnectDirect(IPin *ppinOut, IPin *ppinIn, const AM_MEDIA_TYPE *pmt) 
{
	return m_pFG->ConnectDirect(ppinOut, ppinIn, pmt);
}

STDMETHODIMP CFGManager::Reconnect(IPin *ppin) 
{
	return m_pFG->Reconnect(ppin);
}

STDMETHODIMP CFGManager::Disconnect(IPin *ppin) 
{
	return m_pFG->Disconnect(ppin);
}

STDMETHODIMP CFGManager::SetDefaultSyncSource(void) 
{
	return m_pFG->SetDefaultSyncSource();
}

STDMETHODIMP CFGManager::NonDelegatingQueryInterface( REFIID riid, void ** ppv )
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

