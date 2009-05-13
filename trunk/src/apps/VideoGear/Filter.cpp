#include "StdAfx.h"
#include "Filter.h"
#include "DSUtil.h"

CFilter::CFilter( const CLSID& clsID, LPCWSTR pszName )
: m_clsID(clsID), m_strName(pszName)
{
}

CFilter::~CFilter(void)
{
}

//////////////////////////////////////////////////////////////////////////

CFilterInternal::CFilterInternal( const CLSID& clsID, LPCWSTR pszName, PFNCreateInstance pfnCreateInstance, 
								 UINT uiPinCount, const AMOVIESETUP_PIN* pPins )
: CFilter(clsID, pszName), m_pfnCreateInstance(pfnCreateInstance)
{
}

HRESULT CFilterInternal::CreateInstance( LPUNKNOWN pUnk, IBaseFilter** ppv )
{
	return m_pfnCreateInstance(pUnk, ppv);
}

//////////////////////////////////////////////////////////////////////////

CFilterRegister::CFilterRegister( const CLSID& clsID, LPCWSTR pszName )
: CFilter(clsID, pszName)
{
}

HRESULT CFilterRegister::CreateInstance( LPUNKNOWN pUnk, IBaseFilter** ppv )
{
	return CoCreateInstance(m_clsID, pUnk, CLSCTX_ALL, IID_IBaseFilter, (LPVOID*)ppv);
}

/*[uuid("97f7c4d4-547b-4a5f-8332-536430ad2e4d")]
interface IAMFilterData : public IUnknown
{
	STDMETHOD (ParseFilterData) (BYTE* rgbFilterData, ULONG cb, BYTE** prgbRegFilter2) PURE;
	STDMETHOD (CreateFilterData) (REGFILTER2* prf2, BYTE** prgbFilterData, ULONG* pcb) PURE;
};

void CFilterRegister::ExtractFilterData( BYTE *pData, UINT cbData )
{
	CComPtr<IAMFilterData> pFD;
	BYTE* ptr = NULL;

	if(SUCCEEDED(pFD.CoCreateInstance(CLSID_FilterMapper2))
		&& SUCCEEDED(pFD->ParseFilterData(pData, cbData, (BYTE**)&ptr)))
	{
		REGFILTER2* prf = (REGFILTER2*)*(DWORD*)ptr; // this is f*cked up

		if(prf->dwVersion == 1)
		{
			for(UINT i = 0; i < prf->cPins; i++)
			{
				if(prf->rgPins[i].bOutput)
					continue;

				for(UINT j = 0; j < prf->rgPins[i].nMediaTypes; j++)
				{
					if(!prf->rgPins[i].lpMediaType[j].clsMajorType || !prf->rgPins[i].lpMediaType[j].clsMinorType)
						break;

					const REGPINTYPES& rpt = prf->rgPins[i].lpMediaType[j];
					m_majorTypes[*rpt.clsMajorType][*rpt.clsMinorType] = 0;
				}
			}
		}
		else if(prf->dwVersion == 2)
		{
			for(UINT i = 0; i < prf->cPins2; i++)
			{
				if(prf->rgPins2[i].dwFlags&REG_PINFLAG_B_OUTPUT)
					continue;

				for(UINT j = 0; j < prf->rgPins2[i].nMediaTypes; j++)
				{
					if(!prf->rgPins2[i].lpMediaType[j].clsMajorType || !prf->rgPins2[i].lpMediaType[j].clsMinorType)
						break;

					const REGPINTYPES& rpt = prf->rgPins2[i].lpMediaType[j];
					m_majorTypes[*rpt.clsMajorType][*rpt.clsMinorType] = 0;
				}
			}
		}

		CoTaskMemFree(prf);
	}
	else
	{
		BYTE* base = pData;

#define ChkLen(size) if(pData - base + size > (int)cbData) return;

		ChkLen(4)
		if(*(DWORD*)pData != 0x00000002) return; // only version 2 supported, no samples found for 1
		pData += 4;
		ChkLen(4)
		ChkLen(8)
		DWORD nPins = *(DWORD*)pData; pData += 8;
		while(nPins-- > 0)
		{
			ChkLen(1)
			BYTE n = *pData-0x30; pData++;
			ChkLen(2)
			WORD pi = *(WORD*)pData; pData += 2;
			ASSERT(pi == 'ip');
			ChkLen(1)
			BYTE x33 = *pData; pData++;
			ASSERT(x33 == 0x33);
			ChkLen(8)
			bool fOutput = !!(*pData&REG_PINFLAG_B_OUTPUT);
			pData += 8;
			ChkLen(12)
			DWORD nTypes = *(DWORD*)pData; pData += 12;
			while(nTypes-- > 0)
			{
				ChkLen(1)
				BYTE n = *pData-0x30; pData++;
				ChkLen(2)
				WORD ty = *(WORD*)pData; pData += 2;
				ASSERT(ty == 'yt');
				ChkLen(5)
				BYTE x33 = *pData; pData++;
				ASSERT(x33 == 0x33);
				pData += 4;
				ChkLen(8)
				if(*(DWORD*)pData < (DWORD)(pData-base+8) || *(DWORD*)pData >= cbData 
					|| *(DWORD*)(pData+4) < (DWORD)(pData-base+8) || *(DWORD*)(pData+4) >= cbData)
				{
					pData += 8;
					continue;
				}

				GUID majortype, subtype;
				memcpy(&majortype, &base[*(DWORD*)pData], sizeof(GUID)); pData += 4;
				if(!fOutput) m_majorTypes[majortype][subtype] = 0; 
			}
		}

#undef ChkLen
	}
}*/

