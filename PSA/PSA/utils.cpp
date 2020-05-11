#include"utils.h"
#include <algorithm>

string utils::convert_4_byte(int num)
{
	string result;
	unsigned char h1 = num >> 24;
	unsigned char h2 = num >> 16;
	unsigned char h3 = num >> 8;
	unsigned char h4 = num;

	char buffer[256]{};
	sprintf(buffer, "0x%02X,0x%02X,0x%02X,0x%02X", h1, h2, h3, h4);
	result = buffer;
	return result;
}

string utils::convert_2_byte(short num)
{
	unsigned char high = num >> 8;
	unsigned char low = (unsigned char)num;

	char buf[20]{};
	sprintf(buf, "0x%02X,0x%02X", high, low);
	string result = buf;
	return buf;
}

string utils::convert_2_byte(int num)
{
	unsigned char high = num >> 8;
	unsigned char low = num;

	char buf[20]{};
	sprintf(buf, "0x%02X,0x%02X", high, low);
	string result = buf;
	return buf;
}


string utils::convert_1_byte(int num)
{
	unsigned char xxx = num;
	char buf[20]{};
	sprintf(buf, "0x%02X", xxx);
	return buf;
}


string utils::convert_3_byte(short x1, char x2)
{
	string result;

	unsigned char h3 = x1 >> 8;
	unsigned char h4 = (unsigned char)x1;

	char buffer[256]{};
	sprintf(buffer, "0x%02X,0x%02X,0x%02X", h3, h4, x2);
	result = buffer;
	return result;
}

string utils::convert_3_byte(int x1)
{
	string result;

	unsigned char h1 = x1 >> 16;
	unsigned char h2 = x1 >> 8;
	unsigned char h3 = x1;

	char buffer[256]{};
	sprintf(buffer, "0x%02X,0x%02X,0x%02X", h1, h2, h3);
	result = buffer;
	return result;
}

string utils::get_menu_id_buffer(int menu_num, int menu_sub_num)
{
	string head = "0x17,0xFF,";
	string result;
	unsigned char h1 = menu_num >> 16;
	unsigned char h2 = menu_num >> 8;
	unsigned char h3 = menu_num;
	unsigned char sub = menu_sub_num;

	char buffer[256]{};
	sprintf(buffer, "0x%02X,0x%02X,0x%02X,0x%02X\t", h1, h2, h3, sub);
	result += buffer;
	return result;
}

vector<string> utils::split(string& ori_string, char sig)
{
	unsigned int i = 0;
	vector<string> vec_str;
	string tstr;
	unsigned int flag = 0;
	for (i = 0;i < ori_string.size();i++)
	{

		if (ori_string[i] == '"' && flag == 0)
		{
			flag = 1;
			i++;
		}
		else if (ori_string[i] == '"' && flag == 1)
		{
			flag = 0;
			i++;
		}
		if (ori_string[i] == sig && flag == 0)
		{
			vec_str.push_back(tstr);
			tstr.clear();
		}
		else
		{
			tstr += ori_string[i];
		}
	}
	vec_str.push_back(tstr);
	return vec_str;
}


string& utils::replace_all(string& str, const  string& old_value, const  string& new_value)
{
	while (true)
	{
		string::size_type   pos(0);
		if ((pos = str.find(old_value)) != string::npos)
		{
			str.replace(pos, old_value.length(), new_value);
		}
		else { break; }
	}
	return   str;
}


string utils::get_descript(string xx)
{
	vector<string> result = utils::split(xx, ' ');
	string t_str = result.back();
	string xxx;
	for (size_t i = 4;i < t_str.size();i++)
	{
		xxx.push_back(t_str[i]);
	}
	return xxx;
}

protocol_type utils::get_protocol(string famcanbus)
{

	if (famcanbus == "LIGNE_K")
	{
		return LIGNE_K;
	}
	if (famcanbus == "CAN_IS")
	{
		return CAN_IS;
	}
	if (famcanbus == "CAN_CAR")
	{
		return CAN_CAR;

	}
	if (famcanbus == "CAN_CONF")
	{
		return CAN_CONF;
	}
	if (famcanbus == "CAN_DIAG")
	{
		return CAN_DIAG;
	}
	if (famcanbus == "LIN_BSI_2")
	{
		return LIN_BSI_2;
	}
	if (famcanbus == "CAN_INFO_DIV")
	{
		return CAN_INFO_DIV;
	}
	if (famcanbus == "LIN_BSI_1")
	{
		return LIN_BSI_1;
	}
	if (famcanbus == "LIN_BSM")
	{
		return LIN_BSM;
	}

}


string utils::get_menu_file_name(string file_path)
{
	string path = c_path::source_path + "\\" + file_path + "\\*.*";

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(path.c_str(), &findData);
	if (handle == -1)        // 检查是否成功
		return string();

	do
	{
		if (findData.attrib & _A_SUBDIR)
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;

			// 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
		}
		else
		{
			string tfile_name = findData.name;
			string check_value;
			for (size_t i = 0;i < 5;i++)
			{
				check_value.push_back(tfile_name[i]);
			}
			if (check_value == "MENU_")
			{
				return string(findData.name);
			}
		}
	} while (_findnext(handle, &findData) == 0);

	_findclose(handle);    // 关闭搜索句柄

	return string();
}

string utils::get_variant_file_name(string file_path)
{
	string path = c_path::source_path + "\\" + file_path + "\\*.*";

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(path.c_str(), &findData);
	if (handle == -1)        // 检查是否成功
		return string();

	do
	{
		if (findData.attrib & _A_SUBDIR)
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;

			// 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
		}
		else
		{
			string tfile_name = findData.name;
			transform(tfile_name.begin(), tfile_name.end(), tfile_name.begin(), ::toupper);
			if (tfile_name.find("VARIANTE_CALCULATEUR")!=-1)
			{
				return findData.name;
			}
		}
	} while (_findnext(handle, &findData) == 0);

	_findclose(handle);    // 关闭搜索句柄

	return string();
}

file_type utils::get_func_file_type(string file_name)
{
	transform(file_name.begin(), file_name.end(), file_name.begin(), ::toupper);
	if (file_name.find("IDENTIFICATION")!=-1)
	{
		return ft_ident;
	}
	else if (file_name.find("STANDARD") != -1)
	{
		return ft_standard;
	}
	else if (file_name.find("DEFAUTS") != -1)
	{
		return ft_defauts;
	}
	else if (file_name.find("REPA") != -1)
	{
		return ft_repa;
	}
	else
	{
		return ft_action;
	}
}

string utils::convert_bit2_int(string bit_str)
{
	const char* p = bit_str.c_str();
	unsigned int len = bit_str.size();
	int result = 0;
	unsigned int i = 0;
	for (i = len - 1;; i--)
	{
		result += (p[i] - '0') * pow(2, len - i - 1);
		if (i==0)
		{
			break;
		}
	}
	return utils::convert_2_byte(result);
}
