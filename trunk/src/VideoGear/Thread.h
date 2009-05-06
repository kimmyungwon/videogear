#pragma once

class CCritSec
{
public:
	CCritSec(void);
	~CCritSec(void);
	bool TryEnter(void);
	void Enter(void);
	void Leave(void);
private:
	RTL_CRITICAL_SECTION m_cs;
};

class CAutoLock
{
public:
	CAutoLock(CCritSec* pCS);
	~CAutoLock(void);
private:
	CCritSec* m_pCS;
};

template<typename WorkerT>
class CThread
{
public:
	CThread(WorkerT* pWorker)
		: m_pWorker(pWorker), m_bFinished(false), m_bTerminated(false)
	{
		m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, (LPVOID)this, 0, &m_dwThreadId);
	}
	
	virtual ~CThread(void)
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

	void Terminate(void)
	{
		if (m_hThread != NULL)
			m_bTerminated = true;
	}

	void WaitFor(DWORD dwTimeout)
	{
		if (m_hThread != NULL)
			WaitForSingleObject(m_hThread, dwTimeout);
	}
protected:	
	static DWORD WINAPI ThreadProc(CThread* pThread)
	{
		ASSERT(pThread != NULL && pThread->m_pWorker != NULL);
		pThread->m_pWorker->Execute(pThread->m_bTerminated);
		pThread->m_bFinished = true;
		return 0;
	}
private:
	WorkerT* m_pWorker;
	bool m_bFinished, m_bTerminated;
	HANDLE m_hThread;
	DWORD m_dwThreadId;
};
