#pragma once

#include "AppConfig.h"
#include "Thread.h"

class CFGManager
{
	friend LRESULT CALLBACK VidWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	enum {
		STATE_UNKNOWN = -1,
		STATE_IDLE,
		STATE_STOPPED,
		STATE_RUNNING
	};

	CFGManager(void);
	virtual ~CFGManager(void);
	// 初始化
	HRESULT Initialize(CWnd *pVidWnd);
	// 渲染指定的文件
	HRESULT RenderFile(LPCWSTR pszFile);
	// 获取媒体总长度
	HRESULT GetDuration(LONGLONG &nDuration);
	// 获取可播放的长度
	HRESULT GetAvailable(LONGLONG &nEarliest, LONGLONG &nLastest);
	// 获取当前位置
	HRESULT GetPosition(LONGLONG &nPosition);
	// 设置当前位置
	HRESULT SetPosition(LONGLONG nPosition);
	// 开始播放
	HRESULT Run(void);
	// 停止播放
	HRESULT Stop(void);
protected:
	// 按指定文件查找源滤镜并载入
	HRESULT AddSourceFilter(LPCWSTR pszFile, IBaseFilter **ppFilter);
	// 分离指定的源滤镜
	HRESULT SplitSource(IBaseFilter *pSource, IBaseFilter **ppFilter);
	// 渲染指定的滤镜
	HRESULT RenderFilter(IBaseFilter *pFilter, bool bReportError = false);
	// 渲染指定的输出插针
	HRESULT Render(IPin *pPinOut);
	// 直接连接输出插针到滤镜
	HRESULT ConnectDirect(IPin *pPinOut, IBaseFilter *pFilterIn, const AM_MEDIA_TYPE *pmt);
	// 直接连接两个插针
	HRESULT ConnectDirect(IPin *pPinOut, IPin *pPinIn, const AM_MEDIA_TYPE *pmt);
	// 拆除下游滤镜链
	HRESULT TearDownStream(IUnknown *pUnk);
	// 检查滤镜是否正在使用
	HRESULT CheckInUse(IBaseFilter *pFilter);
	// 清空图表
	HRESULT ClearGraph(void);
	// 调整视频位置和视频窗口大小
	HRESULT AdjustVideoPosition(void);
	// 重新绘制视频
	HREFTYPE RepaintVideo(void);
	// 监听事件
	virtual void GraphEventHandler(bool& bTerminated);
	// 视频窗口消息
	LRESULT VideoWindowMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	int m_state;
	CWnd *m_pVidWnd;
	WNDPROC m_pfnOldVidWndProc;
	CCritSec m_lock;
	CComPtr<IFilterGraph2> m_pGraph;
	CComPtr<IMediaEventEx> m_pME;
	CComPtr<IMediaControl> m_pMC;
	CComPtr<IMediaSeeking> m_pMS;
	CThread<CFGManager> *m_pEventThread;
	VideoRenderMode m_cfgVRM;
	bool m_cfgUseAudioSwitcher;
	CRect m_rctVideo;
	CComPtr<IBaseFilter> m_pVideoRenderer;
	CComPtr<IVMRFilterConfig9> m_pVMR9Cfg;
	CComPtr<IVMRWindowlessControl9> m_pVMR9WC;
	CComPtr<IMFVideoDisplayControl> m_pEVRWC;
	CComPtr<IBaseFilter> m_pAudioRenderer;
	CComPtr<IBaseFilter> m_pAudioSwitcher;
};
