#pragma once

#include "VGBaseObject.h"

using namespace std;
using namespace stdext;

extern CFactoryTemplate *g_Templates;
extern int g_cTemplates;

class CVGFilterManager
{
private:
	typedef hash_map<CVGMediaType, CFactoryTemplate*> mt_lookup_t;
	typedef pair<CVGMediaType, CFactoryTemplate*> mt_lookup_pair_t;
	typedef hash_map<CLSID, CFactoryTemplate*> filter_lookup_t;
	typedef pair<CLSID, CFactoryTemplate*> filter_lookup_pair_t;
private:
	mt_lookup_t			m_lookupMT;		// ����Pin��MediaType���˾���ӳ��
	filter_lookup_t		m_lookupFlt;	// CLSID���˾���ӳ��
public:
	CVGFilterManager(void);
	virtual ~CVGFilterManager(void);
};
