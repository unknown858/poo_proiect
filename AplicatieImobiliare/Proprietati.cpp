#include "Proprietati.h"
#include "client.h"
#include "profilwindow.h"
#include "qpushbutton.h"
#include "postarewindow.h"

#include <QScrollArea>
#include <QMessageBox>

std::string toStringFunc(TipProprietate tip) {
    switch (tip) {
    case TipProprietate::casa:
        return "Casă";
    case TipProprietate::apartament:
        return "Apartament";
    case TipProprietate::teren:
        return "Teren";
    default:
        return "Necunoscut";
    }
}

void ProprietateWidget::afiseazaTextSiImagini()
{
    if(!this->m_proprietate)
        return;

    QFont font("Gabriola", 15, QFont::Normal);

    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);

    widget->setMinimumSize(200, 50);

    if(!this->m_proprietate->getImagini().empty())
    {
        QPixmap pic(m_proprietate->getImagini()[0].c_str());
        QLabel *imagine = new QLabel(this);
        imagine->setPixmap(pic.scaled(400, 500, Qt::KeepAspectRatio));
        layout->addWidget(imagine);
    }

    QVBoxLayout *proprietateLayout = new QVBoxLayout();

    QLabel *tipLabel = new QLabel("Tip: " + QString::fromStdString(toStringFunc(m_proprietate->getTip())), this);
    tipLabel->setFont(font);
    proprietateLayout->addWidget(tipLabel);

    QLabel *tipProprLabel = new QLabel("Proprietate de " + QString::fromStdString(m_proprietate->getTipAnunt()), this);
    tipProprLabel->setFont(font);
    proprietateLayout->addWidget(tipProprLabel);

    QLabel *locLabel = new QLabel("Locație: " + QString::fromStdString(m_proprietate->getLocatie()), this);
    locLabel->setFont(font);
    proprietateLayout->addWidget(locLabel);

    QLabel *pretLabel = new QLabel("Preț: " + QString::number(m_proprietate->getPret()) + " € ", this);
    pretLabel->setFont(font);
    proprietateLayout->addWidget(pretLabel);

    QLabel *suprafataLabel = new QLabel("Suprafață: " + QString::number(m_proprietate->getSuprafata()) + " m2 ", this);
    suprafataLabel->setFont(font);
    proprietateLayout->addWidget(suprafataLabel);

    QLabel *vanzatorLabel = new QLabel("Postat de: " + QString::fromStdString(m_proprietate->getNumeVanzator()), this);
    vanzatorLabel->setFont(font);
    proprietateLayout->addWidget(vanzatorLabel);

    proprietateLayout->setSpacing(10);
    if(m_apelant == "profil" && Client::getInstance().isAdmin() == false)
    {
        if(this->m_proprietate->getNumeVanzator() == Client::getInstance().getUsername())
        {
            QPushButton *stergePostare = new QPushButton("Șterge postare! ", this);
            stergePostare->setFont(font);
            proprietateLayout->addWidget(stergePostare);

            connect(stergePostare, &QPushButton::pressed, this, &ProprietateWidget::stergePostare);
        }
    }

    QPushButton *afisDetalii = new QPushButton(" Apasă aici pentru mai multe detalii! ", this);
    afisDetalii->setFont(font);
    proprietateLayout->addWidget(afisDetalii);

    layout->addLayout(proprietateLayout);

    layout->setSpacing(50);
    widget->adjustSize();

    connect(afisDetalii, &QPushButton::pressed, this, &ProprietateWidget::afiseazaDetalii);
}

void ProprietateWidget::stergePostare()
{
    Client::getInstance().delete_ad(std::to_string(this->m_proprietate->getIdPostare()).c_str());
    if(Client::getInstance().getAddDeletedSuccesfully() == true)
    {
        QMessageBox::information(this, "Informare", "Postarea a fost ștearsă cu succes!");
        ProfilWindow * pw = new ProfilWindow();
        pw->show();
    }
    else
    {
        QMessageBox::critical(this, "Atenție!", "Postarea nu a putut fi ștearsă!");
    }
}

void ProprietateWidget::afiseazaDetalii()
{
    property_type type;
    if(m_proprietate->getTip() == TipProprietate::casa)
    {
        type = property_type::house;
    }
    else if (m_proprietate->getTip() == TipProprietate::apartament)
    {
        type = property_type::apartment;
    }
    else
    {
        type = property_type::land;
    }

    Client::getInstance().clearFeedback();
    Client::getInstance().request_ad_details(std::to_string(m_proprietate->getIdPostare()).c_str(),type);

    Client::getInstance().clearVectorFav();
    Client::getInstance().request_feed_fav(Client::getInstance().getUsername());

    if(!Client::getInstance().isAdmin())
    {
        if(this->m_proprietate->getStatusPostare() == true)
        {
            QMessageBox::critical(this, "Atenție!", "Postarea este blocată, nu puteți vizualiza detaliile!");
            return;
        }
    }

    PostareWindow *postw = new PostareWindow(m_proprietate, this, m_apelant);

    postw->show();
}

