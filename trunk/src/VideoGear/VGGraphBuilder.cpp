#include "StdAfx.h"
#include "VGGraphBuilder.h"

VGGraphBuilder::VGGraphBuilder(void)
{
	
}

VGGraphBuilder::~VGGraphBuilder(void)
{
}

HRESULT VGGraphBuilder::Initialize( void )
{
	HRESULT hr;
	
	m_pGB.Release();
	CHECK(CoCreateInstance(CLSID_FilterMapper2, NULL, CLSCTX_INPROC_SERVER, IID_IFilterMapper2, (LPVOID*)&m_pFM));
	CHECK(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (LPVOID*)&m_pGB));
	CHECK(m_pGB->QueryInterface(IID_IMediaControl, (LPVOID*)&m_pMC));
	return S_OK;
}

HRESULT VGGraphBuilder::ClearGraph( void )
{
	HRESULT hr;

	if (m_pMC == NULL)
		return E_POINTER;
	CHECK(m_pMC->Stop());
	BeginEnumFilters(m_pGB, pEnumFilters, pFilter)
		BeginEnumPins(pFilter, pEnumPins, pPin)
			CHECK(m_pGB->Disconnect(pPin));
		EndEnumPins
		CHECK(m_pGB->RemoveFilter(pFilter));
	EndEnumFilters
	return S_OK;
}

HRESULT VGGraphBuilder::RenderFile( LPCWSTR _FileName )
{
	HRESULT hr;
	wchar_t szTitle[_MAX_FNAME];
	CComPtr<IBaseFilter> pSource;

	if (m_pGB == NULL )
		return E_POINTER;
	// 清理
	CHECK(ClearGraph());
	// 用源滤镜载入文件
	if (!PathFileExistsW(_FileName))
		return VFW_E_NOT_FOUND;
	_wsplitpath_s(_FileName, NULL, 0, NULL, 0, szTitle, _MAX_FNAME, NULL, 0);
	CHECK(m_pGB->AddSourceFilter(_FileName, szTitle, &pSource));
	// 渲染源滤镜
	return Render(pSource);
}

HRESULT VGGraphBuilder::ConnectDirect( CComPtr<IPin> _OutPin, CComPtr<IBaseFilter> _InFilter, const AM_MEDIA_TYPE* _MediaType )
{
	ASSERT(GetPinDir(_OutPin) == PINDIR_OUTPUT && !IsPinConnected(_OutPin));
	BeginEnumPins(_InFilter, pEnumPins, pInPin)
		if (GetPinDir(pInPin) != PINDIR_INPUT || IsPinConnected(pInPin))
			continue;
		if (m_pGB->ConnectDirect(_OutPin, pInPin, _MediaType) == S_OK)
			return S_OK;
	EndEnumPins
	return VFW_E_CANNOT_CONNECT;
}

HRESULT VGGraphBuilder::FindMatchingFilters( const AM_MEDIA_TYPE* _InType, VGMatchingFilters& _Results )
{
	UINT nTotal = 0;
	GUID types[2];
	CComPtr<IEnumMoniker> pEnumSF;
	
	// 首先在GraphBuilder中查找
	ASSERT(m_pGB != NULL);
	BeginEnumFilters(m_pGB, pEnumFilters, pFilter)
		BeginEnumPins(pFilter, pEnumPins, pInPin)
			if (GetPinDir(pInPin) != PINDIR_INPUT || IsPinConnected(pInPin))
				continue;
			if (pInPin->QueryAccept(_InType) == S_OK)
			{
				_Results.push_back(VGFilter(pFilter));
				nTotal++;
			}
		EndEnumFilters
	EndEnumFilters
	// 其次在内部滤镜中查找
	g_internalFilters.FindMatchingFilters(_InType, _Results);
	// 最后在系统注册滤镜库中查找
	ASSERT(m_pFM != NULL);
	types[0] = _InType->majortype;
	types[1] = _InType->subtype;
	if (m_pFM->EnumMatchingFilters(&pEnumSF, 0, FALSE, MERIT_NORMAL, TRUE, 2, types, NULL, NULL, FALSE, FALSE, 
		0, NULL, NULL, NULL) == S_OK)
	{
		for (CComPtr<IMoniker> pMoniker; pEnumSF->Next(1, &pMoniker, NULL) == S_OK; pMoniker.Release())
		{
			_Results.push_back(VGFilter(pMoniker));
		}
	}

	return nTotal > 0 ? S_OK : E_FAIL;
}

HRESULT VGGraphBuilder::Render( CComPtr<IPin> _Pin )
{	
	bool bRendered = false;
	VGMatchingFilters matchings;
	
	ASSERT(GetPinDir(_Pin) == PINDIR_OUTPUT && !IsPinConnected(_Pin));
	BeginEnumMediaTypes(_Pin, pEnumMT, pMT)
		if (FindMatchingFilters(pMT, matchings) == S_OK)
		{
			for (VGMatchingFilters::const_iterator it = matchings.begin(); it != matchings.end(); ++it)
			{
				if (!it->bInGB && m_pGB->AddFilter(it->pFilter, it->strName) != S_OK)
					continue;
				if (ConnectDirect(_Pin, it->pFilter, pMT) == S_OK)
				{
					bRendered = true;
					break;
				}
				else
				{
					m_pGB->RemoveFilter(it->pFilter);
					continue;
				}
			}
		}
	EndEnumMediaTypes(pMT)
	return bRendered ? S_OK : VFW_E_CANNOT_RENDER;
}

HRESULT VGGraphBuilder::Render( CComPtr<IBaseFilter> _Filter )
{
	UINT nTotal = 0, nRendered = 0;
	
	if (_Filter == NULL)
		return E_POINTER;
	BeginEnumPins(_Filter, pEnumPins, pOutPin)
		if (GetPinDir(pOutPin) != PINDIR_OUTPUT || IsPinConnected(pOutPin))
			continue;
		nTotal++;
		if (Render(pOutPin) == S_OK)
			nRendered++;
	EndEnumPins
	if (nRendered > 0)
		return nTotal == nRendered ? S_OK : S_FALSE;
	else
		return nTotal == 0 ? S_OK : VFW_E_CANNOT_RENDER;
}

PIN_DIRECTION VGGraphBuilder::GetPinDir( CComPtr<IPin> _Pin ) const
{
	PIN_DIRECTION pinDir;
	
	ASSERT(_Pin != NULL);
	_Pin->QueryDirection(&pinDir);
	return pinDir;
}

bool VGGraphBuilder::IsPinConnected( CComPtr<IPin> _Pin ) const
{
	CComPtr<IPin> pConnectedPin;
	
	ASSERT(_Pin != NULL);
	_Pin->ConnectedTo(&pConnectedPin);
	return (pConnectedPin == NULL);
}

