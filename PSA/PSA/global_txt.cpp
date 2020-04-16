#include"global_txt.h"
#include"utils.h"

map<string, string> c_txt::m_map;
map<int, string> c_txt::m_exp_map;

map<string, string> c_format::m_map;
map<int, string> c_format::m_exp_map;

map<string, string> c_state::m_map;


c_txt::c_txt()
{
}


string c_txt::convert_to_buffer(int num)
{
	string result = "0x17,0xFF,";
	result += utils::convert_4_byte(num);
	return result;
}


void c_txt::insert_map(string key_str)
{
	string xxx_str = c_du8::convert_du8_string(key_str);
	if (!m_map.count(xxx_str))
	{
		int num = m_map.size() + 1;
		string num_str = convert_to_buffer(num);
		m_map[xxx_str] = num_str;
		string total_str = num_str + "\"" + xxx_str + "\"" + "\n";
		m_exp_map[num] = total_str;
	}
}

string c_txt::get_str_from_map(string str)
{
	string xx = c_du8::convert_du8_string(str);
	return m_map[xx];
}

void c_txt::print(string path)
{
	FILE* fp = fopen(path.c_str(), "w");
	unsigned int i = 0;
	string tbuffer;
	for (i = 1;i < m_exp_map.size() + 1;i++)
	{
		tbuffer = m_exp_map[i];
		fwrite(tbuffer.c_str(), 1, tbuffer.size(), fp);
	}
	fclose(fp);
}


c_format::c_format()
{
}

string c_format::convert_to_buffer(int num)
{
	string head = "0x17,0xFE,";
	return head+utils::convert_4_byte(num);
}

void c_format::print(string path)
{
	FILE* fp = fopen(path.c_str(), "w");
	unsigned int i = 0;
	string tbuffer;
	for (i = 1;i < m_exp_map.size() + 1;i++)
	{
		tbuffer = m_exp_map[i];
		fwrite(tbuffer.c_str(), 1, tbuffer.size(), fp);
	}
}


void c_format::insert(string str)
{
	if (!m_map.count(str))
	{
		int num = m_map.size() + 1;
		string num_str = convert_to_buffer(num);
		m_map[str] = num_str;
		string total_str = num_str + "\"" + str + "\"" + "\n";
		m_exp_map[num] = total_str;
	}
}

string c_format::get_string_from_map(string str)
{
	string xx = c_du8::convert_du8_string(str);
	return m_map[xx];
}

c_conver_tab::c_conver_tab()
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

void c_conver_tab::print(string path)
{
	FILE* fp = fopen(path.c_str(), "w");
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

c_state::c_state()
{

}


void c_state::print(string path)
{
	FILE* fp = fopen(path.c_str(), "w");
	string tbuffer;
	for (map<string, string>::iterator it = m_map.begin(); it != m_map.end(); ++it)
	{
		tbuffer = it->first + "\t" + it->second;
		fwrite(tbuffer.c_str(), 1, tbuffer.size(), fp);
	}
	fclose(fp);
}

void c_state::insert(string key,string value)
{
	if (!m_map.count(key))
	{
		m_map[key] = value;
	}
}

string c_state::convert_to_buffer(int num)
{
	return utils::convert_4_byte(num);
}

