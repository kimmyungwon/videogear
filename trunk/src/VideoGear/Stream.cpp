#include "StdAfx.h"
#include "Stream.h"

CFileStream::CFileStream( void )
: m_hFile(INVALID_HANDLE_VALUE)
{

}

CFileStream::~CFileStream( void )
{
	Close();
}

HRESULT CFileStream::Open( LPCTSTR pszFileName )
{
	Close();

	m_hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		m_strFileName = pszFileName;
		return S_OK;
	}
	else
		return HRESULT_FROM_WIN32(GetLastError());
}

void CFileStream::Close( void )
{
	m_strFileName.Empty();
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

UINT CFileStream::Read( BYTE* pBuffer, UINT cbBuffer )
{
	if (m_hFile != NULL)
	{
		DWORD dwRead = 0;
		ReadFile(m_hFile, (LPVOID)pBuffer, cbBuffer, &dwRead, NULL);
		return dwRead;
	}
	else
		return 0;
}

UINT CFileStream::Write( const BYTE* pBuffer, UINT cbBuffer )
{
	if (m_hFile != NULL)
	{
		DWORD dwWritten = 0;
		WriteFile(m_hFile, (LPVOID)pBuffer, cbBuffer, &dwWritten, NULL);
		return dwWritten;
	}
	else
		return 0;
}

LONGLONG CFileStream::Seek( LONGLONG llOffset, int nOrigin )
{
	if (m_hFile != NULL)
	{
		LARGE_INTEGER liOffset, liPos;

		liOffset.QuadPart = llOffset;
		if (SetFilePointerEx(m_hFile, liOffset, &liPos, nOrigin))
			return liPos.QuadPart;
		else
			return -1;
	}
	else
		return -1;
}