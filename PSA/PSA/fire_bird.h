#pragma once
#include "core/ibpp.h"
#include "core/_ibpp.h"
#include<string>
#include<map>
#include"sqls.h"
using namespace std;
extern c_sql g_sqls;

struct data_unit
{
	string data_value;
	string data_type;
};

class c_fire_bird {
public:
	c_fire_bird(string db_name, string sql);
	vector<map<string, data_unit>>get_result_tab();

public:
	IBPP::Database m_db;
	IBPP::Transaction m_tr;
	IBPP::Statement m_st;
	IBPP::Blob m_res;
};

namespace fire_bird
{
	vector<map<string, data_unit>>get_sql_result(sql_type st_sql, vector<string> vec_str);
	vector<map<string, data_unit>>get_sql_result(string& db_name, const char* format, ...);
}