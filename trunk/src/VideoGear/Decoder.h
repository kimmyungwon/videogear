#pragma once

#include "Output.h"
#include "Thread.h"

class CDecoder
{
public:
	CDecoder(void);
	virtual ~CDecoder(void);
	virtual HRESULT SetOutput(COutput* pOut) = 0;
};

class CFFDecoder : public CDecoder
{
	friend class CFFDecoderWorker;
public:
	virtual HRESULT SetOutput(COutput* pOut);
private:
	COutput* m_pOutput;
};
