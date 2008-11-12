//---------------------------------------------------------------------------

#ifndef HookUtilsH
#define HookUtilsH

#pragma pack(push, 1)

struct NearJmp
{
	byte opcode;
	int operand;
};

struct FarJmp
{
	unsigned short opcode;
	int operand;
};

#pragma pack(pop)

#define FUNC_HEADER 5
#define STUB_SIZE 	FUNC_HEADER + sizeof(NearJmp)

template<typename FuncT>
inline static FuncT GetCodePointer(FuncT pFunc)
{
	FarJmp instJmp;

	ReadProcessMemory(GetCurrentProcess(), pFunc, &instJmp, sizeof(instJmp), NULL);
	if (instJmp.opcode == 0x25FF)
		return GetCodePointer((FuncT)*((int*)instJmp.operand));
	else
    	return pFunc;
}

// 目前只支持部分Win32API，确切的说，是函数头部如下
// mov edi,edi
// push ebp
// mov ebp,esp
// 共5个字节的，如果前5个字节与上述不同但是是完整指令的话亦可
template<typename FuncT>
inline static bool HookAPI(FuncT& pReal, FuncT pHook)
{
	FuncT pOld = NULL, pStub = NULL;
	NearJmp instJmp;

	if (pReal == NULL || pHook == NULL || pReal == pHook)
		return false;
	pOld = GetCodePointer(pReal);
	// 生成Stub，包含原函数的前5个字节以及一个long jmp
	pStub = (FuncT)VirtualAllocEx(GetCurrentProcess(), NULL, STUB_SIZE,
		MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (pStub == NULL) return false;
	// 生成jmp rel32, 跳到原函数的第6个字节(long jmp之后)
	// rel32是当前位置的下个指令相对于新位置的差
	instJmp.opcode = 0xE9;	// jmp rel32
	instJmp.operand = (int)((byte*)pOld + sizeof(instJmp)) -
		(int)((byte*)pStub + FUNC_HEADER + sizeof(instJmp));
	VirtualLock(pStub, STUB_SIZE);
	WriteProcessMemory(GetCurrentProcess(), pStub, pOld, FUNC_HEADER, NULL);
	WriteProcessMemory(GetCurrentProcess(), (byte*)pStub + FUNC_HEADER, &instJmp,
		sizeof(instJmp), NULL);
	VirtualUnlock(pStub, STUB_SIZE);
	// 改写原函数的前5个字节为long jmp
	instJmp.opcode = 0xE9;	// jmp rel32
	instJmp.operand = (int)pHook - ((int)pOld + FUNC_HEADER);
	WriteProcessMemory(GetCurrentProcess(), pOld, &instJmp, sizeof(instJmp), NULL);

	pReal = pStub;
	return true;
}

inline static void UnhookAPI(void *pReal)
{
	NearJmp *pInstJmp;
	void *pOld;

	VirtualLock(pReal, STUB_SIZE);
	pInstJmp = (NearJmp*)((byte*)pReal + FUNC_HEADER);
	pOld = (void*)(pInstJmp->operand + (int)((byte*)pReal + FUNC_HEADER + sizeof(NearJmp)) - sizeof(NearJmp));
	// 恢复原函数的前5个字节
	WriteProcessMemory(GetCurrentProcess(), pOld, pReal,
		FUNC_HEADER, NULL);
	VirtualUnlock(pReal, STUB_SIZE);
	// 释放Stub
	VirtualFreeEx(GetCurrentProcess(), pReal, STUB_SIZE, MEM_DECOMMIT);
}

//---------------------------------------------------------------------------
#endif
