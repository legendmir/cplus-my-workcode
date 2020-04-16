#include"utils.h"

string utils::convert_4_byte(int num)
{
	string result;
	unsigned char h1 = num >> 24;
	unsigned char h2 = num >> 16;
	unsigned char h3 = num >> 8;
	unsigned char h4 = num;

	char buffer[256]{};
	sprintf(buffer, "0x%02X,0x%02X,0x%02X,0x%02X\t", h1, h2, h3, h4);
	result = buffer;
	return result;
}

string utils::convert_2_byte(short num)
{
	unsigned char high = num >> 8;
	unsigned char low = num;

	char buf[20]{};
	sprintf(buf, "0x%02X,0x%02X", high, low);
	string result = buf;
	return buf;
}

string utils::convert_2_byte_1(int num)
{
	unsigned char high = num >> 8;
	unsigned char low = num;

	char buf[20]{};
	sprintf(buf, "0x%02X,0x%02X\t", high, low);
	string result = buf;
	return buf;
}

string utils::convert_1_byte(char num)
{
	char buf[20]{};
	sprintf(buf, "0x%02X", num);
	string result = buf;
	return result;
}

string utils::convert_3_byte(short x1, char x2)
{
	string result;

	unsigned char h3 = x1 >> 8;
	unsigned char h4 = x1;

	char buffer[256]{};
	sprintf(buffer, "0x%02X,0x%02X,0x%02X", h3, h4, x2);
	result = buffer;
	return result;
}

string utils::convert_3_byte_1(int x1)
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