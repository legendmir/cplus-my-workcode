#include"sfile.h"
#include<vector>
#include"sqls.h"
#include"tinyxml2.h"
#include"basic_data.h"
#include"global_txt.h"
#include"log.h"
#include <io.h>

using namespace std;

extern map<string, string> g_map_textbuffer;


int main()
{
	c_format::insert("\"HEXA\"");
	c_format::insert("\"(.{2})(.{3})(.{3})(.{2}),$1 $2 $3 $4\"");
	c_format::insert("\"(.{3})(.{3}),96 $1 $2 80\"");
	c_format::insert("\"(.{2})(.{2}),$1.$2\"");
	c_format::insert("\"(.{3})(.{3})(.{2}),96 $1 $2 $3\"");
	c_format::insert("\"API_DATE_from_JJMMAA_UNM\"");

	//N6A|NFU|ME749|FLEXFUEL|REC
	//N6A|NSB|NFU|ME749|FLEXFUEL|REC
	//N6A|NSB|NFU|ME749|FLEXFUEL|REC
	//N6A|NSB|NFU|ME749|FLEXFUEL|REC
	//N6A|NSB|NFU|ME749|FLEXFUEL|REC


	//c_current_veh t_config;
	//t_config.m_comtype = "T7";
	//t_config.m_moudle_name = "T73_Chine";
	//t_config.m_ecu_type = "ALARME";
	//t_config.m_ecu_descript = "";
	//c_tree xxx("Vehicle\\T7\\ALARMES\\ALARME", "T7_ALARME_mesures_parametres_standard.s", t_config);
	//vector<string> vec_str;
	//xxx.get_vec_scrname(vec_str);

	//vector<string> vec_src;
	//c_cel t_cel;
	//xxx.get_vec_scrname(vec_src, t_cel);
	//sql_info st_ecu_variant_one_line = g_sqls.sql_map[st_ecu_variant_list_one_line_info];
	//vector<string> vec_str1;
	//vec_str1.push_back("BSI_T7_V1");
	//string handled_sql1 = g_sqls.insert_value(st_ecu_variant_one_line.sql, vec_str1);
	//c_fire_bird* veh_result1 = new c_fire_bird(st_ecu_variant_one_line.name, handled_sql1);
	//vector<map<string, data_unit>> one_line_result = veh_result1->get_result_tab();


	//c_conver_tab ccc;
	//ccc.print();

	//c_data_stream_group xx;
	//xx.load_data("1041", "MESUREPARAMETRE11");
	//xx.print("C:\\Users\\Administrator\\Desktop\\PSA_data\\information.txt");

	//string aa = "1111";
	//int cc=utils::convert_bit2_int(aa);


	c_veh xx= c_veh("T73_Chine", "116");
	xx.print_sfile_cmd();
	xx.print_ecus_txt();
	xx.print_quality_txt();



	//c_veh xx = c_veh("P87", "139");
	//xx.print_ecus_txt();
	//xx.print_quality_txt();


	//c_veh xx = c_veh("P84Chine", "137");//4008
	//xx.print_ecus_txt();
	//xx.print_quality_txt();


	//c_veh xx = c_veh("3008C_", "133");
	//xx.print_ecus_txt();
	//xx.print_quality_txt();


	//c_veh xx = c_veh("A94", "130");
	//xx.print_ecus_txt();
	//xx.print_quality_txt();


	return 0;
}
