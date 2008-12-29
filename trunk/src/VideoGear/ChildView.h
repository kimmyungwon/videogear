
// ChildView.h : CChildView 类的接口
//


#pragma once
#include "Player.h"

// CChildView 窗口

class CChildView : public CWnd
{
public:
	CChildView();
	virtual ~CChildView();
public:
	void SetPlayer(CPlayer* pPlayer);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
private:
	CPlayer*	m_pPlayer;

// 生成的消息映射函数
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

