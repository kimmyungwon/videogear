#pragma once

#include "vgInclude.hpp"

VG_NAMESPACE_BEGIN

using namespace multi_index;

enum RegValueType
{
	RegValueType_Normal,
	RegValueType_Real,
};

struct RegValue : public noncopyable
{
	RegValueType m_type;
	wstring m_name;
	DWORD m_dataType;
	string m_data;

	static auto_ptr<RegValue> Create(RegValueType type, const wstring &name, DWORD dataType, BYTE *data, DWORD dataSize)
	{
		auto_ptr<RegValue> value(new RegValue(type, dataType));
		value->m_name = name;
		value->m_dataType = dataType;
		value->m_data.assign(data, data + dataSize);
		return value;
	}

	wstring GetName(void) const
	{
		return to_lower_copy(m_name);
	}
private:
	RegValue(RegValueType type, DWORD dataType)
	{
		m_type = type;
		m_dataType = dataType;
	}
};

typedef multi_index_container<
	RegValue*,
	indexed_by<
		random_access<>,
		ordered_non_unique<member<RegValue, RegValueType, &RegValue::m_type> >,
		ordered_unique<const_mem_fun<RegValue, wstring, &RegValue::GetName> >
	>
> RegValueList;

VG_NAMESPACE_END