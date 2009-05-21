#pragma once

#include "Thread.h"

class CDX9AllocatorPresenter
	: public CUnknown
	, public IVMRSurfaceAllocator9
	, public IVMRImagePresenter9
{
public:
	DECLARE_IUNKNOWN
	CDX9AllocatorPresenter(HWND hWnd, LPUNKNOWN pUnk);
	virtual ~CDX9AllocatorPresenter(void);
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
private:
	HWND m_hWnd;
	CComPtr<IVMRSurfaceAllocatorNotify9> m_pVMR9SAN;
	CComPtr<IDirect3D9> m_pD3d;
	CComPtr<IDirect3DDevice9> m_pD3dDev;
	CComPtr<ID3DXSprite> m_pDXSprite;
	CAtlArray<CComPtr<IDirect3DSurface9> > m_pD3dSurfaces;
	CCritSec m_csSurfaces;
	CComPtr<IDirect3DSurface9> m_pD3dPrivateSurface;
};
