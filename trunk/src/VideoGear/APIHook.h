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
	BYTE gFuncHdr[5];	// ԭ����ͷ����5���ֽ�
	LPVOID lpProxy;
	LongJump jumpToReal, jumpToHook;
	
	hr = VirtualProtect((LPVOID)pfnReal, sizeof(LongJump), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	if (FAILED(hr))
		return hr;
	// ��ȡԭ������ǰ5���ֽ�
	if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)pfnReal, (LPVOID)gFuncHdr, sizeof(gFuncHdr), NULL))
		return E_FAIL;
	// �����ڴ�����Proxy��ԭ������ǰ5���ֽ�+LONGJUMP����10���ֽڣ�
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
	// ���ɵ�ԭ��������ת
	jumpToReal.opcode = 0xE9;
	jumpToReal.operand = ((UINT_PTR)pfnReal + 5) - ((UINT_PTR)lpProxy + 10);
	if (!WriteProcessMemory(GetCurrentProcess(), (LPVOID)((UINT_PTR)lpProxy + 5), (LPCVOID)&jumpToReal, sizeof(jumpToReal), NULL))
	{
		VirtualFree(lpProxy, 0, MEM_RELEASE);
		VirtualProtect((LPVOID)pfnReal, sizeof(LongJump), dwOldProtect, NULL);
		return E_FAIL;
	}
	// ���ɵ�Hook��������ת
	jumpToHook.opcode = 0xE9;
	jumpToHook.operand = (UINT_PTR)pfnHook - ((UINT_PTR)pfnReal + 5);
	// ��дԭ����
	if (!WriteProcessMemory(GetCurrentProcess(), (LPVOID)pfnReal, (LPCVOID)&jumpToHook, sizeof(jumpToHook), NULL))
	{
		VirtualFree(lpProxy, 0, MEM_RELEASE);
		VirtualProtect((LPVOID)pfnReal, sizeof(LongJump), dwOldProtect, NULL);
		return E_FAIL;
	}
	FlushInstructionCache(GetCurrentProcess(), (LPVOID)pfnReal, sizeof(jumpToHook));
	// �ָ�״̬
	VirtualProtect((LPVOID)pfnReal, sizeof(LongJump), dwOldProtect, NULL);

	pfnReal = (FuncT)lpProxy;
	return S_OK;
}

template<typename FuncT>
HRESULT UnhookAPI(FuncT& pfnReal)
{
	BYTE gFuncHdr[5];	// ԭ����ͷ����5���ֽ�
	LongJump jumpToReal;
	LPVOID lpReal;
	HRESULT hr;
	DWORD dwOldProtect;
	
	// ��ȡProxy��ǰ5���ֽڣ��������ԭ������ǰ5���ֽڣ�
	if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)pfnReal, (LPVOID)gFuncHdr, sizeof(gFuncHdr), NULL))
		return E_FAIL;
	// ��ȡ��ת����ȡԭ�����ĵ�ַ
	if (!ReadProcessMemory(GetCurrentProcess(), (LPCVOID)((UINT_PTR)pfnReal + 5), (LPVOID)&jumpToReal, sizeof(jumpToReal), NULL))
		return E_FAIL;
	lpReal = (LPVOID)(((UINT_PTR)pfnReal + 10) + jumpToReal.operand - 5);
	// �ָ�ԭ����ͷ
	hr = VirtualProtect(lpReal, sizeof(LongJump), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	if (FAILED(hr))
		return hr;
	if (!WriteProcessMemory(GetCurrentProcess(), lpReal, (LPCVOID)gFuncHdr, sizeof(gFuncHdr), NULL))
		return E_FAIL;
	VirtualProtect(lpReal, sizeof(LongJump), dwOldProtect, NULL);
	// �ͷ�Proxy
	if (!VirtualFree((LPVOID)pfnReal, 0, MEM_RELEASE))
		return E_FAIL;
	
	pfnReal = (FuncT)lpReal;
	return S_OK;
}