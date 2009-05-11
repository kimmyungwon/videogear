#include "StdAfx.h"
#include "VideoGear.h"
#include "FGManager.h"
#include "DSUtil.h"

CFGManager::CFGManager( void )
: m_bInitialized(false), m_pVidWnd(NULL)
{
}

CFGManager::~CFGManager(void)
{
	ClearGraph();
	m_pGraph = NULL;
}

HRESULT CFGManager::Initialize( CWnd *pVidWnd )
{
	if (m_bInitialized)
		return E_UNEXPECTED;
	RIF(m_pGraph.CoCreateInstance(CLSID_FilterGraph));
#ifdef _DEBUG
	AddToROT(m_pGraph);
#endif
	m_pVidWnd = pVidWnd;
	m_bInitialized = true;
}

HRESULT STDMETHODCALLTYPE CFGManager::RenderFile( LPCWSTR pszFile )
{
	CComPtr<IBaseFilter> pSource;
	HRESULT hr;
	CComPtr<IBaseFilter> pFilter;

	if (FAILED(ClearGraph()))
		return VFW_E_CANNOT_RENDER;
	hr = AddSourceFilter(pszFile, &pSource);
	if (FAILED(hr))
		return VFW_E_CANNOT_RENDER;
	if (hr == S_OK)
	{
		hr = RenderFilter(pSource);
		if (SUCCEEDED(hr))
			return hr;
	}
	else if (SUCCEEDED(SplitSource(pSource, &pFilter)))
	{
		hr = RenderFilter(pFilter);
		if (SUCCEEDED(hr))
			return hr;	
	}
	// äÖÈ¾Ê§°Ü
	TearDownStream(pSource);
	m_pGraph->RemoveFilter(pSource);
	return VFW_E_CANNOT_RENDER;
}

HRESULT CFGManager::AddSourceFilter( LPCWSTR pszFile, IBaseFilter **ppFilter )
{
	HRESULT hr;
	CComPtr<IBaseFilter> pSource;
	CComPtr<IFileSourceFilter> pFileSource;

	RIF(pSource.CoCreateInstance(CLSID_AsyncReader));
	if (SUCCEEDED(hr = pSource.QueryInterface(&pFileSource))
		&& SUCCEEDED(hr = pFileSource->Load(pszFile, NULL))
		&& SUCCEEDED(hr = m_pGraph->AddFilter(pSource, pszFile)))
	{
		return S_FALSE;
	}
	else
		return hr;
}

HRESULT CFGManager::SplitSource( IBaseFilter *pSource, IBaseFilter **ppFilter )
{
	CComPtr<IPin> pPinOut;
	CAtlList<CMediaType> mtsOut;
	CAtlList<CFilter*> filters;

	ASSERT(IsSourceFilter(pSource));
	if (ppFilter == NULL)
		return E_POINTER;
	pPinOut = GetFirstPin(pSource, PINDIR_OUTPUT);
	ASSERT(pPinOut != NULL);
	ExtractMediaTypes(pPinOut, mtsOut);
	RIF(theApp.m_pFilterManager->EnumMatchingFilters(mtsOut, filters));
	while (filters.GetCount() > 0)
	{
		CFilter *pFilter = filters.RemoveHead();
		//TODO: !!!
	}
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

	switch (g_AppCfg.m_vmr)
	{
	case VRM_VMR9:
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
	case VRM_EVR:
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

HRESULT CFGManager::TearDownStream( IUnknown *pUnk )
{
	if (CComQIPtr<IBaseFilter> pFilter = pUnk)
	{
		BeginEnumPins(pFilter, pEnumPins, pPin)
		{
			TearDownStream(pPin);
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
				TearDownStream(pFilter);
				m_pGraph->Disconnect(pPinTo);
				m_pGraph->Disconnect(pPin);
				m_pGraph->RemoveFilter(pFilter);
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