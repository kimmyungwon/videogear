#include "StdAfx.h"
#include "Splitter.h"

UINT CSplitter::GetOutputCount( void ) const
{
	return m_outputs.size();
}

COutput* CSplitter::GetOutput( UINT nIndex )
{
	if (nIndex < m_outputs.size())
		return &m_outputs[nIndex];
	else
		return NULL;
}

void CSplitter::AddOutput( COutput* pOutput )
{
	m_outputs.push_back(pOutput);
}

void CSplitter::RemoveAllOutputs( void )
{
	m_outputs.clear();
}
//////////////////////////////////////////////////////////////////////////

class CFFDecoder
{
public:
	CFFDecoder(CFFSplitter* pSplt)
		: m_pSplitter(pSplt)
	{}

	void Execute(bool& bTerminated)
	{
		AVFormatContext* pFmtCtx = m_pSplitter->m_pFmtCtx;
		AVPacket* pPacket;

		while (!bTerminated)
		{
			pPacket = new AVPacket;
			if (av_read_frame(pFmtCtx, pPacket) < 0)
				break;
			CFFOutput* pOut = (CFFOutput*)m_pSplitter->GetOutput(pPacket->stream_index);
			if (pOut != NULL)
				pOut->Delivery(pPacket);
			av_free_packet(pPacket);
		}
	}
private:
	CFFSplitter* m_pSplitter;
};

//////////////////////////////////////////////////////////////////////////

CFFSplitter::CFFSplitter(void)
: m_pSource(NULL), m_pFmtCtx(NULL), m_pDecodeThread(NULL)
{
}

CFFSplitter::~CFFSplitter(void)
{
	SetSource(NULL);
}

HRESULT CFFSplitter::SetSource( CFFSource* pSrc )
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
			m_pSource = NULL;
		}
		if (pSrc != NULL)
		{
			AVInputFormat* pFmt = NULL;
			
			pFmt = DetectInputFormat(pSrc);
			if (pFmt == NULL)
				return ERROR_BAD_FORMAT;
			if (av_open_input_stream(&m_pFmtCtx, pSrc, pSrc->GetFileNameA(), pFmt, NULL) != 0)
			{
				av_free(m_pFmtCtx);
				m_pFmtCtx = NULL;
				return ERROR_BAD_FORMAT;
			}
			m_pSource = pSrc;
			ParseOutput();
			m_pDecodeThread = new CThread<CFFDecoder>(new CFFDecoder(this));
		}
	}
	return S_OK;
}

#define PROBE_BUF_MIN 2048
#define PROBE_BUF_MAX (1<<20)

AVInputFormat* CFFSplitter::DetectInputFormat( CFFSource* pSrc )
{
	ASSERT(pSrc != NULL);

	CStringA strFileNameA;
	AVProbeData pd;
	AVInputFormat* pFmt;
	
	strFileNameA = pSrc->GetFileNameA();
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
			pd.buf_size = get_buffer(pSrc, pd.buf, nProbeSize);
			memset(&pd.buf[pd.buf_size], 0, AVPROBE_PADDING_SIZE);
			if (url_fseek(pSrc, 0, SEEK_SET) < 0)
			{
				if (FAILED(pSrc->Reload()))
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
		CFFOutput* pOut = new CFFOutput(m_pFmtCtx->streams[i]);
		AddOutput(pOut);
	}
}