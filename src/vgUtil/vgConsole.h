#pragma once

#include "vgInclude.hpp"

VG_NAMESPACE_BEGIN

struct Console
{
	static Console& GetInstance(void);

	void Print(const wchar_t *format, ...);
private:
	Console(void);
private:
	HANDLE m_outputHandle;
};

VG_NAMESPACE_END