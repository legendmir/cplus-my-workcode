#include"tree.h"

vector<int> c_tree::m_vec;
c_tree::c_tree() {
	m_vec.push_back(1);
	m_vec.push_back(2);
}

c_tree::c_tree(vector<int>& vec)
{
	m_vec = vec;
}
