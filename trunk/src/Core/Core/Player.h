#pragma once

#include "IPlayer.h"

class CPlayer
	: public IPlayer
{
public:
	CPlayer(void);
	~CPlayer(void);

	/* IUnknown */
	STDMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppvObj);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
private:
	LONG	m_lRef;
};
