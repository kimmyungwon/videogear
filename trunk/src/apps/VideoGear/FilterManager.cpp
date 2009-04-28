#include "StdAfx.h"
#include "FilterManager.h"
#include "InternalFilters.h"
#include "Utils.h"
#include "VideoGear.h"

bool operator< (const GUID& _left, const GUID& _right)
{
	return memcmp(&_left, &_right, sizeof(GUID)) < 0;
}

//////////////////////////////////////////////////////////////////////////

TRegisteredFilters CFilterManager::ms_regFilters;
TRegisteredCheckBytes CFilterManager::ms_regChkBytes;
TRegisteredExtensions CFilterManager::ms_regExts;
TRegisteredInputMediaTypes CFilterManager::ms_regInputs;

void CFilterManager::RegisterFilter( LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
									const AMOVIESETUP_FILTER* pAMovieSetup_Filter )
{
	ASSERT_POINTER(clsID, CLSID);
	ASSERT_POINTER(pAMovieSetup_Filter, AMOVIESETUP_FILTER);

	ms_regFilters.insert(CLSID(*clsID), new CInternalFilter(name, *clsID, lpfnNew, lpfnInit));

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

void RegisterCheckBytes(const CLSID& clsID, const CString& chkbytes)
{
	CAtlList<CString> tokens;

	Explode(chkbytes, tokens, _T(','));
	while (tokens.GetCount() >= 4)
	{
		CString offset, length, mask, value;
		TCheckBytes bytes;

		offset = tokens.RemoveHead();
		length = tokens.RemoveHead();
		mask = tokens.RemoveHead();
		value = tokens.RemoveHead();
		bytes.clsID = clsID;
		bytes.offset = _tcstoi64(offset, NULL, 10);
		bytes.length = _tcstoul(length, NULL, 10);
		bytes.mask = _tcstoul(mask, NULL, 10);
		HexToBin(value, bytes.bytes);
		CFilterManager::ms_regChkBytes.push_back(bytes);
	}
}

void CFilterManager::RegisterSourceFilter( LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
										  CString chkbytes, LPCTSTR ext /*= NULL*/, ... )
{
	ASSERT_POINTER(clsID, CLSID);

	va_list args;
	
	ms_regFilters.insert(CLSID(*clsID), new CInternalFilter(name, *clsID, lpfnNew, lpfnInit));

	RegisterCheckBytes(*clsID, chkbytes);

	va_start(args, ext);
	for (; ext != NULL; ext = va_arg(args, LPCTSTR))
		ms_regExts.insert(std:: make_pair(CString(ext), *clsID));
	va_end(args);
}

void CFilterManager::RegisterSourceFilter( LPCTSTR name, const CLSID* clsID, LPFNNewCOMObject lpfnNew, LPFNInitRoutine lpfnInit, 
										  const std::vector<CString>& chkbytes, LPCTSTR ext /*= NULL*/, ... )
{
	ASSERT_POINTER(clsID, CLSID);

	va_list args;
	
	ms_regFilters.insert(CLSID(*clsID), new CInternalFilter(name, *clsID, lpfnNew, lpfnInit));

	for (std::vector<CString>::const_iterator it = chkbytes.begin(); it != chkbytes.end(); it++)
		RegisterCheckBytes(*clsID, *it);

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
	m_graph.CoCreateInstance(CLSID_FilterGraph);
}

CFilterManager::~CFilterManager(void)
{
	m_graph.Release();
}

void CFilterManager::Clear( void )
{
	BeginEnumFilters(m_graph, enumFilters, filter)
	{
		BeginEnumPins(filter, enumPins, pin)
		{
			if (IsPinConnected(pin))
				m_graph->Disconnect(pin);
		}
		EndEnumPins
		m_graph->RemoveFilter(filter);
	}
	EndEnumFilters
}

HRESULT CFilterManager::AddSourceFilter( LPCTSTR fileName, IBaseFilter*& filter )
{
	if (filter == NULL)
		return E_POINTER;
	
	CFile file;
	CFileException exception;
	bool filterFound = false;
	CLSID clsID;
	CComPtr<IBaseFilter> source;
	CComPtr<IFileSourceFilter> fileSource;

	if (!file.Open(fileName, CFile::modeRead | CFile::shareDenyNone, &exception))
		return HRESULT_FROM_WIN32(exception.m_lOsError);

	for (TRegisteredCheckBytes::const_iterator it = ms_regChkBytes.begin(); it != ms_regChkBytes.end(); it++)
	{
		if (CheckBytes(file, *it))
		{
			clsID = it->clsID;
			filterFound = true;
			break;
		}
	}
	if (!filterFound)
		return VFW_E_UNKNOWN_FILE_TYPE;

	TRegisteredFilters::iterator itFilter = ms_regFilters.find(clsID);
	if (itFilter == ms_regFilters.end())
		return VFW_E_CANNOT_LOAD_SOURCE_FILTER;
	if (FAILED(itFilter->second->CreateObject(&source))
		|| FAILED(source.QueryInterface(&fileSource))
		|| FAILED(fileSource->Load(fileName, NULL))
		|| FAILED(m_graph->AddFilter(source, itFilter->second->GetName())))
		return VFW_E_CANNOT_LOAD_SOURCE_FILTER;
	
	filter = source;
	return S_OK;
}

HRESULT CFilterManager::Render( IBaseFilter* filter )
{
	if (filter == NULL)
		return E_POINTER;

	BeginEnumPins(filter, enumPins, outPin)
	{
		if (GetPinDir(outPin) != PINDIR_OUTPUT)
			continue;
	}
	EndEnumPins
}

bool CFilterManager::CheckBytes( CFile& file, const TCheckBytes& chkbytes )
{
	CAutoVectorPtr<BYTE> buffer;
	UINT bytesRead;
	
	if (chkbytes.offset >= 0)
		file.Seek(chkbytes.offset, CFile::begin);
	else
		file.Seek(chkbytes.offset, CFile::end);

	buffer.Allocate(chkbytes.length);
	bytesRead = file.Read(buffer.m_p, chkbytes.length);
	if (bytesRead != chkbytes.length)
		return false;

	return memcmp(buffer.m_p, chkbytes.bytes.c_str(), chkbytes.length) == 0;
}