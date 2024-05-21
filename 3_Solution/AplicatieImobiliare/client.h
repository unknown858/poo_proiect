#ifndef CLIENT_H
#define CLIENT_H

#define WIN32_LEAN_AND_MEAN

#include "frameutils.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "Proprietati.h"


enum class ad_type{sale,rent};

enum class property_type { house, land, apartment };

class Client
{
private:
    std::vector<Proprietate*> m_listaPostari;
    std::vector<Proprietate*> m_favorite;
    std::vector<std::string> m_utilizatoriAplicatie;
    std::vector<std::pair<std::string, std::string>> sender_mesaj;

    std::vector<IFeedback*> feed;
    int nrPostari = 0; //in feed

    bool admin=false;
    bool succesSold=false;
    int statusTranzactie;

    std::string m_username;

    float sold_client;
    bool addDeletedSuccesfully = false;

    int stergereOfertant;

    Client();
    ~Client();

    void handle_data_by_type(char* payload, data_type type,int&img_nr, const char* ad_id=" ", int tipFeed = 0);
    void save_image(char* payload, int image_nr, const char* ad_id = " ", int tipFeed = 0);
    void get_ad_summary(char* payload, int tipFeed);
    void get_fav_ad_ids(char* payload);


public:
    std::vector<IFeedback*> getAllFeedback()
    {
        return this->feed;
    }
    SOCKET client_sock;
    static Client& getInstance()
    {
        // Se va crea un obiect static la prima apelare a acestei metode
        static Client instance;
        return instance;
    }
    void send_message(const char* buffer);

    void unblock_post(const char* ad_nr);
    void send_image();
    int log_in(const char* username, const char* parola);
    int sign_in(const char* nume, const char* prenume, const char* email, const char* nr_tel, const char* username, const char* parola);
    void create_ad(const char* username, ad_type ad_t, property_type prp_t,const char* description,const char*price,const char* city,const char*surface,const char*bloacked);			//teren
    void create_ad(const char* username, ad_type ad_t, property_type prp_t,const char* description, const char* price,const char* city,const char* surface,const char*blocked,const char* rooms_nr, const char* bathroom_nr,const char* useful_surface); //apartament, casa
    char* ad_type_to_char(ad_type type);
    char* property_type_to_char(property_type type);
    char* request_next_ad_nr();
    void request_feed(const char* sort_type);
    void set_res(int val){this->statusTranzactie=val;}

    std::string getUsername(){return this->m_username;}
    void adaugaUsername(std::string username){this->m_username=username;}

    void adaugaPostare(Proprietate* pr) {
        this->m_listaPostari.push_back(pr);
        this->nrPostari++;
    }
    std::vector<Proprietate*> getPostari(){return this->m_listaPostari;}
    void adaugaCaleInVector(std::string cale, int idPostare);
    void clearVector(){
        for(auto post :m_listaPostari)
        {
            post->setDimensiunem_imagini0();
        }
        this->nrPostari = -1;
        this->m_listaPostari.clear();
    }

    void clearVectorFav(){

        this->m_favorite.clear();
    }

    void incNrPostari(){this->nrPostari++;}

    bool estePostareInFavorite(Proprietate* pr) {
        for (auto postare : m_favorite) {
            if (postare == pr) {
                return true;
            }
        }
        return false;
    }


    std::vector<Proprietate*> getFavorite(){return this->m_favorite;}

    void add_review(const char* ad_nr, const char* username, const char* message);
    void request_ad_details(const char* ad_id, property_type prt_t);
    void add_to_remove_from_favorite(const char* ad_nr,std::string username,const char *action);
    void delete_ad(const char*ad_id);

    void request_feed_fav(std::string username);
    void request_feed_profile(std::string username);


    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111

    void adaugaFeedback(const char* ad_id, std::string idSender, std::string text, std::string idPostare)
    {
        this->feed.push_back(new Feedback(idSender, text, idPostare));
    }

    bool isAdmin() const { return admin; }

    void setAdmin(bool val) {this->admin=val;}
    void setSold(bool val){this->succesSold=val;}

    void add_funds(const char* value,std::string username);
    void buy_property(std::string username_b, const char*ad_id, const char*value );
    bool getSuccesSold(){return this->succesSold;}

    int getStatusTranzactie(){return this->statusTranzactie;}
    void setSoldClient(float val){this->sold_client=val;}
    float getSoldClient(){return this->sold_client;}

    void block_post(const char*ad_nr);

    void setAddDeleted(bool val)
    {
        this->addDeletedSuccesfully = val;
    }

    bool getAddDeletedSuccesfully()
    {
        return this->addDeletedSuccesfully;
    }



    void adaugaUtilizator(std::string utilizator)
    {
        this->m_utilizatoriAplicatie.push_back(utilizator);
    }

    std::vector<std::string> getVectorUtilizatori()
    {
        return this->m_utilizatoriAplicatie;
    }

    void aduagaSenderMesaj(std::string sender, std::string mesaj)
    {
        this->sender_mesaj.push_back(std::make_pair(sender, mesaj));
    }

    std::vector<std::pair<std::string, std::string>> getSenderMesaj()
    {
        return this->sender_mesaj;
    }

    void cleanSenderMsg()
    {
        if(!this->sender_mesaj.empty())
        {
            this->sender_mesaj.clear();
        }
        if(!this->m_utilizatoriAplicatie.empty())
        {
            this->m_utilizatoriAplicatie.clear();
        }
    }

    void clearFeedback()
    {
        this->feed.clear();
    }

    void adaugaStatusTranzactie(int status)
    {
        this->statusTranzactie = status;
    }

    void setStergereOfertant(int val)
    {
        this->stergereOfertant=val;
    }

    int getStergereOfertant()
    {
        return this->stergereOfertant;
    }

private:
    void get_ad_details(char* payload);
    void get_review(char* payload, const char* ad_id);

    void receiving_data(const char* ad_id = " ");

    void get_users(char* payload);
    void get_message(char* payload);


public:
    void request_chat(std::string username);
    void request_specific_chat(std::string username_1, std::string username_2);
    void send_message1(std::string username_1, std::string username_2, std::string message);
    void request_funds(std::string username);
    void delete_user(std::string username);

};

#endif // CLIENT_H
