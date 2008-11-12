//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Player.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TPlayer::TPlayer(void)
: m_iState(stUninit), m_hVidWnd(NULL)
{

}

TPlayer::~TPlayer(void)
{
	if (m_iState != stUninit)
    	Uninitialize();
}

HRESULT TPlayer::Initialize(HWND hVidWnd)
{
	if (m_iState != stUninit)
    	return E_UNEXPECTED;
	m_hVidWnd = hVidWnd;
	m_pGB = (IGraphBuilder2*)new TFGManager;
	m_pME = m_pGB;
	return S_OK;
}

HRESULT TPlayer::Uninitialize(void)
{
	if (m_iState == stUninit)
    	return S_FALSE;
	m_iState = stUninit;
	m_pME.Release();
	m_pGB.Release();
	m_hVidWnd = NULL;
	return S_OK;
}
