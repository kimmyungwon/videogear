#include "StdAfx.h"
#include "Splitter.h"

class CFFDemuxeWorker
{
public:
	CFFDemuxeWorker(CFFSplitter* pSplt)
		: m_pSplitter(pSplt)
	{}

	void Execute(bool& bTerminated)
	{
		AVFormatContext* pFmtCtx = m_pSplitter->m_pFmtCtx;
		
		while (!bTerminated)
		{
			AVPacket packet;

			if (av_read_frame(pFmtCtx, &packet) < 0)
				break;
			CPin* pOut = m_pSplitter->FindOutputPinByStreamIndex(packet.stream_index);
			if (pOut != NULL)
				pOut->Deliver(new CFFPacket(&packet));
			av_free_packet(&packet);
		}
	}
private:
	CFFSplitter* m_pSplitter;
};

//////////////////////////////////////////////////////////////////////////

#define IO_BUFFER_SIZE	32768

int FFReadPacket( void *opaque, uint8_t *buf, int buf_size )
{
	if (opaque != NULL && buf_size > 0)
		return (int)((CSource*)opaque)->Read((BYTE*)buf, buf_size);
	else
		return 0;
}

int FFWritePacket( void *opaque, uint8_t *buf, int buf_size )
{
	if (opaque != NULL && buf_size > 0)
		return (int)((CSource*)opaque)->Write((BYTE*)buf, buf_size);
	else
		return 0;	
}

int64_t FFSeek( void *opaque, int64_t offset, int whence )
{
	if (opaque != NULL)
		return ((CSource*)opaque)->Seek((LONGLONG)offset, whence);
	else
		return -1;
}

int FFReadPause( void *opaque, int pause )
{
	return AVERROR(ENOSYS);
}

int64_t FFReadSeek( void *opaque, int stream_index, int64_t timestamp, int flags )
{
	return AVERROR(ENOSYS);
}

//////////////////////////////////////////////////////////////////////////

CFFSplitterOutputPin::CFFSplitterOutputPin( CNode* pOwner, AVStream* ffStream )
: CPin(pOwner, PDIR_OUTPUT, MTYPE_VIDEO, MSTYPE_NONE)
{

}

//////////////////////////////////////////////////////////////////////////

CFFSplitter::CFFSplitter(void)
: m_pSource(NULL), m_pFmtCtx(NULL), m_pDecodeThread(NULL)
{
	m_pInput = new CPin(this, PDIR_INPUT, MTYPE_STREAM, MSTYPE_NONE);
	AddPin(m_pInput);
}

CFFSplitter::~CFFSplitter(void)
{
	RemoveAllPins();
}

CFFSplitterOutputPin* CFFSplitter::FindOutputPinByStreamIndex( UINT nIndex )
{
	std::map<UINT, CPin*>::iterator it = m_streamIdxToPin.find(nIndex);
	return it != m_streamIdxToPin.end() ? static_cast<CFFSplitterOutputPin*>(it->second) : NULL;
}

HRESULT CFFSplitter::InitIOContext( void )
{
	UINT nMaxPacketSize, nSize;
	BYTE* pBuffer;

	nMaxPacketSize = m_pSource->GetMaxPacketSize();
	nSize = nMaxPacketSize > 0 ? nMaxPacketSize : IO_BUFFER_SIZE;
	pBuffer = (BYTE*)av_malloc(nSize);
	if (pBuffer == NULL)
		return E_OUTOFMEMORY;
	if (init_put_byte(&m_ffIOCtx, pBuffer, nSize, 0, m_pSource, FFReadPacket, FFWritePacket, FFSeek) < 0)
	{
		av_free(pBuffer);
		return E_FAIL;
	}
	m_ffIOCtx.is_streamed = 0;
	m_ffIOCtx.max_packet_size = nMaxPacketSize;
	return S_OK;
}

void CFFSplitter::FreeIOContext( void )
{
	if (m_ffIOCtx.buffer != NULL)
	{
		av_free(m_ffIOCtx.buffer);
		m_ffIOCtx.buffer = NULL;
	}
	m_ffIOCtx.buf_ptr = NULL;
	m_ffIOCtx.opaque = NULL;
}

#define PROBE_BUF_MIN 2048
#define PROBE_BUF_MAX (1<<20)

AVInputFormat* CFFSplitter::DetectInputFormat( void )
{
	CStringA strFileNameA;
	AVProbeData pd;
	AVInputFormat* pFmt;
	
	strFileNameA = m_pSource->GetNameA();
	pd.filename = strFileNameA;
	pd.buf = NULL;
	pd.buf_size = 0;
	pFmt = av_probe_input_format(&pd, 0);

	if (pFmt == NULL || (pFmt->flags & AVFMT_NOFILE) == 0)
	{
		for (int nProbeSize = PROBE_BUF_MIN; nProbeSize <= PROBE_BUF_MAX && pFmt == NULL; nProbeSize <<= 1)
		{
			// 读取数据
			pd.buf = (unsigned char*)av_realloc(pd.buf, nProbeSize + AVPROBE_PADDING_SIZE);
			pd.buf_size = get_buffer(&m_ffIOCtx, pd.buf, nProbeSize);
			memset(&pd.buf[pd.buf_size], 0, AVPROBE_PADDING_SIZE);
			if (url_fseek(&m_ffIOCtx, 0, SEEK_SET) < 0)
			{
				if (FAILED(m_pSource->Open(m_pSource->GetName())))
					break;
			}
			// 猜测文件类型
			pFmt = av_probe_input_format(&pd, 1);
		}
		av_free(pd.buf);
		pd.buf = NULL;
	}

	if (pFmt != NULL && (pFmt->flags & AVFMT_NEEDNUMBER) != 0)
	{
		if (!av_filename_number_test(strFileNameA))
			return NULL;
	}

	return pFmt;
}

void CFFSplitter::ParseOutput( void )
{
	ASSERT(m_pFmtCtx != NULL);
	for (UINT i = 0; i < m_pFmtCtx->nb_streams; i++)
	{
		CFFSplitterOutputPin* pPin = new CFFSplitterOutputPin(this, m_pFmtCtx->streams[i]);
		AddPin(pPin);
		m_streamIdxToPin.insert(std::make_pair(i, pPin));
	}
}

void CFFSplitter::RemoveAllOutputs( void )
{	
	m_streamIdxToPin.clear();
	for (UINT i = 0; i < GetPinCount();)
	{
		CPin* pPin = GetPin(i);
		ASSERT(pPin != NULL);
		if (pPin->GetDirection() == PDIR_OUTPUT)
			RemovePin(i);
		else
			i++;
	}
}

HRESULT CFFSplitter::CheckInput( CPin* pPinIn )
{
	ASSERT(pPinIn != NULL);
	ASSERT(pPinIn->GetDirection() == PDIR_OUTPUT);

	CSource* pSource = static_cast<CSource*>(pPinIn->GetNode());
	if (pSource == NULL)
		return E_INVALIDARG;
	return S_OK;
}

HRESULT CFFSplitter::CompleteConnect( CPin* pPin, CPin* pPinRecv )
{
	if (pPin == m_pInput)
	{
		ASSERT(pPinRecv != NULL);
		ASSERT(pPinRecv->GetDirection() == PDIR_OUTPUT);
		
		CSource* pSource;
		HRESULT hr;
		AVInputFormat* pFmt = NULL;

		pSource = static_cast<CSource*>(pPinRecv->GetNode());
		if (pSource == NULL)
			return E_INVALIDARG;
		m_pSource = pSource;
		hr = InitIOContext();
		if (FAILED(hr))
		{
			m_pSource = NULL;
			return hr;
		}
		pFmt = DetectInputFormat();
		if (pFmt == NULL)
		{
			FreeIOContext();
			m_pSource = NULL;
			return E_FAIL;
		}
		if (av_open_input_stream(&m_pFmtCtx, &m_ffIOCtx, m_pSource->GetNameA(), pFmt, NULL) < 0)
		{
			FreeIOContext();
			m_pSource = NULL;
			return E_FAIL;
		}
		ParseOutput();
		m_pDecodeThread = new CThread<CFFDemuxeWorker>(new CFFDemuxeWorker(this));
		return S_OK;
	}
	else
		return S_OK;
}

HRESULT CFFSplitter::BreakConnect( CPin* pPin )
{
	ASSERT(pPin != NULL);
	ASSERT(pPin->GetNode() == this);

	if (pPin->GetDirection() == PDIR_INPUT)
	{
		if (m_pDecodeThread != NULL)
		{
			delete m_pDecodeThread;
			m_pDecodeThread = NULL;
		}
		RemoveAllOutputs();
		if (m_pFmtCtx != NULL)
		{
			av_close_input_stream(m_pFmtCtx);
			m_pFmtCtx = NULL;
		}
		FreeIOContext();
		m_pSource = NULL;
	}
	return S_OK;
}

