//---------------------------------------------------------------------------

#ifndef PlayerH
#define PlayerH

#include <vector>
#include "ThreadUtils.h"
#include "FGManager.h"

using namespace std;

struct TElement
{
    String  strPath;
};

class TPlayList : public vector<TElement>, public TCritSec
{
public:
	TPlayList(void): cur_pos(0)	{}

	size_t   cur_pos;
};

class TPlayer
{
	friend int __fastcall EventThreadProc(void*);
public:
	enum { stUninit, stStopped, stPlaying };

	TPlayer(void);
	virtual ~TPlayer(void);
	// 初始化播放器
	HRESULT Initialize(HWND hVidWnd);
	// 释放播放器
	HRESULT Uninitialize(void);
	// 添加到播放列表
	void AddToPlaylist(TElement& elem);
	// 开始播放
    void Play(void);
protected:
	// FilterGraph消息响应
    void OnGraphEvent(long lCode, LONG_PTR lParam1, LONG_PTR lParam2);
private:
    int m_iState;
	HWND m_hVidWnd;
    HANDLE m_hEventThread;
	CComPtr<IGraphBuilder2>	m_pGB;
	CComQIPtr<IMediaControl> m_pMC;
	CComQIPtr<IMediaEventEx> m_pME;
	TPlayList m_playlist;
};

//---------------------------------------------------------------------------
#endif
