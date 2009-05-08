#include "StdAfx.h"
#include "Thread.h"

CCritSec::CCritSec( void )
{
	InitializeCriticalSection(&m_cs);
}

CCritSec::~CCritSec( void )
{
	DeleteCriticalSection(&m_cs);
}

bool CCritSec::TryEnter( void )
{
	return (TryEnterCriticalSection(&m_cs) != FALSE);
}

void CCritSec::Enter( void )
{	
	EnterCriticalSection(&m_cs);
}

void CCritSec::Leave( void )
{
	LeaveCriticalSection(&m_cs);
}

//////////////////////////////////////////////////////////////////////////

CAutoLock::CAutoLock( CCritSec* pCS )
: m_pCS(pCS)
{
	if (m_pCS != NULL)
		m_pCS->Enter();
}

CAutoLock::~CAutoLock( void )
{
	if (m_pCS != NULL)
		m_pCS->Leave();
}

//////////////////////////////////////////////////////////////////////////

CThread::CThread( CWorker* pWorker ) 
: m_pWorker(pWorker), m_bFinished(false), m_bTerminated(false)
{
	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, (LPVOID)this, 0, &m_dwThreadId);
}

CThread::~CThread( void )
{
	if (m_hThread != NULL)
	{
		if (!m_bFinished)
		{
			Terminate();
			WaitFor(INFINITE);
		}
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	if (m_pWorker != NULL)
	{
		delete m_pWorker;
		m_pWorker = NULL;
	}
}

void CThread::Suspend( void )
{
	if (m_hThread != NULL)
		SuspendThread(m_hThread);
}

void CThread::Resume( void )
{
	if (m_hThread != NULL)
		ResumeThread(m_hThread);
}

void CThread::Terminate( void )
{
	if (m_hThread != NULL)
	{
		m_bTerminated = true;
		Resume();
	}
}

void CThread::WaitFor( DWORD dwTimeout )
{
	if (m_hThread != NULL)
		WaitForSingleObject(m_hThread, dwTimeout);
}

DWORD WINAPI CThread::ThreadProc( CThread* pThread )
{
	ASSERT(pThread != NULL && pThread->m_pWorker != NULL);
	pThread->m_pWorker->Execute(pThread->m_bTerminated);
	pThread->m_bFinished = true;
	return 0;
}