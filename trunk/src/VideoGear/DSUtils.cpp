#include "stdafx.h"
#include "DSUtils.h"

CString GetFilterName(IBaseFilter* pFilter)
{
	FILTER_INFO info;

	if (pFilter == NULL || FAILED(pFilter->QueryFilterInfo(&info)))
		return _T("");
	return CString(info.achName);
}

int GetGCD(int n1, int n2)
{
	if (n1 == 0)
		return n2;
	else if (n2 == 0)
		return n1;
	for (int i = n1 % n2; i > 0; i = n1 % n2)
	{
		n1 = n2;
		n2 = i;
	}
	return n2;
}