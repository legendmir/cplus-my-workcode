#pragma once
#include<string>
#include<vector>
#include<map>
#include<io.h>
#include"path.h"
using namespace std;

enum protocol_type {
	LIGNE_K = 1,
	CAN_IS,
	CAN_CAR,
	CAN_CONF,
	CAN_DIAG,
	LIN_BSI_2,
	CAN_INFO_DIV,
	LIN_BSI_1,
	LIN_BSM
};

enum file_type{
	ft_ident,
	ft_standard,
	ft_defauts,
	ft_action,
	ft_repa,
	ft_else,
};

class utils {
public:
	static vector<string> split(string& ori_string, char sig);
	static string convert_4_byte(int num);
	static string convert_2_byte(short num);
	static string convert_2_byte(int num);
	static string convert_1_byte(int num);
	static string convert_3_byte(short x1, char x2);
	static string convert_3_byte(int num);
	static string get_menu_id_buffer(int menu_num, int menu_sub_num);
	static string& replace_all(string& str, const  string& old_value, const  string& new_value);
	static string get_descript(string str);
	static protocol_type get_protocol(string str);
	static string get_menu_file_name(string file_path);
	static string get_variant_file_name(string file_path);
	static file_type get_func_file_type(string file_name);
	static string convert_bit2_int(string bit_str);
	static bool is_script_file(string& file_name);
	static vector<string> get_basic_vec_files(string file_path);
	static vector<string> get_all_vec_files(string file_path);
};