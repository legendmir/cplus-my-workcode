#include<windows.h>
#include<stdio.h>
#include<atlstr.h>
#include <io.h>

HMODULE hmodule = LoadLibrary("libeay32.dll");
int (*pEVP_des_ede3_cbc)() = (int(*)())GetProcAddress(hmodule, "EVP_des_ede3_cbc");
int (*pEVP_md5)() = (int(*)())GetProcAddress(hmodule, "EVP_md5");
void (*pEVP_BytesToKey)(int,int,int,int,int,int,int,int) = (void(*)(int, int, int, int, int, int, int, int))GetProcAddress(hmodule, "EVP_BytesToKey");
void(*pEVP_DecryptInit_ex)(int, int, int, int, int) = (void(*)(int, int, int, int, int))GetProcAddress(hmodule, "EVP_DecryptInit_ex");
void (*pEVP_CIPHER_CTX_init)(int) = (void(*)(int))GetProcAddress(hmodule, "EVP_CIPHER_CTX_init");
int (*pEVP_CIPHER_CTX_block_size)(int) = (int(*)(int))GetProcAddress(hmodule, "EVP_CIPHER_CTX_block_size");
int (*pEVP_DecryptUpdate)(int, int, int, int, int) = (int(*)(int, int, int, int, int))GetProcAddress(hmodule, "EVP_DecryptUpdate");
void (*pEVP_DecryptFinal_ex)(int,int,int) = (void(*)(int,int,int))GetProcAddress(hmodule, "EVP_DecryptFinal_ex");
void (*pEVP_CIPHER_CTX_cleanup)(int) = (void(*)(int))GetProcAddress(hmodule, "EVP_CIPHER_CTX_cleanup");

CString GetDir(CString path)
{
	int i = 0;
	for (i = path.GetLength();i > 0;i--)
	{
		if (path[i]=='\\')
		{
			break;
		}
	}
	CString result = path.Left(i);
	return result;
}

char DecryptFileToXML(CString path)
{
	int v5; // eax
	int v25; // [esp+184h] [ebp-204h]
	int v27; // [esp+198h] [ebp-1F0h]
	//char v28; // [esp+19Ch] [ebp-1ECh]
	char *v29; // [esp+230h] [ebp-158h]
	void *v32=0; // [esp+358h] [ebp-30h]
	int v33; // [esp+35Ch] [ebp-2Ch]
	int v34; // [esp+360h] [ebp-28h]
	int v35 = 0; // [esp+364h] [ebp-24h]
	char v36 = 0; // [esp+36Bh] [ebp-1Dh]
	size_t NumOfElements; // [esp+36Ch] [ebp-1Ch]
	void *DstBuf; // [esp+370h] [ebp-18h]
	FILE *File; // [esp+374h] [ebp-14h]
	errno_t v40; // [esp+378h] [ebp-10h]


	v40 = fopen_s(&File, path, "rb");

	fseek(File, 0, 2);
	NumOfElements = ftell(File);
	fseek(File, 0, 0);
	DstBuf = calloc(NumOfElements, 1u);
	fread(DstBuf, 1u, NumOfElements, File);

	v25 = pEVP_des_ede3_cbc();
	v27 = pEVP_md5();

	char* xxx = new char[0x100];
	pEVP_CIPHER_CTX_init((int)xxx);

	char* Dst1 = new char[0x100];
	char* Dst2 = new char[0x100];
	memset(Dst1, 0, 0x20u);
	memset(Dst2, 0, 0x10u);

	char* buf = new char[8]{};
	memcpy(buf, (char *)DstBuf + 8, 8u);
	char*Dest = new char[0x100]{};
	strcpy(Dest, "Fo4dS9XkZ=RX[B0$6to3#*@s_51");
	int len = strlen(Dest);
	pEVP_BytesToKey(v25, v27, (int)buf, (int)Dest, len, 1, (int)Dst1, (int)Dst2);
	char* v28 = new char[0x100]{};
	pEVP_DecryptInit_ex((int)v28, v25, 0, (int)Dst1, (int)Dst2);
	v5 = pEVP_CIPHER_CTX_block_size((int)v28);

	v29 = (char *)malloc(NumOfElements + v5);
	pEVP_DecryptUpdate((int)v28, (int)v29, (int)&v33, (int)DstBuf + 16, NumOfElements - 16);
	pEVP_DecryptFinal_ex((int)v28, (int)&v29[v33], (int)&v34);

	FILE* fp = NULL;
	CString NewPath = path;
	NewPath.Replace(".tml", ".zip");
	fopen_s(&fp, NewPath, "wb");
	fwrite(v29, NumOfElements + v5, 1, fp);
	fclose(fp);
	pEVP_CIPHER_CTX_cleanup((int)v28);
	

	delete xxx;
	delete Dst1;
	delete Dst2;
	delete buf;
	delete Dest;
	delete v28;
	operator delete[](v29);
	fclose(File);
	return v36;
}

void __cdecl EncryptContents(char *a1, unsigned int a2)
{
	unsigned int i; // [esp+4h] [ebp-4h]

	for (i = 0; i < a2; ++i)
		a1[i] = 16 * (a1[i] & 0xF) | ((a1[i] & 0xF0) >> 4);
}

void DecryptExml(CString path)
{
	FILE* fp = NULL;
	fopen_s(&fp, path, "rb");
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buf = new char[2*len]{};
	fread(buf, len, 1, fp);
	EncryptContents(buf, len);
	fclose(fp);

	FILE* fp1 = NULL;
	fopen_s(&fp1, "E:\\Job Project\\ford\\ford analyze\\ExmlOut.xml", "wb");
	fwrite(buf, len, 1, fp1);
	fclose(fp1);
}

CString GetPathSuffix(CString strPath)
{
	CString strSuffix;
	for (int i = strPath.GetLength() - 1;i >= 0;i--)
	{
		if (strPath[i] != '.')
		{
			strSuffix = strPath[i] + strSuffix;
		}
		else
			break;
	}
	return strSuffix;
}

void DecryptFiles(CString dir)
{
	CString dirNew;
	dirNew = dir + "\\*.*";

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dirNew, &findData);
	if (handle == -1)        // 检查是否成功
		return;
	do
	{
		if (findData.attrib & _A_SUBDIR)
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;

			dirNew = dir + "\\" + findData.name;
			DecryptFiles(dirNew);
		}
		else
		{
			CString strName(findData.name);
			CString strSuffix = GetPathSuffix(strName);
			DWORD dw = 0;
			if (strSuffix == "tml")
			{
				DecryptFileToXML(dir+"\\"+strName);
			}
		}
	} while (_findnext(handle, &findData) == 0);

	_findclose(handle);    // 关闭搜索句柄
}

int main()
{
	DecryptFiles("E:\\Job Project\\ford\\ford analyze\\XMLFiles");
	//DecryptFileToXML("E:\\Job Project\\ford\\ford analyze\\6_2L_PCM_MODEINFO.tml");
	//DecryptExml("E:\\Job Project\\ford\\ford analyze\\71863.exml");
	return 0;
}