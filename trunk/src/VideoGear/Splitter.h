#pragma once

#include "Source.h"
#include "Output.h"
#include "Thread.h"

class CSplitter
{
public:
	virtual ~CSplitter(void)	{}
	virtual HRESULT SetSource(CSource* pSrc) = 0;
	UINT GetOutputCount(void) const;
	COutput* GetOutput(UINT nIndex);
protected:
	void AddOutput(COutput* pOutput);
	void RemoveAllOutputs(void);
private:
	typedef boost::ptr_vector<COutput> OutputList;

	OutputList	m_outputs;
};

class CFFSplitter : public CSplitter
{
	friend class CFFDecoder;
public:
	CFFSplitter(void);
	virtual ~CFFSplitter(void);
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
	CThread<CFFDecoder>* m_pDecodeThread;
};
