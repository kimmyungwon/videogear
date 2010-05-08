#include "vgRegHacker/vgRegTree.hpp"

void vgInjected(void) {}

BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	using namespace VideoGear;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		RegTree::GetInstance().Hook();
		break;
	case DLL_PROCESS_DETACH:
		RegTree::GetInstance().Unhook();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
