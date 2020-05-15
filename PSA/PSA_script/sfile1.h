#pragma once
#include<string>
#include<map>
#include<vector>
using namespace std;



typedef struct menu_info
{
	string menu_string;
	string target_pos;
};

typedef struct msg_info
{
	string ScreenTitle;
	string MsgItem;
	string HelpText;
};

typedef struct ecu_func
{
	string text;
	string buffer_name;
};

enum info_type { type_msg_screen, type_menu_screen, type_src_name, type_ecu_comm, type_sub_tree, type_quit };

typedef struct subtree_info
{
	string file_path;
	string file_name;
};

typedef struct session_info
{
	info_type type;
	string pre_pos;
	string tmp_pos;
	string next_pos;
	vector<menu_info> vecMenuItem;
	msg_info MsgInfo;
	string SrcName;
	ecu_func EcuFunc;
	subtree_info SubTree;
};


class c_path
{
public:
	static string source_path;
	static string xml_file_path;
};


class c_current_veh {
public:
	c_current_veh();
	string m_comtype;
	string m_moudle_name;
	string m_ecu_type;
	string m_ecu_descript;
	string m_file_name;
	string m_file_path;
	map<string, string> map_global_textbuffer;
};


class c_node {
public:
	c_node();
	c_node(int line, string text);
	vector<c_node> get_vec_son();

private:
	string get_m_value(string);
	vector<string> split(string ori_string, char sig);
	string rough_text_handle(string text);
	void insert_map_cel(string);

public:
	int m_line;
	string m_str_name;
	string m_str_value;
	vector<c_node> m_vec_son;
	map<string, string> m_map_attr;
};


enum cel_type { ct_partdiag, ct_normal };
class c_cel :public c_node {
public:
	c_cel();
	c_cel(c_node& tnode);
	c_cel move_next();
public:
	cel_type m_cel_type;
	string m_str_pos;
};

class c_logic_test_node :public c_node {
public:
	c_logic_test_node();
	c_logic_test_node(c_node& tnode);
	bool process_logic_test();
public:
};

class c_math_logic_node :public c_node {
public:
	c_math_logic_node();
	c_math_logic_node(c_node& tnode);
	bool process_math_logic();
};

class c_test_cel :public c_cel {
public:;
	  c_test_cel();
	  c_test_cel(c_cel& tcel);
	  c_cel move_next();
public:
	string m_dest_yes;
	string m_dest_no;
};

class c_for_node :public c_node {
public:
	c_for_node();
	c_for_node(c_node& tnode);
	bool process_for();
};

class c_not_node :public c_node {
public:
	c_not_node();
	c_not_node(c_node& tnode);
	bool process_not();
};

class c_source_node :public c_node {
public:
	c_source_node();
	c_source_node(c_node& tnode);
	string get_source();
public:
	string m_compare_value;
};

class c_data_comparison_node :public c_node //一定有2个source,比较type有EQUAL DIFFERENT之类
{
public:
	c_data_comparison_node(c_node& tnode);
	bool process_datacomparison();
public:
	c_source_node source1;
	c_source_node source2;
	string m_type;
};

class c_act_cel :public c_cel {
public:
	c_act_cel();
	c_act_cel(c_cel& tnode);
	c_cel move_next();
	void process_act();
public:
	string m_dest_pos;
};

class c_text_buffer_node :public c_node {
public:
	c_text_buffer_node(c_node& tnode);
public:
	int m_pos;
	int m_length;
};

class c_destnation_node :public c_node {
public:
	c_destnation_node();
	c_destnation_node(c_node& tnode);

public:
	string m_type_name;
	string m_buffer_name;
};

class c_assignment_node :public c_node {
public:
	c_assignment_node();
	c_assignment_node(c_node& tnode);
	void process_assignment();
public:
	vector<c_node> m_vec_son;
};


class c_ecu_comm_node :public c_node {
public:
	void process_ecu_comm();
	c_ecu_comm_node(c_node& tnode);
	vector<c_node> m_vec_son;
};

class c_ecu_function_call_node :public c_node {
public:
	c_ecu_function_call_node(c_node& tnode);
	void process_ecu_function();
	vector<c_node>m_vec_son;
	string m_name;
	string m_function_parm;
};

class c_target_cel :public c_node {
public:
	c_target_cel();
	c_target_cel(c_node& tnode);

	c_cel m_target_cel;
	string m_target_pos;
};

class c_DefaultSwitchTarget :public c_node
{
public:
	c_DefaultSwitchTarget();
	c_DefaultSwitchTarget(c_node& tnode);
	string m_default_pos;
};

class c_switch_test_node :public c_node
{
public:
	c_switch_test_node();
	c_switch_test_node(c_node& tnode);
	c_source_node m_source;
	c_target_cel m_target_node;
	string m_type_name;
};

class c_start_node :public c_cel
{
public:
	c_start_node();
	c_start_node(c_cel& tnode);
	c_cel move_next();
	string m_dest_pos;
};


class c_switch_cel :public c_cel {
public:
	c_switch_cel();
	c_switch_cel(c_cel& tnode);
	c_cel move_next();
	string process_switch();

public:
	vector<c_node>m_vec_son;
	c_source_node m_source;
	vector<c_switch_test_node> m_vec_switch_node;
	c_DefaultSwitchTarget m_default_target_node;
};


class c_appl_node :public c_node {
public:
	c_appl_node();
	c_appl_node(c_node& tnode);
public:
	bool process_appl();
};


class c_part_diag_cel :public c_cel {
public:
	c_part_diag_cel();
	c_part_diag_cel(c_cel& tnode);
	c_cel move_next();

public:
	c_appl_node m_appl_node;
	c_cel m_cel;
};

class c_function_subtree :public c_node {
public:
	c_function_subtree();
	c_function_subtree(c_node& tnode);
public:
	string m_file_path;
	string m_file_name;
	string m_menu_string;
};

class c_menu_item :c_node {
public:
	c_menu_item();
	c_menu_item(c_node& tnode);
public:
	string m_label;
	string m_target_pos;
};

class c_menu_screen_cel :public c_cel {
public:
	c_menu_screen_cel();
	c_menu_screen_cel(c_cel& tnode);
	c_cel move_next();
public:
	vector<c_function_subtree>m_vec_func_tree;
	vector<c_menu_item>m_vec_menu_item;
	string m_pos;
private:
	string m_dest_pos;
};

class c_label_content :public c_node {
public:
	c_label_content();
	c_label_content(c_node& tnode);
	string m_label_name;
};


class c_diag_screen :public c_cel {
public:
	c_diag_screen();
	c_diag_screen(c_node& tnode);
	c_cel move_next();
	string m_scr_name;
	string m_dest_pos;
};


class c_msg_item :public c_node
{
public:
	c_msg_item();
	c_msg_item(c_node& tnode);

	string m_str_label;
};

class c_Screen_Title:public c_node
{
public:
	c_Screen_Title();
	c_Screen_Title(c_node& tnode);
	string m_str_label;
};

class c_Paragraph :public c_node
{
public:
	c_Paragraph();
	c_Paragraph(c_node& tnode);
	string m_str_value;
};

class c_HelpText :public c_node
{
public:
	c_HelpText();
	c_HelpText(c_node tnode);
	string help_text;
};

class c_msg_screen:public c_node
{
public:
	c_msg_screen();
	c_msg_screen(c_node& tnode);
	c_cel move_next();
public:
	
	string m_title_text;
	string m_help_text;
	string m_msg_text;

	string m_pos;
	string m_dest_pos;
};

class c_global_tree_info {//全局tree信息
public:
	vector<c_node> m_vec_node;
	vector<c_cel> m_vec_cel;
	vector<c_part_diag_cel> m_vec_part_diag;
	map<string, string> m_map_textbuffer;
};

class c_tree {
public:
	c_tree();
	c_tree(string file_path, string file_name, c_current_veh& current_veh);
	static c_cel get_cel_by_pos(string pos);
public:
	void run_script();
private:
	void init_vec_node();
	void init_vec_cel();
public:
	c_global_tree_info m_tree_info;
	string m_full_path;
	string m_file_name;
	static c_current_veh m_current_veh;
	session_info m_session_info;
};

class c_sub_tree_cel :public c_node {
public:
	c_sub_tree_cel(c_node tnode);
	 c_cel move_next();
public:
	string m_file_name;
	string m_file_path;
	string m_pos;
	string m_dest_pos;
};



