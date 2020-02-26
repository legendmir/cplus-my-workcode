#include "state.h"
#include "DataExp.h"

extern VatBin bin;


StateDescript::StateDescript(int addr)
{
	int NameAddr = *(int*)(addr);
	WCHAR* p = (WCHAR*)(NameAddr + int(bin.mpVaddr)) + 3;
	for (int i = 0;p[i] != 0;i++)
	{
		mName += (char)p[i];
	}
	int PieceIdAddr = *(int*)(addr + 4);
	p = (WCHAR*)(PieceIdAddr + int(bin.mpVaddr)) + 3;
	for (int i = 0;p[i] != 0;i++)
	{
		mStatePieceId += (char)p[i];
	}
}

void StateLinkNode::write(HANDLE hfile)
{
	CString buf;
	DWORD dw;
	buf.Format("%08X", GetFOA(mAddr));
	buf = "0x" + buf + ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf.Format("%08X", mXXX_1);
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf.Format("%08X", mStateId_2);
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf.Format("%08X", mStateNodeAddr_3);
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	WriteFile(hfile, "\x0d\x0a", 2, &dw, 0);
}

StateLinkNode::StateLinkNode(int addr)
{
	mAddr = addr;
	mXXX_1 = *(int*)addr;
	mStateId_2 = *(int*)(addr + 4);
	mStateNodeAddr_3 = *(int*)(addr + 8);
}

void StatePiece::write(HANDLE hfile)
{
	CString buf;
	DWORD dw;
	buf.Format("%08X", mAddr - (int)bin.mpVaddr);
	buf = "0x" + buf + ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf.Format("%08X", mStateId_1);
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf.Format("%08X", mXXX_2);
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf.Format("%08X", mNextId_3);
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf.Format("%08X", mStatePieceAddr_4);
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf.Format("%08X", mXXX_5);
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf.Format("%08X", GetFOA(mStateLinkNode_6[0].mAddr));
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf.Format("%08X", GetFOA(mDataTreeAddr_7));
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	WriteFile(hfile, "\x0d\x0a", 2, &dw, 0);
}

void StatePiece::WriteData(HANDLE hfile)
{
	if (mDataTreeAddr_7)
	{
		DataConfiguration::WriteOneDataConfig(GetVA(mDataTreeAddr_7), hfile);
	}
}

StatePiece::StatePiece(int addr)
{
	mAddr = addr;
	mStateId_1 = *(int*)mAddr;
	mXXX_2 = *(int*)(mAddr + 4);
	mNextId_3 = *(int*)(mAddr + 8);
	mStatePieceAddr_4 = *(int*)(mAddr + 12);
	mXXX_5 = *(int*)(mAddr + 16);

	int StateLinkNodeAddr = GetVA(*(int*)(mAddr + 20));
	StateLinkNode tStateLinkNode(StateLinkNodeAddr);
	mStateLinkNode_6.push_back(tStateLinkNode);

	mDataTreeAddr_7 = *(int*)(mAddr + 24);
}

void State::write(HANDLE hfile)
{
	CString buf;
	DWORD dw;
	buf.Format("%08X", mAddr - (int)bin.mpVaddr);
	buf = "0x" + buf + ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);

	buf.Format("%08X", mId);
	buf = buf + ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);

	buf.Format("%08X", mDescriptNum);
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);

	buf.Format("%08X", mStatePieceNum);
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);

	buf.Format("%08X", GetFOA(mStatePieces[0].mAddr));
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);

	buf = mFullName;
	buf += ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);

	WriteFile(hfile, "\x0d\x0a", 2, &dw, 0);

	for (int i = 0;i < mDescriptNum;i++)
	{
		buf = mStateDescripts[i].mName;
		buf += ",";
		WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
		WriteFile(hfile, "\x0d\x0a", 2, &dw, 0);
		buf = mStateDescripts[i].mStatePieceId;
		buf += ",";
		WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
		WriteFile(hfile, "\x0d\x0a", 2, &dw, 0);
	}
	WriteFile(hfile, "\x0d\x0a", 2, &dw, 0);
}

void State::WriteTotolStateData(HANDLE hfile)
{
	for (UINT i=0;i<mStatePieceNum;i++)
	{
		mStatePieces[i].WriteData(hfile);
	}
}

State::State(int addr)
{ 
	mAddr = addr;
	mId = *(int*)addr;

	int NameAddr = GetVA(*(int*)(addr + 4));
	DataStr strName(NameAddr);
	mFullName = strName.mStr;
	//mFullName.Replace('/','_');


	mFlag = *(int*)(addr + 8);
	mXXX = *(int*)(addr + 12);
	mDescriptNum = *(int*)(addr + 16);


	int DescriptAddr = GetVA(*(int*)(addr + 20));
	for (int i = 0;i < mDescriptNum;i++)
	{
		StateDescript tDescript(DescriptAddr + 8 * i);
		mStateDescripts.push_back(tDescript);
	}

	mStatePieceNum = *(int*)(addr + 24);
	//
	int PiecesAddr = GetVA(*(int*)(addr + 28));
	for (int i = 0;i < mStatePieceNum;i++)
	{
		StatePiece tStatePiece(PiecesAddr + 28 * i);
		mStatePieces.push_back(tStatePiece);
	}

	char* pName = (char*)(addr + 32);
	for (int i = 0;pName[i];i++)
	{
		mName += pName[i];
	}
}
