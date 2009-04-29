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
		return Render(filter);
	}
	else
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
		IBaseFilter* filter = *it;
		BeginEnumPins(filter, enumPins, pin)
		{
			RIF(m_graph->Disconnect(pin));
		}
		EndEnumPins
		RIF(m_graph->RemoveFilter(filter));
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

		/* 优先使用已缓存的滤镜 */

		BeginEnumFilters(m_graph, enumCachedFilters, cachedFilter)
		{
			if (!IsEqualCLSID(GetCLSID(filter), GetCLSID(cachedFilter)))
				cachedFilters.push_back(cachedFilter);
		}
		EndEnumFilters

		for (std::vector<IBaseFilter*>::iterator it = cachedFilters.begin(); it != cachedFilters.end(); it++)
		{
			XTRACE(_T("尝试渲染[%s(%s)]->[%s]\n"), GetFilterName(filter), GetPinName(outPin), GetFilterName(*it));
			if (SUCCEEDED(ConnectDirect(outPin, *it, NULL)) && SUCCEEDED(Render(*it)))
			{
				XTRACE(_T("成功渲染[%s(%s)]->[%s]\n"), GetFilterName(filter), GetPinName(outPin), GetFilterName(*it));
				rendered = true;
				renderedCount++;
				break;
			}
		}
		cachedFilters.clear();
		if (rendered)
			continue;

		/* 使用内部滤镜 */

		BeginEnumMediaTypes(outPin, enumTypes, pmt)
		{
			std::list<CFilter*> filters;
			
			if (g_filterMan.EnumMatchingFilters(false, pmt, filters) != S_OK)
				continue;

			for (std::list<CFilter*>::iterator it = filters.begin(); it != filters.end(); it++)
			{
				CComPtr<IBaseFilter> inFilter;
				
				if (FAILED((*it)->CreateObject(&inFilter)))
					continue;

				XTRACE(_T("尝试渲染[%s(%s)]->[%s]\n"), GetFilterName(filter), GetPinName(outPin), (*it)->GetName());
				m_graph->AddFilter(inFilter, (*it)->GetName());
				if (SUCCEEDED(ConnectDirect(outPin, inFilter, pmt)) && SUCCEEDED(Render(inFilter)))
				{
					XTRACE(_T("成功渲染[%s(%s)]->[%s]\n"), GetFilterName(filter), GetPinName(outPin), (*it)->GetName());
					rendered = true;
					renderedCount++;
					break;
				}
				else
				{
					m_graph->RemoveFilter(inFilter);
				}
			}
			if (rendered)
				break;
		}
		EndEnumMediaTypes(pmt)
		if (rendered)
			continue;

		XTRACE(_T("渲染[%s(%s)]失败\n"), GetFilterName(filter), GetPinName(outPin));
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