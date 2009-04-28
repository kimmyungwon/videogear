#include "StdAfx.h"
#include "FilterManager.h"
#include "InternalFilters.h"

void CFilterManager::RegisterFilter( const WCHAR* name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
									const AMOVIESETUP_FILTER* pAMovieSetup_Filter )
{

}

void CFilterManager::RegisterSourceFilter( const WCHAR* name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
										  LPCTSTR chkbytes, LPCTSTR ext /*= NULL*/, ... )
{

}

#if SUPPORT_MK
void RegisterMK(void);	
#endif

#if	SUPPORT_RM
void RegisterRM(void);
#endif

void CFilterManager::RegisterInternalFilters( void )
{
#if SUPPORT_MK
	RegisterMK();
#endif
#if	SUPPORT_RM
	RegisterRM();
#endif
}

CFilterManager::CFilterManager(void)
{
}

CFilterManager::~CFilterManager(void)
{
}