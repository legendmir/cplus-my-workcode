#pragma once
#include<string>
#include<vector>
using namespace std;
class c_du8
{
public:
	c_du8();

public:
	static string get_string_from_du8(string du8_name, int id);

public:
	static string convert_du8_string(string str);
	static char* get_du8_buffer(string du8name);
	static vector<string>split_buffer(string str);
	static vector<string> split(string& ori_string, char sig);

public:
	static const char* m_lang_sig;
	static const char* m_trans_path;

public:
	static char* m_buffer_POLUXDATA;
	static char* m_buffer_POLUXVEH;
	static char* m_buffer_POLUXXPERT;
	static char* m_buffer_POLUXMES;
	static char* m_buffer_ENGINE;
	static char* m_buffer_POLUXPORT;
	static char* m_buffer_POLUXSPV;
	static char* m_buffer_AWMMI;
	static char* m_buffer_DBMMI;
	static char* m_buffer_VEHMNT;
	static char* m_buffer_LAUNCHER;
};