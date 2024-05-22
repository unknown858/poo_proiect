#include "favoritewindow.h"
#include "profilwindow.h"
#include "qlabel.h"
#include "qlayout.h"
#include "client.h"
#include "qpushbutton.h"
#include "qscrollarea.h"
#include "ui_favoritewindow.h"

FavoriteWindow::FavoriteWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FavoriteWindow)
{
    ui->setupUi(this);
    showFullScreen();

    adaugaBg();
    afiseazaNumeClient();
    afiseazaFeed();
    adaugaButonIntoarcere();
}

void FavoriteWindow::adaugaBg()
{
    QPixmap image("C:/imagini/favs.jpg");

    QLabel *label = new QLabel(this);
    label->setPixmap(image);
    label->setScaledContents(true);

    label->setGeometry(0,0,width(),height());

    label->show();
}

FavoriteWindow::~FavoriteWindow()
{
    delete ui;
}

void FavoriteWindow::afiseazaNumeClient()
{
    QLabel *numeC = new QLabel(Client::getInstance().getUsername().c_str(), this);

    layout()->addWidget(numeC);

    QFont font("Gabriola", 18, QFont::Normal);
    numeC->setFont(font);

    QRect rect = numeC->fontMetrics().boundingRect(Client::getInstance().getUsername().c_str());

    int x = width() - rect.width() - 15;
    int y = 15;

    numeC->move(x, y);
    numeC->adjustSize();

    numeC->setStyleSheet("background-color: rgb(254, 253, 240)");
}

void FavoriteWindow::adaugaButonIntoarcere()
{
    QPushButton *button = new QPushButton ("Întoarce-te", this);
    button->setGeometry(80,50,120,40);

    QFont font("Gabriola", 15, QFont::Normal);
    button->setFont(font);

    button->show();
    connect(button, &QPushButton::clicked, this, &FavoriteWindow::onIntoarceButtonPressed);
}

void FavoriteWindow::onIntoarceButtonPressed()
{
    ProfilWindow *pw = new ProfilWindow();
    pw->show();

    this->close();
}

void FavoriteWindow::afiseazaFeed()
{
    QWidget *containerWidget = new QWidget(this);
    QHBoxLayout *containerLayout = new QHBoxLayout(containerWidget);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    containerLayout->addWidget(scrollArea);

    layout()->addWidget(containerWidget);

    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);

    Client::getInstance().clearVectorFav();
    Client::getInstance().request_feed_fav(Client::getInstance().getUsername());

    std::vector<Proprietate*> proprietati = Client::getInstance().getFavorite();

    for (auto proprietate : proprietati)
    {
        ProprietateWidget *proprietateWidget = new ProprietateWidget(proprietate, this, "favorite");

        proprietateWidget->setFixedSize(700, 350);
        contentLayout->addWidget(proprietateWidget);
    }

    contentWidget->setLayout(contentLayout);

    scrollArea->setWidget(contentWidget);

    containerWidget->setMinimumSize(1000, 600);

    layout()->addWidget(containerWidget);

    containerWidget->move(25, 150);
}
