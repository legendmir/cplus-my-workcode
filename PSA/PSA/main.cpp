#include"sfile.h"
#include<vector>
#include"sqls.h"
#include"tinyxml2.h"
#include"basic_data.h"
#include"global_txt.h"

extern map<string, string> g_map_textbuffer;
extern c_txt g_txt;
extern c_format g_format;
extern c_state g_state;
int main()
{

	//N6A|NFU|ME749|FLEXFUEL|REC
	//N6A|NSB|NFU|ME749|FLEXFUEL|REC
	//N6A|NSB|NFU|ME749|FLEXFUEL|REC
	//N6A|NSB|NFU|ME749|FLEXFUEL|REC
	//N6A|NSB|NFU|ME749|FLEXFUEL|REC


	//c_current_veh t_config;
	//t_config.m_comtype = "T7";
	//t_config.m_moudle_name = "T73_Chine";
	//t_config.m_ecu_type = "BOITIER_TELEMATIQUE";
	//t_config.m_ecu_descript = "BOITIER_TELEMATIQUE|VSOFT60";
	//c_tree xxx("vehicle\\T7\\TELEMAT\\BOITIER_TELEMATIQUE", "MENU__T7__TELEMAT__BOITIER_TELEMATIQUE.s", t_config);
	//vector<menu_info> xx_result;
	//xxx.quick_scan(xx_result);


	//sql_info st_ecu_variant_one_line = g_sqls.sql_map[st_ecu_variant_list_one_line_info];
	//vector<string> vec_str1;
	//vec_str1.push_back("BSI_T7_V1");
	//string handled_sql1 = g_sqls.insert_value(st_ecu_variant_one_line.sql, vec_str1);
	//c_fire_bird* veh_result1 = new c_fire_bird(st_ecu_variant_one_line.name, handled_sql1);
	//vector<map<string, data_unit>> one_line_result = veh_result1->get_result_tab();


	c_conver_tab ccc;
	ccc.print();

	c_data_stream_group xx;
	xx.load_data("1041", "MESUREPARAMETRE11");
	xx.print("C:\\Users\\Administrator\\Desktop\\PSA_data\\information.txt");

	g_txt.print();
	g_format.print();
	g_state.print();

	//c_all_veh xx;
	//xx.print_veh_menu();
	//xx.print_ecus_data();

	return 0;
}
