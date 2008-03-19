#include "StdAfx.h"
#include "VGPlayer.h"

CVGPlayer::CVGPlayer(void)
{
	m_pFM = new CVGFilterManager;
}

CVGPlayer::~CVGPlayer(void)
{
}

HRESULT STDMETHODCALLTYPE CVGPlayer::QueryInterface( REFIID refiid, void **ppv )
{
	if (refiid == IID_IVGFilterManager)
	{
		*ppv = m_pFM;
		m_pFM->AddRef();
		return S_OK;
	}
	else
		return CVGUnknownImpl::QueryInterface(refiid, ppv);
}