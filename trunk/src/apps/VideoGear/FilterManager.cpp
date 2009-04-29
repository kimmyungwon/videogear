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
#if SUPPORT_VIDEODEC
	RegisterVideoDec();
#endif
#if SUPPORT_AUDIODEC
	RegisterAudioDec();
#endif
}

HRESULT CFilterManager::EnumMatchingFilters( bool exactMatch, AM_MEDIA_TYPE* inputType, std::list<CFilter*>& filters )
{
	ASSERT(inputType != NULL);

	MediaType reqType(*inputType);
	bool found = false;

	// ÍêÈ«Æ¥Åä
	std::pair<input_types_t::const_iterator, input_types_t::const_iterator> pii = m_inputTypes.equal_range(reqType);
	for (input_types_t::const_iterator it = pii.first; it != pii.second; it++)
	{	
		CFilter* filter = it->second;
		if (std::find(filters.begin(), filters.end(), filter) == filters.end())
			filters.push_back(filter);
		found = true;
	}
	// Ä£ºýÆ¥Åä
	if (!exactMatch && !(IsEqualGUID(inputType->majortype, GUID_NULL) && IsEqualGUID(inputType->subtype, GUID_NULL)))
	{
		if (!IsEqualGUID(inputType->majortype, GUID_NULL))
		{
			CMediaType mt(*inputType);
			mt.majortype = GUID_NULL;
			if (EnumMatchingFilters(true, &mt, filters) == S_OK)
				found = true;
		}
		if (!IsEqualGUID(inputType->subtype, GUID_NULL))
		{
			CMediaType mt(*inputType);
			mt.subtype = GUID_NULL;
			if (EnumMatchingFilters(true, &mt, filters) == S_OK)
				found = true;
		}
		if (!IsEqualGUID(inputType->majortype, GUID_NULL) && !IsEqualGUID(inputType->subtype, GUID_NULL))
		{
			CMediaType mt;
			mt.majortype = GUID_NULL;
			mt.subtype = GUID_NULL;
			if (EnumMatchingFilters(true, &mt, filters) == S_OK)
				found = true;
		}
	}
	return found ? S_OK : E_FAIL;	
}

HRESULT CFilterManager::EnumMatchingSource( LPCTSTR fileName, CSourceFilter*& filter )
{
	CFile file;
	CFileException exception;
	
	if (file.Open(fileName, CFile::modeRead | CFile::shareDenyNone, &exception))
	{
		for (sources_t::iterator it = m_sources.begin(); it != m_sources.end(); it++)
		{
			if (it->second->CheckSignature(file))
			{
				filter = it->second;
				return S_OK;
			}
		}
	}
	return E_FAIL;
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
	m_sources.insert(CLSID(*clsID), filter);
	for (size_t i = 0; i < chkbytesCount; i++)
		filter->AddSignature(chkbytes[i]);
	return filter;
}