#include<map>
#include<string>
#include"sfile.h"

int main()
{
	c_current_veh tt;
	tt.m_moudle_name = "T73_Chine";
	tt.m_ecu_type = "ME749";
	c_tree xx("C:\\Users\\Administrator\\Desktop\\script_reco\\T7_INJ_script_reco_INJ.xml",tt);

	vector<string> vec_item;
	vector<string> vec_input_cohice;
	string output_file;
	

	xx.run_script(vec_item, vec_input_cohice, output_file);
	
	vec_input_cohice.push_back("MEV17_4_EURO4");
	vec_item.clear();

	xx.run_script(vec_item, vec_input_cohice, output_file);
	return 0;
}

