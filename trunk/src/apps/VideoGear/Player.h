#pragma once

#include "Thread.h"
#include "IGraphBuilder2.h"

struct PlayItem
{
	CStringW path;
};

class CPlaylist
{
	friend class CPlayer;
public:	
	UINT Add(LPCWSTR pszFile);
	void Remove(UINT nIndex);
	UINT GetCount(void);
	PlayItem* GetItem(UINT nIndex);
	void Lock(void);
	void Unlock(void);
protected:	
	CPlaylist(void);
private:
	typedef boost::ptr_vector<PlayItem> ItemList;

	CMyCritSec m_lock;
	ItemList m_items;
};

enum PlayerState
{
	PS_STOPPED,
	PS_RUNNING,
	PS_PAUSED
};

class CPlayer
{
public:
	CPlayer(CWnd *pOwner);
	virtual ~CPlayer(void);
	PlayerState GetState(void)	{ return m_state; }
	void NotifyOwnerMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	/* 播放列表操作 */
	HRESULT AddFile(LPCWSTR pszFile);
	HRESULT Play(void);
	HRESULT Stop(void);
protected:
	HRESULT DoPlay(PlayItem& item);
	void AdjustWindowSize(bool bInit = false);
	void AdjustVideoPosition(void);
private:
	CWnd *m_pOwner;
	PlayerState m_state;
	CPlaylist *m_pPlaylist;
	UINT m_nCurIndex;
	PlayItem m_curItem;
	CComPtr<IGraphBuilder2> m_pGB;
	CComPtr<IMediaControl> m_pMC;
	CComPtr<IMediaEventEx> m_pME;
	CComPtr<IBasicVideo2> m_pBV;
	CComPtr<IVideoWindow> m_pVW;
	CComPtr<IMFVideoDisplayControl> m_pVDC;
	CComPtr<IBasicAudio> m_pBA;
};
