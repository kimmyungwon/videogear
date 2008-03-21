#pragma once

#include <functional>

using namespace std;

#define FAILED_RETURN(expr)	if(FAILED(hr = (expr))) return hr

//////////////////////////////////////////////////////////////////////////

__inline CString GetErrorMsg(HRESULT hr)
{
	TCHAR szMsg[MAX_ERROR_TEXT_LEN];
	AMGetErrorText(hr, szMsg, MAX_ERROR_TEXT_LEN);
	return szMsg;
}

//////////////////////////////////////////////////////////////////////////

__inline bool MatchGUID(REFGUID a, REFGUID b)
{
	return InlineIsEqualGUID(a, b) || InlineIsEqualGUID(a, GUID_NULL) || InlineIsEqualGUID(b, GUID_NULL);
}

struct CVGMediaType
{
	GUID clsMajor, clsMinor;

	CVGMediaType(REFGUID major, REFGUID minor): clsMajor(major), clsMinor(minor)	{}
	CVGMediaType(const REGPINTYPES& r): clsMajor(*r.clsMajorType), clsMinor(*r.clsMinorType)	{}

	friend bool operator==(const CVGMediaType& a, const CVGMediaType& b)
	{
		return InlineIsEqualGUID(a.clsMajor, b.clsMajor) && InlineIsEqualGUID(a.clsMinor, b.clsMinor);
	}
};

//////////////////////////////////////////////////////////////////////////

template<typename BASE_INTERFACE, REFIID iid >
//BASE_INTERFACE should derive from IUnknown
class CVGUnknownImpl : public BASE_INTERFACE
{
private:
	volatile LONG m_cRef;
public:
	CVGUnknownImpl(): m_cRef(0) {}
	virtual ~CVGUnknownImpl()	{}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID refiid, void **ppv)
	{
		if(refiid  == IID_IUnknown || refiid == iid)
			*ppv = static_cast<BASE_INTERFACE*>(this);
		// I have to typecast it to BASE_INTERFACE*, I know that
		else
			return E_NOINTERFACE;

		static_cast<IUnknown*>(this)->AddRef();

		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}

	virtual ULONG STDMETHODCALLTYPE Release()
	{
		LONG lRef = 0;

		if(0 == (lRef = InterlockedDecrement(&m_cRef)))
			delete this;
		return lRef;
	}
};

//////////////////////////////////////////////////////////////////////////

struct CVGFilter
{
	CLSID clsID;
	CStringW strName;
	DWORD dwMerit;
	UINT nPins;
	LPFNNewCOMObject lpfnNew;

	CVGFilter(void): clsID(GUID_NULL), strName(L""), dwMerit(0), lpfnNew(NULL)	{}

	CVGFilter(REFCLSID _clsID, LPCWSTR _Name, DWORD _Merit, UINT _Pins, LPFNNewCOMObject _lpfnNew = NULL)
		:clsID(_clsID), strName(_Name), dwMerit(_Merit), nPins(_Pins), lpfnNew(_lpfnNew)	{}

	CVGFilter(const CFactoryTemplate* _Templ)
	{
		CVGFilter(*_Templ->m_ClsID, _Templ->m_Name, _Templ->m_pAMovieSetup_Filter->dwMerit, 
			_Templ->m_pAMovieSetup_Filter->nPins, _Templ->m_lpfnNew);
	}

	HRESULT CreateInstance(LPUNKNOWN pUnkOuter, IBaseFilter** ppBF) const
	{
		CheckPointer(ppBF, E_POINTER);

		HRESULT hr;
		if (lpfnNew != NULL)
		{
			*ppBF = (CBaseFilter*)lpfnNew(pUnkOuter, &hr);
			(*ppBF)->AddRef();
			return hr;
		}
		else
		{
			FAILED_RETURN(CoCreateInstance(clsID, pUnkOuter, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (LPVOID*)ppBF));
			(*ppBF)->AddRef();
			return S_OK;
		}
	}

	CVGFilter& operator=(const CVGFilter& r)
	{
		clsID = r.clsID;
		strName = r.strName;
		dwMerit = r.dwMerit;
		lpfnNew = r.lpfnNew;
		return *this;
	}

	friend bool operator==(const CVGFilter& a, const CVGFilter& b)
	{
		return InlineIsEqualGUID(a.clsID, b.clsID) == TRUE;
	}
};

struct VGFilterComp_InternalFirst : public binary_function <CVGFilter, CVGFilter, bool> 
{
	bool operator()(const CVGFilter& _Left, const CVGFilter& _Right) const
	{
		if (_Left.lpfnNew != NULL && _Right.lpfnNew == NULL)
			return true;
		else if (_Left.lpfnNew == NULL && _Right.lpfnNew != NULL)
			return false;
		else
			return _Left.dwMerit > _Right.dwMerit;
	}
};

struct VGFilterComp : public binary_function <CVGFilter, CVGFilter, bool> 
{
	bool operator()(const CVGFilter& _Left, const CVGFilter& _Right) const
	{
		return _Left.dwMerit > _Right.dwMerit;
	}
};

template<typename CompT>
class CVGFiltersT : public set<CVGFilter, CompT>	{};

typedef CVGFiltersT<VGFilterComp_InternalFirst> CVGFilters;	// 按Merit排列，内部滤镜优先
typedef CVGFiltersT<VGFilterComp> CVGFiltersNIF;	// 只按Merit排列
