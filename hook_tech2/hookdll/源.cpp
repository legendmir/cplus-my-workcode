#include<windows.h>
#include<stdio.h>
#include<atlstr.h>
#include ".\detours\detours.h"
#pragma comment(lib,"detours.lib")
typedef  void(*FUNCTYPE1)(int,int);
typedef  void(*FUNCTYPE2)(int, int,int);
FUNCTYPE1 pOldFunc1= (FUNCTYPE1)0x00449750;
FUNCTYPE2 pOldFunc2= (FUNCTYPE2)0x00449670;


FILE* fp;
int x = fopen_s(&fp, "C:\\Users\\Administrator\\Desktop\\hook\\tech2hook.txt", "w");


extern"C"  void __stdcall pNewFunc1()
{
	DWORD dw = 0;
	CString buf;
	int xx = 0;
	_asm
	{
		push esi
		lea esi, [ebp+0x8]
		mov esi, [esi]
		mov xx,esi
		pop esi
	}
	fprintf(fp, "1 ");
	fprintf(fp, "%08X\n",xx);
}
extern"C" void __stdcall pNewFunc2()
{
	DWORD dw = 0;
	CString buf;
	int xx = 0;
	_asm
	{
		push esi
		lea esi, [ebp + 0x8]
		mov esi, [esi]
		mov xx, esi
		pop esi
	}
	fprintf(fp, "2 ");
	fprintf(fp, "%08X\n", xx);

	fprintf(fp, "2 ");
	fprintf(fp, "%08X\n", xx+1);
}


extern"C" void __declspec(naked)Pre1()
{
	_asm
	{
		pushad
		pushfd

		lea esi, [esp + 0x28]//获取参数
		mov esi, [esi]

		push esi
		call pNewFunc1
		add esp, 4


		popfd//恢复初始环境
		popad

		push pOldFunc1//调用原函数
		ret
	}
}
extern"C" void __declspec(naked)Pre2()
{
	_asm
	{
		pushad
		pushfd

		lea esi, [esp + 0x28]//获取参数
		mov esi, [esi]

		push esi
		call pNewFunc2
		add esp, 4


		popfd//恢复初始环境
		popad

		push pOldFunc2//调用原函数
		ret
	}
}


INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hDLL);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)pOldFunc1, Pre1);
		DetourAttach(&(PVOID&)pOldFunc2, Pre2);
		DetourTransactionCommit();
	}
	break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
