#include "StdAfx.h"
#include "Player.h"

CPlayer::CPlayer(void)
{
}

CPlayer::~CPlayer(void)
{
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