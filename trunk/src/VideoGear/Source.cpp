#include "stdafx.h"
#include "Source.h"

CStringA CSource::GetNameA( void )
{
	USES_CONVERSION;
	return T2A(GetName());
}

//////////////////////////////////////////////////////////////////////////

CFileSource::CFileSource( void )
: m_hFile(INVALID_HANDLE_VALUE)
{

}

CFileSource::~CFileSource( void )
{
	Close();
}

HRESULT CFileSource::Open( LPCTSTR pszFileName )
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

void CFileSource::Close( void )
{
	m_strFileName.Empty();
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

UINT CFileSource::Read( BYTE* pBuffer, UINT cbBuffer )
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

UINT CFileSource::Write( const BYTE* pBuffer, UINT cbBuffer )
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

LONGLONG CFileSource::Seek( LONGLONG llOffset, int nOrigin )
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

