#include "stdafx.h"
#include "vgRegistryKey.h"
#include "vgRegistry.h"

vgRegistryKey::vgRegistryKey( void )
{
	m_key = NULL;
}

vgRegistryKey::~vgRegistryKey( void )
{
	Close();
}

void vgRegistryKey::Close( void )
{
	if (m_key != NULL)
	{
		vgRegistry::Real_RegCloseKey(m_key);
		m_key = NULL;
	}
}

vgRegistryKey::operator HKEY( void ) const
{
	return m_key;
}

PHKEY vgRegistryKey::operator&( void )
{
	assert(m_key == NULL);
	return &m_key;
}

