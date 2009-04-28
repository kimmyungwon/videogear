#include "StdAfx.h"
#include "FilterManager.h"
#include "InternalFilters.h"

bool operator< (const GUID& _left, const GUID& _right)
{
	return memcmp(&_left, &_right, sizeof(GUID)) < 0;
}

//////////////////////////////////////////////////////////////////////////

TRegisteredFilters CFilterManager::ms_regFilters;
TRegisteredExtensions CFilterManager::ms_regExts;
TRegisteredInputMediaTypes CFilterManager::ms_regInputs;

void CFilterManager::RegisterFilter( const WCHAR* name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
									const AMOVIESETUP_FILTER* pAMovieSetup_Filter )
{
	ASSERT_POINTER(clsID, CLSID);
	ASSERT_POINTER(pAMovieSetup_Filter, AMOVIESETUP_FILTER);

	ms_regFilters.insert(GUID(*clsID), new CInternalFilter(name, *clsID, lpfnNew, lpfnInit));

	for (UINT iPin = 0; iPin < pAMovieSetup_Filter->nPins; iPin++)
	{
		const AMOVIESETUP_PIN& pin = pAMovieSetup_Filter->lpPin[iPin];
		if (!pin.bOutput)
		{
			for (UINT iMT = 0; iMT < pin.nMediaTypes; iMT++)
			{
				const REGPINTYPES& mt = pin.lpMediaType[iMT];
				ASSERT_POINTER(mt.clsMajorType, CLSID);
				ASSERT_POINTER(mt.clsMinorType, CLSID);
				ms_regInputs.insert(std::make_pair(TMediaType(*mt.clsMajorType, *mt.clsMinorType), *clsID));
			}
		}
	}
}

void CFilterManager::RegisterSourceFilter( const WCHAR* name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
										  LPCTSTR chkbytes, LPCTSTR ext /*= NULL*/, ... )
{
	ASSERT_POINTER(clsID, CLSID);

	va_list args;
	
	ms_regFilters.insert(GUID(*clsID), new CInternalFilter(name, *clsID, lpfnNew, lpfnInit));

	va_start(args, ext);
	for (; ext != NULL; ext = va_arg(args, LPCTSTR))
		ms_regExts.insert(std:: make_pair(CString(ext), *clsID));
	va_end(args);
}

void CFilterManager::RegisterSourceFilter( const WCHAR* name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
										  const std::vector<CString>& chkbytes, LPCTSTR ext /*= NULL*/, ... )
{
	ASSERT_POINTER(clsID, CLSID);

	va_list args;
	
	ms_regFilters.insert(GUID(*clsID), new CInternalFilter(name, *clsID, lpfnNew, lpfnInit));

	va_start(args, ext);
	for (; ext != NULL; ext = va_arg(args, LPCTSTR))
		ms_regExts.insert(std:: make_pair(CString(ext), *clsID));
	va_end(args);
}

#if SUPPORT_AVI
void RegisterAVI(void);
#endif

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
#if SUPPORT_AVI
	RegisterAVI();
#endif
}

CFilterManager::CFilterManager(void)
{
}

CFilterManager::~CFilterManager(void)
{
}