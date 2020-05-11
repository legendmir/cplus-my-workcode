#include"sfile1.h"


//void get_sfile_buffer(string& path,char* buffer,int&size)
//{
//	FILE* fp = fopen(path.c_str(), "rb");
//	fseek(fp, 0, SEEK_END);
//	size = ftell(fp);
//	fseek(fp, 0, SEEK_SET);
//	buffer = new char[size];
//	fread(buffer, 1, size, fp);
//	fclose(fp);
//}


extern vector<c_global_tree_info> g_tree_info_stack;

int main()
{
	c_current_veh t_config;
	t_config.m_comtype = "T7";
	t_config.m_moudle_name = "T73_Chine";
	t_config.m_ecu_type = "MM6KP";
	t_config.m_ecu_descript = "";
	//c_tree xxx("vehicle\\t7\\INJ\\MM6KP", "T7_MM6KP_mesures_parametres_standard_1300.s", t_config);
	//c_tree xxx("vehicle\\t7\\INJ\\MM6KP", "T7_MM6KP_Boitier_papillon_motorise.s", t_config);
	c_tree xxx("vehicle\\t7\\INJ\\MM6KP", "T7_MM6KP_Changement_calculateur_non_bioflex.s", t_config);


	vector<c_tree>tree_stack;
	tree_stack.push_back(xxx);
	tree_stack.back().m_session_info.tmp_pos= "5_29";

	session_info tem_session;
	while (true)
	{
		if (tree_stack.size()==6)
		{
			int cc = 0;
		}
		tree_stack.back().run_script();
		tem_session = tree_stack.back().m_session_info;
		if (tem_session.type == type_quit)
		{
			if (tree_stack.size()==1)
			{
				break;
			}
			tree_stack.pop_back();
			g_tree_info_stack.pop_back();
		}
		if (tem_session.type == type_menu_screen)
		{
			int xx = 0;
		}
		if (tem_session.type == type_msg_screen)
		{
			int cc = 0;
			
		}
		if (tem_session.type == type_src_name)
		{
			int cc = 0;
		}
		if (tem_session.type == type_sub_tree)
		{
			string xfile_name = tem_session.SubTree.file_name;
			string xfile_path = tem_session.SubTree.file_path;
			c_tree yyy(xfile_path, xfile_name, t_config);
			tree_stack.push_back(yyy);
		}
		tree_stack.back().m_session_info.tmp_pos = tree_stack.back().m_session_info.next_pos;
	}


	return 0;
}