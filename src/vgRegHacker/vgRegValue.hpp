#pragma once

#include "vgInclude.hpp"

VG_NAMESPACE_BEGIN

using namespace multi_index;

struct RegValue
{
	wstring m_name;
	any m_value;

	wstring GetName(void) const
	{
		return to_lower_copy(m_name);
	}
};

typedef multi_index_container<
	RegValue*,
	indexed_by<
		random_access<>,
		ordered_unique<const_mem_fun<RegValue, wstring, &RegValue::GetName> >
	>
> RegValueList;

VG_NAMESPACE_END