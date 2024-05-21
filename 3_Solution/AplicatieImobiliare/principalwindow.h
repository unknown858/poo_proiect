#ifndef PRINCIPALWINDOW_H
#define PRINCIPALWINDOW_H

#include "Proprietati.h"
#include "qcheckbox.h"
#include "qscrollarea.h"

#include <QMainWindow>
#include <QListWidget>

namespace Ui {
class PrincipalWindow;
}

class PrincipalWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrincipalWindow(QWidget *parent = nullptr, QString numeClient = "Neînregistrat", const char * filtre = "000000");
    ~PrincipalWindow();


private slots:
    void afiseazaFeed(const char*filtre = "000000");
    void adaugaFiltrele();
    void sortare();
    void adaugaWhiteBg();
    void afiseazaNumeClient(std::string numeClient);

    void adaugaButonAdaugaPostare();
    void onAdaugaPostareButtonPressed();
    void adaugaProprietate(Proprietate*propr){this->proprietati.push_back(propr);}
    void adaugaButonVizualizeazaProfil();
    void onButonVizualizeazaProfilPressed();
    void adaugaButonAdaugaOfertant();
    void onAdaugaOfertantPressed();
    void adaugaButonVizualizareChat();
    void onVizualizeazaChatPressed();

private:
    Ui::PrincipalWindow *ui;
    std::vector <Proprietate*> proprietati;
    QScrollArea *scrollArea;


    QCheckBox *checkboxCasa;
    QCheckBox *checkboxApartament;
    QCheckBox *checkboxTeren;
    QCheckBox *checkbox0_500;
    QCheckBox *checkbox500_1000 ;
    QCheckBox *checkbox1000_5000;
    QCheckBox *checkbox5000_10000;
    QCheckBox *checkbox10000_50000 ;
    QCheckBox *checkbox50000_100000 ;
    QCheckBox *checkbox100000_200000;
    QCheckBox *checkbox200000_plus;
    QCheckBox *checkboxVanzare;
    QCheckBox *checkboxCumparare ;
    QCheckBox *checkboxCrescator;
    QCheckBox *checkboxDescrescator;
    QCheckBox *checkbox0_50;
    QCheckBox *checkbox50_100;
    QCheckBox *checkbox100_200;
    QCheckBox *checkbox200_500 ;
    QCheckBox *checkbox500_plus;

    QCheckBox *nimicPret;
    QCheckBox *nimicSortPret;
    QCheckBox *nimicTipAnunt;
    QCheckBox *nimicSuprafata;

    QCheckBox *checkboxTip1;
    QCheckBox *checkboxTip2;
    QCheckBox *nimicTip;

};

#endif // PRINCIPALWINDOW_H
