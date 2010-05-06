#pragma once

#include "vgInclude.h"

class vgRegistryKey : public boost::noncopyable
{
public:
	vgRegistryKey(void);
	~vgRegistryKey(void);

	void Close(void);

	operator HKEY(void) const;
	PHKEY operator &(void);
private:
	HKEY m_key;
};