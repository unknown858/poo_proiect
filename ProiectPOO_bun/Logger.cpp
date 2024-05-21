#include "Logger.h"

Logger* Logger::m_ptr = nullptr;

Logger::Logger()
{
	m_log_file.open("log_file.txt", std::ios::out);

	if (!m_log_file.is_open())
	{
		std::cerr << "Error opening log file.\n";
		exit(1);
	}
}

Logger::~Logger()
{
	if (m_log_file.is_open())
	{
		m_log_file.close();
	}
}

char* Logger::get_log_time()
{
	time_t now = time(nullptr);
	tm* time_info = localtime(&now);
	char time_stramp[20];
	strftime(time_stramp, sizeof(time_stramp), "%Y-%m-%d %H:%M:%S", time_info);
	return time_stramp;
}

std::string Logger::level_to_string(log_level level)
{
	switch (level)
	{
	case log_level::DEBUG:return "DEBUG";
		break;
	case log_level::INFO:return "INFO";
		break;
	case log_level::WARNING:return "WARNING";
		break;
	case log_level::ERR:return "ERROR";
		break;
	case log_level::CRITICAL:return "CRITICAL";
		break;
	}
}

void Logger::lgi(log_level level, std::string function_name, std::string msg, std::string additional_info)
{
	std::ostringstream log_entry;
	log_entry << "[" << std::string(get_log_time()) << "] [" << level_to_string(level) << "] [" << function_name << "] : " << msg << "\n";

	if (additional_info != "no")
	{
		log_entry << "\tAdditional information:\n\t\t" << additional_info << "\n";
	}

	if (m_log_file.is_open())
	{
		m_log_file << log_entry.str();
		m_log_file.flush();
	}


}

Logger& Logger::get_instance()
{
	if (m_ptr == nullptr)
		m_ptr = new Logger();

	return *m_ptr;
}

void Logger::destroy_instance()
{
	if (m_ptr != nullptr)
	{
		delete m_ptr;
		m_ptr = nullptr;
	}
}

