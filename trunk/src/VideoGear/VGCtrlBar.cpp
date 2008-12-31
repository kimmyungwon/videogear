// VGCtrlBar.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoGear.h"
#include "VGCtrlBar.h"

enum {
	CBC_PLAYPAUSE,
	CBC_STOP,
	CBC_PROGRESSBAR,
	CBC_VOLUMEBAR
};

struct VGCtrlBarItem
{
	UINT nID;
	WORD nStyle;
	int iImage;
};

static VGCtrlBarItem g_items[] = {
	{ID_CTRL_PLAY, TBBS_BUTTON, 0},
	{ID_CTRL_STOP, TBBS_BUTTON, 2},
	{ID_SEPARATOR, TBBS_SEPARATOR, -1},
	{ID_SEPARATOR, TBBS_SEPARATOR, -1}
};

static const int cVolumeBarWidth = 100;

// CVGCtrlBar

IMPLEMENT_DYNAMIC(CVGCtrlBar, CToolBar)

BOOL CVGCtrlBar::Create( CWnd* pParentWnd )
{	
	if (!__super::CreateEx(pParentWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM))
		return FALSE;
	if (!m_wndProgBar.Create(WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_NOTICKS, CRect(0, 0, 0, 0), this, CBC_PROGRESSBAR))
		return FALSE;
	if (!m_wndVolBar.Create(WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_NOTICKS, CRect(0, 0, 0, 0), this, CBC_VOLUMEBAR))
		return FALSE;
	// 载入位图
	LoadBitmap(IDB_CONTROLBAR);
	// 添加按钮
	SetSizes(CSize(23, 22), CSize(16, 16));
	SetButtons(NULL, ARRAYSIZE(g_items));
	for (int i = 0; i < ARRAYSIZE(g_items); ++i)
		SetButtonInfo(i, g_items[i].nID, g_items[i].nStyle, g_items[i].iImage);
	return TRUE;
}

void CVGCtrlBar::SetPlayer( CPlayer* pPlayer )
{
	m_pPlayer = pPlayer;
	UpdateState();
}

void CVGCtrlBar::UpdateState( void )
{
	if (m_pPlayer == NULL)
		return;
	switch (m_pPlayer->GetState())
	{
	case CPlayer::STATE_IDLE:
		SwitchPlayPause(true);
		EnableButton(CBC_PLAYPAUSE, false);
		EnableButton(CBC_STOP, false);
		break;
	case CPlayer::STATE_STOPPED:
		SwitchPlayPause(true);
		EnableButton(CBC_PLAYPAUSE, true);
		EnableButton(CBC_STOP, false);
		break;
	case CPlayer::STATE_PLAYING:
		SwitchPlayPause(false);
		EnableButton(CBC_PLAYPAUSE, true);
		EnableButton(CBC_STOP, true);
		m_wndProgBar.SetRangeMax(m_pPlayer->GetDuration(), FALSE);
		m_wndProgBar.SetPos(m_pPlayer->GetCurrentPosisiton());
		m_wndVolBar.SetRangeMax(MAX_VOLUME, FALSE);
		m_wndVolBar.SetPos(m_pPlayer->GetVolume());
		break;
	case CPlayer::STATE_PAUSE:
		SwitchPlayPause(true);
		EnableButton(CBC_PLAYPAUSE, true);
		EnableButton(CBC_STOP, true);
		break;
	}
}

void CVGCtrlBar::EnableButton( int nIndex, bool bEnabled )
{
	UINT nStyle = GetButtonStyle(nIndex);
	if (bEnabled)
		SetButtonStyle(nIndex, nStyle & ~TBBS_DISABLED);
	else
		SetButtonStyle(nIndex, nStyle | TBBS_DISABLED);
}

void CVGCtrlBar::SwitchPlayPause( bool bPlay )
{
	UINT nStyle = GetButtonStyle(CBC_PLAYPAUSE);
	if (bPlay)
		SetButtonInfo(CBC_PLAYPAUSE, g_items[CBC_PLAYPAUSE].nID, HIWORD(nStyle) | g_items[CBC_PLAYPAUSE].nStyle, g_items[CBC_PLAYPAUSE].iImage);
	else
		SetButtonInfo(CBC_PLAYPAUSE, ID_CTRL_PAUSE, HIWORD(nStyle) | g_items[CBC_PLAYPAUSE].nStyle, g_items[CBC_PLAYPAUSE].iImage + 1);
}

void CVGCtrlBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
	__super::OnUpdateCmdUI(pTarget, FALSE);
}

BEGIN_MESSAGE_MAP(CVGCtrlBar, CToolBar)
	ON_WM_SIZE()	
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

void CVGCtrlBar::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	CRect rctProg, rctVol;

	if (cx == 0 || cy == 0)
		return;
	// 调整进度条的大小
	GetItemRect(CBC_PROGRESSBAR, &rctProg);
	SetButtonInfo(CBC_PROGRESSBAR, ID_SEPARATOR, TBBS_SEPARATOR, cx - rctProg.left - cVolumeBarWidth);
	GetItemRect(CBC_PROGRESSBAR, &rctProg);
	m_wndProgBar.SetWindowPos(NULL, rctProg.left, rctProg.top, rctProg.Width(), rctProg.Height(), SWP_NOZORDER);
	// 调整音量条的大小
	GetItemRect(CBC_VOLUMEBAR, &rctVol);
	SetButtonInfo(CBC_VOLUMEBAR, ID_SEPARATOR, TBBS_SEPARATOR, cx - rctVol.left);
	GetItemRect(CBC_VOLUMEBAR, &rctVol);
	m_wndVolBar.SetWindowPos(NULL, rctVol.left, rctVol.top, rctVol.Width(), rctVol.Height(), SWP_NOZORDER);
}

void CVGCtrlBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* pBar = reinterpret_cast<CSliderCtrl*>(pScrollBar);
	ENSURE(pBar);

	switch (pBar->GetDlgCtrlID())
	{
	case CBC_PROGRESSBAR:
		break;
	case CBC_VOLUMEBAR:
		if (m_pPlayer != NULL && m_pPlayer->IsMediaPlaying())
			m_pPlayer->SetVolume(pBar->GetPos());
		break;
	default:
		__super::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}


