//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ThreadUtils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TCritSec::TCritSec(void)
{
	m_pCS = new TCriticalSection;
}

TCritSec::~TCritSec(void)
{
    delete m_pCS;
}

TAutoLock::TAutoLock(const TCritSec *pCS)
{
	assert(pCS != NULL);
	m_pCS = pCS->m_pCS;
	m_pCS->Acquire();
	ATLTRACE(L"TAutoLock Created");
}

TAutoLock::~TAutoLock(void)
{
	ATLTRACE(L"TAutoLock Freeing");
	m_pCS->Release();
	m_pCS = NULL;
}