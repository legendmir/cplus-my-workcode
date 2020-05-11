#pragma once
#include<string>
#include<map>
#include<vector>
#include"veh_basic.h"
#include"global_txt.h"
#include"tinyxml2.h"
#include"sqls.h"
#include"fire_bird.h"
#include"path.h"
using namespace std;


class c_xml {
public:
	c_xml(string ecu_name, string veh_name);
public:
	string get_variant_file_name(string ecu_name,string veh_name);
	string m_xml_file_path;
	string m_file_name;
	string m_path;
};

class c_node {
public:
	c_node();
	c_node(int line, string text);
	string get_m_value(string);
	vector<c_node> get_vec_son();

public:
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
	c_cel move_next(vector<menu_info>& xx_result);
	string get_dest();
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
	  virtual c_cel move_next();
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

class c_data_comparison_node :public c_node {
public:
	c_data_comparison_node(c_node& tnode);
	bool process_datacomparison();

public:
	string m_type;
};

class c_act_cel :public c_cel {
public:
	c_act_cel();
	c_act_cel(c_cel& tnode);
	virtual c_cel move_next();
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

class c_source_node :public c_node {
public:
	c_source_node();
	c_source_node(c_node& tnode);
	string get_source();
public:
	string m_compare_value;
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

class c_target_node :public c_node {
public:
	c_target_node();
	c_target_node(c_node& tnode);

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
	c_target_node m_target_node;
	string m_type_name;
};

class c_start_node :public c_cel
{
public:
	c_start_node();
	c_start_node(c_cel& tnode);
	virtual c_cel move_next();
	string m_dest_pos;
};


class c_switch_cel :public c_cel {
public:
	c_switch_cel();
	c_switch_cel(c_cel& tnode);
	virtual c_cel move_next();
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
	c_appl_node(c_node tnode);
public:
	bool process_appl();
};

class c_part_diag_cel :public c_cel {
public:
	c_part_diag_cel();
	c_part_diag_cel(c_cel& tnode);
	virtual c_cel move_next(vector<menu_info>& xx_result);

public:
	c_appl_node m_appl_node;
	c_cel m_cel;
};

class c_global_tree_info {
public:
	vector<c_node> m_vec_node;
	vector<c_cel> m_vec_cel;
	vector<c_part_diag_cel> m_vec_part_diag;
};




class c_function_subtree :public c_node {
public:
	c_function_subtree();
	c_function_subtree(c_node tnode);
public:
	menu_info m_menu_info;
};

class c_menu_screen_cel :public c_cel{
public:
	c_menu_screen_cel();
	c_menu_screen_cel(c_cel tnode, vector<menu_info>& xx_result);
	c_cel move_next();
	vector<c_cel> vec_next_cel;
public:
	int m_mov_flag;
};

class c_label_content :public c_node{
public:
	c_label_content();
	c_label_content(c_node tnode);
	string m_label_name;
};

class c_menu_item :c_node{
public:
	c_menu_item();
	c_menu_item(c_cel tnode);
public:
	string m_label;
	c_target_node m_target;
	menu_info m_item_info;
};

class c_diag_screen :public c_cel{
public:
	c_diag_screen();
	c_diag_screen(c_node tnode);
	string m_scr_name;
};

class c_tree {
public:
	c_tree();
	c_tree(string file_path, string file_name, c_current_veh& current_veh);
	void init_vec_node();
	void init_vec_cel();
	static c_cel get_cel_by_pos(string pos);
	
public:
	void process_tree(vector<menu_info>& xx_result);
	void quick_scan(vector<menu_info>& xx_result);
	void get_vec_scrname(vector<string>&);
public:
	bool is_only_one_menu();
	bool is_script_file();
public:
	c_global_tree_info m_tree_info;
	string m_full_path;
	string m_file_name;
	static c_current_veh m_current_veh ;
};

class c_sub_tree_ex_cel :public c_node {
public:
	c_sub_tree_ex_cel(c_node tnode);
	virtual c_cel move_next();
public:
	c_tree m_tree;
	string m_file_name;
	string m_file_path;
	string m_dest_pos;
};

class c_sub_tree_cel :public c_node {
public:
	c_sub_tree_cel(c_node tnode);
	virtual c_cel move_next();
public:
	c_tree m_tree;
	string m_file_name;
	string m_file_path;
	string m_dest_pos;
};



