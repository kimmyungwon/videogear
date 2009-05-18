#pragma once


// CPlayerBar

class CPlayerBar : public CToolBar
{
	DECLARE_DYNAMIC(CPlayerBar)

public:
	CPlayerBar();
	virtual ~CPlayerBar();
	BOOL Create(CWnd* pParentWnd, UINT nID = AFX_IDW_CONTROLBAR_FIRST);
	void SwitchPlaypause(bool bPaused);
	void EnableProgress(BOOL bEnable);
	void UpdateProgress(int iPosition);
	void UpdateDuration(int iDuration);
	void UpdateAvailable(int iEarliest, int iLastest);
	void UpdateVolume(BYTE nVolume);
private:
	bool m_bCreated;
	CSliderCtrl	m_cmnPorgress;
	CSliderCtrl m_cmnVolumn;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateCtrlStop(CCmdUI *pCmdUI);
};


