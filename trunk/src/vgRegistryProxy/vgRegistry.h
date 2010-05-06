#pragma once

#include "vgInclude.h"
#include "vgRegistryTree.h"

#define VG_REGISTRY_API \
	(RegCloseKey) \
	(RegCreateKeyExA) \
	(RegCreateKeyExW) \
	(RegOpenKeyExA) \
	(RegOpenKeyExW)

class vgRegistry : public vgRegistryTree
{
public:
#define MACRO(r, data, elem) static BOOST_TYPEOF(&elem) BOOST_PP_CAT(Real_, elem);
BOOST_PP_SEQ_FOR_EACH(MACRO, _, VG_REGISTRY_API)
#undef MACRO
public:
	vgRegistry(void);
	~vgRegistry(void);
protected:
	virtual vgRegistryNode* InternalCreateNode(vgRegistryNode *parent, const wstring &name);
};

