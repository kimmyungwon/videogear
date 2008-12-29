#pragma once

#include "IGraphBuilder2.h"

class CFGManager : public CUnknown
					 , public IGraphBuilder2
					 , public CCritSec
{
	DECLARE_IUNKNOWN
public:
	CFGManager(HRESULT *hr = NULL);
	virtual ~CFGManager(void);
protected:
	/* IGraphBuilder2 */
	STDMETHODIMP IsPinConnected(IPin *ppin);
	STDMETHODIMP IsPinDirection(IPin *ppin, PIN_DIRECTION dir);
	/* IFilterGraph2 */
	STDMETHODIMP AddSourceFilterForMoniker(IMoniker *pMoniker, IBindCtx *pCtx, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter);
	STDMETHODIMP ReconnectEx(IPin *ppin, const AM_MEDIA_TYPE *pmt);
	STDMETHODIMP RenderEx(IPin *pPinOut, DWORD dwFlags, DWORD *pvContext);
	/* IGraphBuilder */
	STDMETHODIMP Connect(IPin *ppinOut, IPin *ppinIn);
	STDMETHODIMP Render(IPin *ppinOut);
	STDMETHODIMP RenderFile(LPCWSTR lpcwstrFile, LPCWSTR lpcwstrPlayList);
	STDMETHODIMP AddSourceFilter(LPCWSTR lpcwstrFileName, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter);
	STDMETHODIMP SetLogFile(DWORD_PTR hFile);
	STDMETHODIMP Abort(void);
	STDMETHODIMP ShouldOperationContinue(void);
	/* IFilterGraph */
	STDMETHODIMP AddFilter(IBaseFilter *pFilter, LPCWSTR pName);
	STDMETHODIMP RemoveFilter(IBaseFilter *pFilter);
	STDMETHODIMP EnumFilters(IEnumFilters **ppEnum);
	STDMETHODIMP FindFilterByName(LPCWSTR pName, IBaseFilter **ppFilter);
	STDMETHODIMP ConnectDirect(IPin *ppinOut, IPin *ppinIn, const AM_MEDIA_TYPE *pmt);
	STDMETHODIMP Reconnect(IPin *ppin);
	STDMETHODIMP Disconnect(IPin *ppin);
	STDMETHODIMP SetDefaultSyncSource(void);
	/* INonDelegatingUnknown */
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);
private:
	CComPtr<IFilterGraph2>	m_pFG;
};
