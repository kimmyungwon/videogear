#pragma once

#include "Node.h"
#include "Thread.h"

class CDecoder : public CNode
{
public:
	CDecoder(void);
	virtual ~CDecoder(void);
};

class CFFDecoder : public CDecoder
{
	friend class CFFDecoderWorker;
public:
	CFFDecoder(void);
protected:
	/* CNode */
	virtual HRESULT CheckInput(CPin* pPinIn);
	virtual HRESULT CompleteConnect(CPin* pPin, CPin* pPinRecv);
	virtual HRESULT BreakConnect(CPin* pPin);
	virtual HRESULT DoRun(void);
	virtual HRESULT DoStop(void);
	virtual HRESULT DoPause(void);
private:
	AVCodecContext* m_pCodecCtx;
	CThread<CFFDecoderWorker>* m_pDecodeThread;
};
