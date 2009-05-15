#include "StdAfx.h"
#include "Playlist.h"
#include "FileUtil.h"

#define AUTOLOCK	CAutoLock __lock__(this);

CPlaylist::CPlaylist(void)
{
}

CPlaylist::~CPlaylist(void)
{
}

UINT CPlaylist::GetCount( void )
{
	AUTOLOCK;
	return m_items.size();
}

const MediaItem* CPlaylist::GetItem( UINT nIndex )
{
	AUTOLOCK;
	if (nIndex < m_items.size())
		return &m_items[nIndex];
	else
		return NULL;
}

UINT CPlaylist::AddFile( LPCWSTR pszFile )
{
	AUTOLOCK;
	MediaItem *pItem;

	pItem = new MediaItem;
	pItem->strFullPath = ExpandFileName(pszFile);
	m_items.push_back(pItem);
	return m_items.size() - 1;
}

void CPlaylist::Remove( UINT nIndex )
{
	AUTOLOCK;
	if (nIndex < m_items.size())
		m_items.erase(m_items.begin() + nIndex);
}

void CPlaylist::RemoveAll( void )
{
	AUTOLOCK;
	while (m_items.size() > 0)
		Remove(m_items.size() - 1);
}