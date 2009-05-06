#pragma once

#include "Stream.h"

#define SOURCE_BUFFER_SIZE	32768

class CSource	
{
public:
	CSource(CStream* pStream, bool bOwnsObject = false);
	virtual ~CSource(void)	{}
	virtual const CString& GetFileName(void) const;
	virtual CStringA GetFileNameA(void) const;
	virtual HRESULT Reload(void);
protected:
	CStream* m_pStream;
	bool m_bOwnsObject;
};

class CFFSource 
	: public CSource
	, public ByteIOContext
{
public:
	CFFSource(CStream* pStream, bool bOwnsObject = false);
	virtual ~CFFSource(void);
};
