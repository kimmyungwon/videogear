#pragma once

// {26F1AEB9-E1C0-452c-80AD-0560C668F355}
DEFINE_GUID(IID_IVGEnumFilter, 0x26f1aeb9, 0xe1c0, 0x452c, 0x80, 0xad, 0x5, 0x60, 0xc6, 0x68, 0xf3, 0x55);

DECLARE_INTERFACE_IID_(IVGEnumFilter, IUnknown, "26F1AEB9-E1C0-452c-80AD-0560C668F355")
{
	STDMETHOD(Clone)(IVGEnumFilter **ppEnum);
	STDMETHOD(Reset)(void);
	STDMETHOD(Next)(IBaseFilter **ppvObj);
};

class CVGEnumFilter : public IVGEnumFilter
{
public:
	CVGEnumFilter(void);
	~CVGEnumFilter(void);
};
