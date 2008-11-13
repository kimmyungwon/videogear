#include "StdAfx.h"
#include "FGManager.h"
#include "Utils.h"

CFGManager::CFGManager( __in_opt LPCTSTR pName, __in_opt LPUNKNOWN pUnk )
: m_lRef(0)
{
	m_pUnkInner.CoCreateInstance(CLSID_FilterGraph, pUnk);
	m_pFM.CoCreateInstance(CLSID_FilterMapper2);
}

CFGManager::~CFGManager(void)
{
	CAutoLock cAutoLock(this);

	m_pUnkInner.Release();
}

/* IUnknown */

STDMETHODIMP CFGManager::QueryInterface( REFIID riid, __deref_out void **ppvObj )
{
	CheckPointer(ppvObj, E_POINTER);

	return	QI(IGraphBuilder2)
			QI(IGraphBuilderDeadEnd)
			QI(IFilterGraph2)
			QI(IGraphBuilder)
			QI(IFilterGraph)
			(m_pUnkInner != NULL && riid != IID_IUnknown && m_pUnkInner->QueryInterface(riid, ppvObj) == S_OK) ? S_OK :
			E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CFGManager::AddRef()
{
	LONG lRef = InterlockedIncrement(&m_lRef);
	ASSERT(lRef > 0);
	return max((ULONG)lRef, 1ul);
}

STDMETHODIMP_(ULONG) CFGManager::Release()
{
	LONG lRef = InterlockedDecrement(&m_lRef);
	ASSERT(lRef >= 0);
	if (lRef == 0)
	{
		delete this;
		return 0ul;
	}
	else
		return max((ULONG)lRef, 1ul);
}

/* IFilterGraph */

STDMETHODIMP CFGManager::AddFilter( __in IBaseFilter *pFilter, __in LPCWSTR pName )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->AddFilter(pFilter, pName);
}

STDMETHODIMP CFGManager::RemoveFilter( __in IBaseFilter *pFilter )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->RemoveFilter(pFilter);
}

STDMETHODIMP CFGManager::EnumFilters( __out IEnumFilters **ppEnum )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->EnumFilters(ppEnum);
}

STDMETHODIMP CFGManager::FindFilterByName( __in LPCWSTR pName, __out IBaseFilter **ppFilter )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->FindFilterByName(pName, ppFilter);
}

STDMETHODIMP CFGManager::ConnectDirect( __in IPin *ppinOut, __in IPin *ppinIn, __in_opt const AM_MEDIA_TYPE *pmt )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->ConnectDirect(ppinOut, ppinIn, pmt);
}

STDMETHODIMP CFGManager::Reconnect( __in IPin *ppin )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->Reconnect(ppin);
}

STDMETHODIMP CFGManager::Disconnect( __in IPin *ppin )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->Disconnect(ppin);
}

STDMETHODIMP CFGManager::SetDefaultSyncSource( void )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->SetDefaultSyncSource();
}

/* IGraphBuilder */

STDMETHODIMP CFGManager::Connect( __in IPin *ppinOut, __in IPin *ppinIn )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->Connect(ppinOut, ppinIn);
}

STDMETHODIMP CFGManager::Render( __in IPin *ppinOut )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->Render(ppinOut);
}

STDMETHODIMP CFGManager::RenderFile( __in LPCWSTR lpcwstrFile, __in_opt LPCWSTR lpcwstrPlayList )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->RenderFile(lpcwstrFile, lpcwstrPlayList);
}	

STDMETHODIMP CFGManager::AddSourceFilter( __in LPCWSTR lpcwstrFileName, __in_opt LPCWSTR lpcwstrFilterName, __out IBaseFilter **ppFilter )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->AddSourceFilter(lpcwstrFileName, lpcwstrFilterName, ppFilter);
}

STDMETHODIMP CFGManager::SetLogFile( __in DWORD_PTR hFile )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->SetLogFile(hFile);
}

STDMETHODIMP CFGManager::Abort( void )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->Abort();
}

STDMETHODIMP CFGManager::ShouldOperationContinue( void )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->ShouldOperationContinue();
}

/* IFilterGraph2 */

STDMETHODIMP CFGManager::AddSourceFilterForMoniker( __in IMoniker *pMoniker, __in IBindCtx *pCtx, __in LPCWSTR lpcwstrFilterName, __out IBaseFilter **ppFilter )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->AddSourceFilterForMoniker(pMoniker, pCtx, lpcwstrFilterName, ppFilter);
}

STDMETHODIMP CFGManager::ReconnectEx( __in IPin *ppin, __in_opt const AM_MEDIA_TYPE *pmt )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->ReconnectEx(ppin, pmt);	
}

STDMETHODIMP CFGManager::RenderEx( __in IPin *pPinOut, __in DWORD dwFlags, __reserved DWORD *pvContext )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	CAutoLock cAutoLock(this);

	return CComQIPtr<IFilterGraph2>(m_pUnkInner)->RenderEx(pPinOut, dwFlags, pvContext);
}

/* IGraphBuilder2 */
STDMETHODIMP CFGManager::Clear( void )
{
	if (m_pUnkInner == NULL) return E_UNEXPECTED;

	HRESULT hr;
	CAutoLock cAutoLock(this);
	
	BeginEnumFilters(CComQIPtr<IFilterGraph2>(m_pUnkInner), pEnumFilters, pFilter)
		BeginEnumPins(pFilter, pEnumPins, pPin)
			JIF(Disconnect(pPin));
		EndEnumPins
		JIF(RemoveFilter(pFilter));
	EndEnumFilters

	return S_OK;
}

/* IGraphBuilderDeadEnd */
STDMETHODIMP_(size_t) CFGManager::GetCount( void )	
{
	return 0;
}

STDMETHODIMP CFGManager::GetDeadEnd( __in size_t iIndex, __out AM_MEDIA_TYPE** ppMTs, __out size_t cMTs )	
{
	return E_NOTIMPL;
}