#include "Database.h"

std::string Database::get_SQL_error(unsigned int handle_type, SQLHANDLE& handle)
{
	SQLWCHAR SQL_state[1024];
	SQLINTEGER error_code;
	SQLWCHAR msg[1024];


	SQLGetDiagRec(handle_type, handle, 1, SQL_state, &error_code, msg, 1024, NULL);


	std::wstring state(SQL_state, SQL_state + 5);		//wide unicode char
	std::string error_msg = "  Error details :\n";
	error_msg += "\t\t- SQL state: " + std::string(state.begin(), state.end());
	error_msg += "\n\t\t- error code: " + std::to_string(error_code);
	std::wstring msg_w(msg, msg + std::wcslen(msg));
	error_msg += "\n\t\t- error msg: " + std::string(msg_w.begin(), msg_w.end());

	return error_msg;
}

void Database::connect(const std::string& db_info, const std::string& db_name)
{
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQL_env_handle))
		Logger::get_instance().lgi(log_level::CRITICAL, "connect", "Failed to allocate environment handle\n " + get_SQL_error(SQL_HANDLE_DBC, SQL_env_handle));

	if (SQL_SUCCESS != SQLSetEnvAttr(SQL_env_handle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
		Logger::get_instance().lgi(log_level::CRITICAL, "connect", "Failed to set environment attribute on environment handle \n " + get_SQL_error(SQL_HANDLE_DBC, SQL_env_handle));

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SQL_env_handle, &SQL_connection_handle))
		Logger::get_instance().lgi(log_level::CRITICAL, "connect", "Failed to allocate connection handle using environment handle\n " + get_SQL_error(SQL_HANDLE_DBC, SQL_connection_handle));

	SQLWCHAR retConString[1024];

	std::wstring connection_str = L"DRIVER={SQL Server};SERVER=" + std::wstring(db_info.begin(), db_info.end()) + L";DATABASE=" + std::wstring(db_name.begin(), db_name.end()) + L";Trusted=true;";

	switch (SQLDriverConnect(SQL_connection_handle, NULL, (SQLWCHAR*)connection_str.c_str(), SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT))
	{
	case SQL_SUCCESS:
		Logger::get_instance().lgi(log_level::INFO, "connect", "Database connected successfully!");
		break;
	case SQL_SUCCESS_WITH_INFO:
		Logger::get_instance().lgi(log_level::INFO, "connect", "Database connected successfully!");
		break;
	default:
		Logger::get_instance().lgi(log_level::CRITICAL, "connect", "Failed to connect to database using connection handle \n\t\t" + get_SQL_error(SQL_HANDLE_DBC, SQL_connection_handle));

	}
}

std::string Database::get_ads_nr()
{
	std::vector < std::vector<std::string>>results;
	//fetch_data("SELECT COUNT(*) AS NumarAnunturi FROM Anunt", results);
	fetch_data("SELECT MAX(Id) AS MaxId FROM Anunt;", results);
	return results[0][0];
}

std::string Database::get_image_number(char *ad_nr)
{
	std::vector < std::vector<std::string>>results;
	fetch_data("SELECT COUNT(*) AS Numar_imagini FROM Imagine WHERE [Id anunt] ='"+std::string(ad_nr) + "';", results);
	return results[0][0];
}

bool Database::execute_query(const std::string query)
{
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SQL_connection_handle, &SQL_statement_handle))
	{
		Logger::get_instance().lgi(log_level::ERR, "execute_query", "Failed to allocate statement handle for connection\n" + get_SQL_error(SQL_HANDLE_DBC, SQL_connection_handle)), query;

		return 1;
	}

	std::wstring query_w = L"" + std::wstring(query.begin(), query.end());


	//if (SQL_SUCCESS != SQLExecDirect(SQL_statement_handle, (SQLWCHAR*)query_w.c_str(), SQL_NTS))
	//{
	//	Logger::get_instance().lgi(log_level::ERR, "execute_query", "Failed to execute query\n" + get_SQL_error(SQL_HANDLE_STMT, SQL_statement_handle), query);
	//	return 1;
	//}

	auto ret_code = SQLExecDirect(SQL_statement_handle, (SQLWCHAR*)query_w.c_str(), SQL_NTS);

	if (ret_code != SQL_SUCCESS && ret_code != SQL_SUCCESS_WITH_INFO)
	{
		Logger::get_instance().lgi(log_level::ERR, "execute_query", "Failed to execute query\n" + get_SQL_error(SQL_HANDLE_STMT, SQL_statement_handle));
		SQLFreeHandle(SQL_HANDLE_STMT, SQL_statement_handle); 
		return 1;
	}

	//SQLFreeHandle(SQL_HANDLE_STMT, SQL_statement_handle);

	return 0;
}

bool Database::fetch_data(const std::string &query, std::vector<std::vector<std::string>>& result)
{
	if (execute_query(query))
	{
		Logger::get_instance().lgi(log_level::ERR, "fetch_data", "Failed fetching data", query);
		return 1;
	}

	SQLSMALLINT num_cols = 0;
	SQLNumResultCols(SQL_statement_handle, &num_cols);

	while (SQLFetch(SQL_statement_handle) == SQL_SUCCESS)
	{
		std::vector<std::string>row;

		for (SQLSMALLINT i = 1; i <= num_cols; i++)
		{
			SQLCHAR buffer[SQL_RESULT_LEN];
			SQLLEN buff_len = 0;

			SQLLEN indicator = 0;

			if (SQL_SUCCESS != SQLGetData(SQL_statement_handle, i, SQL_C_CHAR, buffer, SQL_RESULT_LEN, &indicator))
			{
				Logger::get_instance().lgi(log_level::ERR, "fetch_data", "Failed fetching data\n" + get_SQL_error(SQL_HANDLE_STMT, SQL_statement_handle), query);
				return 1;
			}

			if (indicator == SQL_NULL_DATA)
			{
				row.push_back("NULL");
			}
			else
			{
				buffer[sizeof(buffer) - 1] = '\0';
				row.push_back(std::string(reinterpret_cast<char*>(buffer)));
			}
		
		}

		result.push_back(row);
	}

	//print_results(result);
	return 0;
}

std::string Database::generate_insert_query(const std::string table_name, const std::vector<std::string>& columns, const std::vector<std::vector<std::string>>& values)
{

	std::string insert_query = "INSERT INTO " + table_name + " (";
	for (size_t i = 0; i < columns.size(); ++i)
	{
		insert_query += columns[i];
		if (i < columns.size() - 1)
			insert_query += ", ";
	}
	insert_query += ") VALUES ";

	for (size_t i = 0; i < values.size(); ++i)
	{
		if (i > 0)
			insert_query += ", ";

		insert_query += "(";
		for (size_t j = 0; j < values[i].size(); ++j)
		{
			insert_query += "'" + values[i][j] + "'";
			if (j < values[i].size() - 1)
				insert_query += ", ";
		}
		insert_query += ")";
	}

	return insert_query;
}

std::string Database::generate_sort_query(char* payload)
{
	std::string sort_query = "SELECT Id FROM Anunt";

	bool first_filter = 1;

	add_property_type_sort_query(sort_query, payload[0],first_filter);
	add_price_range_sort_query(sort_query, payload[1], first_filter);
	add_ad_type_sort_query(sort_query, payload[2], first_filter);
	add_surface_sort_quey(sort_query, payload[3], first_filter);
	add_order_by_sort_query(sort_query, payload[4], payload[5]);
	
	return sort_query;
}

std::string Database::generate_favorite_query(char* payload)
{
	//SELECT [Id anunt] FROM Favorite WHERE Username='ana_maria'
	return "SELECT [Id anunt] FROM Favorite WHERE Username='"+std::string(payload)+"'";
}

std::string Database::generate_profile_query(char* payload)
{
	return "SELECT Id FROM Anunt WHERE [Id ofertant]='" + std::string(payload) + "'";
}

void Database::add_property_type_sort_query(std::string& sort_query, char byte, bool& first)
{

	if (byte == '1')
	{ 
		sort_query += " WHERE [Tip proprietate]='casa'";
		first = 0;
	}
	else if (byte == '2')
	{
		sort_query += " WHERE [Tip proprietate]='apartament'";
		first = 0;
	}
	else if (byte == '3')
	{
		sort_query += " WHERE [Tip proprietate]='teren'";
		first = 0;
	}
	else if (byte == '4')
	{
		sort_query += " WHERE ([Tip proprietate]='casa' OR [Tip proprietate]='apartament')";
		first = 0;
	}
	else if (byte == '5')
	{
		sort_query += " WHERE ([Tip proprietate]='casa' OR [Tip proprietate]='teren')";
		first = 0;
	}
	else if (byte == '6')
	{
		sort_query += " WHERE ([Tip proprietate]='apartament' OR [Tip proprietate]='teren')";
		first = 0;
	}
	
}

void Database::add_price_range_sort_query(std::string& sort_query, char byte, bool& first)
{
	if (byte == '0')
		return;

	if (first == 0)
	{
		sort_query += " AND ";
	}
	else
	{
		sort_query += " WHERE ";
		first = 0;
	}

	if (byte == '8')
	{
		sort_query += "Pret >= 1000";
		return;
	}

	std::string min_value, max_value;

	if (byte == '1')
	{
		min_value = "0";
		max_value = "500";
	}
	else if (byte == '2')
	{
		min_value = "500";
		max_value = "1000";
	}
	else if (byte == '3')
	{
		min_value = "1000";
		max_value = "5000";
	}
	else if (byte == '4')
	{
		min_value = "5000";
		max_value = "10000";
	}
	else if (byte == '5')
	{
		min_value = "10000";
		max_value = "50000";
	}
	else if (byte == '6')
	{
		min_value = "50000";
		max_value = "100000";
	}
	else if (byte == '7')
	{
		min_value = "100000";
		max_value = "200000";
	}
	
	sort_query += "( Pret BETWEEN " + min_value + " AND " + max_value+" )";
}

void Database::add_ad_type_sort_query(std::string& sort_query, char byte, bool& first)
{
	if (byte == '0')
		return;

	if (first == 0 )
	{
		sort_query += " AND ";
	}
	else
	{
		sort_query += " WHERE ";
		first = 0;
	}

	if (byte == '1')
	{
		sort_query += "[Tip anunt] ='vanzare'";
	}
	else if (byte == '2')
	{
		sort_query += "[Tip anunt] ='inchiriere'";
	}

}

void Database::add_surface_sort_quey(std::string& sort_query, char byte, bool& first)
{
	if (byte == '0')
		return;

	if (first == 0 )
	{
		sort_query += " AND ";
	}
	else
	{
		sort_query += " WHERE ";
		first = 0;
	}

	if (byte == '5')
	{
		sort_query += "Suprafata >=500";
		return;
	}

	std::string min_value, max_value;

	if (byte == '1')
	{
		min_value = "0";
		max_value = "50";
	}
	else if (byte == '2')
	{
		min_value = "50";
		max_value = "100";
	}
	else if (byte == '3')
	{
		min_value = "100";
		max_value = "200";
	}
	else if (byte == '4')
	{
		min_value = "200";
		max_value = "500";
	}
	
	sort_query += "( Suprafata BETWEEN " + min_value + " AND " + max_value+" )";
}

void Database::add_order_by_sort_query(std::string& sort_query, char byte_1, char byte_2)
{
	if (byte_1 != '0')
	{
		sort_query += " ORDER BY ";
		
		if (byte_2 == '0')
			sort_query += "Pret ";
		else
			sort_query += "Suprafata ";

		if (byte_1 == '1')
			sort_query += "ASC";
		else
			sort_query += "DESC";
	}
}


bool Database::insert_data(const std::string& table_name, const std::vector<std::string>& columns, const std::vector<std::vector<std::string>>& values)
{
	std::string insert_query = generate_insert_query(table_name, columns, values);
	return execute_query(insert_query);
}

bool Database::delete_data(const std::string& table_name, const std::string& condition_column, const std::string& condition_value)
{
	std::string delete_query = "DELETE FROM " + table_name + " WHERE " + condition_column + " = '" + condition_value + "'";
	return execute_query(delete_query);
}

std::vector<std::string> Database::fetch_feed_ad_ids(char* payload)
{
	std::string sort_query = generate_sort_query(payload);
	std::vector<std::vector<std::string>>results;

	if (fetch_data(sort_query, results))
	{
		Logger::get_instance().lgi(log_level::ERR, "fetch_ad_ids", "Failed select query.", sort_query);
		exit(1);
	}

	std::vector<std::string >ad_ids;

	for (auto row : results)
	{
		ad_ids.push_back(row[0]);
	}

	return ad_ids;
	
}

std::vector<std::string> Database::fetch_favorite_ad_ids(char* payload)
{
	std::string fav_query = generate_favorite_query(payload);
	std::vector<std::vector<std::string>>results;

	if (fetch_data(fav_query, results))
	{
		Logger::get_instance().lgi(log_level::ERR, "fetch_favorite_ad_ids", "Failed select query.", fav_query);
		exit(1);
	}

	std::vector<std::string >ad_ids;

	for (auto row : results)
	{
		ad_ids.push_back(row[0]);
	}

	return ad_ids;
}

std::vector<std::string> Database::fetch_profite_ad_ids(char* payload)
{
	std::string profile_query = generate_profile_query(payload);
	std::vector<std::vector<std::string>>results;

	if (fetch_data(profile_query, results))
	{
		Logger::get_instance().lgi(log_level::ERR, "fetch_profite_ad_ids", "Failed select query.", profile_query);
		exit(1);
	}

	std::vector<std::string >ad_ids;

	for (auto row : results)
	{
		ad_ids.push_back(row[0]);
	}

	return ad_ids;
	
}

bool Database::check_username_existance(std::string username)
{
	std::string query = "SELECT COUNT(*) FROM [User] WHERE Username='" + username+"' AND Status=0";

	std::vector<std::vector<std::string>>result;
	if (fetch_data(query,result))
	{
		Logger::get_instance().lgi(log_level::ERR, "check_username_existance", "Failed checking username existance", query);
		exit(1);
	}

	if (result[0][0] == "0")
	{
		return 0;	//nu exista
	}

	return 1;	//exista
}

std::string Database::fetch_ad_first_image_path(std::string ad_id)
{
	std::vector<std::vector<std::string>> results;
	fetch_data("SELECT TOP(1) Cale FROM Imagine WHERE [Id anunt] = " + ad_id,results);

	if (results.empty())
	{
		return "1"; //postarea nu are atasate imagini
	}

	return results[0][0];
}

bool Database::remove_ad_from_favorite(std::string ad_id, std::string username)
{
	if (execute_query("DELETE FROM Favorite WHERE [Id anunt]=" + ad_id + " AND [Username]='" + username+"'"))
	{
		return 1;
	}

	return 0;
}

bool Database::delete_ad(char* ad_id)
{
	std::string procedure_call = "EXEC sterge_anunt_si_legaturi @id_anunt = " + std::string(ad_id);
	return execute_query(procedure_call);
}

bool Database::buy_property(std::string username, std::string ad_id)
{
	std::string query = "EXEC Efecuteaza_tranzactie @id_anunt = " + ad_id + ", @cumparator = '" + username + "'";
	return execute_query(query);
}

bool Database::add_funds(std::string username, std::string value)
{
	std::string query="UPDATE [User] SET Sold = Sold + "+value+" WHERE Username = '"+username+"'";
	return execute_query(query);
}

bool Database::update_sold(std::string username, std::string value)
{
	std::string query = "UPDATE [User] SET Sold = Sold - " + value + " WHERE Username = '" + username + "'";
	return execute_query(query);
}

std::string Database::get_sold(std::string username)
{
	std::vector<std::vector<std::string>> results;
	fetch_data("SELECT Sold FROM [User] WHERE Username='" + username + "'", results);

	return results[0][0];
}

bool Database::block_ad(char* ad_id)
{
	std::string query = "UPDATE Anunt SET[Postare blocata] = 1 WHERE Id = " + std::string(ad_id);
	return execute_query(query);
}

bool Database::unblock_ad(char* ad_id)
{
	std::string query = "UPDATE Anunt SET[Postare blocata] = 0 WHERE Id = " + std::string(ad_id);
	return execute_query(query);
}

std::vector<std::string> Database::fetch_chat_users(char* username)
{
	auto username_s = std::string(username);
	std::string query = "SELECT DISTINCT [Id sender] FROM Mesaje WHERE[Id receiver] = '"+username_s+"' UNION SELECT DISTINCT[Id receiver] FROM Mesaje WHERE [Id sender] = '" + username_s + "'" ;

	std::vector<std::vector<std::string>>result;

	if (fetch_data(query, result))
	{
		Logger::get_instance().lgi(log_level::ERR, "fetch_chat_user", "Failed fetching chat users.", query);
		exit(1);
	}

	std::vector<std::string>users;

	for (auto row : result)
	{
		users.push_back(row[0]);
	}

	return users;
}

std::vector<std::vector<std::string>> Database::fetch_chat_messages(std::string username_b, std::string username_s)
{
	std::string query = "SELECT [Id sender], [Id receiver], Mesaj FROM Mesaje WHERE ([Id sender] = '" + username_b + "'AND[Id receiver] = '" + username_s + "') OR([Id sender] = '" + username_s + "'AND[Id receiver] = '" + username_b + "') ORDER BY[Timp trimitere] ASC";

	std::vector<std::vector<std::string>>result;
	if (fetch_data(query, result))
	{
		Logger::get_instance().lgi(log_level::ERR, "fetch_chat_messages", "Failed fetching messages ", query);
		exit(1);
	}

	return result;

}

void Database::add_message(std::string username_1, std::string username_2, std::string message)
{
	
}

bool Database::delete_user(char* payload)
{
	std::string query = "UPDATE [User] SET Status=1 WHERE Username='" + std::string(payload) + "'";
	return execute_query(query);
}

void Database::free_handle()
{
	SQLFreeHandle(SQL_HANDLE_STMT, SQL_statement_handle);
}

bool Database::check_password(std::string username, std::string password)
{
	std::string query = "SELECT Parola FROM [User] WHERE Username='" + std::string(username)+"'";

	std::vector<std::vector<std::string>>result;
	if (fetch_data(query, result))
	{
		Logger::get_instance().lgi(log_level::ERR, "check_password", "Failed checking password ", query);
		exit(1);
	}

	if (std::string(password) == result[0][0])
	{
		return 1;
	}

	return 0;
}

bool Database::check_admin_status(std::string username)
{
	std::string query = "SELECT Admin FROM [User] WHERE Username='" + username + "'";

	std::vector<std::vector<std::string>>result;
	if (fetch_data(query, result))
	{
		Logger::get_instance().lgi(log_level::ERR, "check_password", "Failed checking password ", query);
		exit(1);
	}

	if ("1" == result[0][0])
	{
		return 1;
	}

	return 0;
}

login_result Database::check_user_credentials(std::string username, std::string password)
{	
	if (!check_username_existance(username))
		return login_result::INCORRECT_USERNAME;
	
	if (!check_password(username, password))
		return login_result::INCORRECT_PASSWORD;

	if (check_admin_status(username))
		return login_result::ADMIN;

	return login_result::SUCCESS;
}

Database::Database()
{
	SQL_env_handle = NULL;
	SQL_connection_handle = NULL;
	SQL_statement_handle = NULL;

}

Database::~Database()
{
	SQLFreeHandle(SQL_HANDLE_STMT, SQL_statement_handle);
	SQLDisconnect(SQL_connection_handle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQL_connection_handle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQL_env_handle);
}
