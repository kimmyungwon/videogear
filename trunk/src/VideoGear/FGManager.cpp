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

/* IFilterGraph2 */
HRESULT STDMETHODCALLTYPE CFilterManager::AddSourceFilterForMoniker(IMoniker *pMoniker, IBindCtx *pCtx, 
													LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter) 
{
	return m_pFG->AddSourceFilterForMoniker(pMoniker, pCtx, lpcwstrFilterName, ppFilter);
}

HRESULT STDMETHODCALLTYPE CFilterManager::ReconnectEx(IPin *ppin, const AM_MEDIA_TYPE *pmt) 
{
	return m_pFG->ReconnectEx(ppin, pmt);
}

HRESULT STDMETHODCALLTYPE CFilterManager::RenderEx(IPin *pPinOut, DWORD dwFlags, DWORD *pvContext) 
{
	return m_pFG->RenderEx(pPinOut, dwFlags, pvContext);
}

/* IGraphBuilder */
HRESULT STDMETHODCALLTYPE CFilterManager::Connect(IPin *ppinOut, IPin *ppinIn) 
{
	return m_pFG->Connect(ppinOut, ppinIn);
}

HRESULT STDMETHODCALLTYPE CFilterManager::Render(IPin *ppinOut) 
{
	return m_pFG->Render(ppinOut);
}

HRESULT STDMETHODCALLTYPE CFilterManager::RenderFile(LPCWSTR lpcwstrFile, LPCWSTR lpcwstrPlayList) 
{
	return m_pFG->RenderFile(lpcwstrFile, lpcwstrPlayList);
}

HRESULT STDMETHODCALLTYPE CFilterManager::AddSourceFilter(LPCWSTR lpcwstrFileName, LPCWSTR lpcwstrFilterName, 
														  IBaseFilter **ppFilter) 
{
	return m_pFG->AddSourceFilter(lpcwstrFileName, lpcwstrFilterName, ppFilter);
}

HRESULT STDMETHODCALLTYPE CFilterManager::SetLogFile(DWORD_PTR hFile) 
{
	return m_pFG->SetLogFile(hFile);
}

HRESULT STDMETHODCALLTYPE CFilterManager::Abort(void) 
{
	return m_pFG->Abort();
}

HRESULT STDMETHODCALLTYPE CFilterManager::ShouldOperationContinue(void) 
{
	return m_pFG->ShouldOperationContinue();
}

/* IFilterGraph */
HRESULT STDMETHODCALLTYPE CFilterManager::AddFilter(IBaseFilter *pFilter, LPCWSTR pName) 
{
	return m_pFG->AddFilter(pFilter, pName);
}

HRESULT STDMETHODCALLTYPE CFilterManager::RemoveFilter(IBaseFilter *pFilter) 
{
	return m_pFG->RemoveFilter(pFilter);
}

HRESULT STDMETHODCALLTYPE CFilterManager::EnumFilters(IEnumFilters **ppEnum) 
{
	return m_pFG->EnumFilters(ppEnum);
}

HRESULT STDMETHODCALLTYPE CFilterManager::FindFilterByName(LPCWSTR pName, IBaseFilter **ppFilter) 
{
	return m_pFG->FindFilterByName(pName, ppFilter);
}

HRESULT STDMETHODCALLTYPE CFilterManager::ConnectDirect(IPin *ppinOut, IPin *ppinIn, const AM_MEDIA_TYPE *pmt) 
{
	return m_pFG->ConnectDirect(ppinOut, ppinIn, pmt);
}

HRESULT STDMETHODCALLTYPE CFilterManager::Reconnect(IPin *ppin) 
{
	return m_pFG->Reconnect(ppin);
}

HRESULT STDMETHODCALLTYPE CFilterManager::Disconnect(IPin *ppin) 
{
	return m_pFG->Disconnect(ppin);
}

HRESULT STDMETHODCALLTYPE CFilterManager::SetDefaultSyncSource(void) 
{
	return m_pFG->SetDefaultSyncSource();
}
