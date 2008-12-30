#pragma once

#pragma pack(push, 1)

struct LongJump
{
	BYTE opcode;
	int operand;
};

#pragma pack(pop)

template<typename FuncT>
HRESULT HookAPI(FuncT& pfnReal, FuncT pfnHook)
{
	HRESULT hr;
	DWORD dwOldProtect;
	BYTE gFuncHdr[5];	// 原函数头部的5个字节
	LPVOID lpProxy;
	LongJump jumpToReal, jumpToHook;
	
	hr = VirtualProtect((LPVOID)pfnReal, sizeof(LongJump), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	if (FAILED(hr))
		return hr;
	// 读取原函数的前5个字节
	if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)pfnReal, (LPVOID)gFuncHdr, sizeof(gFuncHdr), NULL))
		return E_FAIL;
	// 申请内存用于Proxy（原函数的前5个字节+LONGJUMP，共10个字节）
	lpProxy = VirtualAlloc(NULL, 10, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (lpProxy == NULL)
	{
		VirtualProtect((LPVOID)pfnReal, sizeof(LongJump), dwOldProtect, NULL);
		return E_FAIL;
	}
	if (!WriteProcessMemory(GetCurrentProcess(), lpProxy, (LPCVOID)gFuncHdr, sizeof(gFuncHdr), NULL))
	{
		VirtualFree(lpProxy, 0, MEM_RELEASE);
		VirtualProtect((LPVOID)pfnReal, sizeof(LongJump), dwOldProtect, NULL);
		return E_FAIL;
	}
	// 生成到原函数的跳转
	jumpToReal.opcode = 0xE9;
	jumpToReal.operand = ((UINT_PTR)pfnReal + 5) - ((UINT_PTR)lpProxy + 10);
	if (!WriteProcessMemory(GetCurrentProcess(), (LPVOID)((UINT_PTR)lpProxy + 5), (LPCVOID)&jumpToReal, sizeof(jumpToReal), NULL))
	{
		VirtualFree(lpProxy, 0, MEM_RELEASE);
		VirtualProtect((LPVOID)pfnReal, sizeof(LongJump), dwOldProtect, NULL);
		return E_FAIL;
	}
	// 生成到Hook函数的跳转
	jumpToHook.opcode = 0xE9;
	jumpToHook.operand = (UINT_PTR)pfnHook - ((UINT_PTR)pfnReal + 5);
	// 改写原函数
	if (!WriteProcessMemory(GetCurrentProcess(), (LPVOID)pfnReal, (LPCVOID)&jumpToHook, sizeof(jumpToHook), NULL))
	{
		VirtualFree(lpProxy, 0, MEM_RELEASE);
		VirtualProtect((LPVOID)pfnReal, sizeof(LongJump), dwOldProtect, NULL);
		return E_FAIL;
	}
	FlushInstructionCache(GetCurrentProcess(), (LPVOID)pfnReal, sizeof(jumpToHook));
	// 恢复状态
	VirtualProtect((LPVOID)pfnReal, sizeof(LongJump), dwOldProtect, NULL);

	pfnReal = (FuncT)lpProxy;
	return S_OK;
}

template<typename FuncT>
HRESULT UnhookAPI(FuncT& pfnReal)
{
	BYTE gFuncHdr[5];	// 原函数头部的5个字节
	LongJump jumpToReal;
	LPVOID lpReal;
	HRESULT hr;
	DWORD dwOldProtect;
	
	// 读取Proxy的前5个字节（保存的是原函数的前5个字节）
	if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)pfnReal, (LPVOID)gFuncHdr, sizeof(gFuncHdr), NULL))
		return E_FAIL;
	// 读取跳转来获取原函数的地址
	if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)((UINT_PTR)pfnReal + 5), (LPVOID)&jumpToReal, sizeof(jumpToReal), NULL))
		return E_FAIL;
	lpReal = (LPVOID)(((UINT_PTR)pfnReal + 10) + jumpToReal.operand - 5);
	// 恢复原函数头
	hr = VirtualProtect(lpReal, sizeof(LongJump), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	if (FAILED(hr))
		return hr;
	if (!WriteProcessMemory(GetCurrentProcess(), lpReal, (LPCVOID)gFuncHdr, sizeof(gFuncHdr), NULL))
		return E_FAIL;
	VirtualProtect(lpReal, sizeof(LongJump), dwOldProtect, NULL);
	// 释放Proxy
	if (!VirtualFree((LPVOID)pfnReal, 0, MEM_RELEASE))
		return E_FAIL;
	
	pfnReal = (FuncT)lpReal;
	return S_OK;
}