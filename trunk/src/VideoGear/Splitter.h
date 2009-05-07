#pragma once

#include "Node.h"
#include "Source.h"
#include "Thread.h"

class CSplitter : public CNode
{
public:
	virtual ~CSplitter(void)	{}
	virtual HRESULT SetSource(CSource* pSrc) = 0;
};

class CFFSplitter : public CSplitter
{
	friend class CFFDemuxeWorker;
public:
	CFFSplitter(void);
	virtual ~CFFSplitter(void);
	/* CSplitter */
	virtual HRESULT SetSource(CSource* pSrc);
protected:
	HRESULT InitIOContext(void);
	void FreeIOContext(void);
	AVInputFormat* DetectInputFormat(void);
	void ParseOutput(void);
private:
	CSource* m_pSource;
	ByteIOContext m_ffIOCtx;
	AVFormatContext* m_pFmtCtx;
	CThread<CFFDemuxeWorker>* m_pDecodeThread;
};
