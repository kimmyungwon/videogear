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
	ASSERT_POINTER(clsID, CLSID);
	ASSERT_POINTER(pAMovieSetup_Filter, AMOVIESETUP_FILTER);
	
	CFilter* filter;

	filter = new CInternalFilter(name, *clsID, lpfnNew, lpfnInit);
	m_filters.insert(CLSID(*clsID), filter);
	for (UINT iPin = 0; iPin < pAMovieSetup_Filter->nPins; iPin++)
	{
		const AMOVIESETUP_PIN& pin = pAMovieSetup_Filter->lpPin[iPin];
		if (!pin.bOutput)
		{
			for (UINT iType = 0; iType < pin.nMediaTypes; iType++)
				m_inputTypes.insert(std::make_pair(MediaType(pin.lpMediaType[iType]), filter));
		}
	}
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
		filter->AddSignature(chkbytes[i]);
	return filter;
}

UINT CFilterManager::EnumMatchingFilters( bool exactMatch, AM_MEDIA_TYPE* inputType, std::list<CFilter*>& filters )
{
	ASSERT(inputType != NULL);

	MediaType reqType(*inputType);
	UINT count = 0;

	// ÍêÈ«Æ¥Åä
	std::pair<input_types_t::const_iterator, input_types_t::const_iterator> pii = m_inputTypes.equal_range(reqType);
	for (input_types_t::const_iterator it = pii.first; it != pii.second; it++)
	{	
		CFilter* filter = it->second;
		filters.push_back(filter);
		count++;
	}
	// Ä£ºýÆ¥Åä
	if (!exactMatch && !(IsEqualGUID(inputType->majortype, GUID_NULL) && IsEqualGUID(inputType->subtype, GUID_NULL)))
	{
		if (!IsEqualGUID(inputType->majortype, GUID_NULL))
		{
			CMediaType mt(*inputType);
			mt.majortype = GUID_NULL;
			count += EnumMatchingFilters(true, &mt, filters);
		}
		if (!IsEqualGUID(inputType->subtype, GUID_NULL))
		{
			CMediaType mt(*inputType);
			mt.subtype = GUID_NULL;
			count += EnumMatchingFilters(true, &mt, filters);
		}
	}
	return count;	
}