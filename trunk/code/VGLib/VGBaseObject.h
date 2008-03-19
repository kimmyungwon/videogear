#pragma once

struct CGUID : public GUID
{
	CGUID(const GUID& r): GUID(r)	{}
	
	friend bool operator==(const CGUID& a, const CGUID& b)
	{
		return IsEqualGUID(a, b) == TRUE;
	}
};

__inline bool MatchGUID(REFGUID a, REFGUID b)
{
	return InlineIsEqualGUID(a, b) || InlineIsEqualGUID(a, GUID_NULL) || InlineIsEqualGUID(b, GUID_NULL);
}

struct CVGMediaType
{
	GUID clsMajor, clsMinor;
	bool bExactMatch;

	CVGMediaType(void): bExactMatch(true)	{}
	CVGMediaType(REFGUID major, REFGUID minor, bool exact = true): clsMajor(major), clsMinor(minor), bExactMatch(exact)	{}
	CVGMediaType(const REGPINTYPES& r): clsMajor(*r.clsMajorType), clsMinor(*r.clsMinorType), bExactMatch(true)	{}

	friend bool operator==(const CVGMediaType& a, const CVGMediaType& b)
	{
		if (a.bExactMatch && b.bExactMatch)
			return InlineIsEqualGUID(a.clsMajor, b.clsMajor) && InlineIsEqualGUID(a.clsMinor, b.clsMinor);
		else
			return MatchGUID(a.clsMajor, b.clsMajor) && MatchGUID(a.clsMinor, b.clsMinor);
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
