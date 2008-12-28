#pragma once

#include "IGraphBuilder2.h"

using namespace std;

struct OpenMediaData
{
	virtual ~OpenMediaData(void);
};

struct OpenFileData : OpenMediaData
{
	vector<CString> gFiles;
	size_t nIndex; 

	OpenFileData(void): nIndex(0)	{}
};

class CPlayer : public CObject
{
	friend DWORD WINAPI GraphEventProc(LPVOID);
public:
	enum {
		STATE_UNKNOWN = 0,
		STATE_IDLE,
		STATE_OPENNING,
		STATE_PLAYING,
		STATE_PAUSE
	};
public:
	CPlayer();
	virtual ~CPlayer();
	// ��ʼ��
	HRESULT Initialize(HWND hwndMsg, HWND hwndVid);
	// ��ý��
	HRESULT OpenMedia(CAutoPtr<OpenMediaData> pOMD);
	// ��������
	HRESULT Stop(void);
private:
	// �˾�����
	void TearDownGraph(void);
	// ��ý��
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


