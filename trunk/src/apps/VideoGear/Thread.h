#pragma once

class CMyCritSec
{
public:
	CMyCritSec(void);
	~CMyCritSec(void);
	bool TryEnter(void);
	void Lock(void);
	void Unlock(void);
private:
	RTL_CRITICAL_SECTION m_cs;
};

class CMyAutoLock
{
public:
	CMyAutoLock(CMyCritSec* pCS);
	~CMyAutoLock(void);
private:
	CMyCritSec* m_pCS;
};

template<typename ClassT>
class CThread
{
public:
	typedef void (ClassT::*PFNExecute)(bool&);
	
	CThread(ClassT *pWorker, PFNExecute pfnExecute)
		: m_pWorker(pWorker), m_pfnExecute(pfnExecute), m_bFinished(false), m_bTerminated(false)
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
	}

	void Suspend(void)
	{
		if (m_hThread != NULL)
			SuspendThread(m_hThread);
	}

	void Resume(void)
	{
		if (m_hThread != NULL)
			ResumeThread(m_hThread);
	}

	void Terminate(void)
	{
		if (m_hThread != NULL)
		{
			m_bTerminated = true;
			Resume();
		}
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
		(pThread->m_pWorker->*(pThread->m_pfnExecute))(pThread->m_bTerminated);
		pThread->m_bFinished = true;
		return 0;
	}
private:
	ClassT *m_pWorker;
	PFNExecute m_pfnExecute;
	bool m_bFinished, m_bTerminated;
	HANDLE m_hThread;
	DWORD m_dwThreadId;
};
