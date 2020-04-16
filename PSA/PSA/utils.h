#pragma once
#include<string>
#include<vector>
using namespace std;

class utils {
public:
	static vector<string> split(string& ori_string, char sig);
	static string convert_4_byte(int num);
	static string convert_2_byte(short num);
	static string convert_2_byte_1(int num);
	static string convert_1_byte(char num);
	static string convert_3_byte(short x1, char x2);
	static string convert_3_byte_1(int num);
	static string get_menu_id_buffer(int menu_num, int menu_sub_num);
	static string& replace_all(string& str, const  string& old_value, const  string& new_value);

};