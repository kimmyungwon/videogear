//---------------------------------------------------------------------------

#ifndef FGManagerH
#define FGManagerH

#include <dshow.h>
#include "ComBase.h"
#include "ThreadUtils.h"

#undef INTERFACE
#define INTERFACE IGraphBuilder2

DECLARE_INTERFACE_IID_(IGraphBuilder2, IFilterGraph2, "{11FE21FA-0602-4361-AC34-0DD55188C7E7}")
{

};

class TFGManager : public TUnknown<IFilterGraph2>, public TCritSec
{
public:
	TFGManager(void);
	virtual ~TFGManager(void);
protected:
	/* IUnknown */
    STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject);

	/* IFilterGraph */
	STDMETHODIMP AddFilter(IBaseFilter *pFilter, LPCWSTR pName);
	STDMETHODIMP RemoveFilter(IBaseFilter *pFilter);
	STDMETHODIMP EnumFilters(IEnumFilters **ppEnum);
	STDMETHODIMP FindFilterByName(LPCWSTR pName, IBaseFilter **ppFilter);
	STDMETHODIMP ConnectDirect(IPin *ppinOut, IPin *ppinIn, const AM_MEDIA_TYPE *pmt);
	STDMETHODIMP Reconnect(IPin *ppin);
	STDMETHODIMP Disconnect(IPin *ppin);
	STDMETHODIMP SetDefaultSyncSource(void);

	/* IGraphBuilder */
	STDMETHODIMP Connect(IPin *ppinOut, IPin *ppinIn);
	STDMETHODIMP Render(IPin *ppinOut);
	STDMETHODIMP RenderFile(LPCWSTR lpcwstrFile, LPCWSTR lpcwstrPlayList);
	STDMETHODIMP AddSourceFilter(LPCWSTR lpcwstrFileName, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter);
	STDMETHODIMP SetLogFile(DWORD_PTR hFile);
	STDMETHODIMP Abort(void);
	STDMETHODIMP ShouldOperationContinue(void);

	/* IFilterGraph2 */
	STDMETHODIMP AddSourceFilterForMoniker(IMoniker *pMoniker, IBindCtx *pCtx, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter);
	STDMETHODIMP ReconnectEx(IPin *ppin, const AM_MEDIA_TYPE *pmt);
	STDMETHODIMP RenderEx(IPin *pPinOut, DWORD dwFlags, DWORD *pvContext);
private:
	CComPtr<IUnknown> m_pUnkInner;
	CComPtr<IFilterMapper2> m_pFM;
};

//---------------------------------------------------------------------------
#endif
