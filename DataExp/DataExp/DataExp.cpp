#include"DataExp.h"

extern VatBin bin;


bool IsEndWith(CString str, CString keyStr)
{
	int len = keyStr.GetLength();
	int FullLen = str.GetLength();
	if (FullLen < len)
	{
		return false;
	}
	bool flag = 1;
	int start = FullLen - len;
	for (int i = 0;i < len;i++)
	{
		if (str[start + i] != keyStr[i])
		{
			flag = 0;
			break;
		}
	}
	return flag;
}

int LoadFile(const char* path, char* psrc)
{
	HANDLE hfile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dw = 0;
	int size = GetFileSize(hfile, 0);
	ReadFile(hfile, psrc, size, &dw, 0);
	CloseHandle(hfile);
	return size;
}

VatBin::VatBin(CString path)
{
	LoadFile(path, mpVaddr);

	mMenuAddr = *(int*)(mpVaddr + 40);//菜单起始地址
	mStateNum = *(int*)(mpVaddr + 44);//状态类型数量
	mStateBeginAddr = *(int*)(mpVaddr + 48);//状态结构起始地址

	int LastStatePieceNum = *(int*)(mpVaddr + mStateBeginAddr + (mStateNum - 1) * 56 + 24);//获取StateLink的首地址
	int LastStateAddr = *(int*)(mpVaddr + mStateBeginAddr + (mStateNum - 1) * 56 + 28);//获取StateLink的首地址

	mStateLinkBeginAddr = LastStateAddr + LastStatePieceNum * 28;//状态link起始地址

	mDataBeginAddr = *(int*)(mpVaddr + 52);//核心数据起始地址data

	int addr = mDataBeginAddr + (int)mpVaddr;

	mDataListBeginAddr = GetDataListBeginAddr(addr);
	mDataListEndAddr = *(int*)(mpVaddr + 60);//核心数据簇结束地址

	mDtcLibNum = *(int*)(mpVaddr + 56);
	mDtcLibAddr = *(int*)(mpVaddr + 60);
	mDtcPerLibAddr = mDtcLibAddr + mDtcLibNum * 12;
	mSomeStr = *(int*)(mpVaddr + 80);

	mDllFunNum = *(int*)(mpVaddr + 72);
	mDllLibAddr = *(int*)(mpVaddr + 76);
}

void VatBin::printDataSeg(CString PathData)
{
	HANDLE hfile = GetFileHandle_W(PathData);
	MDataNode*pdata;
	int addr = mDataBeginAddr + (int)mpVaddr;;
	while (1)
	{
		if (*(int*)addr == 6)
		{
			break;
		}
		pdata = new MDataNode(addr);
		pdata->Write(hfile);
		addr += pdata->mAllLen;
		delete pdata;
	}
	CloseHandle(hfile);
}

void VatBin::printDataListSeg(CString PathData1)
{
	HANDLE hfile = GetFileHandle_W(PathData1);
	MDataListNode* pdata = 0;
	int addr = mDataListBeginAddr + (int)mpVaddr;
	while (1)
	{
		pdata = new MDataListNode(addr);
		if (pdata->mVal6_1 != 6)
		{
			break;
		}
		pdata->Write(hfile);
		addr += pdata->mAllLen;
		delete pdata;
	}
	CloseHandle(hfile);
}

void VatBin::printState(CString Path)
{
	HANDLE hfile = GetFileHandle_W(Path);
	State*pdata = 0;
	WriteTotalStateTab(hfile);
	CloseHandle(hfile);
}

void VatBin::printStatePiece(CString Path)
{
	HANDLE hfile = GetFileHandle_W(Path);
	State*pdata = 0;
	WriteStatePieceTab(hfile);
	CloseHandle(hfile);
}

void VatBin::printMenu(CString Path)
{
	HANDLE hfile = GetFileHandle_W(Path);
	int addr = mMenuAddr + (int)mpVaddr;
	MenuNode* pNode = 0;
	int CheckAddr = 0;
	while (1)
	{
		CheckAddr = *(int*)(addr + 36);
		if (CheckAddr < mMenuAddr)
		{
			return;
		}
		pNode = new MenuNode(addr);
		pNode->write(hfile);
		addr += 44;
	}
	CloseHandle(hfile);
}

void VatBin::printOrinMenu(CString Path)
{
	HANDLE hfile = GetFileHandle_W(Path);
	int addr = mMenuAddr + (int)mpVaddr;
	MenuNode* pNode = 0;
	int CheckAddr = 0;
	while (1)
	{
		CheckAddr = *(int*)(addr + 36);
		if (CheckAddr < mMenuAddr)
		{
			return;
		}
		pNode = new MenuNode(addr);
		pNode->WriteOrin(hfile);
		addr += 44;
	}
	CloseHandle(hfile);
}

void VatBin::printXmenu(CString Path)
{
	HANDLE hfile = GetFileHandle_W(Path);
	int addr = mMenuAddr + (int)mpVaddr;
	MenuNode* pNode = 0;
	int CheckAddr = 0;
	while (1)
	{
		CheckAddr = *(int*)(addr + 36);
		if (CheckAddr < mMenuAddr)
		{
			return;
		}
		pNode = new MenuNode(addr);
		pNode->WriteMenuWithState(hfile);
		addr += 44;
	}
	CloseHandle(hfile);
}

void VatBin::printStateLinkTab(CString Path)
{
	HANDLE hfile = GetFileHandle_W(Path);
	while (1)
	{
		int addr = bin.mStateLinkBeginAddr + (int)mpVaddr;
		StateLinkNode* p = new StateLinkNode(addr);
		if (p->mStateNodeAddr_3 < bin.mStateBeginAddr)
		{
			break;
		}
		p->write(hfile);
		addr += 12;
		delete p;
	}
}

void VatBin::printVinInfo(CString path)
{
	MenuNode menu;
	GetMenuByName(GetVA(mMenuAddr), "Diagnostics", menu);
	for (int i = 0;i < menu.mConfigurations.size();i++)
	{
		if (menu.mConfigurations[i].mConfigType == 0x9E)
		{
			HANDLE hfile = GetFileHandle_W(path);
			menu.mConfigurations[i].WriteOneDataConfig(menu.mConfigurations[i].mConfigNode, hfile);
		}
	}
}

void VatBin::printTotalStateData(CString path)
{
	_mkdir(path);
	int addr = mStateBeginAddr;
	State* pState = NULL;
	StatePiece* pStatePiece = NULL;
	HANDLE hfile = 0;
	CString NewPath;
	CString	tPath;
	CString tbuf;
	for (UINT i=0;i<mStateNum;i++)
	{
		pState = new State(GetVA(addr + 56 * i));
		NewPath = path + "\\" + pState->mFullName;
		_mkdir(NewPath);
		for (UINT j = 0;j < pState->mStatePieceNum;j++)
		{
			tbuf.Format("%d", pState->mStatePieces[j].mStateId_1);
			tPath = NewPath + "\\" + tbuf + ".txt";

			int tAddr = pState->mStatePieces[j].mDataTreeAddr_7;
			if (tAddr)
			{
				hfile = GetFileHandle_W(tPath);
				DataConfiguration::WriteOneDataConfig(GetVA(pState->mStatePieces[j].mDataTreeAddr_7), hfile);
				CloseHandle(hfile);
			}
		}
	}
}

void VatBin::printAggregatedStateData(CString path )
{
	_mkdir(path);
	int addr = mStateBeginAddr;
	State* pState = NULL;
	StatePiece* pStatePiece = NULL;
	HANDLE hfile = 0;
	CString NewPath;
	CString	tPath;
	CString tbuf;
	for (UINT i = 0;i < mStateNum;i++)
	{
		pState = new State(GetVA(addr + 56 * i));
		NewPath = path + "\\" + pState->mFullName+".txt";
		hfile = GetFileHandle_W(NewPath);
		for (UINT j = 0;j < pState->mStatePieceNum;j++)
		{
			int tAddr = pState->mStatePieces[j].mDataTreeAddr_7;
			if (tAddr)
			{
				DataConfiguration::WriteOneDataConfig(GetVA(tAddr), hfile);
			}
		}
		CloseHandle(hfile);
	}
}

void VatBin::printOneCfg(int addr,CString path)
{
	HANDLE hfile = GetFileHandle_W(path);
	DataConfiguration::WriteOneDataConfig(GetVA(addr), hfile);
}

void VatBin::printDtcLib(CString path)
{
	
	HANDLE hfile = GetFileHandle_W(path);
	int tAddr = GetVA(mDtcLibAddr);
	for (int i = 0;i < mDtcLibNum;i++)
	{
		DtcLib tLib(tAddr + i * 12);
		tLib.write(hfile);
	}
	CloseHandle(hfile);
}

void VatBin::printDtcs(CString path)
{
	int AllDtcNum = 0;
	int tAddr = GetVA(mDtcLibAddr);
	for (int i = 0;i < mDtcLibNum;i++)
	{
		DtcLib tLib(tAddr + i * 12);
		AllDtcNum += tLib.mDtcNum;
	}


	HANDLE hfile = GetFileHandle_W(path);
	tAddr = GetVA(mDtcPerLibAddr);
	for (int i = 0;i < AllDtcNum;i++)
	{
		PerDTC tDtc(tAddr + i * 24);
		tDtc.write(hfile);
	}
	CloseHandle(hfile);
}

void VatBin::TravMenuByName(int MenuAddr, CString name, CString path)
{
	MenuNode* pmenu=new MenuNode(MenuAddr);
	int NextAddr = 0;
	if (pmenu->mName == name)
	{
		TravMenu(pmenu->mAddr, path);
		return;
	}
	if (pmenu->mNextMenus.size())
	{
		for (int i = 0;i < pmenu->mNextMenus[0].mNextNum;i++)
		{
			NextAddr = pmenu->mNextMenus[0].mNextMenuAddrs[i];
			TravMenuByName(NextAddr, name, path);
		}
	}
	delete pmenu;
}

void VatBin::printAllTypeOfData(CString path )
{
	_mkdir(path);
	GetDataTypes();
	MDataNode*pdata = NULL;
	int addr = 0;
	HANDLE hfile = 0;
	CString FilePath;
	for (UINT i = 0;i < mDataTypes.size();i++)
	{
		FilePath.Format("%X", mDataTypes[i]);
		FilePath = path + "\\" + FilePath + ".txt";
		hfile = GetFileHandle_W(FilePath);
		addr = GetVA(mDataBeginAddr);
		while (1)
		{
			if (*(int*)addr == 6)
			{
				break;
			}
			pdata = new MDataNode(addr);
			if (pdata->mType_1 == mDataTypes[i])
			{
				pdata->NewWrite(hfile);
			}
			addr += pdata->mAllLen;
			delete pdata;
		}
		CloseHandle(hfile);
	}
}

void VatBin::printOneSpecialFuncByName(int TagDataType,CString name, CString path)
{
	CString tPath = path + name;
	_mkdir(tPath);
	MDataNode*pdata = NULL;
	CString NewPath;
	int addr = GetVA(mDataBeginAddr);
	while (1)
	{
		if (*(int*)addr == 6)
		{
			break;
		}
		pdata = new MDataNode(addr);
		if (pdata->mType_1==TagDataType)
		{
			if (pdata->vecNode_4[2].mStr!="")
			{
				int cc = 0;
			}
			if (pdata->vecNode_4[2].mStrX == name)
			{
				CString csAddr;
				csAddr.Format("%08X", GetFOA(pdata->mAddr));
				csAddr += "_" + pdata->vecNode_4[1].mStrX;
				NewPath = tPath + "\\" + csAddr+".txt";
				HANDLE hfile = GetFileHandle_W(NewPath);
				DataConfiguration::WriteOneDataConfig(pdata->mAddr, hfile);
				CloseHandle(hfile);
			}
		}
		addr += pdata->mAllLen;
		delete pdata;
	}
}

void VatBin::Trace(int TagDataType, int MenuAddr, CString ModuleName,CString path,CString name)
{
	MenuNode* pmenu = new MenuNode(MenuAddr);
	int NextAddr = 0;
	if (pmenu->mName == ModuleName)
	{
		MenuNode* tNextMenu = 0;
		CString tRoutine;
		if (pmenu->mNextMenus.size())
		{
			for (size_t i = 0; i < pmenu->mNextMenus[0].mNextNum; i++)
			{
				tRoutine = path;
				tNextMenu = new MenuNode(pmenu->mNextMenus[0].mNextMenuAddrs[i]);
				tRoutine += tNextMenu->mName;
				vector<int> vecResult;
				for (size_t i = 0; i < tNextMenu->mConfigNum; i++)
				{
					if (tNextMenu->mConfigurations[i].mConfigType == 0xD)
					{
						DataConfiguration::GetOneFuncDataConfigByName(tNextMenu->mConfigurations[i].mConfigNode, TagDataType, name, vecResult);
					}
				}
				CString buf;
				HANDLE hfile;
				CString xx;
				for (size_t i = 0; i < vecResult.size(); i++)
				{
					xx = tRoutine;
					xx.Replace("C:\\Users\\Administrator\\Desktop\\Engine Oil Life Reset\\", "");
					buf.Format("%08X", GetFOA(vecResult[i]));
					xx = buf + "_" + xx;
					xx.Replace("VAT Release_Global_Diagnostics_", "");
					xx.Replace("_Module Diagnostics_Engine Control Module", "");
					tRoutine = "C:\\Users\\Administrator\\Desktop\\Engine Oil Life Reset\\" + xx;
					hfile = GetFileHandle_W(tRoutine + ".txt");
					DataConfiguration::WriteOneDataConfig(vecResult[i], hfile);
					CloseHandle(hfile);
				}
				delete tNextMenu;
			}
			return;
		}
	}
	if (pmenu->mNextMenus.size())
	{
		CString tPath;
		MenuNode* tmenu;
		for (int i = 0;i < pmenu->mNextMenus[0].mNextNum;i++)
		{
			tPath = path;
			NextAddr = pmenu->mNextMenus[0].mNextMenuAddrs[i];
			tmenu = new MenuNode(NextAddr);
			tPath += tmenu->mName + "_";
			Trace(TagDataType, NextAddr, ModuleName,tPath, name);
			delete tmenu;
		}
	}
	delete pmenu;
}


void VatBin::Trace1(int TagDataType, int MenuAddr, CString path, CString name)
{
	MenuNode* pmenu = new MenuNode(MenuAddr);
	int NextAddr = 0;
	CString tRoutine;
	tRoutine = path;
	vector<int> vecResult;
	for (size_t i = 0; i < pmenu->mConfigNum; i++)
	{
		if (pmenu->mConfigurations[i].mConfigType == 0xD)
		{
			DataConfiguration::GetOneFuncDataConfigByName(pmenu->mConfigurations[i].mConfigNode, TagDataType, name, vecResult);
		}
	}
	CString buf;
	HANDLE hfile;
	CString xx;
	for (size_t i = 0; i < vecResult.size(); i++)
	{
		xx = tRoutine;
		CString ttt = "C:\\Users\\Administrator\\Desktop\\Gm Sepcial\\" + name + "\\" + "VAT Release_Global_Diagnostics_";
		xx.Replace(ttt,"");
		xx.Replace("_Module Diagnostics_", "");
		buf.Format("%08X", GetFOA(vecResult[i]));
		xx = buf + "_" + xx;
		tRoutine = "C:\\Users\\Administrator\\Desktop\\Gm Sepcial\\" +name+"\\"+ xx;
		hfile = GetFileHandle_W(tRoutine + ".txt");
		DataConfiguration::WriteOneDataConfig(vecResult[i], hfile);
		CloseHandle(hfile);
	}

	if (pmenu->mNextMenus.size())
	{
		CString tPath;
		MenuNode* tmenu;
		for (int i = 0;i < pmenu->mNextMenus[0].mNextNum;i++)
		{
			tPath = path;
			NextAddr = pmenu->mNextMenus[0].mNextMenuAddrs[i];
			tmenu = new MenuNode(NextAddr);
			tPath += tmenu->mName + "_";
			Trace1(TagDataType, NextAddr, tPath, name);
			delete tmenu;
		}
	}
	delete pmenu;
}


void VatBin::printOneSpecialFuncByNamePro(int TagDataType, int MenuAddr,CString name, CString path)
{
	CString tPath = path + name+"\\";
	_mkdir(tPath);
	//Trace(TagDataType, MenuAddr, module,tPath,name);
	Trace1(TagDataType, MenuAddr, tPath, name);
}

inline HANDLE VatBin::GetFileHandle_W(CString path)
{
	return CreateFile(path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

inline HANDLE VatBin::GetFileHandle_R(CString path)
{
	return CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

int VatBin::GetDataListBeginAddr(int addr)
{
	int NextAddr = addr;
	int CheckValue = 0;
	int dyArrLen = 0;
	int FullLen = 0;

	while (CheckValue != 6)
	{
		dyArrLen = *(int*)(NextAddr + 8);
		FullLen = 12 + 12 * dyArrLen;
		NextAddr += FullLen;
		CheckValue = *(int*)NextAddr;
	}
	return NextAddr - (int)bin.mpVaddr;
}

void VatBin::WriteTotalStateTab(HANDLE hfile)
{
	int addr = mStateBeginAddr + (int)mpVaddr;
	State* pNode = 0;
	for (int i = 0;i < mStateNum;i++)
	{
		pNode = new State(addr);
		pNode->write(hfile);
		addr += 56;
		delete pNode;
	}
}

void VatBin::WriteStatePieceTab(HANDLE hfile)
{
	int addr = mStateBeginAddr + (int)mpVaddr;
	State* pNode = 0;
	for (int i = 0;i < mStateNum;i++)
	{
		pNode = new State(addr);
		for (int i = 0;i < pNode->mStatePieceNum;i++)
		{
			pNode->mStatePieces[i].write(hfile);
		}
		DWORD dw = 0;
		WriteFile(hfile, "\x0d\x0a", 2, &dw, 0);
		addr += 56;
		delete pNode;
	}
}

void VatBin::TravMenu(int addr, CString path)
{
	MenuNode* pmenu=new MenuNode(addr);
	CString NextPath;
	int NextAddr = 0;
	pmenu->CreateFiles(path);//创建菜单包含的相关文件

	NextPath = path + "\\" + pmenu->mName;
	if (pmenu->mNextMenus.size())
	{
		for (int i = 0;i < pmenu->mNextMenus[0].mNextNum;i++)
		{
			NextAddr = pmenu->mNextMenus[0].mNextMenuAddrs[i];
			TravMenu(NextAddr, NextPath);
		}
	}
	delete pmenu;
}

void VatBin::GetVecCstr(int addr,vector<OutMenu>& vecOutMenu)
{
	MenuNode menu(addr);
	int NextAddr = 0;
	if (menu.mName != "")
	{
		OutMenu tMenu;
		tMenu.mAddr = GetFOA(addr);
		tMenu.mName = menu.mName;
		vecOutMenu.push_back(tMenu);
	}
	if (menu.mNextMenus.size())
	{
		for (int i = 0;i < menu.mNextMenus[0].mNextNum;i++)
		{
			NextAddr = menu.mNextMenus[0].mNextMenuAddrs[i];
			GetVecCstr(NextAddr, vecOutMenu);
		}
	}
}

void VatBin::GetTotalMenu(int addr)
{
	MenuNode menu(addr);
	if (menu.mNextMenus.size()&& menu.mNextMenus[0].mOption != "")
	{
		GlobalMenu gMenu;
		gMenu.mTittle = menu.mNextMenus[0].mType;
		vector<OutMenu> vecOutMenu;
		int NextAddr = 0;
		for (int i = 0;i < menu.mNextMenus[0].mNextNum;i++)
		{
			NextAddr = menu.mNextMenus[0].mNextMenuAddrs[i];
			GetVecCstr(NextAddr, vecOutMenu);
		}
		gMenu.mOutMenu = vecOutMenu;
		mGlobalMenu.push_back(gMenu);	
	}
	int NextAddr = 0;
	if (menu.mNextMenus.size())
	{
		for (int i = 0;i < menu.mNextMenus[0].mNextNum;i++)
		{
			NextAddr = menu.mNextMenus[0].mNextMenuAddrs[i];
			GetTotalMenu(NextAddr);
		}
	}
}

void VatBin::TravMenuToMenu(int addr, HANDLE hfile)
{
	MenuNode menu(addr);
	if (menu.mMenuDescripts[0].mDescript == "Vehicle Menu")
	{
		if (mGlobalMenu.size())//进入模块配置数据之前先清空
		{
			mGlobalMenu.clear();
			vector<GlobalMenu>().swap(mGlobalMenu);
		}
		GetTotalMenu(menu.mConfigurations[0].mConfigNode);
		MenuNode NextMenu(menu.mNextMenus[0].mNextMenuAddrs[0]);
		for (UINT i=0;i< NextMenu.mNextMenus[0].mNextNum;i++)
		{
			TravMenuToMenu(NextMenu.mNextMenus[0].mNextMenuAddrs[i], hfile);
		}
	}
	else if (menu.mName == "Under Construction"||
			 menu.mName == "Global Test Menu")
	{
		int cc = 0;
	}
	else if (menu.mName.Find("$",0)!=-1)
	{
		for (int i=0;i< m$MatchesInModule.size();i++)
		{
			for (int j = 0;j < mGlobalMenu.size();j++)
			{
				if (m$MatchesInModule[i].mBCD.mKey == mGlobalMenu[j].mTittle)
				{

				}
			}
		}
	}
	else if (menu.mMenuDescripts[0].mDescript == "Module Data Node")
	{
		if (m$MatchesInModule.size())//进入模块配置数据之前先清空
		{
			m$MatchesInModule.clear();
			vector<The$Dict>().swap(m$MatchesInModule);
		}
		for (int i = 0;i < menu.mConfigNum;i++)
		{
			Get$MatchesInModule(menu.mConfigurations[i].mConfigNode);
		}
		int cc = 0;
		
	}
	else
	{
		menu.WriteStdMenu(hfile);
	}
	int NextAddr = 0;
	if (menu.mNextMenus.size())
	{
		for (int i = 0;i < menu.mNextMenus[0].mNextNum;i++)
		{
			NextAddr = menu.mNextMenus[0].mNextMenuAddrs[i];
			TravMenuToMenu(NextAddr, hfile);
		}
	}
}

void VatBin::TravMenuToMenu_Txt(CString path)
{
	HANDLE hfile = GetFileHandle_W(path);
	int addr = GetVA(mMenuAddr);
	MenuNode* pNode = 0;
	int CheckAddr = 0;
	while (1)
	{
		CheckAddr = *(int*)(addr + 36);
		if (CheckAddr < mMenuAddr)
		{
			return;
		}
		pNode = new MenuNode(addr);
		if (pNode->mNameAddr != 0)
		{
			int flag = 1;
			for (int i = 0;i < mVecMenuNameAddr.size();i++)
			{
				if(pNode->mNameAddr == mVecMenuNameAddr[i])
				{ 
					flag = 0;
				}
			}
			if (flag)
			{
				int cc = 0;
				if (pNode->mName.Find("(Global") == -1 &&
					pNode->mName.Find(" QS") == -1)
				{
					mVecMenuNameAddr.push_back(pNode->mNameAddr);
					pNode->WriteStdMenu_Txt(hfile);
				}
			}
		}
		addr += 44;
		delete pNode;
	}
	CloseHandle(hfile);
}

void VatBin::GetMenuByName(int MenuAddr,CString name, MenuNode& MenuToGet)
{
	MenuNode menu(MenuAddr);
	int NextAddr = 0;

	if(menu.mName == name)
	{
		MenuToGet= menu;
		return;
	}
	if(menu.mNextMenus.size())
	{
		for (int i = 0;i < menu.mNextMenus[0].mNextNum;i++)
		{
			NextAddr = menu.mNextMenus[0].mNextMenuAddrs[i];
			GetMenuByName(NextAddr, name, MenuToGet);
		}
	}
}

void VatBin::Get$MatchesInModule(int addr)
{
	int NodeType = DataConfiguration::GetNodeType(addr);
	if (NodeType == Type_MDataNode)
	{
		MDataNode MNode(addr);
		for (int i = 0;i < MNode.vecNode_4.size();i++)
		{
			if (MNode.vecNode_4[i].mIsStr==true&&
				MNode.vecNode_4[i].mStr.Find("$")!=-1)
			{
				The$Dict OnePiece;
				OnePiece.mStrWith$ = MNode.vecNode_4[i].mStr;
				MDataListNode tNode(GetVA(MNode.vecNode_4[i + 1].mVal_2));
				if (tNode.vecStrs.size())
				{
					BCD tBCD(tNode.vecStrs[0]);
					OnePiece.mBCD = tBCD;
				}

				int flag = 1;
				for (UINT i = 0;i < m$MatchesInModule.size();i++)
				{
					if (OnePiece == m$MatchesInModule[i])
					{
						flag = 0;
					}
				}
				if (flag)
				{
					m$MatchesInModule.push_back(OnePiece);
				}
			}
		}
		for (UINT i = 0;i < MNode.vecNode_4.size();i++)
		{
			int MNodeAddr = MNode.vecNode_4[i].mVal_2;
			bool IsNode = (MNodeAddr >= bin.mDataBeginAddr && MNodeAddr <= bin.mDataListEndAddr);
			if (IsNode)
			{
				Get$MatchesInModule(GetVA(MNodeAddr));
			}
		}
	}
	if (NodeType == Type_MDataListNode)
	{
		MDataListNode MListNode(addr);
		if (MListNode.mType_3 != 5 && MListNode.mType_3 != 3)
		{
			for (UINT i = 0;i < MListNode.vecMDataAddrs.size();i++)
			{
				int MListNodeAddr = MListNode.vecMDataAddrs[i];
				Get$MatchesInModule(GetVA(MListNodeAddr));
			}
		}
	}
}

void VatBin::GetDataTypes()
{
	MDataNode*pdata = NULL;
	int addr = GetVA(mDataBeginAddr);
	while (1)
	{
		if (*(int*)addr == 6)
		{
			break;
		}
		pdata = new MDataNode(addr);
		int flag = 1;
		for (UINT i = 0;i < mDataTypes.size();i++)
		{
			if (pdata->mType_1 == mDataTypes[i])
			{
				flag = 0;
			}
		}
		if (flag)
		{
			mDataTypes.push_back(pdata->mType_1);
		}
		addr += pdata->mAllLen;
		delete pdata;
	}
}

void VatBin::ExpMenu(CString PathMenu, CString PathMenu_Txt)
{
	MenuNode menu;
	GetMenuByName(GetVA(mMenuAddr), "Diagnostics", menu);
	HANDLE hfile = GetFileHandle_W(PathMenu);
	TravMenuToMenu(menu.mAddr, hfile);
	TravMenuToMenu_Txt(PathMenu_Txt);
	CloseHandle(hfile);
}

DataConfiguration::DataConfiguration(int addr)
{
	int StateAddr = *(int*)addr + (int)bin.mpVaddr;
	StateDescript tStateDescript(StateAddr);
	mState.push_back(tStateDescript);
	mConfigType = *(int*)(addr + 4);
	mConfigNode = *(int*)(addr + 8) + (int)bin.mpVaddr;
	if (mConfigType == 0)
	{
		mType = type_SelectMenu;
	}
	else
	{
		mType = type_data;
	}
}

int DataConfiguration::GetNodeType(int addr)
{
	if (*(int*)addr == 6)
	{
		return Type_MDataListNode;
	}
	else
	{
		return Type_MDataNode;
	}
}

void DataConfiguration::WriteOneDataConfig(int addr, HANDLE FileHandle)
{
	int NodeType = GetNodeType(addr);
	if (NodeType == Type_MDataNode)
	{
		MDataNode MNode(addr);
		MNode.Write(FileHandle);
		for (UINT i = 0;i < MNode.vecNode_4.size();i++)
		{
			int MNodeAddr = MNode.vecNode_4[i].mVal_2;
			bool IsNode = (MNodeAddr >= bin.mDataBeginAddr && MNodeAddr <= bin.mDataListEndAddr);
			if (IsNode&&MNode.vecNode_4[i].mType_1!=4 && MNode.vecNode_4[i].mType_1 != 3)
			{
				WriteOneDataConfig(GetVA(MNodeAddr), FileHandle);
			}
			if (IsNode && MNode.vecNode_4[i].mType_1 == 3&& MNodeAddr!=0x05050404&& MNodeAddr != 0x04040101)
			{
				WriteOneDataConfig(GetVA(MNodeAddr), FileHandle);
			}
		}
	}
	if (NodeType == Type_MDataListNode)
	{
		MDataListNode MListNode(addr);
		MListNode.Write(FileHandle);
		if (MListNode.mType_3 != 5 && 
			MListNode.mType_3 != 3 && 
			MListNode.mType_3 != 4 &&
			MListNode.mType_3 != 1)
		{
			for (UINT i = 0;i < MListNode.vecMDataAddrs.size();i++)
			{
				int MListNodeAddr = MListNode.vecMDataAddrs[i];
				WriteOneDataConfig(GetVA(MListNodeAddr), FileHandle);
			}
		}
	}
}

void DataConfiguration::GetOneFuncDataConfigByName(int addr, int type,CString name,vector<int>& result)
{
	int NodeType = GetNodeType(addr);
	if (NodeType == Type_MDataNode)
	{
		MDataNode MNode(addr);
		if (MNode.mType_1== type&&
			MNode.vecNode_4[2].mStrX==name)
		{
			result.push_back(MNode.mAddr);
		}
		for (UINT i = 0;i < MNode.vecNode_4.size();i++)
		{
			int MNodeAddr = MNode.vecNode_4[i].mVal_2;
			bool IsNode = (MNodeAddr >= bin.mDataBeginAddr && MNodeAddr <= bin.mDataListEndAddr);
			//if (IsNode&&MNode.vecNode_4[i].mType_1 != 4)
			//{
			//	GetOneFuncDataConfigByName(GetVA(MNodeAddr), type,name, result);
			//}

			if (IsNode && MNode.vecNode_4[i].mType_1 != 4 && MNode.vecNode_4[i].mType_1 != 3)
			{
				GetOneFuncDataConfigByName(GetVA(MNodeAddr), type, name, result);
			}
			if (IsNode && MNode.vecNode_4[i].mType_1 == 3 && MNodeAddr != 0x05050404 && MNodeAddr != 0x04040101)
			{
				GetOneFuncDataConfigByName(GetVA(MNodeAddr), type, name, result);
			}
		}
	}
	if (NodeType == Type_MDataListNode)
	{
		MDataListNode MListNode(addr);
		if (MListNode.mType_3 != 5 &&
			MListNode.mType_3 != 3 &&
			MListNode.mType_3 != 4 &&
			MListNode.mType_3 != 1)
		{
			for (UINT i = 0;i < MListNode.vecMDataAddrs.size();i++)
			{
				int MListNodeAddr = MListNode.vecMDataAddrs[i];
				GetOneFuncDataConfigByName(GetVA(MListNodeAddr), type, name, result);
			}
		}
	}
}

MenuDescription::MenuDescription(int addr)
{
	int TypeAddr = *(int*)addr + (int)bin.mpVaddr;
	int DescriptAddr = *(int*)(addr + 4) + (int)bin.mpVaddr;
	WCHAR* pType = (WCHAR*)TypeAddr + 3;
	WCHAR* pDescript = (WCHAR*)DescriptAddr + 3;
	for (int i = 0;pType[i] != 0;i++)
	{
		mType += (char)pType[i];
	}
	for (int i = 0;pDescript[i] != 0;i++)
	{
		mDescript += (char)pDescript[i];
		int t = 0;
	}
}

NextMenus::NextMenus(int addr)
{
	int temp = 0;
	temp = *(int*)addr;
	int TypeAddr = temp + (int)bin.mpVaddr;
	WCHAR* pType = (WCHAR*)TypeAddr + 3;
	for (int i = 0;pType[i] != 0;i++)
	{
		mType += (char)pType[i];
	}

	temp = *(int*)(addr + 4);
	if (temp)
	{
		int OptionAddr = temp + (int)bin.mpVaddr;
		WCHAR* pDescript = (WCHAR*)OptionAddr + 3;
		for (int i = 0;pDescript[i] != 0;i++)
		{
			mOption += (char)pDescript[i];
		}
	}
	mNextNum = *(int*)(addr + 8);
	int BaseAddr = *(int*)(addr + 12);
	for (int i = 0;i < mNextNum;i++)
	{
		int tAddr = BaseAddr + 44 * i + (int)bin.mpVaddr;
		mNextMenuAddrs.push_back(tAddr);
	}
}

MenuNode::MenuNode(int addr)
{
	mAddr = addr;
	mSerial = *(int*)addr;
	mNameAddr = *(int*)(addr + 4);
	if (mNameAddr)
	{
		WCHAR* pName = (WCHAR*)(GetVA(mNameAddr)) + 3;
		for (int i = 0;pName[i] != 0;i++)
		{
			mName += (char)pName[i];
		}
	}
	//int StateAddr = *(int*)(addr + 12);
	//if (StateAddr)
	//{
	//	StateAddr = GetVA(StateAddr);
	//	State tState(StateAddr);
	//	mState.push_back(tState);
	//}
	mConfigNum = *(int*)(addr + 16);
	if (mConfigNum)
	{
		int ConfigAddr = GetVA(*(int*)(addr + 20));
		for (int i = 0;i < mConfigNum;i++)
		{
			DataConfiguration tConfig(ConfigAddr + 12 * i);
			mConfigurations.push_back(tConfig);
		}
	}
	mDescriptNum = *(int*)(addr + 32);
	int DescriptAddr = GetVA(*(int*)(addr + 36));
	for (int i = 0;i < mDescriptNum;i++)
	{
		MenuDescription tMenuDescription(DescriptAddr + 8 * i);
		mMenuDescripts.push_back(tMenuDescription);
	}
	int NextMenuAddr = *(int*)(addr + 40);
	if (NextMenuAddr)
	{
		NextMenuAddr = GetVA(NextMenuAddr);
		NextMenus tNextMenus(NextMenuAddr);
		mNextMenus.push_back(tNextMenus);
	}
}

void MenuNode::write(HANDLE hfile)
{
	DWORD dw = 0;
	CString Menu_TxtBuf = GetMenu_TxtBuf();
	WriteFile(hfile, Menu_TxtBuf, Menu_TxtBuf.GetLength(), &dw, 0);
}

void MenuNode::WriteOrin(HANDLE hfile)
{
	CString buf;
	DWORD dw = 0;
	buf.Format("%08X", GetFOA(mAddr));
	buf = "0x" + buf + ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	for (int i=0;i<11;i++)
	{
		buf.Format("%08X", *(int*)(mAddr + 4 * i));
		buf = "0x" + buf + ",";
		WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	}
	buf = "," + mName + ",";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	if (mState.size())
	{
		buf = mState[0].mFullName + "\x0D\x0A";
		WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	}
	else
	{
		WriteFile(hfile, "\x0D\x0A", 2, &dw, 0);
	}
}

void MenuNode::WriteStdMenu_Txt(HANDLE hfile)
{
	DWORD dw = 0;
	CString buf;
	buf = GetMenuNameID() + "\x09" + '"' + mName + '"' + "\x0D\x0A";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
}

void MenuNode::WriteStdMenu(HANDLE hfile)
{
	DWORD dw = 0;
	short num = 0;
	UCHAR tVal = 0;
	CString buf = GetMenuID();

	CString vecBuf = GetNextMenus();

	if (mNextMenus.size())
	{
		num = mNextMenus[0].mNextNum;
	}
	CString str;
	CString Astr;
	str.Format("%02X", (UCHAR)(num >> 8));
	Astr = "0x" + str + ",";
	str.Format("%02X", (UCHAR)num);
	Astr += "0x" + str;

	buf = GetMenuID() + "\x09" + GetMenuNameID() + "\x09" + mTaskId + "\x09"+ Astr+ "\x09"+vecBuf + "\x0D\x0A";

	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
}

void MenuNode::WriteMenuWithState(HANDLE hfile)
{
	DWORD dw = 0;
	CString buf;
	if (mState.size())
	{
		buf = GetMenuNameID() + "\x09" + '"' + mName + '"' + "\x09" + mState[0].mFullName + "\x0D\x0A";
	}
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
}

void MenuNode::CreateConfig(CString path)
{
	if (mConfigurations[0].mType == type_data)
	{
		CString NewPath = path + "\\" + "Configuration";
		_mkdir(NewPath);
		CString ConfigName;
		DWORD dw = 0;
		for (int i = 0;i < mConfigNum;i++)
		{
			ConfigName.Format("%X", mConfigurations[i].mConfigType);
			ConfigName = NewPath + "\\" + ConfigName + ".txt";
			HANDLE hfile = CreateFile(ConfigName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			mConfigurations[i].WriteOneDataConfig(mConfigurations[i].mConfigNode, hfile);
			CloseHandle(hfile);
		}
	}
	//if (mConfigurations[0].mType == type_SelectMenu)
	//{
	//	CString NewPath = path + "\\" + mName + "_GlobalSelectMenu";
	//	VatBin::TravMenu(mConfigurations[0].mConfigNode, NewPath); 
	//}
}

CString MenuNode::GetMenuID()
{
	CString buf;
	CString tStr;
	UCHAR tVal = 0;
	UINT id = 0;
	buf = "0x61,0xFF,";
	id = GetFOA(mAddr);
	for (int i = 0;i < 4;i++)
	{
		tVal = (UCHAR)(id >> 24 - 8 * i);
		tStr.Format("%02X", tVal);
		if (i != 3)
		{
			tStr = "0x" + tStr + ",";
		}
		else
		{
			tStr = "0x" + tStr;
		}
		buf += tStr;
	}
	return buf;
}

CString MenuNode::GetMenuNameID()
{
	CString buf;
	CString tStr;
	UCHAR tVal = 0;
	UINT addr = 0;
	buf = "0x61,0xFF,";
	addr = *(int*)(mAddr+4);
	for (int i = 0;i < 4;i++)
	{
		tVal = (UCHAR)(addr >> 24 - 8 * i);
		tStr.Format("%02X", tVal);
		if (i != 3)
		{
			tStr = "0x" + tStr + ",";
		}
		else
		{
			tStr = "0x" + tStr;
		}
		buf += tStr;
	}
	return buf;
}

void MenuNode::CreateFiles(CString path)
{
	CString FolderName = mName;
	//FolderName.Replace('/', '_');
	CString NewPath;
	if (FolderName != "")
	{
		NewPath = path + "\\" + FolderName;
		NewPath.Replace('/', '_');
		_mkdir(NewPath);
	}
	if (mConfigNum != 0 &&
		mConfigurations.size() != 0)
	{
		NewPath = path + "\\" + mName;
		path = path + "\\" + FolderName;
		path.Replace('/', '_');
		_mkdir(path);
		CreateConfig(path);
	}
}

CString MenuNode::GetMenu_TxtBuf()
{
	CString buf;
	UINT addr = GetFOA(mAddr);
	UCHAR tVal = 0;
	CString tStr;

	buf = "0x61,0xFF,";

	for (int i = 0;i < 4;i++)
	{
		tVal = (UCHAR)(addr >> 24 - 8 * i);
		tStr.Format("%02X", tVal);
		if (i != 3)
		{
			tStr = "0x" + tStr + ",";
		}
		else
		{
			tStr = "0x" + tStr;
		}
		buf += tStr;
	}
	buf = buf + "\x09" + '"' + mName + '"'+"\x0D\x0A";
	return buf;
}

CString MenuNode::GetNextMenus()
{
	CString buf;
	CString tStr;
	CString AllStrs;
	UCHAR tVal = 0;
	UINT addr = 0;
	if (mNextMenus.size())
	{
		for (int i = 0;i < mNextMenus[0].mNextNum;i++)
		{
			buf = "0x61,0xFF,";
			addr = GetFOA(mNextMenus[0].mNextMenuAddrs[i]);
			for (int i = 0;i < 4;i++)
			{
				tVal = (UCHAR)(addr >> 24 - 8 * i);
				tStr.Format("%02X", tVal);
				if (i != 3)
				{
					tStr = "0x" + tStr + ",";
				}
				else
				{
					tStr = "0x" + tStr;
				}
				buf += tStr;
			}
			AllStrs += buf + ",";
		}
	}
	return AllStrs;
}

void MDataListNode::WriteWStr(HANDLE FileHandle, int addr)
{
	char* p = (char*)addr;
	p += 6;//文件中实际宽字符串是起始地址+6开始的
	int len = wcslen((WCHAR*)p);
	DWORD dw = 0;
	for (int i = 0;i < len;i++)//循环写入字符串
	{
		WriteFile(FileHandle, p, 1, &dw, 0);
		p += 2;
	}
}

MDataListNode::MDataListNode(int addr)
{
	mAddr = addr;
	mVal6_1 = *(int*)addr;
	mCnt_2 = *(int*)(addr + 4);
	mType_3 = *(int*)(addr + 8);
	mLen_4 = *(int*)(addr + 12);
	mAllLen = 16 + 8 * mLen_4;

	int tAddr = 0;
	if (mType_3 == 5)
	{
		mNodeType = type_StrContainer;
		for (int i = 0;i < mLen_4;i++)
		{
			tAddr = *(double*)(addr + 16 + 8 * i);
			DataStr str(GetVA(tAddr));
			vecStrs.push_back(str.mStr);
		}
	}
	else
	{
		if (mType_3 == 3)
		{
			mNodeType = type_IntValContainer;
		}
		else
		{
			mNodeType = type_NodeCluster;
		}
		for (int i = 0;i < mLen_4;i++)
		{
			tAddr = *(double*)(addr + 16 + 8 * i);
			vecMDataAddrs.push_back(tAddr);
		}
	}
}

void MDataListNode::Write(HANDLE FileHandle)
{
	DWORD dw = 0;
	CString buf;
	int dwVal = 0;

	buf.Format("%08X", GetFOA(mAddr));
	buf =buf+"\x09";
	WriteFile(FileHandle, buf, buf.GetLength(), &dw, 0);//写入首地址
	for (UINT i = 0;i < 4;i++)//写入前4个整型数据
	{
		dwVal = *(int*)(mAddr + 4 * i);
		if (i == 3)
		{
			buf.Format("%X", dwVal);
		} 
		else
		{
			buf.Format("%08X", dwVal);
		}
		buf += "\x09";
		WriteFile(FileHandle, buf, buf.GetLength(), &dw, 0);
	}
	for (UINT i = 0;i < mLen_4;i++)
	{
		if (mType_3 ==5)
		{
			if (vecStrs.size())
			{
				buf = vecStrs[i]+"\x09";
				WriteFile(FileHandle, buf, buf.GetLength(), &dw, 0);
			}
		}
		else
		{
			buf.Format("%08X", vecMDataAddrs[i]);
			buf = buf + "\x09";
			WriteFile(FileHandle, buf, buf.GetLength(), &dw, 0);
		}
	}
	WriteFile(FileHandle, "\x0d\x0a", 2, &dw, 0);
}

DataStr::DataStr(int addr)
{
	mLanguageType = *(int*)addr;
	mXX = *(short*)(addr + 4);
	WCHAR*p = (WCHAR*)(addr + 6);
	for (int i = 0;i < wcslen(p);i++)
	{
		mStr += (char)p[i];
	}
}

DyArray::DyArray(int addr)
{
	mAddr = addr;
	mType_1 = *(int*)addr;
	mVal_2 = (int)(*(double*)(addr + 4));
	if (mType_1 == 5)
	{
		if (mVal_2 >= bin.mDataBeginAddr&&
			mVal_2 <= bin.mDataListEndAddr ||
			mVal_2 == 0x80000000)//43ec
		{
			mStr.Format("%08X", mVal_2);
		}
		else
		{
			DataStr tStr(GetVA(mVal_2));
			mStrX = tStr.mStr;
			mStr = '"' + tStr.mStr + '"';
			mIsStr = true;
		}
	}
	else
	{
		mStr.Format("%08X", mVal_2);
	}
}

void DyArray::Write(HANDLE hfile)
{
	CString buf;
	DWORD dw = 0;
	CString xx;
	xx.Format("%d", mType_1);
	buf = xx + "\x09" + mStr + "\x09";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
}

void DyArray::NewWrite(HANDLE hfile)
{
	CString buf;
	DWORD dw = 0;
	CString NewBuf;
	if (mVal_2>=bin.mDataListBeginAddr&&
		mVal_2<=bin.mDataListEndAddr)
	{
		MDataListNode tListNode(GetVA(mVal_2));
		if (tListNode.mNodeType==type_StrContainer)
		{
			if (tListNode.vecStrs.size())
			{
				for (UINT i = 0;i < tListNode.vecStrs.size();i++)
				{
					NewBuf += "\x09" + tListNode.vecStrs[i];
				}
			}
			else
			{
				NewBuf += "\x22\x22\x09";
			}
			WriteFile(hfile, NewBuf, NewBuf.GetLength(), &dw, 0);
			return;
		}
		if (tListNode.mNodeType == type_IntValContainer)
		{
			NewBuf = buf;
			CString ttbuf;
			for (UINT i = 0;i < tListNode.vecMDataAddrs.size();i++)
			{
				ttbuf.Format("%08X", tListNode.vecMDataAddrs[i]);
				NewBuf += "\x09" + ttbuf;
			}
			WriteFile(hfile, NewBuf, NewBuf.GetLength(), &dw, 0);
			return;
		}
	}
	CString xx;
	xx.Format("%d", mType_1);
	buf = xx + "\t" + mStr + "\t";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
}

MDataNode::MDataNode(int addr)
{
	mAddr = addr;
	mType_1 = *(int*)addr;
	mCnt_2 = *(int*)(addr + 4);
	mLen_3 = *(int*)(addr + 8);
	for (int i = 0;i < mLen_3;i++)
	{
		DyArray node(addr + 12 + 12 * i);
		vecNode_4.push_back(node);
	}
	mAllLen = 12 + 12 * mLen_3;
}

int MDataNode::isMData()
{
	if (mType_1 != 6)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void MDataNode::Write(HANDLE FileHandle)//将Mdata数据写入到txt文档中
{
	CString str;
	DWORD dw = 0;
	str.Format("%08X", GetFOA(mAddr));
	str = str + "\x09";
	WriteFile(FileHandle, str, str.GetLength(), &dw, 0);//写入首地址
	for (UINT i = 0;i < 3;i++)//写入前三个字段
	{
		str.Format("%08X", *(int*)(mAddr + 4 * i));
		str = str + "\x09";
		WriteFile(FileHandle, str, str.GetLength(), &dw, 0);
	}
	for (UINT i = 0;i < vecNode_4.size();i++)//写入动态数组
	{
		vecNode_4[i].NewWrite(FileHandle);
	}
	WriteFile(FileHandle, "\x0d\x0a", 2, &dw, 0);
}

void MDataNode::NewWrite(HANDLE hfile)
{
	CString str;
	DWORD dw = 0;
	str.Format("%08X", GetFOA(mAddr));
	str = str + "\x09";
	WriteFile(hfile, str, str.GetLength(), &dw, 0);//写入首地址
	for (UINT i = 0;i < 3;i++)//写入前三个字段
	{
		str.Format("%08X", *(int*)(mAddr + 4 * i));
		str = str + "\x09";
		WriteFile(hfile, str, str.GetLength(), &dw, 0);
	}
	for (UINT i = 0;i < vecNode_4.size();i++)//写入动态数组
	{
		vecNode_4[i].NewWrite(hfile);
	}
	WriteFile(hfile, "\x0d\x0a", 2, &dw, 0);
}

bool The$Dict::operator==(The$Dict& other)
{
	if (mBCD.mAstr == other.mBCD.mAstr&&
		mStrWith$==other.mStrWith$)
	{
		return true;
	}
	return false;
}

void The$Dict::WriteModuleMatchedMenu(MenuNode& menu, HANDLE hfile)
{
	DWORD dw = 0;
	short num = 0;
	UCHAR tVal = 0;
	CString buf = menu.GetMenuID();

	CString vecBuf = menu.GetNextMenus();


	CString str;
	CString Astr;
	str.Format("%02X", (UCHAR)(num >> 8));
	Astr = "0x" + str + ",";
	str.Format("%02X", (UCHAR)num);
	Astr += "0x" + str;

	buf = menu.GetMenuID() + "\x09" + menu.GetMenuNameID() + "\x09" + menu.mTaskId + "\x09" + Astr + "\x09" + vecBuf + "\x0D\x0A";

	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
}

BCD::BCD(CString BCDstr)
{
	CString buf;
	int cnt = 0;

	mAstr = BCDstr;
	for (int i = 4;i < BCDstr.GetLength();i++)
	{
		if (BCDstr[i] == ',')
		{
			if (cnt == 0)
			{
				mKey = buf;
				buf = "";
				cnt++;
			}
			if (cnt == 1)
			{
				mVal = buf;
				buf = "";
			}
		}
		else if (i == BCDstr.GetLength()-1)
		{
			mCalcSig = buf;
			int cc = 0;
		}
		else
		{
			buf += BCDstr[i];
		}
		
	}
}

vector<CString> GlobalMenu::GetGlobalMenu_TxtBuf()
{
	vector<CString>vecBuf;
	CString buf;
	buf = "0x61,0xFF,0x00,0x00,0x00,0x00,\x09" + mTittle;
	vecBuf.push_back(buf);


	CString tStr;
	UCHAR tVal = 0;
	UINT addr = 0;
	for (UINT i = 0;i < mOutMenu.size();i++)
	{
		buf = "0x61,0xFF,";
		addr = mOutMenu[i].mAddr;
		for (int j = 0;j < 4;j++)
		{
			tVal = (UCHAR)(addr >> 24 - 8 * i);
			tStr.Format("%02X", tVal);
			if (i != 3)
			{
				tStr = "0x" + tStr + ",";
			}
			else
			{
				tStr = "0x" + tStr;
			}
			buf += tStr;
		}
		buf = buf + "\x09" + '"' + mOutMenu[i].mName + '"' + "\x0D\x0A";
		vecBuf.push_back(buf);
	}
	return vecBuf;
}

void GlobalMenu::write(HANDLE hfile)
{
	vector<CString> vecStr = GetGlobalMenu_TxtBuf();
	for (int i=0;i<vecStr.size();i++)
	{
		
	}
}

void DtcLib::write(HANDLE hfile)
{
	CString buf;
	DWORD dw = 0;
	buf.Format("%08X", GetFOA(mAddr));
	buf += "\x09";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf.Format("%08X", mNameAddr);
	buf += "\x09";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf.Format("%08X", mDtcNum);
	buf += "\x09";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf.Format("%08X", mLibAddr);
	buf += "\x09";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
	buf = mDtcLibName;
	buf += "\x0d\x0a";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
}

DtcLib::DtcLib(int addr)
{
	mAddr = addr;
	mNameAddr = *(int*)addr;
	mDtcNum = *(int*)(addr + 4);
	mLibAddr= *(int*)(addr + 8);
	DataStr tStr(GetVA(mNameAddr));
	mDtcLibName = tStr.mStr;
}

PerDTC::PerDTC(int addr)
{
	mAddr = GetFOA(addr);
	mId = *(int*)addr;
	int tAddr = 0;
	tAddr = *(int*)(addr + 4);
	DataStr tStr1(GetVA(tAddr));
	mstr1 = tStr1.mStr;

	tAddr = *(int*)(addr + 8);
	DataStr tStr2(GetVA(tAddr));
	mstr2 = tStr2.mStr;

	tAddr = *(int*)(addr + 12);
	DataStr tStr3(GetVA(tAddr));
	mstr3 = tStr3.mStr;

	tAddr = *(int*)(addr + 16);
	DataStr tStr4(GetVA(tAddr));
	mstr4 = tStr4.mStr;

	tAddr = *(int*)(addr + 20);
	DataStr tStr5(GetVA(tAddr));
	mstr5 = tStr5.mStr;
}

void PerDTC::write(HANDLE hfile) 
{
	CString buf;
	DWORD dw = 0;
	buf.Format("%08X", mAddr);
	buf += "\x09";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);

	buf.Format("%08X", mId);
	buf = buf + "\x09" + mstr1 + "\x09" + "\x09" + mstr3 + "\x0d\x0a";
	WriteFile(hfile, buf, buf.GetLength(), &dw, 0);
}
