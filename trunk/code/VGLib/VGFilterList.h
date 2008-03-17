#pragma once

// {6145535B-A312-438c-860C-7558ED1D6EB7}
DEFINE_GUID(IID_IVGFilter, 0x6145535b, 0xa312, 0x438c, 0x86, 0xc, 0x75, 0x58, 0xed, 0x1d, 0x6e, 0xb7);

DECLARE_INTERFACE_IID_(IVGFilter, IUnknown, "6145535B-A312-438c-860C-7558ED1D6EB7")
{
	BEGIN_INTERFACE
		/* IUnknown  */
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject) = 0;
		virtual ULONG STDMETHODCALLTYPE AddRef(void) = 0;
		virtual ULONG STDMETHODCALLTYPE Release(void) = 0;
		/* IVGFilter */
		virtual HRESULT STDMETHODCALLTYPE CreateInstance(IBaseFilter **ppvObj) = 0;
		virtual CLSID STDMETHODCALLTYPE GetCLSID(void) = 0;
		virtual DWORD STDMETHODCALLTYPE GetMerit(void) = 0;
		virtual LPCWSTR STDMETHODCALLTYPE GetName(void) = 0;
		virtual DWORD STDMETHODCALLTYPE GetPinCount(void) = 0;
		virtual HRESULT STDMETHODCALLTYPE GetPinInfo(const REGFILTERPINS *pInfo) = 0;
	END_INTERFACE
};

#if defined(_COM_SMARTPTR_TYPEDEF)
_COM_SMARTPTR_TYPEDEF(IVGFilter, __uuidof(IVGFilter));
#endif

using namespace std;

class CVGFilter : public IVGFilter
{
private:
	volatile LONG m_cRef;
	CFactoryTemplate m_Templ;
public:
	CVGFilter(const CFactoryTemplate &Templ);
	virtual ~CVGFilter(void);
	/* IUnknown  */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);	
	/* IVGFilter */
	virtual HRESULT STDMETHODCALLTYPE CreateInstance(IBaseFilter **ppvObj);
	virtual CLSID STDMETHODCALLTYPE GetCLSID(void);
	virtual DWORD STDMETHODCALLTYPE GetMerit(void);
	virtual LPCWSTR STDMETHODCALLTYPE GetName(void);
	virtual DWORD STDMETHODCALLTYPE GetPinCount(void);
	virtual HRESULT STDMETHODCALLTYPE GetPinInfo(const REGFILTERPINS *pInfo);
};

class CVGFilterList : public IEnumUnknown
{
private:
	volatile LONG m_cRef;
	vector<IVGFilterPtr> m_filters;
	UINT m_nPos;
public:
	CVGFilterList(void);
	virtual ~CVGFilterList(void);
	void Add(CFactoryTemplate *pTemplates, UINT nCount);
	void Clear(void);
	/* IUnknown  */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);
	/* IEnumUnknown */
	virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, IUnknown **rgelt, ULONG *pceltFetched);
	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt);
	virtual HRESULT STDMETHODCALLTYPE Reset(void);
	virtual HRESULT STDMETHODCALLTYPE Clone(__RPC__deref_out_opt IEnumUnknown **ppenum);
};
