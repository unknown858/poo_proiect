#include "FrameUtils.h"
#include <fstream>
#include <string>
#include<iostream>

void Frame::set_type(char*& frame, const char type[4])
{
    frame[0] = type[0];
    frame[1] = type[1];
    frame[2] = type[2];
}

void Frame::set_data_type(char*& frame, data_type type)
{
    switch (type)
    {
    case data_type::lgi:
        set_type(frame, "lgi");
        break;
    case data_type::sgi:
        set_type(frame, "sgi");
        break;
    case data_type::req:
        set_type(frame, "req");
        break;
    case data_type::img:
        set_type(frame, "img");
        break;
    case data_type::adl:
        set_type(frame, "adl");
        break;
    case data_type::adh:
        set_type(frame, "adh");
        break;
    case data_type::rev:
        set_type(frame, "rev");
        break;
    case data_type::srt:
        set_type(frame, "srt");
        break;
    case data_type::ads:
        set_type(frame, "ads");
        break;
    case data_type::add:
        set_type(frame, "add");
        break;
    case data_type::fav:
        set_type(frame, "fav");
        break;
    case data_type::dlt:
        set_type(frame, "dlt");
        break;
    case data_type::rqf:
        set_type(frame, "rqf");
        break;
    case data_type::rqp:
        set_type(frame, "rqp");
        break;
    case data_type::fnd:
        set_type(frame, "fnd");
        break;
    case data_type::buy:
        set_type(frame, "buy");
        break;
    case data_type::chm:
        set_type(frame, "chm");
        break;
    case data_type::chu:
        set_type(frame, "chu");
        break;
    case data_type::cha:
        set_type(frame, "cha");
        break;
    case data_type::blk:
        set_type(frame, "blk");
        break;
    case data_type::ubk:
        set_type(frame, "ubk");
        break;
    case data_type::sdm:
        set_type(frame, "sdm");
        break;
    case data_type::sld:
        set_type(frame, "sld");
        break;
    case data_type::dlu:
        set_type(frame, "dlu");
        break;

    }

}

void Frame::swap(char& c1, char& c2)
{
    char tmp = c1;
    c1 = c2;
    c2 = tmp;
}

int Frame::convert_char_to_int(char* frame_size)
{
    int size = atoi(frame_size);
    return size;
}


data_type Frame::get_type(char *frame)
{
    if (strcmp(frame, "img") == 0)
    {
        return data_type::img;
    }
    else if (strcmp(frame, "lgi") == 0)
    {
        return data_type::lgi;
    }
    else if (strcmp(frame, "sgi") == 0)
    {
        return data_type::sgi;
    }
    else if(strcmp(frame,"req") == 0)
    {
        return data_type::req;
    }
    else if (strcmp(frame, "adl") == 0)
    {
        return data_type::adl;
    }
    else if (strcmp(frame, "adh") == 0)
    {
        return data_type::adh;
    }
    else if (strcmp(frame, "rev") == 0)
    {
        return data_type::rev;
    }
    else if (strcmp(frame, "srt") == 0)
    {
        return data_type::srt;
    }
    else if (strcmp(frame, "ads") == 0)
    {
        return data_type::ads;
    }
    else if (strcmp(frame, "add") == 0)
    {
        return data_type::add;
    }
    else if (strcmp(frame, "fav") == 0)
    {
        return data_type::fav;
    }
    else if (strcmp(frame, "dlt") == 0)
    {
        return data_type::dlt;
    }
    else if (strcmp(frame, "rqf") == 0)
    {
        return data_type::rqf;
    }
    else if (strcmp(frame, "rqp") == 0)
    {
        return data_type::rqp;
    }
    else if (strcmp(frame, "fnd") == 0)
    {
        return data_type::fnd;
    }
    else if (strcmp(frame, "buy") == 0)
    {
        return data_type::buy;
    }
    else if (strcmp(frame, "chm") == 0)
    {
        return data_type::chm;
    }
    else if (strcmp(frame, "chu") == 0)
    {
        return data_type::chu;
    }
    else if (strcmp(frame, "cha") == 0)
    {
        return data_type::cha;
    }
    else if (strcmp(frame, "blk") == 0)
    {
        return data_type::blk;
    }
    else if (strcmp(frame, "sdm") == 0)
    {
        return data_type::sdm;
    }
    else if (strcmp(frame, "sld") == 0)
    {
        return data_type::sld;
    }
    else if (strcmp(frame, "dlu") == 0)
    {
        return data_type::dlu;
    }
    else if (strcmp(frame, "ubk") == 0)
    {
        return data_type::ubk;
    }



}

void Frame::set_payload_size(char*& frame, int size)
{
    char* ch_size = convert_frame_size_to_char(size);

    for (int i = 0; i < PAYLOAD_LENGTH; i++)
    {
        frame[DATA_TYPE + i] = ch_size[i];
    }

    delete[]ch_size;

}

void Frame::add_payload(char*& frame, char* payload, int payload_size)
{
    memcpy(frame + HEADER_SIZE, payload, payload_size);
}

void Frame::set_end_delimiter(char*& frame, int payload_size)
{
    for (int i = 0; i < END_DELIMITER; i++)
    {
        frame[HEADER_SIZE + payload_size + 1 + i] = end_delimiter[i];
    }

    frame[HEADER_SIZE + payload_size + 1 + END_DELIMITER] = '\0';
}

void Frame::set_last_frame_byte(char*& frame, char last_frame, int payload_size)
{
    frame[HEADER_SIZE + payload_size] = last_frame;
}

char* Frame::create_frame(char* payload, int payload_size, char last_frame, data_type type)//,char *ip_adress)
{
    char* frame = new char[HEADER_SIZE + payload_size + 4];
    set_data_type(frame, type);
    set_payload_size(frame, payload_size);
    add_payload(frame, payload, payload_size);
    set_last_frame_byte(frame, last_frame, payload_size);

    set_end_delimiter(frame, payload_size);

    return frame;
}

void Frame::get_frame_details(char* frame, data_type& type, int& payload_size, char*& payload, char& last_frame)   //dez
{
    char c_type[DATA_TYPE+1];
    strncpy(c_type, frame, DATA_TYPE);
    c_type[DATA_TYPE] = '\0';
    type = get_type(c_type);

    char size[5];
    strncpy(size, frame + DATA_TYPE, PAYLOAD_LENGTH);

    payload_size = convert_char_to_int(size);

    payload = new char[payload_size + 1];
    strncpy(payload, frame + HEADER_SIZE, payload_size);

    last_frame = frame[HEADER_SIZE + payload_size];
    payload[payload_size] = '\0';

    //delete[]frame;
}

bool Frame::send_data(char* payload, int payload_size, SOCKET& sock, data_type type)
{
    if (payload == nullptr)
    {
        return 1;
    }

    int frames_nr = 0;

    frames_nr = payload_size / PAYLOAD_MAX_SIZE;
    int last_frame_size = payload_size % PAYLOAD_MAX_SIZE;


    for (int i = 0; i < frames_nr; i++)
    {
        char* frame = Frame::create_frame(payload + PAYLOAD_MAX_SIZE * i, PAYLOAD_MAX_SIZE, '0', type);

        std::cout << FRAME_SIZE << "\n";
        ::send(sock, frame, FRAME_SIZE + 1, 0);

        //delete[]frame;
        char ack;
        //::recv(sock, &ack, 1, 0);

    }

    if (last_frame_size > 0)
    {
        char* frame = Frame::create_frame(payload, last_frame_size, '1', type);
        std::cout << last_frame_size << "\n";
        ::send(sock, frame, HEADER_SIZE + last_frame_size + END_DELIMITER + 2, 0);
    }

    return 0;
}


char* Frame::convert_frame_size_to_char(int frame_size)
{
    char* size = new char[PAYLOAD_LENGTH + 1];
    size[PAYLOAD_LENGTH] = '\0';

    for (int i = 0; i < PAYLOAD_LENGTH; i++)
    {
        size[i] = '0';
    }
    int index = 0;

    while (frame_size)
    {
        char digit = frame_size % 10 + '0';
        size[index++] = digit;
        frame_size /= 10;
    }

    index = 4;

    for (int i = 0; i < index / 2; i++)
    {
        swap(size[i], size[index - i - 1]);
    }


    return size;
}


char* Frame::concat_inputs(const char* first_input, ...)
{
    va_list args;
    va_start(args, first_input);

    std::string result;
    const char* tmp = first_input;

    if (tmp != nullptr)
    {
        result += tmp;
        tmp = va_arg(args, const char*);
    }

    while (tmp != nullptr)
    {
        result += '#';
        result += tmp;
        tmp = va_arg(args, const char*);
    }

    va_end(args);

    char* c_result = new char[result.size() + 1];
    strcpy(c_result, result.c_str());
    return c_result;
}

std::vector<std::string> Frame::split_payload(char* payload)
{
    std::vector<std::string> tokens;
    const char* token = std::strtok(payload,"#");
    while (token != nullptr) {
        tokens.push_back(token);
        token = std::strtok(nullptr, "#");
    }
    return tokens;
}

char* Frame::concat_inputs(std::vector<std::string> &input)
{
    std::string payload = input[0];
    for (int i = 1; i < input.size(); i++)
    {
        payload += "#";
        payload += input[i];
    }

    char* c_payload = new char[payload.size() + 1];
    strcpy(c_payload, payload.c_str());
    return c_payload;
}

void Frame::send_ack(SOCKET& sock)
{
    char ack = 'A';
    ::send(sock, &ack, sizeof(ack), 0);
}


bool Frame::save_image(const std::string &image_path,SOCKET& sock, int size)
{
   // std::vector<char> buffer(size);
   // std::string filename = image_path;

   // int received = 0;

   // int bytes = ::recv(sock, buffer.data() ,size, 0);

   // if (bytes == SOCKET_ERROR || bytes == 0) 
   // {
   //     std::cerr << "Failed to receive image data" << std::endl;
   //     return 1;
   // }


   // std::ofstream image_file(filename, std::ios::out | std::ios::binary);

   // if (!image_file.is_open()) {
   //     std::cerr << "Failed to open image file for writing" << std::endl;
   //     return 1;
   // }

   // image_file.write(buffer.data(), size);

   // image_file.close();

   // std::cout << "Image saved as: " << filename << std::endl;
   //// send_ack(sock);
   // return 0;

    std::vector<char> buffer(size);
    std::string filename = image_path;

    int received = 0;

    int bytes = ::recv(sock, buffer.data(), size, 0);

    if (bytes == SOCKET_ERROR || bytes == 0)
    {
        std::cerr << "Failed to receive image data" << std::endl;
        return 1;
    }


    std::ofstream image_file(filename, std::ios::out | std::ios::binary);

    if (!image_file.is_open()) {
        std::cerr << "Failed to open image file for writing" << std::endl;
        return 1;
    }

    image_file.write(buffer.data(), size);

    image_file.close();

    std::cout << "Image saved as: " << filename << std::endl;

    return 0;
}


void Frame::get_image_details(char* payload, int& size, char*& ext)
{
    auto tokens = split_payload(payload);
    ext = new char[tokens[0].size() + 1];
    strcpy(ext, tokens[0].c_str());
    ext[tokens[0].size()] = '\0';
    size = convert_char_to_int((char*)tokens[1].c_str());
}

void Frame::get_image_details(char* payload, char*& ad_nr, char*& ext, int& size)
{
    auto tokens = split_payload(payload);

    ad_nr = new char[tokens[0].size() + 1];
    strcpy(ad_nr, tokens[0].c_str());
    ad_nr[tokens[0].size()] = '\0';

    ext = new char[tokens[1].size() + 1];
    strcpy(ext, tokens[1].c_str());
    ext[tokens[1].size()] = '\0';

    size = convert_char_to_int((char*)tokens[2].c_str());
}

bool Frame::send_image(const std::string& image_path, char* ad_nr, SOCKET& sock)
{
    std::ifstream image_file(image_path, std::ios::binary);

    if (!image_file.is_open()) {
        std::cerr << "Failed to open image file" << std::endl;
        return 1;
    }

    std::filesystem::path p(image_path);
    std::string extension = p.extension().string();


    image_file.seekg(0, std::ios::end);
    int image_size = image_file.tellg();
    image_file.seekg(0, std::ios::beg);

    send_image_details(ad_nr, extension, image_size, sock);

    std::vector<char> image_data(image_size);

    if (!image_file.read(image_data.data(), image_size))
    {
        std::cerr << "Failed to read image data from file" << std::endl;
        return 1;
    }
    image_file.close();

    ::send(sock, image_data.data(), image_size, 0);
   // Frame::receive_ack(sock);


    return 0;
}

void Frame::send_image_details(const char* ad_nr, const std::string& ext, int size, SOCKET& sock)
{
    auto payload_details = concat_inputs(ad_nr, ext.c_str(), std::to_string(size).c_str(), nullptr);
    auto frame = create_frame(payload_details, strlen(payload_details), '1', data_type::img);
    delete[]payload_details;

    ::send(sock, frame, strlen(frame), 0);
    delete[]frame;

    Frame::receive_ack(sock);
}

void Frame::receive_ack(SOCKET& sock)
{
    char ack;
    ::recv(sock, &ack, sizeof(ack), 0);

   while (ack != 'A')
    {
      ::recv(sock, &ack, sizeof(ack), 0);
    }
}