#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void adaugaImagine();
    void creeazaTabelInformatii();
    void creeazaLabelLineEdit();
    void creeazaButonIntoarcere();
    void creeazaButonConectare();
    void onIntoarceButtonPressed();
    void onConectareButtonPressed();
    //void trimiteDateServer();
    bool verificaDate(QString username, QString parola);
    void deschidePaginaPrincipala();
    //void trimitere(QString username, QString parola);

private:
    Ui::LoginWindow *ui;
    QGroupBox *groupBox;
    QVBoxLayout *groupBoxLayout;
    QVBoxLayout *labelLineEditLayout;
    QLineEdit *lineUsername;
    QLineEdit *linePass;
};

#endif // LOGINWINDOW_H
