#pragma once

class CPacket
{
public:
	CPacket(void);
	virtual ~CPacket(void);
	void Initialize(const BYTE* pData, UINT cbData, double dPTS, double dDuration);
	const BYTE* GetData(void) const	{ return m_pData; }
	UINT GetDataSize(void) const	{ return m_cbData; }
	double GetPTS(void) const		{ return m_dPTS; }
	double GetDuration(void) const	{ return m_dDuration; }
private:
	BYTE* m_pData;
	UINT m_cbData;
	double m_dPTS, m_dDuration;
};

class CFFPacket : public CPacket
{
public:
	CFFPacket(AVRational& timebase, AVPacket* pPacket);
};
