#pragma once

#include "IGraphBuilder2.h"

using namespace std;

struct OpenMediaData
{
	virtual ~OpenMediaData(void)	{}
};

struct OpenFileData : OpenMediaData
{
	vector<CString> gFiles;
	size_t nIndex; 

	OpenFileData(void): nIndex(0)	{}
};

#define WM_PLAYER_NOTIFY	WM_APP + 0x100

class CPlayer : public CObject
{
	friend DWORD WINAPI GraphEventProc(LPVOID);
public:
	enum {
		STATE_UNKNOWN = 0,
		STATE_IDLE,
		STATE_STOPPED,
		STATE_OPENNING,
		STATE_PLAYING,
		STATE_PAUSE
	};
public:
	CPlayer();
	virtual ~CPlayer();
	// ��ʼ����״̬
	HRESULT Initialize(HWND hwndMsg, HWND hwndVid);
	UINT GetState(void);
	BOOL IsMediaLoaded(void);
	// ��ý��
	HRESULT OpenMedia(CAutoPtr<OpenMediaData> pOMD);
	HRESULT OpenNext(void);
	// ��������
	HRESULT Play(void);
	HRESULT Stop(void);
	// ��Ƶ����
	HRESULT RepaintVideo(CDC* pDC);
	HRESULT GetVideoPosition(LPRECT lpRect);
	HRESULT UpdateVideoPosition(const LPRECT lpRect);
private:
	// �˾�����
	void ClearGraph(void);
	// ��ý��
	HRESULT RenderStreams(IBaseFilter* pSource);
	HRESULT OpenMediaPrivate(CAutoPtr<OpenMediaData> pOMD);
	HRESULT OpenFilePrivate(const CString& strFile);
	// ��Ϣ����
	void HandleGraphEvent(void);
private:
	UINT	m_nState;
	HWND	m_hwndMsg, m_hwndVid;
	CComPtr<IGraphBuilder2> m_pGraph;
	CComPtr<IMediaControl>	m_pMC;
	CComPtr<IMediaEvent>	m_pME;
	CComPtr<IVMRWindowlessControl>	m_pWC;
	HANDLE	m_hEventThread;
	CAutoPtr<OpenMediaData>	m_pOMD;
};


