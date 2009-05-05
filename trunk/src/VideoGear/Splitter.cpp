#include "StdAfx.h"
#include "Splitter.h"

int match_ext(const char *filename, const char *extensions)
{
	const char *ext, *p;
	char ext1[32], *q;

	if(!filename)
		return 0;

	ext = strrchr(filename, '.');
	if (ext) {
		ext++;
		p = extensions;
		for(;;) {
			q = ext1;
			while (*p != '\0' && *p != ',' && q-ext1<sizeof(ext1)-1)
				*q++ = *p++;
			*q = '\0';
			if (!strcmp(ext1, ext))
				return 1;
			if (*p == '\0')
				break;
			p++;
		}
	}
	return 0;
}

static AVInputFormat *av_probe_input_format2(AVProbeData *pd, int is_opened, int *score_max)
{
	AVInputFormat *fmt1, *fmt;
	int score;

	fmt = NULL;
	for(fmt1 = first_iformat; fmt1 != NULL; fmt1 = fmt1->next) {
		if (!is_opened == !(fmt1->flags & AVFMT_NOFILE))
			continue;
		score = 0;
		if (fmt1->read_probe) {
			score = fmt1->read_probe(pd);
		} else if (fmt1->extensions) {
			if (match_ext(pd->filename, fmt1->extensions)) {
				score = 50;
			}
		}
		if (score > *score_max) {
			*score_max = score;
			fmt = fmt1;
		}else if (score == *score_max)
			fmt = NULL;
	}
	return fmt;
}

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
				av_close_input_stream(m_pFmtCtx);
				m_pFmtCtx = NULL;
			}
			m_pSource = NULL;
		}
		if (pSrc != NULL)
		{
			USES_CONVERSION;
			AVInputFormat* pFmt = NULL;
			CStringA strFileNameA;
			
			pFmt = DetectInputFormat(pSrc);
			if (pFmt == NULL)
				return ERROR_BAD_FORMAT;
			strFileNameA = T2A(pSrc->GetName());
			if (av_open_input_stream(&m_pFmtCtx, pSrc->GetIOContext(), strFileNameA, pFmt, NULL) != 0)
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
			int nScore = nProbeSize < PROBE_BUF_MAX ? AVPROBE_SCORE_MAX / 4 : 0;
			// 读取数据
			pd.buf = (unsigned char*)av_realloc(pd.buf, nProbeSize + AVPROBE_PADDING_SIZE);
			pd.buf_size = get_buffer(pSrc->GetIOContext(), pd.buf, nProbeSize);
			memset(&pd.buf[pd.buf_size], 0, AVPROBE_PADDING_SIZE);
			if (url_fseek(pSrc->GetIOContext(), 0, SEEK_SET) < 0)
			{
				if (FAILED(pSrc->Open(pSrc->GetName())))
					break;
			}
			// 猜测文件类型
			pFmt = av_probe_input_format2(&pd, 1, &nScore);
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