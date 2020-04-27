#pragma once
#include<stdio.h>
#include<string>
#include<windows.h>
class c_log {
public:
	static void print(const char* format, ...);
	static FILE* log_fp;
};