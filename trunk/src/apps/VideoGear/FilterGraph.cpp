#include "StdAfx.h"
#include "FilterGraph.h"

CFilterGraph::CFilterGraph(void)
{
}

CFilterGraph::~CFilterGraph(void)
{
	m_graph = NULL;
}

void CFilterGraph::Initialize( void )
{
	m_graph.CoCreateInstance(CLSID_FilterGraph);
}

void CFilterGraph::ClearGraph( void )
{
	BeginEnumFilters(m_graph, enumFilters, filter)
	{
		BeginEnumPins(filter, enumPins, pin)
		{
			m_graph->Disconnect(pin);
		}
		EndEnumPins
		m_graph->RemoveFilter(filter);
	}
	EndEnumFilters
}