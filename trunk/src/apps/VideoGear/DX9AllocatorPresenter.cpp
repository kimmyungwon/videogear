#include "StdAfx.h"
#include "VideoGear.h"
#include "DX9AllocatorPresenter.h"

CDX9AllocatorPresenter::CDX9AllocatorPresenter( HWND hWnd, LPUNKNOWN pUnk )
: CUnknown(L"CDX9AllocatorPresenter", pUnk), m_hWnd(hWnd)
{
	m_pD3d = Direct3DCreate9(D3D_SDK_VERSION);
}

CDX9AllocatorPresenter::~CDX9AllocatorPresenter(void)
{
	m_pD3d = NULL;
}

HRESULT STDMETHODCALLTYPE CDX9AllocatorPresenter::InitializeDevice( DWORD_PTR dwUserID, VMR9AllocationInfo *lpAllocInfo, 
																   DWORD *lpNumBuffers )
{
	D3DCAPS9 d3dCaps;
	DWORD dwCreateFlags;
	D3DDISPLAYMODE d3dDispMode;
	D3DPRESENT_PARAMETERS d3dParams;
	HRESULT hr;
	
	if (m_pD3dDev != NULL || m_pD3d == NULL)
		return E_UNEXPECTED;
	RIF(m_pD3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps));
	dwCreateFlags = D3DCREATE_MULTITHREADED;
	if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		dwCreateFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		if (d3dCaps.DevCaps & D3DDEVCAPS_PUREDEVICE)
			dwCreateFlags |= D3DCREATE_PUREDEVICE;
	}
	else
		dwCreateFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	RIF(m_pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3dDispMode));
	memset(&d3dParams, 0, sizeof(D3DPRESENT_PARAMETERS));
	d3dParams.BackBufferWidth = d3dDispMode.Width;
	d3dParams.BackBufferHeight = d3dDispMode.Height;
	d3dParams.BackBufferFormat = d3dDispMode.Format;
	d3dParams.BackBufferCount = 1;
	d3dParams.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dParams.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dParams.hDeviceWindow = m_hWnd;
	d3dParams.Windowed = TRUE;
	d3dParams.EnableAutoDepthStencil = FALSE;
	d3dParams.Flags = D3DPRESENTFLAG_VIDEO;
	d3dParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	RIF(m_pD3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL, dwCreateFlags, &d3dParams, &m_pD3dDev));
	RIF(D3DXCreateSprite(m_pD3dDev, &m_pDXSprite));

	lpAllocInfo->dwFlags |= VMR9AllocFlag_TextureSurface;
	m_pD3dSurfaces.SetCount(*lpNumBuffers);
	hr = m_pVMR9SAN->AllocateSurfaceHelper(lpAllocInfo, lpNumBuffers, &m_pD3dSurfaces[0]);
	if (FAILED(hr))
	{
		RIF(m_pD3dDev->CreateOffscreenPlainSurface(lpAllocInfo->dwWidth, lpAllocInfo->dwHeight, lpAllocInfo->Format, 
			D3DPOOL_MANAGED, &m_pD3dPrivateSurface, NULL));
		lpAllocInfo->dwFlags &= ~VMR9AllocFlag_TextureSurface;
		RIF(m_pVMR9SAN->AllocateSurfaceHelper(lpAllocInfo, lpNumBuffers, &m_pD3dSurfaces[0]));
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDX9AllocatorPresenter::TerminateDevice( DWORD_PTR dwID )
{
	m_pD3dSurfaces.RemoveAll();
	m_pDXSprite = NULL;
	m_pD3dDev = NULL;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDX9AllocatorPresenter::GetSurface( DWORD_PTR dwUserID, DWORD SurfaceIndex, DWORD SurfaceFlags, 
															 IDirect3DSurface9 **lplpSurface )
{
	CAutoLock __lock__(&m_csSurfaces);

	if (SurfaceIndex >= m_pD3dSurfaces.GetCount())
		return E_INVALIDARG;
	if (lplpSurface == NULL)
		return E_POINTER;
	return m_pD3dSurfaces[SurfaceIndex].CopyTo(lplpSurface);
}

HRESULT STDMETHODCALLTYPE CDX9AllocatorPresenter::AdviseNotify( IVMRSurfaceAllocatorNotify9 *lpIVMRSurfAllocNotify )
{
	if (m_pVMR9SAN != lpIVMRSurfAllocNotify)
	{
		m_pVMR9SAN = lpIVMRSurfAllocNotify;
		if (m_pD3dDev != NULL)
		{
			ASSERT(m_pD3d != NULL);
			m_pVMR9SAN->SetD3DDevice(m_pD3dDev, m_pD3d->GetAdapterMonitor(D3DADAPTER_DEFAULT));
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDX9AllocatorPresenter::StartPresenting( DWORD_PTR dwUserID )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDX9AllocatorPresenter::StopPresenting( DWORD_PTR dwUserID )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDX9AllocatorPresenter::PresentImage( DWORD_PTR dwUserID, VMR9PresentationInfo *lpPresInfo )
{
	return S_OK;
}