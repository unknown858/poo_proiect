#include "signinwindow.h"
#include "principalwindow.h"
#include "qregularexpression.h"
#include "ui_signinwindow.h"
#include "secondwindow.h"
#include "loginwindow.h"
#include "client.h"

#include <QLabel>
#include <QString>
#include <QLineEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

SigninWindow::SigninWindow(QWidget *parent, std::string username, std::string tip)
    : QMainWindow(parent)
    , m_username(username), m_tip(tip), ui(new Ui::SigninWindow)
{
    ui->setupUi(this);
    showFullScreen();

    adaugaImagineFundal();
    creeazaTabelInformatii();
    creeazaButonIntoarcere();
    creeazaButonCreareCont();
}

SigninWindow::~SigninWindow()
{
    delete ui;
}

void SigninWindow::adaugaImagineFundal()
{
    QLabel *white_bg=new QLabel(this);
    white_bg->setStyleSheet("background-color: white;");
    white_bg->setGeometry(0, 0, width(), height());
    white_bg->show();

    QPixmap image("C:/imagini/sign.jpg");

    QLabel *label = new QLabel(this);
    label->setPixmap(image);
    label->setScaledContents(true);
    label->setGeometry(500, 600, 300, 200);

    label->show();
}

void SigninWindow::creeazaButonIntoarcere()
{
    QPushButton *button = new QPushButton ("Întoarce-te", this);

    button->setGeometry(200,650,180,50);

    QFont font("Gabriola", 18, QFont::Normal);
    button->setFont(font);

    button->show();
    connect(button, &QPushButton::clicked, this, &SigninWindow::onIntoarceButtonPressed);
}

void SigninWindow::creeazaButonCreareCont()
{
    button = new QPushButton ("Creează cont", this);

    button->setGeometry(900,650,180,50);

    QFont font("Gabriola", 18, QFont::Normal);
    button->setFont(font);

    button->show();

    connect(button, &QPushButton::clicked, this, &SigninWindow::trimitereDateCont);
}

void SigninWindow::trimitereDateCont()
{

    QString nume = lineNume->text();
    QString prenume = linePrenume->text();
    QString telefon = lineTelefon->text();
    QString email = lineEmail->text();
    QString username = lineUsername->text();
    QString parola = linePass->text();

    bool dateGresite = verificaDate(nume, prenume, telefon, email, username, parola);
    if(dateGresite)
    {
        creeazaButonCreareCont();
    }
    else
    {
        std::string snume = nume.toStdString();
        std::string sprenume = prenume.toStdString();
        std::string stelefon = telefon.toStdString();
        std::string semail = email.toStdString();
        std::string susername = username.toStdString();
        std::string sparola = parola.toStdString();

        int res = Client::getInstance().sign_in(snume.c_str(),sprenume.c_str(),semail.c_str(), stelefon.c_str(), susername.c_str(), sparola.c_str());

        if(res==0)
        {
            if(m_tip == "administrator")
            {
                QMessageBox::information(this, "Informare", "Contul a fost creat cu succes! Vei fi redirectat către pagina principală <3");
                PrincipalWindow *pw = new PrincipalWindow(nullptr, this->m_username.c_str());
                pw->show();
            }
            else
            {
                QMessageBox::information(this, "Informare", "Contul a fost creat cu succes! Vei fi redirectat către pagina de login <3");
                LoginWindow *lw =new LoginWindow;
                lw->show();
            }
        }
        else if(res==1)
        {
            QMessageBox::critical(this, "Atenție!", "Înregistrare eșuată!");
            clearBox();
        }
        else if(res==2)
        {
            QMessageBox::critical(this, "Atenție!", "Username existent!");
            clearBox();
        }
    }
}

void SigninWindow::clearBox()
{
    this->lineEmail->clear();
    this->lineNume->clear();
    this->lineEmail->clear();
    this->linePrenume->clear();
    this->lineUsername->clear();
    this->linePass->clear();
    this->lineTelefon->clear();
}

bool SigninWindow::verificaDate(QString nume, QString prenume, QString telefon, QString email,
                                QString username, QString parola)
{
    if (nume.isEmpty() || prenume.isEmpty() || telefon.isEmpty() || email.isEmpty() || username.isEmpty() || parola.isEmpty()) {
        QMessageBox::critical(this, "Eroare", "Toate câmpurile trebuie completate!");
        return 1;
    }

    QRegularExpression reNr("^\\+?[0-9]{1,3}-?[0-9]{3,14}$");
    QRegularExpressionMatch matchNr = reNr.match(telefon);
    if (!matchNr.hasMatch()) {
        lineTelefon->setStyleSheet("QLineEdit { color: red; }");
        QMessageBox::critical(this, "Eroare", "Numărul de telefon nu este valid.");
        return 1;
    }

    QRegularExpression reEmail("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}\\b");
    QRegularExpressionMatch matchEmail = reEmail.match(email);
    if (!matchEmail.hasMatch()) {
        lineEmail->setStyleSheet("QLineEdit { color: red; }");
        QMessageBox::critical(this, "Eroare", "Adresa de email nu este validă!");
        return 1;
    }

    QRegularExpression reUser("^[a-zA-Z0-9_]*$"); // Expresie regulată care acceptă doar litere, cifre și caracterul "_"
    QRegularExpressionMatch matchUser = reUser.match(username);
    if (!matchUser.hasMatch()) {
        lineUsername->setStyleSheet("QLineEdit { color: red; }");
        QMessageBox::critical(this, "Eroare", "Numele de utilizator conține caractere nepermise!");
        return 1;
    }

    if (parola.length() < 6) {
        linePass->setStyleSheet("QLineEdit { color: red; }");
        QMessageBox::critical(this, "Eroare", "Parola trebuie să conțină cel puțin 6 caractere.");
        return 1;
    }

    return 0;
}

void SigninWindow::onIntoarceButtonPressed()
{
    if(m_tip == "administrator")
    {
        PrincipalWindow *pw = new PrincipalWindow(nullptr, this->m_username.c_str());
        pw->show();
    }
    else
    {
        SecondWindow *secondw = new SecondWindow(this);
        secondw->show();
    }
}

void SigninWindow::creeazaTabelInformatii()
{
    groupBox= new QGroupBox ("Date personale",this);
    QFont font("Gabriola", 17, QFont::Normal);
    groupBox->setFont(font);

    groupBox->setGeometry(450,100,400,500);

    groupBoxLayout = new QVBoxLayout(groupBox);

    labelLineEditLayout = new QVBoxLayout;

    creeazaLabelLineEdit();

    layout()->addWidget(groupBox);

}

void SigninWindow::creeazaNumePrenume()
{
    QLabel *label = new QLabel("Nume:", groupBox);
    QLabel *label2 = new QLabel("Prenume:", groupBox);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    label2->setFont(font);

    QHBoxLayout *hlayout = new QHBoxLayout(groupBox);
    hlayout->addWidget(label);
    hlayout->addWidget(label2);

    labelLineEditLayout->addLayout(hlayout);

    lineNume = new QLineEdit(this);
    linePrenume = new QLineEdit(this);

    hlayout = new QHBoxLayout(groupBox);
    hlayout->addWidget(lineNume);
    hlayout->addWidget(linePrenume);

    labelLineEditLayout->addLayout(hlayout);

    groupBoxLayout->addLayout(labelLineEditLayout);
}

void SigninWindow::creeazaEmail()
{
    QLabel *label = new QLabel("Email:", groupBox);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);
    lineEmail = new QLineEdit(groupBox);
    labelLineEditLayout->addWidget(lineEmail);

    groupBoxLayout->addLayout(labelLineEditLayout);
}

void SigninWindow::creeazaUserParola()
{
    QLabel *label = new QLabel("Username:", groupBox);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);
    lineUsername = new QLineEdit(groupBox);
    labelLineEditLayout->addWidget(lineUsername);
    groupBoxLayout->addLayout(labelLineEditLayout);

    label = new QLabel("Parolă:",groupBox);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);
    linePass = new QLineEdit(groupBox);
    labelLineEditLayout->addWidget(linePass);
    linePass->setEchoMode(QLineEdit::Password);
    groupBoxLayout->addLayout(labelLineEditLayout);
}

void SigninWindow::creeazaTelefon()
{
    QLabel *label = new QLabel("Număr de telefon:", groupBox);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);
    lineTelefon = new QLineEdit(groupBox);
    labelLineEditLayout->addWidget(lineTelefon);
    groupBoxLayout->addLayout(labelLineEditLayout);
}

void SigninWindow::creeazaLabelLineEdit()
{
    creeazaNumePrenume();
    creeazaTelefon();
    creeazaEmail();
    creeazaUserParola();
}
