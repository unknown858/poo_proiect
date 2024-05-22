#include "cumparawindow.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "ui_cumparawindow.h"
#include "principalwindow.h"
#include "client.h"
#include "multumirewindow.h"

#include <QMessageBox>

CumparaWindow::CumparaWindow(QWidget *parent, Proprietate* propr)
    : QMainWindow(parent), m_propr(propr)
    , ui(new Ui::CumparaWindow)
{
    ui->setupUi(this);
    showFullScreen();

    adaugaBg();
    creeazaButonIntoarcere();
    creeazaButonCumpara();
    creeazaSpatiuIntroducereDateCard();
    adaugaPretul();
}

void CumparaWindow::adaugaBg()
{
    QPixmap image("C:/imagini/bg_addpic.jpg");

    QLabel *label = new QLabel(this);
    label->setPixmap(image);
    label->setScaledContents(true);

    label->setGeometry(0,0,width(),height());
    label->show();
}

void CumparaWindow::creeazaButonIntoarcere()
{
    QPushButton *button = new QPushButton ("Întoarce-te", this);
    button->setStyleSheet("background-color: rgb(254, 253, 240)");
    button->setGeometry(200,650,180,50);

    QFont font("Gabriola", 18, QFont::Normal);
    button->setFont(font);

    button->show();
    connect(button, &QPushButton::clicked, this, &CumparaWindow::onIntoarceButtonPressed);
}

void CumparaWindow::creeazaButonCumpara()
{
    QPushButton *button = new QPushButton ("Cumpără!", this);
    button->setStyleSheet("background-color: rgb(254, 253, 240)");
    button->setGeometry(900,650,180,50);

    QFont font("Gabriola", 18, QFont::Normal);
    button->setFont(font);

    button->show();

    connect(button, &QPushButton::clicked, this, &CumparaWindow::incepeTranzactie);
}

void CumparaWindow::incepeTranzactie()
{
    int res = valideaza();

    if(res == 1)
    {
        creeazaButonCumpara();
    }
    else
    {
        if(this->m_propr->getStatusPostare() == false)
        {
            Client::getInstance().buy_property(Client::getInstance().getUsername(), std::to_string(m_propr->getIdPostare()).c_str(), std::to_string(m_propr->getPret()).c_str());


            if(Client::getInstance().getStatusTranzactie() == 0)
            {
                QMessageBox::information(this, "Informare", "Tranzacție efectuată cu succes!");

                MultumireWindow *mw = new MultumireWindow();
                mw->show();
            }
            else if(Client::getInstance().getStatusTranzactie() == 1)
            {
                QMessageBox::critical(this, "Atenție", "Sold insuficient!");
            }
            else {
                QMessageBox::critical(this, "Atenție", "Tranzacție eșuată!");
            }
        }
        else
        {
            QMessageBox::critical(this, "Atenție!", "Postarea este blocată! Ofertantul a fost șters de pe platformă!");
            PrincipalWindow *pr=new PrincipalWindow(nullptr, Client::getInstance().getUsername().c_str());
            pr->show();

            this->close();
        }
    }
}

void CumparaWindow::creeazaSpatiuIntroducereDateCard()
{
    groupBox= new QGroupBox ("Date card",this);
    groupBox->setStyleSheet("background-color: rgb(254, 253, 240)");

    QFont font("Gabriola", 17, QFont::Normal);
    groupBox->setFont(font);

    groupBox->setGeometry(450,100,400,500);

    groupBoxLayout = new QVBoxLayout(groupBox);

    labelLineEditLayout = new QVBoxLayout;

    adaugaNumeDetinator();
    adaugaIBAN();
    adaugaDataExpCVC();

    layout()->addWidget(groupBox);
}

void CumparaWindow::adaugaNumeDetinator()
{
    QLabel *label = new QLabel("Adaugă nume proprietar:", groupBox);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);
    numeDetinator = new QLineEdit(groupBox);
    labelLineEditLayout->addWidget(numeDetinator);

    groupBoxLayout->addLayout(labelLineEditLayout);
}

void CumparaWindow::adaugaIBAN()
{
    QLabel *label = new QLabel("IBAN:", groupBox);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);

    iban = new QLineEdit(groupBox);

    labelLineEditLayout->addWidget(iban);

    groupBoxLayout->addLayout(labelLineEditLayout);
}

void CumparaWindow::adaugaDataExpCVC()
{
    QLabel *label = new QLabel("Data expirării:", groupBox);
    QLabel *label2 = new QLabel("CVC:", groupBox);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    label2->setFont(font);

    QHBoxLayout *hlayout = new QHBoxLayout(groupBox);
    hlayout->addWidget(label);
    hlayout->addWidget(label2);
    labelLineEditLayout->addLayout(hlayout);

    dataExp = new QLineEdit(this);
    cvc = new QLineEdit(this);

    hlayout = new QHBoxLayout(groupBox);
    hlayout->addWidget(dataExp);
    hlayout->addWidget(cvc);

    labelLineEditLayout->addLayout(hlayout);

    groupBoxLayout->addLayout(labelLineEditLayout);
}

void CumparaWindow::adaugaPretul()
{
    QFont font("Gabriola", 15, QFont::Normal);
    QLabel *label = new QLabel("Preț proprietate: " + QString::number(m_propr->getPret()), groupBox);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);
    groupBoxLayout->addLayout(labelLineEditLayout);
}

CumparaWindow::~CumparaWindow()
{
    delete ui;
}

void CumparaWindow::onIntoarceButtonPressed()
{
    PrincipalWindow *pr=new PrincipalWindow(nullptr, Client::getInstance().getUsername().c_str());
    pr->show();

    //this->close();
}

bool CumparaWindow::valideaza()
{
    if (iban->text().isEmpty() || cvc->text().isEmpty() || this->dataExp->text().isEmpty() || this->numeDetinator->text().isEmpty()) {
        QMessageBox::critical(this, "Eroare", "Toate câmpurile trebuie completate!");
        return 1;
    }

    QRegularExpression reNr("^[A-Z0-9]*$");
    QRegularExpressionMatch matchNr = reNr.match(iban->text());
    if (!matchNr.hasMatch()) {
        iban->setStyleSheet("QLineEdit { color: red; }");
        QMessageBox::critical(this, "Eroare", "IBAN-ul nu este valid.");
        return 1;
    }


    QRegularExpression reNr1("\\d{3,4}");
    QRegularExpressionMatch matchNr1 = reNr.match(cvc->text());
    if (!matchNr1.hasMatch()) {
        cvc->setStyleSheet("QLineEdit { color: red; }");
        QMessageBox::critical(this, "Eroare", "CVC-ul nu este valid.");
        return 1;
    }

    return 0;
}
