#include "StdAfx.h"
#include "VideoGear.h"
#include "FilterGraph.h"

CFilterGraph::CFilterGraph(void)
{
}

CFilterGraph::~CFilterGraph(void)
{
	ClearGraph();
	m_graph = NULL;
}

HRESULT CFilterGraph::Initialize( void )
{
	return m_graph.CoCreateInstance(CLSID_FilterGraph);
}

HRESULT CFilterGraph::RenderFile( LPCWSTR fileName )
{
	CSourceFilter* source;
	CComPtr<IBaseFilter> filter;
	CComPtr<IFileSourceFilter> fileSrc;
	HRESULT hr;
	
	if (m_graph == NULL)
		return E_UNEXPECTED;
	RIF(ClearGraph());
	RIF(InitRenderers());
	if (SUCCEEDED(g_filterMan.EnumMatchingSource(fileName, source))
		&& SUCCEEDED(source->CreateObject(&filter))
		&& SUCCEEDED(filter.QueryInterface(&fileSrc))
		&& SUCCEEDED(fileSrc->Load(fileName, NULL))
		&& SUCCEEDED(m_graph->AddFilter(filter, source->GetName())))
	{
		if (SUCCEEDED(hr = Render(filter)))
			return hr;
		NukeDownstream(filter);
		m_graph->RemoveFilter(filter);
	}
	return VFW_E_CANNOT_RENDER;
}

HRESULT CFilterGraph::ClearGraph( void )
{	
	std::vector<IBaseFilter*> filters;
	
	BeginEnumFilters(m_graph, enumFilters, filter)
	{
		filters.push_back(filter);
	}
	EndEnumFilters

	for (std::vector<IBaseFilter*>::iterator it = filters.begin(); it != filters.end(); it++)
	{
		NukeDownstream(*it);
		m_graph->RemoveFilter(*it);
	}

	return S_OK;
}

HRESULT CFilterGraph::InitRenderers( void )
{
	CComPtr<IBaseFilter> videoRenderer, audioRenderer;

	RIF(videoRenderer.CoCreateInstance(CLSID_VideoRendererDefault));
	RIF(m_graph->AddFilter(videoRenderer, L"Default Video Renderer"));
	RIF(audioRenderer.CoCreateInstance(CLSID_DSoundRender));
	RIF(m_graph->AddFilter(audioRenderer, L"Default DirectSound Renderer"));
	return S_OK;
}

HRESULT CFilterGraph::Render( IBaseFilter* filter )
{
	ASSERT(filter != NULL);

	int totalCount = 0, renderedCount = 0;

	XTRACE(_T("正在渲染[%s]\n"), GetFilterName(filter));
	BeginEnumPins(filter, enumPins, outPin)
	{
		bool rendered = false;
		std::vector<IBaseFilter*> cachedFilters;
		
		if (IsPinConnected(outPin) || GetPinDir(outPin) != PINDIR_OUTPUT)
			continue;
		totalCount++;

		XTRACE(_T("正在渲染[%s(%s)]\n"), GetFilterName(filter), GetPinName(outPin));
		if (SUCCEEDED(Render(outPin)))
		{
			XTRACE(_T("渲染[%s(%s)]成功\n"), GetFilterName(filter), GetPinName(outPin));
			renderedCount++;
		}
		else
		{
			XTRACE(_T("渲染[%s(%s)]失败\n"), GetFilterName(filter), GetPinName(outPin));
			NukeDownstream(outPin);
		}
	}
	EndEnumPins

	if (totalCount > 0)
	{
		if (renderedCount == totalCount) 
			return S_OK;
		else if (renderedCount > 0)
			return S_FALSE;
		else
		{
			XTRACE(_T("渲染[%s]失败\n"), GetFilterName(filter));
			return VFW_E_CANNOT_RENDER;
		}
	}
	else
		return S_OK;
}

HRESULT CFilterGraph::Render( IPin* pin )
{
	if (pin == NULL || IsPinConnected(pin) || GetPinDir(pin) != PINDIR_OUTPUT)
		return E_INVALIDARG;

	/* 1、使用已有的滤镜 */
	{
		CInterfaceList<IBaseFilter> cachedFilters;

		BeginEnumFilters(m_graph, enumCachedFilters, cachedFilter)
		{
			if (!IsEqualCLSID(GetCLSID(GetFilterFromPin(pin)), GetCLSID(cachedFilter)))
				cachedFilters.AddTail(cachedFilter);
		}
		EndEnumFilters

		POSITION pos = cachedFilters.GetHeadPosition();
		while (pos != NULL)
		{
			IBaseFilter* inFilter = cachedFilters.GetNext(pos);
			XTRACE(_T("尝试渲染[%s(%s)]->[%s]\n"), GetFilterName(GetFilterFromPin(pin)), GetPinName(pin), GetFilterName(inFilter));
			if (SUCCEEDED(ConnectDirect(pin, inFilter, NULL)))
			{	
				HRESULT hr = Render(inFilter);
				if (SUCCEEDED(hr))
					return hr;
				m_graph->Disconnect(pin);
			}
		}
	}

	/* 2、使用内部滤镜 */
	{
		CAtlArray<GUID> types;
		std::list<CFilter*> filters;

		ExtractMediaTypes(pin, types);
		if (SUCCEEDED(g_filterMan.EnumMatchingFilters(false, types.GetData(), types.GetCount(), filters)))
		{
			for (std::list<CFilter*>::iterator it = filters.begin(); it != filters.end(); it++)
			{
				CComPtr<IBaseFilter> inFilter;

				if (FAILED((*it)->CreateObject(&inFilter)))
					continue;
				XTRACE(_T("尝试渲染[%s(%s)]->[%s]\n"), GetFilterName(GetFilterFromPin(pin)), GetPinName(pin), (*it)->GetName());
				m_graph->AddFilter(inFilter, (*it)->GetName());
				if (SUCCEEDED(ConnectDirect(pin, inFilter, NULL)))
				{
					HRESULT hr = Render(inFilter);
					if (SUCCEEDED(hr))
						return hr;
					m_graph->Disconnect(pin);
				}
				NukeDownstream(inFilter);
				m_graph->RemoveFilter(inFilter);
			}
		}
	}

	return VFW_E_CANNOT_RENDER;
}

HRESULT CFilterGraph::ConnectDirect( IPin* outPin, IBaseFilter* inFilter, AM_MEDIA_TYPE* pmt )
{
	BeginEnumPins(inFilter, enumPins, inPin)
	{
		if (IsPinConnected(inPin) || GetPinDir(inPin) != PINDIR_INPUT)
			continue;	
		if (SUCCEEDED(m_graph->ConnectDirect(outPin, inPin, pmt)))
			return S_OK;
	}
	EndEnumPins
	return VFW_E_CANNOT_CONNECT;
}

HRESULT CFilterGraph::NukeDownstream( IUnknown* unk )
{
	if(CComQIPtr<IBaseFilter> filter = unk)
	{
		BeginEnumPins(filter, enumPins, pin)
		{
			NukeDownstream(pin);
		}
		EndEnumPins
	}
	else if(CComQIPtr<IPin> pin = unk)
	{
		CComPtr<IPin> pinTo;

		if(GetPinDir(pin) == PINDIR_OUTPUT
			&& SUCCEEDED(pin->ConnectedTo(&pinTo)) && pinTo != NULL)
		{
			if(CComPtr<IBaseFilter> filter = GetFilterFromPin(pinTo))
			{
				NukeDownstream(filter);
				m_graph->Disconnect(pinTo);
				m_graph->Disconnect(pin);
				m_graph->RemoveFilter(filter);
			}
		}
	}
	else
	{
		return E_INVALIDARG;
	}

	return S_OK;
}
