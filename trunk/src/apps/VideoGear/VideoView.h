
// VideoView.h : CVideoView ��Ľӿ�
//


#pragma once
#include "FGManager.h"

// CVideoView ����

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

