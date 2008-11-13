#include "stdafx.h"
#include "Utils.h"

HRESULT FindVideoRenderer(IFilterGraph *pFG, IBaseFilter **ppRenderer)
{
	CheckPointer(pFG, E_POINTER);
	CheckPointer(ppRenderer, E_POINTER);

	BeginEnumFilters(pFG, pEnumFilters, pFilter)
		FILTER_INFO fi;
		PIN_DIRECTION pinDir;
		AM_MEDIA_TYPE mt;
		bool bIsVideo = false;
		ULONG ulIn = 0, ulOut = 0;
		
		pFilter->QueryFilterInfo(&fi);
		TRACE(L"%s\n", fi.achName);
		BeginEnumPins(pFilter, pEnumPins, pPin)
			pPin->QueryDirection(&pinDir);
			if (pinDir == PINDIR_INPUT)
			{
				ulIn++;
				if (pPin->ConnectionMediaType(&mt) == S_OK)
				{
					if (mt.majortype == MEDIATYPE_Video) bIsVideo = true;
					else bIsVideo = false;
					FreeMediaType(mt);
					if (!bIsVideo) break;
				}
			}
			else
				ulOut++;
		EndEnumPins
		if (ulIn != 0 && ulOut == 0 && bIsVideo)
		{
			*ppRenderer = pFilter;
			return S_OK;
		}
	EndEnumFilters

	return E_FAIL;
}