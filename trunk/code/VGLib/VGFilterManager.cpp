#include "StdAfx.h"
#include "VGFilterManager.h"

CEnumFilter::CEnumFilter( void )
{
	m_iter = m_items.end();
}

void CEnumFilter::Add( const CVGFilter& flt )
{
	m_items.insert(flt);
	Reset();	
}

size_t CEnumFilter::GetCount( void )
{
	return m_items.size();
}

HRESULT STDMETHODCALLTYPE CEnumFilter::Next( ULONG celt, GUID *rgelt, ULONG *pceltFetched )
{
	CheckPointer(rgelt, E_POINTER);
	ValidateReadWritePtr(rgelt, celt * sizeof(GUID));
	if (m_iter == m_items.end())
		return E_FAIL;

	ULONG nDone = 0;
	while (m_iter != m_items.end() && nDone < celt)
	{
		rgelt[nDone] = m_iter->clsID;
		m_iter++;
		nDone++;
	}	
	return nDone == celt ? S_OK : S_FALSE;
}

HRESULT STDMETHODCALLTYPE CEnumFilter::Skip( ULONG celt )
{
	if (m_iter == m_items.end())
		return E_FAIL;

	ULONG nDone = 0;
	while (m_iter != m_items.end() && nDone < celt)
	{
		m_iter++;
		nDone++;
	}
	return nDone == celt ? S_OK : S_FALSE;
}

HRESULT STDMETHODCALLTYPE CEnumFilter::Reset( void )
{
	m_iter = m_items.begin();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CEnumFilter::Clone( IEnumGUID **ppenum )
{
	CEnumFilter *pEnum = new CEnumFilter;
	pEnum->m_items = m_items;
	pEnum->m_iter = m_iter;
	pEnum->AddRef();
	(*ppenum) = pEnum;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

extern CFactoryTemplate *g_pTemplates;
extern int g_cTemplates;

CVGFilterManager::CVGFilterManager(void)
{
	const AMOVIESETUP_FILTER *pMSF = NULL;
	const AMOVIESETUP_PIN *pMSP= NULL;
	const AMOVIESETUP_MEDIATYPE *pMSMT = NULL;
	
	// 注册内部滤镜
	for (int i=0; i<g_cTemplates; i++)
		RegisterFilter(g_pTemplates[i]);
	// 枚举系统滤镜
	RegisterSystemFilters(HKEY_CLASSES_ROOT, L"CLSID\\{083863F1-70DE-11d0-BD40-00A0C911CE86}\\Instance");
	//RegisterSystemFilters(HKEY_CURRENT_USER, L"Software\\Microsoft\\ActiveMovie\\devenum\\{E0F158E1-CB04-11D0-BD4E-00A0C911CE86}");
}

CVGFilterManager::~CVGFilterManager(void)
{
}

HRESULT STDMETHODCALLTYPE CVGFilterManager::QueryInterface( REFIID refiid, void **ppv )
{
	if (InlineIsEqualGUID(refiid, IID_IGraphBuilder))
	{
		if (m_pGB != NULL)
		{
			*ppv = m_pGB;
			static_cast<IGraphBuilder*>(*ppv)->AddRef();
			return S_OK;
		}
		else
			return E_FAIL;
	}
	else
		return CVGUnknownImpl::QueryInterface(refiid, ppv);
}

void CVGFilterManager::RegisterSystemFilters( HKEY hkeyRoot, LPCWSTR lpszSubPath )
{
	CRegKey regRoot, regItem;
	DWORD nIdx = 0, nKeyNameLen, nFilterNameLen, nFilterDataSize;
	CLSID clsID;
	WCHAR szKeyName[MAX_PATH], szFilterName[MAX_FILTER_NAME];
	PBYTE pFilterData;

	if (regRoot.Open(hkeyRoot, lpszSubPath) == ERROR_SUCCESS)
	{
		while (regRoot.EnumKey(nIdx++, szKeyName, &(nKeyNameLen = MAX_PATH), NULL) == S_OK)
		{
			if (regItem.Open(regRoot.m_hKey, szKeyName) != ERROR_SUCCESS)
				continue;
			if (regItem.QueryGUIDValue(L"CLSID", clsID) != ERROR_SUCCESS)
				continue;
			if (regItem.QueryStringValue(L"FriendlyName", szFilterName, &(nFilterNameLen = MAX_FILTER_NAME)) != ERROR_SUCCESS)
				continue;
			if (regItem.QueryBinaryValue(L"FilterData", NULL, &nFilterDataSize) == ERROR_SUCCESS)
			{
				pFilterData = (PBYTE)malloc(nFilterDataSize);
				if (regItem.QueryBinaryValue(L"FilterData", pFilterData, &nFilterDataSize) == ERROR_SUCCESS)
				{
					RegisterFilter(clsID, szFilterName, (char*)pFilterData, nFilterDataSize);
				}
				free(pFilterData);
			}			
		}
	}
}


HRESULT STDMETHODCALLTYPE CVGFilterManager::EnumMatchingFilters( IEnumGUID **ppEnum, BOOL bExactMatch, DWORD dwMerit, 
																CLSID clsInMaj, CLSID clsInSub )
{
	CheckPointer(ppEnum, E_POINTER);
	
	HRESULT hr;
	CVGFilters ret;
	CEnumFilter *pEnum = new CEnumFilter;
	if (SUCCEEDED(hr = EnumMatchingFilters(ret, bExactMatch, dwMerit, clsInMaj, clsInSub)))
	{
		for (CVGFilters::const_iterator it=ret.begin(); it!=ret.end(); it++)
			pEnum->Add(*it);
		pEnum->AddRef();
		*ppEnum = pEnum;
		return S_OK;
	}
	else
	{
		delete pEnum;
		*ppEnum = NULL;
		return E_FAIL;
	}
}

HRESULT CVGFilterManager::EnumMatchingFilters( CVGFilters &ret, BOOL bExactMatch, DWORD dwMerit, 
											  CLSID clsInMaj, CLSID clsInSub ) const
{
	// 枚举匹配的内部滤镜
	if (bExactMatch)
	{
		maj2subs_t::const_iterator itMaj = m_lookupMT.find(clsInMaj);
		if (itMaj != m_lookupMT.end())
		{
			guid2ft_itpair_t pFT = itMaj->second.equal_range(clsInSub);
			if (pFT.first != itMaj->second.end() && pFT.second != itMaj->second.end())
			{
				for (guid2ft_t::const_iterator it=pFT.first; it!=pFT.second; it++)
					if (it->second.dwMerit >= dwMerit)
					{
						ret.insert(it->second);
					}
			}
		}
	}
	else	// 非精确匹配时GUID_NULL被看作通配符
	{
		for (maj2subs_t::const_iterator itMaj=m_lookupMT.begin(); itMaj!=m_lookupMT.end(); itMaj++)
		{
			if (!MatchGUID(itMaj->first, clsInMaj))
				continue;

			for (guid2ft_t::const_iterator itSub=itMaj->second.begin(); itSub!=itMaj->second.end(); itSub++)
			{
				if (MatchGUID(itSub->first, clsInSub) && itSub->second.dwMerit >= dwMerit)
					ret.insert(itSub->second);
			}
		}
	}

	return ret.size() > 0 ? S_OK : E_FAIL;
}

HRESULT STDMETHODCALLTYPE CVGFilterManager::Initialize( void )
{
	HRESULT hr;
	
	FAILED_RETURN(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (LPVOID*)&m_pGB));
	FAILED_RETURN(CoCreateInstance(CLSID_FilterMapper2, NULL, CLSCTX_INPROC_SERVER, IID_IFilterMapper2, (LPVOID*)&m_pFM2));
	return S_OK;
}

HRESULT CVGFilterManager::RegisterFilter( const CFactoryTemplate& templ )
{	
	return RegisterFilter(*templ.m_ClsID, templ.m_Name, templ.m_pAMovieSetup_Filter->dwMerit, 
		templ.m_pAMovieSetup_Filter->nPins, templ.m_pAMovieSetup_Filter->lpPin, templ.m_lpfnNew);
}

HRESULT CVGFilterManager::RegisterFilter( REFCLSID clsID, LPCWSTR lpszName, DWORD dwMerit, ULONG nPins, 
										 const REGFILTERPINS* lpPins, LPFNNewCOMObject lpfnNew )
{
	CheckPointer(lpPins, E_POINTER);
	if (m_lookupFlt.find(clsID) != m_lookupFlt.end())
		return ERROR_FILE_EXISTS;

	CVGFilter flt(clsID, lpszName, dwMerit, lpfnNew);
	const AMOVIESETUP_MEDIATYPE *pMSMT = NULL;

	m_lookupFlt[clsID] = flt;
	for (UINT i=0; i<nPins; i++)
	{
		if (lpPins[i].bOutput)
			continue;
		for (UINT j=0; j<lpPins[i].nMediaTypes; j++)
		{
			pMSMT = &lpPins[i].lpMediaType[j];
			(m_lookupMT[*pMSMT->clsMajorType]).insert(make_pair(*pMSMT->clsMinorType, flt));
		}
	}	
	return S_OK;	
}

HRESULT CVGFilterManager::RegisterFilter( REFCLSID clsID, LPCWSTR lpszName, DWORD dwMerit, ULONG nPins2, 
										 const REGFILTERPINS2* lpPins2, LPFNNewCOMObject lpfnNew )
{
	CheckPointer(lpPins2, E_POINTER);
	if (m_lookupFlt.find(clsID) != m_lookupFlt.end())
		return ERROR_FILE_EXISTS;

	CVGFilter flt(clsID, lpszName, dwMerit, lpfnNew);
	const AMOVIESETUP_MEDIATYPE *pMSMT = NULL;

	m_lookupFlt[clsID] = flt;
	for (UINT i=0; i<nPins2; i++)
	{
		if (lpPins2[i].dwFlags & REG_PINFLAG_B_OUTPUT)
			continue;
		for (UINT j=0; j<lpPins2[i].nMediaTypes; j++)
		{
			pMSMT = &lpPins2[i].lpMediaType[j];
			(m_lookupMT[*pMSMT->clsMajorType]).insert(make_pair(*pMSMT->clsMinorType, flt));
		}
	}	
	return S_OK;		
}

HRESULT CVGFilterManager::RegisterFilter( REFCLSID clsID, LPCWSTR lpszName, char* pBuf, DWORD nSize )
{
	// 参考了GraphStudio(Igor Janos)中的部分代码
	DWORD *b = (DWORD*)pBuf, dwVersion = b[0], dwMerit = b[1], *ps = b + 4;
	int cpins1 = b[2], cpins2 = b[3];
	CVGFilter flt(clsID, lpszName, dwMerit);
	
	m_lookupFlt[clsID] = flt;
	for (int i=0; i<cpins1; i++) 
	{
		if ((char*)ps > (pBuf + nSize - 6*4)) break;

		DWORD flags = ps[1], pindir;
		pindir = (flags & 0x08 ? PINDIR_OUTPUT : PINDIR_INPUT);
		int pintypes = ps[3];

		// skip dummy data
		ps += 6;
		for (int j=0; j<pintypes; j++) 
		{
			// make sure we have at least 16 bytes available
			if ((char*)ps > (pBuf + nSize - 16)) break;
			
			DWORD maj_offset = ps[2];
			DWORD min_offset = ps[3];
			if ((maj_offset + 16 <= nSize) && (min_offset + 16 <= nSize)) 
			{
				GUID g, major, minor;
				BYTE *m = (BYTE*)(&pBuf[maj_offset]);
				if ((char*)m > (pBuf+nSize - 16)) break;
				g.Data1 = m[0] | (m[1] << 8) | (m[2] << 16) | (m[3] << 24);
				g.Data2 = m[4] | (m[5] << 8);
				g.Data3 = m[6] | (m[7] << 8);
				memcpy(g.Data4, m+8, 8);
				major = g;

				m = (BYTE*)(&pBuf[min_offset]);
				if ((char*)m > (pBuf+nSize - 16)) break;
				g.Data1 = m[0] | (m[1] << 8) | (m[2] << 16) | (m[3] << 24);
				g.Data2 = m[4] | (m[5] << 8);
				g.Data3 = m[6] | (m[7] << 8);
				memcpy(g.Data4, m+8, 8);
				minor = g;

				if (pindir == PINDIR_INPUT)
				{
					(m_lookupMT[major]).insert(make_pair(minor, flt));
				}
			}
		}
	}

	return S_OK;
}

PIN_DIRECTION CVGFilterManager::GetPinDir( IPin* pPin )
{
	ASSERT(pPin != NULL);

	PIN_DIRECTION pinDir;
	pPin->QueryDirection(&pinDir);
	return pinDir;
}

bool CVGFilterManager::IsPinConnected( IPin* pPin )
{
	ASSERT(pPin != NULL);

	CComPtr<IPin> pConnectedPin;
	pPin->ConnectedTo(&pConnectedPin);
	return pConnectedPin != NULL;
}

HRESULT STDMETHODCALLTYPE CVGFilterManager::RenderFile( LPCWSTR lpszFileName )
{
	CheckPointer(m_pGB, E_POINTER);
	
	HRESULT hr;
	CComPtr<IBaseFilter> pSF, pVid, pAud;
	CComQIPtr<IFileSourceFilter> pFSF;

	FAILED_RETURN(ClearGraph());
	// 加入初始滤镜
	FAILED_RETURN(pVid.CoCreateInstance(CLSID_VideoRendererDefault));
	FAILED_RETURN(m_pGB->AddFilter(pVid, L"Default Video Renderer"));
	FAILED_RETURN(pAud.CoCreateInstance(CLSID_DSoundRender));
	FAILED_RETURN(m_pGB->AddFilter(pAud, L"Default DirectSound Renderer"));
	// 开始渲染
	FAILED_RETURN(pSF.CoCreateInstance(CLSID_AsyncReader));
	pFSF = pSF;
	FAILED_RETURN(pFSF->Load(lpszFileName, NULL));
	FAILED_RETURN(m_pGB->AddFilter(pSF, lpszFileName));
	hr = RenderFilter(pSF);
	if (SUCCEEDED(hr))
		if (hr == S_OK)
			DbgLog((LOG_TRACE, 0, L"渲染文件\"%s\"成功！", lpszFileName));
		else
			DbgLog((LOG_TRACE, 0, L"渲染文件\"%s\"成功，但是有部分数据流未能渲染", lpszFileName));
	else
		DbgLog((LOG_TRACE, 0, L"渲染文件\"%s\"失败，因为：%s", lpszFileName, GetErrorMsg(hr)));
	return hr;	
}

HRESULT CVGFilterManager::RenderFilter( IBaseFilter* pBF )
{
	CheckPointer(pBF, E_POINTER);

	CComPtr<IEnumPins> pEnumPins;
	CComPtr<IPin> pPin;
	UINT nTotal = 0, nRendered = 0;

	DbgLog((LOG_TRACE, 0, L"开始渲染滤镜\"%s\"", GetFilterName(pBF)));
	BeginEnumPins(pBF, pEnumPins, pPin)
	{
		if (GetPinDir(pPin) == PINDIR_OUTPUT && !IsPinConnected(pPin))
		{
			nTotal++;
			if (SUCCEEDED(RenderPin(pPin)))
				nRendered++;
		}
	}
	EndEnumPins

	if (nRendered > 0)
		return nRendered == nTotal ? S_OK : S_FALSE;
	else
		return nTotal == 0 ? S_OK : E_FAIL;
}

HRESULT CVGFilterManager::RenderPin( IPin* pPin )
{
	CheckPointer(pPin, E_POINTER);
	if (GetPinDir(pPin) == PINDIR_INPUT) return VFW_E_INVALID_DIRECTION;
	if (IsPinConnected(pPin)) return VFW_E_ALREADY_CONNECTED;

	CComPtr<IEnumGUID> pEnumGUID;
	CVGFilters matcheds;
	CComPtr<IBaseFilter> pMatched;
	bool bRendered = false;

	DbgLog((LOG_TRACE, 0, L"开始渲染插针\"%s\"", GetPinName(pPin)));
	BeginEnumMediaTypes(pPin, pEnumMT, pmt)
	{
		// 优先使用FilterGraph中已有的滤镜
		BeginEnumFilters(m_pGB, pEnumFilters, pExists)
			if (SUCCEEDED(ConnectDirect(pPin, pExists, *pmt)))
			{
				bRendered = true;
				pMatched = pExists;
				DbgLog((LOG_TRACE, 0, L"成功渲染插针\"%s\"到\"%s\"", GetPinName(pPin), GetFilterName(pExists)));
				break;
			}
		EndEnumFilters
		if (bRendered)
			break;
		// 匹配新滤镜
		if (FAILED(EnumMatchingFilters(matcheds, FALSE, MERIT_NORMAL, pmt->majortype, pmt->subtype)))
			continue;
		for (CVGFilters::const_iterator it=matcheds.begin(); it!=matcheds.end(); it++)
		{
			if (SUCCEEDED(ConnectDirect(pPin, *it, *pmt, &pMatched)))
			{
				bRendered = true;
				DbgLog((LOG_TRACE, 0, L"成功渲染插针\"%s\"到\"%s\"", GetPinName(pPin), it->strName));
				break;
			}
			pMatched.Release();
		}
		if (bRendered)
			break;
	}
	EndEnumMediaTypes(pmt)

	if (bRendered)
		return  RenderFilter(pMatched);
	else
	{
		DbgLog((LOG_TRACE, 0, L"渲染插针\"%s\"失败", GetPinName(pPin)));
		return VFW_E_CANNOT_RENDER;
	}
}

HRESULT CVGFilterManager::ConnectDirect( IPin* pPinOut, const CVGFilter filterIn, const CMediaType& mt, 
										IBaseFilter** pBF )
{
	CheckPointer(pPinOut, E_POINTER);
	CheckPointer(pBF, E_POINTER);
	ASSERT(GetPinDir(pPinOut) == PINDIR_OUTPUT);
	ASSERT(!IsPinConnected(pPinOut));

	HRESULT hr;
	CComPtr<IBaseFilter> pBFIn;
	
	FAILED_RETURN(filterIn.CreateInstance(NULL, &pBFIn));
	FAILED_RETURN(m_pGB->AddFilter(pBFIn, filterIn.strName));
	if (SUCCEEDED(ConnectDirect(pPinOut, pBFIn, mt)))
	{
		*pBF = pBFIn;
		(*pBF)->AddRef();
		return S_OK;
	}
	else
	{
		m_pGB->RemoveFilter(pBFIn);
		*pBF = NULL;
		return VFW_E_CANNOT_CONNECT;
	}
}

HRESULT CVGFilterManager::ConnectDirect( IPin* pPinOut, IBaseFilter* pBFIn, const CMediaType& mt )
{
	CheckPointer(pPinOut, E_POINTER);
	CheckPointer(pBFIn, E_POINTER);
	ASSERT(GetPinDir(pPinOut) == PINDIR_OUTPUT);
	ASSERT(!IsPinConnected(pPinOut));

	IBaseFilter *pBFOut;

	// 防止相同的Filter互相连接造成死循环
	pBFOut = GetFilterFromPin(pPinOut);
	if (InlineIsEqualGUID(GetCLSID(pBFOut), GetCLSID(pBFIn)))
		return VFW_E_CANNOT_CONNECT;

	BeginEnumPins(pBFIn, pEnumPins, pPinIn)
	{
		if (GetPinDir(pPinIn) == PINDIR_OUTPUT || IsPinConnected(pPinIn))
			continue;
		if (SUCCEEDED(m_pGB->ConnectDirect(pPinOut, pPinIn, &mt)))
			return S_OK;
	}
	EndEnumPins

	return VFW_E_CANNOT_CONNECT;
}

HRESULT STDMETHODCALLTYPE CVGFilterManager::ClearGraph( void )
{
	HRESULT hr;
	// 断开所有插针
	BeginEnumFilters(m_pGB, pEnumFilters, pBF)
	{
		BeginEnumPins(pBF, pEnumPins, pPin)
		{
			FAILED_RETURN(pPin->Disconnect());
		}
		EndEnumPins
	}
	EndEnumFilters
	// 清除所有滤镜
	BeginEnumFilters(m_pGB, pEnumFilters, pBF)
	{
		FAILED_RETURN(m_pGB->RemoveFilter(pBF));
		pEnumFilters->Reset();
	}
	EndEnumFilters

	return S_OK;
}