typedef _DWORD DWORD

char *__thiscall C_Tag::GetTagName(char *this)
{
  return this + 0x14;
}

void __thiscall C_Tag_LogicalTest::Process(C_Tag_LogicalTest *this, struct C_Tag *a2)
{
  C_Tag_LogicalTest *v2; // esi
  int v3; // eax
  unsigned int v4; // ebx
  unsigned int v5; // ebp
  int v6; // ecx
  int v7; // eax
  int v8; // ecx
  C_Tag_For *v9; // edi
  char v10; // [esp+Fh] [ebp-1h]

  v2 = this;
  v3 = *((_DWORD *)this + 2);
  v10 = 0;
  if ( v3 )
    v4 = (*((_DWORD *)this + 3) - v3) >> 2;
  else
    v4 = 0;
  v5 = 0;
  if ( v4 )
  {
    while ( 1 )
    {
      v6 = *((_DWORD *)v2 + 2);

      v7 = C_Tag::GetTagName(*(_DWORD *)(*((_DWORD *)v2 + 2) + 4 * v5), "For");
      if ( (unsigned __int8)std::operator==<char,std::char_traits<char>,std::allocator<char>>(v7) )
      {
        v8 = *((_DWORD *)v2 + 2);


        v9 = *(C_Tag_For **)(4 * v5 + *((_DWORD *)v2 + 2));
        if ( C_Tag_For::IsConditionTrue(v9) == 1 )
          break;
      }
      if ( ++v5 >= v4 )
        goto LABEL_16;
    }
    v10 = 1;
    *((_DWORD *)v2 + 13) |= C_Tag::GetRetCode(v9);
  }
LABEL_16:
  *((_BYTE *)C_ExecutionEnvironment::GetSessionContext((C_Tag_LogicalTest *)((char *)v2 + 56)) + 4) = v10;
}