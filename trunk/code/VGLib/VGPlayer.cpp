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
	if (InlineIsEqualGUID(refiid, IID_IVGFilterManager))
	{
		*ppv = m_pFM;
		static_cast<IVGFilterManager*>(*ppv)->AddRef();
		return S_OK;
	}
	else
		return CVGUnknownImpl::QueryInterface(refiid, ppv);
}

HRESULT STDMETHODCALLTYPE CVGPlayer::Initialize( void )
{
	return m_pFM->Initialize();
}