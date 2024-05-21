#ifndef MESAGERIEWINDOW_H
#define MESAGERIEWINDOW_H

#include "qlineedit.h"
#include "qscrollarea.h"
#include <QMainWindow>

namespace Ui {
class MesagerieWindow;
}

class MesagerieWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MesagerieWindow(QWidget *parent = nullptr, std::string receiver= "", std::string pagina = "");
    ~MesagerieWindow();

    void prieteni();
    void conversatie(std::string nume);
    void adaugaConversatie(std::string destinatar);
    void adaugaButonIntoarcere();
    void onIntoarceButtonPressed();
    void trimiteMesajul();

private:
    Ui::MesagerieWindow *ui;

    QScrollArea *scrollArea;

    std::string m_nume;
    std::string m_pagina;

    QLineEdit *lineMsg;
};

#endif // MESAGERIEWINDOW_H
