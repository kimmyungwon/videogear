#pragma once

#include "Node.h"
#include "Source.h"
#include "Thread.h"

class CSplitter : public CNode
{
public:
	virtual ~CSplitter(void)	{}
};

class CFFSplitterOutputPin : public CPin
{
public:
	CFFSplitterOutputPin(AVStream& ffStream);
};

class CFFSplitter : public CNode
{
	friend class CFFDemuxeWorker;
public:
	CFFSplitter(void);
	virtual ~CFFSplitter(void);
protected:
	HRESULT InitIOContext(void);
	void FreeIOContext(void);
	AVInputFormat* DetectInputFormat(void);
	void ParseOutput(void);
	/* CNode */
	virtual HRESULT CheckInput(CPin* pPinIn);
	virtual HRESULT CompleteConnect(PinDirection dir, CPin* pPinIn);
private:
	CSource* m_pSource;
	ByteIOContext m_ffIOCtx;
	AVFormatContext* m_pFmtCtx;
	std::map<UINT, CPin*> m_streamIdxToPin;
	CThread<CFFDemuxeWorker>* m_pDecodeThread;
};
