#pragma once

class CStream
{
public:
	virtual ~CStream(void)	{}
	virtual HRESULT Open(LPCTSTR pszName) = 0;
	virtual void Close(void) = 0;
	virtual const CString& GetName(void) const = 0;
	virtual UINT GetMaxPacketSize(void) = 0;
	virtual UINT Read(BYTE* pBuffer, UINT cbBuffer) = 0;
	virtual UINT Write(const BYTE* pBuffer, UINT cbBuffer) = 0;
	virtual LONGLONG Seek(LONGLONG llOffset, int nOrigin) = 0;
};

class CFileStream : public CStream
{
public:
	CFileStream(void);
	virtual ~CFileStream(void);
	virtual HRESULT Open(LPCTSTR pszFileName);
	virtual void Close(void);
	virtual const CString& GetName(void) const	{ return m_strFileName; }
	virtual UINT GetMaxPacketSize(void)	{ return 0; }
	virtual UINT Read(BYTE* pBuffer, UINT cbBuffer);
	virtual UINT Write(const BYTE* pBuffer, UINT cbBuffer);
	virtual LONGLONG Seek(LONGLONG llOffset, int nOrigin);
private:
	CString m_strFileName;
	HANDLE m_hFile;
};