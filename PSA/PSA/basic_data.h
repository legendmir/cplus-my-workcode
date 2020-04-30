#pragma once
#include"sfile.h"
#include"log.h"
#include"utils.h"
#include"path.h"
using namespace std;

class c_tab_conver
{
public:
	c_tab_conver();

public:
	string m_disc_id;
	string m_dic_name;
	string m_disc_val_lib;
	string m_disc_val_com;
	string m_disc_default;
};


enum stream_type { type_tab_conver, type_line_conver,type_format};




class c_dtc
{
public:
	c_dtc();
	c_dtc(string ecu_id, string ecu_veid);
public:
	string m_file_name;
	string m_all_buffer;
	string m_quality_buffer;
private:
	string get_rec_type(string m_serunfrid_rec);

private:
	string m_serid;
	string m_serunid;
	string m_serunfrid_req;
	string m_serunfrid_rec;
	string m_rec_type;
	string m_req_cmd;
	string m_clr_cmd;
	string m_clr_serid;
	string m_clr_serunfrid;
	string m_rec_info;
	vector<string> m_vec_scrname;
	vector<string> m_vec_scrid;
};



enum ds_load_result {
	ds_load_success,
	ds_load_fail,
};

class c_data_stream
{
public:
	c_data_stream();
	c_data_stream(map<string, data_unit> x_map, string ecu_id, string ecu_veid);

public:
	string get_output_buffer();
	
private:
	vector<c_tab_conver> get_vec_disc(string scr_id, string par_id);
	string get_ser_unid(string par_name);
	string get_rec_id(string ser_unid);
	string get_req_id(string ser_unid);
	string get_request_cmd(string ser_unfrid);
	string get_name_buffer();
	string get_xxx_buffer();
	string get_tail_buffer();
	string get_tab_conver_buffer();
private:

	bool have_line_conver(string line_conver_id);
public:
	ds_load_result load_state;
	stream_type m_type;
	string format_buffer;
	string line_conver_buffer;
	string tab_conver_buffer;
private:
	string m_ecu_id;
	string m_par_id;
	string m_scr_id;
	string m_name;
	string m_par_disc;
	string m_par_name;
	string m_par_libuint;
	string m_par_detail_string;
	string m_ser_unid;
	string m_request_id;
	string m_request_cmd;
	string m_receive_id;
	string m_format_type;
	string m_format_match;
	string m_format_repalce;
	string m_ecu_veid;
private:
	string m_rec_pos;
	string m_rec_len;
	string m_rec_order;
	string m_line_conver_id;

	vector<c_tab_conver>m_vec_tab_conver;
};


class c_data_stream_group
{
public:
	c_data_stream_group();
	c_data_stream_group(int group_num,string ecu_id,string ecu_veid, string scrname,string file_name);

public:
	vector<c_data_stream>m_vec_data_stream;
	string m_buffer;
	string m_file_name;
private:
	string m_scrid;
	string m_name;
	string m_ecu_id;

};


class c_ecu_variant {
public:
	c_ecu_variant();
	c_ecu_variant(string str_var, string ecu_id, map<string, string>& m_func_map,c_current_veh& cur_ceh);

private:
	vector<menu_info> get_func_file(c_current_veh& cur_ceh);
	void get_global_buffer(c_current_veh& cur_veh);

public:
	int is_null;
	string m_buffer;
	static c_dtc m_dtc;

private:
	string m_name;
	string m_id; 
	string m_variant_name;
	string m_ecu_id;
	string m_veid;
	string m_variant_descript;
	string m_re_coframe;
	string m_cmd1;
	string m_cmd2;
	string m_IDEMNEMOVALUE;
	string m_pos;
	string m_length;
	vector<string> m_vec_func_file;


	c_data_stream_group m_ident_group;
	c_data_stream_group m_standard_group;
	
	string m_ident_name;
	string m_standard_name;

};


class c_ecu_variant_group
{
public:
	c_ecu_variant_group(vector<string> vec_var_group_str, string ecu_id,c_current_veh& cur_veh);

public:
	void print();

private:
	vector<string> m_vec_var_str;
public:
	map<string, string> m_func_map;
	vector<c_ecu_variant>m_vec_variant;
};



class c_ecu {
public:
	c_ecu();
	c_ecu(string name,string label,string ve_id,c_current_veh& cur_veh);

private:
	string get_buffer();

public:
	vector<c_ecu_variant_group> m_vec_variant_group;
	string m_buffer;
private:
	string m_fam_id;
	string m_fam_name;
	string m_fam_label;
	string m_file_path;
	string m_file_name;
	protocol_type m_protocal_type;
	string m_famid_request;
	string m_famid_accept;
};

	


class c_veh {
public:
	c_veh();
	c_veh(string mod_name, string mod_id, string reserch_type = "Breakdown_research");
public:
	string get_comtype(string mod_id);
	string get_veid(string veh_comtype);
	string get_ecu_variant_head(char ecn_num, char var_num, short  name_num);
public:
	void print_ecus_txt();
	void print_quality_txt();

public:
	vector<c_ecu> m_vec_ecu;
public:
	string m_veh_name;
	string m_mod_id;
	string m_mod_name;
	string m_veh_label;
	string m_comtype;
	string m_ve_id;
	c_current_veh m_cur_veh;
};






