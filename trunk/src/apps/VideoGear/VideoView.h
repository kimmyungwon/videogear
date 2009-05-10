
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

// 实现
public:
	virtual ~CVideoView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

