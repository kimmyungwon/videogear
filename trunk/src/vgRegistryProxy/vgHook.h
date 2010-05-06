#pragma once

#include "BeaEngine.h"

// trampoline

#pragma pack(push, 1)
struct vgLongJump32
{
	unsigned char m_opcode;
	int m_offset;
};
#pragma pack(pop)

#ifdef _M_IX86
typedef vgLongJump32 vgLongJump;
#else
typedef vgLongJump64 vgLongJump;
#endif

class vgHook
{
private:
	struct HookedItem
	{
		long long m_original;
		int m_entryBlockSize;
		long long m_hook;
		long long m_trampoline;
		int m_trampolineSize;
	};
public:
	~vgHook(void)
	{
		UnhookAll();
	}

	template<typename T>
	T Hook(T original, T hook)
	{
		// 跟踪jmp指令来获得原函数真实的入口地址
		original = (T)GetRealEntryPoint((long long)original);
				
		// 防止重复Hook
		HookedItem newItem;
		newItem.m_original = (long long)original;
		newItem.m_hook = (long long)hook;
		std::pair<HookedItems::const_iterator, bool> itemsPairIB = m_hookedItems.push_back(newItem);
		if (!itemsPairIB.second)
			return original;

		// 计算原函数入口容纳一个LongJump所需转移的最小指令块的大小
		newItem.m_entryBlockSize = CalcEntryBlockSize((long long)original);
		if (newItem.m_entryBlockSize < sizeof(vgLongJump))
			return original;

		// Trampoline需要容纳原函数入口的指令块以及一个LongJump
		newItem.m_trampolineSize = newItem.m_entryBlockSize + sizeof(vgLongJump);
		newItem.m_trampoline = (long long)VirtualAllocEx(GetCurrentProcess(), NULL, newItem.m_trampolineSize, MEM_COMMIT | MEM_RESERVE, 
			PAGE_EXECUTE_READWRITE);

		// 生成Trampoline
		void *entryBlock = malloc(newItem.m_entryBlockSize);
		ReadProcessMemory(GetCurrentProcess(), (LPCVOID)original, entryBlock, newItem.m_entryBlockSize, NULL);
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)newItem.m_trampoline, entryBlock, newItem.m_entryBlockSize, NULL);
		free(entryBlock);

		vgLongJump jumpToOriginal;
		jumpToOriginal.m_opcode = 0xE9;
		jumpToOriginal.m_offset = (int)original + newItem.m_entryBlockSize - ((int)newItem.m_trampoline + newItem.m_trampolineSize);
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)(newItem.m_trampoline + newItem.m_entryBlockSize), &jumpToOriginal, 
			sizeof(jumpToOriginal), NULL);

		// 改写原函数入口
		vgLongJump jumpToHook;
		jumpToHook.m_opcode = 0xE9;
		jumpToHook.m_offset = (int)hook - ((int)original + sizeof(vgLongJump));
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)original, &jumpToHook, sizeof(jumpToHook), NULL);
		FlushInstructionCache(GetCurrentProcess(), (LPCVOID)original, sizeof(vgLongJump));

		m_hookedItems.replace(itemsPairIB.first, newItem);
		return (T)newItem.m_trampoline;
	}

	template<typename T>
	void Unhook(T &original, T hook)
	{
		typedef HookedItems::nth_index<2>::type HookIndex;
		HookIndex &hookIndex = m_hookedItems.get<2>();
		HookIndex::const_iterator iter = hookIndex.find((long long)hook);
		if (iter == hookIndex.end())
			return;
		HookedItem hookedItem = *iter;
		hookIndex.erase(iter);

		Unhook(hookedItem);
		original = (T)hookedItem.m_original;
	}
private:
	int CalcEntryBlockSize(long long func)
	{
		DISASM disasm = {0};
		disasm.EIP = func;

		int blockSize = 0;
		while (blockSize < sizeof(vgLongJump))
		{
			int instLen = Disasm(&disasm);
			if (instLen <= 0)
				break;
			blockSize += instLen;
			disasm.EIP += instLen;
		}
		return blockSize;
	}
		
	long long GetRealEntryPoint(long long func)
	{
		DISASM disasm = {0};
		disasm.EIP = func;
		while (true)
		{
			int instLen = Disasm(&disasm);
			if (instLen > 0)
			{
				break; // TODO:
			}
			else
				break;
			disasm.EIP += instLen;
		}
		return disasm.EIP;
	}

	void Unhook(HookedItem &hookedItem)
	{
		void *entryBlock = malloc(hookedItem.m_entryBlockSize);
		ReadProcessMemory(GetCurrentProcess(), (LPCVOID)hookedItem.m_trampoline, entryBlock, hookedItem.m_entryBlockSize, NULL);
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)hookedItem.m_original, entryBlock, hookedItem.m_entryBlockSize, NULL);
		FlushInstructionCache(GetCurrentProcess(), (LPCVOID)hookedItem.m_original, hookedItem.m_entryBlockSize);
		free(entryBlock);

		VirtualFreeEx(GetCurrentProcess(), (LPVOID)hookedItem.m_trampoline, 0, MEM_RELEASE);
	}

	void UnhookAll(void)
	{
		typedef HookedItems::nth_index<2>::type HookIndex;
		HookIndex &hookIndex = m_hookedItems.get<2>();
		while (hookIndex.size() > 0)
		{
			HookedItem hookedItem = *hookIndex.begin();
			hookIndex.erase(hookIndex.begin());
			Unhook(hookedItem);
		}
	}
private:
	typedef boost::multi_index::multi_index_container<
		HookedItem,
		boost::multi_index::indexed_by<
			boost::multi_index::random_access<>,
			boost::multi_index::ordered_unique<boost::multi_index::member<HookedItem, long long, &HookedItem::m_original> >,
			boost::multi_index::ordered_unique<boost::multi_index::member<HookedItem, long long, &HookedItem::m_hook> >
		>
	> HookedItems;
	HookedItems m_hookedItems;
};