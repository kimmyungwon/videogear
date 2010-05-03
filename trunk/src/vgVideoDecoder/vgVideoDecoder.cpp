#include "stdafx.h"
#include "../vgUtils/vgUtils.h"
#include "../vgUtils/vgUUIDs.h"

struct FFMpegHelper
{
	FFMpegHelper(void)
	{
		avcodec_init();
		avcodec_register_all();
	}
};

bool InitFFMpeg(void)
{
	static FFMpegHelper ffmpeg;
	return true;
};

[uuid("28E54277-DA46-447d-A4F2-E15150A2BA64")]
class vgVideoDecoder : public CTransformFilter
{
public:
	static REGPINTYPES ms_inputTypes[];
	static int ms_inputTypeCount;
	
	vgVideoDecoder(LPUNKNOWN pUnk, HRESULT *phr)
		: CTransformFilter(L"VideoGear - Video Decoder", pUnk, __uuidof(this))
	{
		InitFFMpeg();
		if (phr != NULL)
			*phr = S_OK;
	}

	virtual HRESULT Transform(IMediaSample * pIn, IMediaSample *pOut)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT CheckInputType(const CMediaType* mtIn)
	{
		for (int i = 0; i < ms_inputTypeCount; i++)
		{
			if (mtIn->majortype == *ms_inputTypes[i].clsMajorType 
				&& mtIn->subtype == *ms_inputTypes[i].clsMinorType)
				return S_OK;
		}
		return VFW_E_TYPE_NOT_ACCEPTED;
	}

	virtual HRESULT CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut)
	{
		if (FAILED(CheckInputType(mtIn)) && mtOut->majortype != MEDIATYPE_Video)
			return VFW_E_TYPE_NOT_ACCEPTED;
		
		return S_OK;
	}

	virtual HRESULT DecideBufferSize(IMemAllocator * pAllocator, ALLOCATOR_PROPERTIES *pprop)
	{
		return E_NOTIMPL;
	}

	virtual HRESULT GetMediaType(int iPosition, CMediaType *pMediaType)
	{		
		if (iPosition < 0)
			return E_INVALIDARG;
		if (iPosition >= ms_outputTypeCount)
			return VFW_S_NO_MORE_ITEMS;

		pMediaType->majortype = ms_outputTypes[iPosition].clsMajorType;
		pMediaType->subtype = ms_outputTypes[iPosition].clsMinorType;

		BITMAPINFOHEADER bihOut;
		memset(&bihOut, 0, sizeof(bihOut));
		bihOut.biSize = sizeof(bihOut);
		bihOut.biWidth = w;
		bihOut.biHeight = h;
		bihOut.biPlanes = fmts[iPosition/2].biPlanes;
		bihOut.biBitCount = fmts[iPosition/2].biBitCount;
		bihOut.biCompression = fmts[iPosition/2].biCompression;
		bihOut.biSizeImage = w*h*bihOut.biBitCount>>3;

		return S_OK;
	}
};

REGPINTYPES vgVideoDecoder::ms_inputTypes[] = 
{
	{&MEDIATYPE_Video, &MEDIASUBTYPE_DX50},
};

int vgVideoDecoder::ms_inputTypeCount = _countof(vgVideoDecoder::ms_inputTypes);

//////////////////////////////////////////////////////////////////////////

const AMOVIESETUP_MEDIATYPE sudPinTypesOut[] =
{
	{&MEDIATYPE_Video, &MEDIASUBTYPE_NULL},
};

const AMOVIESETUP_PIN sudpPins[] =
{
	{L"Input", FALSE, FALSE, FALSE, FALSE, &CLSID_NULL, NULL, vgVideoDecoder::ms_inputTypeCount, vgVideoDecoder::ms_inputTypes},
	{L"Output", FALSE, TRUE, FALSE, FALSE, &CLSID_NULL, NULL, _countof(sudPinTypesOut), sudPinTypesOut},
};

const AMOVIESETUP_FILTER sudFilters[] =
{
	{&__uuidof(vgVideoDecoder), L"VideoGear - Video Decoder", MERIT_NORMAL, _countof(sudpPins), sudpPins, CLSID_LegacyAmFilterCategory},
};

CFactoryTemplate g_Templates[] =
{
	{sudFilters[0].strName, sudFilters[0].clsID, CreateInstance<vgVideoDecoder>, NULL, &sudFilters[0]},
};

int g_cTemplates = _countof(g_Templates);

STDAPI DllRegisterServer(void)
{	
	return AMovieDllRegisterServer2(TRUE);
}

STDAPI DllUnregisterServer()
{
	return AMovieDllRegisterServer2(FALSE);
}