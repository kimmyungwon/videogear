#include "StdAfx.h"
#include "Splitter.h"

CSplitter::CSplitter(void)
: m_pSource(NULL), m_pFmtCtx(NULL)
{
}

CSplitter::~CSplitter(void)
{
	SetSource(NULL);
}

HRESULT CSplitter::SetSource( CSource* pSrc )
{
	if (m_pSource != pSrc)
	{
		if (m_pSource != NULL)
		{
			if (m_pFmtCtx != NULL)
			{
				av_free(m_pFmtCtx->iformat);
				av_free(m_pFmtCtx);
				m_pFmtCtx = NULL;
			}
			m_pSource = NULL;
		}
		if (pSrc != NULL)
		{
			AVInputFormat* pFmt = DetectInputFormat(pSrc);
			if (pFmt == NULL)
				return ERROR_BAD_FORMAT;
			if (av_open_input_stream(&m_pFmtCtx, pSrc->GetIOContext(), NULL, pFmt, NULL) != 0)
			{
				av_free(m_pFmtCtx);
				m_pFmtCtx = NULL;
				return ERROR_BAD_FORMAT;
			}
			m_pSource = pSrc;
		}
	}
	return S_OK;
}

#define PROBE_BUF_MIN 2048
#define PROBE_BUF_MAX (1<<20)

AVInputFormat* CSplitter::DetectInputFormat( CSource* pSrc )
{
	ASSERT(pSrc != NULL);

	USES_CONVERSION;
	CStringA strFileNameA;
	AVProbeData pd;
	AVInputFormat* pFmt;
	
	strFileNameA = T2A(pSrc->GetName());
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
			pd.buf_size = get_buffer(pSrc->GetIOContext(), pd.buf, nProbeSize);
			memset(&pd.buf[pd.buf_size], 0, AVPROBE_PADDING_SIZE);
			if (pSrc->Seek(0, SEEK_SET) < 0)
			{
				if (FAILED(pSrc->Open(pSrc->GetName())))
					break;
			}
			// 猜测文件类型
			pFmt = av_probe_input_format(&pd, 1);
		}
		av_free(pd.buf);
	}

	return pFmt;
}