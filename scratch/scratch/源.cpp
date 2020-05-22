#include<windows.h>
#include<stdio.h>

#define _BYTE  unsigned char
#define _WORD  unsigned short
#define _DWORD unsigned int 
#define _QWORD unsigned long long

//#define LOBYTE(x)   (*((BYTE*)&(x)))   // low byte
//#define LOWORD(x)   (*((WORD*)&(x)))   // low word
//#define LODWORD(x)  (*((DWORD*)&(x)))  // low dword
//#define HIBYTE(x)   (*((BYTE*)&(x)+1))
//#define HIWORD(x)   (*((WORD*)&(x)+1))
//#define HIDWORD(x)  (*((DWORD*)&(x)+1))



int __fastcall change_1byte(int a1, signed int a2)
{
    signed int v2; // eax
    int result; // eax
    signed int v4; // ecx

    v2 = -1;
    if (a2 >= 0x30 && a2 <= 0x39 || a2 >= 0x61 && a2 <= 0x66 || (unsigned int)(a2 - 0x41) <= 5)// xx>=0&& xx<=9 || xx='a'-'f'||xx='A'-'F' 
        return a2;
    v4 = 0;
    if (a2 < 0)
        goto LABEL_22;
    do
    {
        if ((v4 < 0x30 || v4 > 0x39) && (v4 < 0x61 || v4 > 0x66) && (v4 < 0x41 || v4 > 0x46) && ++v2 > 0xF)
            v2 = 0;
        ++v4;
    } while (v4 <= a2);
    if (v2 >= 0xA)
        result = v2 + 0x37;
    else
        LABEL_22:
    result = v2 + 0x30;
    return result;
}

signed int __cdecl clac_16_byte(_BYTE* a1, int result, int len)
{
    _BYTE* v3; // esi
    char v4; // cl
    int v5; // edi
    char v6; // cl
    char v7; // dl
    char v8; // cl
    char v9; // dl

    v3 = a1;
    v4 = *a1;
    v5 = 0;
    if (!*a1)
        return v5;
    while (1)
    {
        if ((unsigned __int8)(v4 - 0x30) > 9u)
        {
            if ((unsigned __int8)(v4 - 0x61) > 5u)
            {
                if ((unsigned __int8)(v4 - 0x41) > 5u)
                    goto LABEL_18;
                v6 = v4 - 0x37;
            }
            else
            {
                v6 = v4 - 0x57;
            }
        }
        else
        {
            v6 = v4 - 0x30;
        }
        if (v6 == 0x7F)
            goto LABEL_18;
        v7 = (v3++)[1];
        v8 = 16 * v6;
        if ((unsigned __int8)(v7 - 0x30) > 9u)
        {
            if ((unsigned __int8)(v7 - 0x61) > 5u)
            {
                if ((unsigned __int8)(v7 - 0x41) > 5u)
                    return -1;
                v9 = v7 - 0x37;
            }
            else
            {
                v9 = v7 - 0x57;
            }
        }
        else
        {
            v9 = v7 - 0x30;
        }
        if (v9 == 0x7F)
            return -1;
        if (v5 >= len)
            return -2;
        *(_BYTE*)(v5++ + result) = v9 | v8;
    LABEL_18:
        v4 = (v3++)[1];
        if (!v4)
            return v5;
    }
}
   


char* __cdecl encode_string(const char* a1, char* a2)
{
    signed int v2; // edi
    signed int v3; // eax
    char v4=0; // cl
    signed int v5; // esi
    int v6; // edx
    char* result; // eax
    __int16 v8; // dx
    int v9; // [esp+Ch] [ebp-24h]
    __int16 v10=0; // [esp+10h] [ebp-20h]
    unsigned char v11[16]; // [esp+1Ch] [ebp-14h]

    memset(v11, 0x30, 16);
    v2 = 0xC;
    v3 = strlen(a1);
    if (v3 > 0)
    {
        v5 = v3;
        do
        {
            if (v2 <= 0)
                break;
            v6 = a1[v5-- - 1];
            v11[--v2] = change_1byte(v4, v6);
        } while (v5 > 0);
    }
    clac_16_byte(v11, (int)&v9, 16);
    result = a2;
    v8 = v10;
    *(_DWORD*)a2 = v9;
    *(_WORD*)(a2 + 4) = v8;
    return result;
}




void* __cdecl chg_t(void* a1, const char* a2, const char* a3)
{
    const char* v3; // esi
    char* v4; // ebx
    char i; // al
    char* v6; // esi
    void* result; // eax

    v3 = (const char*)a1;
    v4 = (char*)a1;
    if (a2)
    {
        if (strlen(a2))
        {
            for (i = *(_BYTE*)a1; i; v4 = (char*)v3)
            {
                if (!strchr(a2, i))
                    break;
                i = (v3++)[1];
            }
        }
    }
    if (a3)
    {
        if (strlen(a3))
        {
            if (strlen(v3))
            {
                v6 = (char*)&v3[strlen(v3) - 1];
                while (v6)
                {
                    if (!strchr(a3, *v6))
                        break;
                    *v6-- = 0;
                    if (v6 < v4)
                        break;
                }
            }
        }
    }
    result = a1;
    if (v4 > a1)
    {
        memmove(a1, v4, strlen(v4) + 1);
        result = a1;
    }
    return result;
}


char* __stdcall sub_1000A970(LPCSTR lpRootPathName)
{
    BOOL result; // eax
    char* x_result = 0;
    DWORD FileSystemFlags; // [esp+4h] [ebp-260h]
    DWORD MaximumComponentLength; // [esp+8h] [ebp-25Ch]
    DWORD VolumeSerialNumber; // [esp+Ch] [ebp-258h]
    CHAR FileSystemNameBuffer[0x108]; // [esp+10h] [ebp-254h]
    CHAR VolumeNameBuffer[8]; // [esp+118h] [ebp-14Ch]
    char v7[20]; // [esp+220h] [ebp-44h]

    *(_DWORD*)lpRootPathName = 0xE0D0C0B;
    *((_WORD*)lpRootPathName + 2) = 0x100F;
    result = GetVolumeInformationA(
        "c:\\",
        VolumeNameBuffer,
        0x105u,
        &VolumeSerialNumber,
        &MaximumComponentLength,
        &FileSystemFlags,
        FileSystemNameBuffer,
        0x105u);
    if (result)
    {
        sprintf(v7, "%u", VolumeSerialNumber);
        chg_t(v7, " \t", " \t");
        x_result = encode_string(v7, (char*)lpRootPathName);
    }
    return x_result;
}

int main()
{
    LPCSTR lpRootPathName = new CHAR[6]{};
    sub_1000A970(lpRootPathName);
}