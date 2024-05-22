#include "mesageriewindow.h"
#include "principalwindow.h"
#include "qboxlayout.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "qscrollarea.h"
#include "ui_mesageriewindow.h"

#include "client.h"
#include <QLineEdit>

MesagerieWindow::MesagerieWindow(QWidget *parent, std::string receiver, std::string pagina)
    : QMainWindow(parent)
    , ui(new Ui::MesagerieWindow), m_nume(receiver), m_pagina(pagina)
{
    ui->setupUi(this);
    showFullScreen();
    adaugaButonIntoarcere();
    prieteni();
    if(receiver != "" || !Client::getInstance().getSenderMesaj().empty())
        conversatie(m_nume);

}

MesagerieWindow::~MesagerieWindow()
{
    delete ui;
}


void MesagerieWindow::conversatie(std::string nume)
{
    QFont font("Gabriola", 15, QFont::Normal);
    QWidget *containerWidget = new QWidget(this);
    QHBoxLayout *containerLayout = new QHBoxLayout(containerWidget);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    containerLayout->addWidget(scrollArea);

    layout()->addWidget(containerWidget);

    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    std::vector<std::pair<std::string, std::string>> SM = Client::getInstance().getSenderMesaj();

    for(const auto &sm : SM)
    {
        if(sm.first==Client::getInstance().getUsername())
        {
            QString text = QString::fromStdString(sm.second + " :" + sm.first);
            QLabel *line = new QLabel(text, this);
            line->setFont(font);
            line->setAlignment(Qt::AlignRight);
            contentLayout->addWidget(line);
        }
        else
        {
            QString text = QString::fromStdString(sm.first + ": " + sm.second);
            QLabel *line = new QLabel(text, this);
            line->setFont(font);
            line->setAlignment(Qt::AlignLeft);
            contentLayout->addWidget(line);
        }
    }


    QHBoxLayout* labelLineEditLayout = new QHBoxLayout();
    labelLineEditLayout->setContentsMargins(0, 0, 35, 0);

    lineMsg = new QLineEdit(this);
    labelLineEditLayout->addWidget(lineMsg);
    labelLineEditLayout->setSpacing(10);
    QPushButton *button = new QPushButton("Trimite!", this);
    button->setFont(font);
    button->setFixedSize(120, 30);
    labelLineEditLayout->addWidget(button);
    contentLayout->addLayout(labelLineEditLayout);

    contentWidget->setLayout(contentLayout);

    scrollArea->setWidget(contentWidget);

    containerWidget->setMinimumSize(900, 600);

    layout()->addWidget(containerWidget);

    containerWidget->move(200,150);

    connect(button, &QPushButton::clicked, this, &MesagerieWindow::trimiteMesajul);
}

void MesagerieWindow::trimiteMesajul()
{
    Client::getInstance().send_message1(Client::getInstance().getUsername(), this->m_nume, lineMsg->text().toStdString().c_str());


    Client::getInstance().cleanSenderMsg();
    Client::getInstance().request_chat(Client::getInstance().getUsername());
    Client::getInstance().request_specific_chat(Client::getInstance().getUsername(), this->m_nume);


    MesagerieWindow *mw =new MesagerieWindow(nullptr, this->m_nume, this->m_pagina);
    mw->show();

    this->close();
}

void MesagerieWindow::prieteni()
{
    QFont font("Gabriola", 15, QFont::Normal);

    QWidget *sidebarWidget = new QWidget(this);
    sidebarWidget->setMaximumSize(190, 1300);

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);

    std::vector<std::string> utilizatori = Client::getInstance().getVectorUtilizatori();

    QLabel *label = new QLabel("Conversații", this);
    label->setFont(font);
    sidebarLayout->addWidget(label);

    for(const auto &util : utilizatori)
    {
        QPushButton *button = new QPushButton(util.c_str(), this);
        button->setFont(font);
        sidebarLayout->addWidget(button);
        connect(button, &QPushButton::clicked, this, [=](){ adaugaConversatie(util); });
    }


    QScrollArea *scrollArea1 = new QScrollArea(this);
    scrollArea1->setWidget(sidebarWidget);
    scrollArea1->setGeometry(0, 160, 210, 650);

    scrollArea1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    layout()->addWidget(scrollArea1);
}

void MesagerieWindow::adaugaConversatie(std::string destinatar)
{
    Client::getInstance().cleanSenderMsg();

    Client::getInstance().request_specific_chat(Client::getInstance().getUsername(), destinatar);


    Client::getInstance().request_chat(Client::getInstance().getUsername());


    MesagerieWindow *mw = new MesagerieWindow(nullptr, destinatar);
    mw->show();

    this->close();
}

void MesagerieWindow::adaugaButonIntoarcere()
{
    QPushButton *button = new QPushButton ("Întoarce-te", this);
    button->setGeometry(80,50,120,40);


    QFont font("Gabriola", 15, QFont::Normal);
    button->setFont(font);

    button->show();
    connect(button, &QPushButton::clicked, this, &MesagerieWindow::onIntoarceButtonPressed);
}

void MesagerieWindow::onIntoarceButtonPressed()
{
    PrincipalWindow *pw = new PrincipalWindow (nullptr, QString::fromStdString(Client::getInstance().getUsername()));
    pw->show();

    this->close();
}
