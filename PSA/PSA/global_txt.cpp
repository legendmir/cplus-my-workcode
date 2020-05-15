#include"global_txt.h"
#include"utils.h"

map<string, string> c_txt::m_map;
FILE* c_txt::fp_txt = fopen("C:\\Users\\Administrator\\Desktop\\PSA_data\\en_txt.txt", "w");

map<string, string> c_format::m_map;
FILE* c_format::fp_format = fopen("C:\\Users\\Administrator\\Desktop\\PSA_data\\format.txt", "w");

map<string, string> c_tab::m_map;
FILE* c_tab::fp_tab_conver = fopen("C:\\Users\\Administrator\\Desktop\\PSA_data\\tableConv.txt", "w");

map<string, string> c_sfile_cmd::m_map;
FILE* c_sfile_cmd::fp_sfile_cmd = fopen("C:\\Users\\Administrator\\Desktop\\PSA_data\\sfile_cmd.txt", "w");


c_txt::c_txt()
{
}

void c_txt::insert_map(string key_str)
{
	string repared_str;

	repared_str = c_du8::convert_du8_string(key_str);
	const char* pp = key_str.c_str();
	if (repared_str =="")
	{
		printf("%s\n", pp);
	}
	string buffer;
	if (!m_map.count(repared_str))
	{
		int num = m_map.size() + 1;
		string num_str = "0x17,0xFF," + utils::convert_4_byte(num);
		m_map[repared_str] = num_str;
		buffer = num_str + "\t\"" + repared_str + "\"\n";
		fwrite(buffer.c_str(), 1, buffer.size(), fp_txt);
	}
}

string c_txt::get_str_from_map(string str)
{
	string xx = c_du8::convert_du8_string(str);
	return m_map[xx];
}

c_format::c_format()
{

}

void c_format::insert(string par_name)
{
	string buffer;
	if (!m_map.count(par_name))
	{
		int num = m_map.size() + 1;
		string num_str = string("0xFD,0x00,0x00,") + utils::convert_4_byte(num);
		m_map[par_name] = num_str;
		buffer = num_str + "\t" + par_name +"\n";
		fwrite(buffer.c_str(), 1, buffer.size(), fp_format);
	}
}

string c_format::get_string_from_map(string str)
{
	return m_map[str];
}

c_line_conver::c_line_conver()
{
	vector<string> vec_str;
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_gloabl_conver, vec_str);
	c_conver tt;
	for (u_int i = 0;i < result.size();i++)
	{
		tt.m_factor = result[i]["CONFACTOR"].data_value;
		tt.m_offset = result[i]["CONOFFSET"].data_value;
		tt.m_unit = result[i]["CONUNIT"].data_value;
		tt.m_parid = result[i]["PARID"].data_value;
		m_vec.push_back(tt);
	}
}

string c_conver::convert_head(string& par_id)
{
	string head;
	int id = stoi(par_id);
	head += utils::convert_4_byte(id);
	return head;
}

string c_conver::convert_factor(string& fctor)
{
	int xx = stod(fctor) * 10000;
	return utils::convert_4_byte(xx);
}

string c_conver::convert_offset(string& offset)
{
	int xx = stof(offset) * 10000;
	return utils::convert_4_byte(xx);
}

string c_conver::convert_unit(string& unit)
{
	return "\""+ unit +"\"";
}

void c_line_conver::print()
{
	string new_path = c_path::output_path + "linerConv.txt";
	FILE* fp = fopen(new_path.c_str(), "w");
	string tbuffer;
	for (u_int i = 0;i < m_vec.size();i++)
	{
		tbuffer = m_vec[i].get_buffer();
		fwrite(tbuffer.c_str(), 1, tbuffer.size(), fp);
	}
	fclose(fp);
}

c_conver::c_conver()
{
}

string c_conver::get_buffer()
{
	string buf = convert_head(m_parid) + convert_factor(m_factor) + convert_offset(m_offset)+"\n";
	return buf;
}

c_tab_conver::c_tab_conver()
{

}

void c_tab::insert(string key,string value)
{
	string tbuffer;
	if (!m_map.count(key))
	{
		m_map[key] = value;
		tbuffer = key + "\t" + value + "\n";
		fwrite(tbuffer.c_str(), 1, tbuffer.size(), fp_tab_conver);
	}
}

c_sfile_cmd::c_sfile_cmd()
{
}

void c_sfile_cmd::insert(string str)
{
	string buffer;
	if (!m_map.count(str))
	{
		int num = m_map.size() + 1;
		string num_str = utils::convert_4_byte(num);
		m_map[str] = num_str;
		buffer = num_str + "\t\"" + str + "\"\n";
		fwrite(buffer.c_str(), 1, buffer.size(), fp_sfile_cmd);
	}
}
