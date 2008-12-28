#include "StdAfx.h"
#include "VGPlayer.h"
#include "FGManager.h"
#include "VideoGear.h"

CVGPlaylist::CVGPlaylist( void )
: m_ulNextElem(0)
{

}

UINT_PTR CVGPlaylist::Add( const CString& strPath )
{
	CAutoLock autoLock(this);

	VGPlaylistElement *pElem = new VGPlaylistElement;
	pElem->strPath = strPath;
	m_elements.push_back(pElem);
	return m_elements.size() - 1;
}

void CVGPlaylist::Clear( void )
{
	CAutoLock autoLock(this);

	while (GetCount() > 0)
		Delete(0);
	m_ulNextElem = 0;
}

void CVGPlaylist::Delete( UINT_PTR ulIndex )
{
	CAutoLock autoLock(this);
	
	if (ulIndex >= m_elements.size())
		return;
	m_elements.erase(m_elements.begin() + ulIndex);
	if (ulIndex > m_ulNextElem)
		m_ulNextElem++;
}

VGPlaylistElement* CVGPlaylist::GetAt( UINT_PTR ulIndex )
{
	ASSERT(ulIndex < m_elements.size());

	CAutoLock autoLock(this);

	return &m_elements[ulIndex];
}

UINT CVGPlaylist::GetCount( void )
{
	CAutoLock autoLock(this);
	
	return m_elements.size();
}

VGPlaylistElement* CVGPlaylist::GetCurrent( void )
{
	ASSERT(m_ulNextElem > 0);

	CAutoLock autoLock(this);

	return &m_elements[m_ulNextElem - 1];
}

UINT_PTR CVGPlaylist::GetNextIndex( void )
{
	CAutoLock autoLock(this);

	return m_ulNextElem++;
}

VGPlaylistElement* CVGPlaylist::GetNext( void )
{
	CAutoLock autoLock(this);
	
	if (m_ulNextElem < m_elements.size())
		return &m_elements[m_ulNextElem++];
	else
		return NULL;
}

VGPlaylistElement* CVGPlaylist::operator[]( UINT_PTR ulIndex )
{
	return GetAt(ulIndex);
}

void CVGPlaylist::SetNextIndex( UINT_PTR ulIndex )
{
	CAutoLock autoLock(this);

	m_ulNextElem = ulIndex;
}


//////////////////////////////////////////////////////////////////////////

CVGPlayer::CVGPlayer(void)
: m_wStatus(stateUninitialized), m_hRenderThrd(NULL)
{
}

CVGPlayer::~CVGPlayer(void)
{
	Stop();
	m_pMC.Release();
	m_pGB.Release();
}

CVGPlaylist& CVGPlayer::GetPlaylist( void )
{
	return m_playlist;
}

HRESULT CVGPlayer::Initialize( HWND hwnd )
{
	HRESULT hr;
	
	if (GetState() != stateUninitialized)
		return E_UNEXPECTED;
	m_hwndOwner = hwnd;
	// ��ʼ��DShow
	m_pGB = new CFilterManager(&hr);
	JIF(hr);
	JIF(m_pGB.QueryInterface(&m_pMC));
	SetState(stateIdle);
	return S_OK;
}

HRESULT CVGPlayer::Stop( void )
{
	if (GetState() == stateUninitialized)
		return E_UNEXPECTED;
	if (GetState() == stateOpening)
	{
		m_pGB->Abort();
		if (m_hRenderThrd != NULL)
		{
			TerminateThread(m_hRenderThrd, 0);
			m_hRenderThrd = NULL;
		}
		SetState(stateIdle);
		Clear();
	}
	else if (GetState() == statePlaying || GetState() == statePaused)
	{
		HRESULT hr;
		
		JIF(m_pMC->Stop());
		SetState(stateIdle);
		Clear();
	}
	return S_OK;
}

DWORD WINAPI RenderProc( LPVOID lpThreadParameter )
{
	ASSERT(lpThreadParameter != NULL);
	((CVGPlayer*)lpThreadParameter)->DoPlay();
	return 0;
}

HRESULT CVGPlayer::Play( UINT_PTR ulIndex )
{
	VGPlaylistElement *pElem;
	
	Stop();
	m_playlist.SetNextIndex(ulIndex);
	pElem = m_playlist.GetNext();
	if (pElem == NULL)
		return E_INVALIDARG;
	m_hRenderThrd = CreateThread(NULL, 0, &RenderProc, (LPVOID)this, 0, NULL);
	if (m_hRenderThrd == NULL)
		return E_FAIL;
	return S_OK;
}

void CVGPlayer::Clear( void )
{
	ASSERT(GetState() == stateIdle);
	BeginEnumFilters(m_pGB, pEnumFilters, pFilter)
		BeginEnumPins(pFilter, pEnumPins, pPin)
			if (m_pGB->IsPinConnected(pPin) != S_OK)
				continue;
			m_pGB->Disconnect(pPin);
		EndEnumPins
		m_pGB->RemoveFilter(pFilter.Detach());
	EndEnumFilters
}

void CVGPlayer::DoPlay( void )
{
	VGPlaylistElement *pElem = m_playlist.GetCurrent();
	ASSERT(pElem != NULL);
	if (FAILED(m_pGB->RenderFile(pElem->strPath, NULL)))
		return;
	SetState(statePaused);
	m_pMC->Run();
	SetState(statePlaying);
}

UINT CVGPlayer::GetState( void )
{
	return m_wStatus;
}

void CVGPlayer::SetState( WORD wStatus )
{
	m_wStatus = wStatus;
	SendNotify(VGM_STATE_CHANGED, (int)m_wStatus);
}

void CVGPlayer::SendNotify( UINT nMsg, int iParam )
{
	SendMessage(m_hwndOwner, WM_VGPLAYER, nMsg, iParam); 
}



