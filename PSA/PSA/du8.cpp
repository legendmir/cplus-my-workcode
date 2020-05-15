#include "du8.h"


const char* c_du8::m_trans_path = "F:\\Job Project\\Peugeot\\AWRoot\\dtrd\\trans\\";
const char* c_du8::m_lang_sig = "en_GB";

char* c_du8::m_buffer_POLUXDATA =  get_du8_buffer("POLUXDATA");
char* c_du8::m_buffer_POLUXVEH =   get_du8_buffer("POLUXVEH");
char* c_du8::m_buffer_POLUXXPERT = get_du8_buffer("POLUXXPERT");
char* c_du8::m_buffer_POLUXMES =   get_du8_buffer("POLUXMES");
char* c_du8::m_buffer_ENGINE =     get_du8_buffer("ENGINE");
char* c_du8::m_buffer_POLUXPORT =  get_du8_buffer("POLUXPORT");
char* c_du8::m_buffer_POLUXSPV =   get_du8_buffer("POLUXSPV");
char* c_du8::m_buffer_AWMMI =      get_du8_buffer("AWMMI");
char* c_du8::m_buffer_DBMMI =      get_du8_buffer("DBMMI");
char* c_du8::m_buffer_VEHMNT =     get_du8_buffer("VEHMNT");
char* c_du8::m_buffer_LAUNCHER =   get_du8_buffer("LAUNCHER");

c_du8::c_du8()
{
}

string c_du8::get_string_from_du8(string du8_name, int id)
{
	int start_addr = 0;
	int end_addr = 0;
	unsigned int i = 0;
	string result;
	if (du8_name == "POLUXDATA")
	{
		start_addr = *(int*)(m_buffer_POLUXDATA + 0x64 + 4 * (id - 1));
		end_addr = *(int*)(m_buffer_POLUXDATA + 0x64 + 4 * (id - 1) + 4);
		for (i = start_addr;i < end_addr;i++)
		{
			result.push_back(m_buffer_POLUXDATA[i]);
		}
	}
	if (du8_name == "POLUXVEH")
	{
		start_addr = *(int*)(m_buffer_POLUXVEH + 0x64 + 4 * (id - 1));
		end_addr = *(int*)(m_buffer_POLUXVEH + 0x64 + 4 * (id - 1) + 4);
		for (i = start_addr;i < end_addr;i++)
		{
			result.push_back(m_buffer_POLUXVEH[i]);
		}
	}
	if (du8_name == "POLUXXPERT")
	{
		start_addr = *(int*)(m_buffer_POLUXXPERT + 0x64 + 4 * (id - 1));
		end_addr = *(int*)(m_buffer_POLUXXPERT + 0x64 + 4 * (id - 1) + 4);
		for (i = start_addr;i < end_addr;i++)
		{
			result.push_back(m_buffer_POLUXXPERT[i]);
		}
	}
	if (du8_name == "POLUXMES")
	{
		start_addr = *(int*)(m_buffer_POLUXMES + 0x64 + 4 * (id - 1));
		end_addr = *(int*)(m_buffer_POLUXMES + 0x64 + 4 * (id - 1) + 4);
		for (i = start_addr;i < end_addr;i++)
		{
			result.push_back(m_buffer_POLUXMES[i]);
		}
	}
	if (du8_name == "ENGINE")
	{
		start_addr = *(int*)(m_buffer_ENGINE + 0x64 + 4 * (id - 1));
		end_addr = *(int*)(m_buffer_ENGINE + 0x64 + 4 * (id - 1) + 4);
		for (i = start_addr;i < end_addr;i++)
		{
			result.push_back(m_buffer_ENGINE[i]);
		}
	}
	if (du8_name == "POLUXPORT")
	{
		start_addr = *(int*)(m_buffer_POLUXPORT + 0x64 + 4 * (id - 1));
		end_addr = *(int*)(m_buffer_POLUXPORT + 0x64 + 4 * (id - 1) + 4);
		for (i = start_addr;i < end_addr;i++)
		{
			result.push_back(m_buffer_POLUXPORT[i]);
		}
	}
	if (du8_name == "POLUXSPV")
	{
		start_addr = *(int*)(m_buffer_POLUXSPV + 0x64 + 4 * (id - 1));
		end_addr = *(int*)(m_buffer_POLUXSPV + 0x64 + 4 * (id - 1) + 4);
		for (i = start_addr;i < end_addr;i++)
		{
			result.push_back(m_buffer_POLUXSPV[i]);
		}
	}
	if (du8_name == "AWMMI")
	{
		start_addr = *(int*)(m_buffer_AWMMI + 0x64 + 4 * (id - 1));
		end_addr = *(int*)(m_buffer_AWMMI + 0x64 + 4 * (id - 1) + 4);
		for (i = start_addr;i < end_addr;i++)
		{
			result.push_back(m_buffer_AWMMI[i]);
		}
	}
	if (du8_name == "DBMMI")
	{
		start_addr = *(int*)(m_buffer_DBMMI + 0x64 + 4 * (id - 1));
		end_addr = *(int*)(m_buffer_DBMMI + 0x64 + 4 * (id - 1) + 4);
		for (i = start_addr;i < end_addr;i++)
		{
			result.push_back(m_buffer_DBMMI[i]);
		}
	}
	if (du8_name == "VEHMNT")
	{
		start_addr = *(int*)(m_buffer_VEHMNT + 0x64 + 4 * (id - 1));
		end_addr = *(int*)(m_buffer_VEHMNT + 0x64 + 4 * (id - 1) + 4);
		for (i = start_addr;i < end_addr;i++)
		{
			result.push_back(m_buffer_VEHMNT[i]);
		}
	}
	if (du8_name == "LAUNCHER")
	{
		start_addr = *(int*)(m_buffer_LAUNCHER + 0x64 + 4 * (id - 1));
		end_addr = *(int*)(m_buffer_LAUNCHER + 0x64 + 4 * (id - 1) + 4);
		for (i = start_addr;i < end_addr;i++)
		{
			result.push_back(m_buffer_LAUNCHER[i]);
		}
	}
	return result;
}

string c_du8::convert_du8_string(string str)
{
	if (str[0] != '@')
		return str;
	if (str=="@T")
	{
		return"T";
	}
	unsigned int i = 0;
	vector<string>vec_str = split_buffer(str);
	if (vec_str.size()==1&& vec_str[0][0]=='T')
	{
		return vec_str[0].substr(1, vec_str[0].size());
	}
	vector<string>vec_xxx;
	string result;
	string du8_str;
	for (i = 0;i < vec_str.size();i++)
	{
		if (vec_str[i][0] != 'T'&& vec_str[i][0] != '\\')//vec_str[i].substr(0,2)!="\\*"&& vec_str[i].substr(0, 2)!="\\+"&& vec_str[i].substr(0, 2)!= "\\n")
		{
			vec_xxx = split(vec_str[i], '-');
			if (vec_xxx.size()==2)
			{
				int id = stoi(vec_xxx[0].substr(1, vec_xxx[0].size()));
				if (id != 146035 && id != 304841)
				{
					du8_str = vec_xxx[1];
					du8_str= get_string_from_du8(du8_str, id);
					if (du8_str.size() > 2 && du8_str.substr(du8_str.size() - 2, du8_str.size()) == "*1" && i + 1 < vec_str.size())
					{
						result += du8_str.substr(0, du8_str.size() - 2) + vec_str[i + 1].substr(2, vec_str[i + 1].size());
						i++;
					}
					else
					{
						result += du8_str;
					}
				}
			}
			else
			{
				result += vec_xxx[0];
			}
		}
		else
		{
			if (vec_str[i][0] == 'T')
			{
				result += vec_str[i].substr(1, vec_str[i].size());
			}
			else if(vec_str[i][0] == '\\')
			{
				vec_str[i] = vec_str[i].substr(1, vec_str[i].size());
				if (vec_str[i][0] == '+')
				{
					result += vec_str[i].substr(1, vec_str[i].size());
				}
				else if (vec_str[i][0] == '-')
				{
					result += vec_str[i].substr(1, vec_str[i].size());
				}
				else if (vec_str[i][0] == '*')
				{
					result += vec_str[i].substr(1, vec_str[i].size());
				}
			}
			else
			{
				result += vec_str[i].substr(1, vec_str[i].size());
			}
		}

	}
	return result;
}

char* c_du8::get_du8_buffer(string du8name)
{
	string path = m_trans_path;
	string file_name = path + du8name + m_lang_sig + ".DU8";
	FILE* fp = fopen(file_name.c_str(), "rb");
	fseek(fp, 0, SEEK_END);
	int file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* buffer = new char[file_size];
	fread(buffer, 1, file_size, fp);
	return buffer;
}

vector<string> c_du8::split_buffer(string str)
{
	unsigned int i = 0;
	vector<string>vec_str;
	string temp_str;
	for (i = 0;i < str.size();i++)
	{
		if (str[i] == '@')
		{
			if (temp_str.size())
			{
				vec_str.push_back(temp_str);
			}
			temp_str.clear();
		}
		else
		{
			temp_str.push_back(str[i]);
		}

	}
	if (temp_str.size())
		vec_str.push_back(temp_str);
	return vec_str;
}

vector<string> c_du8::split(string& ori_string, char sig)
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



