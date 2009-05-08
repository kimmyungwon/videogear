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
	CFFSplitterOutputPin(CNode* pOwner, AVStream* ffStream);
private:
	AVStream* m_pStream;
};

class CFFSplitter : public CNode
{
	friend class CFFDemuxeWorker;
public:
	CFFSplitter(void);
	virtual ~CFFSplitter(void);
	CFFSplitterOutputPin* FindOutputPinByStreamIndex(UINT nIndex);
protected:
	HRESULT InitIOContext(void);
	void FreeIOContext(void);
	AVInputFormat* DetectInputFormat(void);
	void ParseOutput(void);
	void RemoveAllOutputs(void);
	/* CNode */
	virtual HRESULT CheckInput(CPin* pPinIn);
	virtual HRESULT CompleteConnect(CPin* pPin, CPin* pPinRecv);
	virtual HRESULT BreakConnect(CPin* pPin);
private:
	CPin* m_pInput;
	CSource* m_pSource;
	ByteIOContext m_ffIOCtx;
	AVFormatContext* m_pFmtCtx;
	std::map<UINT, CPin*> m_streamIdxToPin;
	CThread<CFFDemuxeWorker>* m_pDecodeThread;
};
