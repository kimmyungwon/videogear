#pragma once

#include "IPlayer.h"
#include "FGManager.h"

class CPlayer
	: public IPlayer
{
	friend LRESULT CALLBACK VidWndMsgProc(HWND, UINT, WPARAM, LPARAM);
public:
	CPlayer(void);
	~CPlayer(void);

	/* IPlayer */
	STDMETHODIMP Initialize(__in HWND hVidWnd);
	STDMETHODIMP_(PlayerState) get_State(void);
	STDMETHODIMP_(VideoRenderer) get_VideoRenderer(void);
	STDMETHODIMP set_VideoRenderer(__in VideoRenderer vr);

	/* IUnknown */
	STDMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppvObj);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
protected:
	void Uninitialize(void);
	LRESULT OnVidWndMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void set_State(__in PlayerState state);
private:
	LONG			m_lRef;
	HWND			m_hVidWnd;	// 视频窗口
	WNDPROC			m_pfVWOrdProc;	// 视频窗口的原有消息处理函数
	PlayerState		m_playerState;	// 播放器状态
	VideoRenderer	m_videoRenderer;	// 渲染方式
	CComPtr<IGraphBuilder2>	m_pGB;
	CComQIPtr<IMediaEvent>	m_pME;
};
