#include "StdAfx.h"
#include "VideoGear.h"
#include "FGManager.h"
#include "FilterManager.h"
#include "DSUtil.h"
#include "DbgUtil.h"

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
	RIF(m_pGraph.QueryInterface(&m_pMC));
#ifdef _DEBUG
	AddToROT(m_pGraph);
#endif
	m_pVidWnd = pVidWnd;
	m_bInitialized = true;
	return S_OK;
}

HRESULT CFGManager::RenderFile( LPCWSTR pszFile )
{
	CComPtr<IBaseFilter> pSource;
	HRESULT hr;
	CComPtr<IBaseFilter> pFilter;

	if (FAILED(ClearGraph()))
		return VFW_E_CANNOT_RENDER;
	/* ≥ı ºªØ‰÷»æ∆˜ */
	switch (g_AppCfg.m_VideoRenderMode)
	{
	case VRM_VMR9:
		RIF(m_pVideoRenderer.CoCreateInstance(CLSID_VideoMixingRenderer9));
		if FAILED(hr = m_pGraph->AddFilter(m_pVideoRenderer, L"Video Mixing Renderer 9"))
		{
			m_pVideoRenderer = NULL;
			return VFW_E_CANNOT_RENDER;
		}
		break;
	case VRM_EVR:
		RIF(m_pVideoRenderer.CoCreateInstance(CLSID_EnhancedVideoRenderer));
		if FAILED(hr = m_pGraph->AddFilter(m_pVideoRenderer, L"Enhanced Video Renderer"))
		{
			m_pVideoRenderer = NULL;
			return VFW_E_CANNOT_RENDER;
		}
		break;
	default:
		return VFW_E_CANNOT_RENDER;
	}
	/* º”‘ÿŒƒº˛ */
	hr = AddSourceFilter(pszFile, &pSource);
	if (FAILED(hr))
		return VFW_E_CANNOT_RENDER;
	/* ‰÷»æŒƒº˛ */
	if (hr == S_OK)
	{
		hr = RenderFilter(pSource, true);
		if (SUCCEEDED(hr))
			return hr;
	}
	else if (SUCCEEDED(SplitSource(pSource, &pFilter)))
	{
		hr = RenderFilter(pFilter, true);
		if (SUCCEEDED(hr))
		{
			DumpGraph(m_pGraph, 0);
			return hr;	
		}
	}
	// ‰÷»æ ß∞‹
	TearDownStream(pSource);
	m_pGraph->RemoveFilter(pSource);
	return VFW_E_CANNOT_RENDER;
}

HRESULT CFGManager::AddSourceFilter( LPCWSTR pszFile, IBaseFilter **ppFilter )
{
	HRESULT hr;
	CComPtr<IFileSourceFilter> pFileSource;

	if (ppFilter == NULL)
		return E_POINTER;
	RIF(CoCreateInstance(CLSID_AsyncReader, NULL, CLSCTX_ALL, IID_IBaseFilter, (LPVOID*)ppFilter));
	if (SUCCEEDED(hr = (*ppFilter)->QueryInterface(IID_IFileSourceFilter, (LPVOID*)&pFileSource))
		&& SUCCEEDED(hr = pFileSource->Load(pszFile, NULL))
		&& SUCCEEDED(hr = m_pGraph->AddFilter(*ppFilter, pszFile)))
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
	RIF(g_FilterMgr.EnumMatchingFilters(mtsOut, filters));
	while (filters.GetCount() > 0)
	{
		CFilter *pFilter = filters.RemoveHead();

		if (FAILED(pFilter->CreateInstance(NULL, ppFilter))
			|| FAILED(m_pGraph->AddFilter(*ppFilter, pFilter->GetName())))
			continue;
		if (SUCCEEDED(ConnectDirect(pPinOut, *ppFilter, NULL)))
		{
			return S_OK;
		}
		m_pGraph->RemoveFilter(*ppFilter);
		(*ppFilter)->Release();
	}
	return VFW_E_UNSUPPORTED_STREAM;
}

HRESULT CFGManager::RenderFilter( IBaseFilter *pFilter, bool bReportError )
{
	UINT nTotal = 0, nRendered = 0;
	
	if (pFilter == NULL)
		return E_POINTER;
	XTRACE(L"≥¢ ‘‰÷»æ [%s]\n", GetFilterName(pFilter));
	BeginEnumPins(pFilter, pEnumPins, pPinOut)
	{
		if (!IsPinDir(pPinOut, PINDIR_OUTPUT) || IsPinConnected(pPinOut))
			continue;
		nTotal++;
		if (SUCCEEDED(Render(pPinOut)))
		{
			nRendered++;
		}
		else if (bReportError)
		{
			//TODO: ±®∏Ê¥ÌŒÛ
		}
	}
	EndEnumPins;
	if (nTotal > 0)
	{
		if (nRendered > 0)
			return nRendered == nTotal ? S_OK : S_FALSE;
		else
			return VFW_E_CANNOT_RENDER;
	}
	else
		return S_OK;
}

HRESULT CFGManager::Render( IPin *pPinOut )
{
	CComPtr<IBaseFilter> pFilterOut;
	CAtlList<CMediaType> mtsOut;
	CAtlList<CFilter*> matchingFilters;
	
	if (pPinOut == NULL)
		return E_POINTER;
	if (!IsPinDir(pPinOut, PINDIR_OUTPUT))
		return VFW_E_INVALID_DIRECTION;
	if (IsPinConnected(pPinOut))
		return VFW_E_ALREADY_CONNECTED;
	pFilterOut = GetFilterFromPin(pPinOut);
	XTRACE(L"≥¢ ‘‰÷»æ [%s(%s)]\n", GetFilterName(pFilterOut), GetPinName(pPinOut));
	/* ∆•≈‰“—”–¬Àæµ */
	{
		CInterfaceList<IBaseFilter> pCachedFilters;

		BeginEnumFilters(m_pGraph, pEnumFilters, pCachedFilter)
		{
			if (pCachedFilter == pFilterOut)
				continue;
			pCachedFilters.AddTail(pCachedFilter);
		}
		EndEnumFilters;
		POSITION pos = pCachedFilters.GetHeadPosition();
		while (pos != NULL)
		{
			CComPtr<IBaseFilter> pFilter = pCachedFilters.GetNext(pos);
			XTRACE(L"≥¢ ‘‰÷»æ [%s(%s)] µΩ [%s]\n", GetFilterName(pFilterOut), GetPinName(pPinOut), GetFilterName(pFilter));
			if (SUCCEEDED(ConnectDirect(pPinOut, pFilter, NULL)))
				return S_OK;
		}
	}
	/* ∆•≈‰ƒ⁄≤ø¬Àæµ */
	ExtractMediaTypes(pPinOut, mtsOut);
	if(g_FilterMgr.EnumMatchingFilters(mtsOut, matchingFilters) == S_OK)
	{
		POSITION pos = matchingFilters.GetHeadPosition();
		while (pos != NULL)
		{
			CFilter *pFilter = matchingFilters.GetNext(pos);
			CComPtr<IBaseFilter> pBF;

			if (FAILED(pFilter->CreateInstance(NULL, &pBF))
				|| FAILED(m_pGraph->AddFilter(pBF, pFilter->GetName())))
				continue;
			if (SUCCEEDED(ConnectDirect(pPinOut, pBF, NULL)))
			{
				if (SUCCEEDED(RenderFilter(pBF)))
					return S_OK;
				TearDownStream(pBF);
			}			
			m_pGraph->RemoveFilter(pBF);
		}
	}
	/* Œﬁ∑®‰÷»æ */
	return VFW_E_CANNOT_RENDER;
}

HRESULT CFGManager::ConnectDirect( IPin *pPinOut, IBaseFilter *pFilterIn, const AM_MEDIA_TYPE *pmt )
{
	if (pPinOut == NULL || pFilterIn == NULL)
		return E_POINTER;
	if (!IsPinDir(pPinOut, PINDIR_OUTPUT))
		return VFW_E_INVALID_DIRECTION;
	if (IsPinConnected(pPinOut))
		return VFW_E_ALREADY_CONNECTED;
	BeginEnumPins(pFilterIn, pEnumPins, pPinIn)
	{
		if (!IsPinDir(pPinIn, PINDIR_INPUT) || IsPinConnected(pPinIn))
			continue;
		if (SUCCEEDED(m_pGraph->ConnectDirect(pPinOut, pPinIn, pmt)))
			return S_OK;
	}
	EndEnumPins;
	/* Œﬁ∑®¡¨Ω” */
	return VFW_E_CANNOT_CONNECT;
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

HRESULT CFGManager::ClearGraph( void )
{
	CInterfaceList<IBaseFilter> pFilters;

	RIF(m_pMC->Stop());
	m_pVideoRenderer = NULL;
	BeginEnumFilters(m_pGraph, pEnumFilters, pFilter)
	{
		XTRACE(L"º¥Ω´¥”¬Àæµ÷–…æ≥˝ [%s]", GetFilterName(pFilter));
		pFilters.AddTail(pFilter);
	}
	EndEnumFilters;
	POSITION pos = pFilters.GetHeadPosition();
	while (pos != NULL)
	{
		CComPtr<IBaseFilter> pFilter = pFilters.GetNext(pos);
		m_pGraph->RemoveFilter(pFilter);
	}
	return S_OK;
}