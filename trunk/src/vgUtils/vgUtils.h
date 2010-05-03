#pragma once

template<typename T>
CUnknown* CALLBACK CreateInstance(LPUNKNOWN pUnkOuter, HRESULT *phr)
{
	return new T(pUnkOuter, phr);
}