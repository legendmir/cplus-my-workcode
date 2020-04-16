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

class DyArray //�����ݶζ�̬����Ľڵ�,����һ��type �� һ����������ַָ�������
{
public:
	DyArray(int addr);
	void Write(HANDLE FileHandle);
	void NewWrite(HANDLE);
	int mAddr;
	int mType_1;//��һ���ֶ�����
	int mVal_2;//�ڶ����ֶ�����
	int mIsStr = 0;
	CString mStr;
	CString mStrX;
};

class MDataNode //�����ݶ�0x0580D736-08006F0A
{
public:
	MDataNode(int );
	void Write(HANDLE );//����̬����д��txt�ĵ�
	void NewWrite(HANDLE);//д��ȫ���ַ������ݣ�
	int isMData();
	int mAllLen;
	vector<DyArray> vecNode_4;
public:
	int mAddr;  //��ʼ��ַ
	int mType_1;//�ֶ�1��ֵ
	int mCnt_2; //�ֶ�2ֵ
	int mLen_3;//�ֶ�3ֵ
};

enum MDataListNodeType
{
	type_NodeCluster,
	type_IntValContainer,
	type_StrContainer
};

class MDataListNode//����һС�������ݽڵ��ַ�Ľڵ�0x08006F76-0x0921F80E
{
public:
	MDataListNode(int addr);
	void Write(HANDLE FileHandle);
	void WriteWStr(HANDLE FileHandle, int addr);
	int mVal6_1;//��һ���ε�ֵΪ�̶���6
	int mAllLen;
public:
	int mAddr;//��ʼ��ַ
	int mNodeType;//MDataListNode
	int mCnt_2;//Ӧ����ĳ�����Ͱ�
	int mType_3;//���ܺ�������
	int mLen_4;//Ϊ���������MData�׵�ַ����
	vector<int> vecMDataAddrs;//�����MData���׵�ַ
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

enum NodeType { Type_MDataNode, Type_MDataListNode };//ö�ٵĽڵ�����

class DataConfiguration {//2�����͵�����,һ�����������ݶ�,һ���������������ò˵�
public:
	DataConfiguration(int addr);
	static void WriteOneDataConfig(int addr, HANDLE FileHandle);
	static void GetOneFuncDataConfigByName(int addr,int type,CString name, vector<int>& result);
	static int GetNodeType(int addr);//{ Type_MDataNode, Type_MDataListNode };//ö�ٵĽڵ�����

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
	void TravMenuByName(int MenuAddr, CString name, CString path = "C:\\Users\\Administrator\\Desktop");//�漰�ݹ�,���봫��ַ
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
	static void TravMenu(int MenuAddr, CString path);//�����˵�
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
	int mDtcLibNum;		//56-60����������
	int mDtcLibAddr;	//60-64��������ַ
	int mDtcPerLibAddr;

	int mDllFunNum;     //��̬�⺯����72-76
	int mDllLibAddr;    //64-68��68-72��76-80�ֽڶ�̬������ַ
	int mSomeStr;		//80-84һЩ����Ҫ���ַ���($����֮���)

	//int mVersion;		//0-4�ֽڰ汾����
	//int mX_2;			//4-8�ֽ�ʼ����0
	//int mEnd20Bytes;	//8-12ָ��ĩβ20�ֽ�
	//int mX_4;			//12-16?
	//int mX_5;			//16-20?
	//int mXstrAddr;		//20-24װGlobal module year �ַ���
	//int mLanguageNum;   //24-28����������
	//int mLanguageAddr;  //28-32��������ʼ��ַ
	//int mX_8;           //32-36������ĳ������28e22
	//int mX_9;           //36-40��1CA
	//int mMenuAddr;		//40-44�˵���ַ
	//int mStateNum;		//44-48״̬����
	//int mStateAddr;		//48-52״̬��ַ
	//int mStateLinkAddr;	//��������,״̬�ڵ��ַ
	//int mDataAddr;		//52-56�����ݶε�ַ
	//int mDataListAddr;	//�������ã����ݽڵ��
	//int mDtcLibNum;		//56-60����������
	//int mDtcLibAddr;	//60-64��������ַ
	//int mDllFunNum;     //��̬�⺯����72-76
	//int mDllLibAddr;    //64-68��68-72��76-80�ֽڶ�̬������ַ
	//int mSomeStr;		//80-84һЩ����Ҫ���ַ���($����֮���)

	CString mRoutineCollection;
	int mRoutineCollectionFlag = 0;

	vector<int>mVecMenuNameAddr;
	vector<The$Dict> m$MatchesInModule;
	vector<GlobalMenu> mGlobalMenu;
	vector<int>mDataTypes;
};
