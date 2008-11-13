#pragma once

#include "IPlayer.h"

class CPlayer
	: public IPlayer
{
public:
	CPlayer(void);
	~CPlayer(void);

	/* IPlayer */
	STDMETHODIMP Initialize(__in HWND hVidWnd);
	STDMETHODIMP_(PlayerState) get_State(void);
	STDMETHODIMP_(VideoRenderer) get_VideoRenderer(void);
	STDMETHODIMP set_VideoRenderer(__in VideoRenderer vr);

	/* IUnknown */
	STDMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppvObj);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
private:
	LONG			m_lRef;
	HWND			m_hVidWnd;
	PlayerState		m_playerState;
	VideoRenderer	m_videoRenderer;
};
