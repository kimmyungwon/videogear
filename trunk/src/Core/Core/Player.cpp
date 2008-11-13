#include "StdAfx.h"
#include "Player.h"
#include "Utils.h"

using namespace std;

static map<HWND, CPlayer*> mapWndToPlayer;

LRESULT CALLBACK VidWndMsgProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CPlayer *pPlayer = NULL;
	
	map<HWND, CPlayer*>::const_iterator itPlayer = mapWndToPlayer.find(hwnd);
	if (itPlayer == mapWndToPlayer.end() || itPlayer->second == NULL)
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	pPlayer = itPlayer->second;
	return pPlayer->OnVidWndMsg(uMsg, wParam, lParam);
}

DWORD WINAPI GraphEventProc( LPVOID lpParam )
{
	CPlayer *pPlayer = (CPlayer*)lpParam;
	long lEventCode;
	LONG_PTR lParam1, lParam2;
	
	ASSERT(pPlayer != NULL);
	while (pPlayer->m_pME->GetEvent(&lEventCode, &lParam1, &lParam2, INFINITE) == S_OK)
	{
		pPlayer->OnGraphEvent(lEventCode, lParam1, lParam2);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

CPlayer::CPlayer(void)
: m_lRef(0), m_hMsgWnd(NULL), m_hVidWnd(NULL), m_playerState(PS_UNINITIALIZED), m_videoRenderer(VR_DEFAULT)
{
}

CPlayer::~CPlayer(void)
{
	if (get_State() != PS_UNINITIALIZED)
		Uninitialize();
}

void CPlayer::SendPlayerMessage( UINT uCode, void *pParam, bool bWait /*= false*/ )
{
	if (m_hMsgWnd != NULL)
		if (bWait) ::SendMessage(m_hMsgWnd, WM_VGPLAYER, (WPARAM)uCode, (LPARAM)pParam);
		else PostMessage(m_hMsgWnd, WM_VGPLAYER, (WPARAM)uCode, (LPARAM)pParam);
}

/* IPlayer */

STDMETHODIMP CPlayer::Initialize( __in HWND hVidWnd, __in_opt HWND hMsgWnd )
{
	HRESULT hr;
	
	if (get_State() != PS_UNINITIALIZED) return E_UNEXPECTED;
	if (hVidWnd == NULL) return E_INVALIDARG;
	if (mapWndToPlayer.find(hVidWnd) != mapWndToPlayer.end()) return E_INVALIDARG;
	// �ػ���Ƶ���ڵ���Ϣ
	m_hMsgWnd = hMsgWnd;
	m_hVidWnd = hVidWnd;
	mapWndToPlayer.insert(make_pair(m_hVidWnd, this));
	m_pfVWOrdProc = (WNDPROC)GetWindowLong(m_hVidWnd, GWL_WNDPROC);
	SetWindowLong(m_hVidWnd, GWL_WNDPROC, (LONG)&VidWndMsgProc);
	// ����DShow���
	m_pGB = (IGraphBuilder2*)new CFGManager(NULL, (IPlayer*)this);
	FAILED_RET(m_pGB.QueryInterface(&m_pME));
	FAILED_RET(m_pGB.QueryInterface(&m_pMC));
	// ��ʼFilterGraph�¼������߳�
	m_hEventThread = CreateThread(NULL, 0, GraphEventProc, (LPVOID)this, 0, NULL);

	set_State(PS_STOPPED);
	return S_OK;
}

STDMETHODIMP CPlayer::Uninitialize( void )
{
	HRESULT hr;
	
	if (get_State() == PS_UNINITIALIZED) return S_FALSE;
	FAILED_RET(Stop());
	// ֹͣFilterGraph��Ϣ�߳�
	TerminateThread(m_hEventThread, 0);
	WaitForSingleObject(m_hEventThread, INFINITE);
	// �ͷ�DShow��Դ
	m_pME.Release();
	m_pGB.Release();
	// �ָ���Ƶ���ڵ�ԭ����Ϣ����
	SetWindowLong(m_hVidWnd, GWL_WNDPROC, (LONG)m_pfVWOrdProc);
	map<HWND, CPlayer*>::const_iterator itPlayer = mapWndToPlayer.find(m_hVidWnd);
	if (itPlayer != mapWndToPlayer.end())
		mapWndToPlayer.erase(itPlayer);

	set_State(PS_UNINITIALIZED);
	return S_OK;
}

STDMETHODIMP CPlayer::OpenUrl( __in LPCWSTR lpcwstrUrl )
{
	HRESULT hr;
	
	if (m_pGB == NULL) return E_UNEXPECTED;
	switch (get_State())
	{
	case PS_UNINITIALIZED:	return E_UNEXPECTED;
	case PS_PLAYING:
	case PS_PAUSE:
		FAILED_RET(Stop());
		break;
	}
	FAILED_RET(m_pGB->RenderFile(lpcwstrUrl, NULL));
	set_State(PS_PAUSE);
	return S_OK;
}

STDMETHODIMP CPlayer::Play( void )
{
	HRESULT hr;

	if (m_pGB == NULL || m_pMC == NULL) return E_UNEXPECTED;
	switch (get_State())
	{
	case PS_UNINITIALIZED:	return E_UNEXPECTED;
	case PS_PLAYING:	return S_FALSE;
	}
	FAILED_RET(m_pMC->Run());
	set_State(PS_PLAYING);
	return S_OK;
}

STDMETHODIMP CPlayer::Pause( void )
{
	return E_NOTIMPL;
}

STDMETHODIMP CPlayer::Stop( void )
{
	HRESULT hr;
	
	if (m_pGB == NULL || m_pMC == NULL) return E_UNEXPECTED;
	switch (get_State())
	{
	case PS_UNINITIALIZED:	return E_UNEXPECTED;
	case PS_STOPPED:	return S_FALSE;
	}
	FAILED_RET(m_pMC->Stop());
	FAILED_RET(m_pGB->Clear());
	set_State(PS_STOPPED);
	return S_OK;
}	

STDMETHODIMP_(ULONG) CPlayer::get_State( void )
{
	return m_playerState;
}

STDMETHODIMP_(ULONG) CPlayer::get_VideoRenderer( void )
{
	return m_videoRenderer;
}

STDMETHODIMP CPlayer::set_VideoRenderer( __in ULONG ulVal )
{
	return E_NOTIMPL;
}

/* IUnknown */

STDMETHODIMP CPlayer::QueryInterface( REFIID riid, __deref_out void **ppvObj )
{
	return	QI(IPlayer)
			QI(IUnknown)
			E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CPlayer::AddRef()
{
	LONG lRef = InterlockedIncrement(&m_lRef);
	ASSERT(lRef > 0);
	return max((ULONG)lRef, 1ul);
}

STDMETHODIMP_(ULONG) CPlayer::Release()
{
	LONG lRef = InterlockedDecrement(&m_lRef);
	ASSERT(lRef >= 0);
	if (lRef == 0)
	{
		delete this;
		return 0ul;
	}
	else
		return max((ULONG)lRef, 1ul);
}

void CPlayer::OnGraphEvent( long lCode, LONG_PTR lParam1, LONG_PTR lParam2 )
{
	CAutoLock cAutoLock(this);
}

LRESULT CPlayer::OnVidWndMsg( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return CallWindowProc(m_pfVWOrdProc, m_hVidWnd, uMsg, wParam, lParam);
}

void CPlayer::set_State( __in ULONG ulVal )
{
	m_playerState = ulVal;
}
