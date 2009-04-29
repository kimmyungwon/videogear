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

HRESULT CFilterManager::EnumMatchingFilters( bool exactMatch, const GUID* inputTypes, size_t inputTypeCount, std::list<CFilter*>& filters )
{
	ASSERT(inputTypes != NULL);

	bool found = false;

	for (size_t iType = 0; iType < inputTypeCount; iType += 2)
	{
		MediaType reqType(inputTypes[iType], inputTypes[iType + 1]);

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
		if (!exactMatch && !(IsEqualGUID(reqType.major, GUID_NULL) && IsEqualGUID(reqType.minor, GUID_NULL)))
		{
			if (!IsEqualGUID(reqType.major, GUID_NULL))
			{
				GUID types[] = {GUID_NULL, reqType.minor};
				if (EnumMatchingFilters(true, types, 2, filters) == S_OK)
					found = true;
			}
			if (!IsEqualGUID(reqType.minor, GUID_NULL))
			{
				GUID types[] = {reqType.major, GUID_NULL};
				if (EnumMatchingFilters(true, types, 2, filters) == S_OK)
					found = true;
			}
			if (!IsEqualGUID(reqType.major, GUID_NULL) && !IsEqualGUID(reqType.minor, GUID_NULL))
			{
				GUID types[] = {GUID_NULL, GUID_NULL};
				if (EnumMatchingFilters(true, types, 2, filters) == S_OK)
					found = true;
			}
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