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
	CHECK(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (LPVOID*)&m_pGB));
	return S_OK;
}