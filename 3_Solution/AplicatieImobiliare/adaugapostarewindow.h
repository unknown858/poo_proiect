#ifndef ADAUGAPOSTAREWINDOW_H
#define ADAUGAPOSTAREWINDOW_H

#include "qboxlayout.h"
#include "qcheckbox.h"
#include "qgroupbox.h"
#include "qlineedit.h"
#include "client.h"

#include <QMainWindow>

namespace Ui {
class AdaugaPostareWindow;
}

class AdaugaPostareWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdaugaPostareWindow(QWidget *parent = nullptr, std::string pagina ="");
    ~AdaugaPostareWindow();

    void introduDatele();
    void creeazaTipAnunt();
    void creeazaTipPropr();
    void creeazaDesc();
    void creeazaPret();
    void creeazaLocatie();
    void creeazaSuprafata();
    void creeazaNrCamere();

    void adaugaButonTrimitere();
    void adaugaButonIntoarcere();
    void onIntoarcerePressed();

    bool verificaDate(QString l_nrCamere, QString l_descriere, QString l_pret, QString l_locatie, QString l_suprafata);
    void clearBox();
    void trimitereDatePostare();

    void introduCaile();
    void creeazaCale1();
    void creeazaCale2();
    void creeazaCale3();
    void creeazaCale4();
    void creeazaCale5();
    void creeazaCale6();

    ad_type getAddType();
    property_type getPropretyType();

    void onBtnTerenToggled(bool checked);
    void adaugaBg();

private:
    std::string m_pagina;

    Ui::AdaugaPostareWindow *ui;

    QHBoxLayout *checkboxLayout;
    QHBoxLayout *checkboxLayout1;

    QGroupBox * gbox;
    QVBoxLayout *groupBoxLayout;
    QVBoxLayout *labelLineEditLayout;

    QGroupBox * gbox1;
    QVBoxLayout *groupBoxLayout1;
    QVBoxLayout *labelLineEditLayout1;

    QLineEdit *tipAnunt;
    QLineEdit *tipProp;
    QLineEdit *descriere;
    QLineEdit *pret;
    QLineEdit *locatie;
    QLineEdit *suprafata;
    QLineEdit *nrCamere;

    QLineEdit *cale1;
    QLineEdit *cale2;
    QLineEdit *cale3;
    QLineEdit *cale4;
    QLineEdit *cale5;
    QLineEdit *cale6;

    QCheckBox *btnVanzare;
    QCheckBox *btnInchiriere;
    QCheckBox *btnCasa;
    QCheckBox *btnApartament;
    QCheckBox *btnTeren;
};

#endif // ADAUGAPOSTAREWINDOW_H
