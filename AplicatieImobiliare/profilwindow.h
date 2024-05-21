#ifndef PROFILWINDOW_H
#define PROFILWINDOW_H

#include "qlineedit.h"
#include "qscrollarea.h"
#include "client.h"

#include <QMainWindow>

namespace Ui {
class ProfilWindow;
}

class ProfilWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProfilWindow(QWidget *parent = nullptr, std::string username = Client::getInstance().getUsername());
    ~ProfilWindow();
    void afiseazaFeed();
    void adaugaButonIntoarcere();
    void onIntoarceButtonPressed();
    void afiseazaNumeClient();
    void afisezaFavorite();
    void adaugaButonVizualizeazaFavorite();
    void adaugaButonAdaugaPostare();
    void onAdaugaPostareButtonPressed();
    void adaugaLabelIntroducereSold();
    void adaugaSoldul();
    void adaugaSoldCurent();

private:
    Ui::ProfilWindow *ui;
    QScrollArea *scrollArea;

    QLineEdit *lineMsg;

    std::string m_username;

};

#endif // PROFILWINDOW_H
