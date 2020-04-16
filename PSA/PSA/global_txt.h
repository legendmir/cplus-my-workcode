#pragma once
#include<string>
#include<vector>
#include<map>
#include"du8.h"
#include"sqls.h"
#include"fire_bird.h"
#include"basic_data.h"
using namespace std;


class c_state
{
public:
	c_state();
public:
	void print(string path = "C:\\Users\\Administrator\\Desktop\\PSA_data\\state.txt");
	void insert(string key, string value);
	string convert_to_buffer(int num);
	static map<string, string>m_map;
};


class c_format
{
public:
	c_format();
public:
	string m_match;
	string m_replce;
private:
	string convert_to_buffer(int num);
public:
	void print(string path = "C:\\Users\\Administrator\\Desktop\\PSA_data\\format.txt");
	void insert(string str);
	string get_string_from_map(string str);
	static map<string, string>m_map;
	static map<int, string>m_exp_map;
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

class c_conver_tab
{
public:
	c_conver_tab();
	

public:
	void print(string path="C:\\Users\\Administrator\\Desktop\\PSA_data\\conver_tab.txt");
private:
	vector<c_conver> m_vec;
};


class c_txt {
public:
	c_txt();

public:
	void print(string path= "C:\\Users\\Administrator\\Desktop\\PSA_data\\en_txt.txt");
	void insert_map(string key_str);
	string get_str_from_map(string str);


private:
	string convert_to_buffer(int num);
public:
	static map<string, string>m_map;
	static map<int, string>m_exp_map;

};