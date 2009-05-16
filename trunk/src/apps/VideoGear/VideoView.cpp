
// VideoView.cpp : CVideoView 类的实现
//

#include "stdafx.h"
#include "VideoGear.h"
#include "VideoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INVALID_ITEM_INDEX	((UINT)0xFFFFFFFF)
#define TIMER_PROGRESS		0x100

// CVideoView

CVideoView::CVideoView()
: m_nItemIndex(INVALID_ITEM_INDEX)
{m_pFGMgr = new CFGManager;
	__hook(&CFGManager::OnStateChanged, m_pFGMgr, &CVideoView::FGManagerStateChanged);
	__hook(&CFGManager::OnMediaCompleted, m_pFGMgr, &CVideoView::FGManagerMediaCompleted);
}

CVideoView::~CVideoView()
{
	__unhook(&CFGManager::OnMediaCompleted, m_pFGMgr, &CVideoView::FGManagerMediaCompleted);
	__unhook(&CFGManager::OnStateChanged, m_pFGMgr, &CVideoView::FGManagerStateChanged);
	SAFE_DELETE(m_pFGMgr);
}

void CVideoView::SetItemIndex( UINT nIndex )
{
	if (m_nItemIndex != nIndex)
	{
		KillTimer(TIMER_PROGRESS);
		m_pCtrlBar->UpdateProgress(0);
		m_pCtrlBar->UpdateDuration(0);
		m_pCtrlBar->EnableProgress(FALSE);
		m_pFGMgr->Stop();
		m_nItemIndex = INVALID_ITEM_INDEX;
		if (const MediaItem *pItem = m_playlist.GetItem(nIndex))
		{
			HRESULT hr = m_pFGMgr->RenderFile(pItem->strFullPath);
			if (SUCCEEDED(hr))
			{
				int iDuration;

				m_pFGMgr->Run();
				m_nItemIndex = nIndex;
				m_pFGMgr->GetDuration(iDuration);
				m_pCtrlBar->EnableProgress(TRUE);
				m_pCtrlBar->UpdateDuration(iDuration);
				m_pCtrlBar->UpdateProgress(0);
				SetTimer(TIMER_PROGRESS, 200, NULL);
			}
		}
	}
}

void CVideoView::FGManagerStateChanged( int iNewState )
{
	
}

void CVideoView::FGManagerMediaCompleted( void )
{
	if (m_nItemIndex < m_playlist.GetCount())
		SetItemIndex(m_nItemIndex + 1);
}

BOOL CVideoView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CVideoView, CWnd)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	ON_WM_TIMER()
END_MESSAGE_MAP()

int CVideoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (FAILED(m_pFGMgr->Initialize(this)))
		return -1;

	DragAcceptFiles(TRUE);

	return 0;
}

void CVideoView::OnDropFiles( HDROP hDropInfo )
{
	m_playlist.Lock();
	DWORD dwCount = DragQueryFileW(hDropInfo, 0xFFFFFFFF, NULL, 0);
	UINT nStartIndex;
	for (DWORD i = 0; i < dwCount; i++)
	{
		WCHAR szFile[MAX_PATH];
		UINT nIndex;
		
		DragQueryFileW(hDropInfo, i, szFile, _countof(szFile));
		nIndex = m_playlist.AddFile(szFile);
		if (i == 0)
			nStartIndex = nIndex;
	}
	DragFinish(hDropInfo);
	m_playlist.Unlock();
	SetItemIndex(nStartIndex);
}

void CVideoView::OnTimer( UINT_PTR nIDEvent )
{
	switch (nIDEvent)
	{
	case TIMER_PROGRESS:
		{
			int iPos, iDuration, iEarliest, iLastest;

			if (m_pCtrlBar != NULL)
			{
				if (SUCCEEDED(m_pFGMgr->GetDuration(iDuration)))
					m_pCtrlBar->UpdateDuration(iDuration);
				if (SUCCEEDED(m_pFGMgr->GetAvailable(iEarliest, iLastest)))
					m_pCtrlBar->UpdateAvailable(iEarliest, iLastest);
				if (SUCCEEDED(m_pFGMgr->GetPosition(iPos)))
					m_pCtrlBar->UpdateProgress(iPos);
			}			
		}
		break;
	}
}

