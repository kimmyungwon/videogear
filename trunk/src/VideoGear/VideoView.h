
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

// ʵ��
public:
	virtual ~CVideoView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

