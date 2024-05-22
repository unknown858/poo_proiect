#include "multumirewindow.h"
#include "principalwindow.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "ui_multumirewindow.h"
#include "client.h"

MultumireWindow::MultumireWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MultumireWindow)
{
    ui->setupUi(this);
    showFullScreen();

    adaugaImagine();
    adaugaButonIntoarcere();
}

MultumireWindow::~MultumireWindow()
{
    delete ui;
}

void MultumireWindow::adaugaButonIntoarcere()
{
    QPushButton *button = new QPushButton ("Întoarce-te", this);

    button->setGeometry(200,650,180,50);

    QFont font("Gabriola", 18, QFont::Normal);
    button->setFont(font);

    button->show();
    connect(button, &QPushButton::clicked, this, &MultumireWindow::onIntoarceButtonPressed);
}


void MultumireWindow::onIntoarceButtonPressed()
{
    PrincipalWindow *pw = new PrincipalWindow(nullptr, QString::fromStdString(Client::getInstance().getUsername()));
    pw->show();

    this->close();
}


void MultumireWindow::adaugaImagine()
{
    QPixmap pic("C:/imagini/meme.jpg");
    QLabel *imagine = new QLabel(this);

    imagine->setPixmap(pic);
    imagine->setScaledContents(true);

    imagine->setGeometry(430,120,400,400);

    imagine->show();
}
