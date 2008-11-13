#include "StdAfx.h"
#include "Player.h"

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

CPlayer::CPlayer(void)
: m_lRef(0), m_hVidWnd(NULL), m_playerState(psUninitialized), m_videoRenderer(vrDefault)
{
}

CPlayer::~CPlayer(void)
{
	if (get_State() != psUninitialized)
		Uninitialize();
}

/* IPlayer */

STDMETHODIMP CPlayer::Initialize( __in HWND hVidWnd )
{
	if (get_State() != psUninitialized) return E_UNEXPECTED;
	if (hVidWnd == NULL) return E_INVALIDARG;
	if (mapWndToPlayer.find(hVidWnd) != mapWndToPlayer.end()) return E_INVALIDARG;

	m_hVidWnd = hVidWnd;
	m_pfVWOrdProc = (WNDPROC)GetWindowLong(m_hVidWnd, GWL_WNDPROC);
	SetWindowLong(m_hVidWnd, GWL_WNDPROC, (LONG)&VidWndMsgProc);
	m_pGB = (IGraphBuilder2*)new CFGManager(NULL, (IPlayer*)this);
	m_pME = m_pGB;
	set_State(psStopped);
	return S_OK;
}

STDMETHODIMP_(PlayerState) CPlayer::get_State( void )
{
	return m_playerState;
}

STDMETHODIMP_(VideoRenderer) CPlayer::get_VideoRenderer( void )
{
	return m_videoRenderer;
}

STDMETHODIMP CPlayer::set_VideoRenderer( __in VideoRenderer vr )
{
	return E_NOTIMPL;
}

/* IUnknown */

STDMETHODIMP CPlayer::QueryInterface( REFIID riid, __deref_out void **ppvObj )
{
	return GetInterface((IUnknown*)this, ppvObj);
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

void CPlayer::Uninitialize( void )
{
	ASSERT(get_State() != psUninitialized);
	//TODO: 停止消息线程
	m_pME.Release();
	//TODO: 停止播放
	m_pGB.Release();
	set_State(psUninitialized);
}

LRESULT CPlayer::OnVidWndMsg( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return CallWindowProc(m_pfVWOrdProc, m_hVidWnd, uMsg, wParam, lParam);
}

void CPlayer::set_State( __in PlayerState state )
{
	m_playerState = state;
}