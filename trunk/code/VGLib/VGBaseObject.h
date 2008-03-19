#pragma once

struct CGUID : public GUID
{
	CGUID(const GUID& r): GUID(r)	{}
	
	friend bool operator==(const CGUID& a, const CGUID& b)
	{
		return IsEqualGUID(a, b) == TRUE;
	}
};

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
