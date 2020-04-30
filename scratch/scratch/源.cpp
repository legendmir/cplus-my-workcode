#include"tree.h"
#include<windows.h>

const char* aIknpoiu0q7c9yl = "IKnpoiu0q7C9YLc5vEQ8/edAwsDUZf2BzPhSFXmlRj4Otax6+rb1HJMGTgVW3kNy";

#define LODWORD(x)  (*((DWORD*)&(x))) 

__int64 __fastcall sub_6D050(__int64 a1, char a2, __int64 a3)
{
    __int64 v3; // rax

    if (!a1)
        return 0i64;
    if (a3)
    {
        v3 = 0i64;
        do
            *(BYTE*)(a1 + v3++) = a2;
        while (a3 != v3);
        a1 += a3;
    }
    return a1;
}

struct byte_arr {
    char field_0;
    char field_1;
    char field_2;
    char field_3;
    char field_4;
    char field_5;
    char field_6;
    char field_7;
};

__int64 __fastcall sub_68080(char* a1, char* a2)
{
    char* v2; // r14
    char v3; // dl
    char* v4; // rdi
    __int64 v5; // r9
    __int64 v7; // r8
    signed int v8; // esi
    __int64 v9; // rax
    __int64 v10; // rdx
    __int64 v11; // rsi
    __int64 v12; // rax
    __int64 v13; // rax
    __int64 v14; // rcx
    signed __int64 v15; // rcx
    __int64 v16; // rsi
    signed __int64 v17; // rax
    byte_arr v19; // [rsp+21h] [rbp-27h]

    v2 = a2;
    v3 = *a1;
    v4 = v2;
    if (*a1)
    {
        v5 = 0i64;
        while (a1[v5++ + 1] != 0)
            ;
        v7 = 0i64;
        v4 = v2;
        v8 = 0;
        while (1)
        {
            LODWORD(v5) = v5 - 1;
            v9 = v8++;
            *(&v19.field_0 + v9) = v3;
            if (v8 == 4)
            {
                v10 = 0i64;
                do
                {
                    v11 = 0i64;
                    while (aIknpoiu0q7c9yl[v11] != *(&v19.field_0 + v10))
                    {
                        if (++v11 == 64)
                            goto LABEL_13;
                    }
                    *(&v19.field_0 + v10) = v11;
                LABEL_13:
                    ++v10;
                } while (v10 != 4);
                v19.field_4 = 4 * v19.field_0 | ((unsigned __int8)v19.field_1 >> 4) & 3;
                v19.field_5 = 16 * v19.field_1 | ((unsigned __int8)v19.field_2 >> 2) & 0xF;
                v19.field_6 = v19.field_3 + (v19.field_2 << 6);
                *v4 = v19.field_4;
                v12 = 0i64;
                do
                {
                    v4[v12 + 1] = *(&v19.field_5 + v12);
                    ++v12;
                } while (v12 != 2);
                v4 += 3;
                v8 = 0;
            }
            if (!(DWORD)v5)
                break;
            v3 = a1[++v7];
        }
        if (v8)
        {
            if (v8 <= 3)
                sub_6D050((__int64)&v19 + v8, 0, (unsigned int)(3 - v8) + 1i64);
            v13 = 0i64;
            do
            {
                v14 = 0i64;
                while (aIknpoiu0q7c9yl[v14] != *(&v19.field_0 + v13))
                {
                    if (++v14 == 64)
                        goto LABEL_27;
                }
                *(&v19.field_0 + v13) = v14;
            LABEL_27:
                ++v13;
            } while (v13 != 4);
            v19.field_4 = 4 * v19.field_0 | ((unsigned __int8)v19.field_1 >> 4) & 3;
            v19.field_5 = 16 * v19.field_1 | ((unsigned __int8)v19.field_2 >> 2) & 0xF;
            v19.field_6 = v19.field_3 + (v19.field_2 << 6);
            if (v8 >= 2)
            {
                v15 = (unsigned int)(v8 - 2) + 1i64;
                v16 = (unsigned int)(v8 - 1);
                *v4 = 4 * v19.field_0 | ((unsigned __int8)v19.field_1 >> 4) & 3;
                if ((DWORD)v16 != 1)
                {
                    v17 = 1i64;
                    do
                    {
                        v4[v17] = *(&v19.field_4 + v17);
                        ++v17;
                    } while (v16 != v17);
                }
                v4 += v15;
            }
        }
    }
    *v4 = 0;
    return (unsigned int)((DWORD)v4 - (DWORD)v2);
}



int main()
{
    char* a1 = (char*)"wMPXwMajZngPUdiVUMgPfGYxwMka";
    char* result = new char[200]{};
    
    sub_68080(a1, result);


	return 0;
}