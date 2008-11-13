#pragma once

[uuid("A9139C3B-0E71-4704-98FC-6FB66D53EDD7")]
interface IGraphBuilder2 : public IFilterGraph2
{
	STDMETHOD(Clear)(void) PURE;
};

[uuid("FB747D95-8A05-49a8-B644-4FE066A90C52")]
interface IGraphBuilderDeadEnd : public IUnknown
{
	STDMETHOD_(size_t, GetCount)(void) PURE;
	STDMETHOD(GetDeadEnd)(__in size_t iIndex, __out AM_MEDIA_TYPE** ppMTs, __out size_t cMTs) PURE;
};