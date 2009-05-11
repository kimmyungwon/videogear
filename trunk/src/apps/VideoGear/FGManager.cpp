#include "StdAfx.h"
#include "VideoGear.h"
#include "FGManager.h"
#include "DSUtil.h"

CFGManager::CFGManager( CWnd *pVidWnd )
: CUnknown(L"CFGManager", NULL)
, m_pVidWnd(pVidWnd)
{
	if (SUCCEEDED(m_pGraph.CoCreateInstance(CLSID_FilterGraph)))
	{
		CComPtr<IRunningObjectTable> pROT;

		if (SUCCEEDED(GetRunningObjectTable(0, &pROT)))
		{
			CStringW strName;
			CComPtr<IMoniker> pMoniker;
			DWORD dwRegister;

			strName.Format(L"FilterGraph %08x pid %08x", (DWORD_PTR)(IUnknown*)m_pGraph, GetCurrentProcessId());
			if (SUCCEEDED(CreateItemMoniker(L"!", strName, &pMoniker)))
			{
				pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, m_pGraph, pMoniker, &dwRegister);
			}
		}
	}
}

CFGManager::~CFGManager(void)
{
	ClearGraph();
	m_pGraph = NULL;
}

STDMETHODIMP CFGManager::NonDelegatingQueryInterface( REFIID riid, __deref_out void **ppv )
{
	if (riid == IID_IVMRWindowlessControl9)
	{
		return SUCCEEDED(m_pVideoRenderer->QueryInterface(riid, ppv)) ? S_OK : E_NOINTERFACE;
	}
	else if (riid == IID_IMFVideoDisplayControl)
	{
		CComQIPtr<IMFGetService> pGetSrv = m_pVideoRenderer;
		if (pGetSrv == NULL)
			return E_NOINTERFACE;
		return SUCCEEDED(pGetSrv->GetService(MR_VIDEO_RENDER_SERVICE, riid, ppv)) ? S_OK : E_NOINTERFACE;
	}
	else
	{
		return	QI(IGraphBuilder2)
				QI(IFilterGraph2)
				QI(IGraphBuilder)
				QI(IFilterGraph)
				m_pGraph->QueryInterface(riid, ppv);
	}
}

HRESULT STDMETHODCALLTYPE CFGManager::AddFilter( IBaseFilter *pFilter, LPCWSTR pName )
{
	return m_pGraph->AddFilter(pFilter, pName);
}

HRESULT STDMETHODCALLTYPE CFGManager::RemoveFilter( IBaseFilter *pFilter )
{
	return m_pGraph->RemoveFilter(pFilter);
}

HRESULT STDMETHODCALLTYPE CFGManager::EnumFilters( IEnumFilters **ppEnum )
{
	return m_pGraph->EnumFilters(ppEnum);
}

HRESULT STDMETHODCALLTYPE CFGManager::FindFilterByName( LPCWSTR pName, IBaseFilter **ppFilter )
{
	return m_pGraph->FindFilterByName(pName, ppFilter);
}

HRESULT STDMETHODCALLTYPE CFGManager::ConnectDirect(IPin *ppinOut, IPin *ppinIn, const AM_MEDIA_TYPE *pmt)
{
	return m_pGraph->ConnectDirect(ppinOut, ppinIn, pmt);
}

HRESULT STDMETHODCALLTYPE CFGManager::Reconnect(IPin *ppin)
{
	return m_pGraph->Reconnect(ppin); 
}

HRESULT STDMETHODCALLTYPE CFGManager::Disconnect(IPin *ppin)
{
	return m_pGraph->Disconnect(ppin);
}

HRESULT STDMETHODCALLTYPE CFGManager::SetDefaultSyncSource(void)
{
	return m_pGraph->SetDefaultSyncSource();
}

HRESULT STDMETHODCALLTYPE CFGManager::Connect(IPin *ppinOut, IPin *ppinIn)
{
	return m_pGraph->Connect(ppinOut, ppinIn);
}

HRESULT STDMETHODCALLTYPE CFGManager::Render(IPin *ppinOut)
{
	if (ppinOut == NULL)
		return E_POINTER;
	if (IsPinConnected(ppinOut))
		return VFW_E_ALREADY_CONNECTED;

	XTRACE(L"Rendering [%s(%s)]\n", GetFilterName(GetFilterFromPin(ppinOut)), GetPinName(ppinOut));

	IBaseFilter* pFilterOut;
	
	CAtlList<CMediaType> mtsOut;
	CAtlList<CFilter*> filters;

	/* Æ¥ÅäÒÑÓÐÂË¾µ */

	{
		CAtlList<IBaseFilter*> pCachedFilters;
		CComPtr<IEnumFilters> pEnumFilters;

		pFilterOut = GetFilterFromPin(ppinOut);
		if (SUCCEEDED(EnumFilters(&pEnumFilters)))
		{
			for (CComPtr<IBaseFilter> pFilter; pEnumFilters->Next(1, &pFilter, NULL) == S_OK; pFilter.Release())
			{
				if (pFilterOut == pFilter)
					continue;
				pCachedFilters.AddTail(pFilter);
			}
		}
		POSITION pos = pCachedFilters.GetHeadPosition();
		while (pos != NULL)
		{
			IBaseFilter* pFilter = pCachedFilters.GetNext(pos);
			XTRACE(L"Trying to render [%s(%s)] to [%s]\n", GetFilterName(GetFilterFromPin(ppinOut)), GetPinName(ppinOut), 
				GetFilterName(pFilter));
			if (SUCCEEDED(ConnectDirectEx(ppinOut, pFilter, NULL)))
				return S_OK;
		}
	}
	
	/* Æ¥ÅäÄÚ²¿ÂË¾µ */

	ExtractMediaTypes(ppinOut, mtsOut);
	if(theApp.m_pFilterManager->EnumMatchingFilters(mtsOut, filters) == S_OK)
	{
		POSITION pos = filters.GetHeadPosition();
		while (pos != NULL)
		{
			CFilter *pFilter = filters.GetNext(pos);
			CComPtr<IBaseFilter> pBF;

			if (FAILED(pFilter->CreateInstance(NULL, &pBF))
				|| FAILED(AddFilter(pBF, pFilter->GetName())))
				continue;
			if (SUCCEEDED(ConnectDirectEx(ppinOut, pBF, NULL)))
			{
				if (SUCCEEDED(RenderFilter(pBF)))
					return S_OK;
				NukeDownstream(ppinOut);
			}			
			RemoveFilter(pBF);
		}
	}

	return VFW_E_CANNOT_RENDER; //m_pGraph->Render(ppinOut);
}

HRESULT STDMETHODCALLTYPE CFGManager::RenderFile(LPCWSTR lpcwstrFile, LPCWSTR lpcwstrPlayList)
{
	CComPtr<IBaseFilter> pAudioRenderer;
	CComPtr<IBaseFilter> pSource;
	HRESULT hr;
	
	RIF(ClearGraph());

	switch (g_appCfg.m_VideoRenderer)
	{
	case VR_VMR9:
		RIF(m_pVideoRenderer.CoCreateInstance(CLSID_VideoMixingRenderer9));
		RIF(AddFilter(m_pVideoRenderer, L"Video Mixing Renderer 9"));
		if (m_pVidWnd != NULL)
		{
			CComPtr<IVMRFilterConfig9> pCfg;
			CComPtr<IVMRWindowlessControl9> pWC;

			RIF(m_pVideoRenderer.QueryInterface(&pCfg));
			RIF(pCfg->SetRenderingMode(VMR9Mode_Windowless));
			RIF(m_pVideoRenderer.QueryInterface(&pWC));
			RIF(pWC->SetVideoClippingWindow(m_pVidWnd->m_hWnd));
		}
		break;
	case VR_EVR:
		RIF(m_pVideoRenderer.CoCreateInstance(CLSID_EnhancedVideoRenderer));
		RIF(AddFilter(m_pVideoRenderer, L"Enhanced Video Renderer"));
		break;
	default:
		RIF(m_pVideoRenderer.CoCreateInstance(CLSID_VideoRendererDefault));
		RIF(AddFilter(m_pVideoRenderer, L"Default Video Renderer"));
	}
	RIF(pAudioRenderer.CoCreateInstance(CLSID_DSoundRender));
	RIF(AddFilter(pAudioRenderer, L"Default DirectSound Renderer"));

	RIF(AddSourceFilter(lpcwstrFile, NULL, &pSource));
	if (SUCCEEDED(hr = RenderFilter(pSource)))
	{			
		DumpGraph(m_pGraph, 0);
		return hr;
	}
	else
	{
		return VFW_E_CANNOT_RENDER;
	}
}

HRESULT STDMETHODCALLTYPE CFGManager::AddSourceFilter(LPCWSTR lpcwstrFileName, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter)
{
	return m_pGraph->AddSourceFilter(lpcwstrFileName, lpcwstrFilterName, ppFilter);
}

HRESULT STDMETHODCALLTYPE CFGManager::SetLogFile(DWORD_PTR hFile)
{
	return m_pGraph->SetLogFile(hFile);
}

HRESULT STDMETHODCALLTYPE CFGManager::Abort(void)
{
	return m_pGraph->Abort();
}

HRESULT STDMETHODCALLTYPE CFGManager::ShouldOperationContinue(void)
{
	return m_pGraph->ShouldOperationContinue();
}

HRESULT STDMETHODCALLTYPE CFGManager::AddSourceFilterForMoniker(IMoniker *pMoniker, IBindCtx *pCtx, LPCWSTR lpcwstrFilterName, IBaseFilter **ppFilter)
{
	return m_pGraph->AddSourceFilterForMoniker(pMoniker, pCtx, lpcwstrFilterName, ppFilter);
}

HRESULT STDMETHODCALLTYPE CFGManager::ReconnectEx(IPin *ppin, const AM_MEDIA_TYPE *pmt)
{
	return m_pGraph->ReconnectEx(ppin, pmt);
}

HRESULT STDMETHODCALLTYPE CFGManager::RenderEx(IPin *pPinOut, DWORD dwFlags, DWORD *pvContext)
{
	return m_pGraph->RenderEx(pPinOut, dwFlags, pvContext);
}

HRESULT STDMETHODCALLTYPE CFGManager::NukeDownstream( IUnknown *pUnk )
{
	if (CComQIPtr<IBaseFilter> pFilter = pUnk)
	{
		BeginEnumPins(pFilter, pEnumPins, pPin)
		{
			NukeDownstream(pPin);
		}
		EndEnumPins
		return S_OK;
	}
	else if (CComQIPtr<IPin> pPin = pUnk)
	{
		CComPtr<IPin> pPinTo;

		if (IsPinDir(pPin, PINDIR_OUTPUT) && SUCCEEDED(pPin->ConnectedTo(&pPinTo)) && pPinTo != NULL)
		{
			if (CComPtr<IBaseFilter> pFilter = GetFilterFromPin(pPinTo))
			{
				NukeDownstream(pFilter);
				Disconnect(pPinTo);
				Disconnect(pPin);
				RemoveFilter(pFilter);
			}
		}
		return S_OK;
	}
	else
		return E_INVALIDARG;
}

HRESULT STDMETHODCALLTYPE CFGManager::ClearGraph( void )
{
	CComPtr<IEnumFilters> pEnumFilters;
	CAtlList<IBaseFilter*> pFilters;

	m_pVideoRenderer = NULL;
	RIF(EnumFilters(&pEnumFilters));
	for (CComPtr<IBaseFilter> pFilter; pEnumFilters->Next(1, &pFilter, NULL) == S_OK; pFilter.Release())
	{
		pFilters.AddTail(pFilter);
	}
	POSITION pos = pFilters.GetHeadPosition();
	while (pos != NULL)
	{
		IBaseFilter* pFilter = pFilters.GetNext(pos);
		RemoveFilter(pFilter);
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CFGManager::RenderFilter( IBaseFilter *pFilter )
{
	if (pFilter == NULL)
		return E_POINTER;

	CComPtr<IEnumPins> pEnumPins;
	int nTotal = 0, nRendered = 0;

	BeginEnumPins(pFilter, pEnumPins, pPin)
	{
		if (!IsPinDir(pPin, PINDIR_OUTPUT) || IsPinConnected(pPin))
			continue;
		nTotal++;
		if (SUCCEEDED(Render(pPin)))
			nRendered++;
	}
	EndEnumPins

	if (nTotal > 0)
		if (nRendered > 0)
			return nRendered == nTotal ? S_OK : S_FALSE;
		else
			return VFW_E_CANNOT_RENDER;
	else
		return S_OK;
}

HRESULT STDMETHODCALLTYPE CFGManager::ConnectDirectEx( IPin *ppinOut, IBaseFilter *pFilter, const AM_MEDIA_TYPE *pmt )
{
	if (ppinOut == NULL || pFilter == NULL)
		return E_POINTER;
	if (IsPinConnected(ppinOut))
		return VFW_E_ALREADY_CONNECTED;

	BeginEnumPins(pFilter, pEnumPins, ppinIn)
	{
		if (!IsPinDir(ppinIn, PINDIR_INPUT) || IsPinConnected(ppinIn))
			continue;

		if (pmt != NULL)
		{
			HRESULT hr = ConnectDirect(ppinOut, ppinIn, pmt);
			if (SUCCEEDED(hr))
				return hr;
		}
		else
		{
			HRESULT hr = VFW_E_CANNOT_RENDER;
			
			BeginEnumMediaTypes(ppinOut, pEnumMT, pmtOut)
			{
				hr = ConnectDirect(ppinOut, ppinIn, pmtOut);
				if (SUCCEEDED(hr))
					break;
			}
			EndEnumMediaTypes(pmtOut)
			if (SUCCEEDED(hr))
				return hr;
		}
	}
	EndEnumPins

	return VFW_E_CANNOT_CONNECT;
}

