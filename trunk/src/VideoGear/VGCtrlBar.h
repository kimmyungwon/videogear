#pragma once


// CVGCtrlBar

class CVGCtrlBar : public CDialogBar
{
	DECLARE_DYNAMIC(CVGCtrlBar)

public:
	virtual BOOL Create(CWnd* pParentWnd);

protected:
	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


