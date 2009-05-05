#pragma once

#include "Source.h"

class CSplitter
{
public:
	CSplitter(void);
	virtual ~CSplitter(void);
	HRESULT SetSource(CSource* pSrc);
protected:
	AVInputFormat* DetectInputFormat(CSource* pSrc);
private:
	CSource* m_pSource;
	AVFormatContext* m_pFmtCtx;
};
