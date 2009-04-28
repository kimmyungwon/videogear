
// VideoView.cpp : CVideoView 类的实现
//

#include "stdafx.h"
#include "VideoGear.h"
#include "VideoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVideoView

CVideoView::CVideoView()
{
}

CVideoView::~CVideoView()
{
}


BEGIN_MESSAGE_MAP(CVideoView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CVideoView 消息处理程序

BOOL CVideoView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CVideoView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO: 在此处添加消息处理程序代码
	
	// 不要为绘制消息而调用 CWnd::OnPaint()
}

