#pragma once


// CPlayerBar

class CPlayerBar : public CToolBar
{
	DECLARE_DYNAMIC(CPlayerBar)

public:
	CPlayerBar();
	virtual ~CPlayerBar();
	BOOL Create(CWnd* pParentWnd, UINT nID = AFX_IDW_CONTROLBAR_FIRST);
	void EnableProgress(BOOL bEnable);
	void UpdateProgress(int *piPos, int *piDuration);
private:
	bool m_bCreated;
	CSliderCtrl	m_cmnPorgress;
	CSliderCtrl m_cmnVolumn;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


