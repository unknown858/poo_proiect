#ifndef PROPRIETATI_H
#define PROPRIETATI_H

#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <string>

enum class TipProprietate
{
    casa, apartament, teren
};

class IFeedback
{
protected:
    std::string m_userGiver;
    std::string m_feedback;
    std::string m_idPostare;

public:
    virtual ~IFeedback() = default;

    virtual std::string getUserGiver() = 0;
    virtual std::string getFeedback() = 0;
    virtual std::string getIdPostare() = 0;
};

class Feedback : public IFeedback
{
private:
    // std::string m_userGiver;
    // std::string m_feedback;

public:
    Feedback()=default;
    Feedback(std::string userGiver, std::string feedback, std::string idPost)
    {
        this->m_userGiver=userGiver;
        this->m_feedback=feedback;
        this->m_idPostare=idPost;
    }
    std::string getUserGiver() {return this->m_userGiver;}
    std::string getFeedback() {return this->m_feedback;}
    std::string getIdPostare()
    {
        return this->m_idPostare;
    }
};

class Proprietate
{
protected:
    std::vector<IFeedback*> m_feedb;

    TipProprietate m_tip;

    std::string m_tipAnunt;
    std::string m_vanzator;
    float m_pret;
    float m_suprafata;
    std::string m_locatie;
    std::vector<std::string> m_imagini;
    int dimensiunem_imagini = -1;

    std::string m_descriere;
    int m_nrCamere;

    bool m_blocat = false;
    int m_idPostare;

    std::string m_data;


public:
    Proprietate(TipProprietate tip, float pret, float suprafata, std::string tipAnunt, std::string vanzator, std::string locatie, bool blocat, int idPostare):
        m_tip(tip), m_tipAnunt(tipAnunt), m_vanzator(vanzator), m_pret(pret), m_suprafata(suprafata), m_locatie(locatie), m_blocat(blocat), m_idPostare(idPostare) {}



    virtual ~Proprietate() {

    }

    virtual std::string getTipAnunt() = 0;
    virtual std::string getNumeVanzator() = 0;
    virtual TipProprietate getTip()= 0;
    virtual float getSuprafata() =0;
    virtual int getNrCamere() = 0;
    virtual float getPret() = 0;
    virtual std::vector<std::string> getImagini() = 0;
    virtual std::string getLocatie() = 0 ;
    virtual std::string getDescriere() = 0 ;
    virtual std::vector<IFeedback*> getFeedback() = 0;
    virtual void adaugaCaleImagine(std::string cale) = 0;
    virtual void setDimensiunem_imagini0() = 0;
    virtual int getDimensiunem_imagini() = 0;
    virtual void incDimm_imagini() = 0;
    virtual int getIdPostare() = 0;
    virtual std::string getData() = 0;
    virtual void setData(std::string data) = 0;
    virtual void setDescriere(std::string desc) = 0;
    virtual void setNrCamere(int nr) = 0;
    //virtual void addFeedback(std::string idSender, std::string text) = 0;

    virtual bool getStatusPostare() = 0;
    virtual void clearFeedback() = 0;
    virtual void setStatusPostare(bool val) = 0;
};

class CasaApartament : public Proprietate
{
private:
    int m_nrCamere;
public:
    CasaApartament(TipProprietate tip, float pret, float suprafata, std::string tipAnunt,
                   std::string vanzator, std::string locatie, bool blocat, int idPostare):
        Proprietate(tip, pret, suprafata, tipAnunt, vanzator, locatie, blocat, idPostare){}



    ~CasaApartament() override {}

    TipProprietate getTip() override {return this->m_tip;}
    float getSuprafata() override {return this->m_suprafata;};
    int getNrCamere() override {return this->m_nrCamere;};
    float getPret() override {return this->m_pret;};
    std::vector<std::string> getImagini() override {return this->m_imagini;};
    std::string getTipAnunt() override {return this->m_tipAnunt;};
    std::string getNumeVanzator() override {return this->m_vanzator;};
    std::string getLocatie() override {return this->m_locatie;}
    std::string getDescriere() override { return this->m_descriere;}
    std::vector<IFeedback*> getFeedback() override { return this->m_feedb;}
    void adaugaCaleImagine(std::string cale) override {
        for (const auto &img : this->m_imagini)
        {
            if(!strcmp(img.c_str(), cale.c_str()))
            {
                return;
            }
        }
        this->m_imagini.push_back(cale);
        this->dimensiunem_imagini++;
    }

    void setDimensiunem_imagini0() override {this->dimensiunem_imagini = -1;}
    int getDimensiunem_imagini() override { return this->dimensiunem_imagini;}
    void incDimm_imagini() override {this->dimensiunem_imagini++;}
    int getIdPostare() override
    {
        return this->m_idPostare;
    }
    std::string getData() override { return this->m_data;}

    void setData(std::string data) override {this->m_data = data;}
    void setDescriere(std::string desc) override {this->m_descriere=desc;}
    void setNrCamere(int nr) override {this->m_nrCamere = nr;}

    //void addFeedback(std::string idSender, std::string text) override {this->m_feedb.push_back(new Feedback(idSender, text));}

    void clearFeedback() override {this->m_feedb.clear();}
    bool getStatusPostare() override
    {
        return this->m_blocat;
    }

    void setStatusPostare(bool val) override
    {
        this->m_blocat = val;
    }
};

class Teren : public Proprietate
{
public:
    Teren(TipProprietate tip, float pret, float suprafata, std::string tipAnunt, std::string vanzator, std::string locatie, bool blocat, int idPostare) :
        Proprietate(tip, pret, suprafata, tipAnunt, vanzator, locatie, blocat, idPostare) {}


    ~Teren() override {}

    TipProprietate getTip() override {return this->m_tip;}
    float getSuprafata() override {return this->m_suprafata;};
    int getNrCamere() override {return -1;};
    float getPret() override {return this->m_pret;};
    std::vector<std::string> getImagini() override {return this->m_imagini;};
    std::string getTipAnunt() override {return this->m_tipAnunt;};
    std::string getNumeVanzator() override {return this->m_vanzator;};
    std::string getLocatie() override {return this->m_locatie;}
    std::string getDescriere() override { return this->m_descriere;}
    std::vector<IFeedback*> getFeedback() override { return this->m_feedb;}
    void adaugaCaleImagine(std::string cale) override {
        for (const auto &img : this->m_imagini)
        {
            if(!strcmp(img.c_str(), cale.c_str()))
            {
                return;
            }
        }
        this->m_imagini.push_back(cale);
        this->dimensiunem_imagini++;
    }
    void setDimensiunem_imagini0() override {this->dimensiunem_imagini = -1;}
    int getDimensiunem_imagini() override { return this->dimensiunem_imagini;}
    void incDimm_imagini() override {this->dimensiunem_imagini++;}
    int getIdPostare() override
    {
        return this->m_idPostare;
    }
    std::string getData() override { return this->m_data;}


    void setData(std::string data) override{this->m_data = data;}
    void setDescriere(std::string desc) override {this->m_descriere=desc;}
    void setNrCamere(int nr) override {this->m_nrCamere = nr;}

    //void addFeedback(std::string idSender, std::string text) override {this->m_feedb.push_back(new Feedback(idSender, text));}

    void clearFeedback() override {this->m_feedb.clear();}
    bool getStatusPostare() override
    {
        return this->m_blocat;
    }

    void setStatusPostare(bool val) override
    {
        this->m_blocat = val;
    }
};

class ProprietateWidget : public QWidget {
    Q_OBJECT
public:
    ProprietateWidget(Proprietate* proprietate, QWidget *parent = nullptr, std::string apelant = "")
        : QWidget(parent), m_apelant(apelant), m_proprietate(proprietate) {
        afiseazaTextSiImagini();
    }

signals:
    void imagineSelectata(QPixmap *imagine);

private slots:
    void afiseazaTextSiImagini();
    void afiseazaDetalii();
    void stergePostare();

private:

    std::string m_apelant;
    Proprietate *m_proprietate;
};


#endif // PROPRIETATI_H
