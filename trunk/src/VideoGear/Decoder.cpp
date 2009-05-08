#include "StdAfx.h"
#include "Decoder.h"
#include "Splitter.h"

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

CFFDecoder::CFFDecoder( void )
: m_pCodecCtx(NULL)
{
	
}

HRESULT CFFDecoder::CheckInput( CPin* pPinIn )
{
	ASSERT(pPinIn != NULL);
	ASSERT(pPinIn->GetDirection() == PDIR_INPUT);

	MediaType mtIn;
	CodecID codecId;
	
	mtIn = pPinIn->GetMediaType();
	codecId = (CodecID)(mtIn.minor & ~MTYPE_FFMPEG);
	if (codecId == CODEC_ID_NONE)
		return VGERR_TYPE_NOT_ACCEPT;
	return avcodec_find_decoder(codecId) != NULL ? S_OK : VGERR_TYPE_NOT_ACCEPT;
}

HRESULT CFFDecoder::CompleteConnect( CPin* pPin, CPin* pPinRecv )
{
	if (pPin->GetDirection() == PDIR_INPUT)
	{
		ASSERT(pPinRecv != NULL);
		ASSERT(pPinRecv->GetDirection() == PDIR_INPUT);

		CFFSplitterOutputPin* pPinOut = static_cast<CFFSplitterOutputPin*>(pPinRecv);
		if (pPinOut != NULL)
		{
			ASSERT(pPinOut->GetStream() != NULL);
			m_pCodecCtx = pPinOut->GetStream()->codec;
			return S_OK;
		}
		else
			return VGERR_NOT_SUPPORTED;
	}
	else
		return S_OK;
}

HRESULT CFFDecoder::BreakConnect( CPin* pPin )
{
	if (pPin->GetDirection() == PDIR_INPUT)
	{
		m_pCodecCtx = NULL;
		return S_OK;
	}
	else
		return S_OK;
}

HRESULT CFFDecoder::DoRun( void )
{
	if (GetState() == STATE_STOPPED)
		m_pDecodeThread = new CThread<CFFDecoderWorker>(new CFFDecoderWorker(this));
	else
	{
		if (m_pDecodeThread == NULL)
			return E_UNEXPECTED;
		m_pDecodeThread->Resume();
	}
	return S_OK;
}

HRESULT CFFDecoder::DoStop( void )
{
	if (m_pDecodeThread == NULL)
		return VGERR_NOT_SUPPORTED;
	if (GetState() == STATE_PAUSED)
		m_pDecodeThread->Resume();
	delete m_pDecodeThread;
	m_pDecodeThread = NULL;
	return S_OK;
}

HRESULT CFFDecoder::DoPause( void )
{
	if (m_pDecodeThread == NULL)
		return VGERR_NOT_SUPPORTED;
	m_pDecodeThread->Suspend();
	return S_OK;
}