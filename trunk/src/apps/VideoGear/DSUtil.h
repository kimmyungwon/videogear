#pragma once

#include "..\..\dsutil\DSUtil.h"

HRESULT AddToROT(IFilterGraph *pFG);
bool IsSourceFilter(IBaseFilter *pFilter);
bool IsPinConnected(IPin *pPin);
bool IsPinDir(IPin *pPin, PIN_DIRECTION dir);
void CStringToBin(const CStringW &str, std::string &bytes);