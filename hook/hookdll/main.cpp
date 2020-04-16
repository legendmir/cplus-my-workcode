#include<windows.h>
#include<stdio.h>
#include<atlstr.h>
#include "detours\detours.h"
#pragma  comment(lib,"detours.lib")
typedef  void(*FUNCTYPE)(int);
FUNCTYPE pOldFunc;

//HANDLE hfile = CreateFile("C:\\Users\\Administrator\\Desktop\\test\\AWFIn_HookOut.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
HANDLE hfile = CreateFile("C:\\Users\\Administrator\\Desktop\\test\\MCC_HookOut.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


extern"C" void _declspec(dllexport) pNewFunc()
{
	DWORD dw = 0;
	char* buf = NULL;
	char* buffer = NULL;
	_asm
	{
		push esi
		lea esi,[ebp+0xc]
		mov esi,[esi]
		mov buffer,esi


		lea esi, [ebp + 0x8]
		mov esi,[esi]
		mov esi,[esi]
		mov esi, [esi]
		mov buf,esi
		pop esi

	}

	WriteFile(hfile, buf, strlen(buf), &dw, 0);
	WriteFile(hfile, "\x0d\x0a", 2, &dw, 0);

	WriteFile(hfile, buffer, strlen(buffer), &dw,0);
	WriteFile(hfile, "\x0d\x0a", 2, &dw, 0);
}

extern"C" void __declspec(naked)Pre()
{
	_asm
	{
		pushad
		pushfd

		lea esi, [esp + 0x28]//获取参数
		mov esi, [esi]

		push esi
		push ecx
		call pNewFunc
		add esp,8


		popfd//恢复初始环境
		popad

		push pOldFunc//调用原函数
		ret
	}
}

void GetTargetApi()
{
	HMODULE hmodule = GetModuleHandle("actbd32fbvc.dll");
	if (!hmodule)
	{
		printf("获取模块失败\n");
	}
	pOldFunc = (FUNCTYPE)GetProcAddress(hmodule, "?ExecQuery@CTable@@QAE_NPBD@Z");
	if (!pOldFunc)
		printf("获取?ExecQuery@CTable@@QAE_NPBD@Z失败\n");
}




INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
	{
		GetTargetApi();
		DisableThreadLibraryCalls(hDLL);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)pOldFunc, Pre);
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
