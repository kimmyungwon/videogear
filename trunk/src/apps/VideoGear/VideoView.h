
// VideoView.h : CVideoView 类的接口
//


#pragma once
#include "FGManager.h"

// CVideoView 窗口

class CVideoView : public CWnd
{
public:
	CVideoView();
	virtual ~CVideoView();
protected:
	void FGManagerStateChanged(int iNewState);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
private:
	CFGManager* m_pFGMgr;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

