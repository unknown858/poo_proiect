#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "secondwindow.h"

#include <QStackedLayout>
#include <qtimer.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    showFullScreen();

    setImage();
    setTitle();

    QTimer::singleShot(2000, this, &MainWindow::afiseazaSecondWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTitle()
{
    titlu = new QLabel ("Cel mai bun site de imobiliare!", this);

    QFont font("Gabriola", 50, QFont::Normal);
    titlu->setFont(font);

    titlu->adjustSize();

    int windowWidth = width();

    int labelWidth = titlu->width();

    int labelX = (windowWidth - labelWidth) / 2;
    int labelY = 300;

    titlu->move(labelX, labelY);

    titlu->show();
}

void MainWindow::setImage()
{
    imagine_pagprinc=new QLabel(this);
    QPixmap imagine("C:/imagini/imag_mainwindow.jpg");

    imagine_pagprinc->setPixmap(imagine);
    imagine_pagprinc->setScaledContents(true);

    imagine_pagprinc->setGeometry(0, 0, width(), height());

    imagine_pagprinc->show();
}

void MainWindow::afiseazaSecondWindow()
{
    SecondWindow *secondWindow = new SecondWindow(this);
    secondWindow->show();

    //this->hide();
}
