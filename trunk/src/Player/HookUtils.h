//---------------------------------------------------------------------------

#ifndef HookUtilsH
#define HookUtilsH

#define STUB_SIZE 10

#pragma pack(push, 1)

struct NearJmp
{
	unsigned char opcode;
	int operand;
};

struct FarJmp
{
	unsigned short opcode;
	int operand;
};

#pragma pack(pop)

template<typename T>
struct THookAPI
{
	T pRealFunc;	// ��ʵ������ַ
	T pMyFunc;		// �º�����ַ

	THookAPI(T pOld, T pNew)
	{

	}

	~THookAPI(void)
	{
        void *pOld;

		VirtualLock(pRealFunc, STUB_SIZE);
		pOld = (void*)*((int*)((unsigned char*)pRealFunc + 6));
		// �ָ�ԭ������ǰ5���ֽ�
		WriteProcessMemory(GetCurrentProcess(), pOld, pRealFunc, 5, NULL);
		VirtualUnlock(pRealFunc, STUB_SIZE);
		// �ͷ�Stub
		VirtualFreeEx(GetCurrentProcess(), pRealFunc, STUB_SIZE, MEM_DECOMMIT);
	}
};

void* GetCodePointer(void *pFunc)
{
	FarJmp instJmp;

	ReadProcessMemory(GetCurrentProcess(), pFunc, &instJmp, sizeof(instJmp), NULL);
	if (instJmp.opcode == 0x25FF)
		return (void*)*((int*)instJmp.operand);
	else
    	return pFunc;
}

bool HookAPI(void *&pReal, void *pHook)
{
	void *pOld, *pStub;
	NearJmp instJmp;

	if (pReal == NULL || pHook == NULL || pReal == pHook) return false;
	pOld = GetCodePointer(pReal);
	// ����Stub������ԭ������ǰ5���ֽ��Լ�һ��long jmp
	pStub = VirtualAllocEx(GetCurrentProcess(), NULL, STUB_SIZE,
		MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (pStub == NULL) return false;
	// ����jmp rel32, ����ԭ�����ĵ�11���ֽ�(long jmp֮��)
	// rel32�ǵ�ǰλ�õ��¸�ָ���������λ�õĲ�
	instJmp.opcode = 0xE9;	// jmp rel32
	instJmp.operand = (int)((unsigned char*)pOld + 5) - (int)((unsigned char*)pStub + 10);
	VirtualLock(pStub, STUB_SIZE);
	WriteProcessMemory(GetCurrentProcess(), pStub, pOld, 5, NULL);	// mov edi,edi; push ebp; mov ebp,esp;
	WriteProcessMemory(GetCurrentProcess(), (unsigned char*)pStub + 5, &instJmp,
		sizeof(instJmp), NULL);
	VirtualUnlock(pStub, STUB_SIZE);
	// ��дԭ������ǰ5���ֽ�Ϊlong jmp
	instJmp.opcode = 0xE9;	// jmp rel32
	instJmp.operand = (int)pHook - ((int)pOld + 5);
	WriteProcessMemory(GetCurrentProcess(), pOld, &instJmp, sizeof(instJmp), NULL);

	pReal = pStub;
	return true;
}

//---------------------------------------------------------------------------
#endif
