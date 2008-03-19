#include "StdAfx.h"
#include "VGFilterManager.h"

CVGFilterManager::CVGFilterManager(void)
{
	for (int i=0; i<g_cTemplates; i++)
	{
		m_lookupFlt.insert(filter_lookup_pair_t(*g_Templates[i].m_ClsID, &g_Templates[i]));
	}
}

CVGFilterManager::~CVGFilterManager(void)
{
}
