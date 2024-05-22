#include "loginwindow.h"
#include "qregularexpression.h"
#include "ui_loginwindow.h"
#include "secondwindow.h"
#include "client.h"
#include "principalwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    showFullScreen();

    adaugaImagine();
    creeazaTabelInformatii();
    creeazaButonConectare();
    creeazaButonIntoarcere();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::adaugaImagine()
{
    QLabel *white_bg=new QLabel(this);
    white_bg->setStyleSheet("background-color: white;");
    white_bg->setGeometry(0, 0, width(), height());
    white_bg->show();

    QPixmap image("C:/imagini/modern_city.jpg");

    QLabel *label = new QLabel(this);
    label->setPixmap(image);
    label->setScaledContents(true);
    label->setGeometry(600, 190, 600, 300);
    label->show();
}

void LoginWindow::creeazaTabelInformatii()
{
    groupBox= new QGroupBox ("Date personale",this);
    QFont font("Gabriola", 17, QFont::Normal);
    groupBox->setFont(font);

    groupBox->setGeometry(200,240,300,200);

    groupBoxLayout = new QVBoxLayout(groupBox);

    labelLineEditLayout = new QVBoxLayout;

    creeazaLabelLineEdit();

    layout()->addWidget(groupBox);

}

void LoginWindow::creeazaLabelLineEdit()
{
    QLabel *label = new QLabel("Username:", groupBox);
    QFont font("Gabriola", 15, QFont::Normal);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);
    lineUsername = new QLineEdit(groupBox);

    labelLineEditLayout->addWidget(lineUsername);
    groupBoxLayout->addLayout(labelLineEditLayout);

    label = new QLabel("Parolă:",this);
    label->setFont(font);
    labelLineEditLayout->addWidget(label);
    linePass = new QLineEdit(groupBox);
    linePass->setEchoMode(QLineEdit::Password);

    labelLineEditLayout->addWidget(linePass);
    groupBoxLayout->addLayout(labelLineEditLayout);
}


void LoginWindow::creeazaButonIntoarcere()
{
    QPushButton *button = new QPushButton ("Întoarce-te", this);

    button->setGeometry(200,650,180,50);

    QFont font("Gabriola", 18, QFont::Normal);
    button->setFont(font);

    button->show();
    connect(button, &QPushButton::clicked, this, &LoginWindow::onIntoarceButtonPressed);
}

void LoginWindow::creeazaButonConectare()
{
    QPushButton *button = new QPushButton ("Autentifică-te", this);

    button->setGeometry(900,650,180,50);

    QFont font("Gabriola", 18, QFont::Normal);
    button->setFont(font);

    button->show();
    connect(button, &QPushButton::clicked, this, &LoginWindow::onConectareButtonPressed);
}

void LoginWindow::onConectareButtonPressed()
{
    QString username = lineUsername->text();
    QString parola = linePass->text();

    std::string susername = username.toStdString();
    std::string sparola = parola.toStdString();

    int res = Client::getInstance().log_in(susername.c_str(),sparola.c_str());

    if(res==0)
    {
        deschidePaginaPrincipala();
    }
    else if(res==1)
    {
        this->lineUsername->clear();
        QMessageBox::critical(this, "Atenție!", "Username-ul este greșit!");
    }
    else if(res==2)
    {
        QMessageBox::critical(this, "Atenție!", "Parola este greșită!");
        this->linePass->clear();
    }
}

void LoginWindow::deschidePaginaPrincipala()
{
    QString username = lineUsername->text();

    Client::getInstance().adaugaUsername(username.toStdString());
    PrincipalWindow *pw = new PrincipalWindow(nullptr, username);
    pw->show();

    //this->close();
}

void LoginWindow::onIntoarceButtonPressed()
{
    SecondWindow *secondw = new SecondWindow(this);
    secondw->show();

    //this->close();
}

bool LoginWindow::verificaDate(QString username, QString parola)
{
    if (username.isEmpty() || parola.isEmpty()) {
        QMessageBox::critical(this, "Eroare", "Toate câmpurile trebuie completate!");
        return 1;
    }


    QRegularExpression reUser("^[a-zA-Z0-9_]*$"); // Expresie regulată care acceptă doar litere, cifre și caracterul "_"
    QRegularExpressionMatch matchUser = reUser.match(username);
    if (!matchUser.hasMatch()) {
        lineUsername->setStyleSheet("QLineEdit { color: red; }");
        QMessageBox::critical(this, "Eroare", "Numele de utilizator conține caractere nepermise!");
        return 1;
    }

    if (parola.length() < 5) {
        linePass->setStyleSheet("QLineEdit { color: red; }");
        QMessageBox::critical(this, "Eroare", "Parola trebuie să conțină cel puțin 6 caractere.");
        return 1;
    }

    return 0;
}
