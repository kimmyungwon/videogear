
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "VideoView.h"
#include "PlayerBar.h"

class CMainFrame : public CFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected: 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
private:
	CVideoView	m_wndView;
	CPlayerBar	m_barControl;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnFileOpen();
	afx_msg void OnCtrlPlaypause();
};


