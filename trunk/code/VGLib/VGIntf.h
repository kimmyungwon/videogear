#pragma once

#define VG_DECLARE_IUNKNOWN(class_name)	\
	private:	\
		volatile LONG m_cRef;	\
	public:	\
		virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, __RPC__deref_out void **ppvObject );	\
		virtual ULONG STDMETHODCALLTYPE AddRef( void );	\
		virtual ULONG STDMETHODCALLTYPE Release( void );

#define VG_IMPLEMENT_ADDREF(class_name) ULONG STDMETHODCALLTYPE class_name::AddRef( void ) {	\
											return InterlockedIncrement(&m_cRef); } 
#define VG_IMPLEMENT_RELEASE(class_name) ULONG STDMETHODCALLTYPE class_name::Release( void ) {	\
											LONG lRef = InterlockedDecrement(&m_cRef);			\
											if (lRef == 0) { delete this; }						\
											return lRef; }
#define VG_IMPLEMENT_QI(class_name) HRESULT STDMETHODCALLTYPE class_name::QueryInterface(	\
										REFIID riid, __RPC__deref_out void **ppvObject ) {	\
										if (riid == IID_IUnknown) {							\
											GetInterface((LPUNKNOWN)this, ppvObject);		\
											return S_OK; }									\
										return E_NOINTERFACE; }
										

//////////////////////////////////////////////////////////////////////////

#undef  INTERFACE
#define INTERFACE   IVGPlayer

DECLARE_INTERFACE_IID_(IVGPlayer, IUnknown, "{23333A3C-A9D5-4978-9365-FC81A0B1FD6F}")
{
public:
	BEGIN_INTERFACE

	/* IUnknown */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, __RPC__deref_out void **ppvObject ) = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef( void ) = 0;
	virtual ULONG STDMETHODCALLTYPE Release( void ) = 0;

	END_INTERFACE
};

#undef  INTERFACE
#define INTERFACE   IVGFilterManager

DECLARE_INTERFACE_IID_(IVGFilterManager, IUnknown, "{30377130-AC30-49e6-B0CD-2127A2343EE4}")
{
	BEGIN_INTERFACE

	/* IUnknown */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, __RPC__deref_out void **ppvObject ) = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef( void ) = 0;
	virtual ULONG STDMETHODCALLTYPE Release( void ) = 0;

	END_INTERFACE	
};