
#include<Windows.h>
#include"hook___.h"
void hide_self(void* module)
{
	void* pPEB = nullptr;


	_asm
	{
		push eax
		mov eax, fs: [0x30]
		mov pPEB, eax
		pop eax
	}

	void* pLDR = *((void**)((unsigned char*)pPEB + 0xc));
	void* pCurrent = *((void**)((unsigned char*)pLDR + 0x0c));
	void* pNext = pCurrent;
	
	do
	{
		void* pNextPoint = *((void**)((unsigned char*)pNext));
		void* pLastPoint = *((void**)((unsigned char*)pNext + 0x4));
		void* nBaseAddress = *((void**)((unsigned char*)pNext + 0x18));

		if (nBaseAddress == module)
		{
			*((void**)((unsigned char*)pLastPoint)) = pNextPoint;
			*((void**)((unsigned char*)pNextPoint + 0x4)) = pLastPoint;
			pCurrent = pNextPoint;
		}

		pNext = *((void**)pNext);
	} while (pCurrent != pNext);
}


bool _stdcall DllMain(void* dllhandle, unsigned long reason, void* breserved)
{

	if (reason == DLL_PROCESS_ATTACH)
	{
		hide_self(dllhandle);
		hooks::hook();
	}



	return true;

}