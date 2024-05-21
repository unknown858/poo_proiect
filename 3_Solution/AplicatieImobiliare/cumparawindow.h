#ifndef CUMPARAWINDOW_H
#define CUMPARAWINDOW_H

#include "qboxlayout.h"
#include "qgroupbox.h"
#include "Proprietati.h"

#include <QMainWindow>

namespace Ui {
class CumparaWindow;
}

class CumparaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CumparaWindow(QWidget *parent = nullptr, Proprietate* propr = nullptr);
    ~CumparaWindow();

    void onIntoarceButtonPressed();
    void creeazaSpatiuIntroducereDateCard();
    void adaugaNumeDetinator();
    void adaugaIBAN();
    void adaugaDataExpCVC();
    void creeazaButonIntoarcere();
    void creeazaButonCumpara();
    void incepeTranzactie();
    void adaugaPretul();
    void adaugaBg();
    bool valideaza();

private:
    Proprietate* m_propr;
    Ui::CumparaWindow *ui;

    QGroupBox *groupBox;
    QVBoxLayout *groupBoxLayout;
    QVBoxLayout *labelLineEditLayout;

    QLineEdit *numeDetinator;
    QLineEdit *iban;
    QLineEdit *dataExp;
    QLineEdit *cvc;
};

#endif // CUMPARAWINDOW_H
