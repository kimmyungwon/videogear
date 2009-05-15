#pragma once

#include "Thread.h"

struct MediaItem
{
	CStringW strFullPath;
};

class CPlaylist : public CCritSec
{
public:
	CPlaylist(void);
	virtual ~CPlaylist(void);
	UINT GetCount(void);
	const MediaItem* GetItem(UINT nIndex);
	UINT AddFile(LPCWSTR pszFile);
	void Remove(UINT nIndex);
	void RemoveAll(void);
private:
	typedef boost::ptr_vector<MediaItem> ItemList;

	ItemList m_items;
};
