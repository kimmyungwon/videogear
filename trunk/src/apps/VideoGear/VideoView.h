
// VideoView.h : CVideoView ��Ľӿ�
//


#pragma once
#include "FGManager.h"

// CVideoView ����

class CVideoView : public CWnd
{
// ����
public:
	CVideoView();

// ����
public:

// ����
public:

// ��д
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	CFGManager* m_pFGMgr;

// ʵ��
public:
	virtual ~CVideoView();

// ���ɵ���Ϣӳ�亯��
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

