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
