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
};

class CPlayer : public CObject
{
	friend DWORD WINAPI GraphEventProc(LPVOID);
public:
	CPlayer();
	virtual ~CPlayer();
	// ��ʼ��
	HRESULT Initialize(HWND hwndMsg, HWND hwndVid);
	// ��ý��
	HRESULT OpenMedia(OpenMediaData* pOMD);
private:
	// ��ý��
	HRESULT OpenMediaPrivate(OpenMediaData* pOMD);
	// ��Ϣ����
	void HandleGraphEvent(void);
private:
	HWND	m_hwndMsg, m_hwndVid;
	CComPtr<IGraphBuilder2> m_pGraph;
	CComPtr<IMediaControl>	m_pMC;
	CComPtr<IMediaEvent>	m_pME;
	HANDLE	m_hEventThread;
};


