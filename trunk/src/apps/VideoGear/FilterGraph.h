#pragma once

#include "FilterManager.h"

class CFilterGraph
{
public:
	CFilterGraph(void);
	virtual ~CFilterGraph(void);
	void Initialize(void);
protected:
	void ClearGraph(void);
private:
	CComPtr<IGraphBuilder>	m_graph;
};
