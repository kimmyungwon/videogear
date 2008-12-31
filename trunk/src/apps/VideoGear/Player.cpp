// Player.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoGear.h"
#include "Player.h"
#include "FGManager.h"
#include "DSUtils.h"
#include "AppSetting.h"
#include "FakeFilterMapper2.h"

//////////////////////////////////////////////////////////////////////////

DWORD WINAPI GraphEventProc( LPVOID lpParameter )
{
	ASSERT(lpParameter != NULL);

	CPlayer* pPlayer = (CPlayer*)lpParameter;
	pPlayer->HandleGraphEvent();
	return 0;
}

// CPlayer

CPlayer::CPlayer()
: m_nState(STATE_UNKNOWN), m_hEventThread(NULL)
{
}

CPlayer::~CPlayer()
{
	Stop();
	if (m_hEventThread != NULL)
	{
		TerminateThread(m_hEventThread, 0);
		m_hEventThread = NULL;
	}
	m_pME = NULL;
	m_pBA = NULL;
	m_pMS = NULL;
	m_pMC = NULL;
	m_pGraph = NULL;
}

HRESULT CPlayer::Initialize( HWND hwndMsg, HWND hwndVid )
{
	HRESULT hr;

	if (m_nState != STATE_UNKNOWN)
		return E_UNEXPECTED;
	m_hwndMsg = hwndMsg;
	m_hwndVid = hwndVid;	
	m_pGraph = new CFGManager(NULL, &hr);
	if (FAILED(hr))
		return hr;
	JIF(m_pGraph.QueryInterface(&m_pMC));
	JIF(m_pGraph.QueryInterface(&m_pMS));
	JIF(m_pGraph.QueryInterface(&m_pBA));
	JIF(m_pGraph.QueryInterface(&m_pME));
	m_hEventThread = CreateThread(NULL, 0, &GraphEventProc, (LPVOID)this, 0, NULL);
	if (m_hEventThread == NULL)
		return HRESULT_FROM_WIN32(GetLastError());
	m_nState = STATE_IDLE;
	return S_OK;
}

UINT CPlayer::GetState( void )
{
	return m_nState;
}

BOOL CPlayer::IsMediaLoaded( void )
{
	return m_nState == STATE_STOPPED || m_nState == STATE_PLAYING || m_nState == STATE_PAUSE;
}

BOOL CPlayer::IsMediaPlaying( void )
{
	return m_nState == STATE_PLAYING || m_nState == STATE_PAUSE;
}

HRESULT CPlayer::OpenMedia( CAutoPtr<OpenMediaData> pOMD )
{
	Stop();
	if (m_pOMD != NULL)
	{
		m_pOMD.Free();
		m_nState = STATE_IDLE;
	}	
	return OpenMediaPrivate(pOMD);
}

HRESULT CPlayer::Play( void )
{	
	HRESULT hr;

	switch (m_nState)
	{
	case STATE_PLAYING:
		return S_OK;
	case STATE_STOPPED:
	case STATE_PAUSE:
		JIF(m_pMC->Run());
		m_nState = STATE_PLAYING;
		return S_OK;
	default:
		return E_UNEXPECTED;
	}
}

HRESULT CPlayer::Pause( void )
{
	HRESULT hr;
	
	switch (m_nState)
	{
	case STATE_PLAYING:
		JIF(m_pMC->Pause());
		m_nState = STATE_PAUSE;
		return S_OK;
	case STATE_PAUSE:
		return S_OK;
	default:
		return E_UNEXPECTED;
	}
}

HRESULT CPlayer::Stop( void )
{	
	HRESULT hr;
	
	switch (m_nState)
	{
	case STATE_UNKNOWN:
		return E_UNEXPECTED;		
	case STATE_OPENNING:
		ASSERT(false);
		break;
	case STATE_STOPPED:
	case STATE_PLAYING:
	case STATE_PAUSE:
		JIF(m_pMC->Stop());
		ClearGraph();
		m_nState = STATE_STOPPED;
		break;
	}
	return S_OK;
}

int CPlayer::GetDuration( void )
{
	DWORD dwCaps;
	LONGLONG nDuration;

	if (!IsMediaLoaded()
		|| m_pMS == NULL
		|| FAILED(m_pMS->GetCapabilities(&dwCaps))
		|| (dwCaps & AM_SEEKING_CanGetDuration) == 0
		|| FAILED(m_pMS->GetDuration(&nDuration)))
		return 0;
	return (int)(nDuration / 10000);	// 转为毫秒
}

int CPlayer::GetCurrentPosisiton( void )
{
	LONGLONG nCurrent;
	
	if (!IsMediaLoaded() 
		|| m_pMS == NULL
		|| FAILED(m_pMS->GetCurrentPosition(&nCurrent)))
		return 0;
	return (int)(nCurrent / 10000);	// 转为毫秒
}

int CPlayer::GetVolume( void )
{
	long nVolume;

	if (m_pBA == NULL
		|| FAILED(m_pBA->get_Volume(&nVolume)))
		return 0;
	return (nVolume + 10000) / 100;	// 转为dB
}

HRESULT CPlayer::SetVolume( int nVol )
{	
	if (m_pBA == NULL)
		return E_UNEXPECTED;
	return m_pBA->put_Volume(nVol * 100 - 10000);
}

HRESULT CPlayer::RepaintVideo( CDC* pDC )
{
	CheckPointer(pDC, E_POINTER);
	if (!IsMediaLoaded())
		return E_UNEXPECTED;
	return m_pWC->RepaintVideo(m_hwndVid, pDC->GetSafeHdc());
}

HRESULT CPlayer::GetVideoPosition( LPRECT lpRect )
{
	if (!IsMediaLoaded() || m_pWC == NULL)	
		return E_UNEXPECTED;
	return m_pWC->GetVideoPosition(NULL, lpRect);
}

HRESULT CPlayer::UpdateVideoPosition( const LPRECT lpRect, bool bInitial /* 是否使用初始化视频大小 */ )
{
	CAppSetting& s = AfxGetAppSetting();
	HRESULT hr;
	LONG nDstW, nDstH, nVidW, nVidH, nARW, nARH, nGCD, nNewW, nNewH, nNewL, nNewT;
	CRect rctWnd;
	
	if (!IsMediaLoaded() || m_pWC == NULL)	
		return E_UNEXPECTED;
	// 调整视频和窗口大小
	JIF(m_pWC->GetNativeVideoSize(&nVidW, &nVidH, &nARW, &nARH));	
	if (nVidW > 0 && nVidH > 0)
	{
		// 按指定的宽高比计算视频大小
		switch (s.GetVideoAspectRatioMode())
		{
		case VARM_VIDEO:
			// 使用上面GetNativeVideoSize获取到的宽高比
			break;
		case VARM_SCREEN:
			nARW = GetSystemMetrics(SM_CXSCREEN);
			nARH = GetSystemMetrics(SM_CYSCREEN);
			nGCD = GetGCD(nARW, nARH);
			nARW /= nGCD;
			nARH /= nGCD;
			nVidW = nVidH * nARW / nARH;
			break;
		case VARM_KEEP_4x3:
			nARW = 4;
			nARH = 3;
			nVidW = nVidH * nARW / nARH;
			break;
		case VARM_KEEP_16x9:
			nARW = 16;
			nARH = 9;
			nVidW = nVidH * nARW / nARH;
			break;
		default:
			ASSERT(false);
			break;
		}
		// 按指定的初始大小缩放视频
		if (!bInitial || s.GetInitialVideoSize() == IVS_DONTCARE)
		{
			nDstW = lpRect->right - lpRect->left;
			nDstH = lpRect->bottom - lpRect->top;
			if (nDstW / (double)nDstH >= nARW / (double)nARH)
			{
				nNewH = nDstH;
				nNewW = nNewH * nARW / nARH;
			}
			else
			{
				nNewW = nDstW;
				nNewH = nNewW * nARH / nARW;
			}
		}
		else
		{
			switch (s.GetInitialVideoSize())
			{
			case IVS_ZOOM50:
				nNewW = nVidW / 2;
				nNewH = nVidH / 2;
				break;
			case IVS_ZOOM100:
				nNewW = nVidW;
				nNewH = nVidH;
				break;
			case IVS_ZOOM200:
				nNewW = nVidW * 2;
				nNewH = nVidH * 2;
				break;
			default:
				ASSERT(false);
				break;
			}
			SendNotify(PN_VIDEOWINDOW_NEED_RESIZE, (LPARAM)(LPSIZE)&CSize(nNewW, nNewH));
			GetClientRect(m_hwndVid, &rctWnd);
			nDstW = rctWnd.Width();
			nDstH = rctWnd.Height();
		}
		nNewL = (nDstW - nNewW) / 2;
		nNewT = (nDstH - nNewH) / 2;
		JIF(m_pWC->SetVideoPosition(NULL, CRect(nNewL, nNewT, nNewL + nNewW, nNewT + nNewH)));
	}
	return S_OK;
}

void CPlayer::ClearGraph( void )
{
	m_pWC->SetVideoClippingWindow(NULL);
	m_pWC = NULL;
	BeginEnumFilters(m_pGraph, pEnumFilters, pFilter)
	{
		TRACE1("Removing %s\n", GetFilterName(pFilter));
		m_pGraph->RemoveFilter(pFilter);
		pEnumFilters->Reset();
	}
	EndEnumFilters
}

HRESULT CPlayer::RenderStreams( IBaseFilter* pSource )
{
	HRESULT hr;
	CComPtr<IBaseFilter> pVMR;
	CComPtr<IVMRFilterConfig> pConfig;
	CComPtr<IVMRWindowlessControl> pWC;
	CComPtr<IBaseFilter> pDSound;
	UINT nTotal, nRendered;

	// 初始化VMR Windowless
	JIF(pVMR.CoCreateInstance(CLSID_VideoMixingRenderer, NULL, CLSCTX_INPROC_SERVER));
	JIF(pVMR.QueryInterface(&pConfig));
	JIF(pConfig->SetRenderingMode(VMRMode_Windowless));
	JIF(pVMR.QueryInterface(&pWC));
	JIF(pWC->SetAspectRatioMode(VMR_ARMODE_NONE));
	JIF(pWC->SetVideoClippingWindow(m_hwndVid));
	JIF(m_pGraph->AddFilter(pVMR, _T("Video Mixing Renderer")));
	// 初始化DSound
	JIF(pDSound.CoCreateInstance(CLSID_DSoundRender, NULL, CLSCTX_INPROC_SERVER));
	JIF(m_pGraph->AddFilter(pDSound, _T("Default DirectSound Renderer")));
	// 渲染
	nTotal = nRendered = 0;
	BeginEnumPins(pSource, pEnumPins, pPin)
		ASSERT(m_pGraph->IsPinConnected(pPin) == S_FALSE);
		if (m_pGraph->IsPinDirection(pPin, PINDIR_OUTPUT) != S_OK)
			continue;
		nTotal++;
		if (SUCCEEDED(m_pGraph->RenderEx(pPin, AM_RENDEREX_RENDERTOEXISTINGRENDERERS, NULL)))
			nRendered++;
	EndEnumPins

	if (nTotal > 0)
	{
		if (nRendered == nTotal)
			hr = S_OK;
		else if (nRendered > 0)
			hr = S_FALSE;
		else
			hr = VFW_E_CANNOT_RENDER;

	}
	else
		hr = S_OK;
	if (SUCCEEDED(hr))
	{
		CRect rctWnd;
				
		m_nState = STATE_PAUSE;
		m_pWC = pWC;
		GetClientRect(m_hwndVid, &rctWnd);
		UpdateVideoPosition(&rctWnd, true);		
	}
	return hr;	
}

HRESULT CPlayer::OpenMediaPrivate( CAutoPtr<OpenMediaData> pOMD )
{
	CheckPointer(pOMD, E_POINTER);

	HRESULT hr;

	m_nState = STATE_OPENNING;
	if (OpenFileData* pOFD = dynamic_cast<OpenFileData*>(pOMD.m_p))
	{
		JIF(OpenFilePrivate(pOFD->strFile));
	}
	else
		return E_INVALIDARG;

	m_pOMD = pOMD;
	m_nState = STATE_STOPPED;
	SendNotify(PN_MEDIA_OPENED, 0);
	return S_OK;
}

HRESULT CPlayer::OpenFilePrivate( const CString& strFile )
{
	HRESULT hr;
	CComPtr<IBaseFilter> pSource;

	JIF(m_pGraph->AddSourceFilter(strFile, NULL, &pSource));
	return RenderStreams(pSource);
}

void CPlayer::HandleGraphEvent( void )
{
	ASSERT(m_pME != NULL);

	long nEventCode;
	LONG_PTR nParam1, nParam2;

	while (m_pME->GetEvent(&nEventCode, &nParam1, &nParam2, INFINITE) == S_OK)
	{
		TRACE1("EventCode = %d\n", nEventCode);
		switch (nEventCode)
		{
		case EC_COMPLETE:
			Stop();
			break;
		}
		m_pME->FreeEventParams(nEventCode, nParam1, nParam2);
	}
}

void CPlayer::SendNotify( UINT nMsg, LPARAM lParam )
{
	SendMessage(m_hwndMsg, WM_PLAYER_NOTIFY, nMsg, lParam);
}












