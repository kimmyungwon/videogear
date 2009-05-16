// UIControl.cpp : implementation file
//

#include "stdafx.h"
#include "VideoGear.h"
#include "UIControl.h"

// CUIControl

IMPLEMENT_DYNAMIC(CUIControl, CControlBar)

CUIControl::CUIControl()
: m_pCanvas(NULL), m_pPainter(NULL)
{

}

CUIControl::~CUIControl()
{
	SAFE_DELETE(m_pPainter);
	SAFE_DELETE(m_pCanvas);
}

BOOL CUIControl::Create( CWnd* pParentWnd, UIAlign uiAlign, DWORD dwStyle, CRect &rect, UINT nID )
{	
	ASSERT_VALID(pParentWnd);

	dwStyle &= ~CBRS_ALL;
	dwStyle |= WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_SIZE_FIXED;
	switch (uiAlign)
	{
	case UIAL_LEFT:
		dwStyle |= CBRS_ALIGN_LEFT;
		break;
	case UIAL_TOP:
		dwStyle |= CBRS_ALIGN_TOP;
		break;
	case UIAL_RIGHT:
		dwStyle |= CBRS_ALIGN_RIGHT;
		break;
	case UIAL_BOTTOM:
		dwStyle |= CBRS_ALIGN_BOTTOM;
		break;
	default:
		dwStyle |= CBRS_FLOATING|CBRS_FLOAT_MULTI;
	}		
	m_dwStyle = (dwStyle & CBRS_ALL);
	
	CString strWndCls = AfxRegisterWndClass(0, NULL, (HBRUSH)(COLOR_BTNFACE+1));
	if (!CWnd::Create(strWndCls, NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;
	
	return TRUE;
}

void CUIControl::DoPaint( Graphics &canvas )
{
	Color clBtnFace;

	clBtnFace.SetFromCOLORREF(GetSysColor(COLOR_BTNFACE));
	canvas.Clear(clBtnFace);
}

CSize CUIControl::CalcFixedLayout( BOOL bStretch, BOOL bHorz )
{
	CRect rctWnd;
	
	GetWindowRect(rctWnd);
	if (m_dwStyle & CBRS_ALIGN_ANY)
	{
		return CSize((bStretch && bHorz) ? 32767 : rctWnd.Width()
					, (bStretch && !bHorz) ? 32767 : rctWnd.Height());
	}
	else
		return rctWnd.Size();
}

void CUIControl::OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler )
{

}

BEGIN_MESSAGE_MAP(CUIControl, CControlBar)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CUIControl message handlers

void CUIControl::OnSize( UINT nType, int cx, int cy )
{
	CControlBar::OnSize(nType, cx, cy);

	SAFE_DELETE(m_pPainter);
	SAFE_DELETE(m_pCanvas);
	m_pCanvas = new Bitmap(cx, cy, PixelFormat24bppRGB);
	m_pPainter = new Graphics(m_pCanvas);
}

BOOL CUIControl::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

void CUIControl::OnPaint()
{
	CPaintDC dc(this);
	ASSERT(m_pPainter != NULL);
	{
		CRect rctClient;

		Graphics screen(dc);
		DoPaint(*m_pPainter);
		screen.DrawImage(m_pCanvas, 0, 0);
	}
}
