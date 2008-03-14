#include "StdAfx.h"
#include "VGPlayer.h"

IVGPlayer* WINAPI CreatePlayer(void)
{
	CVGPlayer *pPlayer = new CVGPlayer;
	pPlayer->AddRef();
	return (IVGPlayer*)pPlayer;
}

//////////////////////////////////////////////////////////////////////////

VG_IMPLEMENT_ADDREF(CVGPlayer)
VG_IMPLEMENT_RELEASE(CVGPlayer)
VG_IMPLEMENT_QI(CVGPlayer)

CVGPlayer::CVGPlayer(void)
{
}

CVGPlayer::~CVGPlayer(void)
{
}
