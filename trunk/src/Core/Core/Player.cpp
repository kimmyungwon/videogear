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
: m_lRef(0), m_hMsgWnd(NULL), m_hVidWnd(NULL), m_playerState(PS_UNINITIALIZED)
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
	// 截获视频窗口的消息
	m_hMsgWnd = hMsgWnd;
	m_hVidWnd = hVidWnd;
	mapWndToPlayer.insert(make_pair(m_hVidWnd, this));
	m_pfVWOrdProc = (WNDPROC)GetWindowLong(m_hVidWnd, GWL_WNDPROC);
	SetWindowLong(m_hVidWnd, GWL_WNDPROC, (LONG)&VidWndMsgProc);
	// 创建DShow组件
	m_pGB = (IGraphBuilder2*)new CFGManager(L"CFGManager", NULL);
	JIF(m_pGB.QueryInterface(&m_pME));
	JIF(m_pGB.QueryInterface(&m_pMC));
	// 开始FilterGraph事件处理线程
	m_hEventThread = CreateThread(NULL, 0, GraphEventProc, (LPVOID)this, 0, NULL);

	set_State(PS_IDLE);
	return S_OK;
}

STDMETHODIMP CPlayer::Uninitialize( void )
{
	HRESULT hr;
	
	if (get_State() == PS_UNINITIALIZED) return S_FALSE;
	JIF(Stop());
	// 停止FilterGraph消息线程
	TerminateThread(m_hEventThread, 0);
	WaitForSingleObject(m_hEventThread, INFINITE);
	// 释放DShow资源
	m_pME.Release();
	m_pMC.Release();
	m_pGB.Release();
	// 恢复视频窗口的原有消息处理
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
	CComPtr<IBaseFilter> pRenderer;
	CRect rctVid;
	
	if (get_State() == PS_UNINITIALIZED || m_pGB == NULL) return E_UNEXPECTED;
	JIF(Stop());
	if (get_State() != PS_IDLE) return E_UNEXPECTED;
	set_State(PS_RENDERING);
	JIF(m_pGB->RenderFile(lpcwstrUrl, NULL));
	set_State(PS_STOPPED);

	switch (gAppCfg.get_VideoOutputMode())
	{
	case VR_VMR9Windowed:
	case VR_VMR9Renderless:
		return E_NOTIMPL;
	default:
		JIF(FindVideoRenderer(m_pGB, &pRenderer));
		JIF(m_pGB.QueryInterface(&m_pVW));
		JIF(m_pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS));
		JIF(m_pVW->put_Owner((OAHWND)m_hVidWnd));
		GetClientRect(m_hVidWnd, &rctVid);
		JIF(m_pVW->SetWindowPosition(0, 0, rctVid.Width(), rctVid.Height()));
		JIF(m_pVW->put_Visible(OATRUE));
	}

	return S_OK;
}

STDMETHODIMP CPlayer::Play( void )
{
	HRESULT hr;

	if (m_pGB == NULL || m_pMC == NULL) return E_UNEXPECTED;
	switch (get_State())
	{
	case PS_UNINITIALIZED:	
	case PS_IDLE:
		return E_UNEXPECTED;
	case PS_PLAYING:	return S_FALSE;
	}
	JIF(m_pMC->Run());
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
	case PS_IDLE:	return S_FALSE;
	}
	JIF(m_pMC->Stop());
	if (m_pVW != NULL)
	{
		m_pVW->put_Visible(OAFALSE);
		m_pVW->put_Owner((OAHWND)NULL);
		m_pVW.Release();
	}
	JIF(m_pGB->Clear());
	set_State(PS_IDLE);
	return S_OK;
}	

STDMETHODIMP_(ULONG) CPlayer::get_State( void )
{
	CAutoLock cAutoLock(this);
	
	return m_playerState;
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
	TRACE("CPlayer::AddRef() = %d\n", lRef);
	ASSERT(lRef > 0);
	return max((ULONG)lRef, 1ul);
}

STDMETHODIMP_(ULONG) CPlayer::Release()
{
	LONG lRef = InterlockedDecrement(&m_lRef);
	TRACE("CPlayer::Release() = %d\n", lRef);
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
	CAutoLock cAutoLock(this);
	
	m_playerState = ulVal;
}
