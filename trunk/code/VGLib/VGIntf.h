#pragma once

#define VG_DECLARE_IUNKNOWN(class_name)	\
	private:	\
		volatile LONG m_cRef;	\
	public:	\
		virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, __RPC__deref_out void **ppvObject );	\
		virtual ULONG STDMETHODCALLTYPE AddRef( void );	\
		virtual ULONG STDMETHODCALLTYPE Release( void );

#define VG_IMPLEMENT_CONSTRUCTOR_BEGIN(class_name) class_name::class_name(void) :m_cRef(0) {
#define VG_IMPLEMENT_CONSTRUCTOR_END(class_name) }
#define VG_IMPLEMENT_DESTRUCTOR_BEGIN(class_name) class_name::~class_name(void) {
#define VG_IMPLEMENT_DESTRUCTOR_END(class_name) InterlockedDecrement(&m_cRef); }
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

interface DECLSPEC_UUID("30377130-AC30-49e6-B0CD-2127A2343EE4") IVGFilterManager;
interface DECLSPEC_UUID("23333A3C-A9D5-4978-9365-FC81A0B1FD6F") IVGPlayer;

#if defined(_COM_SMARTPTR_TYPEDEF)
_COM_SMARTPTR_TYPEDEF(IVGFilterManager, __uuidof(IVGFilterManager));
_COM_SMARTPTR_TYPEDEF(IVGPlayer, __uuidof(IVGPlayer));
#endif

typedef interface IVGFilterManager		IVGFilterManager;
typedef interface IVGPlayer				IVGPlayer;

#undef  INTERFACE
#define INTERFACE   IVGFilterManager

DECLARE_INTERFACE_(IVGFilterManager, IUnknown)
{
	/* IUnknown */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, __RPC__deref_out void **ppvObject ) = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef( void ) = 0;
	virtual ULONG STDMETHODCALLTYPE Release( void ) = 0;

	/* IVGFilterManager */
	virtual HRESULT STDMETHODCALLTYPE Initialize(void) = 0;
};

#undef  INTERFACE
#define INTERFACE   IVGPlayer

DECLARE_INTERFACE_(IVGPlayer, IUnknown)
{
	/* IUnknown */
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, __RPC__deref_out void **ppvObject ) = 0;
	virtual ULONG STDMETHODCALLTYPE AddRef( void ) = 0;
	virtual ULONG STDMETHODCALLTYPE Release( void ) = 0;

	/* IVGPlayer */
	virtual HRESULT STDMETHODCALLTYPE Initialize(void) = 0;
};