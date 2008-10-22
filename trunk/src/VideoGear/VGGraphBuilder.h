#pragma once

#include "VGFilters.h"

class VGGraphBuilder
{
public:
	VGGraphBuilder(void);
	~VGGraphBuilder(void);
	HRESULT Initialize(void);
private:
	CComPtr<IGraphBuilder> m_pGB;
};
