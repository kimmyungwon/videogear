//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Player.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

int __fastcall EventThreadProc(void* pParam)
{
	TPlayer* pPlayer = (TPlayer*)pParam;
	long lEventCode;
	LONG_PTR lParam1, lParam2;

	while (pPlayer->m_pME->GetEvent(&lEventCode, &lParam1, &lParam2, INFINITE) == S_OK)
	{
		pPlayer->OnGraphEvent(lEventCode, lParam1, lParam2);
    }
	return 0;
}

TPlayer::TPlayer(void)
: m_iState(stUninit), m_pVidWnd(NULL), m_hEventThread(NULL)
{

}

TPlayer::~TPlayer(void)
{
	if (m_iState != stUninit)
    	Uninitialize();
}

HRESULT TPlayer::Initialize(TWinControl* pVidWnd)
{
	UINT uiThreadId;

	if (m_iState != stUninit)
		return E_UNEXPECTED;
	assert(pVidWnd != NULL);
	m_pVidWnd = pVidWnd;
	m_pVidWndOldWndProc = m_pVidWnd->WindowProc;
	m_pVidWnd->WindowProc = OnVidWndMsg;
	m_pGB = (IGraphBuilder2*)new TFGManager;
	m_pMC = m_pGB;
	m_pME = m_pGB;
	m_hEventThread = (HANDLE)BeginThread(NULL, 0, EventThreadProc, (void*)this,
		0, uiThreadId);
    m_iState = stStopped;
	return S_OK;
}

HRESULT TPlayer::Uninitialize(void)
{
	if (m_iState == stUninit)
		return S_FALSE;
	m_iState = stUninit;
	TerminateThread(m_hEventThread, 0);
	m_hEventThread = NULL;
	m_pME.Release();
	m_pGB.Release();
	m_pVidWnd->WindowProc = m_pVidWndOldWndProc;
	m_pVidWnd = NULL;
	return S_OK;
}

void TPlayer::AddToPlaylist(TElement& elem)
{
	TAutoLock lock(&m_playlist);

    m_playlist.push_back(elem);
}

void TPlayer::Play(void)
{
	if (m_iState != stStopped) return;

	TAutoLock lock(&m_playlist);

	if (m_playlist.size() == 0 || m_playlist.cur_pos >= m_playlist.size()) return;
	if (m_pGB->RenderFile(m_playlist[m_playlist.cur_pos].strPath.c_str(), NULL) != S_OK)
		return;
	m_pVW = m_pGB;
	if (m_pVW != NULL)
	{
    	m_pVW->put_WindowStyle(WS_CHILD);
		m_pVW->put_Owner((OAHWND)m_pVidWnd->Handle);
		m_pVW->put_Left(0);
        m_pVW->put_Top(0);
        m_pVW->put_Visible(OATRUE);
	}
	if (m_pMC->Run() != S_OK) return;
	m_iState = stPlaying;
}

void TPlayer::OnGraphEvent(long lCode, LONG_PTR lParam1, LONG_PTR lParam2)
{

}

void __fastcall TPlayer::OnVidWndMsg(TMessage &Message)
{
    assert(m_pVidWndOldWndProc != NULL);
	m_pVidWndOldWndProc(Message);
	if (m_pVW != NULL)
	{
		m_pVW->NotifyOwnerMessage((OAHWND)m_pVidWnd->Handle, Message.Msg,
			Message.WParam, Message.LParam);
    }
}
