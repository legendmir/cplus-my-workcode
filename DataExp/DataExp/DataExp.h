#pragma once
#include<windows.h>
#include<vector>
#include<atlstr.h>
#include <direct.h>  
#include "state.h"

using namespace std;

#define GetVA(x) x+(int)bin.mpVaddr

#define GetFOA(x) x-(int)bin.mpVaddr

class DataStr {
public:
	DataStr(int addr);
	DataStr() {};
	int mLanguageType;
	short mXX;
	CString mStr;
};

class DyArray //主数据段动态数组的节点,包括一个type 和 一个浮点数地址指向的内容
{
public:
	DyArray(int addr);
	void Write(HANDLE FileHandle);
	void NewWrite(HANDLE);
	int mAddr;
	int mType_1;//第一个字段内容
	int mVal_2;//第二个字段内容
	int mIsStr = 0;
	CString mStr;
	CString mStrX;
};

class MDataNode //主数据段0x0580D736-08006F0A
{
public:
	MDataNode(int );
	void Write(HANDLE );//将动态数组写入txt文档
	void NewWrite(HANDLE);//写入全部字符串数据；
	int isMData();
	int mAllLen;
	vector<DyArray> vecNode_4;
public:
	int mAddr;  //起始地址
	int mType_1;//字段1的值
	int mCnt_2; //字段2值
	int mLen_3;//字段3值
};

enum MDataListNodeType
{
	type_NodeCluster,
	type_IntValContainer,
	type_StrContainer
};

class MDataListNode//保存一小段主数据节点地址的节点0x08006F76-0x0921F80E
{
public:
	MDataListNode(int addr);
	void Write(HANDLE FileHandle);
	void WriteWStr(HANDLE FileHandle, int addr);
	int mVal6_1;//第一个段的值为固定的6
	int mAllLen;
public:
	int mAddr;//起始地址
	int mNodeType;//MDataListNode
	int mCnt_2;//应该是某种类型吧
	int mType_3;//可能和序号相关
	int mLen_4;//为后续保存的MData首地址数量
	vector<int> vecMDataAddrs;//保存的MData的首地址
	vector<CString>vecStrs;
};

class MenuDescription {
public:

	MenuDescription(int addr);
	CString mType;
	CString mDescript;
};

class NextMenus {
public:

	NextMenus(int addr);
	CString mType;
	CString mOption;
	int mNextNum;
	vector<int> mNextMenuAddrs;
};

enum DataConfigurationType{type_data,type_SelectMenu};

enum NodeType { Type_MDataNode, Type_MDataListNode };//枚举的节点类型

class DataConfiguration {//2种类型的配置,一种是配置数据段,一种是配置汽车配置菜单
public:
	DataConfiguration(int addr);
	static void WriteOneDataConfig(int addr, HANDLE FileHandle);
	static void GetOneFuncDataConfigByName(int addr,int type,CString name, vector<int>& result);
	static int GetNodeType(int addr);//{ Type_MDataNode, Type_MDataListNode };//枚举的节点类型

public:
	int mType;//{type_data,type_SelectMenu}
	vector<StateDescript> mState;
	int mConfigType;
	int mConfigNode;
};

class OutMenu
{
public:
	int mAddr;
	CString mName;
};

class GlobalMenu
{
public:
	CString mTittle;
	vector<OutMenu> mOutMenu;
	vector<CString> GetGlobalMenu_TxtBuf();
	void write(HANDLE);
};

class MenuNode {
public:
	MenuNode(int addr);
	MenuNode() {};
	void write(HANDLE hfile);
	void WriteOrin(HANDLE hfile);
	void WriteStdMenu_Txt(HANDLE);
	void WriteStdMenu(HANDLE);
	void WriteMenuWithState(HANDLE);
	void CreateFiles(CString path);
public:
	int mAddr;
	int mSerial;
	int mNameAddr;
	CString mName;
	vector<State> mState;
	int mConfigNum;
	vector<DataConfiguration> mConfigurations;
	int mDescriptNum;
	vector<MenuDescription> mMenuDescripts;
	vector<NextMenus> mNextMenus;
	CString mTaskId = "0x00,0x00,0x00,0x00";

public:
	//void CreateInfo(CString path);
	void CreateConfig(CString path);
	CString GetMenuID();
	CString GetMenuNameID();
	CString GetMenu_TxtBuf();
	CString GetNextMenus();
};

class BCD
{
public:
	BCD() {};
	BCD(CString);
	CString mAstr;
	CString mKey;
	CString mVal;
	CString mCalcSig;
};

class The$Dict {
public:
	The$Dict() {};
	CString mStrWith$;
	BCD mBCD;
public:
	bool operator==(The$Dict&);
	void WriteModuleMatchedMenu(MenuNode& ,HANDLE);
};

class DtcLib {
public:
	DtcLib(int);
	void write(HANDLE);
	int mAddr;
	int mNameAddr;
	int mDtcNum;
	int mLibAddr;
	CString mDtcLibName;
};

class PerDTC{
public:
	PerDTC(int);
	void write(HANDLE);
	int mAddr;
	int mId;
	CString mstr1;
	CString mstr2;
	CString mstr3;
	CString mstr4;
	CString mstr5;
};

class VatBin {
public:
	VatBin(CString Path);

	void printOneCfg(int addr,CString path = "C:\\Users\\Administrator\\Desktop\\testData.txt");
	void printMenu(CString Path = "C:\\Users\\Administrator\\Desktop\\menu.txt");
	void printOrinMenu(CString Path = "C:\\Users\\Administrator\\Desktop\\menu.txt");
	void printXmenu(CString Path = "C:\\Users\\Administrator\\Desktop\\menu.txt");
	void printState(CString Path = "C:\\Users\\Administrator\\Desktop\\StateTotalTab.txt");
	void printStatePiece(CString Path = "C:\\Users\\Administrator\\Desktop\\StatePieceTab.txt");
	void printStateLinkTab(CString Path = "C:\\Users\\Administrator\\Desktop\\StateLinkTab.txt");
	void printDataSeg(CString PathData = "C:\\Users\\Administrator\\Desktop\\data.txt");
	void printDataListSeg(CString Data1Path = "C:\\Users\\Administrator\\Desktop\\data1.txt");
	void printVinInfo(CString path = "C:\\Users\\Administrator\\Desktop\\VinInfo.txt");
	void printTotalStateData(CString path = "C:\\Users\\Administrator\\Desktop\\ToltalStates");
	void printAggregatedStateData(CString path = "C:\\Users\\Administrator\\Desktop\\AggregatedStateData");
	void TravMenuByName(int MenuAddr, CString name, CString path = "C:\\Users\\Administrator\\Desktop");//涉及递归,必须传地址
	void printAllTypeOfData(CString path = "C:\\Users\\Administrator\\Desktop\\TypesOfData");
	void printOneSpecialFuncByName(int TagDataType, CString name, CString path = "C:\\Users\\Administrator\\Desktop\\");


	void Trace(int TagDataType, int MenuAddr, CString module, CString path, CString name);
	void Trace1(int TagDataType, int MenuAddr, CString path, CString name);


	void printOneSpecialFuncByNamePro(int TagDataType, int MenuAddr, CString name, CString path = "C:\\Users\\Administrator\\Desktop\\Gm Sepcial\\");
	void printDtcLib(CString path = "C:\\Users\\Administrator\\Desktop\\DtcLib.txt");
	void printDtcs(CString path = "C:\\Users\\Administrator\\Desktop\\Dtcs.txt");


public:
	void TravMenuToMenu(int addr, HANDLE hfile);
	void TravMenuToMenu_Txt(CString path = "C:\\Users\\Administrator\\Desktop\\Menu_Txt.txt");
	void ExpMenu(CString path1 = "C:\\Users\\admin\\Desktop\\MENU.TXT", CString path2 = "C:\\Users\\Administrator\\Desktop\\Menu_Txt.txt");

public:
	static void TravMenu(int MenuAddr, CString path);//遍历菜单
	void GetMenuByName(int MenuAddr, CString name, MenuNode& MenuToGet);
	void GetTotalMenu(int addr);

private:
	void WriteTotalStateTab(HANDLE hfile);
	void WriteStatePieceTab(HANDLE hfile);
	void GetVecCstr(int addr, vector<OutMenu>& vecOutMenu);
	int GetDataListBeginAddr(int addr);
	void Get$MatchesInModule(int addr);
	void GetDataTypes();

	inline HANDLE GetFileHandle_W(CString);
	inline HANDLE GetFileHandle_R(CString);

public:
	char* mpVaddr = new char[0x10000000]{};

	int mDataBeginAddr;
	int mDataListBeginAddr;
	int mDataListEndAddr;
	int mMenuAddr;
	int mStateNum;
	int mStateBeginAddr;
	int mStateLinkBeginAddr;
	int mDtcLibNum;		//56-60故障码数量
	int mDtcLibAddr;	//60-64故障码库地址
	int mDtcPerLibAddr;

	int mDllFunNum;     //动态库函数名72-76
	int mDllLibAddr;    //64-68和68-72和76-80字节动态库名地址
	int mSomeStr;		//80-84一些很重要的字符串($变种之类的)

	//int mVersion;		//0-4字节版本描述
	//int mX_2;			//4-8字节始终是0
	//int mEnd20Bytes;	//8-12指向末尾20字节
	//int mX_4;			//12-16?
	//int mX_5;			//16-20?
	//int mXstrAddr;		//20-24装Global module year 字符串
	//int mLanguageNum;   //24-28多语言类型
	//int mLanguageAddr;  //28-32多语言起始地址
	//int mX_8;           //32-36可能是某种数量28e22
	//int mX_9;           //36-40？1CA
	//int mMenuAddr;		//40-44菜单地址
	//int mStateNum;		//44-48状态数量
	//int mStateAddr;		//48-52状态地址
	//int mStateLinkAddr;	//计算所得,状态节点地址
	//int mDataAddr;		//52-56主数据段地址
	//int mDataListAddr;	//计算所得，数据节点簇
	//int mDtcLibNum;		//56-60故障码数量
	//int mDtcLibAddr;	//60-64故障码库地址
	//int mDllFunNum;     //动态库函数名72-76
	//int mDllLibAddr;    //64-68和68-72和76-80字节动态库名地址
	//int mSomeStr;		//80-84一些很重要的字符串($变种之类的)

	CString mRoutineCollection;
	int mRoutineCollectionFlag = 0;

	vector<int>mVecMenuNameAddr;
	vector<The$Dict> m$MatchesInModule;
	vector<GlobalMenu> mGlobalMenu;
	vector<int>mDataTypes;
};
