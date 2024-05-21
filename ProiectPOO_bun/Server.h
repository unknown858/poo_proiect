#pragma once
#undef UNICODE
#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Database.h"
#include "Logger.h"
#include "FrameUtils.h"


class Server
{
private:

	static Server* m_ptr;

	short m_port;
	SOCKET m_listen_sock;
	std::string m_ad_nr;
	std::unique_ptr<Database>m_db;
	std::vector<SOCKET>m_client_socks;


	Server(const char* ip_adress, short listen_port);
	Server(const Server& obj) {};
	~Server() {};//TO DO 

	WSADATA init_winsock();
	addrinfo configure_hints(short listen_port);
	addrinfo* get_server_addres_info(const char* ip_adress, short listen_port, const addrinfo& hints);
	SOCKET create_socket(addrinfo* server_adress);
	void set_sock_non_blocking(SOCKET& sock);
	void bind_sock(SOCKET& sock, addrinfo* adress_info);
	void listen();

	void init_master(fd_set& master);
	void handle_incoming_connections(FD_SET& master);
	void handle_client_data(const FD_SET& master);
	std::string get_client_ip_adress(const SOCKET& sock);
	//std::string get_client_ip_adress(const SOCKET& sock);

	void clear_result(std::vector<std::vector<std::string>>& results);

	void handle_data_by_type(char*payload,data_type type, SOCKET client_sock);

	void log_in(char*payload, SOCKET client_sock);
	char database_login_response(std::string username, std::string password);

	void sign_in(char* payload, SOCKET client_sock);
	char insert_user_into_database(std::vector<std::string>user_data);

	void save_image(char* payload, SOCKET& client_sock, int& add_post);
	char insert_image_info_into_database(char* ad_nr, std::string& image_path);

	void send_ads_nr(SOCKET client_sock);

	void add_adl(char* payload, SOCKET client_sock);
	char insert_adl_info_into_database(std::vector<std::string>ad_info);

	void add_adh(char* payload, SOCKET client_sock);
	char insert_adh_info_into_database(std::vector<std::string>ad_info);
	
	void add_review(char* payload, SOCKET client_sock);
	char insert_review_into_database(std::vector<std::string>review);

	void send_ad_summary(char* ad_id,SOCKET& client_sock);
	void send_ads_summary(std::vector<std::string>ads_id, SOCKET& client_sock);

	void send_ad_details(char*payload, SOCKET& client_sock);
	void send_ad_info(std::string ad_id,std::string property_type,SOCKET& client_sock);
	void send_ad_images(std::string ad_id,SOCKET& client_sock);
	void send_ad_reviews(std::string ad_id,SOCKET& client_sock);

	void send_feed(char* payload, SOCKET client_sock);
	void send_favorite_feed(char* payload, SOCKET& client_sock);
	void send_profile_feed(char* payload, SOCKET& client_sock);
	void send_ad_ids(std::vector<std::string>& ad_id, data_type type, SOCKET& client_sock);

	void add_to_remove_from_favorite(char* payload, SOCKET& client_sock);
	char add_favorite_status_to_database(std::vector<std::string>info);
	char remove_favorite_status(std::vector<std::string>info);

	bool delete_ad(char* payload);
	void delete_ad(char* payload, SOCKET& client_socket);

	void send_funds(char* payload, SOCKET& client_sock);
	void add_funds(char* payload, SOCKET& client_socket);
	void buy_property(char* payload, SOCKET& client_sock);
	bool compare_values(std::string& value1, std::string value2);

	void block_ad(char* payload, SOCKET& client_sock);
	void unblock_ad(char* payload, SOCKET& client_sock);
	
	void send_chat(char* payload, SOCKET& client_sock);
	void send_specific_chat(char* payload, SOCKET& client_sock);
	void send_chat_users(char* payload, SOCKET& client_sock);
	void send_chat_messages(std::string username_1,std::string username_2, SOCKET& client_sock);

	void add_message_into_database(char* payload, SOCKET& client_sock);

	void delete_user(char* payload, SOCKET& client_sock);
public:

	void wait_connection();

	static Server& get_instance();
	static void destroy_instance();

};

