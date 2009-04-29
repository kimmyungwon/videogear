#pragma once

#include "FilterManager.h"

class CFilterGraph
{
public:
	CFilterGraph(void);
	virtual ~CFilterGraph(void);
	HRESULT Initialize(void);
	HRESULT RenderFile(LPCWSTR fileName);
protected:
	HRESULT NukeDownstream(IUnknown* unk);
	HRESULT ClearGraph(void);
	HRESULT InitRenderers(void);
	HRESULT Render(IBaseFilter* filter);
	HRESULT Render(IPin* pin);
	HRESULT ConnectDirect(IPin* outPin, IBaseFilter* inFilter, AM_MEDIA_TYPE* pmt);
private:
	CComPtr<IGraphBuilder>	m_graph;
};
