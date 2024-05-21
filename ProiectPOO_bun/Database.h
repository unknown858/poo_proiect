#pragma once
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>
#include <vector>

#include "Logger.h"

#define SQL_RESULT_LEN 2000

enum class login_result {SUCCESS, INCORRECT_USERNAME,INCORRECT_PASSWORD, ADMIN};

class Database
{

private:

	SQLHANDLE SQL_env_handle;			// pentru a reprezenta un descriptor de mediu pentru lucrul cu bazele de date
	SQLHANDLE SQL_connection_handle;	// descriptor de conexiune la o baza de date
	SQLHANDLE SQL_statement_handle;		//n descriptor de instructiune SQL

	std::string get_SQL_error(unsigned int handle_type, SQLHANDLE& handle);

	bool execute_query(const std::string query);

	std::string generate_insert_query(const std::string table_name, const std::vector<std::string>& columns, const std::vector<std::vector<std::string>>& values);
	std::string generate_sort_query(char* payload);
	std::string generate_favorite_query(char* payload);
	std::string generate_profile_query(char* payload);

	void add_property_type_sort_query(std::string&sort_query,char byte,bool &first);
	void add_price_range_sort_query(std::string& sort_quey,char byte, bool& first);
	void add_ad_type_sort_query(std::string& sort_query,char byte, bool& first);
	void add_surface_sort_quey(std::string& sort_query,char byte, bool& first);
	void add_order_by_sort_query(std::string& sort_query,char byte_1,char byte_2);
	//void print_results(std::vector<std::vector<std::string>>& result);

	bool check_password(std::string username, std::string password);
	bool check_admin_status(std::string username);

public:

	void connect(const std::string& db_info, const std::string& db_name);

	std::string get_ads_nr();
	std::string get_image_number(char*ad_nr);

	bool fetch_data(const std::string &query, std::vector<std::vector<std::string>>& result);
	bool insert_data(const std::string& table_name, const std::vector<std::string>& columns, const std::vector<std::vector<std::string>>& values);
	bool delete_data(const std::string& table_name, const std::string& condition_column, const std::string& condition_value);

	std::vector<std::string> fetch_feed_ad_ids(char* payload);
	std::vector<std::string>fetch_favorite_ad_ids(char* payload);
	std::vector<std::string>fetch_profite_ad_ids(char* payload);

	login_result check_user_credentials(std::string username, std::string password);
	bool check_username_existance(std::string username);

	std::string fetch_ad_first_image_path(std::string ad_id);

	bool remove_ad_from_favorite(std::string ad_id, std::string username);

	bool delete_ad(char* payload);
	bool buy_property(std::string username,std::string ad_id);

	bool add_funds(std::string username, std::string value);
	bool update_sold(std::string username, std::string value);

	std::string get_sold(std::string username);

	bool block_ad(char* ad_id);
	bool unblock_ad(char* ad_id);

	std::vector<std::string>fetch_chat_users(char* username);
	std::vector<std::vector<std::string>>fetch_chat_messages(std::string username_b, std::string username_s);

	void add_message(std::string username_1, std::string username_2, std::string message);

	bool delete_user(char* payload );

	void free_handle();

	Database();
	~Database();

};

