#include "adaugapostarewindow.h"
#include "principalwindow.h"
#include "profilwindow.h"
#include "qpushbutton.h"
#include "ui_adaugapostarewindow.h"
#include "client.h"
#include "frameutils.h"

#include <QMessageBox>
#include <QButtonGroup>

AdaugaPostareWindow::AdaugaPostareWindow(QWidget *parent, std::string pagina)
    : QMainWindow(parent)
    , m_pagina(pagina), ui(new Ui::AdaugaPostareWindow)
{
    ui->setupUi(this);
    showFullScreen();
    adaugaBg();
    introduDatele();
    introduCaile();
    adaugaButonTrimitere();
    adaugaButonIntoarcere();

    connect(btnTeren, &QCheckBox::toggled, this, &AdaugaPostareWindow::onBtnTerenToggled);
}

void AdaugaPostareWindow::adaugaBg()
{
    QPixmap image("C:/imagini/pic1.jpg");

    QLabel *label = new QLabel(this);
    label->setPixmap(image);
    label->setScaledContents(true);

    label->setGeometry(0,0,width(),height());
    label->show();
}

void AdaugaPostareWindow::onBtnTerenToggled(bool checked) {
    if (checked)
    {
        nrCamere->setEnabled(false);
        nrCamere->clear();
    } else
    {
        nrCamere->setEnabled(true);
    }
}

AdaugaPostareWindow::~AdaugaPostareWindow()
{
    delete ui;
}

void AdaugaPostareWindow::adaugaButonTrimitere()
{
    QPushButton *button = new QPushButton ("Trimite date", this);

    button->setGeometry(875,650,180,50);
    button->setStyleSheet("background-color: white;");

    QFont font("Gabriola", 17, QFont::Normal);
    button->setFont(font);

    button->show();
    connect(button, &QPushButton::clicked, this, &AdaugaPostareWindow::trimitereDatePostare);
}
void AdaugaPostareWindow::adaugaButonIntoarcere()
{
    QPushButton *button = new QPushButton ("Întoarce-te", this);

    button->setGeometry(230,650,180,50);
    button->setStyleSheet("background-color: white;");

    QFont font("Gabriola", 17, QFont::Normal);
    button->setFont(font);

    button->show();
    connect(button, &QPushButton::clicked, this, &AdaugaPostareWindow::onIntoarcerePressed);
}
void AdaugaPostareWindow::onIntoarcerePressed()
{
    PrincipalWindow *pw =new PrincipalWindow(nullptr, Client::getInstance().getUsername().c_str());
    pw->show();
}

void AdaugaPostareWindow::introduDatele()
{
    gbox= new QGroupBox ("Informații postare",this);
    gbox->setStyleSheet("background-color: white;");
    QFont font("Gabriola", 17, QFont::Normal);
    gbox->setFont(font);

    gbox->setGeometry(140,100,400,500);

    groupBoxLayout = new QVBoxLayout(gbox);

    labelLineEditLayout = new QVBoxLayout;

    creeazaTipAnunt();
    creeazaTipPropr();
    creeazaDesc();
    creeazaPret();
    creeazaLocatie();
    creeazaSuprafata();
    creeazaNrCamere();

    layout()->addWidget(gbox);
}

void AdaugaPostareWindow::introduCaile()
{
    gbox1= new QGroupBox ("Căile pentru poze",this);
    gbox1->setStyleSheet("background-color: white;");
    QFont font("Gabriola", 17, QFont::Normal);
    gbox1->setFont(font);

    gbox1->setGeometry(750,100,400,500);

    groupBoxLayout1 = new QVBoxLayout(gbox1);

    labelLineEditLayout1 = new QVBoxLayout;

    creeazaCale1();
    creeazaCale2();
    creeazaCale3();
    creeazaCale4();
    creeazaCale5();
    creeazaCale6();

    layout()->addWidget(gbox1);
}

void AdaugaPostareWindow::creeazaTipAnunt()
{
    QFont font("Gabriola", 15, QFont::Normal);
    QFont font1("Gabriola", 12, QFont::Normal);
    QWidget *sidebarWidget = new QWidget(this);
    sidebarWidget->setFixedSize(190, 1000);
    QLabel *tipAnuntLabel = new QLabel("Tip anunț:", sidebarWidget);
    tipAnuntLabel->setFont(font);
    labelLineEditLayout->addWidget(tipAnuntLabel);

    checkboxLayout = new QHBoxLayout();
    labelLineEditLayout->addLayout(checkboxLayout);


    btnVanzare = new QCheckBox("Vânzare", sidebarWidget);
    btnVanzare->setFont(font1);
    checkboxLayout->addWidget(btnVanzare);

    btnInchiriere = new QCheckBox("Închiriere", sidebarWidget);
    checkboxLayout->addWidget(btnInchiriere);
    btnInchiriere->setFont(font1);

    QButtonGroup *buttonGroup = new QButtonGroup(sidebarWidget);
    buttonGroup->addButton(btnVanzare);
    buttonGroup->addButton(btnInchiriere);
    buttonGroup->setExclusive(true);

    groupBoxLayout->addLayout(labelLineEditLayout);
}
void AdaugaPostareWindow::creeazaTipPropr()
{
    QFont font("Gabriola", 15, QFont::Normal);
    QFont font1("Gabriola", 12, QFont::Normal);
    QWidget *sidebarWidget = new QWidget(this);
    sidebarWidget->setFixedSize(190, 1000);
    QLabel *tipAnuntLabel = new QLabel("Tip proprietate:", sidebarWidget);
    tipAnuntLabel->setFont(font);
    labelLineEditLayout->addWidget(tipAnuntLabel);

    checkboxLayout1 = new QHBoxLayout();
    labelLineEditLayout->addLayout(checkboxLayout1);


    btnCasa = new QCheckBox("Casă", sidebarWidget);
    btnCasa->setFont(font1);
    checkboxLayout1->addWidget(btnCasa);

    btnTeren = new QCheckBox("Teren", sidebarWidget);
    btnTeren->setFont(font1);
    checkboxLayout1->addWidget(btnTeren);

    btnApartament = new QCheckBox("Apartament", sidebarWidget);
    btnApartament->setFont(font1);
    checkboxLayout1->addWidget(btnApartament);

    QButtonGroup *buttonGroup = new QButtonGroup(sidebarWidget);
    buttonGroup->addButton(btnCasa);
    buttonGroup->addButton(btnTeren);
    buttonGroup->addButton(btnApartament);
    buttonGroup->setExclusive(true);

    groupBoxLayout->addLayout(labelLineEditLayout);
}
void AdaugaPostareWindow::creeazaDesc()
{
    QLabel *label = new QLabel("Descriere", gbox);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);
    descriere = new QLineEdit(gbox);
    labelLineEditLayout->addWidget(descriere);

    groupBoxLayout->addLayout(labelLineEditLayout);
}
void AdaugaPostareWindow::creeazaPret()
{
    QLabel *label = new QLabel("Preț în euro", gbox);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);
    pret = new QLineEdit(gbox);
    labelLineEditLayout->addWidget(pret);

    groupBoxLayout->addLayout(labelLineEditLayout);
}
void AdaugaPostareWindow::creeazaLocatie()
{
    QLabel *label = new QLabel("Locație", gbox);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);
    locatie = new QLineEdit(gbox);
    labelLineEditLayout->addWidget(locatie);

    groupBoxLayout->addLayout(labelLineEditLayout);
}
void AdaugaPostareWindow::creeazaSuprafata()
{
    QLabel *label = new QLabel("Suprafața în m2", gbox);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);
    suprafata = new QLineEdit(gbox);
    labelLineEditLayout->addWidget(suprafata);

    groupBoxLayout->addLayout(labelLineEditLayout);
}
void AdaugaPostareWindow::creeazaNrCamere()
{
    QLabel *label = new QLabel("Numărul de camere", gbox);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);
    nrCamere = new QLineEdit(gbox);
    labelLineEditLayout->addWidget(nrCamere);

    groupBoxLayout->addLayout(labelLineEditLayout);
}

void AdaugaPostareWindow::creeazaCale1()
{
    QLabel *label = new QLabel("Cale1", gbox1);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout1->addWidget(label);
    cale1 = new QLineEdit(gbox1);
    labelLineEditLayout1->addWidget(cale1);

    groupBoxLayout1->addLayout(labelLineEditLayout1);
}
void AdaugaPostareWindow::creeazaCale2()
{
    QLabel *label = new QLabel("Cale2", gbox1);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout1->addWidget(label);
    cale2 = new QLineEdit(gbox1);
    labelLineEditLayout1->addWidget(cale2);

    groupBoxLayout1->addLayout(labelLineEditLayout1);
}
void AdaugaPostareWindow::creeazaCale3()
{
    QLabel *label = new QLabel("Cale3", gbox1);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout1->addWidget(label);
    cale3 = new QLineEdit(gbox1);
    labelLineEditLayout1->addWidget(cale3);

    groupBoxLayout1->addLayout(labelLineEditLayout1);
}
void AdaugaPostareWindow::creeazaCale4()
{
    QLabel *label = new QLabel("Cale4", gbox1);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout1->addWidget(label);
    cale4 = new QLineEdit(gbox1);
    labelLineEditLayout1->addWidget(cale4);

    groupBoxLayout1->addLayout(labelLineEditLayout1);
}
void AdaugaPostareWindow::creeazaCale5()
{
    QLabel *label = new QLabel("Cale5", gbox1);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout1->addWidget(label);
    cale5 = new QLineEdit(gbox1);
    labelLineEditLayout1->addWidget(cale5);

    groupBoxLayout1->addLayout(labelLineEditLayout1);
}
void AdaugaPostareWindow::creeazaCale6()
{
    QLabel *label = new QLabel("Cale6", gbox1);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout1->addWidget(label);
    cale6 = new QLineEdit(gbox1);
    labelLineEditLayout1->addWidget(cale6);

    groupBoxLayout1->addLayout(labelLineEditLayout1);
}

void AdaugaPostareWindow::trimitereDatePostare()
{
    QString l_descriere = this->descriere->text();
    QString l_pret = this->pret->text();
    QString l_suprafata = this->suprafata->text();
    QString l_locatie = this->locatie->text();
    QString l_nrCam=this->nrCamere->text();

    QString l_cale1 = this->cale1->text();
    QString l_cale2 = this->cale2->text();
    QString l_cale3 = this->cale3->text();
    QString l_cale4 = this->cale4->text();
    QString l_cale5 = this->cale5->text();
    QString l_cale6 = this->cale6->text();

    bool dateGresite = verificaDate(l_nrCam, l_descriere, l_pret, l_locatie, l_suprafata);

    if(dateGresite)
    {
        adaugaButonTrimitere();
    }
    else
    {
        std::string sdescriere = l_descriere.toStdString();
        std::string spret = l_pret.toStdString();
        std::string ssuprafata = l_suprafata.toStdString();
        std::string slocatie = l_locatie.toStdString();
        std::string snrCam=l_nrCam.toStdString();

        std::string scale1 = l_cale1.toStdString();
        std::string scale2 = l_cale2.toStdString();
        std::string scale3 = l_cale3.toStdString();
        std::string scale4 = l_cale4.toStdString();
        std::string scale5 = l_cale5.toStdString();
        std::string scale6 = l_cale6.toStdString();

        ad_type tipAd = getAddType();
        property_type  tipPr = getPropretyType();

        char * nr = Client::getInstance().request_next_ad_nr();
        if(!btnTeren->isChecked())
        {
            Client::getInstance().create_ad(Client::getInstance().getUsername().c_str(), tipAd, tipPr, sdescriere.c_str(), spret.c_str(), slocatie.c_str(), ssuprafata.c_str(), "0", snrCam.c_str(),"1","1");
        }
        else
        {
            Client::getInstance().create_ad(Client::getInstance().getUsername().c_str(), tipAd, tipPr, sdescriere.c_str(), spret.c_str(), slocatie.c_str(), ssuprafata.c_str(), "0");
        }

        if(!scale1.empty())
        {
            Frame::send_image((char*)scale1.c_str(),nr,Client::getInstance().client_sock);
            Frame::receive_ack(Client::getInstance().client_sock);
        }
        if(!scale2.empty())
        {
            Frame::send_image((char*)scale2.c_str(),nr,Client::getInstance().client_sock);
            Frame::receive_ack(Client::getInstance().client_sock);
        }
        if(!scale3.empty())
        {
            Frame::send_image((char*)scale3.c_str(),nr,Client::getInstance().client_sock);
            Frame::receive_ack(Client::getInstance().client_sock);
        }
        if(!scale4.empty())
        {
            Frame::send_image((char*)scale4.c_str(),nr,Client::getInstance().client_sock);
            Frame::receive_ack(Client::getInstance().client_sock);
        }
        if(!scale5.empty())
        {
            Frame::send_image((char*)scale5.c_str(),nr,Client::getInstance().client_sock);
            Frame::receive_ack(Client::getInstance().client_sock);
        }
        if(!scale6.empty())
        {
            Frame::send_image((char*)scale6.c_str(),nr,Client::getInstance().client_sock);
            Frame::receive_ack(Client::getInstance().client_sock);
        }

        QMessageBox::information(this, "Informare", "Postarea a fost adaugată cu succes!");

        ProfilWindow * pw = new ProfilWindow();
        pw->show();
    }
}

ad_type AdaugaPostareWindow::getAddType()
{
    if (btnVanzare->isChecked())
    {
        return ad_type::sale;
    }
    else
    {
        return ad_type::rent;
    }
}

property_type AdaugaPostareWindow::getPropretyType()
{
    if (btnCasa->isChecked())
    {
        return property_type::house;
    }
    else if(btnApartament->isChecked())
    {
        return property_type::apartment;
    }
    else
    {
        return property_type::land;
    }
}

void AdaugaPostareWindow::clearBox()
{
    this->tipAnunt->clear();
    this->tipProp->clear();
    this->descriere->clear();
    this->pret->clear();
    this->locatie->clear();
    this->suprafata->clear();
}

bool AdaugaPostareWindow::verificaDate(QString l_nrCamere, QString l_descriere, QString l_pret, QString l_locatie, QString l_suprafata)
{
    if (l_descriere.isEmpty() || l_pret.isEmpty() || l_locatie.isEmpty() || l_suprafata.isEmpty()) {
        QMessageBox::critical(this, "Eroare", "Toate câmpurile din 'Informații postare' trebuie completate!");
        return 1;
    }

    QRegularExpression cifre("^[\\+]?[0-9]+$");
    QRegularExpressionMatch matchNr = cifre.match(l_pret);
    if (!matchNr.hasMatch()) {
        pret->setStyleSheet("QLineEdit { color: red; }");
        QMessageBox::critical(this, "Eroare", "Trebuie să introduceți doar cifre!");
        return 1;
    }

    if(!btnTeren->isChecked())
    {
        matchNr=cifre.match(l_nrCamere);
        if (!matchNr.hasMatch()) {
        suprafata->setStyleSheet("QLineEdit { color: red; }");
        QMessageBox::critical(this, "Eroare", "Trebuie să introduceți doar cifre!");
        return 1;
        }
    }

    matchNr=cifre.match(l_suprafata);
    if (!matchNr.hasMatch()) {
        suprafata->setStyleSheet("QLineEdit { color: red; }");
        QMessageBox::critical(this, "Eroare", "Trebuie să introduceți doar cifre!");
        return 1;
    }

    if (!btnVanzare->isChecked() && !btnInchiriere->isChecked())
    {
        btnVanzare->setStyleSheet("QCheckBox { color: red; }");
        btnInchiriere->setStyleSheet("QCheckBox { color: red; }");
        QMessageBox::critical(this, "Eroare", "Selectați un tip de anunț (Vânzare sau Închiriere)!");
        return false;
    }

    if (!btnCasa->isChecked() && !btnApartament->isChecked() && !btnTeren->isChecked())
    {
        btnCasa->setStyleSheet("QCheckBox { color: red; }");
        btnApartament->setStyleSheet("QCheckBox { color: red; }");
        btnTeren->setStyleSheet("QCheckBox { color: red; }");
        QMessageBox::critical(this, "Eroare", "Selectați un tip de proprietate (Casă, Apartament sau Teren)!");
        return 0;
    }

    return 0;
}
