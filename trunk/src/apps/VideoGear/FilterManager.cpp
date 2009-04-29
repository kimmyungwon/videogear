#include "StdAfx.h"
#include "FilterManager.h"

CFilterManager g_filterMan;

CFilterManager::CFilterManager(void)
{
}

CFilterManager::~CFilterManager(void)
{
}

void CFilterManager::Initialize( void )
{
#if SUPPORT_AVI
	RegisterAVI();
#endif
#if SUPPORT_MK
	RegisterMK();
#endif
#if SUPPORT_RM
	RegisterRM();
#endif
}

CFilter* CFilterManager::RegisterFilter( LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, 
										LPFNInitRoutine lpfnInit, const AMOVIESETUP_FILTER* pAMovieSetup_Filter )
{
	CFilter* filter;

	filter = new CInternalFilter(name, *clsID, lpfnNew, lpfnInit);
	m_filters.insert(CLSID(*clsID), filter);
	return filter;
}

CSourceFilter* CFilterManager::RegisterSourceFilter( LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, 
													LPFNInitRoutine lpfnInit, LPCTSTR* chkbytes, 
													size_t chkbytesCount, LPCTSTR* ext, size_t extCount )
{
	CSourceFilter* filter;

	filter = new CInternalSourceFilter(name, *clsID, lpfnNew, lpfnInit);
	m_filters.insert(CLSID(*clsID), filter);
	for (size_t i = 0; i < chkbytesCount; i++)
	{
		filter->AddSignature(chkbytes[i]);
	}
	return filter;
}