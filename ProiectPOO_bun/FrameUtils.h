#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <vector>
#include <string>

#include <filesystem>


#define FRAME_SIZE 3009				//10008

#define HEADER_SIZE	7		
#define DATA_TYPE 3
#define PAYLOAD_LENGTH 4
#define PAYLOAD_MAX_SIZE 3000			 //9999
#define END_DELIMITER 2

const char  end_delimiter[] = "##";

//	   type_data  payload_size   payload   last_frame    end of frame
//	   |-------| |-------------| |------|    |---|	   |--------------|   
//	    3 bytes	     4 bytes	    1030	   1			  2 bytes


//last frame :
//		0 -> pachet care nu e sf datelor
//		1 -> pachet de sfarsit  


//data type:
//
//	lgi (log in)   -> username#parola
//	sgi (sign in)  -> username#parola#extra1#extra2#...
//	adl (ad land)  -> username#tip_anunt#tip_proprietate#descriere#pret#oras#suprafata#anunt_blocat
//  adh (ad homes) -> username#tip_anunt#tip_proprietate#descriere#pret#oras#suprafata#nr_camere#anunt_blocat#nr_bai#suprafata_utila
//	rev (review)   -> id_anunt#username#mesaj
//
//							byte_0       byte_1			  byte_2		 byte_3				byte_4					 byte_5
//  srt (sort ads) ->	  |-------|    |-------|		|-------|		|-------|		  |-------|				    |-------|		
//						
//						  0 - default   0 - default		0 - default      0 - default	   0 - default				0 - pret				
//						  1 - casa      1 - 0-500		1 - vanzare		 1 - 0-50		   1 - crescator			1 - dimesniune
//						  2 - apart     2 - 500-1000	2 - inchiriere	 2 - 50-100		   2 - descrescator			
//						  3 - teren      ....							.....									   
//			
//
//						   tip_prop		  pret			tip_anunt		  dimensiune	   tip_sortare		criteriu_sortare
//		
//	ads (ad summary) -> username#tip_anunt#tip_proprietate#pret#oras#suprafata#anunt_blocat
// 
//	add (ad details) -> descriere#nr_camere#nr_bai#suprafata_utila   (apartament / casa)
//					 ->	descriere					 				 (teren)
// 
//  dlt (delete) -> id_anunt 
//	rqf (request favorite) -> username
// 
//	rqp (request profile) -> username
//  fnd (funds) ->
//  buy -> 
//  chm (chat messages) -> username_s#username_r
//  chu (chat usernames) -> username
//  cha (chat all) -> username_s#username_r
//	sdm (send message) -> username_s#username_r#mesaj
//	sld (sold) -> username
//  ubk (unblock) -> id_ad


enum class data_type { img, lgi, sgi, adl, adh, req, rev, srt, ads, add, fav, dlt, rqf, rqp, fnd, buy, chm, chu, cha, blk, ubk, sdm, sld, dlu};

class Frame
{
	static void set_data_type(char*& frame, data_type type);
	static void set_payload_size(char*& frame, int size);
	static void add_payload(char*& frame, char* payload, int payload_size);
	static void set_end_delimiter(char*& frame, int payload_size);
	static void set_last_frame_byte(char*& frame, char last_frame, int payload_size);

	static char* convert_frame_size_to_char(int frame_size);
	static void swap(char& c1, char& c2);

	static int convert_char_to_int(char* frame_size);

	static data_type get_type(char *frame);
	static void set_type(char*& frame, const char type[4]);

	static void send_image_details(const char* ad_nr, const std::string& ext, int size, SOCKET& sock);

public:

	static char* create_frame(char* payload, int payload_size, char last_frame, data_type type);
	static bool send_data(char* payload, int payload_size, SOCKET& sock, data_type type);
	static void get_frame_details(char* frame, data_type& type, int& payload_size, char*& payload, char& last_frame);
	
	static char* concat_inputs(const char* first_input, ...);
	static std::vector<std::string>split_payload(char* payload);

	static char* concat_inputs(std::vector<std::string> &input);

	static void send_ack(SOCKET& sock);
	static void receive_ack(SOCKET& sock);

	static bool save_image(const std::string& image_path,SOCKET& sock, int size);
	static void get_image_details(char* payload, int& size, char*& ext);
	static void get_image_details(char* payload, char*& ad_nr, char*& ext, int& size);

	static bool send_image(const std::string& image_path, char* ad_nr, SOCKET& sock);

};

