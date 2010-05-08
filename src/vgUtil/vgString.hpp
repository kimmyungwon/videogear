#pragma once

#include "vgInclude.hpp"

VG_NAMESPACE_BEGIN

class String
{
public:
	static void Split(const wstring &str, const wchar_t *delims, vector<wstring> &result)
	{
		typedef tokenizer<char_separator<wchar_t>, wstring::const_iterator, wstring> tokenizer;
		char_separator<wchar_t> sep(delims);
		tokenizer tokens(str, sep);
		for (tokenizer::const_iterator iter = tokens.begin(); iter != tokens.end(); iter++)
			result.push_back(*iter);
	}
};

VG_NAMESPACE_END