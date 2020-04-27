#pragma once
#include<string>
#include<map>
using namespace std;
class c_current_veh {
public:
	c_current_veh();
	string m_comtype;
	string m_moudle_name;
	string m_ecu_type;
	string m_ecu_descript;
	string m_file_name;
	string m_file_path;
	map<string, string> map_global_textbuffer;
};

class menu_info {
public:
	string m_file_path;
	string m_file_name;
	string m_menu_string;
};