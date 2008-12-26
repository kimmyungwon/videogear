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

private:
	typedef ptr_vector<VGPlaylistElement> elements_t;

	elements_t	m_elements;
};

class CVGPlayer
{
public:
	CVGPlayer(void);
	virtual ~CVGPlayer(void);
	HRESULT Initialize(HWND hwnd);
protected:
	void Clear(void);
private:
	CComPtr<IGraphBuilder2>	m_pGB;
	CVGPlaylist				m_playlist;
};
