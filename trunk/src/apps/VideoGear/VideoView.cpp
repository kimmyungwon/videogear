
// VideoView.cpp : CVideoView 类的实现
//

#include "stdafx.h"
#include "VideoGear.h"
#include "VideoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVideoView

CVideoView::CVideoView()
{
	//m_pPlayer = new CPlayer(this);
}

CVideoView::~CVideoView()
{
	//SAFE_DELETE(m_pPlayer);
}

BEGIN_MESSAGE_MAP(CVideoView, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CVideoView 消息处理程序

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

LRESULT CVideoView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lr = CWnd::WindowProc(message, wParam, lParam);
	//m_pPlayer->NotifyOwnerMessage(message, wParam, lParam);
	return lr;
}

BOOL CVideoView::OnEraseBkgnd( CDC* pDC )
{
	//if (m_pPlayer->GetState() == PS_STOPPED)
	{
		return CWnd::OnEraseBkgnd(pDC);
	}
	/*else
		return TRUE;*/
}

void CVideoView::OnDropFiles( HDROP hDropInfo )
{
	/*DWORD dwCount = DragQueryFileW(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for (DWORD i = 0; i < dwCount; i++)
	{
		WCHAR szFile[MAX_PATH];
		
		DragQueryFileW(hDropInfo, i, szFile, _countof(szFile));
		m_pPlayer->AddFile(szFile);
	}
	DragFinish(hDropInfo);
	m_pPlayer->Play();*/
}
