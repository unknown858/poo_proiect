#include "profilwindow.h"
#include "adaugapostarewindow.h"
#include "qpushbutton.h"
#include "ui_profilwindow.h"
#include "client.h"
#include "principalwindow.h"
#include "favoritewindow.h"

#include <QMessageBox>
#include <string>

ProfilWindow::ProfilWindow(QWidget *parent, std::string username)
    : QMainWindow(parent), ui(new Ui::ProfilWindow), m_username(username)
{
    ui->setupUi(this);
    showFullScreen();

    Client::getInstance().request_funds(Client::getInstance().getUsername());
    afiseazaFeed();
    adaugaButonIntoarcere();
    afiseazaNumeClient();
    adaugaButonVizualizeazaFavorite();
    adaugaButonAdaugaPostare();
    adaugaLabelIntroducereSold();
    adaugaSoldCurent();
}

ProfilWindow::~ProfilWindow()
{
    delete ui;
}

void ProfilWindow::adaugaSoldCurent()
{
    if(!Client::getInstance().isAdmin())
    {
        if(m_username == Client::getInstance().getUsername())
        {
            QFont font("Gabriola", 15, QFont::Normal);
            QLabel *label = new QLabel("Sold curent: " + QString::number(Client::getInstance().getSoldClient()) + " € ", this);
            label->setFont(font);
            label->setGeometry(900,50,180,40);
            label->show();
        }
    }
}

void ProfilWindow::adaugaLabelIntroducereSold()
{
    if(!Client::getInstance().isAdmin() && this->m_username==Client::getInstance().getUsername())
    {
        QHBoxLayout* labelLineEditLayout = new QHBoxLayout();

        QLabel *labelUsername = new QLabel("Adaugă soldul cardului!", this);
        QFont font("Gabriola", 15, QFont::Normal);
        labelUsername->setFont(font);
        labelLineEditLayout->addWidget(labelUsername);

        lineMsg = new QLineEdit(this);
        labelLineEditLayout->addWidget(lineMsg);

        QPushButton *button = new QPushButton("Adaugă!", this);
        button->setFont(font);
        button->setFixedSize(120, 30);
        labelLineEditLayout->addWidget(button);
        labelLineEditLayout->setSpacing(10);
        labelLineEditLayout->setContentsMargins(0, 0, 35, 0);

        QWidget *localWidget = new QWidget(this);
        localWidget->setLayout(labelLineEditLayout);
        localWidget->setGeometry(750, 715, 550, 100);
        localWidget->show();
        connect(button, &QPushButton::clicked, this, &ProfilWindow::adaugaSoldul);
    }
}

void ProfilWindow::adaugaButonAdaugaPostare()
{
    if(this->m_username == Client::getInstance().getUsername())
    {
        if(!Client::getInstance().isAdmin())
        {
            QPushButton *button = new QPushButton ("Adaugă postare", this);

            button->setGeometry(600,50,120,40);


            QFont font("Gabriola", 15, QFont::Normal);
            button->setFont(font);

            button->show();
            connect(button, &QPushButton::clicked, this, &ProfilWindow::onAdaugaPostareButtonPressed);
        }
    }
}

void ProfilWindow::adaugaSoldul()
{
    Client::getInstance().add_funds(lineMsg->text().toStdString().c_str(), m_username.c_str());
    if(Client::getInstance().getSuccesSold()==0)
    {
        QMessageBox::information(this, "Informare", "Soldul a fost adăugat cu succes!");
        ProfilWindow * pw = new ProfilWindow(nullptr, m_username);
        pw->show();
    }
    else
    {
        QMessageBox::critical(this, "Atenție", "Soldul nu a putut fi introdus! Vă rugăm să reintroduceți!");
    }
}

void ProfilWindow::onAdaugaPostareButtonPressed()
{
    AdaugaPostareWindow *apw =  new AdaugaPostareWindow(this);
    apw->show();

    this->close();
}

void ProfilWindow::afisezaFavorite()
{
    FavoriteWindow *fw = new FavoriteWindow();
    fw->show();

    this->close();
}

void ProfilWindow::afiseazaNumeClient()
{
    QLabel *numeC = new QLabel(m_username.c_str(), this);

    layout()->addWidget(numeC);

    QFont font("Gabriola", 18, QFont::Normal);
    numeC->setFont(font);

    QRect rect = numeC->fontMetrics().boundingRect(m_username.c_str());

    int x = width() - rect.width() - 15;
    int y = 15;

    numeC->move(x, y);
    numeC->adjustSize();

    numeC->setStyleSheet("background-color: rgb(254, 253, 240)");
}

void ProfilWindow::adaugaButonIntoarcere()
{
    QPushButton *button = new QPushButton ("Întoarce-te", this);
    button->setGeometry(80,50,120,40);


    QFont font("Gabriola", 15, QFont::Normal);
    button->setFont(font);

    button->show();
    connect(button, &QPushButton::clicked, this, &ProfilWindow::onIntoarceButtonPressed);
}

void ProfilWindow::adaugaButonVizualizeazaFavorite()
{
    if(!Client::getInstance().isAdmin())
    {
        QPushButton *button = new QPushButton ("Vizualizează favorite!", this);
        button->setGeometry(300,50,180,40);

        QFont font("Gabriola", 15, QFont::Normal);
        button->setFont(font);

        button->show();
        connect(button, &QPushButton::clicked, this, &ProfilWindow::afisezaFavorite);
    }
}

void ProfilWindow::onIntoarceButtonPressed()
{
    PrincipalWindow *pw = new PrincipalWindow(nullptr, Client::getInstance().getUsername().c_str());
    pw->show();

    this->close();
}

void ProfilWindow::afiseazaFeed()
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

    const char * filtre = "000000";
    Client::getInstance().request_feed(filtre);

    std::vector<Proprietate*> proprietati = Client::getInstance().getPostari();

    for (auto proprietate : proprietati)
    {
        if(proprietate->getNumeVanzator() == m_username.c_str())
        {
            ProprietateWidget *proprietateWidget = new ProprietateWidget(proprietate, this, "profil");

            proprietateWidget->setFixedSize(700, 350);
            contentLayout->addWidget(proprietateWidget);
        }
    }

    contentWidget->setLayout(contentLayout);

    scrollArea->setWidget(contentWidget);

    containerWidget->setMinimumSize(900, 600);

    layout()->addWidget(containerWidget);

    containerWidget->move(25, 150);
}
