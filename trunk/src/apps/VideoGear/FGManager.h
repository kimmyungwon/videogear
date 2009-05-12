#pragma once

#include "AppConfig.h"

class CFGManager
{
public:
	CFGManager(void);
	virtual ~CFGManager(void);
	// 初始化
	HRESULT Initialize(CWnd *pVidWnd);
	// 渲染指定的文件
	HRESULT RenderFile(LPCWSTR pszFile);
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
	// 清除没有连接的滤镜
	HRESULT RemoveIfNotUsed(CComPtr<IBaseFilter> &pFilter);
	// 清空图表
	HRESULT ClearGraph(void);
private:
	bool m_bInitialized;
	CWnd *m_pVidWnd;
	CComPtr<IFilterGraph2> m_pGraph;
	CComPtr<IMediaControl> m_pMC;
	VideoRenderMode m_cfgVRM;
	bool m_cfgUseAudioSwitcher;
	CComPtr<IBaseFilter> m_pVideoRenderer;
	CComPtr<IBaseFilter> m_pAudioRenderer;
	CComPtr<IBaseFilter> m_pAudioSwitcher;
};
