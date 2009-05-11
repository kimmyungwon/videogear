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
	HRESULT RenderFilter(IBaseFilter *pFilter);
	// 渲染指定的输出插针
	HRESULT Render(IPin *ppinOut);
	// 直接连接输出插针到滤镜
	HRESULT ConnectDirect(IPin *pPinOut, IBaseFilter *pFilterIn);
	// 拆除下游滤镜链
	HRESULT TearDownStream(IUnknown *pUnk);
	// 清空图表
	HRESULT ClearGraph(void);
private:
	bool m_bInitialized;
	CWnd *m_pVidWnd;
	CComPtr<IFilterGraph2> m_pGraph;
	VideoRenderMode m_vmr;
	CComPtr<IBaseFilter> m_pVideoRenderer;
};
