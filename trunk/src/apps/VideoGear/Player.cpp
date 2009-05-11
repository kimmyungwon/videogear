#include "StdAfx.h"
#include "VideoGear.h"
#include "Player.h"
#include "FileUtil.h"
#include "FGManager.h"

#define WM_GRAPH_NOTIFY	(WM_APP + 0x1000)

CPlaylist::CPlaylist( void )
{

}

UINT CPlaylist::Add( LPCWSTR pszFile )
{
	PlayItem* pItem = new PlayItem;
	pItem->path = ExpandFileName(pszFile);
	CMyAutoLock lock(&m_lock);
	m_items.push_back(pItem);
	return m_items.size() - 1;
}

void CPlaylist::Remove( UINT nIndex )
{
	CMyAutoLock lock(&m_lock);
	if (nIndex < m_items.size())
		m_items.erase(m_items.begin() + nIndex);
}

UINT CPlaylist::GetCount( void )
{
	CMyAutoLock lock(&m_lock);
	return m_items.size();
}

PlayItem* CPlaylist::GetItem( UINT nIndex )
{
	CMyAutoLock lock(&m_lock);
	if (nIndex < m_items.size())
		return &m_items[nIndex];
	else
		return NULL;
}

void CPlaylist::Lock( void )
{
	m_lock.Enter();
}

void CPlaylist::Unlock( void )
{
	m_lock.Leave();
}
//////////////////////////////////////////////////////////////////////////

CPlayer::CPlayer( CWnd *pOwner )
: m_pOwner(pOwner), m_state(PS_STOPPED), m_nCurIndex(0)
{
	m_pPlaylist = new CPlaylist;
	m_pGB = new CFGManager;
}

CPlayer::~CPlayer(void)
{
	Stop();
	SAFE_DELETE(m_pPlaylist);
}

void CPlayer::NotifyOwnerMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if (g_appCfg.m_VideoRenderer != VR_EVR && m_pVW != NULL)
		m_pVW->NotifyOwnerMessage((OAHWND)m_pOwner->GetSafeHwnd(), uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_SIZE:
		AdjustWindowSize();
		AdjustVideoPosition();
		break;
	case WM_PAINT:
		if (g_appCfg.m_VideoRenderer == VR_EVR && m_pVDC != NULL)
			m_pVDC->RepaintVideo();
		break;
	case WM_GRAPH_NOTIFY:
		break;
	}
}

HRESULT CPlayer::AddFile( LPCWSTR pszFile )
{
	m_pPlaylist->Add(pszFile);
	return S_OK;
}

HRESULT CPlayer::Play( void )
{
	if (!IsWindow(m_pOwner->GetSafeHwnd()))
		return E_UNEXPECTED;
	if (m_state == PS_RUNNING)
		RIF(Stop());
	if (m_state == PS_STOPPED)
	{
		m_pPlaylist->Lock();
		if (m_nCurIndex < m_pPlaylist->GetCount())
			m_curItem = *m_pPlaylist->GetItem(m_nCurIndex);
		m_pPlaylist->Unlock();
		return DoPlay(m_curItem);
	}
	else if (m_state == PS_PAUSED)
	{
		return m_pMC->Pause();
	}
	return S_OK;
}

HRESULT CPlayer::Stop( void )
{
	if (m_state == PS_RUNNING || m_state == PS_PAUSED)
	{
		ASSERT(m_pMC != NULL);
		RIF(m_pMC->Stop());
		if (m_pVDC != NULL)
		{
			m_pVDC->SetVideoWindow(NULL);
			m_pVDC = NULL;
		}
		else if (m_pVW != NULL)
		{
			m_pVW->put_Visible(OAFALSE);
			m_pVW->put_Owner((OAHWND)NULL);
			m_pVW = NULL;
		}
		m_pBV = NULL;
		m_pBA = NULL;
		m_pMC = NULL;
		m_pME = NULL;
		RIF(m_pGB->ClearGraph());
	}
	m_state = PS_STOPPED;
	return S_OK;
}

HRESULT CPlayer::DoPlay( PlayItem& item )
{
	RIF(m_pGB->RenderFile(item.path, NULL));
	RIF(m_pGB.QueryInterface(&m_pMC));
	RIF(m_pGB.QueryInterface(&m_pME));
	m_pGB.QueryInterface(&m_pBV);
	if (g_appCfg.m_VideoRenderer == VR_EVR)
		m_pGB.QueryInterface(&m_pVDC);
	else
		m_pGB.QueryInterface(&m_pVW);
	m_pGB.QueryInterface(&m_pBA);
	
	RIF(m_pME->SetNotifyWindow((OAHWND)m_pOwner->GetSafeHwnd(), WM_GRAPH_NOTIFY, (LONG_PTR)this));
	AdjustWindowSize(true);
	AdjustVideoPosition();
	RIF(m_pMC->Run());

	m_state = PS_RUNNING;
	return S_OK;
}

void CPlayer::AdjustWindowSize( bool bInit )
{
	if (IsWindow(m_pOwner->GetSafeHwnd()))
	{
		if (g_appCfg.m_VideoRenderer == VR_EVR && m_pVDC != NULL)
		{
			if (bInit)
			{
				m_pVDC->SetVideoWindow(m_pOwner->GetSafeHwnd());
				m_pVDC->SetAspectRatioMode(MFVideoARMode_None);
			}
		}
		else if (g_appCfg.m_VideoRenderer != VR_EVR && m_pVW != NULL)
		{
			CRect rctWnd;

			m_pOwner->GetClientRect(&rctWnd);
			if (bInit)
			{
				m_pVW->put_WindowStyle(WS_CHILD|WS_VISIBLE);
				m_pVW->put_Owner((OAHWND)m_pOwner->GetSafeHwnd());
				m_pVW->put_Visible(OATRUE);
			}
			m_pVW->SetWindowPosition(0, 0, rctWnd.Width(), rctWnd.Height());
		}
	}
	
}

void CPlayer::AdjustVideoPosition( void )
{
	if (IsWindow(m_pOwner->GetSafeHwnd()))
	{
		CRect rctWnd;
		
		m_pOwner->GetClientRect(&rctWnd);		
		if (g_appCfg.m_VideoRenderer == VR_EVR && m_pVDC != NULL)
		{
			CSize VideoSize;
			long nDstW, nDstH;
			
			m_pVDC->GetNativeVideoSize(NULL, &VideoSize);
			if (VideoSize.cx / VideoSize.cy <= rctWnd.Width() / rctWnd.Height())
			{
				nDstH = rctWnd.Height();
				nDstW = nDstH * VideoSize.cx / VideoSize.cy;
			}
			else
			{
				nDstW = rctWnd.Width();
				nDstH = nDstW * VideoSize.cy / VideoSize.cx;
			}
			m_pVDC->SetVideoPosition(NULL, CRect(CPoint((rctWnd.Width() - nDstW) / 2, (rctWnd.Height() - nDstH) / 2), CSize(nDstW, nDstH)));
		}
		else if (g_appCfg.m_VideoRenderer != VR_EVR && m_pBV != NULL)
		{
			long nVidW, nVidH;
			long nDstW, nDstH;
			
			m_pBV->GetVideoSize(&nVidW, &nVidH);
			if (nVidW / nVidH <= rctWnd.Width() / rctWnd.Height())
			{
				nDstH = rctWnd.Height();
				nDstW = nDstH * nVidW / nVidH;
			}
			else
			{
				nDstW = rctWnd.Width();
				nDstH = nDstW * nVidH / nVidW;
			}
			m_pBV->SetDestinationPosition((rctWnd.Width() - nDstW) / 2, (rctWnd.Height() - nDstH) / 2, nDstW, nDstH);
		}
	}
}

