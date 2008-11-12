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

// Ŀǰֻ֧�ֲ���Win32API��ȷ�е�˵���Ǻ���ͷ������
// mov edi,edi
// push ebp
// mov ebp,esp
// ��5���ֽڵģ����ǰ5���ֽ���������ͬ����������ָ��Ļ����
template<typename FuncT>
inline static bool HookAPI(FuncT& pReal, FuncT pHook)
{
	FuncT pOld = NULL, pStub = NULL;
	NearJmp instJmp;

	if (pReal == NULL || pHook == NULL || pReal == pHook)
		return false;
	pOld = GetCodePointer(pReal);
	// ����Stub������ԭ������ǰ5���ֽ��Լ�һ��long jmp
	pStub = (FuncT)VirtualAllocEx(GetCurrentProcess(), NULL, STUB_SIZE,
		MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (pStub == NULL) return false;
	// ����jmp rel32, ����ԭ�����ĵ�6���ֽ�(long jmp֮��)
	// rel32�ǵ�ǰλ�õ��¸�ָ���������λ�õĲ�
	instJmp.opcode = 0xE9;	// jmp rel32
	instJmp.operand = (int)((byte*)pOld + sizeof(instJmp)) -
		(int)((byte*)pStub + FUNC_HEADER + sizeof(instJmp));
	VirtualLock(pStub, STUB_SIZE);
	WriteProcessMemory(GetCurrentProcess(), pStub, pOld, FUNC_HEADER, NULL);
	WriteProcessMemory(GetCurrentProcess(), (byte*)pStub + FUNC_HEADER, &instJmp,
		sizeof(instJmp), NULL);
	VirtualUnlock(pStub, STUB_SIZE);
	// ��дԭ������ǰ5���ֽ�Ϊlong jmp
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
	// �ָ�ԭ������ǰ5���ֽ�
	WriteProcessMemory(GetCurrentProcess(), pOld, pReal,
		FUNC_HEADER, NULL);
	VirtualUnlock(pReal, STUB_SIZE);
	// �ͷ�Stub
	VirtualFreeEx(GetCurrentProcess(), pReal, STUB_SIZE, MEM_DECOMMIT);
}

//---------------------------------------------------------------------------
#endif
