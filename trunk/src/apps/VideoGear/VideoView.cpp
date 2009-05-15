
// VideoView.cpp : CVideoView ���ʵ��
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
	m_pFGMgr = new CFGManager;
	__hook(&CFGManager::OnStateChanged, m_pFGMgr, &CVideoView::FGManagerStateChanged);
}

CVideoView::~CVideoView()
{
	__unhook(&CFGManager::OnStateChanged, m_pFGMgr, &CVideoView::FGManagerStateChanged);
	SAFE_DELETE(m_pFGMgr);
}

void CVideoView::FGManagerStateChanged( int iNewState )
{
	
}

BOOL CVideoView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CVideoView, CWnd)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

int CVideoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (FAILED(m_pFGMgr->Initialize(this)))
		return -1;

	return 0;
}

void CVideoView::OnDropFiles( HDROP hDropInfo )
{
	/*DWORD dwCount = DragQueryFileW(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for (DWORD i = 0; i < dwCount; i++)
	{
		WCHAR szFile[MAX_PATH];
		
		DragQueryFileW(hDropInfo, i, szFile, _countof(szFile));
		m_pFGMgr->RenderFile(szFile);
	}
	DragFinish(hDropInfo);
	m_pFGMgr->Run();*/
}

