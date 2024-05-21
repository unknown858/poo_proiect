#define _CRT_SECURE_NO_WARNINGS
#include "client.h"
#include "frameutils.h"

#include <cstdio>
#include <iostream>
#include <stdio.h>

#include <QMessageBox>


Client::Client()
{
    WSADATA wsaData;

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    //inet_pton(AF_INET, "172.16.41.161", &clientService.sin_addr.s_addr);
    //inet_pton(AF_INET, "127.0.0.1", &clientService.sin_addr.s_addr);
    inet_pton(AF_INET, "172.20.10.3", &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(12345);
    int ret = connect(client_sock, (sockaddr*)&clientService, sizeof(clientService));
    if (ret == SOCKET_ERROR) {
        printf("connect function failed with error: %d\n", WSAGetLastError());

    }
}

Client::~Client()
{
    closesocket(client_sock);
    WSACleanup();
}

void Client::get_fav_ad_ids(char* payload)
{
    auto tokens = Frame::split_payload(payload);

    if(tokens[0]=="empty")
    {
        return;
    }

    for (auto token : tokens)
    {
        for(auto post :m_listaPostari)
        {
            if (post->getIdPostare()==std::stoi(token))
            {
                this->m_favorite.push_back(post);
            }
        }
    }

    std::cout << "\n";
}

void Client::send_message(const char* buffer)
{
    send(client_sock, buffer, strlen(buffer) + 1, 0);
}

int Client::log_in(const char* username, const char* parola)
{

    char* payload = Frame::concat_inputs(username, parola, nullptr);
    Frame::send_data(payload,strlen(payload),client_sock,data_type::lgi);

    delete[]payload;

    char rasp;
    ::recv(client_sock, &rasp, sizeof(rasp), 0);

    if (rasp=='0')
    {
        this->setAdmin(false);
        return 0; //date corecte
    }
    else if(rasp=='1')
    {
        return 1; //username invalid
    }
    else if(rasp=='2')
    {
        return 2; //Parola invalida
    }
    else if ( rasp == '3')
    {
        this->setAdmin(true);
        return 0;
    }


    return -1;
}

int Client::sign_in(const char* nume, const char* prenume, const char* email, const char* nr_tel, const char* username, const char* parola)
{
    char* payload = Frame::concat_inputs(nume, prenume, email, nr_tel, username, parola,nullptr);
    Frame::send_data(payload, strlen(payload), client_sock, data_type::sgi);
    delete[]payload;

    char rasp;
    ::recv(client_sock, &rasp, sizeof(rasp), 0);

    if (rasp == '0')
    {
        return 0; //Inregistrare efectuata cu succes
    }
    else if(rasp=='1')
    {
        return 1; //Inregistrare esuata
    }
    else if (rasp == '2')
    {
        return 2; //Username existent
    }

    return -1;
}

void Client::create_ad(const char* username, ad_type ad_t, property_type prp_t, const char* description, const char* price, const char* city, const char* surface, const char* blocked)
{

    char* c_ad_t = ad_type_to_char(ad_t);
    char* c_prp_t = property_type_to_char(prp_t);
    char* payload = Frame::concat_inputs(username, c_ad_t, c_prp_t, description, price, city, surface,blocked,nullptr);

    delete[]c_ad_t;
    delete[]c_prp_t;

    Frame::send_data(payload, strlen(payload), client_sock, data_type::adl);

    delete[]payload;

    char resp;
    ::recv(client_sock, &resp, sizeof(resp), 0);

    if (resp == '0')
    {
        std::cout << "Anunt adaugat cu succes.\n";
    }
    else
    {
        std::cout << "Eroare la adaugarea anuntului.\n";
    }
}

void Client::create_ad(const char* username, ad_type ad_t, property_type prp_t, const char* description, const char* price, const char* city, const char* surface, const char* blocked, const char* rooms_nr, const char* bathroom_nr, const char* useful_surface)
{
    char* c_ad_t = ad_type_to_char(ad_t);
    char* c_prp_t = property_type_to_char(prp_t);
    char* payload = Frame::concat_inputs(username, c_ad_t, c_prp_t, description, price, city,surface,blocked,rooms_nr,bathroom_nr,useful_surface, nullptr);

    delete[]c_ad_t;
    delete[]c_prp_t;

    Frame::send_data(payload, strlen(payload), client_sock, data_type::adh);

    delete[]payload;

    char resp;
    ::recv(client_sock, &resp, sizeof(resp), 0);

    if (resp == '0')
    {
        std::cout << "Anunt imobil adaugat cu succes.\n";
    }
    else
    {
        std::cout << "Eroare la adaugarea anuntului imobil.\n";
    }
}

char* Client::ad_type_to_char(ad_type type)
{
    char* c_type = nullptr;

    switch (type)
    {
    case ad_type::sale:
        c_type = new char[strlen("vanzare") + 1];
        strcpy(c_type, "vanzare");
        c_type[strlen("vanzare")] = '\0';
        break;
    case ad_type::rent:
        c_type = new char[strlen("inchiriere") + 1];
        strcpy(c_type, "inchiriere");
        c_type[strlen("inchiriere")] = '\0';
        break;
    }

    return c_type;
}

char* Client::property_type_to_char(property_type type)
{
    char* c_type = nullptr;

    switch (type)
    {
    case property_type::house:
        c_type = new char[strlen("casa") + 1];
        strcpy(c_type, "casa");
        c_type[strlen("casa")] = '\0';
        break;
    case property_type::land:
        c_type = new char[strlen("teren") + 1];
        strcpy(c_type, "teren");
        c_type[strlen("teren")] = '\0';
        break;
    case property_type::apartment:
        c_type = new char[strlen("apartament") + 1];
        strcpy(c_type, "apartament");
        c_type[strlen("apartament")] = '\0';
        break;
    }

    return c_type;
}

char* Client::request_next_ad_nr()
{
    Frame::send_data((char*)"id_postare_viitoare", 19, client_sock, data_type::req);

    char buff[3];

    int bytes=::recv(client_sock, buff, strlen(buff)-1, 0);

    buff[bytes] = '\0';

    char* id_postare = new char[bytes + 1];
    strcpy(id_postare, buff);
    id_postare[strlen(id_postare)] = '\0';

    return id_postare;
}

void Client::request_feed(const char* sort_type)
{
    Frame::send_data((char*)sort_type, strlen(sort_type), client_sock, data_type::srt);

    receiving_data();
}

void Client::handle_data_by_type(char* payload, data_type type,int&img_nr, const char* ad_id, int tipFeed)
{
    switch (type)
    {
    case data_type::img:
        save_image(payload,img_nr, ad_id, tipFeed);
        img_nr++;
        break;
    case data_type::ads:
        get_ad_summary(payload, tipFeed);
        Frame::send_ack(Client::getInstance().client_sock);
        break;
    case data_type::add:
        get_ad_details(payload);
        Frame::send_ack((Client::getInstance().client_sock));
        break;
    case data_type::rev:
        get_review(payload, ad_id);
        Frame::send_ack((Client::getInstance().client_sock));
        break;
    case data_type::dlt:
        delete_ad(payload);
        break;
    case data_type::rqf:
        get_fav_ad_ids(payload);
        Frame::send_ack(client_sock);
        break;
    case data_type::chu:
        get_users(payload);
        Frame::send_ack(client_sock);
        break;
    case data_type::chm:
        get_message(payload);
        Frame::send_ack(client_sock);
        break;
    default:
        break;
    }


}


void Client::save_image(char* payload, int image_nr, const char* ad_id, int tipFeed)
{
    int img_size;
    char* ad_nr;
    char* img_ext;

    Frame::get_image_details(payload, ad_nr, img_ext, img_size);
    Frame::send_ack(client_sock);

    std::string path = "C:/imagini/postari_primite/";
    path += "img" + std::string(ad_nr) + "_" + std::to_string(image_nr) + img_ext;
    Client::getInstance().adaugaCaleInVector(path, std::stoi(ad_nr));


    if (Frame::save_image(path, client_sock, img_size))
    {
        std::cout << "Imaginea nu a putut fi salvata.\n";

    }
    else
    {
        std::cout << "Imaginea  afost salvata.\n";
    }

    delete[]img_ext;
    delete[]ad_nr;
    img_ext = nullptr;

}

void Client::get_ad_summary(char* payload, int tipFeed)
{
    //[Id ofertant] , [Tip anunt], [Tip proprietate], Pret, Oras, Suprafata, [Postare blocata] [id_postare]
    auto tokens = Frame::split_payload(payload);


    TipProprietate tipPr;
    if(tokens[2]=="casa")
    {
        tipPr=TipProprietate::casa;
    }
    else if(tokens[2]=="apartament")
    {
        tipPr=TipProprietate::apartament;
    }
    else
    {
        tipPr=TipProprietate::teren;
    }

    if(tipPr==TipProprietate::teren)
    {
        Client::getInstance().adaugaPostare(new Teren(tipPr, std::stof(tokens[3]), std::stof(tokens[5]), tokens[1],
                                              tokens[0], tokens[4], std::stoi(tokens[6]), std::stoi(tokens[7])));
    }
    else
    {
        Client::getInstance().adaugaPostare(new CasaApartament(tipPr, std::stof(tokens[3]), std::stof(tokens[5]), tokens[1],
                                                  tokens[0], tokens[4], std::stoi(tokens[6]), std::stoi(tokens[7])));
    }
}

void Client::adaugaCaleInVector(std::string cale, int idPostare)
{
    for(auto post : m_listaPostari)
    {
        if (post->getIdPostare() == idPostare)
        {
            post->adaugaCaleImagine(cale);
        }
    }
    //this->m_listaPostari[this->nrPostari]->adaugaCaleImagine(cale);
}

void Client::add_review(const char* ad_nr, const char* username, const char* message)
{
    char* payload = Frame::concat_inputs(ad_nr, username, message,nullptr);
    Frame::send_data(payload, strlen(payload), client_sock, data_type::rev);
    delete[]payload;

    char resp;
    ::recv(client_sock, &resp, sizeof(resp), 0);

    if (resp == '0')
    {
        std::cout << "Review adaugat cu succes.\n";
    }
    else
    {
        std::cout << "Eroare la adaugarea review-ului.\n";
    }

}

void Client::request_ad_details(const char* ad_id, property_type prt_t)
{
    char* c_prt_t = property_type_to_char(prt_t);
    char* payload = Frame::concat_inputs(ad_id,c_prt_t,nullptr);
    Frame::send_data(payload, strlen(payload), client_sock, data_type::add);

    delete[]payload;
    delete[]c_prt_t;

    receiving_data(ad_id);
}

void Client::add_to_remove_from_favorite(const char* ad_nr, std::string username, const char *action)
{
    // action - 0 ->adauga la favorite
    //		  - 1 -> sterge de la favorite

    char* payload = Frame::concat_inputs(ad_nr, username.data(),action, nullptr);
    Frame::send_data(payload, strlen(payload), client_sock, data_type::fav);

    delete[]payload;

    char resp;
    ::recv(client_sock, &resp, sizeof(resp), 0);

}

void Client::delete_ad(const char* ad_nr)
{
    Frame::send_data((char*)ad_nr, strlen(ad_nr),client_sock,data_type::dlt);

    char resp;
    ::recv(client_sock, &resp, sizeof(resp), 0);

    if(resp == 0)
    {
        this->setAddDeleted(false);
    }
    else
    {
        this->setAddDeleted(true);
    }

}


void Client::get_ad_details(char* payload)
{
    //SELECT [Tip proprietate],Descriere, Data FROM Anunt WHERE Id=2  ->teren  idpostare
    //SELECT A.[Tip proprietate], A.Descriere, AI.[Numar camere], AI.[Numar bai], AI.[Suprafata utila], A.Data FROM Anunt A JOIN
    //[Anunt imobil] AI ON A.Id = AI.[Id anunt] WHERE A.Id =2 ->casa    idpostare

    auto tokens = Frame::split_payload(payload);
    int valoare = 0;
    std::cout<<valoare<<std::endl;
    if(tokens[0] == "teren")
    {
        for(auto post : m_listaPostari)
        {
            if(post->getIdPostare() == std::stoi(tokens[3]))
            {
                post->setData(tokens[2]);
                post->setDescriere(tokens[1]);
                break;
            }
        }
    }
    else
    {
        for(auto post : m_listaPostari)
        {
            if(post->getIdPostare() == std::stoi(tokens[6]))
            {
                post->setData(tokens[5]);
                post->setDescriere(tokens[1]);
                valoare = std::stoi(tokens[2]);
                std::cout<<valoare<<std::endl;
                post->setNrCamere(valoare);
                break;
            }
        }
    }

}

void Client::get_review(char* payload, const char* ad_id)
{
    //"SELECT [Id user], Mesaj FROM Reviews WHERE [Id anunt]="
    std::cout<<"Functia get_review\n";
    auto tokens = Frame::split_payload(payload);
    this->adaugaFeedback(ad_id, tokens[0], tokens[1], tokens[2]);
}


void Client::receiving_data(const char* ad_id)
{
    static int img_nr = 0;
    int tipFeed = 0; //1 normal 2 favorite 3 profil
    while (1)
    {

        char buff[FRAME_SIZE + 1];
        int recv_byte = ::recv(client_sock, buff, FRAME_SIZE, 0);

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

            if ((type == data_type::ads || type==data_type::add || type==data_type::rqf || type == data_type::cha || type == data_type::chu) && strcmp(payload, (char*)"end") == 0)
            {
                img_nr = 0;

                break;
            }

            if (img_nr == 0 && type==data_type::add)
            {
                img_nr = 1;
            }

            if(type == data_type::sld)
            {
                auto sold = std::stof(payload);
                this->setSoldClient(sold);
                std::cout << "Soldul este: " << sold << "\n";
                break;
            }

            handle_data_by_type(payload, type, img_nr, ad_id, tipFeed);

        }
    }
}


void Client::request_feed_fav(std::string username)
{
    char* payload = Frame::concat_inputs(username.data(), nullptr);
    Frame::send_data(payload, strlen(payload), client_sock, data_type::rqf);

    delete[]payload;

    receiving_data();
}

void Client::request_feed_profile(std::string username)
{
    char* payload = Frame::concat_inputs(username.data(), nullptr);
    Frame::send_data(payload, strlen(payload), client_sock, data_type::rqp);

    delete[]payload;

    receiving_data();
}

void Client::add_funds(const char* value,std::string username)
{
    char* payload = Frame::concat_inputs(username.data(),value,nullptr);
    Frame::send_data(payload, strlen(payload), client_sock, data_type::fnd);

    delete[]payload;

    char resp;
    ::recv(client_sock, &resp, sizeof(resp), 0);
    if (resp == '0')
    {
        this->set_res(0);
    }
    else if (resp == '1')
    {
        this->set_res(1);
    }

}


void Client::buy_property(std::string username_b, const char* ad_id, const char*value)
{
    char* payload = Frame::concat_inputs(username_b.data(), ad_id,value, nullptr);
    Frame::send_data(payload, strlen(payload), client_sock, data_type::buy);

    delete[]payload;

    char resp;
    ::recv(client_sock, &resp, sizeof(resp), 0);

    std::string str = std::to_string(resp);

    int str1 = (int)resp - 48;

    this->adaugaStatusTranzactie(str1);
}

void Client::block_post(const char* ad_nr)
{
    Frame::send_data((char*)ad_nr, strlen(ad_nr), client_sock, data_type::blk);

    char resp;
    ::recv(client_sock, &resp, sizeof(resp), 0);

    for(auto post : this->m_listaPostari)
    {
        if (strcmp(std::to_string(post->getIdPostare()).c_str(), ad_nr) == 0)
        {
            if(resp == '0')
            {
                post->setStatusPostare(true);
            }
            else
            {
                post->setStatusPostare(false);
            }
        }
    }
}



void Client::request_chat(std::string username)
{
    Frame::send_data(username.data(), username.size(), client_sock, data_type::chu);

    receiving_data();
}


void Client::request_specific_chat(std::string username_b, std::string username_s)
{
    //select [Id sender], [Id receiver], Mesaj,[Timp trimitere] from Mesaje
    char* payload = Frame::concat_inputs(username_b.data(), username_s.data(),nullptr);

    Frame::send_data(payload, strlen(payload), client_sock, data_type::cha);

    delete[]payload;

    receiving_data();

}

void Client::send_message1(std::string username_1, std::string username_2, std::string message)
{
    char* payload = Frame::concat_inputs(username_1.data(), username_2.data(),message.data(), nullptr);

    Frame::send_data(payload, strlen(payload), client_sock, data_type::sdm);

    delete[]payload;

    char resp;
    ::recv(client_sock, &resp, sizeof(resp), 0);

    if (resp == '0')
    {
        std::cout << "Mesajul a fost trimis";
    }
    else if (resp == '1')
    {
        std::cout << "Eroare la trimiterea mesajului";
    }

}

void Client::get_users(char* payload)
{
    auto tokens = Frame::split_payload(payload);

    if(tokens[0] == "empty")
    {
        return;
    }

    for (auto token : tokens)
    {
        this->adaugaUtilizator(token);
    }
}

void Client::get_message(char* payload)
{
    //SELECT [Id sender], [Id receiver], Mesaj FROM Mesaje ORDER BY [Timp trimitere] ASC
    auto tokens = Frame::split_payload(payload);

    if(tokens[0] == "empty")
    {
        return;
    }

    this->aduagaSenderMesaj(tokens[0], tokens[2]);
    Frame::send_ack((Client::getInstance().client_sock));

    std::cout << "\n";
}

void Client::request_funds(std::string username)
{
    Frame::send_data(username.data(), username.size(), client_sock, data_type::sld);

    receiving_data();
}


void Client::delete_user(std::string username)
{
    Frame::send_data(username.data(), username.size(), client_sock, data_type::dlu);

    char resp;
    ::recv(client_sock, &resp, sizeof(resp), 0);

    if (resp == '0')
    {
        this->setStergereOfertant(0);
    }
    else if (resp == '1')
    {
        this->setStergereOfertant(1);
    }
}

void Client::unblock_post(const char* ad_nr)
{
    Frame::send_data((char*)ad_nr, strlen(ad_nr), client_sock, data_type::ubk);

    char resp;
    ::recv(client_sock, &resp, sizeof(resp), 0);

    for(auto post : this->m_listaPostari)
    {
        if (strcmp(std::to_string(post->getIdPostare()).c_str(), ad_nr) == 0)
        {
            if(resp == '0')
            {
                post->setStatusPostare(true);
            }
            else
            {
                post->setStatusPostare(false);
            }
        }
    }
}
