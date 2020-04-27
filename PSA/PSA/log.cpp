#include "log.h"


FILE* c_log::log_fp = fopen("C:\\Users\\Administrator\\Desktop\\PSA_data\\log\\log.txt", "w");

void c_log::print(const char* format, ...)
{
	va_list _va_list;
	va_start(_va_list, format);
	char szBuf[200]{};
	vsprintf(szBuf, format, _va_list);
	va_end(_va_list);
	fprintf(log_fp, szBuf);
}
