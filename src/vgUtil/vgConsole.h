#pragma once

#include "vgInclude.h"

struct vgConsole
{
	static vgConsole& GetInstance(void);

	void Print(const wchar_t *format, ...);
private:
	vgConsole(void);
private:
	HANDLE m_outputHandle;
};