
// VideoView.h : CVideoView ��Ľӿ�
//


#pragma once

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
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	//CPlayer *m_pPlayer;

// ʵ��
public:
	virtual ~CVideoView();

// ���ɵ���Ϣӳ�亯��
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

