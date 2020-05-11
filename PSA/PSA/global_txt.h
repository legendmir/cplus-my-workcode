#pragma once
#include<string>
#include<vector>
#include<map>
#include"du8.h"
#include"sqls.h"
#include"fire_bird.h"
#include"basic_data.h"
using namespace std;


class c_tab
{
public:
	c_tab();
public:
	static void insert(string key, string value);
	static map<string, string>m_map;
	static FILE* fp_tab_conver;
};


class c_format
{
public:
	c_format();
public:
	static void insert(string par_name);
	static string get_string_from_map(string str);
	static map<string, string>m_map;
	static FILE* fp_format;
};


class c_conver
{
public:
	c_conver();
private:
	string convert_head(string& par_id);
	string convert_factor(string& fctor);
	string convert_offset(string& offset);
	string convert_unit(string& unit);

public:
	string get_buffer();
	string m_factor;
	string m_offset;
	string m_unit;
	string m_parid;
};

class c_line_conver
{
public:
	c_line_conver();
public:
	void print();
private:
	vector<c_conver> m_vec;
};

class c_txt {
public:
	c_txt();

public:
	static void insert_map(string key_str);
	static string get_str_from_map(string str);
	static FILE* fp_txt;
public:
	static map<string, string>m_map;
};