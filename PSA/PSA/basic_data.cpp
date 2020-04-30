#include "basic_data.h"
extern c_sql g_sqls;

c_dtc c_ecu_variant::m_dtc;

c_veh::c_veh()
{

}

c_veh::c_veh(string mod_name,string mod_id, string reserch_type)
{

	m_mod_name = mod_name;
	m_mod_id = mod_id;
	vector<string> vec_parm;
	vector<map<string, data_unit>> result;
	//查询 COMTYPE
	vec_parm.push_back(mod_id);
	result = fire_bird::get_sql_result(st_veh_comtype, vec_parm);
	vector<string> vec_com_type;
	for (size_t i = 0; i < result.size(); i++)
	{
		int flag = 1;
		for (size_t j = 0; j < vec_com_type.size(); j++)
		{
			if (!strcmp(result[i]["ARCNAME"].data_value.c_str(), vec_com_type[j].c_str()))
			{
				flag=0;
				break;
			}
		}
		if (flag)
		{
			vec_com_type.push_back(result[i]["ARCNAME"].data_value);
		}
	}
	if (!vec_com_type.size())
	{
		c_log::print("c_veh::mod_name: %s mod_id: %s 没有通讯类型\n", mod_name.c_str(), mod_id.c_str());
		return;
	}
	else if(vec_com_type.size()>1)
	{
		c_log::print("c_veh::mod_name: %s mod_id: %s 有多余1种comtype: %s ,%s ...\n", mod_name.c_str(), mod_id.c_str(), vec_com_type[0].c_str(),vec_com_type[1].c_str());
	}
	else
	{
		m_comtype = result[0]["ARCNAME"].data_value;
	}
	

	//cha veid
	vec_parm.clear();
	vec_parm.push_back(m_comtype);
	result = fire_bird::get_sql_result(st_veh_id, vec_parm);
	if (!result.size())
	{
		c_log::print("c_veh::search VEHID failed ,mod_name: % s mod_id : % s\n", mod_name.c_str(), mod_id.c_str());
		return;
	}
	else
	{
		m_ve_id = result[0]["VEHID"].data_value;
	}
	
	//获取vec—— ecu信息
	vec_parm.clear();
	vec_parm.push_back(mod_name);
	vec_parm.push_back(reserch_type);
	result = fire_bird::get_sql_result(st_ecu_info, vec_parm);
	c_ecu t_ecu;
	for (size_t i = 0; i < result.size(); i++)
	{
		if (result[i]["FAMILYNAME"].data_value == "BMF" ||
			result[i]["FAMILYNAME"].data_value == "MDS"
			//||result[i]["FAMILYNAME"].data_value == "INJ"
			)
		{
			continue;
		}
		m_cur_veh.m_comtype = m_comtype;
		m_cur_veh.m_moudle_name = m_mod_name;
		m_cur_veh.m_file_name = result[i]["FILENAME"].data_value;
		m_cur_veh.m_file_path = result[i]["PATH"].data_value;
		t_ecu = c_ecu(result[i]["FAMILYNAME"].data_value, result[i]["FAMILYLABEL"].data_value , m_ve_id, m_cur_veh);
		m_vec_ecu.push_back(t_ecu);
		//break;//
	}
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


void c_veh::print_ecus_txt()
{
	string file_name = "ecu_" + m_mod_id + ".txt";
	string xpath = c_path::output_path+ file_name;
	FILE* fp = fopen(xpath.c_str(), "w");
	string head = "0x17," + utils::convert_2_byte(stoi(m_mod_id));
	string tbuffer;
	vector<string>vec_temp;

	for (size_t i = 0;i < m_vec_ecu.size();i++)
	{
		tbuffer = head + "," + utils::convert_4_byte(i + 1) + "\t" + m_vec_ecu[i].m_buffer + "\t" + utils::convert_2_byte(stoi(m_mod_id)) + "," + utils::convert_1_byte(int(i + 1));
		tbuffer += "\n";
		fwrite(tbuffer.c_str(), 1, tbuffer.size(), fp);
	}
	fclose(fp);
}

void c_veh::print_quality_txt()
{
	string file_name = "quality_" + m_mod_id + ".txt";
	string xpath = c_path::output_path + file_name;
	FILE* fp = fopen(xpath.c_str(), "w");


	string head = utils::convert_2_byte(stoi(m_mod_id));
	string ecu_num;
	string group_num;
	string var_num;
	string tbuffer;

	for (size_t i = 0; i < m_vec_ecu.size(); i++)
	{
		ecu_num = utils::convert_1_byte(i + 1);
		for (size_t j = 0; j < m_vec_ecu[i].m_vec_variant_group.size(); j++)
		{
			group_num = utils::convert_1_byte(j + 1);
			m_vec_ecu[i].m_vec_variant_group[j].print();
			for (size_t k = 0; k < m_vec_ecu[i].m_vec_variant_group[j].m_vec_variant.size(); k++)
			{
				var_num = utils::convert_1_byte(k + 1);
				tbuffer = head + "," + ecu_num + "," + group_num + "," + var_num + "\t" + m_vec_ecu[i].m_vec_variant_group[j].m_vec_variant[k].m_buffer + "\n";
				fwrite(tbuffer.c_str(), 1, tbuffer.size(), fp);
			}
		}
	}
	fclose(fp);
}



c_ecu::c_ecu()
{
}

c_ecu::c_ecu(string name, string label, string ve_id,c_current_veh& cur_veh)
{
	m_fam_name = name;
	m_fam_label = label;
	m_file_name = cur_veh.m_file_name;
	m_file_path = cur_veh.m_file_path;
	
	vector<string>vec_parm;
	vec_parm.push_back(ve_id);
	vec_parm.push_back(m_fam_name);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_ecu_protcol, vec_parm);
	if (!result.size())
	{
		c_log::print("c_ecu::search FAMID failed ,fam_name:%s  \n", m_fam_name.c_str(), m_fam_label.c_str());
	}
	m_fam_id = result[0]["FAMID"].data_value;
	m_famid_request = result[0]["FAMIDCANR"].data_value;
	m_famid_accept = result[0]["FAMIDCANE"].data_value;
	m_protocal_type = utils::get_protocol(result[0]["FAMCANBUS"].data_value);
	m_buffer = get_buffer();


	vec_parm.clear();
	vec_parm.push_back(m_fam_id);
	result = fire_bird::get_sql_result(st_ecu_variant_list, vec_parm);
	if (!result.size())
	{
		c_log::print("c_ecu::search var_list failed ,fam_id %s \n", m_fam_id.c_str());
		return;
	}
	vector<string> vec_str;
	string pre_str;
	string tem_str;
	for (size_t i = 0; i < result.size(); i++)
	{
		tem_str = result[i]["ECUNAME"].data_value;
		if (i == 0)
		{
			pre_str = result[i]["ECUNAME"].data_value;
			vec_str.push_back(result[i]["VARIANTES"].data_value);
		}
		else
		{
			if (!strcmp(tem_str.c_str(),pre_str.c_str()))
			{
				vec_str.push_back(result[i]["VARIANTES"].data_value);
			}
			else
			{
				cur_veh.m_ecu_type = result[i-1]["ECUNAME"].data_value;
				c_ecu_variant_group t_var_group = c_ecu_variant_group(vec_str, result[i - 1]["ECUID"].data_value, cur_veh);
				m_vec_variant_group.push_back(t_var_group);
				vec_str.clear();
			}
		}
		pre_str = tem_str;
	}
}

string c_ecu::get_buffer()
{
	string buffer;
	c_txt::insert_map(m_fam_name);
	c_txt::insert_map(m_fam_label);
	buffer += c_txt::get_str_from_map(m_fam_name) + "\t";
	buffer += c_txt::get_str_from_map(m_fam_label) + "\t";
	buffer += utils::convert_1_byte(m_protocal_type) + "\t";
	buffer += utils::convert_2_byte(stoi(m_famid_request,0,16)) + "\t";
	buffer += utils::convert_2_byte(stoi(m_famid_accept, 0, 16));
	return buffer;
}


c_ecu_variant::c_ecu_variant()
{
}

c_ecu_variant::c_ecu_variant(string str_var, string ecu_id, map<string, string>& m_func_map, c_current_veh& cur_veh)
{
	is_null = 0;
	vector<string>vec_parm;
	vec_parm.push_back(str_var);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_ecu_variant_list_all, vec_parm);
	if (!result.size())
	{
		is_null = 1;
		return;
	}
	m_name = result[0]["ECUNAME"].data_value;
	m_variant_name = result[0]["VARIANTENAME"].data_value;
	m_ecu_id = ecu_id;
	m_veid = result[0]["ECUVEID"].data_value;
	m_re_coframe = result[0]["REQUESTRECOFRAME"].data_value;
	m_cmd1 = result[0]["REQUESTINITFRAME"].data_value;
	m_cmd2 = result[0]["REQUESTFINDIAGFRAME"].data_value;
	m_IDEMNEMOVALUE = result[0]["IDEMNEMOVALUE"].data_value;
	m_pos = result[0]["ISPBYTEPOS"].data_value;
	m_length = result[0]["ISPBYTELENGTH"].data_value;
	
	vec_parm.clear();
	vec_parm.push_back(m_ecu_id);
	vec_parm.push_back(str_var);
	result= fire_bird::get_sql_result(st_ecu_descript_info, vec_parm);
	if (!result.size())
	{
		c_log::print("c_ecu_variant::search RECDESCRIPT info failed, %s  %s \n", ecu_id.c_str(), str_var.c_str());
		return;
	}
	if (result.size() > 1)
	{
		c_log::print("c_ecu_variant::search RECDESCRIPT not only one, %s  %s \n", ecu_id.c_str(), str_var.c_str());
	}
	m_variant_descript = utils::get_descript(result[0]["RECDESCRIPT"].data_value);
	cur_veh.m_ecu_descript = m_variant_descript;


	m_buffer = "\"" + cur_veh.m_moudle_name+","+ m_name + ","+m_ecu_id+","+ m_variant_name +"," + m_variant_descript +","+ m_re_coframe + "," + m_cmd1 + "," + m_cmd2 + "," + m_IDEMNEMOVALUE + "," + m_pos + "," + m_length + ",";
	vector<menu_info>menu_func_result = get_func_file(cur_veh);
	get_global_buffer(cur_veh);//获取全局的global buffer 到veh.map
	string new_file_name;
	for (size_t i = 0; i < menu_func_result.size(); i++)
	{
		new_file_name = menu_func_result[i].m_file_name;
		new_file_name = new_file_name.substr(0, new_file_name.size()-2);
		int len = new_file_name.size();
		file_type ft = utils::get_func_file_type(new_file_name);
		if (ft == ft_ident)
		{
			new_file_name = m_ecu_id + "_" + new_file_name ;
			if (!m_func_map.count(new_file_name))
			{
				vector<string> vec_scrname;
				c_tree t_tree(menu_func_result[i].m_file_path, menu_func_result[i].m_file_name, cur_veh);
				c_cel t_cel;
				t_tree.get_vec_scrname(vec_scrname, t_cel);
				string all_buffer;
				for (size_t i = 0; i < vec_scrname.size(); i++)
				{
					m_ident_group = c_data_stream_group(i+1,m_ecu_id,m_veid, vec_scrname[i], new_file_name);
					all_buffer += m_ident_group.m_buffer;
				}
				m_func_map[new_file_name] = all_buffer;
			}
			m_ident_name = new_file_name;
		}
		if (ft == ft_standard)
		{
			if (!m_func_map.count(new_file_name))
			{
				vector<string> vec_scrname;
				if (menu_func_result[i].m_file_name=="T7_HDI_SID807_BR2_mesures_parametres_standard_312.s")
				{
					int cc = 0;
				}
				c_tree t_tree(menu_func_result[i].m_file_path, menu_func_result[i].m_file_name, cur_veh);
				c_cel t_cel;
				t_tree.get_vec_scrname(vec_scrname, t_cel);
				string all_buffer;
				for (size_t i = 0; i < vec_scrname.size(); i++)
				{
					m_standard_group = c_data_stream_group(i+1, m_ecu_id,m_veid, vec_scrname[i], new_file_name);
					all_buffer += m_standard_group.m_buffer;
				}
				m_func_map[new_file_name] = all_buffer;
			}
			m_standard_name = new_file_name;
		}
		if (ft == ft_defauts)
		{
			new_file_name = ecu_id + "_dtc";
			if (!m_func_map.count(new_file_name))
			{
				m_dtc = c_dtc(ecu_id,m_veid);
				m_func_map[new_file_name]=m_dtc.m_all_buffer;
			}
		}
	}
	
	m_buffer += m_ident_name + "," + m_standard_name + "," + m_dtc.m_file_name + ",";
	
	
	if (m_dtc.m_quality_buffer=="")
	{
		m_buffer = m_buffer + ",,\"";
	}
	else
	{
		m_buffer = m_buffer + m_dtc.m_quality_buffer + "\"";
	}
}


vector<menu_info> c_ecu_variant::get_func_file( c_current_veh& cur_veh)
{
	c_tree t_tree;
	c_tree t_var_tree;
	string new_file_name;
	string new_file_path;
	if (cur_veh.m_file_name.substr(0, 4) == "MENU")
	{
		new_file_name = cur_veh.m_file_name;
		new_file_path = cur_veh.m_file_path;
	}
	else
	{
		new_file_path = cur_veh.m_file_path + "\\" + m_name;
		new_file_name = utils::get_menu_file_name(new_file_path);
	}
	t_tree = c_tree(new_file_path, new_file_name, cur_veh);
	vector<menu_info> menu_func_result;
	t_tree.quick_scan(menu_func_result);
	if (!menu_func_result.size())
	{
		c_log::print("c_ecu_variant::获取ecu功能menu失败 %s  %s\n", m_variant_name, m_ecu_id);
	}
	return menu_func_result;
}

void c_ecu_variant::get_global_buffer(c_current_veh& cur_veh)
{
	c_tree t_tree;
	c_tree t_var_tree;
	string new_file_name;
	string new_file_path;
	string variant_file_name;
	if (cur_veh.m_file_name.substr(0, 4) == "MENU")
	{
		new_file_name = cur_veh.m_file_name;
		new_file_path = cur_veh.m_file_path;
		variant_file_name = utils::get_variant_file_name(new_file_path);
	}
	else
	{
		new_file_path = cur_veh.m_file_path + "\\" + m_name;
		new_file_name = utils::get_menu_file_name(new_file_path);
		variant_file_name = utils::get_variant_file_name(new_file_path);
	}


	if (new_file_name!=""&& new_file_name!="MENU__T7__ABRASR__ABS81.s"
		&& new_file_name != "MENU__T84__AUTORADIO__AUTORADIO.s")
	{
		map<string, string> tmp_map;
		t_tree = c_tree(new_file_path, new_file_name, cur_veh);
		vector<menu_info> xxx;
		t_tree.process_tree(xxx);
		tmp_map= t_tree.m_current_veh.map_global_textbuffer;
		for (auto it : tmp_map)
		{
			cur_veh.map_global_textbuffer[it.first] = it.second;
		}
		if (tmp_map.size())
		{
			//c_log::print("have global_buffer:ecu_id:%s ,file_name: %s\n", m_ecu_id.c_str(), new_file_name.c_str());
		}
	}

	if (variant_file_name != "")
	{
		map<string, string> tmp_map1;
		t_var_tree = c_tree(new_file_path, variant_file_name, cur_veh);
		vector<menu_info> xxx;
		t_var_tree.process_tree(xxx);
		tmp_map1= t_var_tree.m_current_veh.map_global_textbuffer;
		for (auto it : tmp_map1)
		{
			cur_veh.map_global_textbuffer[it.first] = it.second;
		}
		if (cur_veh.map_global_textbuffer.size())
		{
			//c_log::print("have global_buffer:ecu_id:%s ,file_name: %s\n", m_ecu_id.c_str(), variant_file_name.c_str());
		}
	}
	if (cur_veh.map_global_textbuffer.size()==2)
	{
		//c_log::print("have 2 global_buffer:ecu_id:%s ,file_name: %s\n", m_ecu_id.c_str(), variant_file_name.c_str());
	}
}


c_data_stream_group::c_data_stream_group()
{

}

c_data_stream_group::c_data_stream_group(int group_num, string ecu_id,string ecu_veid, string scrname, string file_name)
{
	m_file_name = file_name;
	vector<string> vec_parm;
	vector<map<string, data_unit>> result;
	vec_parm.push_back(ecu_id);
	vec_parm.push_back(scrname);
	result = fire_bird::get_sql_result(st_ecu_identification, vec_parm);
	if (!result.size())
	{
		c_log::print("c_data_stream_group:: search SCRID failed ,ecu_id: %s,scrname: %s", ecu_id.c_str(), scrname.c_str());
		return;
	}
	m_scrid = result[0]["SCRID"].data_value;
	m_name = result[0]["SCRTITRE"].data_value;
	m_ecu_id = result[0]["ECUID"].data_value;
	
	c_txt::insert_map(m_name);
	m_buffer = utils::convert_1_byte(group_num)+",0x00,0x00\t" + c_txt::get_str_from_map(m_name) + "\n";

	vec_parm.clear();
	vec_parm.push_back(m_scrid);
	result = fire_bird::get_sql_result(st_id_data_stream, vec_parm);
	if (!result.size())
	{
		c_log::print("c_data_stream_group::search data_stream_group failed ,scrid: %s", m_scrid.c_str(), scrname.c_str());
		return;
	}
	int cnt = 1;
	for (size_t i = 0; i < result.size(); i++)
	{
		//if (i==7&& scrname=="MESUREPARAMETRE3")
		//{
		//	int cc = 0;
		//}
		c_data_stream tt= c_data_stream(result[i],ecu_id, ecu_veid);
		if (tt.load_state==ds_load_success)
		{
			m_buffer += utils::convert_1_byte(group_num) + "," + utils::convert_2_byte(cnt++) + "\t" + tt.get_output_buffer();
			m_vec_data_stream.push_back(tt);
		}
	}
}


c_data_stream::c_data_stream()
{
}

c_data_stream::c_data_stream(map<string, data_unit> x_map,string ecu_id,string ecu_veid)
{

		if (x_map["PARIDCONF"].data_value == "1")
		{
			load_state = ds_load_fail;
			return;
		}
		m_ecu_id = ecu_id;
		m_ecu_veid = ecu_veid;
		m_scr_id = x_map["SCRID"].data_value;
		m_par_id = x_map["PARID"].data_value;
		m_name = x_map["PARLIBMP"].data_value;
		m_par_name = x_map["PARNAME"].data_value;
		m_par_disc = x_map["PARDISC"].data_value;
		m_par_libuint = x_map["PARLIBUNIT"].data_value;
		m_par_detail_string = x_map["PARAIDE"].data_value;
		m_format_match = x_map["PARFORMATMATCH"].data_value;
		m_format_repalce = x_map["PARFORMATREPLACE"].data_value;
		m_ser_unid = get_ser_unid(m_par_name);
		if (m_ser_unid=="")
		{
			load_state = ds_load_fail;
			return;
		}
		m_request_id = get_req_id(m_ser_unid);
		m_receive_id = get_rec_id(m_ser_unid);
		m_request_cmd = get_request_cmd(m_request_id);

		vector<string> vec_parm;
		vec_parm.push_back(m_receive_id);
		vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_data_stream_receive_info, vec_parm);
		if (!result.size())
		{
			c_log::print("c_data_stream::receive_id : %s no result\n", m_receive_id.c_str());
			load_state = ds_load_fail;
			return;
		}
		int flag = 1;
		for (size_t i = 0;i < result.size();i++)
		{
			if (result[i]["PARSNAME"].data_value == m_par_name)
			{
				m_rec_pos = result[i]["ISPBYTEPOS"].data_value;
				m_rec_len = result[i]["ADDBYTELENGTH"].data_value;
				m_rec_order = result[i]["ADDBYTEORDER"].data_value;
				m_line_conver_id = result[i]["PARID"].data_value;
				if (m_rec_pos=="")
				{
					int cc = 0;
				}
				flag = 0;
				break;
			}
		}
		if (flag)
		{
			c_log::print("c_data_stream::receive_id :%s no actual parsname:%s\n", m_receive_id.c_str(), m_par_name.c_str());
			load_state = ds_load_fail;
			return;
		}
		
		
		if (x_map["PARFORMATTYPE"].data_value == "CHAR")
		{
			m_type = type_format;
			format_buffer = "\"" + c_du8::convert_du8_string(m_format_match) + "," + c_du8::convert_du8_string(m_format_repalce) + "\"";
		}
		else
		{
			if (x_map["PARFORMATTYPE"].data_value == "NUMERIC"&& m_line_conver_id !=""&&have_line_conver(m_line_conver_id))
			{
				m_type = type_line_conver;
			}
			else
			{
				m_type = type_tab_conver;
				m_vec_tab_conver = get_vec_disc(m_scr_id, m_par_id);
			}
		}
		load_state = ds_load_success;
}


vector<c_tab_conver> c_data_stream::get_vec_disc(string scr_id, string par_id)
{
	vector<string> vec_parm;
	vec_parm.push_back(scr_id);
	vec_parm.push_back(par_id);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_data_stream_state_disc, vec_parm);
	c_tab_conver tt;
	vector<c_tab_conver> vec;
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

string c_data_stream::get_ser_unid(string par_name)
{
	vector<string> vec_parm;
	vec_parm.push_back(m_ecu_veid);
	vec_parm.push_back(par_name);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_data_stream_serun_id, vec_parm);
	if (!result.size())
	{
		c_log::print("c_data_stream::search SERUNID failed, ecu_id: %s ,veid: %s ,par_name: %s \n", m_ecu_id.c_str(), m_ecu_veid.c_str(), par_name.c_str());
		return string();
	}
	else
	{
		return result[0]["SERUNID"].data_value;
	}
}

string c_data_stream::get_req_id(string ser_unid)
{
	vector<string> vec_parm;
	vec_parm.push_back(ser_unid);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_data_stream_serunfr_id, vec_parm);
	return result[0]["SERUNFRID"].data_value;
	
}

string c_data_stream::get_rec_id(string ser_unid)
{
	vector<string> vec_parm;
	vec_parm.push_back(ser_unid);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_data_stream_serunfr_id, vec_parm);
	return result[1]["SERUNFRID"].data_value;
}

string c_data_stream::get_request_cmd(string ser_unfrid)
{
	vector<string> vec_parm;
	vec_parm.push_back(ser_unfrid);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_data_stream_request_info, vec_parm);
	
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
	tbuffer += get_name_buffer();
	tbuffer += get_xxx_buffer();
	tbuffer += get_tail_buffer();
	tbuffer += "\n";
	return tbuffer;
}


string c_data_stream::get_name_buffer()
{
	string tbuffer;
	c_txt::insert_map(m_name);
	tbuffer = c_txt::get_str_from_map(m_name) + "\t";

	if (m_par_libuint!="NULL")
	{
		c_txt::insert_map(m_par_libuint);
		tbuffer += c_txt::get_str_from_map(m_par_libuint) + "\t";
	}
	else
	{
		tbuffer += "0x00\t";
	}

	if (m_par_detail_string!="NULL")
	{
		c_txt::insert_map(m_par_detail_string);
		tbuffer += c_txt::get_str_from_map(m_par_detail_string) + "\t";
	}
	else
	{
		tbuffer += "0x00\t";
	}
	return tbuffer;
}

string c_data_stream::get_xxx_buffer()
{
	string tbuffer;
	if (m_type == type_format)
	{
		c_format::insert(format_buffer);
		tbuffer = c_format::get_string_from_map(format_buffer);
	}
	else if (m_type == type_tab_conver)
	{
		string xx = get_tab_conver_buffer();
		if (xx!="")
		{
			tbuffer = string("0xFE,") + utils::convert_3_byte(stoi(m_scr_id)) + "," + utils::convert_3_byte(stoi(m_par_id));
			c_tab::insert(tbuffer, xx);//!error;
		}
		else
		{
			c_format::insert("\"HEXA\"");
			tbuffer = c_format::get_string_from_map("\"HEXA\"");
		}
	}
	else if (m_type == type_line_conver)
	{
		tbuffer = string("0xFF,0x00,0x00,")+utils::convert_4_byte(stoi(m_line_conver_id));
	}
	return tbuffer+"\t";
}

string c_data_stream::get_tail_buffer()
{
	string rec_info = m_request_cmd + "\t"
					+ utils::convert_1_byte(stoi(m_rec_pos)) + "\t"
					+ utils::convert_1_byte(stoi(m_rec_len));

	return rec_info;
}

bool c_data_stream::have_line_conver(string line_conver_id)
{
	vector<string> vec_parm;
	vec_parm.push_back(line_conver_id);
	vector<map<string, data_unit>> result = fire_bird::get_sql_result(st_data_stream_algorithm, vec_parm);
	if (result.size())
	{
		return true;
	}
	else
	{
		return false;
	}
}

string c_data_stream::get_tab_conver_buffer()
{
	string tbuffer;
	if (m_vec_tab_conver.size())
	{
		for (u_int i = 0;i < m_vec_tab_conver.size();i++)
		{
			tbuffer += utils::convert_2_byte(stoi(m_vec_tab_conver[i].m_disc_val_com)) + ",";
			c_txt::insert_map(m_vec_tab_conver[i].m_disc_val_lib);
			tbuffer += c_txt::get_str_from_map(m_vec_tab_conver[i].m_disc_val_lib) + "\t";
		}
		return tbuffer;
	}
	else
	{
		return string();
	}
}

c_dtc::c_dtc()
{
}

c_dtc::c_dtc(string ecu_id,string ecu_veid)
{
	m_file_name = ecu_id + "_dtc";

	vector<string> vec_parm;
	vector<map<string, data_unit>> result;
	vec_parm.push_back(ecu_veid);
	result = fire_bird::get_sql_result(st_dtc_serid, vec_parm);
	if (result.size())
	{
		m_serid = result[0]["SERID"].data_value;
	}
	else
	{
		c_log::print("c_dtc:: search SERID failed ,ecu_veid: % s\n", ecu_veid.c_str());
		return;
	}

	vec_parm.clear();
	vec_parm.push_back(m_serid);
	result = fire_bird::get_sql_result(st_dtc_serunid, vec_parm);

	if (result.size())
	{
		m_serunid = result[0]["SERUNID"].data_value;
	}
	else
	{
		c_log::print("c_dtc::search SERUNID failed ,ecu_veid: % s\n", ecu_veid.c_str());
		return;
	}

	vec_parm.clear();
	vec_parm.push_back(m_serunid);
	result = fire_bird::get_sql_result(st_dtc_serunfrid, vec_parm);

	if (result.size())
	{
		m_serunfrid_req = result[0]["SERUNFRID"].data_value;
		m_serunfrid_rec = result[1]["SERUNFRID"].data_value;
	}
	else
	{
		c_log::print("c_dtc:: search SERUNFRID failed ,ecu_veid: % s\n", ecu_veid.c_str());
		return;
	}


	vec_parm.clear();
	vec_parm.push_back(m_serunfrid_req);
	result = fire_bird::get_sql_result(st_dtc_req_cmd, vec_parm);
	for (size_t i = 0; i < result.size(); i++)
	{
		if (result[i]["ISPVALUE"].data_value != "NULL")
		{
			m_req_cmd += result[i]["ISPVALUE"].data_value;
		}
	}
	m_rec_type = get_rec_type(m_serunfrid_rec);//get 


	vec_parm.clear();
	vec_parm.push_back(ecu_veid);
	result = fire_bird::get_sql_result(st_dtc_clr_serid, vec_parm);
	if (result.size())
	{
		m_clr_serid = result[0]["SERID"].data_value;
	}
	else
	{
		c_log::print("c_dtc::search SERID failed , ecu_id:%s  ecu_veid: %s \n", ecu_id.c_str(), ecu_veid.c_str());
		return;
	}

	vec_parm.clear();
	vec_parm.push_back(m_clr_serid);
	result = fire_bird::get_sql_result(st_dtc_clr_serunfrid, vec_parm);
	if (result.size())
	{
		m_clr_serunfrid = result[0]["SERUNFRID"].data_value;
	}
	else
	{
		c_log::print("c_dtc::search SERUNFRID failed, ecu_id:%s  ecu_veid: %s \n", ecu_id.c_str(), ecu_veid.c_str());
		return;
	}
	
	vec_parm.clear();
	vec_parm.push_back(m_clr_serunfrid);
	result = fire_bird::get_sql_result(st_dtc_clr_cmd, vec_parm);
	for (size_t i = 0; i < result.size(); i++)
	{
		if (result[i]["ISPVALUE"].data_value != "NULL")
		{
			m_clr_cmd += result[i]["ISPVALUE"].data_value;
		}
	}

	m_quality_buffer = m_req_cmd + "," + m_rec_type + "," + m_clr_cmd;//获取dtc 信息




	//vec_parm.clear();//get old dtc lib
	//vec_parm.push_back(ecu_id);
	//result = fire_bird::get_sql_result(st_dtc_scrname, vec_parm);
	//for (size_t i = 0; i < result.size(); i++)
	//{
	//	int flag = 1;
	//	for (size_t j = 0; j < m_vec_scrname.size(); j++)
	//	{
	//		if (!strcmp(m_vec_scrname[j].c_str(), result[i]["SCRNAME"].data_value.c_str()))
	//		{
	//			flag=0;
	//			break;
	//		}
	//	}
	//	if (flag)
	//	{
	//		m_vec_scrname.push_back(result[i]["SCRNAME"].data_value);
	//	}
	//}
	//
	//for (size_t i = 0; i < m_vec_scrname.size(); i++)
	//{
	//	vec_parm.clear();
	//	vec_parm.push_back(ecu_id);
	//	vec_parm.push_back(m_vec_scrname[i]);
	//	result = fire_bird::get_sql_result(st_dtc_scrid, vec_parm);
	//	if (result.size())
	//	{
	//		m_vec_scrid.push_back(result[0]["SCRID"].data_value);
	//	}
	//	else
	//	{
	//		c_log::print("c_dtc::search SCRID failed ,ecu_id: % s ,scrname : % s\n", ecu_id.c_str(), m_vec_scrname[i].c_str());
	//		continue;
	//	}
	//}


	vec_parm.clear();//get new dtc lib
	vec_parm.push_back(ecu_id);
	vec_parm.push_back(ecu_id);
	result = fire_bird::get_sql_result(st_dtc_newlib, vec_parm);
	if (!result.size())
	{
		c_log::print("c_dtc:: search DTC LIB failed , ecu_id : %s\n", ecu_id.c_str());
	}

	string dtc_code;
	int hex_dtc_code;
	string dtc_descript;
	string pre_code_name;
	string tmp_code_name;
	for (size_t i = 0; i < result.size(); i++)
	{
		tmp_code_name = result[i]["DTCCODE"].data_value;
		if (i==0)
		{
			pre_code_name = result[i]["DTCCODE"].data_value;

			dtc_code = pre_code_name;
			hex_dtc_code = stoi(dtc_code, 0, 16);
			dtc_descript = c_du8::convert_du8_string(result[i]["DTCLABEL"].data_value);
			m_all_buffer += utils::convert_3_byte(hex_dtc_code) + "\t\"" + dtc_code + "\"\t\"" + dtc_descript + "\"\n";
		}
		else
		{
			if (strcmp(tmp_code_name.c_str(), pre_code_name.c_str()))
			{
				dtc_code = tmp_code_name;
				hex_dtc_code = stoi(dtc_code, 0, 16);
				dtc_descript = c_du8::convert_du8_string(result[i]["DTCLABEL"].data_value);
				m_all_buffer += utils::convert_3_byte(hex_dtc_code) + "\t\"" + dtc_code + "\"\t\"" + dtc_descript + "\"\n";
			}
		} 
		pre_code_name = tmp_code_name;
	}
}

string c_dtc::get_rec_type(string m_serunfrid_rec)
{

	vector<string> vec_parm;
	vector<map<string, data_unit>> result;
	vec_parm.push_back(m_serunfrid_rec);
	result = fire_bird::get_sql_result(st_dtc_rec_info, vec_parm);


	if (result.size() == 5 &&
		result[1]["ISPBYTEPOS"].data_value == "2" && result[1]["PARSNAME"].data_value == "NRODTC"         && result[1]["PARENCODING"].data_value == "HEXA"&& result[1]["ADDBITLENGTH"].data_value == "NULL"&&
		result[2]["ISPBYTEPOS"].data_value == "1" && result[2]["PARSNAME"].data_value == "DTC_ORIGIN"     && result[2]["PARENCODING"].data_value == "BMP" && result[2]["ADDBITLENGTH"].data_value == "16" &&
		result[3]["ISPBYTEPOS"].data_value == "1" && result[3]["PARSNAME"].data_value == "DTC_FAULT_TYPE" && result[3]["PARENCODING"].data_value == "BMP" && result[3]["ADDBITLENGTH"].data_value == "16" &&
		result[4]["ISPBYTEPOS"].data_value == "1" && result[4]["PARSNAME"].data_value == "DTC_CODE"       && result[4]["PARENCODING"].data_value == "HEXA" && result[4]["ADDBITLENGTH"].data_value == "16")
	{
		return "01";
	}

	if (result.size() == 3 &&
		result[1]["ISPBYTEPOS"].data_value == "1" && result[1]["PARSNAME"].data_value == "DTC_CODE"     && result[1]["PARENCODING"].data_value == "HEXA" && result[1]["ADDBYTELENGTH"].data_value == "2" && 
		result[2]["ISPBYTEPOS"].data_value == "3" && result[2]["PARSNAME"].data_value == "DTC_STATUS_1" && result[2]["PARENCODING"].data_value == "HEXA" && result[2]["ADDBYTELENGTH"].data_value == "1")
	{
		return "02";
	}

	if (result.size() == 4 &&
		result[1]["ISPBYTEPOS"].data_value == "2" && result[1]["PARSNAME"].data_value == "NRODTC"       && result[1]["PARENCODING"].data_value == "HEXA" && result[1]["ADDBYTELENGTH"].data_value == "1" &&
		result[2]["ISPBYTEPOS"].data_value == "1" && result[2]["PARSNAME"].data_value == "DTC_CODE"     && result[2]["PARENCODING"].data_value == "HEXA" && result[2]["ADDBYTELENGTH"].data_value == "2" &&
		result[3]["ISPBYTEPOS"].data_value == "3" && result[3]["PARSNAME"].data_value == "DTC_STATUS_1" && result[3]["PARENCODING"].data_value == "HEXA" && result[3]["ADDBYTELENGTH"].data_value == "1" )	
	{
		return "03";
	}

	if (result.size() == 4 &&
		result[1]["ISPBYTEPOS"].data_value == "2" && result[1]["PARSNAME"].data_value == "NRODTC"       && result[1]["PARENCODING"].data_value == "HEXA" && result[1]["ADDBITLENGTH"].data_value == "1" && 
		result[2]["ISPBYTEPOS"].data_value == "1" && result[2]["PARSNAME"].data_value == "DTC_CODE"     && result[2]["PARENCODING"].data_value == "HEXA" && result[2]["ADDBITLENGTH"].data_value == "2" &&
		result[3]["ISPBYTEPOS"].data_value == "3" && result[3]["PARSNAME"].data_value == "DTC_STATUS_1" && result[3]["PARENCODING"].data_value == "HEXA" && result[3]["ADDBITLENGTH"].data_value == "NULL")
	{
		return "04";
	}

	if (result.size() == 6 &&
		result[1]["ISPBYTEPOS"].data_value == "2" && result[1]["PARSNAME"].data_value == "ReportDtcByStatusMask" && result[1]["PARENCODING"].data_value == "HEXA" && result[1]["ADDBYTELENGTH"].data_value == "1"&&
		result[2]["ISPBYTEPOS"].data_value == "3" && result[2]["PARSNAME"].data_value == "DTCSAM"                && result[2]["PARENCODING"].data_value == "HEXA" && result[2]["ADDBYTELENGTH"].data_value == "1" &&
		result[3]["ISPBYTEPOS"].data_value == "1" && result[3]["PARSNAME"].data_value == "DTC_CODE"              && result[3]["PARENCODING"].data_value == "HEXA" && result[3]["ADDBYTELENGTH"].data_value == "2" && 
		result[4]["ISPBYTEPOS"].data_value == "3" && result[4]["PARSNAME"].data_value == "DTC_FAULT_TYPE"        && result[4]["PARENCODING"].data_value == "HEXA" && result[4]["ADDBYTELENGTH"].data_value == "1" &&
		result[5]["ISPBYTEPOS"].data_value == "4" && result[5]["PARSNAME"].data_value == "DTC_STATUS_1"          && result[5]["PARENCODING"].data_value == "BMP" && result[5]["ADDBYTELENGTH"].data_value == "NULL")

	{
		return "05";
	}

	return"00";
}



c_ecu_variant_group::c_ecu_variant_group(vector<string> vec_var_group_str,string ecu_id ,c_current_veh& cur_veh)
{
	string str_variant;
	c_ecu_variant ecu_var;
	
	for (size_t i = 0; i < vec_var_group_str.size(); i++)
	{
		if (ecu_id!="1045")
		{
			continue;
		}
		ecu_var = c_ecu_variant(vec_var_group_str[i], ecu_id, m_func_map, cur_veh);
		if (!ecu_var.is_null)
		{
			m_vec_variant.push_back(ecu_var);
		}
	}
}

void c_ecu_variant_group::print()
{
	string file_path;
	FILE* fp;
	for (auto it:m_func_map)
	{
		file_path = c_path::func_path + it.first + ".txt";
		fp=fopen(file_path.c_str(), "w");
		fwrite(it.second.c_str(), 1, it.second.size(), fp);
		fclose(fp);
	}
}


