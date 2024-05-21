#ifndef MULTUMIREWINDOW_H
#define MULTUMIREWINDOW_H

#include <QMainWindow>

namespace Ui {
class MultumireWindow;
}

class MultumireWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MultumireWindow(QWidget *parent = nullptr);
    ~MultumireWindow();

    void adaugaImagine();
    void adaugaButonIntoarcere();
    void onIntoarceButtonPressed();

private:
    Ui::MultumireWindow *ui;
};

#endif // MULTUMIREWINDOW_H
