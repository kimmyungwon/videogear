// Player.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoGear.h"
#include "Player.h"
#include "FGManager.h"
#include "DSUtils.h"

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
	m_pGraph = new CFGManager(&hr);
	if (FAILED(hr))
		return hr;
	JIF(m_pGraph.QueryInterface(&m_pMC));
	JIF(m_pGraph.QueryInterface(&m_pME));
	m_hEventThread = CreateThread(NULL, 0, &GraphEventProc, (LPVOID)this, 0, NULL);
	if (m_hEventThread == NULL)
		return HRESULT_FROM_WIN32(GetLastError());
	return S_OK;
}

UINT CPlayer::GetState( void )
{
	return m_nState;
}

BOOL CPlayer::IsMediaLoaded( void )
{
	return m_nState == STATE_PLAYING || m_nState == STATE_PAUSE;
}

HRESULT CPlayer::OpenMedia( CAutoPtr<OpenMediaData> pOMD )
{
	return OpenMediaPrivate(pOMD);
}

HRESULT CPlayer::OpenNext( void )
{
	if (m_pOMD == NULL)
		return E_UNEXPECTED;
	Stop();
	if (OpenFileData* pOFD = dynamic_cast<OpenFileData*>(m_pOMD.m_p))
	{
		ASSERT(pOFD->gFiles.size() > 0);
		pOFD->nIndex = (pOFD->nIndex + 1) % pOFD->gFiles.size();
		return OpenFilePrivate(pOFD->gFiles[pOFD->nIndex]);
	}
	else
		return E_INVALIDARG;
}

HRESULT CPlayer::Play( void )
{
	if (!IsMediaLoaded())
		return E_UNEXPECTED;
	return m_pMC->Run();
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

HRESULT CPlayer::UpdateVideoPosition( const LPRECT lpRect )
{
	HRESULT hr;
	LONG nDstW, nDstH, nVidW, nVidH, nARW, nARH, nNewW, nNewH, nNewL, nNewT;
	
	if (!IsMediaLoaded() || m_pWC == NULL)	
		return E_UNEXPECTED;
	nDstW = lpRect->right - lpRect->left;
	nDstH = lpRect->bottom - lpRect->top;
	JIF(m_pWC->GetNativeVideoSize(&nVidW, &nVidH, &nARW, &nARH));
	if (nVidW != 0 && nVidH != 0)
	{
		if (nDstW / (double)nDstH >= nVidW / (double)nVidH)
		{
			nNewH = nDstH;
			nNewW = nNewH * nVidW / nVidH;
		}
		else
		{
			nNewW = nDstW;
			nNewH = nNewW * nVidH / nVidW; 
		}
		nNewL = (nDstW - nNewW) / 2;
		nNewT = (nDstH - nNewH) / 2;
		JIF(m_pWC->SetVideoPosition(NULL, CRect(nNewL, nNewT, nNewL + nNewW, nNewT + nNewH)));
	}
	return S_OK;
}

void CPlayer::ClearGraph( void )
{
	m_pWC.Release();
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

	// ��ʼ��VMR Windowless
	JIF(pVMR.CoCreateInstance(CLSID_VideoMixingRenderer, NULL, CLSCTX_INPROC_SERVER));
	JIF(pVMR.QueryInterface(&pConfig));
	JIF(pConfig->SetRenderingMode(VMRMode_Windowless));
	JIF(pVMR.QueryInterface(&pWC));
	JIF(pWC->SetAspectRatioMode(VMR_ARMODE_NONE));
	JIF(pWC->SetVideoClippingWindow(m_hwndVid));
	JIF(m_pGraph->AddFilter(pVMR, _T("Video Mixing Renderer")));
	// ��ʼ��DSound
	JIF(pDSound.CoCreateInstance(CLSID_DSoundRender, NULL, CLSCTX_INPROC_SERVER));
	JIF(m_pGraph->AddFilter(pDSound, _T("Default DirectSound Renderer")));
	// ��Ⱦ
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
		UpdateVideoPosition(&rctWnd);		
	}
	return hr;	
}

HRESULT CPlayer::OpenMediaPrivate( CAutoPtr<OpenMediaData> pOMD )
{
	CheckPointer(pOMD, E_POINTER);

	HRESULT hr;

	Stop();
	if (m_pOMD != NULL)
	{
		m_pOMD.Free();
		m_nState = STATE_IDLE;
	}
	m_nState = STATE_OPENNING;
	if (OpenFileData* pOFD = dynamic_cast<OpenFileData*>(pOMD.m_p))
	{
		if (pOFD->nIndex < pOFD->gFiles.size())
			JIF(OpenFilePrivate(pOFD->gFiles[pOFD->nIndex]));
	}
	else
		return E_INVALIDARG;
	m_pOMD = pOMD;
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
			m_nState = STATE_STOPPED;
			OpenNext();
			Play();
			break;
		}
		m_pME->FreeEventParams(nEventCode, nParam1, nParam2);
	}
}











