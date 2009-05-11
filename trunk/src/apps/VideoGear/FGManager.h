#pragma once

#include "AppConfig.h"

class CFGManager
{
public:
	CFGManager(void);
	virtual ~CFGManager(void);
	// ��ʼ��
	HRESULT Initialize(CWnd *pVidWnd);
	// ��Ⱦָ�����ļ�
	HRESULT RenderFile(LPCWSTR pszFile);
protected:
	// ��ָ���ļ�����Դ�˾�������
	HRESULT AddSourceFilter(LPCWSTR pszFile, IBaseFilter **ppFilter);
	// ����ָ����Դ�˾�
	HRESULT SplitSource(IBaseFilter *pSource, IBaseFilter **ppFilter);
	// ��Ⱦָ�����˾�
	HRESULT RenderFilter(IBaseFilter *pFilter, bool bReportError = false);
	// ��Ⱦָ�����������
	HRESULT Render(IPin *pPinOut);
	// ֱ������������뵽�˾�
	HRESULT ConnectDirect(IPin *pPinOut, IBaseFilter *pFilterIn, const AM_MEDIA_TYPE *pmt);
	// ��������˾���
	HRESULT TearDownStream(IUnknown *pUnk);
	// ���ͼ��
	HRESULT ClearGraph(void);
private:
	bool m_bInitialized;
	CWnd *m_pVidWnd;
	CComPtr<IFilterGraph2> m_pGraph;
	CComPtr<IMediaControl> m_pMC;
	VideoRenderMode m_vmr;
	CComPtr<IBaseFilter> m_pVideoRenderer;
};
