#pragma once

#include "Thread.h"

class CDX9AllocatorPresenter
	: public CUnknown
	, public IVMRSurfaceAllocator9
	, public IVMRImagePresenter9
	, public IVMRWindowlessControl9
{
public:
	DECLARE_IUNKNOWN
	CDX9AllocatorPresenter(HWND hWnd, LPUNKNOWN pUnk);
	virtual ~CDX9AllocatorPresenter(void);
	/* CUnknown */
	virtual HRESULT STDMETHODCALLTYPE NonDelegatingQueryInterface(REFIID riid, __deref_out void ** ppv);
	/* IVMRSurfaceAllocator9 */
	virtual HRESULT STDMETHODCALLTYPE InitializeDevice(DWORD_PTR dwUserID, VMR9AllocationInfo *lpAllocInfo,
		DWORD *lpNumBuffers);
	virtual HRESULT STDMETHODCALLTYPE TerminateDevice(DWORD_PTR dwID);
	virtual HRESULT STDMETHODCALLTYPE GetSurface(DWORD_PTR dwUserID, DWORD SurfaceIndex, DWORD SurfaceFlags,
		IDirect3DSurface9 **lplpSurface);
	virtual HRESULT STDMETHODCALLTYPE AdviseNotify(IVMRSurfaceAllocatorNotify9 *lpIVMRSurfAllocNotify);
	/* IVMRImagePresenter9 */
	virtual HRESULT STDMETHODCALLTYPE StartPresenting(DWORD_PTR dwUserID);
	virtual HRESULT STDMETHODCALLTYPE StopPresenting(DWORD_PTR dwUserID);
	virtual HRESULT STDMETHODCALLTYPE PresentImage(DWORD_PTR dwUserID, VMR9PresentationInfo *lpPresInfo);
	/* IVMRWindowlessControl9 */
	virtual HRESULT STDMETHODCALLTYPE GetNativeVideoSize(LONG *lpWidth, LONG *lpHeight, LONG *lpARWidth, LONG *lpARHeight);
	virtual HRESULT STDMETHODCALLTYPE GetMinIdealVideoSize(LONG *lpWidth, LONG *lpHeight)	{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE GetMaxIdealVideoSize(LONG *lpWidth, LONG *lpHeight)	{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE SetVideoPosition(const LPRECT lpSRCRect, const LPRECT lpDSTRect);
	virtual HRESULT STDMETHODCALLTYPE GetVideoPosition(LPRECT lpSRCRect, LPRECT lpDSTRect)	{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE GetAspectRatioMode(DWORD *lpAspectRatioMode)	{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE SetAspectRatioMode(DWORD AspectRatioMode)	{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE SetVideoClippingWindow(HWND hwnd)	{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE RepaintVideo(HWND hwnd, HDC hdc)	{ return S_OK; }
	virtual HRESULT STDMETHODCALLTYPE DisplayModeChanged(void)	{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE GetCurrentImage(BYTE **lpDib)	{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE SetBorderColor(COLORREF Clr)	{ return E_NOTIMPL; }
	virtual HRESULT STDMETHODCALLTYPE GetBorderColor(COLORREF *lpClr)	{ return E_NOTIMPL; }
private:
	HWND m_hWnd;
	CComPtr<IVMRSurfaceAllocatorNotify9> m_pVMR9SAN;
	CSize m_szNative, m_szAspectRatio;
	CComPtr<IDirect3D9> m_pD3d;
	CComPtr<IDirect3DDevice9> m_pD3dDev;
	CComPtr<ID3DXSprite> m_pDXSprite;
	CAtlArray<CComPtr<IDirect3DSurface9> > m_pD3dSurfaces;
	CCritSec m_csSurfaces;
	CComPtr<IDirect3DSurface9> m_pD3dPrivateSurface;
	CRect m_rcSrc, m_rcDst;
};
