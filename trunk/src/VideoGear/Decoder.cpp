#include "StdAfx.h"
#include "Decoder.h"

CDecoder::CDecoder(void)
{
}

CDecoder::~CDecoder(void)
{
}

//////////////////////////////////////////////////////////////////////////

class CFFDecoderWorker
{
public:
	CFFDecoderWorker(CFFDecoder* pDecoder)
		: m_pDecoder(pDecoder)
	{

	}

	void Execute(bool& bTerminated)
	{
		
	}
private:
	CFFDecoder* m_pDecoder;
};

//////////////////////////////////////////////////////////////////////////

