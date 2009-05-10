#pragma once

// {E7B6C506-7A32-40ce-A8C2-F88DF1FF50AA}
DEFINE_GUID(IID_IGraphBuilder2, 
			0xe7b6c506, 0x7a32, 0x40ce, 0xa8, 0xc2, 0xf8, 0x8d, 0xf1, 0xff, 0x50, 0xaa);

DECLARE_INTERFACE_IID_(IGraphBuilder2, IFilterGraph2, "{E7B6C506-7A32-40ce-A8C2-F88DF1FF50AA}")
{
	virtual HRESULT STDMETHODCALLTYPE NukeDownstream(IPin *ppinOut) = 0;
	virtual HRESULT STDMETHODCALLTYPE ClearGraph(void) = 0;
	virtual HRESULT STDMETHODCALLTYPE RenderFilter(IBaseFilter *pFilter) = 0;
	virtual HRESULT STDMETHODCALLTYPE ConnectDirectEx(IPin *ppinOut, IBaseFilter *pFilter, const AM_MEDIA_TYPE *pmt) = 0;
};