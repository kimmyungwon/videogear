#include "StdAfx.h"
#include "VGVideoDecFilter.h"

struct FfmpegCodecs
{
	const GUID* subtype;
	CodecID codecID;
	FOURCC fourCC;
};

const FfmpegCodecs g_ffCodecs[] = 
{
	/* MPEG4 */
	{&MEDIASUBTYPE_XVID, CODEC_ID_MPEG4, MAKEFOURCC('X', 'V', 'I', 'D')},
	{&MEDIASUBTYPE_xvid, CODEC_ID_MPEG4, MAKEFOURCC('x', 'v', 'i', 'd')},
	/* H264 */
	{&MEDIASUBTYPE_H264, CODEC_ID_H264, MAKEFOURCC('H', '2', '6', '4')},
};

struct VideoOutputFormats
{
	const GUID* subtype;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
};

const VideoOutputFormats g_vidOutFmts[] =
{
	{&MEDIASUBTYPE_YUY2, 1, 16, '2YUY'},
};

//////////////////////////////////////////////////////////////////////////

// 顺序必须和g_ffCodecs完全一致
const AMOVIESETUP_MEDIATYPE CVGVideoDecFilter::ms_PinTypesIn[] = 
{
	/* XVID */
	{&MEDIATYPE_Video, &MEDIASUBTYPE_XVID},
	{&MEDIATYPE_Video, &MEDIASUBTYPE_xvid},
	/* H264 */
	{&MEDIATYPE_Video, &MEDIASUBTYPE_H264},
};

const UINT CVGVideoDecFilter::ms_nPinTypesInCount = _countof(CVGVideoDecFilter::ms_PinTypesIn);

const AMOVIESETUP_MEDIATYPE CVGVideoDecFilter::ms_PinTypesOut[] = 
{
	{&MEDIATYPE_Video, &MEDIASUBTYPE_NULL}
};

const UINT CVGVideoDecFilter::ms_nPinTypesOutCount = _countof(CVGVideoDecFilter::ms_PinTypesOut);

CUnknown* CALLBACK CVGVideoDecFilter::CreateInstance( LPUNKNOWN pUnk, HRESULT *pHr )
{
	CVGVideoDecFilter* pFilter = new CVGVideoDecFilter(pUnk);
	if (pFilter == NULL && pHr != NULL)
		*pHr = E_OUTOFMEMORY;
	return pFilter;
}

CVGVideoDecFilter::CVGVideoDecFilter( __inout_opt LPUNKNOWN pUnk )
: CVideoTransformFilter(NAME("VideoGear - Video Decoder"), pUnk, CLSID_VGVideoDecFilter)
{
	m_lWidth = m_lHeight = 0;

	avcodec_init();
	avcodec_register_all();
}

CVGVideoDecFilter::~CVGVideoDecFilter(void)
{
}

HRESULT CVGVideoDecFilter::CheckInputType( const CMediaType* mtIn )
{
	for (int i = 0; i < _countof(ms_PinTypesIn); i++)
	{
		if (*ms_PinTypesIn[i].clsMajorType == mtIn->majortype
			&& *ms_PinTypesIn[i].clsMinorType == mtIn->subtype)
			return S_OK;
	}
	return VFW_E_TYPE_NOT_ACCEPTED;
}

HRESULT CVGVideoDecFilter::SetMediaType( PIN_DIRECTION direction,const CMediaType *pmt )
{
	if (direction == PINDIR_INPUT)
	{
		int nCodec;
		
		nCodec = FindCodec(pmt);
		if (nCodec == -1)
			return VFW_E_TYPE_NOT_ACCEPTED;
		
		m_pAVCodec = avcodec_find_decoder(g_ffCodecs[nCodec].codecID);
		if (m_pAVCodec == NULL)
			return VFW_E_UNSUPPORTED_VIDEO;
		m_pAVCodecCtx = avcodec_alloc_context();
		if (m_pAVCodecCtx == NULL)
			return E_POINTER;
		m_pAVFrame = avcodec_alloc_frame();
		if (m_pAVFrame == NULL)
			return E_POINTER;

		if (pmt->formattype == FORMAT_VideoInfo)
		{
			VIDEOINFOHEADER* vih = (VIDEOINFOHEADER*)pmt->pbFormat;
			m_pAVCodecCtx->width = vih->bmiHeader.biWidth;
			m_pAVCodecCtx->height = vih->bmiHeader.biHeight;
			m_pAVCodecCtx->codec_tag = vih->bmiHeader.biCompression;
			m_lAspectRatioX = vih->bmiHeader.biWidth * vih->bmiHeader.biYPelsPerMeter;
			m_lAspectRatioY = vih->bmiHeader.biHeight * vih->bmiHeader.biXPelsPerMeter;
		}
		else if (pmt->formattype == FORMAT_VideoInfo2)
		{
			VIDEOINFOHEADER2* vih2 = (VIDEOINFOHEADER2*)pmt->pbFormat;
			m_pAVCodecCtx->width = vih2->bmiHeader.biWidth;
			m_pAVCodecCtx->height = vih2->bmiHeader.biHeight;
			m_pAVCodecCtx->codec_tag = vih2->bmiHeader.biCompression;
			m_lAspectRatioX = vih2->dwPictAspectRatioX;
			m_lAspectRatioY = vih2->dwPictAspectRatioY;
		}
		m_lWidth = m_pAVCodecCtx->width;
		m_lHeight = m_pAVCodecCtx->height;

		m_pAVCodecCtx->opaque = this;
		m_pAVCodecCtx->codec_tag = g_ffCodecs[nCodec].fourCC;
		m_pAVCodecCtx->workaround_bugs = FF_BUG_AUTODETECT;
		m_pAVCodecCtx->strict_std_compliance = FF_COMPLIANCE_NORMAL;
		m_pAVCodecCtx->error_recognition = FF_ER_CAREFUL;
		m_pAVCodecCtx->idct_algo = FF_IDCT_AUTO;
		m_pAVCodecCtx->error_concealment = FF_EC_GUESS_MVS | FF_EC_DEBLOCK;
		m_pAVCodecCtx->dsp_mask = FF_MM_FORCE | GetCPUFeatures();
		m_pAVCodecCtx->debug_mv = 0;
		//m_pAVCodecCtx->get_format = ff_get_format;
		if (avcodec_open(m_pAVCodecCtx, m_pAVCodec) < 0)
			return VFW_E_INVALID_MEDIA_TYPE;
	}
	return S_OK;
}

HRESULT CVGVideoDecFilter::GetMediaType( int iPosition, __inout CMediaType* pMediaType )
{
	if (iPosition >= 0 && iPosition < _countof(g_vidOutFmts))
	{		
		pMediaType->majortype = MEDIATYPE_Video;
		pMediaType->subtype = *g_vidOutFmts[iPosition].subtype;
		
		pMediaType->formattype = FORMAT_VideoInfo;
		VIDEOINFOHEADER* vih = (VIDEOINFOHEADER*)pMediaType->AllocFormatBuffer(sizeof(VIDEOINFOHEADER));
		memset(vih, 0, sizeof(VIDEOINFOHEADER));
		BITMAPINFOHEADER& bih = vih->bmiHeader;
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biWidth = m_lWidth;
		bih.biHeight = m_lHeight;
		bih.biPlanes = g_vidOutFmts[iPosition].biPlanes;
		bih.biBitCount = g_vidOutFmts[iPosition].biBitCount;
		bih.biCompression = g_vidOutFmts[iPosition].biCompression;
		bih.biSizeImage = bih.biWidth * bih.biHeight * bih.biBitCount >> 3;
		bih.biXPelsPerMeter = bih.biWidth * m_lAspectRatioY;
		bih.biYPelsPerMeter = bih.biHeight * m_lAspectRatioX;

		CMediaType& mtIn = m_pInput->CurrentMediaType();
		VIDEOINFOHEADER* vihIn = (VIDEOINFOHEADER*)mtIn.pbFormat;
		VIDEOINFOHEADER* vihOut = (VIDEOINFOHEADER*)pMediaType->pbFormat;
		vihOut->AvgTimePerFrame = vihIn->AvgTimePerFrame;
		vihOut->dwBitRate = vihIn->dwBitRate;
		vihOut->dwBitErrorRate = vihIn->dwBitErrorRate;
		vihOut->rcSource = vihIn->rcSource;
		vihOut->rcTarget = vihIn->rcTarget;
		return S_OK;
	}
	else
		return VFW_S_NO_MORE_ITEMS;
}

HRESULT CVGVideoDecFilter::CheckTransform( const CMediaType* mtIn, const CMediaType* mtOut )
{
	m_pSwsCtx = sws_getContext(m_lWidth, m_lHeight, m_pAVCodecCtx->pix_fmt, m_lWidth, m_lHeight, PIX_FMT_YUYV422, SWS_FAST_BILINEAR|SWS_CPU_CAPS_MMX, NULL, NULL, NULL);
	if (m_pSwsCtx != NULL)
		return S_OK;
	else
		return VFW_E_TYPE_NOT_ACCEPTED;
}

HRESULT CVGVideoDecFilter::DecideBufferSize( IMemAllocator* pAllocator, __inout ALLOCATOR_PROPERTIES* pprop )
{
	if (!m_pInput->IsConnected())
		return E_UNEXPECTED;

	CMediaType& mtIn = m_pInput->CurrentMediaType();
	pprop->cBuffers = 1;
	if (mtIn.formattype == FORMAT_VideoInfo)
		pprop->cbBuffer = ((VIDEOINFOHEADER*)mtIn.pbFormat)->bmiHeader.biSizeImage;
	else 
		pprop->cbBuffer = ((VIDEOINFOHEADER2*)mtIn.pbFormat)->bmiHeader.biSizeImage;
	pprop->cbAlign = 1;
	pprop->cbPrefix = 0;

	ALLOCATOR_PROPERTIES pActual;
	HRESULT hr;
	if (FAILED(hr = pAllocator->SetProperties(pprop, &pActual)))
		return hr;
	if (pActual.cBuffers < pprop->cBuffers || pActual.cbBuffer < pprop->cbBuffer)
		return E_FAIL;
	else
		return S_OK;
}

HRESULT CVGVideoDecFilter::Transform( IMediaSample* pIn, IMediaSample* pOut )
{
	BYTE* pBufIn, pBufOut;
	BYTE* p
	
	pIn->GetPointer(&pBufIn);
	pOut->GetPointer(&pBufOut);
	avcodec_decode_video2(m_pAVCodecCtx, m_pAVFrame, )
	
	return E_NOTIMPL;
}

int CVGVideoDecFilter::FindCodec( const CMediaType* pmt )
{
	for (UINT i = 0; i < ms_nPinTypesInCount; i++)
	{
		if (pmt->majortype == *ms_PinTypesIn[i].clsMajorType
			&& pmt->subtype == *ms_PinTypesIn[i].clsMinorType)
		{
			return i;
		}
	}
	return -1;
}

WORD CVGVideoDecFilter::GetCPUFeatures( void )
{
	int CPUInfo[4];
	WORD wFlags = 0;
	
	__cpuid(CPUInfo, 1);
	if (CPUInfo[2] & 1)			wFlags |= FF_MM_SSE3;
	if (CPUInfo[2] >> 9 & 1)	wFlags |= FF_MM_SSSE3;
	if (CPUInfo[2] >> 19 & 1)	wFlags |= FF_MM_SSE4;
	if (CPUInfo[2] >> 20 & 1)	wFlags |= FF_MM_SSE42;
	if (CPUInfo[3] >> 23 & 1)	wFlags |= FF_MM_MMX;
	if (CPUInfo[3] >> 25 & 1)	wFlags |= FF_MM_SSE;
	if (CPUInfo[3] >> 26 & 1)	wFlags |= FF_MM_SSE2;
	__cpuid(CPUInfo, 0x80000001);
	if (CPUInfo[3] >> 22 & 1)	wFlags |= FF_MM_MMX2;
	if (CPUInfo[3] >> 30 & 1)	wFlags |= FF_MM_3DNOWEXT;
	if (CPUInfo[3] >> 31 & 1)	wFlags |= FF_MM_3DNOW;
	return wFlags;
}

