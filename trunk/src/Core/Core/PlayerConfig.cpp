#include "StdAfx.h"
#include "PlayerConfig.h"

CPlayerConfig gAppCfg;

//////////////////////////////////////////////////////////////////////////

CPlayerConfig::CPlayerConfig(void)
{
	m_ulVidOutMode = VR_DEFAULT;
}

CPlayerConfig::~CPlayerConfig(void)
{
}

/* IPlayerConfig */
STDMETHODIMP_(ULONG) CPlayerConfig::get_VideoOutputMode(void)
{
	CAutoLock cAutoLock(this);

	return m_ulVidOutMode;
}

STDMETHODIMP CPlayerConfig::set_VideoOutputMode(ULONG ulVal)
{
	CAutoLock cAutoLock(this);

	if (ulVal != VR_DEFAULT)
		return E_INVALIDARG;
	m_ulVidOutMode = ulVal;
	return S_OK;
}	