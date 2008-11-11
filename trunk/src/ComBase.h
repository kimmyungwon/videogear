//---------------------------------------------------------------------------

#ifndef ComBaseH
#define ComBaseH

template<class T> inline static T ourmax( const T & a, const T & b )
{
    return a > b ? a : b;
}

template<typename BaseT>
class TUnknown : protected BaseT
{
public:
	TUnknown(void): m_lRef(0)	{}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject)
	{
		if (riid == __uuidof(BaseT))
		{
        	*ppvObject = (BaseT*)this;
			return S_OK;
		}
		else if (riid == IID_IUnknown)
		{
			*ppvObject = (IUnknown*)this;
			return S_OK;
		}
		else
        	return E_NOINTERFACE;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		LONG lRef = InterlockedIncrement(&m_lRef);
		assert(lRef > 0);
		return ourmax((ULONG)lRef, 1ul);
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
    	LONG lRef = InterlockedDecrement(&m_lRef);
		assert(lRef >= 0);
		if (lRef == 0)
		{
			delete this;
			return 0ul;
		}
		else
			return ourmax((ULONG)lRef, 1ul);
	}
private:
	volatile LONG m_lRef;
};

//---------------------------------------------------------------------------
#endif
