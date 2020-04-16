#include "basic_data.h"
#include"veh_basic.h"
#include"utils.h"
#include <io.h>
#include <stdio.h>
extern c_sql g_sqls;
extern string g_source_path;

c_txt g_txt;
c_format g_format;
c_state g_state;

c_veh::c_veh()
{

}

string c_veh::get_comtype(string mod_id)
{
	sql_info veh_comtype = g_sqls.sql_map[st_veh_comtype];
	vector<string> vec_str;
	vec_str.push_back(mod_id);
	string handled_sql = g_sqls.replace_parm(veh_comtype.sql, vec_str);
	c_fire_bird xx(veh_comtype.name, handled_sql);
	vector<map<string, data_unit>> result = xx.get_result_tab();
	string xstr = result[0]["ARCNAME"].data_value;
	return xstr;
}

string c_veh::get_veid(string veh_comtype)
{
	if (veh_comtype == "NULL")
	{
		return "";
	}
	sql_info veid = g_sqls.sql_map[st_veh_id];
	vector<string> vec_str;
	vec_str.push_back(veh_comtype);
	string handled_sql = g_sqls.replace_parm(veid.sql, vec_str);
	c_fire_bird xx(veid.name, handled_sql);
	vector<map<string, data_unit>> result = xx.get_result_tab();
	string xstr = result[0]["VEHID"].data_value;
	return xstr;
}


string c_veh::get_ecu_variant_head(char ecu_num,char var_num,short name_num)
{
	string buffer = utils::convert_2_byte(stoi(m_mod_id))+",";
	buffer += utils::convert_1_byte(ecu_num)+",";
	buffer += utils::convert_1_byte(var_num) + ",";
	buffer += utils::convert_2_byte(name_num) + "\t";
	return buffer;
}


string c_veh::get_descript(string xx)
{
	vector<string> result = utils::split(xx, ' ');
	string t_str = result.back();
	u_int i = 0;
	string xxx;
	for (i = 4;i < t_str.size();i++)
	{
		xxx.push_back(t_str[i]);
	}
	return xxx;
}

c_ecu_variant c_veh::get_one_ecu_variant(string str,string ecu_id)
{
	sql_info ecu_variant_list_all = g_sqls.sql_map[st_ecu_variant_list_all];
	vector<string> t_vec;
	t_vec.push_back("\'\'" + str + "\'\'");
	string handled_sql = g_sqls.replace_parm(ecu_variant_list_all.sql, t_vec);
	c_fire_bird* veh_result1 = new c_fire_bird(ecu_variant_list_all.name, handled_sql);
	vector<map<string, data_unit>> xx_result = veh_result1->get_result_tab();
	delete veh_result1;
	if (!xx_result.size())
	{
		return c_ecu_variant();
	}

	c_ecu_variant t_ecu_variant;
	t_ecu_variant.m_name = xx_result[0]["ECUNAME"].data_value;
	t_ecu_variant.m_variant_name = xx_result[0]["VARIANTENAME"].data_value;
	t_ecu_variant.m_ecu_id = ecu_id;
	t_ecu_variant.m_re_coframe = xx_result[0]["REQUESTRECOFRAME"].data_value;
	t_ecu_variant.m_cmd1 = xx_result[0]["REQUESTINITFRAME"].data_value;
	t_ecu_variant.m_cmd2 = xx_result[0]["REQUESTFINDIAGFRAME"].data_value;
	t_ecu_variant.m_IDEMNEMOVALUE = xx_result[0]["IDEMNEMOVALUE"].data_value;
	t_ecu_variant.m_pos = xx_result[0]["ISPBYTEPOS"].data_value;
	t_ecu_variant.m_length = xx_result[0]["ISPBYTELENGTH"].data_value;

	sql_info ecu_descript = g_sqls.sql_map[st_ecu_descript_info];
	vector<string> vec_str2;
	vec_str2.push_back(t_ecu_variant.m_ecu_id);
	vec_str2.push_back(t_ecu_variant.m_variant_name);
	string handled_sql2 = g_sqls.replace_parm(ecu_descript.sql, vec_str2);
	c_fire_bird* veh_result2 = new c_fire_bird(ecu_descript.name, handled_sql2);
	vector<map<string, data_unit>> descript_result = veh_result2->get_result_tab();
	if (descript_result.size())
	{
		if (descript_result.size() > 1)
		{
			int cc = 0;
		}
		t_ecu_variant.m_variant_descript = get_descript(descript_result[0]["RECDESCRIPT"].data_value);
	}
	else
	{
		t_ecu_variant.m_variant_descript = "NULL";
	}
	delete veh_result2;

	return t_ecu_variant;
}

vector<variant_stru> c_veh::get_variants_can_be_searched()
{
	vector<variant_stru> tvec;
	int ecu_num = 0;
	for (u_int i = 0;i < m_vec_ecu.size();i++)
	{
		vector<string>vec_ecu_id;
		vector<string>vec_variant_name;
		m_vec_ecu[i].get_ecu_variants(vec_variant_name, vec_ecu_id);
		if (m_vec_ecu[i].m_fam_name == "BMF" || m_vec_ecu[i].m_fam_name == "MDS")
		{
			continue;
		}
		ecu_num++;
		for (u_int j = 0;j < vec_variant_name.size();j++)
		{
			c_ecu_variant t_ecu_variant = get_one_ecu_variant(vec_variant_name[j], vec_ecu_id[j]);
			if (t_ecu_variant.m_variant_descript != "")
			{
				variant_stru  tt;
				tt.ecu_variant = t_ecu_variant;
				tt.file_name = m_vec_ecu[i].m_file_name;
				tt.file_path = m_vec_ecu[i].m_file_path;
				tt.ecu_num = ecu_num;
				tvec.push_back(tt);
			}
		}
	}
	return tvec;
}


string c_veh::get_ecu_quality_buffer(variant_stru& var_stru, int var_num, int name_num)
{
	c_current_veh t_current_veh;
	string tbuffer;
	tbuffer = get_ecu_variant_head(var_stru.ecu_num, var_num, name_num);
	t_current_veh.m_ecu_type = var_stru.ecu_variant.m_name;
	t_current_veh.m_ecu_descript = var_stru.ecu_variant.m_variant_descript;
	t_current_veh.m_comtype = m_veh_comtype;
	t_current_veh.m_moudle_name = m_mod_name;
	tbuffer += var_stru.ecu_variant.get_ecu_variant_content(var_stru.file_name, var_stru.file_path, t_current_veh) + "\n";
	return tbuffer;
}

void c_veh::load_ecu_data()
{
	sql_info ecu_info = g_sqls.sql_map[st_ecu_info];
	vector<string> vec_str;
	vec_str.push_back(m_mod_name);
	string handled_sql = g_sqls.replace_parm(ecu_info.sql, vec_str);
	c_fire_bird veh_result(ecu_info.name, handled_sql);
	vector<map<string, data_unit>> result = veh_result.get_result_tab();


	c_ecu t_ecu;
	for (u_int i = 0;i < result.size();i++)
	{
		t_ecu.m_fam_name = result[i]["FAMILYNAME"].data_value;
		t_ecu.m_fam_label = result[i]["FAMILYLABEL"].data_value;
		t_ecu.m_file_name = result[i]["FILENAME"].data_value;
		t_ecu.m_file_path = result[i]["PATH"].data_value;
		t_ecu.get_protcol_info(m_ve_id);
		m_vec_ecu.push_back(t_ecu);//
	}
}

void c_veh::print_ecus_txt(string path)
{
	string file_name = "ecu_" + m_mod_id + ".txt";
	string xpath = path + file_name;
	FILE* fp = fopen(xpath.c_str(), "w");
	string head = "0x17,0x00,";
	u_int i = 0;
	string tbuffer;
	vector<string>vec_temp;

	for (i = 0;i < m_vec_ecu.size();i++)
	{
		tbuffer = head + utils::convert_4_byte(i + 1) + m_vec_ecu[i].get_one_ecu_text(stoi(m_mod_id), i + 1);
		tbuffer += "\n";
		fwrite(tbuffer.c_str(), 1, tbuffer.size(), fp);
	}
	fclose(fp);
}

void c_veh::print_ecus_quality_txt(string path)
{
	string file_name = "quality_" + m_mod_id + ".txt";
	string xpath = path + file_name;
	FILE* fp = fopen(xpath.c_str(), "w");

	vector<variant_stru> tvec = get_variants_can_be_searched();

	int pre_ecu_num = 1;
	int  var_num = 1;
	int  name_num = 1;
	c_ecu_variant pre_ecu_variant;

	for (u_int i = 0;i < tvec.size();i++)
	{
		//if (tvec[i].ecu_variant.m_name == "BOITIER_TELEMATIQUE")
		{
			if (i == 0)
			{
				pre_ecu_variant = tvec[i].ecu_variant;
				pre_ecu_num = tvec[0].ecu_num;
			}
			if (!strcmp(tvec[i].ecu_variant.m_name.c_str(), pre_ecu_variant.m_name.c_str()))
			{
				if (i != 0)
				{
					name_num++;
				}
			}
			else
			{
				//增加default 菜单
				name_num = 1;
				var_num++;
			}
			if (tvec[i].ecu_num != pre_ecu_num)
			{
				var_num = 1;
			}
			string tbuffer = get_ecu_quality_buffer(tvec[i], var_num, name_num);
			fwrite(tbuffer.c_str(), 1, tbuffer.size(), fp);

			pre_ecu_variant = tvec[i].ecu_variant;
			pre_ecu_num = tvec[i].ecu_num;
		}
	}
	fclose(fp);

}

void c_veh::print_ecus_quality_check(string path)
{
	string file_name = "quality_" + m_mod_id + ".txt";
	string xpath = path + file_name;
	FILE* fp = fopen(xpath.c_str(), "w");

	vector<variant_stru> tvec = get_variants_can_be_searched();

	int pre_ecu_num = 1;
	int  var_num = 1;
	int  name_num = 1;
	c_ecu_variant pre_ecu_variant;

	for (u_int i = 0;i < tvec.size();i++)
	{
		//if (tvec[i].ecu_variant.m_name == "BOITIER_TELEMATIQUE")
		{
			if (i == 0)
			{
				pre_ecu_variant = tvec[i].ecu_variant;
				pre_ecu_num = tvec[0].ecu_num;
			}
			if (!strcmp(tvec[i].ecu_variant.m_name.c_str(), pre_ecu_variant.m_name.c_str()))
			{
				if (i != 0)
				{
					name_num++;
				}
			}
			else
			{
				//增加default 菜单
				name_num = 1;
				var_num++;
			}
			if (tvec[i].ecu_num != pre_ecu_num)
			{
				var_num = 1;
			}
			string tbuffer = get_ecu_quality_buffer(tvec[i], var_num, name_num);
			fwrite(tbuffer.c_str(), 1, tbuffer.size(), fp);

			pre_ecu_variant = tvec[i].ecu_variant;
			pre_ecu_num = tvec[i].ecu_num;
		}
	}
	fclose(fp);

}

c_all_veh::c_all_veh()
{
	sql_info veh_sql = g_sqls.sql_map[st_veh_info];
	c_fire_bird veh_result(veh_sql.name, veh_sql.sql);
	vector<map<string, data_unit>> result = veh_result.get_result_tab();

	c_veh t_veh;
	for (u_int i = 0;i < result.size();i++)
	{
		t_veh.m_veh_name = result[i]["VEHNAME"].data_value;
		t_veh.m_mod_id = result[i]["MODID"].data_value;
		t_veh.m_mod_name = result[i]["MODNAME"].data_value;
		t_veh.m_veh_label = result[i]["MODTHESAU"].data_value;
		t_veh.m_veh_comtype = t_veh.get_comtype(t_veh.m_mod_id);
		t_veh.m_ve_id = t_veh.get_veid(t_veh.m_veh_comtype);
		m_vec_veh.push_back(t_veh);
	}
}


void c_all_veh::print_ecus_data(string path)
{
	u_int i = 0;
	for (i = 0;i < m_vec_veh.size();i++)
	{
		if (m_vec_veh[i].m_mod_name == "T73_Chine")
		{
			m_vec_veh[i].load_ecu_data();
			m_vec_veh[i].print_ecus_txt();
			//m_vec_veh[i].print_ecus_quality_check();
			m_vec_veh[i].print_ecus_quality_txt();
		}

	}
}



void c_all_veh::print_veh_menu(string path)
{
	string xpath = path + "veh_menu.txt";
	FILE* f_veh_menu = fopen(xpath.c_str(), "w");;

	u_int i = 0;
	int menu_num = 1;
	int menu_sub_num = 1;
	string tbuffer;

	c_veh pre_veh = m_vec_veh[0];
	g_txt.insert_map(m_vec_veh[0].m_veh_name);
	g_txt.insert_map(m_vec_veh[0].m_veh_label);
	g_txt.insert_map(m_vec_veh[0].m_mod_name);
	tbuffer = utils::get_menu_id_buffer(menu_num, menu_sub_num);
	tbuffer += g_txt.get_str_from_map(m_vec_veh[0].m_veh_name);
	tbuffer += g_txt.get_str_from_map(m_vec_veh[0].m_veh_label);
	tbuffer += g_txt.get_str_from_map(m_vec_veh[0].m_mod_name);

	char buff[256]{};
	int mod_id = stoi(m_vec_veh[0].m_mod_id);
	int high = mod_id / 256;
	int low = mod_id % 256;
	sprintf(buff, "0x%02X,0x%02X\t", high, low);
	
	tbuffer += buff;
	tbuffer += "\n";
	fwrite(tbuffer.c_str(), 1, tbuffer.size(), f_veh_menu);
	
	for (i = 1;i < m_vec_veh.size();i++)
	{
		g_txt.insert_map(m_vec_veh[i].m_veh_name);
		g_txt.insert_map(m_vec_veh[i].m_veh_label);
		g_txt.insert_map(m_vec_veh[i].m_mod_name);

		if (m_vec_veh[i].m_veh_name != pre_veh.m_veh_name)
		{
			menu_sub_num = 1;
			menu_num++;
		}
		else
		{
			menu_sub_num++;
		}

		tbuffer = utils::get_menu_id_buffer(menu_num, menu_sub_num);
		tbuffer += g_txt.get_str_from_map(m_vec_veh[i].m_veh_name);
		tbuffer += g_txt.get_str_from_map(m_vec_veh[i].m_veh_label);
		tbuffer += g_txt.get_str_from_map(m_vec_veh[i].m_mod_name);

		char buff[256]{};
		int mod_id = stoi(m_vec_veh[i].m_mod_id);
		int high = mod_id / 256;
		int low = mod_id % 256;
		sprintf(buff, "0x%02X,0x%02X\t", high, low);

		tbuffer += buff;

		tbuffer += "\n";
		fwrite(tbuffer.c_str(), 1, tbuffer.size(), f_veh_menu);
		
		pre_veh = m_vec_veh[i];
	}
}

c_ecu::c_ecu()
{
}

void c_ecu::get_protcol_info(string veid)
{
	sql_info protocol = g_sqls.sql_map[st_ecu_protcol];

	vector<string> vec_str;
	vec_str.push_back(veid);
	vec_str.push_back(m_fam_name);
	string handled_sql = g_sqls.replace_parm(protocol.sql, vec_str);
	c_fire_bird veh_result(protocol.name, handled_sql);
	vector<map<string, data_unit>> result = veh_result.get_result_tab();
	u_int i = 0;

	string famcanbus = result[0]["FAMCANBUS"].data_value;
	if (famcanbus == "LIGNE_K") 
	{
		m_protocal_type = LIGNE_K;
	}
	if (famcanbus == "CAN_IS")
	{
		m_protocal_type = CAN_IS;
	}
	if (famcanbus == "CAN_CAR")
	{
		m_protocal_type = CAN_CAR;

	}
	if (famcanbus == "CAN_CONF")
	{
		m_protocal_type = CAN_CONF;
	}
	if (famcanbus == "CAN_DIAG")
	{
		m_protocal_type = CAN_DIAG;
	}
	if (famcanbus == "LIN_BSI_2")
	{
		m_protocal_type = LIN_BSI_2;
	}
	if (famcanbus == "CAN_INFO_DIV")
	{
		m_protocal_type = CAN_INFO_DIV;
	}
	if (famcanbus == "LIN_BSI_1")
	{
		m_protocal_type = LIN_BSI_1;
	}
	if (famcanbus == "LIN_BSM")
	{
		m_protocal_type = LIN_BSM;
	}

	m_famid_request = result[0]["FAMIDCANE"].data_value;
	m_famid_accept = result[0]["FAMIDCANR"].data_value;
	m_fam_id= result[0]["FAMID"].data_value;
}

string c_ecu::get_one_ecu_text(int mod_id,int cnt)
{
	string tbuffer;
	g_txt.insert_map(m_fam_name);
	g_txt.insert_map(m_fam_label);
	
	tbuffer += g_txt.get_str_from_map(m_fam_name);
	tbuffer += g_txt.get_str_from_map(m_fam_label);

	tbuffer += utils::convert_1_byte(m_protocal_type) + "\t";
	tbuffer += utils::convert_2_byte(stoi(m_famid_accept, 0, 16)) + "\t";
	tbuffer += utils::convert_2_byte(stoi(m_famid_request,0,16)) + "\t";
	tbuffer += utils::convert_3_byte(short(mod_id), char(cnt));
	
	return tbuffer;
	
}

void c_ecu::get_ecu_variants(vector<string>&vec_variant_name, vector<string>&vec_ecu_id)
{
	sql_info st_ecu_variant = g_sqls.sql_map[st_ecu_variant_list];

	vector<string> vec_str;
	vec_str.push_back(m_fam_id);
	string handled_sql = g_sqls.replace_parm(st_ecu_variant.sql, vec_str);
	c_fire_bird* veh_result=new c_fire_bird(st_ecu_variant.name, handled_sql);
	vector<map<string, data_unit>> result = veh_result->get_result_tab();
	delete veh_result;
	u_int i = 0;
	for (i = 0;i < result.size();i++)
	{
		vec_variant_name.push_back(result[i]["VARIANTES"].data_value);
		vec_ecu_id.push_back(result[i]["ECUID"].data_value);
	}
}

c_ecu_variant::c_ecu_variant()
{
}

string c_ecu_variant::get_ecu_variant_content(string file_name, string file_path, c_current_veh& current_veh)
{
	string result;
	result = "\""+m_name + "," + m_variant_name + "," + m_re_coframe + "," + m_cmd1 + "," + m_cmd2 + "," + m_IDEMNEMOVALUE + "," + m_pos + "," + m_length+",";

	c_tree t_tree;
	string new_file_name;
	string new_file_path;
	if (file_name.substr(0, 4) == "MENU")
	{
		new_file_name = file_name;
		new_file_path = file_path;
	}
	else
	{
		new_file_path = file_path + "\\" + m_name;
		new_file_name = get_menu_file_name(new_file_path);
	}
	t_tree = c_tree(new_file_path, new_file_name, current_veh);


	vector<menu_info> menu_func_result;
	t_tree.quick_scan(menu_func_result);
	string xx;
	for (u_int i = 0;i < menu_func_result.size();i++)
	{
		xx += menu_func_result[i].m_file_name + ",";
	}
	result += xx;
	return result;
}

string c_ecu_variant::get_menu_file_name(string file_path)
{
	string path =  g_source_path+"\\" + file_path + "\\*.*";

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(path.c_str(), &findData);
	if (handle == -1)        // 检查是否成功
		return string();

	do
	{
		if (findData.attrib & _A_SUBDIR)
		{
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;

			// 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
		}
		else
		{
			string tfile_name=findData.name;
			string check_value;
			for (u_int i = 0;i < 5;i++)
			{
				check_value.push_back(tfile_name[i]);
			}
			if (check_value == "MENU_")
			{
				return string(findData.name);
			}
		}
	} while (_findnext(handle, &findData) == 0);

	_findclose(handle);    // 关闭搜索句柄
	
	return string();
}

void c_ecu_variant::load_qulity_config(vector<menu_info1> vec_menu)
{
	for (u_int i = 0;i < vec_menu.size();i++)
	{
		if (vec_menu[i].m_file_name == "IDENTIFICATION_GEN.s")
		{
			m_identification.load_data(m_ecu_id, "IDENTIFICATION_GEN.s");
		}
	}
}

c_data_stream_group::c_data_stream_group()
{

}

void c_data_stream_group::load_data(string ecu_id, string group_name)
{
	init_name_and_scrid(ecu_id, group_name);
	init_vec_data_stream();
}

void c_data_stream_group::init_vec_data_stream()
{
	sql_info xx = g_sqls.sql_map[st_id_data_stream];
	vector<string> vec_str;
	vec_str.push_back(m_scrid);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_id_data_stream, vec_str);
	
	c_data_stream tt;
	for (u_int i = 0;i < result.size();i++)
	{
		if (result[i]["PARIDCONF"].data_value == "1")
		{
			continue;
		}

		if (result[i]["PARFORMATTYPE"].data_value=="CHAR")
		{
			tt.m_type = type_CHAR;
		}
		else
		{
			if (result[i]["PARDISC"].data_value == "1")
			{
				tt.m_type = type_state;
			}
			else
			{
				tt.m_type = type_no_state;
			}
		}
		tt.m_scr_id = m_scrid;
		tt.m_par_id = result[i]["PARID"].data_value;
		tt.m_name = result[i]["PARLIBMP"].data_value;
		tt.m_par_name= result[i]["PARNAME"].data_value;
		tt.m_par_disc = result[i]["PARDISC"].data_value;
		tt.m_par_libuint = result[i]["PARLIBUNIT"].data_value;
		tt.m_par_detail_string = result[i]["PARAIDE"].data_value;
		tt.m_format_type = result[i]["PARFORMATTYPE"].data_value;
		tt.m_format_match = result[i]["PARFORMATMATCH"].data_value;
		tt.m_format_repalce = result[i]["PARFORMATREPLACE"].data_value;
		tt.m_ser_unid = tt.get_stream_srunid(tt.m_par_name,m_ecu_id);
		tt.m_request_id = tt.get_req_id(tt.m_ser_unid);
		tt.m_receive_id = tt.get_rec_id(tt.m_ser_unid);
		tt.m_request_cmd = tt.get_request_cmd(tt.m_request_id);
		if (tt.m_type == type_state)
		{
			tt.m_vec_disc = tt.get_vec_disc(m_scrid, tt.m_par_id);
		}

		tt.m_receive_info.init_receive_info(tt.m_receive_id,tt.m_par_name, tt.m_type);


		m_vec_data_stream.push_back(tt);
	}
}

void c_data_stream_group::init_name_and_scrid(string ecu_id, string group_name)
{
	vector<string> vec_str;
	vec_str.push_back(ecu_id);
	vec_str.push_back(group_name);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_ecu_identification, vec_str);
	m_scrid = result[0]["SCRID"].data_value;
	m_name = result[0]["SCRTITRE"].data_value;
	m_ecu_id= result[0]["ECUID"].data_value;
}

void c_data_stream_group::print(string path)
{
	
	//string new_path = path + g_txt.convert_du8_string(m_name) + ".txt";
	FILE* fp = fopen(path.c_str(), "w");
	string tbuffer;
	for (u_int i = 0;i < m_vec_data_stream.size();i++)
	{
		tbuffer = m_vec_data_stream[i].get_output_buffer();
		fwrite(tbuffer.c_str(), 1, tbuffer.size(), fp);
	}
	fclose(fp);
}


string c_data_stream::get_stream_srunid(string par_name,string ecu_id)
{
	vector<string> vec_str;
	vec_str.push_back(ecu_id);
	vec_str.push_back(par_name);
	vector<map<string, data_unit>> result=fire_bird::get_sql_result(st_data_stream_serun_id, vec_str);
	return result[0]["SERUNID"].data_value;
}




c_data_stream::c_data_stream()
{
}

vector<c_disc> c_data_stream::get_vec_disc(string scr_id, string par_id)
{
	vector<string> vec_str;
	vec_str.push_back(scr_id);
	vec_str.push_back(par_id);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_data_stream_state_disc, vec_str);
	c_disc tt;
	vector<c_disc> vec;
	for (u_int i = 0;i < result.size();i++)
	{
		tt.m_disc_id = result[i]["DISCID"].data_value;
		tt.m_dic_name = result[i]["DISCNAME"].data_value;
		tt.m_disc_val_lib = result[i]["DISCVALLIB"].data_value;
		tt.m_disc_val_com = result[i]["DISCVALCOM"].data_value;
		tt.m_disc_default = result[i]["DISCDEFAULT"].data_value;
		vec.push_back(tt);
	}
	return vec;

}

string c_data_stream::get_req_id(string ser_unid)
{
	vector<string> vec_str;
	vec_str.push_back(ser_unid);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_data_stream_serunfr_id, vec_str);
	return result[0]["SERUNFRID"].data_value;
	
}

string c_data_stream::get_rec_id(string ser_unid)
{
	vector<string> vec_str;
	vec_str.push_back(ser_unid);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_data_stream_serunfr_id, vec_str);
	return result[1]["SERUNFRID"].data_value;
}



string c_data_stream::get_request_cmd(string ser_unfrid)
{
	vector<string> vec_str;
	vec_str.push_back(ser_unfrid);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_data_stream_request_info, vec_str);
	
	u_int size = result.size();
	string xx;
	for (u_int i = 0;i < size;i++)
	{
		if (result[i]["ISPVALUE"].data_value != "")
		{
			xx += "0x" + result[i]["ISPVALUE"].data_value;
			if (i + 1 != size)
			{
				xx += ",";
			}
		}
	}
	return xx;
}

string c_data_stream::get_output_buffer()
{
	string tbuffer;
	tbuffer = get_head_buffer();
	tbuffer += get_name_buffer();
	tbuffer += get_xxx_buffer();
	tbuffer += get_tail_buffer();
	tbuffer += "\n";
	return tbuffer;
}


string c_data_stream::get_head_buffer()
{
	return "0x00,0x00,0x00,0x00\t";
}

string c_data_stream::get_name_buffer()
{
	string tbuffer;
	g_txt.insert_map(m_name);
	tbuffer = g_txt.get_str_from_map(m_name);
	return tbuffer;
}

string c_data_stream::get_xxx_buffer()
{
	string tbuffer;
	if (m_type == type_CHAR)
	{
		string xx ="\"" + m_format_match + "\",\"" + m_format_repalce + "\"";
		g_format.insert(xx);
		tbuffer = string("0xFF,") + g_format.get_string_from_map(xx);
	}
	else if (m_type == type_state)
	{
		string state_head= utils::convert_3_byte_1(stoi(m_scr_id)) + "," + utils::convert_3_byte_1(stoi(m_par_id));
		g_state.insert(state_head, get_vec_disc_buffer());
		tbuffer = string("0xFE,") + state_head+"\t";
	}
	else if (m_type == type_no_state)
	{
		tbuffer = string("0xFD,")+utils::convert_4_byte(stoi(m_par_id))+"\t";
	}

	return tbuffer;
}

string c_data_stream::get_tail_buffer()
{
	string tbuffer;
	tbuffer += m_request_cmd + "\t";
	tbuffer += m_receive_info.m_pos + "\t";
	tbuffer += m_receive_info.m_length;
	return tbuffer;
}

string c_data_stream::get_vec_disc_buffer()
{
	string tbuffer;
	for (u_int i = 0;i < m_vec_disc.size();i++)
	{
		tbuffer += utils::convert_2_byte_1(stoi(m_vec_disc[i].m_disc_val_com));
		g_txt.insert_map(m_vec_disc[i].m_disc_val_lib);
		tbuffer += g_txt.get_str_from_map(m_vec_disc[i].m_disc_val_lib) + "\t";
	}
	tbuffer += "\n";
	return tbuffer;
}

c_dtc::c_dtc()
{
}

c_receive_info::c_receive_info()
{
}

void c_receive_info::init_receive_info(string ser_unfrid,string par_name,stream_type stream_type)
{
	vector<string> vec_str;
	vec_str.push_back(ser_unfrid);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_data_stream_receive_info, vec_str);

	u_int size = result.size();
	for (u_int i = 0;i < size;i++)
	{
		if (result[i]["PARSNAME"].data_value == par_name)
		{
			m_pos = result[i]["ISPBYTEPOS"].data_value;
			m_length = result[i]["ADDBYTELENGTH"].data_value;
			m_order = result[i]["ADDBYTEORDER"].data_value;
			m_par_id = result[i]["PARID"].data_value;
			m_par_encoding = result[i]["PARENCODING"].data_value;
			m_algrithm = get_algorithm();
		}
	}
}

c_algorithm c_receive_info::get_algorithm()
{
	vector<string> vec_str;
	vec_str.push_back(m_par_id);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_data_stream_algorithm, vec_str);
	


	c_algorithm xx;
	if (result.size())
	{
		xx.m_confactor = result[0]["CONFACTOR"].data_value;
		xx.m_con_unit = result[0]["CONUNIT"].data_value;
		xx.m_offset = result[0]["CONOFFSET"].data_value;
	}
	return xx;
}


c_disc::c_disc()
{

}

c_algorithm::c_algorithm()
{
}

