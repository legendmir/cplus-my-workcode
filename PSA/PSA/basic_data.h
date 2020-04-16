#pragma once
#include"fire_bird.h"
#include"sqls.h"
#include"sfile.h"






class c_disc
{
public:
	c_disc();

public:
	string m_disc_id;
	string m_dic_name;
	string m_disc_val_lib;
	string m_disc_val_com;
	string m_disc_default;
};



class c_algorithm
{
public:
	c_algorithm();
public:
	string m_confactor;
	string m_offset;
	string m_con_unit;
};



enum stream_type { type_state, type_no_state,type_CHAR};

class c_receive_info
{
public:
	c_receive_info();

	void init_receive_info(string ser_unfrid,string par_name, stream_type stream_type);
	c_algorithm get_algorithm();
public:
	string m_pos;
	string m_length;
	string m_order;
	string m_par_id;
	string m_par_encoding;
	c_algorithm m_algrithm;
};


class c_dtc
{
public:
	c_dtc();
};


class c_data_stream
{
public:
	c_data_stream();

public:
	vector<c_disc> get_vec_disc(string scr_id, string par_id);
	string get_rec_id(string ser_unid);
	string get_req_id(string ser_unid);
	string get_stream_srunid(string par_name, string ecu_id);
	string get_request_cmd(string ser_unfrid);

	string get_output_buffer();
	
private:
	string get_head_buffer();
	string get_name_buffer();
	string get_xxx_buffer();
	string get_tail_buffer();


private:
	string get_vec_disc_buffer();


public:
	stream_type m_type;
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

	vector<c_disc>m_vec_disc;
	c_receive_info m_receive_info;
};

class menu_info1 {
public:
	string m_file_path;
	string m_file_name;
	string m_menu_string;
};


class c_data_stream_group
{
public:
	c_data_stream_group();
	void load_data(string ecu_id,string group_name);

private:
	void init_vec_data_stream();
	void init_name_and_scrid(string ecu_id, string group_name);


public:
	void print(string path);//输出文件


public:
	string m_scrid;
	string m_name;
	string m_ecu_id;
	vector<c_data_stream>m_vec_data_stream;
};


class c_ecu_variant {
public:
	c_ecu_variant();
	string get_ecu_variant_content(string file_name, string file_path, c_current_veh& current_veh);
private:
	string get_menu_file_name(string file_path);

	void load_qulity_config(vector<menu_info1> vec_menu);

public:
	string m_name;
	string m_id; 
	string m_variant_name;
	string m_ecu_id;
	string m_variant_descript;
	string m_re_coframe;
	string m_cmd1;
	string m_cmd2;
	string m_IDEMNEMOVALUE;
	string m_pos;
	string m_length;

public:
	c_data_stream_group m_identification;
	c_dtc         m_dt_code;
};

enum protocol_type{ 
	LIGNE_K =1, 	
	CAN_IS,
	CAN_CAR,
	CAN_CONF,
	CAN_DIAG,
	LIN_BSI_2,
	CAN_INFO_DIV,
	LIN_BSI_1,
	LIN_BSM
};

class c_ecu {
public:
	c_ecu();

	void get_protcol_info(string veid);
	string get_one_ecu_text(int mod_id, int cnt);
	void get_ecu_variants(vector<string>& vec_variant_name, vector<string>& vec_ecu_id);


public:
	string m_fam_id;
	string m_fam_name;
	string m_fam_label;
	string m_file_path;
	string m_file_name;
	protocol_type m_protocal_type;
	string m_famid_request;
	string m_famid_accept;
};

struct variant_stru {
	string file_name;
	string file_path;
	c_ecu_variant ecu_variant;
	int ecu_num;
};

class c_veh {
public:
	c_veh();

public:
	string get_comtype(string mod_id);
	string get_veid(string veh_comtype);
	string get_ecu_variant_head(char ecn_num, char var_num, short  name_num);
	c_ecu_variant get_one_ecu_variant(string, string);
private:
	string get_descript(string xx);
	vector<variant_stru> get_variants_can_be_searched();
	string get_ecu_quality_buffer(variant_stru& var_stru, int var_num, int name_num);
	
public:
	void load_ecu_data();
	void print_ecus_txt(string path="C:\\Users\\Administrator\\Desktop\\PSA_data\\");
	void print_ecus_quality_txt(string path = "C:\\Users\\Administrator\\Desktop\\PSA_data\\");
	void print_ecus_quality_check(string path = "C:\\Users\\Administrator\\Desktop\\PSA_data\\");//查看ecu 配置文件

public:
	string m_veh_name;
	string m_mod_id;
	string m_mod_name;
	string m_veh_label;
	string m_veh_comtype;
	string m_ve_id;
	vector<c_ecu> m_vec_ecu;
	vector<c_ecu_variant> m_vec_ecu_variant;
};

class c_all_veh {
public:
	c_all_veh();
	void print_veh_menu(string path = "C:\\Users\\Administrator\\Desktop\\PSA_data\\");
	void print_ecus_data(string path = "C:\\Users\\Administrator\\Desktop\\PSA_data\\");
public:
	vector<c_veh> m_vec_veh;
};






