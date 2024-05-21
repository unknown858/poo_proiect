#include "secondwindow.h"
#include "loginwindow.h"
#include "ui_secondwindow.h"
#include "signinwindow.h"

SecondWindow::SecondWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
    showFullScreen();

    loadResources();
}

void SecondWindow::loadResources()
{
    adaugaWhitebg();
    adaugaImagine();
    adaugaButonSignin();
    adaugaButonLogin();
}

SecondWindow::~SecondWindow()
{
    delete ui;
}

void SecondWindow::adaugaWhitebg()
{
    white_bg=new QLabel(this);
    white_bg->setStyleSheet("background-color: white;");
    white_bg->setGeometry(0, 0, width(), height());
    white_bg->show();
}

void SecondWindow::adaugaImagine()
{
    QPixmap pic("C:/imagini/scndwindow_pic.jpg");

    imagine = new QLabel (this);

    imagine->setPixmap(pic);
    imagine->setScaledContents(true);
    imagine->setGeometry(550,100, 600,600);

    imagine->show();
}

void SecondWindow::adaugaButonSignin()
{
    signin = new QPushButton ("Înregistrează-te", this);

    signin->setGeometry(200,320,180,60);

    QFont font("Gabriola", 20, QFont::Normal);
    signin->setFont(font);

    signin->show();
    connect(signin, &QPushButton::clicked, this, &SecondWindow::onSigninButtonPressed);
}

void SecondWindow::adaugaButonLogin()
{
    login = new QPushButton ("Conectează-te", this);

    login->setGeometry(200,450,180,60);

    QFont font("Gabriola", 20, QFont::Normal);
    login->setFont(font);

    login->show();
    connect(login, &QPushButton::clicked, this, &SecondWindow::onLoginButtonPressed);
}

void SecondWindow::onLoginButtonPressed()
{
    LoginWindow *loginw = new LoginWindow(this);

    loginw->show();
}

void SecondWindow::onSigninButtonPressed()
{
    SigninWindow *signinw = new SigninWindow(this);

    signinw->show();
}
