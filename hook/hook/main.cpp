#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<atlstr.h>
#include<tlhelp32.h>
//ԭ�������Ͷ���

SIZE_T GetPidByName(CString pName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return 0;
	}
	PROCESSENTRY32 pi;
	pi.dwSize = sizeof(PROCESSENTRY32); //��һ��ʹ�ñ����ʼ����Ա
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
	//1.ʹ��PID�򿪽��̻�ȡȨ��
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, Pid);
	if (hProcess==INVALID_HANDLE_VALUE)
	{
		printf("�����ڴ�ʧ�ܣ�\n");
		return false;
	}

	//2.�����ڴ�,д��DLL·��
	int nLen = strlen("C:\\Users\\Administrator\\Desktop\\test\\hookdll.dll") + 1;
	LPVOID pBuf = VirtualAllocEx(hProcess, NULL, nLen, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!pBuf)
	{
		printf("�����ڴ�ʧ�ܣ�\n");
		return false;
	}
	//3.д���ڴ�
	SIZE_T dwWrite = 0;
	if (!WriteProcessMemory(hProcess, pBuf, "C:\\Users\\Administrator\\Desktop\\test\\hookdll.dll", nLen, &dwWrite))
	{
		printf("д���ڴ�ʧ�ܣ�\n");
		return false;
	}
	//4.����Զ���̣߳��öԷ�����LoadLibrary
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, NULL,
		(LPTHREAD_START_ROUTINE)LoadLibrary, pBuf, 0, 0);
	if (!hRemoteThread)
		printf("inject �ɹ�!\n");
	//5.�ȴ��߳̽�������,�ͷ���Դ
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
