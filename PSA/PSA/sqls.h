#pragma once
#include<map>
#include<string>
#include<vector>
using namespace std;
struct sql_info
{
	string name;
	string sql;
};


enum sql_type {
	st_veh_info,
	st_ecu_info,
	st_ecu_descript_info,
	st_veh_comtype,
	st_veh_id, 
	st_ecu_protcol,
	st_ecu_variant_list,
	st_ecu_variant_list_all,
	st_ecu_identification,//�汾��Ϣ
	st_id_data_stream,
	st_data_stream_serun_id,
	st_data_stream_serunfr_id,
	st_data_stream_request_info,
	st_data_stream_receive_info,
	st_data_stream_state_disc,
	st_data_stream_line_disc,
	st_data_stream_algorithm,
	st_gloabl_conver,
	st_dtc_serid,
	st_dtc_serunid,
	st_dtc_serunfrid,
	st_dtc_req_cmd,
	st_dtc_clr_serid,
	st_dtc_clr_serunfrid,
	st_dtc_clr_cmd,
	st_dtc_rec_info,
	st_dtc_scrname,
	st_dtc_scrid,
	st_dtc_lib,
	st_dtc_newlib,
};

class c_sql {
public:
	c_sql();
	string replace_parm(string& ori_val,vector<string>& vec_val);
	map<sql_type, sql_info> sql_map;
};

