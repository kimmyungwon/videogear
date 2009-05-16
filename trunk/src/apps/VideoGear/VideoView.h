
// VideoView.h : CVideoView 类的接口
//


#pragma once
#include "FGManager.h"
#include "Playlist.h"
#include "PlayerBar.h"

class CVideoView : public CWnd
{
public:
	CPlayerBar	*m_pCtrlBar;
	
	CVideoView();
	virtual ~CVideoView();
	int GetState(void)	{ return m_pFGMgr->GetState(); }
protected:
	void SetItemIndex(UINT nIndex);
	void FGManagerStateChanged(int iNewState);
	void FGManagerMediaCompleted(void);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
private:
	CFGManager	*m_pFGMgr;
	CPlaylist	m_playlist;
	UINT		m_nItemIndex;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

