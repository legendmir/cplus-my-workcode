#include "sfile1.h"

vector<c_global_tree_info> g_tree_info_stack;
c_current_veh c_tree::m_current_veh;

string c_path::source_path = "F:\\Job Project\\Peugeot\\AWRoot\\dtrd\\tree\\";
string c_path::xml_file_path = "F:\\Job Project\\Peugeot\\AWRoot\\dtrd\\database\\xml\\contextualisation.xml";

c_current_veh::c_current_veh()
{
}

std::string& replace_all_vec(std::string& str, std::string& old_value, std::vector<std::string>& vec_new_value)//字符串替换
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

vector<string> c_node::split(string ori_string, char sig)//字符串按sig符号('='之类)拆解
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
			tstr = "";
		}
		else
		{
			tstr += ori_string[i];
		}
	}
	vec_str.push_back(tstr);
	return vec_str;
}

string c_node::rough_text_handle(string text)//拆解xml文件每行的<>
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
		tstr+=text[i];
	}
	return tstr;
}
c_node::c_node()
{
}
c_node::c_node(int line, string text)//初始化节点
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

string c_node::get_m_value(string text)//获取节点后面的字符串<Comment>Fin de gamme  "Fin de gamme"为获取的内容
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

vector<c_node> c_node::get_vec_son()//获取子节点vector
{
	unsigned int i = 0;
	vector<c_node>vec_son;
	c_node son = g_tree_info_stack.back().m_vec_node[m_line + 1];
	if (son.m_str_name == '/' + m_str_name)
	{
		return vec_son;
	}
	vec_son.push_back(son);
	for (i = m_line + 2;i < g_tree_info_stack.back().m_vec_node.size();i++)
	{
		if (g_tree_info_stack.back().m_vec_node[i].m_str_name == "/" + son.m_str_name &&
			g_tree_info_stack.back().m_vec_node[i + 1].m_str_name != m_str_name)
		{
			if (g_tree_info_stack.back().m_vec_node[i + 1].m_str_name == "/" + m_str_name)
			{
				break;
			}
			son = g_tree_info_stack.back().m_vec_node[i + 1];
			vec_son.push_back(son);
		}
	}
	return vec_son;
}

void c_node::insert_map_cel(string tstr)//获取节点属性
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
	if (m_str_name == "PartDiag")//节点分为2类 ct_partdiag 和ct_normal
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
c_cel c_cel::move_next()//部分节点move
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
		dest_cel = part_diag_cel.move_next();
	}
	else if (m_str_name == "Start")
	{
		c_start_node start_cel(*this);
		dest_cel = start_cel.move_next();
	}
	return dest_cel;
}


c_test_cel::c_test_cel()
{
}

c_test_cel::c_test_cel(c_cel& tcel)//构造
{
	m_line = tcel.m_line;
	m_map_attr = tcel.m_map_attr;
	m_str_name = tcel.m_str_name;
	m_str_value = tcel.m_str_value;
	m_vec_son = tcel.m_vec_son;

	m_dest_yes = tcel.m_map_attr["destyesc"] + "_" + tcel.m_map_attr["destyesl"];
	m_dest_no = tcel.m_map_attr["destnoc"] + "_" + tcel.m_map_attr["destnol"];
}

c_cel c_test_cel::move_next()//Test节点move
{
	bool test_result = true;
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

c_math_logic_node::c_math_logic_node(c_node& tnode)//构造
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
}

bool c_math_logic_node::process_math_logic()//处理math_logic,子节点有LogicalTest和Assignment并存
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

c_logic_test_node::c_logic_test_node(c_node& tnode)//构造
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
}

bool c_logic_test_node::process_logic_test()//logic_test节点,子节点是多个For节点组成
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

bool c_for_node::process_for()//处理for节点,子节点包含有NOT,DataComparison,两种类型,这里属于有点绕的
{
	bool xx = true;
	unsigned int i = 0;
	for (i = 0;i < m_vec_son.size();i++)
	{
		if (m_vec_son[i].m_str_name == "NOT")
		{
			c_not_node not_node = c_not_node(m_vec_son[i]);
			if (!not_node.process_not())
			{
				xx = false;
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


c_data_comparison_node::c_data_comparison_node(c_node& tnode)//data_comparison节点,一定是有2个source节点,比较type有EQUAL DIFFERENT之类
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();

	source1 = c_source_node(m_vec_son[0]);
	source2 = c_source_node(m_vec_son[1]);
	m_type = tnode.m_map_attr["type"];//比较类型
}

bool c_data_comparison_node::process_datacomparison()//处理data_comparison 节点
{
	unsigned int i = 0;
	unsigned int flag = 0;
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
			//特殊处理source1是RECONNU(中午意思"认可的"),source2为OUI(中文意思"是的")则返回true
		{
			if (!strcmp(source2.m_compare_value.c_str(), "OUI"))
				return true;
			else
				return false;
		}

		if (!strcmp(source1.m_compare_value.c_str(), "VCHK") || !strcmp(source2.m_compare_value.c_str(), "VCHK"))
			//如果source1是VCHK直接返回true
		{
			return true;
		}
		if (!strcmp(source1.m_str_name.c_str(), "FloatNumber") || !strcmp(source2.m_compare_value.c_str(), "0"))
			//如果source1是FloatNumber直接返回false
		{
			return false;
		}
		if (!strcmp(source1.m_compare_value.c_str(), source2.m_compare_value.c_str()))
			//直接比较compare_value
		{
			return true;
		}
	}
	if (!strcmp(m_type.c_str(), "DIFFERENT"))
		//如果是DIFFERNT,暂时遇到的都是直接compare_value比较
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
	if (source_name == "VehicleData")//如果source名是VehicleData,则直接从m_current_veh中取m_moudle_name赋予m_compare_value
	{
		m_compare_value = c_tree::m_current_veh.m_moudle_name;
	}
	if (source_name == "ASCIIString")//如果source名是ASCIIString,直接取值赋予m_compare_value
	{
		m_compare_value = m_vec_son[0].m_map_attr["value"];
	}
	if (source_name == "FloatVar")//如果source名是FloatVar,直接取值赋予m_compare_value
	{
		m_compare_value = m_vec_son[0].m_map_attr["name"];
	}
	if (source_name == "TextBuffer")//如果是TextBuffer相较更复杂,有带pos和不带pos 的情况
	{
		c_text_buffer_node text_buffer_node(g_tree_info_stack.back().m_vec_node[m_vec_son[0].m_line]);
		string text_name = text_buffer_node.m_map_attr["name"];
		string text_buf = g_tree_info_stack.back().m_map_textbuffer[text_name];
		if (text_buffer_node.m_vec_son.size() == 0)//不带pos直接取值
		{
			m_compare_value = text_buf;
		}
		else//带pos要根据pos和length取值
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
	if (source_name == "GlobalTextBuffer")//全局的Buffer,需要保存到m_current_veh.map_global_textbuffer中
	{
		string gbuffer_name = m_vec_son[0].m_map_attr["name"];
		if (!c_tree::m_current_veh.map_global_textbuffer.count(gbuffer_name))
		{
			//c_log::print("get global buffer failed, file : %s, ecu :%s buffer_name ,:%s\n", c_tree::m_current_veh.m_file_name.c_str(), c_tree::m_current_veh.m_ecu_type.c_str(), gbuffer_name.c_str());
		}
		else
		{
			m_compare_value = c_tree::m_current_veh.map_global_textbuffer[gbuffer_name];
		}
	}
	if (source_name == "ECUData")//如果是ECUData,则ParamName基本是C,一定是获取m_current_veh.m_ecu_type;
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
	//以下是在原厂代码中找到的所有的类型,暂时没遇到
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

c_text_buffer_node::c_text_buffer_node(c_node& tnode)//TextBuffer 节点处理,分为有pos,和没有pos两种情况
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
			c_node position_node = g_tree_info_stack.back().m_vec_node[m_vec_son[i].m_line + 1];
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

c_act_cel::c_act_cel(c_cel& tnode)//构造
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

void c_act_cel::process_act()//处理Act节点,下方通常有ECUCommunication节点和MathAndLogic节点
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

c_DefaultSwitchTarget::c_DefaultSwitchTarget(c_node& tnode)//默认switch目标
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

c_switch_cel::c_switch_cel(c_cel& tnode)//构造
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

c_cel c_switch_cel::move_next()//switch 的move
{
	string result_pos = process_switch();
	return c_tree::get_cel_by_pos(result_pos);
}

string c_switch_cel::process_switch()//处理Switch,基本都是判断source 与destination 相等("EQUAL")的情况
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

c_switch_test_node::c_switch_test_node(c_node& tnode)//Switch节点构造
{
	m_vec_son = tnode.get_vec_son();

	m_source = c_source_node(m_vec_son[0]);
	m_target_node = c_target_cel(m_vec_son[1]);
	m_type_name = tnode.m_map_attr["type"];
}

c_sub_tree_cel::c_sub_tree_cel(c_node tnode)//SubTree 节点构造
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();

	m_dest_pos = m_map_attr["destc"] + "_" + m_map_attr["destl"];
	m_pos= m_map_attr["posc"] + "_" + m_map_attr["posl"];
	m_file_name = m_map_attr["FileName"];
	m_file_path = m_vec_son[0].m_map_attr["path"];
}

c_cel c_sub_tree_cel::move_next()//根据pos mov
{
	return c_tree::get_cel_by_pos(m_dest_pos);
}

c_ecu_function_call_node::c_ecu_function_call_node(c_node& tnode)//初始化
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
}

void c_ecu_function_call_node::process_ecu_function()
{
	//处理ecu_function,如果name 属性为 ECU_DESCRIPTOR,
	//则将c_tree::m_current_veh.m_ecu_descript赋值给Destination节点里面的buffer;
	if (!strcmp(m_map_attr["name"].c_str(), "ECU_DESCRIPTOR"))
	{
		for (size_t i = 0; i < m_vec_son.size(); i++)
		{
			if (m_vec_son[i].m_str_name == "Destination")
			{
				c_destnation_node dest_node(m_vec_son[i]);
				g_tree_info_stack.back().m_map_textbuffer[dest_node.m_buffer_name] = c_tree::m_current_veh.m_ecu_descript;
			}
		}
	}
}

c_assignment_node::c_assignment_node()
{

}

c_assignment_node::c_assignment_node(c_node& tnode)//赋值节点构造
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
}

void c_assignment_node::process_assignment()//处理赋值节点
{
	c_source_node tsource(m_vec_son[0]);
	c_destnation_node tdestination(m_vec_son[1]);
	g_tree_info_stack.back().m_map_textbuffer[tdestination.m_buffer_name] = tsource.m_compare_value;
	if (tdestination.m_type_name == "GlobalTextBuffer")
	{
		c_tree::m_current_veh.map_global_textbuffer[tdestination.m_buffer_name] = tsource.m_compare_value;
	}
}

c_destnation_node::c_destnation_node()
{

}

c_destnation_node::c_destnation_node(c_node& tnode)//处理destnation节点
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();

	m_type_name = m_vec_son[0].m_str_name;
	m_buffer_name = m_vec_son[0].m_map_attr["name"];
}

void c_ecu_comm_node::process_ecu_comm()//处理ecu_comm节点,如果ecu_comm节点下面有ecu_function_call节点,则处理ecu_func call;
{
	if (m_vec_son[0].m_str_name == "ECUFunctionCall")
	{
		c_ecu_function_call_node tfunc_call(m_vec_son[0]);
		tfunc_call.process_ecu_function();
	}
}

c_ecu_comm_node::c_ecu_comm_node(c_node& tnode)//构造
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
}

c_target_cel::c_target_cel()
{
}

c_target_cel::c_target_cel(c_node& tnode)//target(目标)节点,有一个目标坐标
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_target_pos = m_map_attr["destc"] + "_" + m_map_attr["destl"];
	m_target_cel = c_tree::get_cel_by_pos(m_target_pos);
}

c_appl_node::c_appl_node()
{
}

c_appl_node::c_appl_node(c_node& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
}

bool c_appl_node::process_appl()//处理appl 节点,附带逻辑判断,位于PartDiag节点内和MenuItem节点内,子节点全是For节点
{
	bool result = false;
	for (unsigned int i = 0;i < m_vec_son.size();i++)
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

c_part_diag_cel::c_part_diag_cel(c_cel& tnode)//part_diag 节点包含一个appl 和cel(通常是menuscreen)
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

c_cel c_part_diag_cel::move_next()
{
	return m_cel.move_next();
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

c_tree::c_tree(string file_path, string file_name, c_current_veh& current_veh)//构造tree,初始化基本结构(node,cel)
{
	m_current_veh = current_veh;
	m_full_path = c_path::source_path + "\\" + file_path.c_str() + "\\" + file_name.c_str();
	m_file_name = file_name;
	init_vec_node();//初始化node
	g_tree_info_stack.push_back(m_tree_info);
	init_vec_cel();//初始化cel
	g_tree_info_stack.pop_back();
	g_tree_info_stack.push_back(m_tree_info);
}

void c_tree::init_vec_node()//读文件,初始化vector<node>
{
	FILE* fp = fopen(m_full_path.c_str(), "rb");
	fseek(fp, 0, SEEK_END);
	unsigned int len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buffer = new char[len];
	fread(buffer, 1, len, fp);
	fclose(fp);
	unsigned int i = 0;
	unsigned int line_start = 0;
	unsigned int line_cnt = 0;
	char* tbuf = 0;
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
				c_node tnode(line_cnt++, tstring);
				m_tree_info.m_vec_node.push_back(tnode);
			}
		}
		i++;
	}
}

void c_tree::init_vec_cel()//初始化 vector<cel>
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

c_cel c_tree::get_cel_by_pos(string pos)//根据cel节点的pos获取目标cel, pos可能位于,PartDiag节点之中
{
	unsigned int i = 0;
	c_cel dest_cel;
	int flag = 1;

	for (i = 0;i < g_tree_info_stack.back().m_vec_cel.size();i++)
		//先查找是否是cel坐标
	{
		string xx = g_tree_info_stack.back().m_vec_cel[i].m_str_pos;
		if (!strcmp(pos.c_str(), g_tree_info_stack.back().m_vec_cel[i].m_str_pos.c_str()))
		{
			flag = 0;
			dest_cel = g_tree_info_stack.back().m_vec_cel[i];
			break;
		}
	}
	if (flag)
	{
		//不是cel坐标,在查找PartDiag节点, 通常是有两个以上的PartDiag节点有相同的坐标,这个过程需要先处理appl判断,然后确定最终的坐标
		for (i = 0;i < g_tree_info_stack.back().m_vec_part_diag.size();i++)
		{
			c_part_diag_cel part_diag_cel(g_tree_info_stack.back().m_vec_part_diag[i]);
			if (part_diag_cel.m_cel.m_str_pos == pos)
			{
				if (part_diag_cel.m_appl_node.process_appl() == true)//判断appl条件是否满足
				{
					dest_cel = part_diag_cel.m_cel;
					break;
				}
			}
		}
	}
	return dest_cel;
}

void c_tree::run_script()
{
	c_cel pre_cel;
	c_cel next_cel;
	if (m_session_info.tmp_pos=="")
	{
		pre_cel = m_tree_info.m_vec_cel[0];
	}
	else
	{
		pre_cel = c_tree::get_cel_by_pos(m_session_info.tmp_pos);
	}
	next_cel = pre_cel;

	while (1)
	{
		if (next_cel.m_str_name == "End")
		{
			m_session_info.type = type_quit;
			return;
		}

		if (next_cel.m_str_name == "MenuScreen")
		{

			c_menu_screen_cel menu_screen(next_cel);
			for (size_t j = 0; j < menu_screen.m_vec_menu_item.size(); j++)
			{
				menu_info tt;
				tt.menu_string = menu_screen.m_vec_menu_item[j].m_label;
				tt.target_pos = menu_screen.m_vec_menu_item[j].m_target_pos;
				m_session_info.vecMenuItem.push_back(tt);//需要显示的item菜单,且选择之后需要跳转
			}
			m_session_info.type = type_menu_screen;
			m_session_info.pre_pos = pre_cel.m_str_pos;
			m_session_info.tmp_pos = menu_screen.m_pos;
			m_session_info.next_pos = "";

			return;
			//以上循环获取菜单,并显示
			//根据所选菜单进行下一步操作
		}
		else if (next_cel.m_str_name == "MsgScreen")
		{

			//获取,msg信息
			c_msg_screen msg_screen = c_msg_screen(next_cel);
			msg_info xx;
			xx.HelpText = msg_screen.m_help_text;
			xx.MsgItem = msg_screen.m_msg_text;
			xx.ScreenTitle = msg_screen.m_title_text;

			m_session_info.type = type_msg_screen;
			m_session_info.MsgInfo = xx;
			m_session_info.pre_pos = pre_cel.m_str_pos;
			m_session_info.tmp_pos = msg_screen.m_pos;
			m_session_info.next_pos = msg_screen.m_dest_pos;
			return;
	
		}
		else if (next_cel.m_str_name == "DiagScreen")
		{
			//获取到的数据流组id
			c_diag_screen xx = c_diag_screen(next_cel);
			m_session_info.type = type_src_name;
			m_session_info.SrcName= xx.m_scr_name;
			m_session_info.pre_pos = pre_cel.m_str_pos;
			m_session_info.tmp_pos = xx.m_str_pos;
			m_session_info.next_pos = xx.m_dest_pos;
			return;
		}
		else if (next_cel.m_str_name == "Act")
		{
			//后续发命令功能
			c_act_cel act_cel(next_cel);
	/*		info.type = type_ecu_comm;
			info.EcuFunc.text =;
			info.pre_pos = pre_cel.m_str_pos;
			info.tmp_pos = act_cel.m_str_pos;
			info.next_pos = act_cel.m_dest_pos;*/
			pre_cel = next_cel;
			next_cel = act_cel.move_next();
		}
		else if (next_cel.m_str_name == "SubTree")
		{
			c_sub_tree_cel subtree(next_cel);
			if (
				subtree.m_file_name != "tlch_matrix.s" &&
				subtree.m_file_name != "MATRIX_TELEXX_CTX.s" &&
				subtree.m_file_name != "script_tokenizer.s"
				//闭掉几个共有s文件的访问,位于Vehicle/gen/contextualisation_gen.xml
				)
			{
				subtree_info xx;
				xx.file_name = subtree.m_file_name;
				xx.file_path = subtree.m_file_path;
				m_session_info.type = type_sub_tree;
				m_session_info.SubTree = xx;
				m_session_info.pre_pos = pre_cel.m_str_pos;
				m_session_info.tmp_pos = subtree.m_pos;
				m_session_info.next_pos = subtree.m_dest_pos;
				return;
			}
		}
		else
		{
			pre_cel = next_cel;
			next_cel = pre_cel.move_next();
		}
	}
}


c_menu_screen_cel::c_menu_screen_cel()
{

}

c_menu_screen_cel::c_menu_screen_cel(c_cel& tnode)
{
	m_line = tnode.m_line;
	m_map_attr = tnode.m_map_attr;
	m_str_name = tnode.m_str_name;
	m_str_value = tnode.m_str_value;
	m_vec_son = tnode.get_vec_son();
	m_dest_pos = tnode.m_map_attr["destc"] + "_" + tnode.m_map_attr["destl"];
	m_pos = tnode.m_map_attr["posc"] + "_" + tnode.m_map_attr["posl"];
	unsigned int i = 0;
	int item_cnt = 0;

	for (i = 0;i < m_vec_son.size();i++)
	{
		if (m_vec_son[i].m_str_name == "FunctionSubTree")
		{
			m_vec_func_tree.push_back(c_function_subtree(m_vec_son[i]));
		}
		if (m_vec_son[i].m_str_name == "MenuItem")
		{
			m_vec_menu_item.push_back(c_menu_item(m_vec_son[i]));
		}
	}

}

c_cel c_menu_screen_cel::move_next()
{
	return c_tree::get_cel_by_pos(m_dest_pos);
}

c_function_subtree::c_function_subtree()
{
}

c_function_subtree::c_function_subtree(c_node& tnode)
{
	m_map_attr = tnode.m_map_attr;
	m_vec_son = tnode.get_vec_son();


	m_file_name = m_map_attr["FileName"];
	m_file_path = m_vec_son[0].m_map_attr["path"];
	m_menu_string = c_label_content(m_vec_son[1]).m_label_name;
}

c_label_content::c_label_content()
{
}

c_label_content::c_label_content(c_node& tnode)//label_content节点通常位于MenuItem节点个MsgScreen节点之中
{
	m_vec_son = tnode.get_vec_son();
	m_label_name = m_vec_son[0].get_vec_son()[0].m_str_value;//根据观察m_label_name处于该位置(m_vec_son[0].get_vec_son()[0]);
}

c_menu_item::c_menu_item()
{
}

c_menu_item::c_menu_item(c_node& tnode)
{
	vector<c_node>m_vec_son = tnode.get_vec_son();
	unsigned int i = 0;
	for (i = 0;i < m_vec_son.size();i++)
	{
		if (m_vec_son[i].m_str_name == "LabelContent")
		{
			m_label = c_label_content(m_vec_son[i]).m_label_name;
		}
		if (m_vec_son[i].m_str_name == "TargetCell")
		{
			m_target_pos = c_target_cel(m_vec_son[i]).m_target_pos;
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

bool c_not_node::process_not()//处理NOT节点
{
	bool result = false;
	if (m_vec_son[0].m_str_name == "DataComparison")//通常NOT节点的子节点是DataComparison,有例外
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
		//c_log::print("not met node,ecu:%s ,file: %s, descript: %s\n", c_tree::m_current_veh.m_ecu_type.c_str(), c_tree::m_current_veh.m_file_name.c_str(), c_tree::m_current_veh.m_ecu_descript.c_str());
	}
	return result;
}

c_diag_screen::c_diag_screen()
{
}

c_diag_screen::c_diag_screen(c_node& tnode)//DiagScreen节点,即数据流组id
{
	m_vec_son = tnode.get_vec_son();
	m_scr_name = m_vec_son[0].m_map_attr["ScreenName"];
	m_dest_pos= tnode.m_map_attr["destc"] + "_" + tnode.m_map_attr["destl"];
}

c_cel c_diag_screen::move_next()
{
	return c_tree::get_cel_by_pos(m_dest_pos);
}

c_msg_screen::c_msg_screen()
{
}

c_msg_screen::c_msg_screen(c_node& tnode)
{
	m_vec_son = tnode.get_vec_son();
	m_dest_pos = tnode.m_map_attr["destc"] + "_" + tnode.m_map_attr["destl"];
	m_pos= tnode.m_map_attr["posc"] + "_" + tnode.m_map_attr["posl"];

	for (size_t i = 0; i < m_vec_son.size(); i++)
	{
		if (m_vec_son[i].m_str_name=="MsgItem")
		{
			m_msg_text = c_msg_item(m_vec_son[i]).m_str_label;
		}
		if (m_vec_son[i].m_str_name=="ScreenTitle")
		{
			m_title_text = c_Screen_Title(m_vec_son[i]).m_str_label;
		}
		if (m_vec_son[i].m_str_name == "HelpText")
		{
			m_help_text =c_HelpText(m_vec_son[i]).help_text ;
		}
	}
}

c_cel c_msg_screen::move_next()
{
	return c_tree::get_cel_by_pos(m_dest_pos);
}

c_msg_item::c_msg_item()
{
}

c_msg_item::c_msg_item(c_node& tnode)
{
	m_vec_son = tnode.get_vec_son();
	for (size_t i = 0; i < m_vec_son.size(); i++)
	{
		if (m_vec_son[i].m_str_name=="LabelContent")
		{
			m_str_label = c_label_content(m_vec_son[i]).m_label_name;
		}
	}
}

c_Screen_Title::c_Screen_Title()
{
}

c_Screen_Title::c_Screen_Title(c_node& tnode)
{
	m_vec_son = tnode.get_vec_son();
	for (size_t i = 0; i < m_vec_son.size(); i++)
	{
		if (m_vec_son[i].m_str_name == "LabelContent")
		{
			m_str_label = c_label_content(m_vec_son[i]).m_label_name;
		}
	}
}

c_Paragraph::c_Paragraph()
{
}

c_Paragraph::c_Paragraph(c_node& tnode)
{
	for (size_t i = 0; i < m_vec_son.size(); i++)
	{
		if (m_vec_son[i].m_str_name == "ThesauRequest")
		{
			m_str_value = m_vec_son[i].m_str_value;
		}
	}
}

c_HelpText::c_HelpText()
{
}

c_HelpText::c_HelpText(c_node tnode)
{
	m_vec_son = tnode.get_vec_son();
	for (size_t i = 0; i < m_vec_son.size(); i++)
	{
		if (m_vec_son[i].m_str_name == "Paragraph")
		{
			help_text = c_Paragraph(m_vec_son[i]).m_str_value;
		}
	}
}
