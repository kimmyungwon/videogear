#pragma once

#include "Source.h"
#include "Output.h"
#include "Thread.h"

class CSplitter
{
public:
	virtual ~CSplitter(void)	{}
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
	HRESULT SetSource(CFFSource* pSrc);
protected:
	AVInputFormat* DetectInputFormat(CFFSource* pSrc);
	void ParseOutput(void);
private:
	CFFSource* m_pSource;
	AVFormatContext* m_pFmtCtx;
	CThread<CFFDecoder>* m_pDecodeThread;
};
