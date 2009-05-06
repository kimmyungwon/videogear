#pragma once

class CPacket
{
public:
	enum {
		TYPE_UNKNOWN = -1,
		TYPE_VIDEO,
		TYPE_AUDIO,
		TYPE_SUBTITLE
	};

	CPacket(int nType);
	virtual ~CPacket(void);
	int GetType(void) const	{ return m_nType; }
private:
	int m_nType;
};

class CFFPacket : public CPacket
{
public:
	CFFPacket(int nType);
};
