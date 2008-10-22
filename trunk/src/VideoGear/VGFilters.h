#pragma once

using namespace std;

struct VGMediaType
{
	CLSID clsMajor;
	CLSID clsMinor;

	VGMediaType(void): clsMajor(GUID_NULL), clsMinor(GUID_NULL)	{}

	VGMediaType(const CLSID& _Major, const CLSID& _Minor)
		: clsMajor(_Major), clsMinor(_Minor)	{}

	VGMediaType& operator=(const AM_MEDIA_TYPE* rhs)
	{
		clsMajor = rhs->majortype;
		clsMinor = rhs->subtype;
		return *this;
	}

	bool operator<(const VGMediaType& rhs) const
	{
		if (memcmp(&clsMajor, &rhs.clsMajor, sizeof(CLSID)) < 0)
			return true;
		else
			return (memcmp(&clsMinor, &rhs.clsMinor, sizeof(CLSID)) < 0);
	}
};

struct VGFilter
{
	bool bInGB;
	CStringW strName;
	CComPtr<IBaseFilter> pFilter;

	VGFilter(IBaseFilter* _Filter): bInGB(true)	// �Ѿ���GraphBuilder�е��˾�
	{
		pFilter = _Filter;
	}

	VGFilter(const CStringW& _Name, IBaseFilter* _Filter): bInGB(false)	// �ڡ��ⲿ�˾�
	{
		strName = _Name;
		pFilter = _Filter;
	}

	VGFilter(IMoniker* _Moniker): bInGB(false) 	// ϵͳ����ע���˾�
	{
		CComPtr<IPropertyBag> pPB;
		CComVariant var;
		CLSID clsid;
		
		if (_Moniker == NULL)
			return;
		if (SUCCEEDED(_Moniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pPB)))
		{
			if(SUCCEEDED(pPB->Read(CComBSTR(_T("FriendlyName")), &var, NULL)))
			{
				strName = var.bstrVal;
				var.Clear();
			}

			if(SUCCEEDED(pPB->Read(CComBSTR(_T("CLSID")), &var, NULL)))
			{
				CLSIDFromString(var.bstrVal, &clsid);
				var.Clear();
				CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pFilter);
			}
		}
	}
};

typedef vector<VGFilter> VGMatchingFilters;

class VGFilters
{
public:
	VGFilters(void);
	~VGFilters(void);
	// ���ҽ���ָ��MediaType��ת���˾�
	HRESULT FindMatchingFilters(const AM_MEDIA_TYPE* _InType, VGMatchingFilters& _Results);
protected:
	// ע��ת���˾�
	void RegisterTransformFilters(CFactoryTemplate* _Templates, int _Count);
private:
	typedef multimap<VGMediaType, CFactoryTemplate*>	pinTypes_t;

	pinTypes_t	m_pinTypes;
};

extern VGFilters g_internalFilters;
