#pragma once

// {8BAC0406-06F0-4232-B6BB-87F92DBC7945}
DEFINE_GUID(IID_IVGFilterManager, 0x8bac0406, 0x6f0, 0x4232, 0xb6, 0xbb, 0x87, 0xf9, 0x2d, 0xbc, 0x79, 0x45);
// {14868E59-6444-4bca-9004-6C697E4E66ED}
DEFINE_GUID(IID_IVGPlayer, 0x14868e59, 0x6444, 0x4bca, 0x90, 0x4, 0x6c, 0x69, 0x7e, 0x4e, 0x66, 0xed);

#undef  INTERFACE
#define INTERFACE   IVGFilterManager

DECLARE_INTERFACE_IID_(IVGFilterManager, IUnknown, "8BAC0406-06F0-4232-B6BB-87F92DBC7945")
{
	// *** IUnknown methods ***
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject) = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) = 0;
	// *** IVGFilterManager methods ***
	virtual HRESULT STDMETHODCALLTYPE EnumMatchingFilters(IEnumGUID **ppEnum, BOOL bExactMatch, DWORD dwMerit, 
														  CLSID clsInMaj, CLSID clsInSub) = 0;
	virtual HRESULT STDMETHODCALLTYPE Initialize(void) = 0;
	virtual HRESULT STDMETHODCALLTYPE RenderFile(LPCWSTR lpszFileName) = 0;
};

#undef  INTERFACE
#define INTERFACE   IVGPlayer

DECLARE_INTERFACE_IID_(IVGPlayer, IUnknown, "14868E59-6444-4bca-9004-6C697E4E66ED")
{
	// *** IUnknown methods ***
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject) = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef(void) = 0;
	virtual ULONG STDMETHODCALLTYPE Release(void) = 0;
	// *** IVGPlayer methods ***
	virtual HRESULT STDMETHODCALLTYPE Initialize(void) = 0;
};


