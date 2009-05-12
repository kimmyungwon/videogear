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
	/* 保存本次渲染所用的配置 */
	m_cfgVRM = g_AppCfg.m_VideoRenderMode;
	m_cfgUseAudioSwitcher = g_AppCfg.m_bUseAudioSwitcher;
	/* 初始化视频渲染器 */
	switch (m_cfgVRM)
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
	/* 初始化音频渲染器 */
	RIF(m_pAudioRenderer.CoCreateInstance(CLSID_DSoundRender));
	RIF(m_pGraph->AddFilter(m_pAudioRenderer, L"Default DirectSound Renderer"));
	/* 初始化音频切换器 */
	if (m_cfgUseAudioSwitcher)
	{
		RIF(g_FilterMgr.AddAudioSwitcherToGraph(m_pGraph, &m_pAudioSwitcher));
	}
	/* 加载文件 */
	hr = AddSourceFilter(pszFile, &pSource);
	if (FAILED(hr))
		return VFW_E_CANNOT_RENDER;
	/* 渲染文件 */
	if (hr == S_OK)
	{
		hr = RenderFilter(pSource, true);		
	}
	else if (SUCCEEDED(SplitSource(pSource, &pFilter)))
	{
		hr = RenderFilter(pFilter, true);
	}
	/* 清理无用的滤镜 */
	RemoveIfNotUsed(m_pVideoRenderer);
	RemoveIfNotUsed(m_pAudioRenderer);
	if (m_cfgUseAudioSwitcher)
		RemoveIfNotUsed(m_pAudioSwitcher);
	/* 返回 */
	if (SUCCEEDED(hr))
	{
		// 渲染成功
		DumpGraph(m_pGraph, 0);
		return hr;
	}
	else
	{
		// 渲染失败
		TearDownStream(pSource);
		m_pGraph->RemoveFilter(pSource);
		return VFW_E_CANNOT_RENDER;
	}
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
	XTRACE(L"尝试渲染 [%s]\n", GetFilterName(pFilter));
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
			//TODO: 报告错误
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
	XTRACE(L"尝试渲染 [%s(%s)]\n", GetFilterName(pFilterOut), GetPinName(pPinOut));
	/* 匹配已有滤镜 */
	{
		CInterfaceList<IBaseFilter> pCachedFilters;

		BeginEnumFilters(m_pGraph, pEnumFilters, pCachedFilter)
		{
			if (pCachedFilter == pFilterOut || pCachedFilter == m_pAudioSwitcher)
				continue;
			pCachedFilters.AddTail(pCachedFilter);
		}
		EndEnumFilters;
		POSITION pos = pCachedFilters.GetHeadPosition();
		while (pos != NULL)
		{
			CComPtr<IBaseFilter> pFilter = pCachedFilters.GetNext(pos);
			XTRACE(L"尝试渲染 [%s(%s)] 到 [%s]\n", GetFilterName(pFilterOut), GetPinName(pPinOut), GetFilterName(pFilter));
			if (SUCCEEDED(ConnectDirect(pPinOut, pFilter, NULL)))
				return S_OK;
		}
	}
	/* 匹配内部滤镜 */
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
	/* 无法渲染 */
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
		if (SUCCEEDED(ConnectDirect(pPinOut, pPinIn, pmt)))
			return S_OK;
	}
	EndEnumPins;
	/* 无法连接 */
	return VFW_E_CANNOT_CONNECT;
}

HRESULT CFGManager::ConnectDirect( IPin *pPinOut, IPin *pPinIn, const AM_MEDIA_TYPE *pmt )
{	
	CComPtr<IBaseFilter> pFilterOut, pFilterIn;
	
	if (pPinOut == NULL || pPinIn == NULL)
		return E_POINTER;
	if (!IsPinDir(pPinOut, PINDIR_OUTPUT) || !IsPinDir(pPinIn, PINDIR_INPUT))
		return VFW_E_INVALID_DIRECTION;
	if (IsPinConnected(pPinOut) || IsPinConnected(pPinIn))
		return VFW_E_ALREADY_CONNECTED;
	/* 在AudioRenderer前加入AudioSwitcher */
	pFilterOut = GetFilterFromPin(pPinOut);
	pFilterIn = GetFilterFromPin(pPinIn);
	if (m_cfgUseAudioSwitcher && pFilterOut != m_pAudioSwitcher && pFilterIn == m_pAudioRenderer)
	{
		CComPtr<IPin> pSWPinIn, pSWPinOut;
		
		/* 连接到AudioSwitcher */
		pSWPinIn = GetFirstDisconnectedPin(m_pAudioSwitcher, PINDIR_INPUT);
		ASSERT(pSWPinIn != NULL);
		RIF(m_pGraph->ConnectDirect(pPinOut, pSWPinIn, pmt));
		/* 连接AudioSwitcher到AudioRenderer */
		pSWPinOut = GetFirstDisconnectedPin(m_pAudioSwitcher, PINDIR_OUTPUT);
		ASSERT(pPinOut != NULL);
		if (!IsPinConnected(pSWPinOut))
		{
			RIF(ConnectDirect(pSWPinOut, m_pAudioRenderer, NULL));
		}
		return S_OK;
	}
	else
		return m_pGraph->ConnectDirect(pPinOut, pPinIn, pmt);
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

HRESULT CFGManager::RemoveIfNotUsed( CComPtr<IBaseFilter> &pFilter )
{
	if (pFilter != NULL)
	{
		bool bNeedRemove = true;
		BeginEnumPins(pFilter, pEnumPins, pPin)
		{
			if (IsPinConnected(pPin))
			{
				bNeedRemove = false;
				break;
			}
		}
		EndEnumPins;
		if (bNeedRemove)
		{
			m_pGraph->RemoveFilter(pFilter);
			pFilter = NULL;
		}
	}	
	return S_OK;
}

HRESULT CFGManager::ClearGraph( void )
{
	CInterfaceList<IBaseFilter> pFilters;

	RIF(m_pMC->Stop());
	m_pVideoRenderer = NULL;
	BeginEnumFilters(m_pGraph, pEnumFilters, pFilter)
	{
		XTRACE(L"即将从滤镜中删除 [%s]\n", GetFilterName(pFilter));
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

