#pragma once

enum PlayerState : UINT
{
	psUninitialized,
	psStopped,
	psPlaying,
	psPause
};

enum VideoRenderer : UINT
{
	vrDefault
};

[uuid("588BC08D-103A-4fce-B23D-E4DC8B2EC3C6")]
interface IPlayer : IUnknown
{
	STDMETHOD(Initialize)(__in HWND hVidWnd) PURE;
	STDMETHOD_(PlayerState, get_State)(void) PURE;
	STDMETHOD_(VideoRenderer, get_VideoRenderer)(void) PURE;
	STDMETHOD(set_VideoRenderer)(__in VideoRenderer vr) PURE;
};
