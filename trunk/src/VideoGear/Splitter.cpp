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
			COutput* pOut = m_pSplitter->GetOutput(packet.stream_index);
			if (pOut != NULL)
				pOut->Delivery(new CFFPacket(&packet));
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

CFFSplitter::CFFSplitter(void)
: m_pSource(NULL), m_pFmtCtx(NULL), m_pDecodeThread(NULL)
{
}

CFFSplitter::~CFFSplitter(void)
{
	SetSource(NULL);
}

HRESULT CFFSplitter::SetSource( CSource* pSrc )
{
	if (m_pSource != pSrc)
	{
		if (m_pSource != NULL)
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
		if (pSrc != NULL)
		{
			HRESULT hr;
			AVInputFormat* pFmt = NULL;
			
			m_pSource = pSrc;
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
		}
	}
	return S_OK;
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
		COutput* pOut = new CFFOutput;
		((CFFOutput*)pOut)->Initialize(m_pFmtCtx->streams[i]);
		AddOutput(pOut);
	}
}