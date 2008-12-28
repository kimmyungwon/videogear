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
	// 初始化
	HRESULT Initialize(HWND hwndMsg, HWND hwndVid);
	// 打开媒体
	HRESULT OpenMedia(OpenMediaData* pOMD);
private:
	// 打开媒体
	HRESULT OpenMediaPrivate(OpenMediaData* pOMD);
	// 消息处理
	void HandleGraphEvent(void);
private:
	HWND	m_hwndMsg, m_hwndVid;
	CComPtr<IGraphBuilder2> m_pGraph;
	CComPtr<IMediaControl>	m_pMC;
	CComPtr<IMediaEvent>	m_pME;
	HANDLE	m_hEventThread;
};


