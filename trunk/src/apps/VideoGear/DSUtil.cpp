#include "stdafx.h"
#include "DSUtil.h"

bool IsPinConnected(IPin *pPin)
{
	ASSERT(pPin != NULL);

	CComPtr<IPin> ppinTo;

	return SUCCEEDED(pPin->ConnectedTo(&ppinTo)) && ppinTo != NULL;
}

bool IsPinDir(IPin *pPin, PIN_DIRECTION dir)
{
	ASSERT(pPin != NULL);

	PIN_DIRECTION pinDir;

	return SUCCEEDED(pPin->QueryDirection(&pinDir)) && pinDir == dir;
}