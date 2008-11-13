#pragma once

#include "IPlayer.h"
#include "PlayerConfig.h"
#include "FGManager.h"

class CPlayer
	: public IPlayer
	, public CCritSec
{
	friend LRESULT CALLBACK VidWndMsgProc(HWND, UINT, WPARAM, LPARAM);
	friend DWORD WINAPI GraphEventProc(LPVOID);
public:
	CPlayer(void);
	~CPlayer(void);

	/* IPlayer */
	STDMETHODIMP Initialize(__in HWND hVidWnd, __in_opt HWND hMsgWnd);
	STDMETHODIMP Uninitialize(void);
	STDMETHODIMP OpenUrl(__in LPCWSTR lpcwstrUrl);
	STDMETHODIMP Play(void);
	STDMETHODIMP Pause(void);
	STDMETHODIMP Stop(void);
	STDMETHODIMP_(ULONG) get_State(void);
	STDMETHODIMP_(ULONG) get_VideoRenderer(void);
	STDMETHODIMP set_VideoRenderer(__in ULONG ulVal);

	/* IUnknown */
	STDMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppvObj);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
protected:
	void SendPlayerMessage(UINT uCode, void *pParam, bool bWait = false);
	void OnGraphEvent(long lCode, LONG_PTR lParam1, LONG_PTR lParam2);
	LRESULT OnVidWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void set_State(__in ULONG ulVal);
private:
	LONG		m_lRef;
	HWND		m_hMsgWnd;		// 消息窗口，接收播放器消息
	HWND		m_hVidWnd;		// 视频窗口
	WNDPROC		m_pfVWOrdProc;	// 视频窗口的原有消息处理函数
	ULONG		m_playerState;	// 播放器状态
	HANDLE		m_hEventThread;	// FilterGraph消息处理线程
	CComPtr<IGraphBuilder2>		m_pGB;
	CComQIPtr<IMediaControl>	m_pMC;
	CComQIPtr<IMediaEvent>		m_pME;
	CComQIPtr<IVideoWindow>		m_pVW;
};
