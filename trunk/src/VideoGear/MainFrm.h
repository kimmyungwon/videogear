
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "ChildView.h"
#include "VGCtrlBar.h"
#include "VGPlayer.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CVGCtrlBar	m_pnlCtrl;
	CChildView	m_wndView;

private:
	CVGPlayer	m_player;

// 生成的消息映射函数
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg LRESULT OnPlayerMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFileQOpen();
};


