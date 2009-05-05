#include "StdAfx.h"
#include "Source.h"

int CSource::FFReadPacket( void *opaque, uint8_t *buf, int buf_size )
{
	if (opaque != NULL && buf_size > 0)
		return (int)((CSource*)opaque)->Read((BYTE*)buf, buf_size);
	else
		return 0;
}

int CSource::FFWritePacket( void *opaque, uint8_t *buf, int buf_size )
{
	if (opaque != NULL && buf_size > 0)
		return (int)((CSource*)opaque)->Write((BYTE*)buf, buf_size);
	else
		return 0;	
}

int64_t CSource::FFSeek( void *opaque, int64_t offset, int whence )
{
	if (opaque != NULL)
		return ((CSource*)opaque)->Seek((LONGLONG)offset, whence);
	else
		return -1;
}

CSource::CSource(void)
: m_pFFByteIO(NULL)
{
}

CSource::~CSource(void)
{
	Uninitialize();
}

HRESULT CSource::Initialize( void )
{
	if (m_pFFByteIO != NULL)
		Uninitialize();

	UINT nSize;
	BYTE* pBuffer;

	nSize = GetMaxPacketSize();
	if (nSize == 0)
		nSize = SOURCE_BUFFER_SIZE;
	pBuffer = (BYTE*)av_mallocz(nSize > 0 ? nSize : SOURCE_BUFFER_SIZE);
	m_pFFByteIO = av_alloc_put_byte(pBuffer, nSize, 0, this, FFReadPacket, FFWritePacket, FFSeek);
	if (m_pFFByteIO != NULL)
	{
		return S_OK;
	}
	else
	{
		av_free(pBuffer);
		return E_OUTOFMEMORY;
	}
}

void CSource::Uninitialize( void )
{
	if (m_pFFByteIO != NULL)
	{
		if (m_pFFByteIO->buffer != NULL)
		{
			av_free(m_pFFByteIO->buffer); 
			m_pFFByteIO->buffer = NULL;
		}
		av_free(m_pFFByteIO);
		m_pFFByteIO = NULL;
	}
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
	Uninitialize();

	m_hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		HRESULT hr = Initialize();
		if (SUCCEEDED(hr))
			m_strFileName = pszFileName;
		else
		{
			CloseHandle(m_hFile);
			m_hFile = NULL;
		}
		return hr;
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