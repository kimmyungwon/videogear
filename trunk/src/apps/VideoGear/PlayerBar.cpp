// PlayerBar.cpp : implementation file
//

#include "stdafx.h"
#include "VideoGear.h"
#include "PlayerBar.h"

enum {
	BTN_PLAYPAUSE,
	BTN_STOP,
	CMN_PROGRESS,
	BTN_AUDIO,
	CMN_VOLUMN
};

#define PROGRESSCMN_THUMBLENGTH	18
#define VOLUMNCMN_WIDTH			100
#define VOLUMNCMN_THUMBLENGTH	18

// CPlayerBar

IMPLEMENT_DYNAMIC(CPlayerBar, CToolBar)

CPlayerBar::CPlayerBar()
{
	m_bCreated = false;
}

CPlayerBar::~CPlayerBar()
{
}

BOOL CPlayerBar::Create( CWnd* pParentWnd, UINT nID /*= AFX_IDW_CONTROLBAR_FIRST*/ )
{
	if (!CToolBar::Create(pParentWnd, WS_CHILD|WS_VISIBLE|CBRS_ALIGN_BOTTOM|CBRS_BORDER_BOTTOM|CBRS_BORDER_TOP|CBRS_BORDER_3D, nID))
		return FALSE;
	if (!LoadBitmap(IDB_PLAYBAR))
		return FALSE;
	if (!m_cmnPorgress.Create(WS_CHILD|WS_VISIBLE|WS_DISABLED|TBS_HORZ|TBS_BOTH|TBS_NOTICKS|TBS_FIXEDLENGTH|TBS_ENABLESELRANGE, 
		CRect(), this, AFX_IDW_PANE_FIRST))
		return FALSE;
	m_cmnPorgress.SetThumbLength(PROGRESSCMN_THUMBLENGTH);
	if (!m_cmnVolumn.Create(WS_CHILD|WS_VISIBLE|TBS_HORZ|TBS_TOP|TBS_NOTICKS|TBS_FIXEDLENGTH, CRect(), this, AFX_IDW_PANE_FIRST))
		return FALSE;
	m_cmnVolumn.SetThumbLength(VOLUMNCMN_THUMBLENGTH);
	m_cmnVolumn.SetRange(0, 255);
	m_cmnVolumn.SetPos(255);

	SetBorders(1, 0, 1, 0);
	SetSizes(CSize(23, 22), CSize(16, 16));
	SetButtons(NULL, 5);
	SetButtonInfo(BTN_PLAYPAUSE, ID_CTRL_PLAYPAUSE, TBBS_BUTTON, 0);
	SetButtonInfo(BTN_STOP, ID_CTRL_STOP, TBBS_BUTTON, 2);
	SetButtonInfo(CMN_PROGRESS, ID_SEPARATOR, TBBS_SEPARATOR, -1);
	SetButtonInfo(BTN_AUDIO, ID_CTRL_AUDIO, TBBS_BUTTON, 12);
	SetButtonInfo(CMN_VOLUMN, ID_SEPARATOR, TBBS_SEPARATOR, -1);
	m_bCreated = true;

	return TRUE;
}

void CPlayerBar::SwitchPlaypause( bool bDoPause )
{
	SetButtonInfo(BTN_PLAYPAUSE, ID_CTRL_PLAYPAUSE, TBBS_BUTTON, bDoPause ? 1 : 0);
}

void CPlayerBar::EnableStop( bool bEnable )
{
	if (bEnable)
		SetButtonStyle(BTN_STOP, TBBS_BUTTON);
	else
		SetButtonStyle(BTN_STOP, TBBS_DISABLED);
}

void CPlayerBar::EnableProgress( bool bEnable )
{
	m_cmnPorgress.EnableWindow(bEnable);
}

void CPlayerBar::UpdateProgress( int iPosition )
{
	m_cmnPorgress.SetPos(iPosition);
}

void CPlayerBar::UpdateDuration( int iDuration )
{
	m_cmnPorgress.SetRange(0, iDuration);
}

void CPlayerBar::UpdateAvailable( int iEarliest, int iLastest )
{
	m_cmnPorgress.SetSelection(iEarliest, iLastest);
}

void CPlayerBar::UpdateVolume( BYTE nVolume )
{
	m_cmnVolumn.SetPos(nVolume);
}

void CPlayerBar::OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler )
{
	CToolBar::OnUpdateCmdUI(pTarget, FALSE);
}

BEGIN_MESSAGE_MAP(CPlayerBar, CToolBar)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CPlayerBar message handlers

void CPlayerBar::OnSize(UINT nType, int cx, int cy)
{
	CToolBar::OnSize(nType, cx, cy);

	if (m_bCreated && cx > 0 && cy > 0)
	{
		CRect rctClient, rctMute, rctCmn;
		
		GetClientRect(rctClient);
		GetItemRect(BTN_AUDIO, rctMute);
		GetItemRect(CMN_PROGRESS, rctCmn);
		rctCmn.right = rctClient.right - rctMute.Width() - VOLUMNCMN_WIDTH;
		SetButtonInfo(CMN_PROGRESS, ID_SEPARATOR, TBBS_SEPARATOR, rctCmn.Width());
		GetItemRect(CMN_PROGRESS, rctCmn);
		m_cmnPorgress.SetWindowPos(NULL, rctCmn.left, rctCmn.top, rctCmn.Width(), rctCmn.Height(), SWP_NOZORDER);

		GetItemRect(CMN_VOLUMN, rctCmn);
		rctCmn.right = rctClient.right;
		SetButtonInfo(CMN_VOLUMN, ID_SEPARATOR, TBBS_SEPARATOR, rctCmn.Width());
		GetItemRect(CMN_VOLUMN, rctCmn);
		m_cmnVolumn.SetWindowPos(NULL, rctCmn.left, rctCmn.top, rctCmn.Width(), rctCmn.Height(), SWP_NOZORDER);
	}
}



