// Player.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoGear.h"
#include "Player.h"
#include "FGManager.h"

DWORD WINAPI GraphEventProc( LPVOID lpParameter )
{
	ASSERT(lpParameter != NULL);

	CPlayer* pPlayer = (CPlayer*)lpParameter;
	pPlayer->HandleGraphEvent();
	return 0;
}

// CPlayer

CPlayer::CPlayer()
: m_hEventThread(NULL)
{
}

CPlayer::~CPlayer()
{
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

	m_hwndMsg = hwndMsg;
	m_hwndVid = hwndVid;
	
	m_pGraph = new CFGManager(&hr);
	if (FAILED(hr))
		return hr;

	if (FAILED(hr = m_pGraph.QueryInterface(&m_pMC)))
		return hr;

	if (FAILED(hr = m_pGraph.QueryInterface(&m_pME)))
		return hr;

	m_hEventThread = CreateThread(NULL, 0, &GraphEventProc, (LPVOID)this, 0, NULL);
	if (m_hEventThread == NULL)
		return HRESULT_FROM_WIN32(GetLastError());

	return S_OK;
}

HRESULT CPlayer::OpenMediaPrivate( OpenMediaData* pOMD )
{
	return E_NOTIMPL;
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

