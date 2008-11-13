#include "StdAfx.h"
#include "Player.h"

CPlayer::CPlayer(void)
: m_lRef(0), m_hVidWnd(NULL), m_playerState(psUninitialized), m_videoRenderer(vrDefault)
{
}

CPlayer::~CPlayer(void)
{
}

/* IPlayer */

STDMETHODIMP CPlayer::Initialize( __in HWND hVidWnd )
{
	return E_NOTIMPL;
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

