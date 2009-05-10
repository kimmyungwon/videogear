#pragma once

class CFGManager
	: public CUnknown
	, public IFilterGraph2
{
public:
	CFGManager(void);
	~CFGManager(void);
	virtual HRESULT STDMETHODCALLTYPE NukeDownstream(IPin *ppinOut);
	virtual HRESULT STDMETHODCALLTYPE RenderFilter(IBaseFilter *pFilter);
	virtual HRESULT STDMETHODCALLTYPE ConnectDirectEx(IPin *ppinOut, IBaseFilter *pFilter, const AM_MEDIA_TYPE *pmt);
	/* CUnknown */
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);
	/* IFIlterGraph */
	virtual HRESULT STDMETHODCALLTYPE AddFilter(IBaseFilter *pFilter, LPCWSTR pName);
	virtual HRESULT STDMETHODCALLTYPE RemoveFilter(IBaseFilter *pFilter);
	virtual HRESULT STDMETHODCALLTYPE EnumFilters(IEnumFilters **ppEnum);
	virtual HRESULT STDMETHODCALLTYPE FindFilterByName(LPCWSTR pName, IBaseFilter **ppFilter);
	virtual HRESULT STDMETHODCALLTYPE ConnectDirect(IPin *ppinOut, IPin *ppinIn, const AM_MEDIA_TYPE *pmt);
	virtual HRESULT STDMETHODCALLTYPE Reconnect(IPin *ppin);
	virtual HRESULT STDMETHODCALLTYPE Disconnect(IPin *ppin);
	virtual HRESULT STDMETHODCALLTYPE SetDefaultSyncSource(void);
	/* IGraphBuilder */
	virtual HRESULT STDMETHODCALLTYPE Connect(IPin *ppinOut, IPin *ppinIn);
	virtual HRESULT STDMETHODCALLTYPE Render(IPin *ppinOut);
	virtual HRESULT STDMETHODCALLTYPE RenderFile(LPCWSTR lpcwstrFile, LPCWSTR lpcwstrPlayList);
	virtual HRESULT STDMETHODCALLTYPE AddSourceFilter(LPCWSTR lpcwstrFileName, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter);
	virtual HRESULT STDMETHODCALLTYPE SetLogFile(DWORD_PTR hFile);
	virtual HRESULT STDMETHODCALLTYPE Abort(void);
	virtual HRESULT STDMETHODCALLTYPE ShouldOperationContinue(void);
	/* IFilterGraph2 */
	virtual HRESULT STDMETHODCALLTYPE AddSourceFilterForMoniker(IMoniker *pMoniker, IBindCtx *pCtx, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter);
	virtual HRESULT STDMETHODCALLTYPE ReconnectEx(IPin *ppin, const AM_MEDIA_TYPE *pmt);
	virtual HRESULT STDMETHODCALLTYPE RenderEx(IPin *pPinOut, DWORD dwFlags, DWORD *pvContext);
protected:
	DECLARE_IUNKNOWN
private:
	CComPtr<IFilterGraph2> m_pGraph;
};