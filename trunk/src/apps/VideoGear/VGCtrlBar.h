#pragma once

#include "Player.h"

// CVGCtrlBar

class CVGCtrlBar : public CToolBar
{
	DECLARE_DYNAMIC(CVGCtrlBar)
public:
	virtual BOOL Create(CWnd* pParentWnd);
	void SetPlayer(CPlayer* pPlayer);
	void UpdateState(void);
protected:
	void EnableButton(int nIndex, bool bEnabled);
	void SwitchPlayPause(bool bPlay);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler);
private:
	CSliderCtrl	m_wndProgBar;
	CSliderCtrl m_wndVolBar;
	CPlayer*	m_pPlayer;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


