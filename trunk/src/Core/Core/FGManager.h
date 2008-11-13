#pragma once

#include "IGraphBuilder2.h"

class CFGManager
	: public CUnknown
	, public IGraphBuilder2
	, public CCritSec
{
public:
	CFGManager(__in_opt LPCTSTR pName, __in_opt LPUNKNOWN pUnk);
	~CFGManager(void);

	DECLARE_IUNKNOWN;
	/* INonDelegatingUnknown */
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppvObj);
protected:

	/* IFilterGraph */
	STDMETHODIMP AddFilter(__in IBaseFilter *pFilter, __in LPCWSTR pName);
	STDMETHODIMP RemoveFilter(__in IBaseFilter *pFilter);
	STDMETHODIMP EnumFilters(__out IEnumFilters **ppEnum);
	STDMETHODIMP FindFilterByName(__in LPCWSTR pName, __out IBaseFilter **ppFilter);
	STDMETHODIMP ConnectDirect(__in IPin *ppinOut, __in IPin *ppinIn, __in_opt const AM_MEDIA_TYPE *pmt);
	STDMETHODIMP Reconnect(__in IPin *ppin);
	STDMETHODIMP Disconnect(__in IPin *ppin);
	STDMETHODIMP SetDefaultSyncSource(void);

	/* IGraphBuilder */
	STDMETHODIMP Connect(__in IPin *ppinOut, __in IPin *ppinIn);
	STDMETHODIMP Render(__in IPin *ppinOut);
	STDMETHODIMP RenderFile(__in LPCWSTR lpcwstrFile, __in_opt LPCWSTR lpcwstrPlayList);
	STDMETHODIMP AddSourceFilter(__in LPCWSTR lpcwstrFileName, __in_opt LPCWSTR lpcwstrFilterName, 
		__out IBaseFilter **ppFilter);
	STDMETHODIMP SetLogFile(__in DWORD_PTR hFile);
	STDMETHODIMP Abort(void);
	STDMETHODIMP ShouldOperationContinue(void);

	/* IFilterGraph2 */
	STDMETHODIMP AddSourceFilterForMoniker(__in IMoniker *pMoniker, __in IBindCtx *pCtx, __in LPCWSTR lpcwstrFilterName,
		__out IBaseFilter **ppFilter);
	STDMETHODIMP ReconnectEx(__in IPin *ppin, __in_opt const AM_MEDIA_TYPE *pmt);
	STDMETHODIMP RenderEx(__in IPin *pPinOut, __in DWORD dwFlags, __reserved DWORD *pvContext);
private:
	CComPtr<IUnknown>	m_pUnkInner;
	CComPtr<IFilterMapper2>	m_pFM;
};
