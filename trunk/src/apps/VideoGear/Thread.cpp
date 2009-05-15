#include "StdAfx.h"
#include "Thread.h"

CMyCritSec::CMyCritSec( void )
{
	InitializeCriticalSection(&m_cs);
}

CMyCritSec::~CMyCritSec( void )
{
	DeleteCriticalSection(&m_cs);
}

bool CMyCritSec::TryEnter( void )
{
	return (TryEnterCriticalSection(&m_cs) != FALSE);
}

void CMyCritSec::Lock( void )
{	
	EnterCriticalSection(&m_cs);
}

void CMyCritSec::Unlock( void )
{
	LeaveCriticalSection(&m_cs);
}