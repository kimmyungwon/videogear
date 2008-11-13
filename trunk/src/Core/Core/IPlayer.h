#pragma once

#define WM_VGPLAYER	(WM_APP + 0x100)

enum 
{
	PS_UNINITIALIZED,
	PS_IDLE,
	PS_RENDERING,
	PS_STOPPED,
	PS_PLAYING,
	PS_PAUSE
};

enum
{
	VR_DEFAULT,
	VR_VMR9Windowed,
	VR_VMR9Renderless
};

enum 
{
	TF_FRAME,
	TF_TIME
};

[uuid("601805E8-17BF-4050-8899-176843AC42BB")]
interface IPlayerConfig : IUnknown
{
	STDMETHOD_(ULONG, get_VideoOutputMode)(void) PURE;
	STDMETHOD(set_VideoOutputMode)(ULONG ulVal) PURE;
};

[uuid("588BC08D-103A-4FCE-B23D-E4DC8B2EC3C6")]
interface IPlayer : IUnknown
{
	STDMETHOD(Initialize)(__in HWND hVidWnd, __in_opt HWND hMsgWnd) PURE;
	STDMETHOD(Uninitialize)(void) PURE;
	STDMETHOD(OpenUrl)(__in LPCWSTR lpcwstrUrl) PURE;
	STDMETHOD(Play)(void) PURE;
	STDMETHOD(Pause)(void) PURE;
	STDMETHOD(Stop)(void) PURE;
	STDMETHOD_(ULONG, get_State)(void) PURE;
};
