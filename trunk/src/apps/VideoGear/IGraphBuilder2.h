#pragma once

// {BF22C03E-D98F-4f89-929D-8D9548029E3D}
DEFINE_GUID(IID_IGraphBuilder2, 
			0xbf22c03e, 0xd98f, 0x4f89, 0x92, 0x9d, 0x8d, 0x95, 0x48, 0x2, 0x9e, 0x3d);

#undef INTERFACE
#define INTERFACE IGraphBuilder2

DECLARE_INTERFACE_(IGraphBuilder2, IFilterGraph2)
{
	STDMETHOD(IsPinConnected)(THIS_ IPin *ppin) PURE;
	STDMETHOD(IsPinDirection)(THIS_ IPin *ppin, PIN_DIRECTION dir) PURE;
};