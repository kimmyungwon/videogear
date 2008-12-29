
// ChildView.h : CChildView ��Ľӿ�
//


#pragma once
#include "Player.h"

// CChildView ����

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

// ���ɵ���Ϣӳ�亯��
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

