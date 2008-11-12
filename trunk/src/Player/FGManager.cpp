//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FGManager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TFGManager::TFGManager(void)
: TCritSec()
{
	m_pUnkInner.CoCreateInstance(CLSID_FilterGraph);
	m_pFM.CoCreateInstance(CLSID_FilterMapper2);
}

TFGManager::~TFGManager(void)
{

}

/* IUnknown */
STDMETHODIMP TFGManager::QueryInterface(REFIID riid, void **ppvObject)
{
	if (TUnknown<IFilterGraph2>::QueryInterface(riid, ppvObject) == S_OK)
		return S_OK;
	else if (m_pUnkInner != NULL)
		return m_pUnkInner->QueryInterface(riid, ppvObject);
	else
    	return E_NOINTERFACE;
}

/* IFilterGraph */
STDMETHODIMP TFGManager::AddFilter(IBaseFilter *pFilter, LPCWSTR pName)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->AddFilter(pFilter, pName);
}

STDMETHODIMP TFGManager::RemoveFilter(IBaseFilter *pFilter)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->RemoveFilter(pFilter);
}

STDMETHODIMP TFGManager::EnumFilters(IEnumFilters **ppEnum)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->EnumFilters(ppEnum);
}

STDMETHODIMP TFGManager::FindFilterByName(LPCWSTR pName, IBaseFilter **ppFilter)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->FindFilterByName(pName, ppFilter);
}

STDMETHODIMP TFGManager::ConnectDirect(IPin *ppinOut, IPin *ppinIn, const AM_MEDIA_TYPE *pmt)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->ConnectDirect(ppinOut, ppinIn, pmt);
}

STDMETHODIMP TFGManager::Reconnect(IPin *ppin)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->Reconnect(ppin);
}

STDMETHODIMP TFGManager::Disconnect(IPin *ppin)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->Disconnect(ppin);
}

STDMETHODIMP TFGManager::SetDefaultSyncSource(void)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->SetDefaultSyncSource();
}

/* IGraphBuilder */
STDMETHODIMP TFGManager::Connect(IPin *ppinOut, IPin *ppinIn)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->Connect(ppinOut, ppinIn);
}

STDMETHODIMP TFGManager::Render(IPin *ppinOut)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->Render(ppinOut);
}

STDMETHODIMP TFGManager::RenderFile(LPCWSTR lpcwstrFile, LPCWSTR lpcwstrPlayList)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->RenderFile(lpcwstrFile, lpcwstrPlayList);
}

STDMETHODIMP TFGManager::AddSourceFilter(LPCWSTR lpcwstrFileName, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->AddSourceFilter(lpcwstrFileName,
		lpcwstrFilterName, ppFilter);
}

STDMETHODIMP TFGManager::SetLogFile(DWORD_PTR hFile)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->SetLogFile(hFile);
}

STDMETHODIMP TFGManager::Abort(void)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

   	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->Abort();
}

STDMETHODIMP TFGManager::ShouldOperationContinue(void)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

   	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->ShouldOperationContinue();
}

/* IFilterGraph2 */
STDMETHODIMP TFGManager::AddSourceFilterForMoniker(IMoniker *pMoniker, IBindCtx *pCtx,
	LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->AddSourceFilterForMoniker(pMoniker,
		pCtx, lpcwstrFilterName, ppFilter);
}

STDMETHODIMP TFGManager::ReconnectEx(IPin *ppin, const AM_MEDIA_TYPE *pmt)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->ReconnectEx(ppin, pmt);
}
STDMETHODIMP TFGManager::RenderEx(IPin *pPinOut, DWORD dwFlags, DWORD *pvContext)
{
	if(m_pUnkInner == NULL) return E_UNEXPECTED;

	TAutoLock lock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->RenderEx(pPinOut, dwFlags, pvContext);
}
