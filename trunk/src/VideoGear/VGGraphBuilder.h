#pragma once

#include "VGFilters.h"

using namespace std;

class VGGraphBuilder
{
public:
	VGGraphBuilder(void);
	~VGGraphBuilder(void);
	HRESULT Initialize(void);
	HRESULT ClearGraph(void);
	HRESULT RenderFile(LPCWSTR _FileName);
protected:
	HRESULT ConnectDirect(CComPtr<IPin> _OutPin, CComPtr<IBaseFilter> _InFilter, const AM_MEDIA_TYPE* _MediaType);
	HRESULT FindMatchingFilters(const AM_MEDIA_TYPE* _InType, VGMatchingFilters& _Results);
	HRESULT Render(CComPtr<IPin> _Pin);
	HRESULT Render(CComPtr<IBaseFilter> _Filter);
private:
	PIN_DIRECTION GetPinDir(CComPtr<IPin> _Pin) const;
	bool IsPinConnected(CComPtr<IPin> _Pin) const;
private:
	CComPtr<IFilterMapper2>	m_pFM;
	CComPtr<IGraphBuilder>	m_pGB;
	CComPtr<IMediaControl>	m_pMC;
};
