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

struct CWorker
{
	virtual void Execute(bool& bTerminated) = 0;
};

class CThread
{
public:
	CThread(CWorker* pWorker);
	virtual ~CThread(void);
	void Suspend(void);
	void Resume(void);
	void Terminate(void);
	void WaitFor(DWORD dwTimeout);
protected:	
	static DWORD WINAPI ThreadProc(CThread* pThread);
private:
	CWorker* m_pWorker;
	bool m_bFinished, m_bTerminated;
	HANDLE m_hThread;
	DWORD m_dwThreadId;
};
