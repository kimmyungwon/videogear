#pragma once

#include "AppConfig.h"
#include "Thread.h"

[event_source(native, optimize=speed)]
class CFGManager : public CCritSec
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
	// ��ȡ��ǰ״̬
	int GetState(void);
	// ��ʼ��
	HRESULT Initialize(CWnd *pVidWnd);
	// ��Ⱦָ�����ļ�
	HRESULT RenderFile(LPCWSTR pszFile);
	// ��ȡý���ܳ���
	HRESULT GetDuration(int &nDuration);
	// ��ȡ�ɲ��ŵĳ���
	HRESULT GetAvailable(int &nEarliest, int &nLastest);
	// ��ȡ��ǰλ��
	HRESULT GetPosition(int &nPosition);
	// ���õ�ǰλ��
	HRESULT SetPosition(int nPosition);
	// ��ʼ����
	HRESULT Run(void);
	// ֹͣ����
	HRESULT Stop(void);
public:
	__event void OnStateChanged(int iNewState);
	__event void OnMediaCompleted(void);
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
	// ֱ��������������
	HRESULT ConnectDirect(IPin *pPinOut, IPin *pPinIn, const AM_MEDIA_TYPE *pmt);
	// ��������˾���
	HRESULT TearDownStream(IUnknown *pUnk);
	// ����˾��Ƿ�����ʹ��
	HRESULT CheckInUse(IBaseFilter *pFilter);
	// ���ͼ��
	HRESULT ClearGraph(void);
	// ������Ƶλ�ú���Ƶ���ڴ�С
	HRESULT AdjustVideoPosition(void);
	// ���»�����Ƶ
	HREFTYPE RepaintVideo(void);
	// �����¼�
	virtual void GraphEventHandler(bool& bTerminated);
	// ��Ƶ������Ϣ
	LRESULT VideoWindowMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void SetState(int iNewState);
private:
	int m_iState;
	CWnd *m_pVidWnd;
	WNDPROC m_pfnOldVidWndProc;
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
