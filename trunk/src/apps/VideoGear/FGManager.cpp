#include "StdAfx.h"
#include "VideoGear.h"
#include "FGManager.h"
#include "FilterManager.h"
#include "DSUtil.h"
#include "DbgUtil.h"

#define AUTOLOCK	CAutoLock __lock__(&m_lock);
#define RTimePerMSec	10000

CAtlMap<HWND, CFGManager*> g_HWNDToFGMgr;

LRESULT CALLBACK VidWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CFGManager *pFGMgr;

	if (g_HWNDToFGMgr.Lookup(hwnd, pFGMgr) && pFGMgr != NULL)
	{
		return pFGMgr->VideoWindowMessageHandler(uMsg, wParam, lParam);
	}
	else
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

CFGManager::CFGManager( void )
: m_state(STATE_UNKNOWN), m_pVidWnd(NULL), m_pfnOldVidWndProc(NULL), m_pEventThread(NULL)
{
}

CFGManager::~CFGManager(void)
{
	ClearGraph();
	SAFE_DELETE(m_pEventThread);
	if (m_pVidWnd != NULL && m_pfnOldVidWndProc != NULL)
	{
		SetWindowLongPtrW(m_pVidWnd->m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_pfnOldVidWndProc);
		g_HWNDToFGMgr.RemoveKey(m_pVidWnd->m_hWnd);
	}
	m_pMS = NULL;
	m_pMC = NULL;
	m_pME = NULL;
	m_pGraph = NULL;
}

HRESULT CFGManager::Initialize( CWnd *pVidWnd )
{		
	if (m_state != STATE_UNKNOWN)
		return E_UNEXPECTED;
	if (pVidWnd == NULL || !IsWindow(pVidWnd->m_hWnd) || g_HWNDToFGMgr.Lookup(pVidWnd->m_hWnd) != NULL)
		return E_INVALIDARG;
	RIF(m_pGraph.CoCreateInstance(CLSID_FilterGraph));
	RIF(m_pGraph.QueryInterface(&m_pME));
	RIF(m_pGraph.QueryInterface(&m_pMC));
	RIF(m_pGraph.QueryInterface(&m_pMS));
#ifdef _DEBUG
	AddToROT(m_pGraph);
#endif
	m_pVidWnd = pVidWnd;
	g_HWNDToFGMgr[m_pVidWnd->m_hWnd] = this;
	m_pfnOldVidWndProc = (WNDPROC)GetWindowLongPtrW(m_pVidWnd->m_hWnd, GWLP_WNDPROC);
	SetWindowLongPtrW(m_pVidWnd->m_hWnd, GWLP_WNDPROC, (LONG_PTR)VidWndProc);
	/* 启动事件监听线程 */
	m_pME->SetNotifyFlags(0);
	m_pEventThread = new CThread<CFGManager>(this, &CFGManager::GraphEventHandler);
	/* 返回 */
	m_state = STATE_IDLE;
	return S_OK;
}

HRESULT CFGManager::RenderFile( LPCWSTR pszFile )
{
	CComPtr<IBaseFilter> pSource;
	HRESULT hr;
	CComPtr<IBaseFilter> pFilter;

	if (m_state == STATE_UNKNOWN)
		return E_UNEXPECTED;
	if (m_state != STATE_IDLE && FAILED(ClearGraph()))
		return VFW_E_CANNOT_RENDER;
	/* 保存本次渲染所用的配置 */
	m_cfgVRM = g_AppCfg.m_VideoRenderMode;
	m_cfgUseAudioSwitcher = g_AppCfg.m_bUseAudioSwitcher;
	/* 初始化视频渲染器 */
	switch (m_cfgVRM)
	{
	case VRM_VMR9:
		RIF(m_pVideoRenderer.CoCreateInstance(CLSID_VideoMixingRenderer9));
		RIF(m_pVideoRenderer.QueryInterface(&m_pVMR9Cfg));
		RIF(m_pVMR9Cfg->SetRenderingMode(VMR9Mode_Windowless));
		RIF(m_pVideoRenderer.QueryInterface(&m_pVMR9WC));
		RIF(m_pVMR9WC->SetVideoClippingWindow(m_pVidWnd->m_hWnd));
		if FAILED(hr = m_pGraph->AddFilter(m_pVideoRenderer, L"Video Mixing Renderer 9"))
		{
			m_pVideoRenderer = NULL;
			return VFW_E_CANNOT_RENDER;
		}
		break;
	case VRM_EVR:
		RIF(m_pVideoRenderer.CoCreateInstance(CLSID_EnhancedVideoRenderer));
		{
			CComPtr<IMFGetService> pGetSrv;
			RIF(m_pVideoRenderer.QueryInterface(&pGetSrv));
			RIF(pGetSrv->GetService(MR_VIDEO_RENDER_SERVICE, IID_IMFVideoDisplayControl, (LPVOID*)&m_pEVRWC));
		}
		RIF(m_pEVRWC->SetVideoWindow(m_pVidWnd->m_hWnd));
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
	if (CheckInUse(m_pVideoRenderer) == S_FALSE)
	{
		m_pVMR9WC = NULL;
		m_pVMR9Cfg = NULL;
		m_pEVRWC = NULL;
		m_pGraph->RemoveFilter(m_pVideoRenderer);
		m_pVideoRenderer = NULL;
	}
	if (CheckInUse(m_pAudioRenderer) == S_FALSE)
	{
		m_pGraph->RemoveFilter(m_pAudioRenderer);
		m_pAudioRenderer = NULL;
	}
	if (m_cfgUseAudioSwitcher && CheckInUse(m_pAudioSwitcher) == S_FALSE)
	{
		m_pGraph->RemoveFilter(m_pAudioSwitcher);
		m_pAudioSwitcher = NULL;
	}
	/* 返回 */
	if (SUCCEEDED(hr))
	{
		// 渲染成功
		m_state = STATE_STOPPED;
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

HRESULT CFGManager::GetDuration( LONGLONG &nDuration )
{
	if (m_state >= STATE_STOPPED)
	{
		RIF(m_pMS->GetDuration(&nDuration));
		nDuration /= RTimePerMSec;
		return S_OK;
	}
	else
		return E_FAIL;
}

HRESULT CFGManager::GetAvailable( LONGLONG &nEarliest, LONGLONG &nLastest )
{
	if (m_state >= STATE_STOPPED)
	{
		RIF(m_pMS->GetAvailable(&nEarliest, &nLastest));
		nEarliest /= RTimePerMSec;
		nLastest /= RTimePerMSec;
		return S_OK;
	}
	else
		return E_FAIL;	
}

HRESULT CFGManager::GetPosition( LONGLONG &nPosition )
{
	if (m_state >= STATE_RUNNING)
	{
		RIF(m_pMS->GetCurrentPosition(&nPosition));
		nPosition /= RTimePerMSec;
		return S_OK;
	}
	else
		return E_FAIL;	
}

HRESULT CFGManager::SetPosition( LONGLONG nPosition )
{
	if (m_state >= STATE_RUNNING)
	{
		DWORD dwCaps;
		LONGLONG nCurrent;
		
		RIF(m_pMS->GetCapabilities(&dwCaps));
		nPosition *= RTimePerMSec;
		RIF(m_pMS->GetCurrentPosition(&nCurrent));
		if (nPosition > nCurrent && (dwCaps & AM_SEEKING_CanSeekForwards)
			|| nPosition < nCurrent && (dwCaps & AM_SEEKING_CanSeekBackwards))
		{
			nPosition -= nCurrent;
			RIF(m_pMS->SetPositions(&nPosition, AM_SEEKING_RelativePositioning, NULL, AM_SEEKING_NoPositioning));
			return S_OK;
		}
		else if (nPosition == nCurrent)
			return S_OK;
		else
			return E_FAIL;
	}
	else
		return E_FAIL;	
}

HRESULT CFGManager::Run( void )
{
	switch (m_state)
	{
	case STATE_STOPPED:
		AdjustVideoPosition();
		return m_pMC->Run();
	case STATE_RUNNING:
		return S_FALSE;
	default:
		return E_FAIL;
	}
}

HRESULT CFGManager::Stop( void )
{
	switch (m_state)
	{
	case STATE_STOPPED:
		return S_FALSE;
	case STATE_RUNNING:
		return m_pMC->Stop();
	default:
		return E_FAIL;
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
			{
				XTRACE(L"成功渲染 [%s(%s)]\n", GetFilterName(pFilterOut), GetPinName(pPinOut));
				return S_OK;
			}
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
				{
					XTRACE(L"成功渲染 [%s(%s)]\n", GetFilterName(pFilterOut), GetPinName(pPinOut));
					return S_OK;
				}
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

HRESULT CFGManager::CheckInUse( IBaseFilter *pFilter )
{
	if (pFilter == NULL)
		return E_POINTER;
	BeginEnumPins(pFilter, pEnumPins, pPin)
	{
		if (IsPinConnected(pPin))
		{
			return S_OK;
			break;
		}
	}
	EndEnumPins;
	return S_FALSE;
}

HRESULT CFGManager::ClearGraph( void )
{
	CInterfaceList<IBaseFilter> pFilters;

	if (m_state == STATE_UNKNOWN)
		return E_UNEXPECTED;
	RIF(m_pMC->Stop());
	m_rctVideo.SetRectEmpty();
	m_pVMR9WC = NULL;
	m_pVMR9Cfg = NULL;
	m_pEVRWC = NULL;
	m_pVideoRenderer = NULL;
	m_pAudioRenderer = NULL;
	m_pAudioSwitcher = NULL;
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
	m_state = STATE_IDLE;
	return S_OK;
}

HRESULT CFGManager::AdjustVideoPosition( void )
{
	AUTOLOCK;
	CSize sizeVideo, sizeARVideo;
	CRect rctWnd;
	long lNewWidth, lNewHeight, lNewLeft, lNewTop;
	
	ASSERT(m_pVidWnd != NULL);
	m_pVidWnd->GetClientRect(rctWnd);
	/* 获取视频原始大小 */
	if (m_cfgVRM == VRM_VMR9 && m_pVMR9WC != NULL)
	{
		RIF(m_pVMR9WC->GetNativeVideoSize(&sizeVideo.cx, &sizeVideo.cy, &sizeARVideo.cx, &sizeARVideo.cy));	
	}
	else if (m_cfgVRM == VRM_EVR && m_pEVRWC != NULL)
	{
		RIF(m_pEVRWC->GetNativeVideoSize(&sizeVideo, &sizeARVideo));
	}
	else
		return E_UNEXPECTED;
	XTRACE(L"Video is %d:%d\n", sizeVideo.cx, sizeVideo.cy);
	if (sizeVideo.cx == 0 || sizeVideo.cy == 0)
		return E_UNEXPECTED;
	/* 按比例缩放 */
	if (sizeVideo.cx / (double)sizeVideo.cy <= rctWnd.Width() / (double)rctWnd.Height())
	{
		lNewHeight = rctWnd.Height();
		lNewWidth = lNewHeight * sizeVideo.cx / sizeVideo.cy;
	}
	else
	{
		lNewWidth = rctWnd.Width();
		lNewHeight = lNewWidth * sizeVideo.cy / sizeVideo.cx;
	}
	lNewLeft = (rctWnd.Width() - lNewWidth) / 2;
	lNewTop = (rctWnd.Height() - lNewHeight) / 2;
	m_rctVideo.SetRect(lNewLeft, lNewTop, lNewLeft + lNewWidth, lNewTop + lNewHeight);
	if (m_cfgVRM == VRM_VMR9)
	{
		return m_pVMR9WC->SetVideoPosition(NULL, m_rctVideo);
	}
	else if (m_cfgVRM == VRM_EVR)
	{
		return m_pEVRWC->SetVideoPosition(NULL, m_rctVideo);
	}
	else
		return E_UNEXPECTED;
}

HREFTYPE CFGManager::RepaintVideo( void )
{
	CPaintDC dc(m_pVidWnd);
	
	if (dc.m_hDC != NULL && !m_rctVideo.IsRectEmpty())
	{
		CRect rctClient;
		CRgn rgnClient, rgnVideo, rgnNotVideo;

		m_pVidWnd->GetClientRect(rctClient);
		rgnClient.CreateRectRgnIndirect(rctClient);	
		rgnVideo.CreateRectRgnIndirect(m_rctVideo);
		rgnNotVideo.CreateRectRgn(0, 0, 0, 0);
		rgnNotVideo.CombineRgn(&rgnClient, &rgnVideo, RGN_DIFF);
		dc.FillRgn(&rgnNotVideo, CBrush::FromHandle((HBRUSH)GetStockBrush(BLACK_BRUSH)));
		/* 绘制视频 */
		if (m_cfgVRM == VRM_VMR9 && m_pVMR9WC != NULL)
		{
			m_pVMR9WC->RepaintVideo(m_pVidWnd->m_hWnd, dc.m_hDC);
			return S_OK;
		}
		else if (m_cfgVRM == VRM_EVR && m_pEVRWC != NULL)
		{
			m_pEVRWC->RepaintVideo();
			return S_OK;
		}
	}
	return E_FAIL;
}

void CFGManager::GraphEventHandler( bool& bTerminated )
{
	while (!bTerminated)
	{
		long lEvCode;
		LONG_PTR lParam1, lParam2;
		
		if (m_pME->GetEvent(&lEvCode, &lParam1, &lParam2, 1) != S_OK)
			continue;
		switch (lEvCode)
		{
		case EC_LENGTH_CHANGED:
			break;
		case EC_VIDEO_SIZE_CHANGED:
			AdjustVideoPosition();
			break;
		}
		m_pME->FreeEventParams(lEvCode, lParam1, lParam2);
	}
}

LRESULT CFGManager::VideoWindowMessageHandler( UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
	switch (uMsg)
	{
	case WM_SIZE:
		if (m_state >= STATE_RUNNING)
			AdjustVideoPosition();
		break;
	case WM_ERASEBKGND:
		if (m_state >= STATE_RUNNING)
			return TRUE;
		break;
	case WM_PAINT:
		if (m_state >= STATE_RUNNING && SUCCEEDED(RepaintVideo()))
			return 0;
		break;		
	}
	return m_pfnOldVidWndProc(m_pVidWnd->m_hWnd, uMsg, wParam, lParam);
}