#include "StdAfx.h"
#include "VGPlayer.h"
#include "FGManager.h"

CVGPlayer::CVGPlayer(void)
{
}

CVGPlayer::~CVGPlayer(void)
{
	Clear();
}

HRESULT CVGPlayer::Initialize( HWND hwnd )
{
	HRESULT hr;
	
	Clear();
	// ≥ı ºªØDShow
	m_pGB = new CFilterManager(&hr);
	if (FAILED(hr))
		return hr;
	return S_OK;
}

void CVGPlayer::Clear( void )
{
	//TODO: Õ£÷π≤•∑≈
	m_pGB = NULL;
}