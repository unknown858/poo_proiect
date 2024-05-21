#ifndef FAVORITEWINDOW_H
#define FAVORITEWINDOW_H

#include "qscrollarea.h"
#include <QMainWindow>

namespace Ui {
class FavoriteWindow;
}

class FavoriteWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FavoriteWindow(QWidget *parent = nullptr);
    ~FavoriteWindow();

    void onIntoarceButtonPressed();
    void afiseazaFeed();
    void adaugaButonIntoarcere();
    void afiseazaNumeClient();
    void adaugaBg();


private:
    Ui::FavoriteWindow *ui;
    QScrollArea *scrollArea;
};

#endif // FAVORITEWINDOW_H
