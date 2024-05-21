#pragma comment (lib, "Ws2_32.lib")
#include "Server.h"

#define MAX_CLIENTS 5

Server* Server::m_ptr = nullptr;

Server& Server::get_instance()
{
	if (m_ptr == nullptr)
		m_ptr = new Server("172.20.10.3", 12345);
	//m_ptr = new Server("172.16.41.161", 12345);
	//m_ptr = new Server("127.0.0.1", 12345);

	return *m_ptr;
}

void Server::destroy_instance()
{
	if (m_ptr != nullptr)
	{
		delete m_ptr;
		m_ptr = nullptr;
	}
}
//initializare biblioteca winsock
WSADATA Server::init_winsock()
{
	WSADATA  wsa_data;
	int error = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (error)
	{
		Logger::get_instance().lgi(log_level::CRITICAL, "init_winsock", "WSAStartup failed with error : " + WSAGetLastError());
		exit(1);
	}

	return wsa_data;
}

addrinfo Server::configure_hints(short listen_port)
{
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET; //IPv4
	hints.ai_socktype = SOCK_STREAM;//TCP
	hints.ai_flags = AI_PASSIVE;//socket pasiv
	return hints;
}

addrinfo* Server::get_server_addres_info(const char* ip_adress, short listen_port, const addrinfo& hints)
{
	addrinfo* server_adress = nullptr;
	int error = getaddrinfo(ip_adress, std::to_string(listen_port).c_str(), &hints, &server_adress);
	if (error)
	{
		Logger::get_instance().lgi(log_level::CRITICAL, "get_server_addres_info", "getaddrinfo failed with error : " + WSAGetLastError());
		exit(1);
	}

	return server_adress;
}

SOCKET Server::create_socket(addrinfo* adress_info)
{
	SOCKET sock = socket(adress_info->ai_family, adress_info->ai_socktype, adress_info->ai_protocol);
	if (sock == INVALID_SOCKET)
	{
		Logger::get_instance().lgi(log_level::CRITICAL, "create_socket", "failed creating socket with error: " + WSAGetLastError());
		freeaddrinfo(adress_info);
		WSACleanup();
		exit(1);
	}

	return sock;
}

void Server::bind_sock(SOCKET& sock, addrinfo* adress_info)
{
	int error = bind(sock, adress_info->ai_addr, adress_info->ai_addrlen);
	if (error == SOCKET_ERROR)
	{
		Logger::get_instance().lgi(log_level::CRITICAL, "bind_sock", "Bind failed with error code : " + WSAGetLastError());
		closesocket(sock);
		freeaddrinfo(adress_info);
		WSACleanup();
		exit(1);
	}
	else
	{
		freeaddrinfo(adress_info);
	}
}

void Server::listen()
{
	int error = ::listen(m_listen_sock, SOMAXCONN);
	if (error == SOCKET_ERROR)
	{
		Logger::get_instance().lgi(log_level::CRITICAL, "listen", "Listen failed with error code : " + WSAGetLastError());
		closesocket(m_listen_sock);
		WSACleanup();
		exit(1);
	}
}

void Server::set_sock_non_blocking(SOCKET& sock)
{
	u_long i_mode = 1;
	auto error = ioctlsocket(sock, FIONBIO, &i_mode);
	if (error == SOCKET_ERROR)
	{
		Logger::get_instance().lgi(log_level::ERR, "set_sock_non_blocking", "ioctlsocket failed with error : " + WSAGetLastError());
	}
}

Server::Server(const char* ip_adress, short listen_port) :m_port(listen_port), m_db(std::make_unique<Database>())
{
	WSADATA wsa_data = init_winsock();
	addrinfo hints = configure_hints(listen_port);
	addrinfo* server_adress = get_server_addres_info(ip_adress, listen_port, hints);
	m_listen_sock = create_socket(server_adress);
	set_sock_non_blocking(m_listen_sock);
	bind_sock(m_listen_sock, server_adress);
	listen();

	m_db->connect("localhost, 49899", "AgentieImobiliara");
}

void Server::init_master(fd_set& master)
{
	FD_ZERO(&master);
	FD_SET(m_listen_sock, &master);
}

void Server::handle_client_data(const FD_SET& master)
{
	for (auto& sock : m_client_socks)
	{
		if (FD_ISSET(sock, &master))
		{

			char buff[FRAME_SIZE+1];
			int recv_byte = ::recv(sock, buff, FRAME_SIZE, 0);

			if (recv_byte <= 0)
			{
				continue;
			}
			else
			{	
				char last_frame;
				data_type type;
				int payload_size;
				char* payload;

				Frame::get_frame_details(buff, type, payload_size, payload, last_frame);

				handle_data_by_type(payload, type, sock);

			}
		}
	}
}

std::string Server::get_client_ip_adress(const SOCKET& sock)
{
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	auto error = getpeername(sock, (struct sockaddr*)(&client_addr), &client_addr_len);

	if (error == SOCKET_ERROR)
	{
		Logger::get_instance().lgi(log_level::ERR, "get_client_ip_adress", "getpeername failed with error : " + WSAGetLastError());

	}

	char ip_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
	ip_str[strlen(ip_str)] = '\0';
	return ip_str;
}

void Server::clear_result(std::vector<std::vector<std::string>>& results)
{
	for (auto &row : results)
	{
		for (auto &col : row)
		{
			col.clear();
		}
		
		row.clear();
	}

	results.clear();
}


void Server::handle_data_by_type(char* payload, data_type type,SOCKET client_sock)
{
	static int add_post = 0;

	switch (type)
	{
	case data_type::img:
		save_image(payload, client_sock,add_post);
		break;
	case data_type::lgi:
		log_in(payload, client_sock);
		break;
	case data_type::sgi:
		sign_in(payload, client_sock);
		break;
	case data_type::req:
		send_ads_nr(client_sock);
		break;
	case data_type::adl:
		add_adl(payload, client_sock);
		break;
	case data_type::adh:
		add_adh(payload, client_sock);
		break;
	case data_type::rev:
		add_review(payload, client_sock);
		break;
	case data_type::srt:
		send_feed(payload, client_sock);
		break;
	case data_type::add:
		send_ad_details(payload, client_sock);
		break;
	case data_type::fav:
		add_to_remove_from_favorite(payload, client_sock);
		break;
	case data_type::dlt:
		delete_ad(payload, client_sock);
		break;
	case data_type::rqf:
		send_favorite_feed(payload, client_sock);
		break;
	case data_type::rqp:
		send_profile_feed(payload, client_sock);
		break;
	case data_type::fnd:
		add_funds(payload, client_sock);
		break;
	case data_type::buy:
		buy_property(payload, client_sock);
		break;
	case data_type::blk:
		block_ad(payload, client_sock);
		break;
	case data_type::ubk:
		unblock_ad(payload, client_sock);
		break;
	case data_type::chu:
		send_chat(payload, client_sock);
		break;
	case data_type::cha:
		send_specific_chat(payload, client_sock);
		break;
	case data_type::sld:
		send_funds(payload, client_sock);
		break;
	case data_type::dlu:
		delete_user(payload, client_sock);
		break;
	case data_type::sdm:
		add_message_into_database(payload, client_sock);
		break;
	default:
		break;
	}
}

void Server::log_in(char* payload, SOCKET client_sock)
{
	auto tokens = Frame::split_payload(payload);
	auto resp = database_login_response(tokens[0], tokens[1]);
	::send(client_sock, &resp, sizeof(resp), 0);
}

char Server::database_login_response(std::string username, std::string password)
{
	switch (m_db->check_user_credentials(username, password))
	{
	case::login_result::INCORRECT_USERNAME:
			return '1';
			break;
	case::login_result::INCORRECT_PASSWORD:
			return '2';
			break;
	case::login_result::ADMIN:
			return '3';
			break;
	case::login_result::SUCCESS:
			return '0';
			break;
	}
}

void Server::sign_in(char* payload, SOCKET client_sock)
{
	auto tokens = Frame::split_payload(payload);
	auto resp = insert_user_into_database(tokens);
	::send(client_sock, &resp, sizeof(resp), 0);
}

char Server::insert_user_into_database(std::vector<std::string> user_data)
{
	if (m_db->check_username_existance(user_data[4]) == 1)
	{
		return '2';   //inregistrare invalida, nu pot exista 2 conturi si acelas username
	}

	user_data.push_back("0");   // nu este ofertant
	user_data.push_back("0");	// nu este blocat de admin
	user_data.push_back("0");	// nu este admin

	std::vector<std::vector<std::string>>data;
	data.push_back(user_data);
	if (m_db->insert_data("[User]", { "Nume","Prenume","Email","[Numar telefon]","Username","Parola","Ofertant","Status","Admin"}, data))
	{
		Logger::get_instance().lgi(log_level::ERR, "insert_user_into_database", "Sign in failed", "Data of clientwith username " + user_data[5] + " couldn't be added to database.");
		return '1';
	}

	Logger::get_instance().lgi(log_level::INFO, "insert_user_into_database", "New account created. Client with username " + user_data[5] + " added.");

	return '0';
}

void Server::add_adl(char* payload, SOCKET client_sock)
{
	auto tokens = Frame::split_payload(payload);
	auto resp = insert_adl_info_into_database(tokens);
	::send(client_sock, &resp, sizeof(resp), 0);
}

char Server::insert_adl_info_into_database(std::vector<std::string> ad_info)
{
	//INSERT INTO Anunt([Id ofertant], [Tip anunt], [Tip proprietate], Descriere, Data, Pret, Oras, Suprafata,[Postare blocata])
	//	VALUES
	//	(1, 1, 'casa', 'Oferim o casă frumoasă într-o zonă liniștită', '2024-05-10', 250000, 'București', 150.5,"0"),

	std::vector<std::vector<std::string>>data;
	ad_info.insert(ad_info.begin(), m_ad_nr);

	data.push_back(ad_info);

	if (m_db->insert_data("Anunt", { "Id","[Id ofertant]","[Tip anunt]","[Tip proprietate]","Descriere","Pret","Oras","Suprafata","[Postare blocata]" }, data))
	{
		Logger::get_instance().lgi(log_level::ERR, "insert_adl_info_into_database", "Land ad info coudn't be inserted to database.","Ad number : "+m_ad_nr+"\nUser : "+ad_info[1]);
		return '1';
	}

	Logger::get_instance().lgi(log_level::INFO, "insert_adl_info_into_database", "Land ad inserted.", "Ad number : " + m_ad_nr + "\nUser : " + ad_info[1]);

	return '0';
}

void Server::add_adh(char* payload, SOCKET client_sock)
{
	auto tokens = Frame::split_payload(payload);
	auto resp = insert_adh_info_into_database(tokens);
	::send(client_sock, &resp, sizeof(resp), 0);
}

char Server::insert_adh_info_into_database(std::vector<std::string> ad_info)
{
	std::vector<std::vector<std::string>>data;

	auto resp = insert_adl_info_into_database(std::vector<std::string>(ad_info.begin(), ad_info.begin()+8));

	ad_info.insert(ad_info.begin()+8, m_ad_nr);
	data.push_back(std::vector(ad_info.begin() + 8, ad_info.end()));

	if (resp == '1')
	{
		m_db->delete_data("[Anunt]", "Id", m_ad_nr);
		Logger::get_instance().lgi(log_level::ERR, "insert_adh_info_into_database", "Home ad info coudn't be inserted to database.", "Ad number : " + m_ad_nr + "\nUser : " + ad_info[1]);
		return '1';
	}
	else
	{
		if (m_db->insert_data("[Anunt imobil]", { "[Id anunt]","[Numar camere]"," [Numar bai]"," [Suprafata utila]" }, data))
		{
			m_db->delete_data("[Anunt]", "Id", m_ad_nr);

			Logger::get_instance().lgi(log_level::ERR, "insert_adh_info_into_database", "Home ad info coudn't be inserted to database.", "Ad number : " + m_ad_nr + "\nUser : " + ad_info[1]);
			return '1';
		}
	}

	Logger::get_instance().lgi(log_level::INFO, "insert_adh_info_into_database", "Home ad inserted.", "Ad number : " + m_ad_nr + "\nUser : " + ad_info[1]);

	return '0';
}

void Server::add_review(char* payload, SOCKET client_sock)
{
	auto tokens = Frame::split_payload(payload);
	auto resp = insert_review_into_database(tokens);
	::send(client_sock, &resp, sizeof(resp), 0);
}

char Server::insert_review_into_database(std::vector<std::string> review)
{
	//INSERT INTO Reviews([Id anunt], [Id user], [Mesaj]) VALUES('1', 'dani09', 'Da, este bine');

	std::vector<std::vector<std::string>>data;
	data.push_back(review);
	if (m_db->insert_data("Reviews", { "[Id anunt]","[Id user]","[Mesaj]"}, data))
	{
		Logger::get_instance().lgi(log_level::ERR, "insert_review_into_database", "Review couldn't be added.", "Ad number : " + review[0] + " Username : "+review[1]);
		return '1';
	}

	Logger::get_instance().lgi(log_level::INFO, "insert_review_into_database", "Review added.", "Ad number : " + review[0] + " Username : " + review[1]);

	return '0';
}

void Server::send_feed(char* payload, SOCKET client_sock)
{
	std::vector<std::string>ad_id = m_db->fetch_feed_ad_ids(payload);

	/*for (auto ad : ad_id)
	{
		send_ad_summary(ad.data(),client_sock);
	}*/

	send_ads_summary(ad_id, client_sock);

	//Frame::send_data((char*)"end", strlen("end"), client_sock, data_type::ads);
}

void Server::send_ad_summary(char* ad_id,SOCKET &client_sock)
{
	std::vector<std::vector<std::string>>results;
	m_db->fetch_data("SELECT [Id ofertant], [Tip anunt], [Tip proprietate], Pret, Oras, Suprafata, [Postare blocata], Id FROM Anunt WHERE Id="+std::string(ad_id), results);
	
	char* payload = Frame::concat_inputs(results[0]);
	Frame::send_data(payload,strlen(payload), client_sock,data_type::ads);

	delete[]payload;


	std::string first_image_path = m_db->fetch_ad_first_image_path(ad_id);

	Frame::receive_ack(client_sock);

	if(first_image_path!="1")
	{ 
		Frame::send_image(first_image_path,ad_id,client_sock);
	}

}

void Server::send_ads_summary(std::vector<std::string>ads_id, SOCKET& client_sock)
{
	for (auto ad : ads_id)
	{
		send_ad_summary(ad.data(), client_sock);
	}

	Frame::send_data((char*)"end", strlen("end"), client_sock, data_type::ads);
}

void Server::send_ad_details(char*payload,SOCKET& client_sock)
{
	auto tokens = Frame::split_payload(payload);

	send_ad_info(tokens[0], tokens[1], client_sock);
	send_ad_images(tokens[0],client_sock);
	send_ad_reviews(tokens[0], client_sock);

	//std::vector<std::vector<std::string>>results;

	//if (tokens[1] == "teren")
	//{
	//	if (m_db->fetch_data("SELECT  [Tip proprietate], Descriere, Data, Id FROM Anunt WHERE Id=" + tokens[0], results))
	//	{
	//		Logger::get_instance().lgi(log_level::ERR, "send_ad_details", "Ad details couldn't be fetched.", "Ad number : " + tokens[0] );
	//	}
	//}
	//else
	//{
	//	if (m_db->fetch_data("SELECT A.[Tip proprietate], A.Descriere, AI.[Numar camere], AI.[Numar bai], AI.[Suprafata utila], A.Data, A.Id FROM Anunt A JOIN [Anunt imobil] AI ON A.Id = AI.[Id anunt] WHERE A.Id =" + tokens[0], results))
	//	{
	//		Logger::get_instance().lgi(log_level::ERR, "send_ad_details", "Ad details couldn't be fetched.", "Ad number : " + tokens[0]);
	//	}
	//}
	//char* frame_payload = Frame::concat_inputs(results[0]);

	//Frame::send_data(frame_payload, strlen(frame_payload), client_sock, data_type::add);

	//clear_result(results);

	//if (m_db->fetch_data("SELECT [Id user], Mesaj FROM Reviews WHERE [Id anunt]=" + tokens[0], results))
	//{
	//	Logger::get_instance().lgi(log_level::ERR, "send_ad_details", "Reviews couldn't be fetched.", "Ad number : " + tokens[0]);
	//}

	//if (!results.empty())
	//{
	//	for (auto review : results)
	//	{
	//		auto payload = Frame::concat_inputs(review[0].data(), review[1].data(), nullptr);
	//		Frame::send_data(payload, strlen(payload), client_sock, data_type::rev);
	//	}
	//}

	//clear_result(results);

	//delete[]payload;

	//m_db->fetch_data("SELECT Cale FROM Imagine WHERE [Id anunt]=" + tokens[0], results);

	//for (int i = 1; i < results.size(); i++)
	//{
	//	Frame::send_image(results[i][0], tokens[0].data(), client_sock);
	//}
	
	Frame::send_data((char*)"end", strlen("end"), client_sock, data_type::add);
}

void Server::send_ad_info(std::string ad_id, std::string property_type, SOCKET& client_sock)
{	std::vector<std::vector<std::string>>results;

	if (property_type== "teren")
	{
		if (m_db->fetch_data("SELECT  [Tip proprietate], Descriere, Data, Id FROM Anunt WHERE Id=" + ad_id, results))
		{
			Logger::get_instance().lgi(log_level::ERR, "send_ad_details", "Ad details couldn't be fetched.", "Ad number : " + ad_id);
		}
	}
	else
	{
		if (m_db->fetch_data("SELECT A.[Tip proprietate], A.Descriere, AI.[Numar camere], AI.[Numar bai], AI.[Suprafata utila], A.Data, A.Id FROM Anunt A JOIN [Anunt imobil] AI ON A.Id = AI.[Id anunt] WHERE A.Id =" +ad_id, results))
		{
			Logger::get_instance().lgi(log_level::ERR, "send_ad_details", "Ad details couldn't be fetched.", "Ad number : " + ad_id);
		}
	}
	char* frame_payload = Frame::concat_inputs(results[0]);

	Frame::send_data(frame_payload, strlen(frame_payload), client_sock, data_type::add);

	delete[]frame_payload;

	Frame::receive_ack(client_sock);
}

void Server::send_ad_images(std::string ad_id, SOCKET& client_sock)
{
	std::vector<std::vector<std::string>>results;

	m_db->fetch_data("SELECT Cale FROM Imagine WHERE [Id anunt]=" + ad_id, results);

	for (int i = 1; i < results.size(); i++)
	{
		Frame::send_image(results[i][0], ad_id.data(), client_sock);
	}

	
}

void Server::send_ad_reviews(std::string ad_id, SOCKET& client_sock)
{
	std::vector<std::vector<std::string>>results;

	if (m_db->fetch_data("SELECT [Id user], Mesaj, [Id anunt] FROM Reviews WHERE [Id anunt]=" + ad_id, results))
	{
		Logger::get_instance().lgi(log_level::ERR, "send_ad_details", "Reviews couldn't be fetched.", "Ad number : " +ad_id);
	}

	if (!results.empty())
	{
		for (auto review : results)
		{
			auto payload = Frame::concat_inputs(review[0].data(), review[1].data(),review[2].data(), nullptr);
			Frame::send_data(payload, strlen(payload), client_sock, data_type::rev);
			Frame::receive_ack(client_sock);

			delete[]payload;
		}
	}

}

void Server::add_to_remove_from_favorite(char* payload, SOCKET& client_sock)
{
	auto tokens = Frame::split_payload(payload);
	char resp = '0';

	if (tokens[2] == "0")
	{
		resp = add_favorite_status_to_database(tokens);
	}
	else
	{
		resp = remove_favorite_status(tokens);
	}

	::send(client_sock, &resp, sizeof(resp), 0);
}

char Server::add_favorite_status_to_database(std::vector<std::string> info)
{
	std::vector<std::vector<std::string>>results;
	m_db->fetch_data("SELECT *FROM Favorite WHERE [Id anunt] = "+ info[0] + "AND[Username] = '"+info[1]+"'", results);

	if (!results.empty())
	{
		return '2';
	}

	//INSERT INTO Favorite ([Id anunt],[Username]) VALUES ('8','alex_stoica');
	info.pop_back();
	std::vector<std::vector<std::string>>data;
	data.push_back(info);

	if (m_db->insert_data("Favorite", { "[Id anunt]","[Username]" }, data))
	{
		Logger::get_instance().lgi(log_level::ERR, "add_favorite_status_to_database", "Favorite status couldn't be added.", "Ad number : " + info[0] + " Username : " + info[1]);
		return '1';
	}
	else
	{
		return '0';
	}

}

char Server::remove_favorite_status(std::vector<std::string> info)
{
	std::vector<std::vector<std::string>>results;
	m_db->fetch_data("SELECT *FROM Favorite WHERE [Id anunt] = " + info[0] + "AND[Username] = '" + info[1] + "'", results);

	if (results.empty())
	{
		return '5';
	}

	//INSERT INTO Favorite ([Id anunt],[Username]) VALUES ('8','alex_stoica');

	std::vector<std::vector<std::string>>data;
	data.push_back(info);

	if(m_db->remove_ad_from_favorite(info[0],info[1]))
	{
		Logger::get_instance().lgi(log_level::ERR, "remove_favorite_status", "Favorite status couldn't be removed.", "Ad number : " + info[0] + " Username : " + info[1]);
		return '4';
	}
	else
	{
		return '3';
	}
}

bool Server::delete_ad(char* payload)
{
	if (m_db->delete_ad(payload))
	{
		Logger::get_instance().lgi(log_level::ERR, "delete_ad", "Ad coudn't be deleted.", "Ad number : " + std::string(payload));
		return 1;
	}

	Logger::get_instance().lgi(log_level::INFO, "delete_ad", "Ad successfully deleted.", "Ad number : " + std::string(payload));
	return 0;
}

void Server::delete_ad(char* payload, SOCKET& client_socket)
{
	char resp = '1';

	m_db->free_handle();

	if (delete_ad(payload))
	{
		resp = '0';
	}
	

	::send(client_socket, &resp, sizeof(resp), 0);
}

void Server::add_funds(char* payload, SOCKET& client_socket)
{
	auto tokens = Frame::split_payload(payload);
	char resp = '0';
	
	if (m_db->add_funds(tokens[0], tokens[1]))
	{
		Logger::get_instance().lgi(log_level::ERR, "add_funds", "Sold coudn't be added.", "Username : " + tokens[0]+ " Value : "+tokens[1]);
		resp = '1';
	}
	else
	{
		Logger::get_instance().lgi(log_level::INFO, "add_funds", "Sold added successfully .", "Username : " + tokens[0] + " Value : " + tokens[1]);
		resp = '0';
	}

	::send(client_socket, &resp, sizeof(resp), 0);
}

void Server::buy_property(char* payload, SOCKET& client_sock)
{
	auto tokens = Frame::split_payload(payload);
	char resp = '0';

	auto sold = m_db->get_sold(tokens[0]);

	if (compare_values(sold, tokens[2]))
	{
		if (m_db->buy_property(tokens[0], tokens[1]))
		{
			resp = '2';
		}
	}
	else
	{
		resp = '1';
		//fonduri insuficiente
	}

	m_db->free_handle();

	::send(client_sock, &resp, sizeof(resp), 0);
	
}


bool Server::compare_values(std::string& value1, std::string value2)
{
	auto value1_f = std::stof(value1);
	auto value2_f = std::stof(value2);

	if (value1_f > value2_f)
		return 1;

	return 0;
}

void Server::block_ad(char* payload, SOCKET& client_sock)
{
	char resp = '0';
	if (m_db->block_ad(payload))
	{
		resp = '1';
	}

	::send(client_sock, &resp, sizeof(resp), 0);
}

void Server::unblock_ad(char* payload, SOCKET& client_sock)
{
	char resp = '0';

	if (m_db->unblock_ad(payload))
	{
		resp = '1';
	}

	::send(client_sock, &resp, sizeof(resp), 0);
}

void Server::send_chat(char* payload, SOCKET& client_sock)
{
	send_chat_users(payload, client_sock);
	//Frame::receive_ack(client_sock);
	Frame::send_data((char*)"end", strlen("end"), client_sock, data_type::chu);
}

void Server::send_specific_chat(char* payload, SOCKET& client_sock)
{
	auto tokens = Frame::split_payload(payload);
	//send_chat_users(tokens[0].data(), client_sock);
	send_chat_messages(tokens[0], tokens[1], client_sock);

	Frame::receive_ack(client_sock);
	Frame::send_data((char*)"end", strlen("end"),client_sock,data_type::cha);
}

void Server::send_chat_users(char* payload, SOCKET& client_sock)
{
	std::vector<std::string>users = m_db->fetch_chat_users(payload);

	send_ad_ids(users, data_type::chu, client_sock);

}

void Server::send_chat_messages(std::string username_1, std::string username_2, SOCKET& client_sock)
{
	auto result = m_db->fetch_chat_messages(username_1, username_2);

	for (auto row : result)
	{
		char* payload = Frame::concat_inputs(row);
		Frame::send_data(payload, strlen(payload),client_sock,data_type::chm);
		Frame::receive_ack(client_sock);

		delete[]payload;
	}

	if (result.empty())
	{
		Frame::send_data((char*)"empty", strlen("empty"), client_sock, data_type::chm);
	}
}

void Server::add_message_into_database(char* payload, SOCKET& client_sock)
{
	//INSERT INTO Mesaje ([Id sender],[Id receiver],[Mesaj]) VALUES ('ana_maria','alex_stoica','cf')
	auto tokens = Frame::split_payload(payload);
	char resp = '0';

	std::vector<std::vector<std::string>>data;
	data.push_back(tokens);
	if (m_db->insert_data("Mesaje", { "[Id sender]","[Id receiver]","[Mesaj]"}, data))
	{
		Logger::get_instance().lgi(log_level::ERR, "add_message_into_database", "Adding chat message failed");
		resp = '1';
		
	}

	Logger::get_instance().lgi(log_level::INFO, "add_message_into_database", "Message addes succesfully " );

	::send(client_sock, &resp, sizeof(resp),0);
}

void Server::send_funds(char* payload, SOCKET& client_sock)
{
	std::string value = m_db->get_sold(payload);

	Frame::send_data(value.data(), value.size(),client_sock,data_type::sld);
}

void Server::delete_user(char* payload, SOCKET& client_sock)
{
	char resp = '0';

	if (m_db->delete_user(payload))
	{
		resp = '1';
	}

	::send(client_sock, &resp, sizeof(resp), 0);
}

void Server::send_favorite_feed(char* payload, SOCKET& client_sock)
{
	std::vector<std::string>ad_id = m_db->fetch_favorite_ad_ids(payload);
	
	send_ad_ids(ad_id, data_type::rqf, client_sock);

	Frame::send_data((char*)"end", strlen("end"), client_sock, data_type::rqf);
}

void Server::send_profile_feed(char* payload, SOCKET& client_sock)
{
	std::vector<std::string>ad_id = m_db->fetch_feed_ad_ids(payload);

	send_ad_ids(ad_id, data_type::rqp, client_sock);

	Frame::send_data((char*)"end", strlen("end"), client_sock, data_type::rqf);
}

void Server::send_ad_ids(std::vector<std::string>& ad_id, data_type type,SOCKET&client_sock)
{
	char* payload = nullptr;

	if (!ad_id.empty())
	{
		payload = Frame::concat_inputs(ad_id);
	}
	else
	{
		payload = new char[6];
		strcpy(payload, "empty");
		payload[5] = '\0';
	}

	
	Frame::send_data(payload, strlen(payload), client_sock, type);
	Frame::receive_ack(client_sock);
	//Frame::send_data((char*)"end", strlen("end"), client_sock, data_type::rqf);

}

void Server::send_ads_nr(SOCKET client_sock)
{
	int id = 0;
	 m_ad_nr= m_db->get_ads_nr();
	 if(m_ad_nr!="NULL")
	 { 
		 id = std::stoi(m_ad_nr);
	 }
	 
	 id++;
	 m_ad_nr = std::to_string(id);
	::send(client_sock, m_ad_nr.c_str(), m_ad_nr.size(), 0);
}

void Server::save_image(char* payload, SOCKET& client_sock, int &add_post)
{
	int img_size;
	char* ad_nr;
	char*img_ext;

	Frame::get_image_details(payload,ad_nr,img_ext, img_size);

	Frame::send_ack(client_sock);

	//Frame::receive_ack(client_sock);

	std::string path = "C:\\Users\\laris\\Desktop\\ATM\\Anul II\\Semestrul II\\POO\\ProiectPOO_bun\\photos\\";
	path+="img" + std::string(ad_nr) + "_" + m_db->get_image_number(ad_nr)+img_ext;

	if (Frame::save_image(path, client_sock, img_size))
	{
		Logger::get_instance().lgi(log_level::ERR, "save_image", "Image coudn't be saved.");

	}
	else
	{
		Logger::get_instance().lgi(log_level::INFO, "save_image", "Image saved.");
	}

	insert_image_info_into_database(ad_nr, path);

	Frame::send_ack(client_sock);
	//Frame::receive_ack(client_sock);
	//if (add_post)
	//{
	//	Frame::send_ack(client_sock);
	//}

	delete[]img_ext;
	delete[]ad_nr;
	img_ext = nullptr;
	Frame::send_ack(client_sock);

}

char Server::insert_image_info_into_database(char* ad_nr, std::string& image_path)
{
	//INSERT INTO Imagine([Id anunt], Cale) VALUES('1', 'C://....');

	std::vector<std::vector<std::string>>data(1);

	data[0].push_back(std::string(ad_nr));
	data[0].push_back(image_path);


	if (m_db->insert_data("Imagine", { "[Id anunt]","Cale" }, data))
	{
		Logger::get_instance().lgi(log_level::ERR, "insert_image_info_into_database", "Image couldn't be inserted into database.");
		return '1';
	}

	Logger::get_instance().lgi(log_level::INFO, "insert_image_info_into_database", "Image info inserted into database.");

	return '0';
}

void Server::handle_incoming_connections(FD_SET& master)
{
	if (FD_ISSET(m_listen_sock, &master))
	{

		SOCKET new_client_sock = accept(m_listen_sock, nullptr, nullptr);

		FD_SET(new_client_sock, &master);
		m_client_socks.push_back(new_client_sock);

		if (new_client_sock != INVALID_SOCKET)
		{

			std::cout << "Client connected\n";

			Logger::get_instance().lgi(log_level::INFO, "handle_incoming_connections", "Client connected from IP " + /*std::string(get_client_ip_adress(new_client_sock))*/get_client_ip_adress(new_client_sock), "no");

		}
		else
		{
			//Logger::get_instance().lgi(log_level::INFO, "handle_incoming_connections", "Creating socket failed.Client couldn't connect.", "no");
		}

	}
}

void Server::wait_connection()
{
	FD_SET	master;
	init_master(master);

	while (1)
	{
		handle_incoming_connections(master);
		handle_client_data(master);
	}
}

