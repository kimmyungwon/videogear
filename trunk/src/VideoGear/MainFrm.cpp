
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "VideoGear.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_FILE_QOPEN, &CMainFrame::OnFileQOpen)
	ON_COMMAND(ID_CTRL_PLAYPAUSE, &CMainFrame::OnCtrlPlaypause)
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
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("未能创建视图窗口\n");
		return -1;
	}

	if (!m_pnlCtrl.Create(this))
	{
		TRACE0("未能创建控制条\n");
		return -1;
	}

	HRESULT hr;
	
	if (FAILED(hr = m_player.Initialize(GetSafeHwnd())))
	{
		TRACE0("未能创建播放器\n");
		return -1;
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
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
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
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
	// 执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnFileQOpen()
{
	CFileDialog dlgOpen(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_FILEMUSTEXIST, L"所有文件|*.*",
		this, 0, TRUE);
	if (dlgOpen.DoModal() == IDOK)
	{
		CString strPath;
		POSITION pos = dlgOpen.GetStartPosition();
		while (pos != NULL)
		{
			strPath = dlgOpen.GetNextPathName(pos);
		}
	}
}

void CMainFrame::OnCtrlPlaypause()
{
	TRACE0("OnCtrlPlaypause\n");
}


