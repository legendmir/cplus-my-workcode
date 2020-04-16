#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<atlstr.h>
#include<tlhelp32.h>
//原函数类型定义

SIZE_T GetPidByName(CString pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return 0;
	}
	PROCESSENTRY32 pi;
	pi.dwSize = sizeof(PROCESSENTRY32); //第一次使用必须初始化成员
	BOOL bRet = Process32First(hSnapshot, &pi);
	while (bRet)
	{
		if (pi.szExeFile == pName)
		{
			return pi.th32ProcessID;
		}
		bRet = Process32Next(hSnapshot, &pi);
	}
	return 0;
}

bool RemoteThreadInject(CString pName)
{
	SIZE_T Pid = GetPidByName(pName);
	//1.使用PID打开进程获取权限
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, Pid);
	if (hProcess==INVALID_HANDLE_VALUE)
	{
		printf("申请内存失败！\n");
		return false;
	}

	//2.申请内存,写入DLL路径
	int nLen = strlen("C:\\Users\\Administrator\\Desktop\\test\\hookdll.dll") + 1;
	LPVOID pBuf = VirtualAllocEx(hProcess, NULL, nLen, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!pBuf)
	{
		printf("申请内存失败！\n");
		return false;
	}
	//3.写入内存
	SIZE_T dwWrite = 0;
	if (!WriteProcessMemory(hProcess, pBuf, "C:\\Users\\Administrator\\Desktop\\test\\hookdll.dll", nLen, &dwWrite))
	{
		printf("写入内存失败！\n");
		return false;
	}
	//4.创建远程线程，让对方调用LoadLibrary
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, NULL,
		(LPTHREAD_START_ROUTINE)LoadLibrary, pBuf, 0, 0);
	if (!hRemoteThread)
		printf("inject 成功!\n");
	//5.等待线程结束返回,释放资源
	WaitForSingleObject(hRemoteThread, -1);
	CloseHandle(hRemoteThread);
	VirtualFreeEx(hProcess, pBuf, 0, MEM_FREE);
	return true;
}

int main()
{
	RemoteThreadInject("MCComm.exe");
	//RemoteThreadInject("AWFInterpreter_vc80.exe");
	system("pause\n");
	return 0;
}
