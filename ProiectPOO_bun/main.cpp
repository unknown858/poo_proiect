#include <iostream>
#include "Server.h"
#include "Database.h"


int main()
{
	Server& server = Server::get_instance();

	//server.m_db->delete_data("[Anunt]", "Id", "1");

	server.wait_connection();

 
	return 0;

}