#include "StdAfx.h"
#include "VGPlayer.h"
#include "FGManager.h"

struct PlayerMsgThreadContext
{
	CVGPlayer *pOwner;
	HANDLE hIOCP;
};

enum {
	pmShutdown = 0xFFFFFFFFul
};

DWORD WINAPI PlayerMsgProc( PlayerMsgThreadContext *lpThreadParameter )
{
	DWORD dwMsg;
	ULONG_PTR ulParam1, ulParam2;
	
	ASSERT(lpThreadParameter != NULL);
	while (true)
	{
		GetQueuedCompletionStatus(lpThreadParameter->hIOCP, &dwMsg, &ulParam1, (LPOVERLAPPED*)&ulParam2, INFINITE);
		switch (dwMsg)
		{
		case pmShutdown:
			return 0;
		}
	}		
}

CVGPlayer::CVGPlayer(void)
: m_hIOCP(NULL), m_hMsgThrd(NULL)
{
}

CVGPlayer::~CVGPlayer(void)
{
	Clear();
	m_pGB = NULL;
}

HRESULT CVGPlayer::Initialize( HWND hwnd )
{
	HRESULT hr;
	static PlayerMsgThreadContext context;
	
	Clear();
	// 初始化DShow
	m_pGB = new CFilterManager(&hr);
	if (FAILED(hr))
		return hr;
	// 建立线程
	m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
	context.pOwner = this;
	context.hIOCP = m_hIOCP;
	m_hMsgThrd = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&PlayerMsgProc, (LPVOID)&context, 0, NULL);
	return S_OK;
}

void CVGPlayer::Clear( void )
{
	//TODO: 停止播放
	if (m_hIOCP != NULL)
	{
		PostQueuedCompletionStatus(m_hIOCP, pmShutdown, (ULONG_PTR)0, (LPOVERLAPPED)0);
		if (m_hMsgThrd != NULL)
		{
			WaitForSingleObject(m_hMsgThrd, INFINITE);
			CloseHandle(m_hMsgThrd);
			m_hMsgThrd = NULL;
		}
		CloseHandle(m_hIOCP);
		m_hIOCP = NULL;
	}
}