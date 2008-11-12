//---------------------------------------------------------------------------

#ifndef ThreadUtilsH
#define ThreadUtilsH

#include <SyncObjs.hpp>

class TCritSec
{
	friend class TAutoLock;
public:
	TCritSec(void);
	virtual ~TCritSec(void);
private:
	TCriticalSection	*m_pCS;
};

class TAutoLock
{
public:
	TAutoLock(const TCritSec *pCS);
	virtual ~TAutoLock(void);
private:
	TCriticalSection	*m_pCS;
};

//---------------------------------------------------------------------------
#endif
