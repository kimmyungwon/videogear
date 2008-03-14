#include "StdAfx.h"
#include "VGPlayer.h"

IVGPlayer* WINAPI CreatePlayer(void)
{
	IVGPlayerPtr pPlayer = new CVGPlayer;
	return pPlayer;
}

//////////////////////////////////////////////////////////////////////////

VG_IMPLEMENT_ADDREF(CVGPlayer)
VG_IMPLEMENT_RELEASE(CVGPlayer)
VG_IMPLEMENT_QI(CVGPlayer)

VG_IMPLEMENT_CONSTRUCTOR_BEGIN(CVGPlayer)
	m_pFilterManager = new CVGFilterManager;
VG_IMPLEMENT_CONSTRUCTOR_END(CVGPlayer)

VG_IMPLEMENT_DESTRUCTOR_BEGIN(CVGPlayer)
VG_IMPLEMENT_DESTRUCTOR_END(CVGPlayer)

HRESULT STDMETHODCALLTYPE CVGPlayer::Initialize( void )
{
	HRESULT hr;

	hr = m_pFilterManager->Initialize();
	if (FAILED(hr))
		return hr;

	return S_OK;
}