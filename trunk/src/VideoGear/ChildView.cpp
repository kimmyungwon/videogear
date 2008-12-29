
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "VideoGear.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
: m_pPlayer(NULL)
{
}

CChildView::~CChildView()
{
}

void CChildView::SetPlayer( CPlayer* pPlayer )
{
	m_pPlayer = pPlayer;	
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	if (m_pPlayer != NULL && m_pPlayer->IsMediaLoaded())
		m_pPlayer->RepaintVideo(&dc);
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	if (m_pPlayer != NULL && m_pPlayer->IsMediaLoaded())
	{
	}
	else
	{
		CRect rect;
		
		GetClientRect(&rect);
		pDC->FillSolidRect(&rect, 0);
	}
	return TRUE;
}

void CChildView::OnSize( UINT nType, int cx, int cy )
{
	__super::OnSize(nType, cx, cy);

	if (m_pPlayer != NULL)
		m_pPlayer->UpdateVideoPosition(CRect(0, 0, cx, cy));
}
