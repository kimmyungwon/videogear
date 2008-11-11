//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FGManager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TFGManager::TFGManager(void)
{
	m_pUnkInner.CoCreateInstance(CLSID_FilterGraph);
	m_pFM.CoCreateInstance(CLSID_FilterMapper2);
}

/* IFilterGraph */
STDMETHODIMP TFGManager::AddFilter(IBaseFilter *pFilter, LPCWSTR pName) {}
STDMETHODIMP TFGManager::RemoveFilter(IBaseFilter *pFilter) {}
STDMETHODIMP TFGManager::EnumFilters(IEnumFilters **ppEnum) {}
STDMETHODIMP TFGManager::FindFilterByName(LPCWSTR pName, IBaseFilter **ppFilter) {}
STDMETHODIMP TFGManager::ConnectDirect(IPin *ppinOut, IPin *ppinIn, const AM_MEDIA_TYPE *pmt) {}
STDMETHODIMP TFGManager::Reconnect(IPin *ppin)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->Reconnect(ppin);
}

STDMETHODIMP TFGManager::Disconnect(IPin *ppin)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->Disconnect(ppin);
}

STDMETHODIMP TFGManager::SetDefaultSyncSource(void)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->SetDefaultSyncSource();
}

/* IGraphBuilder */
STDMETHODIMP TFGManager::Connect(IPin *ppinOut, IPin *ppinIn) {}
STDMETHODIMP TFGManager::Render(IPin *ppinOut) {}
STDMETHODIMP TFGManager::RenderFile(LPCWSTR lpcwstrFile, LPCWSTR lpcwstrPlayList) {}
STDMETHODIMP TFGManager::AddSourceFilter(LPCWSTR lpcwstrFileName, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter) {}
STDMETHODIMP TFGManager::SetLogFile(DWORD_PTR hFile)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->SetLogFile(hFile);
}

STDMETHODIMP TFGManager::Abort(void)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

   	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->Abort();
}

STDMETHODIMP TFGManager::ShouldOperationContinue(void)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

   	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->ShouldOperationContinue();
}

/* IFilterGraph2 */
STDMETHODIMP TFGManager::AddSourceFilterForMoniker(IMoniker *pMoniker, IBindCtx *pCtx,
	LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->AddSourceFilterForMoniker(pMoniker,
		pCtx, lpcwstrFilterName, ppFilter);
}

STDMETHODIMP TFGManager::ReconnectEx(IPin *ppin, const AM_MEDIA_TYPE *pmt)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

   	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->ReconnectEx(ppin, pmt);
}
STDMETHODIMP TFGManager::RenderEx(IPin *pPinOut, DWORD dwFlags, DWORD *pvContext) {}
