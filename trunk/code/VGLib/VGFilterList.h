#pragma once

// {8E4FADF4-A761-49cd-B4AF-99FD531660F2}
DEFINE_GUID(IID_IVGFilterList, 0x8e4fadf4, 0xa761, 0x49cd, 0xb4, 0xaf, 0x99, 0xfd, 0x53, 0x16, 0x60, 0xf2);


DECLARE_INTERFACE_IID_(IVGFilterList, IUnknown, "8E4FADF4-A761-49cd-B4AF-99FD531660F2")
{
	BEGIN_INTERFACE
		/* IUnknown  */
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject) = 0;
		virtual ULONG STDMETHODCALLTYPE AddRef(void) = 0;
		virtual ULONG STDMETHODCALLTYPE Release(void) = 0;
		/* IVGFilterList */
		virtual HRESULT STDMETHODCALLTYPE Add(IBaseFilter *pBF) = 0;
		virtual HRESULT STDMETHODCALLTYPE Clear(void) = 0;
		virtual HRESULT STDMETHODCALLTYPE Delete(DWORD nIndex) = 0;
		virtual HRESULT STDMETHODCALLTYPE Get(DWORD nIndex, IBaseFilter** ppBF) = 0;
		virtual DWORD STDMETHODCALLTYPE GetCount(void) = 0;
	END_INTERFACE
};

#if defined(_COM_SMARTPTR_TYPEDEF)
_COM_SMARTPTR_TYPEDEF(IVGFilterList, __uuidof(IVGFilterList));
#endif

using namespace std;

class CVGFilterList : public IVGFilterList
{
private:
	volatile LONG m_cRef;
	vector<IBaseFilter*> m_filters;
public:
	CVGFilterList(void);
	virtual ~CVGFilterList(void);
	/* IUnknown  */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef(void);
	virtual ULONG STDMETHODCALLTYPE Release(void);
	/* IVGFilterList */
	virtual HRESULT STDMETHODCALLTYPE Add(IBaseFilter *pBF);
	virtual HRESULT STDMETHODCALLTYPE Clear(void);
	virtual HRESULT STDMETHODCALLTYPE Delete(DWORD nIndex);
	virtual HRESULT STDMETHODCALLTYPE Get(DWORD nIndex, IBaseFilter** ppBF);
	virtual DWORD STDMETHODCALLTYPE GetCount(void);
};