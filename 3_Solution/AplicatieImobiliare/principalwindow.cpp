#include "principalwindow.h"
#include "mesageriewindow.h"
#include "qbuttongroup.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "ui_principalwindow.h"
#include "client.h"
#include "profilwindow.h"
#include "signinwindow.h"
#include "adaugapostarewindow.h"
#include "Proprietati.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QCheckBox>

PrincipalWindow::~PrincipalWindow()
{
    delete ui;
}

PrincipalWindow::PrincipalWindow(QWidget *parent, QString numeClient, const char *filtre)
    : QMainWindow(parent)
    , ui(new Ui::PrincipalWindow)
{
    std::string nume = numeClient.toStdString();
    Client::getInstance().adaugaUsername(nume);
    ui->setupUi(this);
    showFullScreen();
    adaugaWhiteBg();

    afiseazaNumeClient(Client::getInstance().getUsername());

    afiseazaFeed(filtre);
    adaugaFiltrele();
    adaugaButonAdaugaPostare();
    adaugaButonVizualizeazaProfil();
    adaugaButonAdaugaOfertant();
    adaugaButonVizualizareChat();
}

void PrincipalWindow::adaugaButonAdaugaOfertant()
{
    if(Client::getInstance().isAdmin())
    {
        QPushButton *button = new QPushButton ("Adaugă ofertant!", this);

        button->setGeometry(350,50,180,40);


        QFont font("Gabriola", 15, QFont::Normal);
        button->setFont(font);

        button->show();
        connect(button, &QPushButton::clicked, this, &PrincipalWindow::onAdaugaOfertantPressed);
    }
}

void PrincipalWindow::onAdaugaOfertantPressed()
{
    SigninWindow *sw = new SigninWindow(nullptr, Client::getInstance().getUsername(), "administrator");
    sw->show();
}

void PrincipalWindow::adaugaButonVizualizeazaProfil()
{
    if(!Client::getInstance().isAdmin())
    {
        QPushButton *button = new QPushButton ("Accesează profilul meu!", this);

        button->setGeometry(300,50,180,40);


        QFont font("Gabriola", 15, QFont::Normal);
        button->setFont(font);

        button->show();
        connect(button, &QPushButton::clicked, this, &PrincipalWindow::onButonVizualizeazaProfilPressed);
    }
}

void PrincipalWindow::adaugaButonVizualizareChat()
{
    if(!Client::getInstance().isAdmin())
    {
        QPushButton *button = new QPushButton ("Vizualizează chat!", this);

        button->setGeometry(600,50,180,40);


        QFont font("Gabriola", 15, QFont::Normal);
        button->setFont(font);

        button->show();
        connect(button, &QPushButton::clicked, this, &PrincipalWindow::onVizualizeazaChatPressed);
    }
}

void PrincipalWindow::onVizualizeazaChatPressed()
{
    Client::getInstance().cleanSenderMsg();

    Client::getInstance().request_chat(Client::getInstance().getUsername());

    MesagerieWindow *mw =new MesagerieWindow();
    mw->show();
}

void PrincipalWindow::onButonVizualizeazaProfilPressed()
{
    ProfilWindow * pw = new ProfilWindow();
    pw->show();
}

void PrincipalWindow::adaugaFiltrele()
{
    QFont font("Gabriola", 15, QFont::Normal);
    QFont font1("Gabriola", 12, QFont::Normal);
    QWidget *sidebarWidget = new QWidget(this);
    sidebarWidget->setFixedSize(190, 1300);

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);

    QLabel *tipProprietateLabel = new QLabel("Tip proprietate:", sidebarWidget);
    tipProprietateLabel->setFont(font);
    sidebarLayout->addWidget(tipProprietateLabel);

    checkboxCasa = new QCheckBox("Casa", sidebarWidget);
    checkboxApartament = new QCheckBox("Apartament", sidebarWidget);
    checkboxTeren = new QCheckBox("Teren", sidebarWidget);

    checkboxCasa->setFont(font1);
    checkboxApartament->setFont(font1);
    checkboxTeren->setFont(font1);

    sidebarLayout->addWidget(checkboxCasa);
    sidebarLayout->addWidget(checkboxApartament);
    sidebarLayout->addWidget(checkboxTeren);

    QLabel *intervalPretLabel = new QLabel("Interval de preț:", sidebarWidget);
    intervalPretLabel->setFont(font);
    sidebarLayout->addWidget(intervalPretLabel);


    checkbox0_500 = new QCheckBox("0-500", sidebarWidget);
    checkbox500_1000 = new QCheckBox("500-1000", sidebarWidget);
    checkbox1000_5000 = new QCheckBox("1000-5000", sidebarWidget);
    checkbox5000_10000 = new QCheckBox("5000-10000", sidebarWidget);
    checkbox10000_50000 = new QCheckBox("10000-50000", sidebarWidget);
    checkbox50000_100000 = new QCheckBox("50000-100000", sidebarWidget);
    checkbox100000_200000 = new QCheckBox("100000-200000", sidebarWidget);
    checkbox200000_plus = new QCheckBox("200000+", sidebarWidget);
    nimicPret = new QCheckBox("Fără selecție", sidebarWidget);


    checkbox0_500->setFont(font1);
    checkbox500_1000->setFont(font1);
    checkbox1000_5000->setFont(font1);
    checkbox5000_10000->setFont(font1);
    checkbox10000_50000->setFont(font1);
    checkbox50000_100000->setFont(font1);
    checkbox100000_200000->setFont(font1);
    checkbox200000_plus->setFont(font1);
    nimicPret->setFont(font1);

    sidebarLayout->addWidget(checkbox0_500);
    sidebarLayout->addWidget(checkbox500_1000);
    sidebarLayout->addWidget(checkbox1000_5000);
    sidebarLayout->addWidget(checkbox5000_10000);
    sidebarLayout->addWidget(checkbox10000_50000);
    sidebarLayout->addWidget(checkbox50000_100000);
    sidebarLayout->addWidget(checkbox100000_200000);
    sidebarLayout->addWidget(checkbox200000_plus);
    sidebarLayout->addWidget(nimicPret);

    QButtonGroup *buttonGroup = new QButtonGroup(sidebarWidget);
    buttonGroup->addButton(checkbox0_500);
    buttonGroup->addButton(checkbox500_1000);
    buttonGroup->addButton(checkbox1000_5000);
    buttonGroup->addButton(checkbox5000_10000);
    buttonGroup->addButton(checkbox10000_50000);
    buttonGroup->addButton(checkbox50000_100000);
    buttonGroup->addButton(checkbox100000_200000);
    buttonGroup->addButton(checkbox200000_plus);
    buttonGroup->addButton(nimicPret);
    buttonGroup->setExclusive(true);

    QLabel *tipTranzactieLabel = new QLabel("Tip tranzacție:", sidebarWidget);
    tipTranzactieLabel->setFont(font);
    sidebarLayout->addWidget(tipTranzactieLabel);

    checkboxVanzare = new QCheckBox("Vânzare", sidebarWidget);
    checkboxCumparare = new QCheckBox("Închiriere", sidebarWidget);
    nimicTipAnunt = new QCheckBox("Fără selecție", sidebarWidget);

    checkboxVanzare->setFont(font1);
    checkboxCumparare->setFont(font1);
    nimicTipAnunt->setFont(font1);

    sidebarLayout->addWidget(checkboxVanzare);
    sidebarLayout->addWidget(checkboxCumparare);
    sidebarLayout->addWidget(nimicTipAnunt);

    QButtonGroup *buttonGroup2 = new QButtonGroup(sidebarWidget);
    buttonGroup2->addButton(checkboxVanzare);
    buttonGroup2->addButton(checkboxCumparare);
    buttonGroup2->addButton(nimicTipAnunt);


    QLabel *sortarePretLabel = new QLabel("Sortare:", sidebarWidget);
    sortarePretLabel->setFont(font);
    sidebarLayout->addWidget(sortarePretLabel);
    QLabel *dupaCe = new QLabel("   Criteriu:", sidebarWidget);
    dupaCe->setFont(font);
    sidebarLayout->addWidget(dupaCe);

    checkboxTip1 = new QCheckBox("Preț", sidebarWidget);
    checkboxTip2 = new QCheckBox("Suprafață", sidebarWidget);
    nimicTip = new QCheckBox("Fără selecție", sidebarWidget);

    checkboxTip1->setFont(font1);
    checkboxTip2->setFont(font1);
    nimicTip->setFont(font1);

    sidebarLayout->addWidget(checkboxTip1);
    sidebarLayout->addWidget(checkboxTip2);
    sidebarLayout->addWidget(nimicTip);

    QButtonGroup *buttonGroup5 = new QButtonGroup(sidebarWidget);
    buttonGroup5->addButton(checkboxTip1);
    buttonGroup5->addButton(checkboxTip2);
    buttonGroup5->addButton(nimicTip);

    QLabel *ordine = new QLabel("   Sens:", sidebarWidget);
    ordine->setFont(font);
    sidebarLayout->addWidget(ordine);

    checkboxCrescator = new QCheckBox("Crescător", sidebarWidget);
    checkboxDescrescator = new QCheckBox("Descrescător", sidebarWidget);
    nimicSortPret = new QCheckBox("Fără selecție", sidebarWidget);

    checkboxCrescator->setFont(font1);
    checkboxDescrescator->setFont(font1);
    nimicSortPret->setFont(font1);

    sidebarLayout->addWidget(checkboxCrescator);
    sidebarLayout->addWidget(checkboxDescrescator);
    sidebarLayout->addWidget(nimicSortPret);

    QButtonGroup *buttonGroup3 = new QButtonGroup(sidebarWidget);
    buttonGroup3->addButton(checkboxCrescator);
    buttonGroup3->addButton(checkboxDescrescator);
    buttonGroup3->addButton(nimicSortPret);

    QLabel *dimensiuneProprietateLabel = new QLabel("Dimensiune proprietate:", sidebarWidget);
    dimensiuneProprietateLabel->setFont(font);
    sidebarLayout->addWidget(dimensiuneProprietateLabel);

    checkbox0_50 = new QCheckBox("0-50 m2", sidebarWidget);
    checkbox50_100 = new QCheckBox("50-100 m2", sidebarWidget);
    checkbox100_200 = new QCheckBox("100-200 m2", sidebarWidget);
    checkbox200_500 = new QCheckBox("200-500 m2", sidebarWidget);
    checkbox500_plus = new QCheckBox("500+ m2", sidebarWidget);
    nimicSuprafata = new QCheckBox("Fără selecție", sidebarWidget);

    checkbox0_50->setFont(font1);
    checkbox50_100->setFont(font1);
    checkbox100_200->setFont(font1);
    checkbox200_500->setFont(font1);
    checkbox500_plus->setFont(font1);
    nimicSuprafata->setFont(font1);

    sidebarLayout->addWidget(checkbox0_50);
    sidebarLayout->addWidget(checkbox50_100);
    sidebarLayout->addWidget(checkbox100_200);
    sidebarLayout->addWidget(checkbox200_500);
    sidebarLayout->addWidget(checkbox500_plus);
    sidebarLayout->addWidget(nimicSuprafata);

    QButtonGroup *buttonGroup1 = new QButtonGroup(sidebarWidget);
    buttonGroup1->addButton(checkbox0_50);
    buttonGroup1->addButton(checkbox50_100);
    buttonGroup1->addButton(checkbox100_200);
    buttonGroup1->addButton(checkbox200_500);
    buttonGroup1->addButton(checkbox500_plus);
    buttonGroup1->addButton(nimicSuprafata);
    buttonGroup->setExclusive(true);


    QPushButton *sortareButton = new QPushButton("Sortare", sidebarWidget);
    sortareButton->setFont(font);
    sidebarLayout->addWidget(sortareButton);

    sidebarLayout->setSpacing(10);

    connect(sortareButton, &QPushButton::clicked, this, &PrincipalWindow::sortare);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(sidebarWidget);
    scrollArea->setGeometry(0, 160, 210, 650);

    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    layout()->addWidget(scrollArea);
}

void PrincipalWindow::sortare()
{
    char filtru[7];
    if(checkboxCasa->isChecked() && !checkboxApartament->isChecked() && !checkboxTeren->isChecked())
    {
        filtru[0]= '1';
    }
    else if(!checkboxCasa->isChecked() && checkboxApartament->isChecked() && !checkboxTeren->isChecked())
    {
        filtru[0]= '2';
    }
    else if(!checkboxCasa->isChecked() && !checkboxApartament->isChecked() && checkboxTeren->isChecked())
    {
        filtru[0] = '3';
    }
    else if(checkboxCasa->isChecked() && checkboxApartament->isChecked() && !checkboxTeren->isChecked())
    {
        filtru[0] = '4';
    }
    else if(checkboxCasa->isChecked() && checkboxApartament->isChecked() && !checkboxTeren->isChecked())
    {
        filtru[0] = '5';
    }
    else if(!checkboxCasa->isChecked() && checkboxApartament->isChecked() && checkboxTeren->isChecked())
    {
        filtru[0] = '6';
    }
    else
    {
        filtru[0] = '0';
    }

    if(checkbox0_500->isChecked())
    {
        filtru[1] ='1';
    }
    else if(checkbox500_1000->isChecked())
    {
        filtru[1] = '2';
    }
    else if(checkbox1000_5000->isChecked())
    {
        filtru[1] = '3';
    }
    else if(checkbox5000_10000->isChecked())
    {
        filtru[1] = '4';
    }
    else if(checkbox10000_50000->isChecked())
    {
        filtru[1] = '5';
    }
    else if(checkbox50000_100000->isChecked())
    {
        filtru[1] = '6';
    }
    else if(checkbox100000_200000->isChecked())
    {
        filtru[1] = '7';
    }
    else if(checkbox200000_plus->isChecked())
    {
        filtru[1] = '8';
    }
    else
    {
        filtru[1] = '0';
    }

    if(checkboxVanzare->isChecked())
    {
        filtru[2] = '1';
    }
    else if (checkboxCumparare->isChecked())
    {
        filtru[2] = '2';
    }
    else
    {
        filtru[2] = '0';
    }


    if(checkbox0_50->isChecked())
    {
        filtru[3] = '1';
    }
    else if(checkbox50_100->isChecked())
    {
        filtru[3] = '2';
    }
    else if(checkbox100_200->isChecked())
    {
        filtru[3] = '3';
    }
    else if(checkbox200_500->isChecked())
    {
        filtru[4] = '4';
    }
    else if(checkbox500_plus->isChecked())
    {
        filtru[3] = '5';
    }
    else
    {
        filtru[3] = '0';
    }

    if(checkboxCrescator->isChecked())
    {
        filtru[4] = '1';
    }
    else if(checkboxDescrescator->isChecked())
    {
        filtru[4] = '2';
    }
    else
    {
        filtru[4] = '0';
    }

    if(checkboxTip2->isChecked())
    {
        filtru[5] = '1';
    }
    else
    {
        filtru[5] = '0';
    }

    filtru[6] = '\0';

    checkboxCasa->setChecked(false);
    checkboxApartament->setChecked(false);
    checkboxTeren->setChecked(false);
    checkbox0_500->setChecked(false);
    checkbox500_1000->setChecked(false);
    checkbox1000_5000->setChecked(false);
    checkbox5000_10000->setChecked(false);
    checkbox10000_50000->setChecked(false);
    checkbox50000_100000->setChecked(false);
    checkbox100000_200000->setChecked(false);
    checkbox200000_plus->setChecked(false);
    checkboxVanzare->setChecked(false);
    checkboxCumparare->setChecked(false);
    checkboxCrescator->setChecked(false);
    checkboxDescrescator->setChecked(false);
    checkbox0_50->setChecked(false);
    checkbox50_100->setChecked(false);
    checkbox100_200->setChecked(false);
    checkbox200_500->setChecked(false);
    checkbox500_plus->setChecked(false);
    nimicPret->setChecked(false);
    nimicSortPret->setChecked(false);
    nimicTipAnunt->setChecked(false);
    nimicSuprafata->setChecked(false);
    checkboxTip2->setChecked(false);
    checkboxTip1->setChecked(false);
    nimicTip->setChecked(false);

    PrincipalWindow *pws = new PrincipalWindow(nullptr, Client::getInstance().getUsername().c_str(), filtru);
    pws->show();
}

void PrincipalWindow::adaugaButonAdaugaPostare()
{
    if(!Client::getInstance().isAdmin())
    {
        QPushButton *button = new QPushButton ("Adaugă postare", this);

        button->setGeometry(80,50,120,40);


        QFont font("Gabriola", 15, QFont::Normal);
        button->setFont(font);

        button->show();
        connect(button, &QPushButton::clicked, this, &PrincipalWindow::onAdaugaPostareButtonPressed);
    }
}

void PrincipalWindow::onAdaugaPostareButtonPressed()
{
    AdaugaPostareWindow *apw =  new AdaugaPostareWindow(this);
    apw->show();
}

void PrincipalWindow::adaugaWhiteBg()
{
    QLabel *white_bg=new QLabel(this);
    white_bg->setStyleSheet("background-color: white;");
    white_bg->setGeometry(0, 0, width(), height());
    white_bg->show();
}

void PrincipalWindow::afiseazaNumeClient(std::string numeClient)
{
    QLabel *numeC = new QLabel(numeClient.c_str(), this);

    layout()->addWidget(numeC);

    QFont font("Gabriola", 18, QFont::Normal);
    numeC->setFont(font);

    QRect rect = numeC->fontMetrics().boundingRect(numeClient.c_str());

    int x = width() - rect.width() - 10;
    int y = 15;

    numeC->move(x, y);
    numeC->adjustSize();

    numeC->setStyleSheet("background-color: rgb(254, 253, 240)");
}

void PrincipalWindow::afiseazaFeed(const char*filtre)
{
    QWidget *containerWidget = new QWidget(this);
    QHBoxLayout *containerLayout = new QHBoxLayout(containerWidget);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    containerLayout->addWidget(scrollArea);

    layout()->addWidget(containerWidget);

    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);

    Client::getInstance().clearVector();
    Client::getInstance().clearVectorFav();
    Client::getInstance().request_feed(filtre);

    std::vector<Proprietate*> proprietati = Client::getInstance().getPostari();

    for (auto proprietate : proprietati)
    {
        ProprietateWidget *proprietateWidget = new ProprietateWidget(proprietate);

        proprietateWidget->setFixedSize(700, 350);
        contentLayout->addWidget(proprietateWidget);
    }

    contentWidget->setLayout(contentLayout);

    scrollArea->setWidget(contentWidget);

    containerWidget->setMinimumSize(900, 600);

    layout()->addWidget(containerWidget);

    containerWidget->move(200,150);
}
