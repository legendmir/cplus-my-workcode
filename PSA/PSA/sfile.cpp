#include "sfile.h"

c_sql g_sqls;
vector<c_global_tree_info> g_tree_info;
map<string, string> g_map_textbuffer;
c_current_veh c_tree::m_current_veh;
extern c_txt g_txt;

typedef  unsigned int u_int;

std::string& replace_all_vec(std::string& str, std::string& old_value, std::vector<std::string>& vec_new_value)
{
	int i = 0;
	std::string::size_type   pos(0);
	for (i = 0;i < vec_new_value.size();i++)
	{
		if ((pos = str.find(old_value)) != std::string::npos)
		{
			str.replace(pos, old_value.length(), vec_new_value[i]);
		}
	}
	return   str;
}

vector<string> c_node::split(string ori_string, char sig)
{
	unsigned int i = 0;
	vector<string> vec_str;
	string tstr;
	unsigned int flag = 0;
	for (i = 0;i < ori_string.size();i++)
	{
		if (ori_string[i] == '"' && flag == 0)
		{
			flag = 1;
			i++;
		}
		else if (ori_string[i] == '"' && flag == 1)
		{
			flag = 0;
			i++;
		}
		if (ori_string[i] == sig && flag == 0)
		{
			vec_str.push_back(tstr);
			tstr.clear();
		}
		else
		{
			tstr += ori_string[i];
		}
	}
	vec_str.push_back(tstr);
	return vec_str;
}

string c_node::rough_text_handle(string text)
{
	unsigned int i;
	string tstr;
	unsigned int len = text.size();

	for (i = 1;i < len;i++)
	{
		if (i == len - 1 && text[len - 1] == '>')
		{
			break;
		}
		tstr.push_back(text[i]);
	}
	return tstr;
}
c_node::c_node()
{
}
c_node::c_node(int line, string text)
{
	m_line = line;
	string new_text = rough_text_handle(text);
	new_text = get_m_value(new_text);
	vector<string> vec_text = split(new_text, ' ');
	m_str_name = vec_text[0];

	unsigned int i = 0;
	for (i = 1;i < vec_text.size();i++)
	{
		insert_map_cel(vec_text[i]);
	}
}
string c_node::get_m_value(string text)
{
	unsigned int sig = text.find('>');
	if (sig != -1)
	{
		vector<string> vec_str = split(text, '>');
		m_str_value = vec_str[1];
		return vec_str[0];
	}
	else
	{
		m_str_value = "";
		return text;
	}
}

vector<c_node> c_node::get_vec_son()
{
	unsigned int i = 0;
	vector<c_node>vec_son;
	c_node son = g_tree_info.back().m_vec_node[m_line + 1];
	if(son.m_str_name=='/'+m_str_name)
	{ 
		return vec_son;
	}
	vec_son.push_back(son);
	for (i = m_line + 2;i < g_tree_info.back().m_vec_node.size();i++)
	{
		if (g_tree_info.back().m_vec_node[i].m_str_name == "/" + son.m_str_name && g_tree_info.back().m_vec_node[i + 1].m_str_name != m_str_name)
		{
			if (g_tree_info.back().m_vec_node[i + 1].m_str_name == "/" + m_str_name)
			{
				break;
			}
			son = g_tree_info.back().m_vec_node[i + 1];
			vec_son.push_back(son);
		}
	}
	return vec_son;
}

void c_node::insert_map_cel(string tstr)
{
	vector<string> t = split(tstr, '=');
	m_map_attr[t[0]] = t[1];
}

c_cel::c_cel()
{
}
c_cel::c_cel(c_node& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
	if (m_str_name == "PartDiag")
	{
		m_str_pos = m_vec_son[1].m_map_attr["posc"] + "_" + m_vec_son[1].m_map_attr["posl"];
		m_cel_type = ct_partdiag;
	}
	else
	{
		m_str_pos = m_map_attr["posc"] + "_" + m_map_attr["posl"];
		m_cel_type = ct_normal;
	}
}
c_cel c_cel::move_next(vector<menu_info>& xx_result)
{
	c_cel dest_cel;
	if (m_str_name == "Test")
	{
		c_test_cel test_cel(*this);
		dest_cel = test_cel.move_next();
	}
	else if (m_str_name == "Switch")
	{
		c_switch_cel x_switch_cel(*this);
		dest_cel = x_switch_cel.move_next();
	}
	else if (m_str_name == "PartDiag")
	{
		c_part_diag_cel part_diag_cel(*this);
		dest_cel = part_diag_cel.move_next(xx_result);
	}
	else if (m_str_name == "SubTree")
	{
		c_sub_tree_cel subtree_cel(*this);
		if (subtree_cel.m_file_name != "MODEDEGRADE_EMF.s" &&
			subtree_cel.m_file_name != "tlch_matrix.s"&&
			subtree_cel.m_file_name != "MATRIX_TELEXX_CTX.s"&&
			subtree_cel.m_file_name != "script_tokenizer.s"
			//&&subtree_cel.m_file_name != "contextualisation_gen.s"
			)
		{
			subtree_cel.m_tree.process_tree(xx_result);
			dest_cel = subtree_cel.move_next();
		}
		else
		{
			g_tree_info.pop_back();
			dest_cel = subtree_cel.move_next();
		}
	}
	else if (m_str_name == "SubTreeEx")
	{
		c_sub_tree_ex_cel subtree_ex_cel(*this);
		if (subtree_ex_cel.m_tree.m_file_name!="")
		{
			subtree_ex_cel.m_tree.process_tree(xx_result);
		}
		dest_cel = subtree_ex_cel.move_next();
	}
	else if (m_str_name == "Act")
	{
		c_act_cel act_cel(*this);
		dest_cel = act_cel.move_next();
	}
	else if (m_str_name == "Start")
	{
		c_start_node start_cel(*this);
		dest_cel = start_cel.move_next();
	}
	else if (m_str_name == "MsgScreen")
	{
		c_start_node start_cel(*this);
		dest_cel = start_cel.move_next();
	}
	return dest_cel;
}


string c_cel::get_dest()
{
	return string();
}

c_test_cel::c_test_cel()
{
}

c_test_cel::c_test_cel(c_cel& tcel)
{
	m_line = tcel.m_line;
	m_map_attr = tcel.m_map_attr;
	m_str_name = tcel.m_str_name;
	m_str_value = tcel.m_str_value;
	m_vec_son = tcel.m_vec_son;

	m_dest_yes = tcel.m_map_attr["destyesc"] + "_" + tcel.m_map_attr["destyesl"];
	m_dest_no = tcel.m_map_attr["destnoc"] + "_" + tcel.m_map_attr["destnol"];
}

c_cel c_test_cel::move_next()
{
	bool test_result=true;
	c_cel dest_cel;
	if (m_vec_son[0].m_str_name == "MathAndLogic")
	{
		c_math_logic_node math_logic_node(m_vec_son[0]);
		test_result = math_logic_node.process_math_logic();
	}
	unsigned int i = 0;
	if (test_result)
	{
		dest_cel = c_tree::get_cel_by_pos(m_dest_yes);
	}
	else
	{
		dest_cel = c_tree::get_cel_by_pos(m_dest_no);
	}

	return dest_cel;
}

c_math_logic_node::c_math_logic_node()
{
}

c_math_logic_node::c_math_logic_node(c_node& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
}

bool c_math_logic_node::process_math_logic()
{
	bool logic_result = true;
	unsigned int i = 0;
	for (i = 0;i < m_vec_son.size();i++)
	{
		if (m_vec_son[i].m_str_name == "LogicalTest")
		{
			c_logic_test_node logic_test_node(m_vec_son[i]);
			logic_result = logic_test_node.process_logic_test();
		}
		if (m_vec_son[i].m_str_name == "Assignment")
		{
			c_assignment_node assignment_node(m_vec_son[i]);
			assignment_node.process_assignment();
		}
	}
	return logic_result;
}

c_logic_test_node::c_logic_test_node(c_node& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
}

bool c_logic_test_node::process_logic_test()
{
	unsigned int i = 0;
	bool result = false;
	for (i = 0;i < m_vec_son.size();i++)
	{
		if (m_vec_son[i].m_str_name == "For")
		{
			c_for_node temp_for_node(m_vec_son[i]);
			result = temp_for_node.process_for();
			if (result)
			{
				break;
			}
		}
	}
	return result;
}

c_logic_test_node::c_logic_test_node()
{
}

c_for_node::c_for_node()
{
}

c_for_node::c_for_node(c_node& tnode)
{
	m_vec_son = tnode.get_vec_son();
}

bool c_for_node::process_for()
{
	bool xx = true;
	u_int i = 0;
	for (i = 0;i < m_vec_son.size();i++)
	{
		if (m_vec_son[i].m_str_name == "NOT")
		{
			c_not_node not_node = c_not_node(m_vec_son[i]);
			if (!not_node.process_not())
			{
				xx=false;
			}
		}
		if (m_vec_son[i].m_str_name == "DataComparison")
		{
			c_data_comparison_node data_compar_node(m_vec_son[i]);
			if (!data_compar_node.process_datacomparison())
			{
				xx = false;
			}
		}
	}
	return xx;
}



c_data_comparison_node::c_data_comparison_node(c_node& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();

	m_type = tnode.m_map_attr["type"];
}

bool c_data_comparison_node::process_datacomparison()
{
	unsigned int i = 0;
	unsigned int flag = 0;

	c_source_node source1(m_vec_son[0]);
	c_source_node source2(m_vec_son[1]);

	if (!strcmp(m_type.c_str(), "EQUAL"))
	{
		if (!strcmp(source1.m_str_name.c_str(), "ECUData") && !strcmp(source1.m_compare_value.c_str(), "STATE"))
		{
			if (!strcmp(source2.m_compare_value.c_str(), "OK"))
				return true;
			else
				return false;
		}

		if (!strcmp(source1.m_str_name.c_str(), "TextBuffer") && !strcmp(source1.m_compare_value.c_str(), "RECONNU"))
		{
			if (!strcmp(source2.m_compare_value.c_str(), "OUI"))
				return true;
			else
				return false;
		}

		
		if (!strcmp(source1.m_compare_value.c_str(), "VCHK") || !strcmp(source2.m_compare_value.c_str(), "VCHK"))
		{
			return true;
		}
		if (!strcmp(source1.m_str_name.c_str(), "FloatNumber") || !strcmp(source2.m_compare_value.c_str(), "0"))
		{
			return false;
		}
		if (!strcmp(source1.m_compare_value.c_str(), source2.m_compare_value.c_str()))
		{
			return true;
		}
	}
	if (!strcmp(m_type.c_str(), "DIFFERENT"))
	{
		if (!strcmp(source1.m_compare_value.c_str(), source2.m_compare_value.c_str()))
		{
			return false;
		}
	}
	return false;
}

c_source_node::c_source_node()
{
}

c_source_node::c_source_node(c_node& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
	m_str_name = m_vec_son[0].m_str_name;

	m_compare_value = get_source();
}

string c_source_node::get_source()
{
	string source_name = m_vec_son[0].m_str_name;
	if (source_name == "VehicleData")
	{
		m_compare_value = c_tree::m_current_veh.m_moudle_name;
	}
	if (source_name == "ASCIIString")
	{
		m_compare_value = m_vec_son[0].m_map_attr["value"];
	}
	if (source_name == "FloatVar")
	{
		m_compare_value = m_vec_son[0].m_map_attr["name"];
	}
	if (source_name == "TextBuffer")
	{
		c_text_buffer_node text_buffer_node(g_tree_info.back().m_vec_node[m_vec_son[0].m_line]);
		string text_name = text_buffer_node.m_map_attr["name"];
		string text_buf = g_map_textbuffer[text_name];//c_tree::m_current_veh.map_global_textbuffer[text_name];
		if (text_buffer_node.m_vec_son.size() == 0)
		{
			m_compare_value = text_buf;
		}
		else
		{
			int max_len = text_buffer_node.m_pos - 1 + text_buffer_node.m_length;
			if (!strcmp(text_name.c_str(), "OUTPUT_DIAG_MODE") && text_buf != "NULL")
			{
				if (max_len > text_buf.size())
				{
					m_compare_value = "";
				}
				else
				{
					m_compare_value = text_buf.substr(text_buffer_node.m_pos - 1, text_buffer_node.m_length);
				}
			}
			else
			{
				m_compare_value = "";
			}
			if (!strcmp(text_name.c_str(), "REPC9"))
			{
				if (max_len > text_buf.size())
				{
					m_compare_value = "";
				}
				else
				{
					m_compare_value = text_buf.substr(text_buffer_node.m_pos - 1, text_buffer_node.m_length);
				}
			}
		}
	}
	if (source_name == "ECUData")
	{
		m_compare_value = m_vec_son[0].m_map_attr["ParamName"];
		if (!strcmp(m_compare_value.c_str(), "C"))
		{
			m_compare_value = c_tree::m_current_veh.m_ecu_type;
		}
	}
	if (source_name == "FloatNumber")
	{
		m_compare_value = m_str_value;
	}
	if (source_name == "NamedFloatVar")
	{
		m_compare_value = m_map_attr["name"];
	}
	if (source_name == "StandardValue")
	{

	}
	if (source_name == "Hexaunsigned int")
	{
	}
	if (source_name == "FloatVariable")
	{
	}
	if (source_name == "GlobalFloatVar")
	{

	}
	if (source_name == "NamedGlobalFloatVar")
	{

	}
	if (source_name == "GlobalTextBuffer")
	{
		string gbuffer_name= m_vec_son[0].m_map_attr["name"];
		if (!c_tree::m_current_veh.map_global_textbuffer.count(gbuffer_name))
		{
			c_log::print("get global buffer failed, file : %s, ecu :%s buffer_name ,:%s\n", c_tree::m_current_veh.m_file_name.c_str(), c_tree::m_current_veh.m_ecu_type.c_str(), gbuffer_name.c_str());
		}
		else
		{
			m_compare_value = c_tree::m_current_veh.map_global_textbuffer[gbuffer_name];
		}
	}
	if (source_name == "WideTextBuffer")
	{

	}
	if (source_name == "GlobalWideTextBuffer")
	{

	}
	if (source_name == "Buffer")
	{

	}
	if (source_name == "GlobalBuffer")
	{

	}
	if (source_name == "NamedBuffer")
	{

	}
	if (source_name == "NamedGlobalBuffer")
	{

	}

	if (source_name == "BufferVariable")
	{

	}
	if (source_name == "HexaBuffer")
	{

	}
	if (source_name == "ApplicationData")
	{

	}
	if (source_name == "UserData")
	{

	}
	if (source_name == "SharedContextData")
	{

	}
	return m_compare_value;
}

c_text_buffer_node::c_text_buffer_node(c_node& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();

	unsigned int i = 0;
	for (i = 0;i < m_vec_son.size();i++)
	{
		if (m_vec_son[i].m_str_name == "Position")
		{
			c_node position_node = g_tree_info.back().m_vec_node[m_vec_son[i].m_line + 1];
			string position_value = position_node.m_str_value;
			m_pos = atoi(position_value.c_str());
		}
		if (m_vec_son[i].m_str_name == "Length")
		{
			m_length = atoi(m_vec_son[i].m_str_value.c_str());
		}
	}
}

c_act_cel::c_act_cel()
{
}

c_act_cel::c_act_cel(c_cel& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();

	m_dest_pos = tnode.m_map_attr["destc"] + "_" + tnode.m_map_attr["destl"];
}

c_cel c_act_cel::move_next()
{
	process_act();
	return c_tree::get_cel_by_pos(m_dest_pos);
}

void c_act_cel::process_act()
{
	unsigned int i = 0;
	for (i = 0;i < m_vec_son.size();i++)
	{
		if (m_vec_son[i].m_str_name == "ECUCommunication")
		{
			c_ecu_comm_node tcomm_node(m_vec_son[i]);
			tcomm_node.process_ecu_comm();
		}
		if (m_vec_son[i].m_str_name == "MathAndLogic")
		{
			c_math_logic_node tmath_node(m_vec_son[i]);
			tmath_node.process_math_logic();
		}
	}
	return;
}

c_DefaultSwitchTarget::c_DefaultSwitchTarget()
{
}

c_DefaultSwitchTarget::c_DefaultSwitchTarget(c_node& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();


	m_default_pos = m_vec_son[0].m_map_attr["destc"] + "_" + m_vec_son[0].m_map_attr["destl"];
}

c_switch_cel::c_switch_cel()
{
}

c_switch_cel::c_switch_cel(c_cel& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();

	unsigned int i = 0;
	for (i = 0;i < m_vec_son.size();i++)
	{
		if (m_vec_son[i].m_str_name == "Source")
		{
			c_source_node tsource(m_vec_son[i]);
			m_source = tsource;
		}
		if (m_vec_son[i].m_str_name == "SwitchTest")
		{
			c_switch_test_node tt(m_vec_son[i]);
			m_vec_switch_node.push_back(tt);
		}
		if (m_vec_son[i].m_str_name == "DefaultSwitchTarget")
		{
			c_DefaultSwitchTarget t_target_node(m_vec_son[i]);
			m_default_target_node = t_target_node;
		}
	}
}

c_cel c_switch_cel::move_next()
{
	string result_pos = process_switch();
	return c_tree::get_cel_by_pos(result_pos);
}

string c_switch_cel::process_switch()
{
	unsigned int i = 0;
	unsigned int flag = 1;
	string source_value = m_source.m_compare_value;
	string result_pos;
	for (i = 0;i < m_vec_switch_node.size();i++)
	{
		if (!strcmp(m_vec_switch_node[i].m_type_name.c_str(), "EQUAL"))
		{
			if (!strcmp(m_vec_switch_node[i].m_source.m_compare_value.c_str(), source_value.c_str()))
			{
				flag = 0;
				result_pos = m_vec_switch_node[i].m_target_node.m_target_pos;
				break;
			}
		}
	}
	if (flag)
	{
		result_pos = m_default_target_node.m_default_pos;
	}
	return result_pos;
}

c_switch_test_node::c_switch_test_node()
{
}

c_switch_test_node::c_switch_test_node(c_node& tnode)
{
	vector<c_node>vec_son = tnode.get_vec_son();
	c_source_node t_source(vec_son[0]);
	m_source = t_source;
	c_target_node t_target(vec_son[1]);
	m_target_node = t_target;
	m_type_name = tnode.m_map_attr["type"];
}

c_sub_tree_cel::c_sub_tree_cel(c_node tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();

	m_dest_pos = m_map_attr["destc"] + "_" + m_map_attr["destl"];
	m_file_name = m_map_attr["FileName"];
	m_file_path = m_vec_son[0].m_map_attr["path"];
	c_tree t_tree(m_file_path, m_file_name,c_tree::m_current_veh);
	m_tree = t_tree;
}

c_cel c_sub_tree_cel::move_next()
{
	return c_tree::get_cel_by_pos(m_dest_pos);
}

c_ecu_function_call_node::c_ecu_function_call_node(c_node& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();

}

void c_ecu_function_call_node::process_ecu_function()
{
	if (!strcmp(m_map_attr["name"].c_str(),"ECU_DESCRIPTOR"))
	{
		for (size_t i = 0; i < m_vec_son.size(); i++)
		{
			if (m_vec_son[i].m_str_name=="Destination")
			{
				c_destnation_node dest_node(m_vec_son[i]);
				g_map_textbuffer[dest_node.m_buffer_name] = c_tree::m_current_veh.m_ecu_descript;
			}
		}
	}
}

c_assignment_node::c_assignment_node()
{

}

c_assignment_node::c_assignment_node(c_node& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
}

void c_assignment_node::process_assignment()
{
	c_source_node tsource(m_vec_son[0]);
	c_destnation_node tdestination(m_vec_son[1]);
	g_map_textbuffer[tdestination.m_buffer_name] = tsource.m_compare_value;
	if (tdestination.m_type_name=="GlobalTextBuffer")
	{
		c_tree::m_current_veh.map_global_textbuffer[tdestination.m_buffer_name]= tsource.m_compare_value;
	}
}

c_destnation_node::c_destnation_node()
{
}

c_destnation_node::c_destnation_node(c_node& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();

	m_type_name = m_vec_son[0].m_str_name;
	m_buffer_name = m_vec_son[0].m_map_attr["name"];
}

void c_ecu_comm_node::process_ecu_comm()
{
	if (m_vec_son[0].m_str_name == "ECUFunctionCall")
	{
		c_ecu_function_call_node tfunc_call(m_vec_son[0]);
		tfunc_call.process_ecu_function();
	}
}

c_ecu_comm_node::c_ecu_comm_node(c_node& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
}

c_target_node::c_target_node()
{
}

c_target_node::c_target_node(c_node& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_target_pos = m_map_attr["destc"] + "_" + m_map_attr["destl"];
}

c_appl_node::c_appl_node()
{
}

c_appl_node::c_appl_node(c_node tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
}

bool c_appl_node::process_appl()
{
	
	bool result = false;
	for (u_int i = 0;i < m_vec_son.size();i++)
	{
		if (m_vec_son[i].m_str_name == "For")
		{
			c_for_node for_node(m_vec_son[i]);
			result = for_node.process_for();
			if (result)
			{
				return true;
			}
		}
	}
	return result;
}

c_part_diag_cel::c_part_diag_cel()
{
}

c_part_diag_cel::c_part_diag_cel(c_cel& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();

	c_appl_node tappl(m_vec_son[0]);
	m_appl_node = tappl;
	c_cel  tcel(m_vec_son[1]);
	m_cel = tcel;
}

c_cel c_part_diag_cel::move_next(vector<menu_info>& xx_result)
{
	return m_cel.move_next(xx_result);
}

c_start_node::c_start_node()
{
}

c_start_node::c_start_node(c_cel& tnode)
{
	m_dest_pos = tnode.m_map_attr["destc"] + "_" + tnode.m_map_attr["destl"];
}

c_cel c_start_node::move_next()
{
	return c_tree::get_cel_by_pos(m_dest_pos);
}

c_tree::c_tree()
{
}

c_tree::c_tree(string file_path, string file_name, c_current_veh& current_veh)
{
	m_current_veh = current_veh;
	m_full_path = c_path::source_path + "\\" + file_path.c_str() + "\\" + file_name.c_str();
	m_file_name = file_name;
	init_vec_node();
	g_tree_info.push_back(m_tree_info);
	init_vec_cel();
	g_tree_info.pop_back();
	g_tree_info.push_back(m_tree_info);
}

void c_tree::init_vec_node()
{

	FILE* fp = fopen(m_full_path.c_str(), "rb");
	fseek(fp, 0, SEEK_END);
	unsigned int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buffer = new char[len];
	char* tbuf = 0;
	fread(buffer, 1, len, fp);
	fclose(fp);

	unsigned int i = 0;
	unsigned int line_start = 0;
	unsigned int line_cnt = 0;
	while (i < len)
	{
		if (buffer[i] == '\x0D' & buffer[i + 1] == '\x0A')
		{
			buffer[i] = '\x00';
			tbuf = buffer + line_start;
			i += 2;
			line_start = i;
			if (tbuf[1] != '!')
			{
				string tstring(tbuf);
				if (line_cnt == 19)
				{
					unsigned int cc = 0;
				}
				c_node tnode(line_cnt++, tstring);
				m_tree_info.m_vec_node.push_back(tnode);
			}
		}
		i++;
	}
}

void c_tree::init_vec_cel()
{
	vector<c_node> tree_son = m_tree_info.m_vec_node[0].get_vec_son();
	c_node tnode = tree_son[2];
	vector<c_node> cel_son = tnode.get_vec_son();
	unsigned int i = 0;
	c_cel tcel;
	for (i = 0;i < cel_son.size();i++)
	{
		tcel = cel_son[i];
		if (tcel.m_str_name == "PartDiag")
		{
			m_tree_info.m_vec_part_diag.push_back(tcel);
		}
		else
		{
			m_tree_info.m_vec_cel.push_back(tcel);
		}
	}
}

c_cel c_tree::get_cel_by_pos(string pos)
{
	unsigned int i = 0;
	c_cel dest_cel;
	int flag = 1;

	for (i = 0;i < g_tree_info.back().m_vec_cel.size();i++)
	{
		string xx = g_tree_info.back().m_vec_cel[i].m_str_pos;
		if (!strcmp(pos.c_str(), g_tree_info.back().m_vec_cel[i].m_str_pos.c_str()))
		{
			flag = 0;
			dest_cel = g_tree_info.back().m_vec_cel[i];
			break;
		}
	}
	if (flag)
	{
		for (i = 0;i < g_tree_info.back().m_vec_part_diag.size();i++)
		{
			c_part_diag_cel part_diag_cel(g_tree_info.back().m_vec_part_diag[i]);
			if (part_diag_cel.m_cel.m_str_pos== pos)
			{
				if (part_diag_cel.m_appl_node.process_appl() == true)
				{
					dest_cel = part_diag_cel.m_cel;
					break;
				}
			}
		}
	}
	return dest_cel;
}

void c_tree::process_tree(vector<menu_info>& xx_result)
{
	c_cel pre_cel = g_tree_info.back().m_vec_cel[0];
	c_cel next_cel = pre_cel;
	while (next_cel.m_str_name != "End")
	{
		if (next_cel.m_str_name== "MenuScreen")
		{
			c_menu_screen_cel menu_screen_cel(pre_cel, xx_result);
			break;;
		}
		else
		{
			next_cel = pre_cel.move_next(xx_result);
			if (next_cel.m_str_pos=="")
			{
				c_log::print("run tree failed , file_path : %s , file_name : %s\n", m_current_veh.m_file_path.c_str(), m_current_veh.m_file_name.c_str());
				break;
			}
			pre_cel = next_cel;
		}
	}
	g_tree_info.pop_back();
}

void c_tree::quick_scan(vector<menu_info>& xx_result)
{
	if (is_only_one_menu())
	{
		for (int i = 0;i < g_tree_info.back().m_vec_cel.size();i++)
		{
			if (g_tree_info.back().m_vec_cel[i].m_str_name == "MenuScreen")
			{
				c_menu_screen_cel menu_screen_cel(g_tree_info.back().m_vec_cel[i], xx_result);
				return;
			}
		}
	}
	else
	{
		process_tree(xx_result);
	}
}

void c_tree::get_vec_scrname(vector<string>&vec_scr_name,c_cel& x_cel)
{
	c_cel pre_cel;
	c_cel next_cel;
	vector<menu_info> xx_result;
	if (x_cel.m_str_name=="")
	{
		pre_cel = g_tree_info.back().m_vec_cel[0];
		next_cel = pre_cel;
	}
	else
	{
		pre_cel = x_cel;
		next_cel = pre_cel;
	}
	while (next_cel.m_str_name != "End")
	{
		if (next_cel.m_str_name == "DiagScreen")
		{
			c_diag_screen tt = c_diag_screen(next_cel);
			vec_scr_name.push_back(tt.m_scr_name);
			return;
		}
		else if(next_cel.m_str_name == "MenuScreen")
		{
			c_menu_screen_cel menu_screen_cel= c_menu_screen_cel(next_cel, xx_result);
			for (size_t i = 0; i < menu_screen_cel.vec_next_cel.size(); i++)
			{
				get_vec_scrname(vec_scr_name, menu_screen_cel.vec_next_cel[i]);
			}
			return;
		}
		else
		{
			if (next_cel.m_str_name == "")
			{
				return;
			}
			next_cel = pre_cel.move_next(xx_result);
			pre_cel = next_cel;
		}

	}
	return;
	//g_tree_info.pop_back();
}



bool c_tree::is_only_one_menu()
{
	unsigned int i = 0;
	int cnt = 0;
	for (i = 0;i < g_tree_info.back().m_vec_cel.size();i++)
	{
		if (g_tree_info.back().m_vec_cel[i].m_str_name == "MenuScreen")
		{
			cnt++;
		}
	}
	if (cnt == 1)
	{
		return true;
	}
	return false;
}

bool c_tree::is_script_file()
{
	unsigned int i = 0;
	string xxx;
	for (i = 0;i < 12;i++)
	{
		xxx.push_back(m_file_name[i]);
	}
	if (xxx == "script_reco_")
		return true;
	return false;
}

c_xml::c_xml(string ecu_name, string veh_name)
{
	m_xml_file_path = "F:\\Job Project\\Peugeot\\AWRoot\\dtrd\\database\\xml\\contextualisation.xml";
	get_variant_file_name(ecu_name, veh_name);
}

string c_xml::get_variant_file_name(string ecu_name, string veh_name)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(m_xml_file_path.c_str());
	tinyxml2::XMLElement* ecu_element = doc.RootElement()->FirstChildElement();

	while (ecu_element)
	{
		const char* t = ecu_element->FindAttribute("name")->Value();
		if (!strcmp(t, ecu_name.c_str()))//MM6KP
		{
			break;
		}
		ecu_element = ecu_element->NextSiblingElement();
	}

	if (ecu_element == 0)
	{
		return string();
	}
	tinyxml2::XMLElement* veh_element = ecu_element->FirstChildElement();
	while (veh_element)
	{
		const char* t = veh_element->FindAttribute("name")->Value();
		if (!strcmp(t, veh_name.c_str()))//T7
		{
			break;
		}
		veh_element = veh_element->NextSiblingElement();
	}
	if (veh_element==0)
	{
		return string();
	}
	m_path = veh_element->FirstChildElement()->GetText();
	m_file_name = veh_element->FirstChildElement()->NextSiblingElement()->GetText();
	return string();
}

c_sub_tree_ex_cel::c_sub_tree_ex_cel(c_node tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();

	m_dest_pos = m_map_attr["destc"] + "_" + m_map_attr["destl"];

	c_xml xx(c_tree::m_current_veh.m_ecu_type, c_tree::m_current_veh.m_comtype);
	if (xx.m_file_name=="")
	{
		return;
	}
	c_tree t_tree(xx.m_path, xx.m_file_name, c_tree::m_current_veh);
	m_tree = t_tree;
}

c_cel c_sub_tree_ex_cel::move_next()
{
	return c_tree::get_cel_by_pos(m_dest_pos);
}

c_menu_screen_cel::c_menu_screen_cel()
{
}

c_menu_screen_cel::c_menu_screen_cel(c_cel tnode, vector<menu_info>& xx_result)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();

	unsigned int i = 0;
	int item_cnt = 0;
	for (i = 0;i < m_vec_son.size();i++)
	{
		if (m_vec_son[i].m_str_name == "MenuItem")
		{
			item_cnt++;
			c_menu_item menu_item(m_vec_son[i]);
			c_cel xx_cel = c_tree::get_cel_by_pos(menu_item.m_target.m_target_pos);
			vec_next_cel.push_back(xx_cel);
		}
	}
	if (item_cnt != m_vec_son.size())
	{
		for (i = 0;i < m_vec_son.size();i++)
		{
			if (m_vec_son[i].m_str_name == "FunctionSubTree")
			{
				c_function_subtree t(m_vec_son[i]);
				if (t.m_menu_info.m_menu_string != "@P16148-POLUXDATA")
				{
					t.m_menu_info.m_menu_string = c_du8::convert_du8_string(t.m_menu_info.m_menu_string);
					xx_result.push_back(t.m_menu_info);
				}
			}
			if (m_vec_son[i].m_str_name == "MenuItem")
			{
				c_menu_item menu_item(m_vec_son[i]);
				if (menu_item.m_item_info.m_file_path != "")
				{
					menu_item.m_item_info.m_menu_string = c_du8::convert_du8_string(menu_item.m_item_info.m_menu_string);
					xx_result.push_back(menu_item.m_item_info);
				}
			}
		}
	}
}


c_cel c_menu_screen_cel::move_next()
{
	return c_cel();
}

c_function_subtree::c_function_subtree()
{
}

c_function_subtree::c_function_subtree(c_node tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
	
	m_menu_info.m_file_name = m_map_attr["FileName"];
	m_menu_info.m_file_path = m_vec_son[0].m_map_attr["path"];

	c_label_content tt(m_vec_son[1]);
	m_menu_info.m_menu_string = tt.m_label_name;//m_vec_son[1].get_vec_son()[0].get_vec_son()[0].m_str_value;
}

c_label_content::c_label_content()
{
}

c_label_content::c_label_content(c_node tnode)
{
	m_vec_son = tnode.get_vec_son();
	m_label_name = m_vec_son[0].get_vec_son()[0].m_str_value;
}

c_menu_item::c_menu_item()
{
}

c_menu_item::c_menu_item(c_cel tnode)
{
	vector<c_node>m_vec_son = tnode.get_vec_son();
	unsigned int i = 0;
	for (i = 0;i < m_vec_son.size();i++)
	{
		if (m_vec_son[i].m_str_name == "LabelContent")
		{
			c_label_content label_content(m_vec_son[i]);
			m_item_info.m_menu_string = label_content.m_label_name;
			m_label = label_content.m_label_name;
		}
		if (m_vec_son[i].m_str_name == "TargetCell")
		{
			m_target = c_target_node(m_vec_son[i]);
			string dest_pos = m_target.m_target_pos;
			c_cel t_cel=c_tree::get_cel_by_pos(dest_pos);
			if (t_cel.m_str_name == "SubTree")
			{
				c_sub_tree_cel sub_tree_cel(t_cel);
				m_item_info.m_file_path = sub_tree_cel.m_file_path;
				m_item_info.m_file_name = sub_tree_cel.m_file_name;
				g_tree_info.pop_back();
			}
		}
	}
}

c_not_node::c_not_node()
{
}

c_not_node::c_not_node(c_node& tnode)
{
	m_vec_son = tnode.get_vec_son();
}

bool c_not_node::process_not()
{
	bool result = false;
	if (m_vec_son[0].m_str_name == "DataComparison")
	{
		c_data_comparison_node compar_node = c_data_comparison_node(m_vec_son[0]);
		result = compar_node.process_datacomparison();
		if (result)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		c_log::print("not met node,ecu:%s ,file: %s, descript: %s\n", c_tree::m_current_veh.m_ecu_type.c_str(), c_tree::m_current_veh.m_file_name.c_str(), c_tree::m_current_veh.m_ecu_descript.c_str());
	}
	return result;
}

c_diag_screen::c_diag_screen()
{
}

c_diag_screen::c_diag_screen(c_node tnode)
{
	m_vec_son = tnode.get_vec_son();
	m_scr_name = m_vec_son[0].m_map_attr["ScreenName"];
}
