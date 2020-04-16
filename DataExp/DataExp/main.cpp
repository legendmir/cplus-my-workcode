#include"DataExp.h"

//VatBin bin("F:\\Job Project\\GM\\New\\vatbin.bin");
//VatBin bin("F:\\Job Project\\GM\\New\\binarys\\OV_OpelVauxhall_v2018.9.0_vatbin.bin");
//VatBin bin("F:\\Job Project\\GM\\New\\binarys\\OpelVauxhall_v2019.2.0_vatbin.bin");
VatBin bin("F:\\Job Project\\GM\\New\\vatbin.bin");

vector<CString> get_vec_str(CString path= "C:\\Users\\Administrator\\Desktop\\Gm Sepcial\\func_list.txt")
{
	HANDLE hfile= CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int size = GetFileSize(hfile, 0);
	DWORD xx;
	char* buf = new char[size] {};
	ReadFile(hfile, buf, size, &xx, 0);
	CString temp;
	vector<CString>result;
	for (int i = 0;i < size;i++)
	{
		temp += buf[i];
		if (buf[i] == '\n')
		{
			result.push_back(temp);
			temp = "";
		}
	}
	return result;
}

int main()
{

	//bin.TravMenuByName(GetVA(bin.mMenuAddr), "Diagnostics", "C:\\Users\\Administrator\\Desktop");
	//bin.TravMenuByName(GetVA(bin.mMenuAddr), "2017", "C:\\Users\\Administrator\\Desktop");
	//bin.printOrinMenu();
	//bin.printDataSeg();
	//bin.printDataListSeg();
	//bin.printAllTypeOfData();
	//bin.printOneCfg(0x3CDC68E,"C:\\Users\\Administrator\\Desktop\\xxx.txt");
	//bin.printDataSeg();
	//bin.printDataListSeg();
	//bin.printOneSpecialFuncByName(0x444b, "Engine Oil Life Reset");
	//bin.TravMenuByName(GetVA(bin.mMenuAddr),"");
	//bin.TravMenu(GetVA(bin.mMenuAddr),"C:\\Users\\admin\\Desktop\\xxx123");

	vector<CString> vec_str = get_vec_str();
	for (int i = 0;i < vec_str.size();i++)
	{
		CString xx = vec_str[i];
		xx.Replace("\r\n","");
		bin.printOneSpecialFuncByNamePro(0x444b, GetVA(bin.mMenuAddr), xx);
	}

	//bin.printOneSpecialFuncByNamePro(0x444b,GetVA(bin.mMenuAddr),"Yaw Rate Sensor Learn");
	return 0;
}


