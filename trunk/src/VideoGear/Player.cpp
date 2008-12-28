// Player.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoGear.h"
#include "Player.h"
#include "FGManager.h"

#define JIF(ret)	{ if(FAILED(hr = (ret))) return hr; }

#define BeginEnumFilters(pFilterGraph, pEnumFilters, pBaseFilter) \
	{CComPtr<IEnumFilters> pEnumFilters; \
	if(pFilterGraph && SUCCEEDED(pFilterGraph->EnumFilters(&pEnumFilters))) \
	{ \
	for(CComPtr<IBaseFilter> pBaseFilter; S_OK == pEnumFilters->Next(1, &pBaseFilter, 0); pBaseFilter = NULL) \
		{ \

#define EndEnumFilters }}}

#define BeginEnumCachedFilters(pGraphConfig, pEnumFilters, pBaseFilter) \
	{CComPtr<IEnumFilters> pEnumFilters; \
	if(pGraphConfig && SUCCEEDED(pGraphConfig->EnumCacheFilter(&pEnumFilters))) \
	{ \
	for(CComPtr<IBaseFilter> pBaseFilter; S_OK == pEnumFilters->Next(1, &pBaseFilter, 0); pBaseFilter = NULL) \
		{ \

#define EndEnumCachedFilters }}}

#define BeginEnumPins(pBaseFilter, pEnumPins, pPin) \
	{CComPtr<IEnumPins> pEnumPins; \
	if(pBaseFilter && SUCCEEDED(pBaseFilter->EnumPins(&pEnumPins))) \
	{ \
	for(CComPtr<IPin> pPin; S_OK == pEnumPins->Next(1, &pPin, 0); pPin = NULL) \
		{ \

#define EndEnumPins }}}

#define BeginEnumMediaTypes(pPin, pEnumMediaTypes, pMediaType) \
	{CComPtr<IEnumMediaTypes> pEnumMediaTypes; \
	if(pPin && SUCCEEDED(pPin->EnumMediaTypes(&pEnumMediaTypes))) \
	{ \
	AM_MEDIA_TYPE* pMediaType = NULL; \
	for(; S_OK == pEnumMediaTypes->Next(1, &pMediaType, NULL); DeleteMediaType(pMediaType), pMediaType = NULL) \
		{ \

#define EndEnumMediaTypes(pMediaType) } if(pMediaType) DeleteMediaType(pMediaType); }}

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

HRESULT CPlayer::OpenMedia( CAutoPtr<OpenMediaData> pOMD )
{
	return OpenMediaPrivate(pOMD);
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
	case STATE_PLAYING:
	case STATE_PAUSE:
		JIF(m_pMC->Stop());
		TearDownGraph();
		m_nState = STATE_IDLE;
		break;
	}
	return S_OK;
}

void CPlayer::TearDownGraph( void )
{
	BeginEnumFilters(m_pGraph, pEnumFilters, pFilter)
		BeginEnumPins(pFilter, pEnumPins, pPin)
			if (m_pGraph->IsPinConnected(pPin) == S_OK)
				m_pGraph->Disconnect(pPin);
		EndEnumPins
		m_pGraph->RemoveFilter(pFilter.Detach());
	EndEnumFilters
}

HRESULT CPlayer::OpenMediaPrivate( CAutoPtr<OpenMediaData> pOMD )
{
	CheckPointer(pOMD, E_POINTER);

	HRESULT hr;

	if (m_nState == STATE_OPENNING || m_nState == STATE_PLAYING || m_nState == STATE_PAUSE)
		Stop();
	if (m_pOMD != NULL)
		m_pOMD.Free();
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
	CComPtr<IBaseFilter> pVMR;
	CComPtr<IVMRFilterConfig> pConfig;
	CComPtr<IVMRWindowlessControl> pWC;

	JIF(pVMR.CoCreateInstance(CLSID_VideoMixingRenderer, NULL, CLSCTX_INPROC_SERVER));
	JIF(pVMR.QueryInterface(&pConfig));
	JIF(pVMR.QueryInterface(&pWC));
	JIF(pConfig->SetRenderingMode(VMRMode_Windowless));
	JIF(pWC->SetVideoClippingWindow(m_hwndVid));
	m_pWC = pWC;
	m_nState = STATE_PAUSE;
	return S_OK;
}

void CPlayer::HandleGraphEvent( void )
{
	ASSERT(m_pME != NULL);

	long nEventCode;
	LONG_PTR nParam1, nParam2;

	m_pME->GetEvent(&nEventCode, &nParam1, &nParam2, INFINITE);
	/*switch (nEventCode)
	{

	}*/
	m_pME->FreeEventParams(nEventCode, nParam1, nParam2);
}




