#pragma once

#include "IGraphBuilder2.h"

using namespace boost;

struct VGPlaylistElement
{
	CString strPath;
};

class CVGPlaylist : public CCritSec
{
public:
	CVGPlaylist(void);
	UINT_PTR Add(const CString& strPath);
	void Clear(void);
	void Delete(UINT_PTR ulIndex);
	VGPlaylistElement* GetAt(UINT_PTR ulIndex);
	UINT GetCount(void);
	VGPlaylistElement* GetCurrent(void);
	VGPlaylistElement* GetNext(void);
	UINT_PTR GetNextIndex(void);
	void SetNextIndex(UINT_PTR ulIndex);

	VGPlaylistElement* operator[](UINT_PTR ulIndex);
private:
	typedef ptr_vector<VGPlaylistElement> elements_t;

	elements_t	m_elements;
	UINT_PTR	m_ulNextElem;
};

#define WM_VGPLAYER	WM_APP + 0x100

enum {
	VGM_STATE_CHANGED
};

class CVGPlayer
{
	friend DWORD WINAPI RenderProc(LPVOID);
public:
	enum {
		stateUninitialized,
		stateIdle,
		stateOpening,
		statePlaying,
		statePaused
	};

	CVGPlayer(void);
	virtual ~CVGPlayer(void);
	CVGPlaylist& GetPlaylist(void);
	HRESULT Initialize(HWND hwnd);
	HRESULT Stop(void);
	HRESULT Play(UINT_PTR ulIndex);
protected:
	void Clear(void);
	void DoPlay(void);
	UINT GetState(void);
	void SetState(WORD wStatus);
	void SendNotify(UINT nMsg, int nParam = 0);
private:
	WORD			m_wStatus;
	HWND			m_hwndOwner;
	CComPtr<IGraphBuilder2>	m_pGB;
	CComPtr<IMediaControl>	m_pMC;
	CVGPlaylist		m_playlist;
	HANDLE			m_hRenderThrd; 
};
