#include"DataExp.h"

//VatBin bin("F:\\Job Project\\GM\\New\\vatbin.bin");
//VatBin bin("F:\\Job Project\\GM\\New\\binarys\\OV_OpelVauxhall_v2018.9.0_vatbin.bin");
//VatBin bin("F:\\Job Project\\GM\\New\\binarys\\OpelVauxhall_v2019.2.0_vatbin.bin");
VatBin bin("F:\\Job Project\\GM\\New\\vatbin.bin");

int main()
{

	bin.TravMenuByName(GetVA(bin.mMenuAddr), "2017", "C:\\Users\\Administrator\\Desktop");
	//bin.TravMenuByName(GetVA(bin.mMenuAddr), "2017", "C:\\Users\\admin\\Desktop");
	//bin.printOrinMenu();
	//bin.printDataSeg();
	//bin.printDataListSeg();
	//bin.printAllTypeOfData();
	//bin.printOneCfg(0x03C42B86,"C:\\Users\\admin\\Desktop\\xxx.txt");
	//bin.printDataSeg();
	//bin.printDataListSeg();
	//bin.printOneSpecialFuncByName(0x444b, "Engine Oil Life Reset");
	//bin.TravMenuByName(GetVA(bin.mMenuAddr),"");
	//bin.TravMenu(GetVA(bin.mMenuAddr),"C:\\Users\\admin\\Desktop\\xxx123");
	//bin.printOneSpecialFuncByNamePro(0x444b,GetVA(bin.mMenuAddr),"Engine Oil Life Reset");
	return 0;
}


