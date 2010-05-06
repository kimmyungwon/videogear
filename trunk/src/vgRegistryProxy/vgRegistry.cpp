#include "StdAfx.h"
#include "vgRegistry.h"

#define MACRO(r, data, elem) BOOST_TYPEOF(&elem) vgRegistry::BOOST_PP_CAT(Real_, elem) = elem;
BOOST_PP_SEQ_FOR_EACH(MACRO, _, VG_REGISTRY_API)
#undef MACRO

vgRegistry::vgRegistry(void)
{
}


vgRegistry::~vgRegistry(void)
{
}

vgRegistryNode* vgRegistry::InternalCreateNode(vgRegistryNode *parent, const wstring &name)
{
	vgRegistryNode *newNode = __super::InternalCreateNode(parent, name);
	RegOpenKeyExW(newNode->m_rootKey, InternalGetPath(newNode).c_str(), 0, KEY_READ, &newNode->m_realKey);
	return newNode;
}