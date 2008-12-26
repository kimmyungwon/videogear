#pragma once

#include "VGPlayer.h"

// CVGCtrlBar

class CVGCtrlBar : public CToolBar
{
	DECLARE_DYNAMIC(CVGCtrlBar)
public:
	virtual BOOL Create(CWnd* pParentWnd);
	void PlayerStateChanged(WORD wNewState);
protected:
	void EnableButton(int nIndex, bool bEnabled);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler);
private:
	CSliderCtrl	m_wndProgBar;
	CSliderCtrl m_wndVolBar;

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


