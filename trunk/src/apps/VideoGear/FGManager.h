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
	HRESULT RenderFilter(IBaseFilter *pFilter);
	// ��Ⱦָ�����������
	HRESULT Render(IPin *ppinOut);
	// ֱ������������뵽�˾�
	HRESULT ConnectDirect(IPin *pPinOut, IBaseFilter *pFilterIn);
	// ��������˾���
	HRESULT TearDownStream(IUnknown *pUnk);
	// ���ͼ��
	HRESULT ClearGraph(void);
private:
	bool m_bInitialized;
	CWnd *m_pVidWnd;
	CComPtr<IFilterGraph2> m_pGraph;
	VideoRenderMode m_vmr;
	CComPtr<IBaseFilter> m_pVideoRenderer;
};
