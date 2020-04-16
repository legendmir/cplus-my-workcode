#include "fire_bird.h"


string g_db_path = "G:\\code\\C++\\cplus-my-workcode\\PSA\\PSA\\db_path\\";

char* new_buffer(int size)
{
	char* xx = new char[size + 1];
	memset(xx, 0, size + 1);
	return xx;
}

c_fire_bird::c_fire_bird(string db_name, string sql)
{
	string file_path = g_db_path + db_name+".FDB";
	const char* db_path = file_path.c_str();
	m_db = IBPP::DatabaseFactory("", db_path, "SYSDBA", "masterkey");
	m_db->Connect();
	m_tr = IBPP::TransactionFactory(m_db);
	m_tr->Start();
	m_st = IBPP::StatementFactory(m_db, m_tr);
	m_res = IBPP::BlobFactory(m_db, m_tr);
	m_st->Execute(sql.c_str());
}


vector<map<string, data_unit>> c_fire_bird::get_result_tab()
{
	int i = 0;
	int j = 0;
	vector<map<string, data_unit>>vec_map;
	string data_name;
	data_unit t_unit;
	t_unit.xx_float_value = 0;
	char* p_blob = 0;
	for (i = 0;m_st->Fetch() != 0;i++)
	{
		map<string, data_unit> t_map;
		int xx = m_st->Columns();
		for (j = 0;j < m_st->Columns();j++)
		{
			IBPP::SDT data_type = m_st->ColumnType(j + 1);

			int size = m_st->ColumnSize(j + 1);
			char* pbuf = 0;
			if (data_type == IBPP::sdArray)
			{
				t_unit.data_type = "sdArray";
				pbuf = new_buffer(size);
				m_st->Get(j + 1, pbuf);
				t_unit.data_value = pbuf;
				delete pbuf;
			}
			if (data_type == IBPP::sdBlob)
			{
				t_unit.data_type = "sdBlob";
				m_st->Get(j + 1, m_res);
				m_res->Open();
				int bolb_size = 0;
				m_res->Info(&bolb_size, NULL, NULL);
				p_blob = new_buffer(bolb_size + 1);
				m_res->Read(p_blob, bolb_size);
				t_unit.data_value = p_blob;
				delete p_blob;
				m_res->Close();
			}
			if (data_type == IBPP::sdString)
			{
				t_unit.data_type = "sdString";
				pbuf = new_buffer(size);
				m_st->Get(j + 1, pbuf);
				if (strlen(pbuf) == 0)
					t_unit.data_value = "NULL";
				else
					t_unit.data_value = pbuf;
				delete pbuf;
			}
			if (data_type == IBPP::sdSmallint)
			{
				t_unit.data_type = "sdSmallint";
				short t_short = 0;
				m_st->Get(j + 1, &t_short);
				t_unit.data_value = to_string(t_short);
			}
			if (data_type == IBPP::sdInteger)
			{
				t_unit.data_type = "sdInteger";
				int t_int = 0;
				m_st->Get(j + 1, &t_int);
				t_unit.data_value = to_string(t_int);
			}
			if (data_type == IBPP::sdLargeint)
			{
				t_unit.data_type = "sdLargeint";
				pbuf = new_buffer(size);
				m_st->Get(j + 1, pbuf);
				t_unit.data_value = pbuf;
			}
			if (data_type == IBPP::sdFloat)
			{
				t_unit.data_type = "sdFloat";
				float t_float = 0;
				m_st->Get(j + 1, &t_float);
				t_unit.xx_float_value = t_float;
				t_unit.data_value = to_string(t_float);
			}
			if (data_type == IBPP::sdDouble)
			{
				t_unit.data_type = "sdDouble";
				double tdouble = 0;
				m_st->Get(j + 1, &tdouble);
				t_unit.data_value = to_string(tdouble);
			}

			data_name = m_st->ColumnName(j + 1);
			t_map[data_name] = t_unit;
		}
		vec_map.push_back(t_map);
	}

	return vec_map;
}


namespace fire_bird
{
	vector<map<string, data_unit>> fire_bird::get_sql_result(sql_type st_sql, vector<string> vec_str)
	{
		sql_info xx = g_sqls.sql_map[st_sql];
		string handled_sql = g_sqls.replace_parm(xx.sql, vec_str);
		c_fire_bird veh_result(xx.name, handled_sql);
		return veh_result.get_result_tab();
	}
}