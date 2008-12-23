#pragma once


// CVGCtrlBar

class CVGCtrlBar : public CToolBar
{
	DECLARE_DYNAMIC(CVGCtrlBar)

public:
	virtual BOOL Create(CWnd* pParentWnd);
private:
	CSliderCtrl	m_wndProgBar;
	CSliderCtrl m_wndVolBar;
protected:
	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPlaypause(void);
};


