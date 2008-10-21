#pragma once

using namespace std;

struct VGMediaType
{
	CLSID clsMajor;
	CLSID clsMinor;

	VGMediaType(const CLSID& _Major, const CLSID& _Minor)
		: clsMajor(_Major), clsMinor(_Minor)	{}

	bool operator<(const VGMediaType& rhs) const
	{
		if (memcmp(&clsMajor, &rhs.clsMajor, sizeof(CLSID)) < 0)
			return true;
		else
			return (memcmp(&clsMinor, &rhs.clsMinor, sizeof(CLSID)) < 0);
	}
};

class VGFilters
{
public:
	VGFilters(void);
	~VGFilters(void);
protected:
	void RegisterFilters(CFactoryTemplate* _Templates, int _Count);
private:
	typedef multimap<VGMediaType, CFactoryTemplate*>	pinTypes_t;

	pinTypes_t	m_pinTypes;
};

extern VGFilters g_filters;
