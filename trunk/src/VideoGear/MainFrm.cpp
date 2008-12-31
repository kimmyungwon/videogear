
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "VideoGear.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

enum {
	TIMER_GETPOSITION = 0x100
};

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_PLAYER_NOTIFY, &CMainFrame::OnPlayerNotify)
	ON_WM_TIMER()
	ON_COMMAND(ID_FILE_QOPEN, &CMainFrame::OnFileQOpen)
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}

	if (!m_wndCtrl.Create(this))
	{
		TRACE0("未能创建控制条\n");
		return -1;
	}

	if (FAILED(m_player.Initialize(GetSafeHwnd(), m_wndView.GetSafeHwnd())))
	{
		TRACE0("未能创建播放器\n");
		return -1;
	}

	m_wndView.SetPlayer(&m_player);
	m_wndCtrl.SetPlayer(&m_player);

	SetTimer(TIMER_GETPOSITION, 500, NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !__super::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	__super::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	__super::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// 否则，执行默认处理
	return __super::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

LRESULT CMainFrame::OnPlayerNotify( WPARAM wParam, LPARAM lParam )
{
	switch (wParam)
	{
	case PN_MEDIA_OPENED:
		m_wndCtrl.UpdateState();
		break;
	case PN_VIDEOWINDOW_NEED_RESIZE:
		{
			CRect rect(CPoint(0, 0), *(LPSIZE)lParam);
			AdjustWindowRectEx(&rect, GetStyle(), TRUE, GetExStyle());
			SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE|SWP_NOZORDER);
		}
		break;
	}
	return 0;
}

void CMainFrame::OnTimer( UINT_PTR nIDEvent )
{
	__super::OnTimer(nIDEvent);
	
	switch (nIDEvent)
	{
	case TIMER_GETPOSITION:
		if (m_player.GetState() == CPlayer::STATE_PLAYING)
		{
			m_wndCtrl.UpdateState();
		}
		break;
	}
}

void CMainFrame::OnFileQOpen( void )
{
	CFileDialog dlgOpen(TRUE, NULL, NULL, OFN_EXPLORER|OFN_FILEMUSTEXIST, NULL, this);
	if (dlgOpen.DoModal() == IDOK)
	{
		CAutoPtr<OpenFileData> pOFD(new OpenFileData);
		pOFD->strFile = dlgOpen.GetPathName();
		if (SUCCEEDED(m_player.OpenMedia(pOFD)))
			m_player.Play();
	}
}