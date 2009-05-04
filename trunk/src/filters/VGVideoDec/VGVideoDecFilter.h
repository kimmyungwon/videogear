#pragma once

// {920F62E9-9DB9-414a-94B6-0E2119A3DAFF}
DEFINE_GUID(CLSID_VGVideoDecFilter, 
			0x920f62e9, 0x9db9, 0x414a, 0x94, 0xb6, 0xe, 0x21, 0x19, 0xa3, 0xda, 0xff);

class CVGVideoDecFilter
	: public CVideoTransformFilter
{
public:
	static const AMOVIESETUP_MEDIATYPE ms_PinTypesIn[];
	static const UINT ms_nPinTypesInCount;
	static const AMOVIESETUP_MEDIATYPE ms_PinTypesOut[];
	static const UINT ms_nPinTypesOutCount;
	static CUnknown* CALLBACK CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr);
public:
	CVGVideoDecFilter(__inout_opt LPUNKNOWN pUnk);
	virtual ~CVGVideoDecFilter(void);
	/* CTransformFilter */
	virtual HRESULT CheckInputType(const CMediaType* mtIn);
	virtual HRESULT SetMediaType(PIN_DIRECTION direction, const CMediaType* pmt);
	virtual HRESULT GetMediaType(int iPosition, __inout CMediaType* pMediaType); 
	virtual HRESULT CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut);
	virtual HRESULT DecideBufferSize(IMemAllocator* pAllocator, __inout ALLOCATOR_PROPERTIES* pprop);
	virtual HRESULT Transform(IMediaSample* pIn, IMediaSample* pOut);
protected:
	int FindCodec(const CMediaType* pmt);
	WORD GetCPUFeatures(void);
private:
	LONG	m_lWidth, m_lHeight, m_lAspectRatioX, m_lAspectRatioY;
	/* ffmpeg */
	AVCodec*		m_pAVCodec;
	AVCodecContext*	m_pAVCodecCtx;
	AVFrame*		m_pAVFrame;
	SwsContext*		m_pSwsCtx;
};
