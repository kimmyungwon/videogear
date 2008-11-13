#pragma once

#include "IPlayer.h"
#include "Utils.h"

class CPlayerConfig
	: public IPlayerConfig
	, public CCritSec
{
public:
	CPlayerConfig(void);
	~CPlayerConfig(void);

	/* IPlayerConfig */
	STDMETHODIMP_(ULONG) get_VideoOutputMode(void);
	STDMETHODIMP set_VideoOutputMode(ULONG ulVal);
	
	/* IUnknown */
	STDMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppvObj)
	{
		return	QI(IPlayerConfig)
				QI(IUnknown)
				E_NOINTERFACE;
	}

	STDMETHODIMP_(ULONG) AddRef()	{ return 1ul; }
	STDMETHODIMP_(ULONG) Release()	{ return 1ul; }
private:
	ULONG	m_ulVidOutMode;
};

extern CPlayerConfig gAppCfg;
