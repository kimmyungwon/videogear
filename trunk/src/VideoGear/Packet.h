#pragma once

class CPacket
{
public:
	CPacket(void);
	virtual ~CPacket(void);
	void Initialize(const BYTE* pData, UINT cbData, LONGLONG llPTS, LONGLONG llDuration);
	const BYTE* GetData(void) const	{ return m_pData; }
	UINT GetDataSize(void) const	{ return m_cbData; }
	LONGLONG GetPTS(void) const		{ return m_llPTS; }
	LONGLONG GetDuration(void) const	{ return m_llDuration; }
private:
	BYTE* m_pData;
	UINT m_cbData;
	LONGLONG m_llPTS, m_llDuration;
};

class CFFPacket : public CPacket
{
public:
	CFFPacket(AVPacket* pPacket);
};
