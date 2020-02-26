#pragma once
#include<atlstr.h>
#include <vector>


class StateDescript {
public:
	StateDescript(int);
	CString mName;
	CString mStatePieceId;
};

class StateLinkNode {
public:
	StateLinkNode(int);
	void write(HANDLE);
public:
	int mAddr;
	int mXXX_1;
	int mStateId_2;
	int mStateNodeAddr_3;
};

class StatePiece {
public:
	StatePiece(int);
	void write(HANDLE);
	void WriteData(HANDLE);
public:
	int mAddr;
	int mStateId_1;
	int mXXX_2;
	int mNextId_3;
	int mStatePieceAddr_4;//StatePiece* 指向另一个StatePiece
	int mXXX_5;
	std::vector<StateLinkNode> mStateLinkNode_6;
	int mDataTreeAddr_7;
};

class State {


public:
	State(int);
	void write(HANDLE); 
	void WriteTotolStateData(HANDLE);

public:
	int mAddr;
	int mId;
	CString mFullName;
	int mFlag;
	int mXXX;
	int mDescriptNum;
	std::vector<StateDescript> mStateDescripts;
	int mStatePieceNum;
	std::vector<StatePiece> mStatePieces;
	CString mName;
};