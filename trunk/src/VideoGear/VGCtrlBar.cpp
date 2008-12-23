// VGCtrlBar.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoGear.h"
#include "VGCtrlBar.h"

static int g_controls[] = {
	CBC_PLAYPAUSE,
	CBC_STOP,
	CBC_SLIDER
};

// CVGCtrlBar

IMPLEMENT_DYNAMIC(CVGCtrlBar, CDialogBar)

BOOL CVGCtrlBar::Create( CWnd* pParentWnd )
{	
	return CDialogBar::Create(pParentWnd, IDD_CONTROLBAR, CBRS_BOTTOM|CBRS_TOOLTIPS, IDD_CONTROLBAR);
}

void CVGCtrlBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
	CDialogBar::OnUpdateCmdUI(pTarget, FALSE);
}

BOOL CVGCtrlBar::PreCreateWindow(CREATESTRUCT& cs)
{
	return CDialogBar::PreCreateWindow(cs);
}

BEGIN_MESSAGE_MAP(CVGCtrlBar, CDialogBar)
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CVGCtrlBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	CRect rctClient, rctBorders;
	int nBtnX = 0, nBtnW, nBtnH;
	
	GetClientRect(&rctClient);
	rctBorders = GetBorders();
	nBtnW = nBtnH = rctClient.Height() - rctBorders.top - rctBorders.bottom;
	for (int i = 0; i < ARRAYSIZE(g_controls); ++i)
	{
		CWnd* pCtrl = GetDlgItem(g_controls[i]);
		if (pCtrl != NULL)
			pCtrl->SetWindowPos(NULL, nBtnX, 0, nBtnW, nBtnH, SWP_NOZORDER);
		nBtnX += nBtnW;
	}
}
