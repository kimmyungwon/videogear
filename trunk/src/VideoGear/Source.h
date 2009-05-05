#pragma once

#define SOURCE_BUFFER_SIZE	32768

class CSource : public CObject
{
protected:
	static int FFReadPacket(void *opaque, uint8_t *buf, int buf_size);
	static int FFWritePacket(void *opaque, uint8_t *buf, int buf_size);
	static int64_t FFSeek(void *opaque, int64_t offset, int whence);
	static int FFReadPause(void *opaque, int pause);
	static int64_t FFReadSeek(void *opaque, int stream_index, int64_t timestamp, int flags);
public:
	CSource(void);
	virtual ~CSource(void);
	virtual HRESULT Open(LPCTSTR pszName) = 0;
	virtual void Close(void) = 0;
	virtual CString GetName(void) = 0;
	ByteIOContext* GetIOContext(void)	{ return m_pFFByteIO; }
protected:
	HRESULT Initialize(void);
	void Uninitialize(void);
	virtual UINT GetMaxPacketSize(void) = 0;
	virtual UINT Read(BYTE* pBuffer, UINT cbBuffer) = 0;
	virtual UINT Write(const BYTE* pBuffer, UINT cbBuffer) = 0;
	virtual LONGLONG Seek(LONGLONG llOffset, int nOrigin) = 0;
private:
	ByteIOContext* m_pFFByteIO;
};

class CFileSource : public CSource
{
public:
	CFileSource(void);
	virtual ~CFileSource(void);
	virtual HRESULT Open(LPCTSTR pszFileName);
	virtual void Close(void);
	virtual CString GetName(void)	{ return m_strFileName; }
protected:
	virtual UINT GetMaxPacketSize(void)	{ return 0; }
	virtual UINT Read(BYTE* pBuffer, UINT cbBuffer);
	virtual UINT Write(const BYTE* pBuffer, UINT cbBuffer);
		virtual LONGLONG Seek(LONGLONG llOffset, int nOrigin);
private:
	CString m_strFileName;
	HANDLE m_hFile;
};
