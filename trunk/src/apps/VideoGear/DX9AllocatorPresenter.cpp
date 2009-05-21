#include "StdAfx.h"
#include "VideoGear.h"
#include "DX9AllocatorPresenter.h"
#include "DSUtil.h"

CDX9AllocatorPresenter::CDX9AllocatorPresenter( HWND hWnd, LPUNKNOWN pUnk )
: CUnknown(L"CDX9AllocatorPresenter", pUnk), m_hWnd(hWnd)
{
	m_pD3d = Direct3DCreate9(D3D_SDK_VERSION);
}

CDX9AllocatorPresenter::~CDX9AllocatorPresenter(void)
{
	m_pD3d = NULL;
}

HRESULT STDMETHODCALLTYPE CDX9AllocatorPresenter::NonDelegatingQueryInterface( REFIID riid, __deref_out void ** ppv )
{
	return	QI(IVMRSurfaceAllocator9)
			QI(IVMRImagePresenter9)
			QI(IVMRWindowlessControl9)
			CUnknown::NonDelegatingQueryInterface(riid, ppv);
}

HRESULT STDMETHODCALLTYPE CDX9AllocatorPresenter::InitializeDevice( DWORD_PTR dwUserID, VMR9AllocationInfo *lpAllocInfo, 
																   DWORD *lpNumBuffers )
{
	D3DCAPS9 d3dCaps;
	DWORD dwCreateFlags;
	D3DDISPLAYMODE d3dDispMode;
	D3DPRESENT_PARAMETERS d3dParams;
	
	if (m_pD3dDev != NULL || m_pD3d == NULL)
		return E_UNEXPECTED;
	m_szNative = lpAllocInfo->szNativeSize;
	m_szAspectRatio = lpAllocInfo->szAspectRatio;
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
	RIF(m_pD3dDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_GAUSSIANQUAD));
	RIF(m_pD3dDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_GAUSSIANQUAD));
	RIF(D3DXCreateSprite(m_pD3dDev, &m_pDXSprite));
	if (m_pVMR9SAN != NULL)
		m_pVMR9SAN->SetD3DDevice(m_pD3dDev, m_pD3d->GetAdapterMonitor(D3DADAPTER_DEFAULT));

	m_pD3dSurfaces.SetCount(*lpNumBuffers);
	return m_pVMR9SAN->AllocateSurfaceHelper(lpAllocInfo, lpNumBuffers, &m_pD3dSurfaces[0]);
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
	CComPtr<IDirect3DSurface9> pScreen;
	
	RIF(m_pD3dDev->GetRenderTarget(0, &pScreen));
	RIF(m_pD3dDev->BeginScene());
	m_pD3dDev->StretchRect(lpPresInfo->lpSurf, m_rcSrc, pScreen, m_rcDst, D3DTEXF_LINEAR);
	m_pD3dDev->EndScene();
	m_pD3dDev->Present(NULL, NULL, NULL, NULL);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDX9AllocatorPresenter::GetNativeVideoSize( LONG *lpWidth, LONG *lpHeight, LONG *lpARWidth, LONG *lpARHeight )
{
	if (lpWidth != NULL)
		*lpWidth = m_szNative.cx;
	if (lpHeight != NULL)
		*lpHeight = m_szNative.cy;
	if (lpARWidth != NULL)
		*lpARWidth = m_szAspectRatio.cx;
	if (lpARHeight != NULL)
		*lpARHeight = m_szAspectRatio.cy;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDX9AllocatorPresenter::SetVideoPosition( const LPRECT lpSRCRect, const LPRECT lpDSTRect )
{
	if (lpSRCRect != NULL)
		m_rcSrc = lpSRCRect;
	else
		m_rcSrc = CRect(CPoint(0, 0), m_szNative);
	m_rcDst = lpDSTRect;
	return S_OK;
}