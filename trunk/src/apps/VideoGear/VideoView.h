
// VideoView.h : CVideoView 类的接口
//


#pragma once

// CVideoView 窗口

class CVideoView : public CWnd
{
// 构造
public:
	CVideoView();

// 属性
public:

// 操作
public:

// 重写
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	//CPlayer *m_pPlayer;

// 实现
public:
	virtual ~CVideoView();

// 生成的消息映射函数
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

