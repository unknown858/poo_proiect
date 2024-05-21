#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <optional>

enum class log_level { DEBUG, INFO, WARNING, ERR, CRITICAL };

class Logger
{
	static Logger* m_ptr;
	std::ofstream m_log_file;

	Logger();
	Logger(const Logger& obj) {};
	~Logger();

	char* get_log_time();
	std::string level_to_string(log_level level);

public:

	void lgi(log_level level, std::string function_name, std::string msg, std::string additional_info = "no");
	static Logger& get_instance();
	static void destroy_instance();
};

