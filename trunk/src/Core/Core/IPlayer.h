#pragma once

enum 
{
	PS_UNINITIALIZED,
	PS_STOPPED,
	PS_PLAYING,
	PS_PAUSE
};

enum
{
	VR_DEFAULT
};

enum 
{
	TF_FRAME,
	TF_TIME
};

[uuid("588BC08D-103A-4fce-B23D-E4DC8B2EC3C6")]
interface IPlayer : IUnknown
{
	STDMETHOD(Initialize)(__in HWND hVidWnd) PURE;
	STDMETHOD(Uninitialize)(void) PURE;
	STDMETHOD(OpenUrl)(__in LPCWSTR lpcwstrUrl) PURE;
	STDMETHOD(Play)(void) PURE;
	STDMETHOD(Pause)(void) PURE;
	STDMETHOD(Stop)(void) PURE;
	STDMETHOD_(ULONG, get_State)(void) PURE;
	STDMETHOD_(ULONG, get_VideoRenderer)(void) PURE;
	STDMETHOD(set_VideoRenderer)(__in ULONG ulVal) PURE;
};
