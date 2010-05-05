#pragma once

class vgHook;

class vgRegistryHook
{
public:
	static vgRegistryHook& Instance(void);

	~vgRegistryHook(void);

	void Enable(void);
	void Disable(void);
private:
	vgRegistryHook(void);
private:
	vgHook *m_codeHook;
};