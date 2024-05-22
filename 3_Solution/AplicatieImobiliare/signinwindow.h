#ifndef SIGNINWINDOW_H
#define SIGNINWINDOW_H

#include <QGroupBox>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

struct DateTrimitereCont {
    QString nume;
    QString prenume;
    QString telefon;
    QString email;
    QString username;
    QString parola;
};

namespace Ui {
class SigninWindow;
}

class SigninWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SigninWindow(QWidget *parent = nullptr, std::string username = "", std::string tip = "");
    ~SigninWindow();

private slots:
    void adaugaImagineFundal();
    void creeazaTabelInformatii();
    void creeazaLabelLineEdit();
    void creeazaNumePrenume();
    void creeazaButonIntoarcere();
    void creeazaButonCreareCont();
    void onIntoarceButtonPressed();
    void trimitereDateCont();
    void creeazaEmail();
    void creeazaUserParola();
    void creeazaTelefon();
    bool verificaDate(QString nume, QString prenume, QString telefon, QString email, QString username, QString parola);

    void clearBox();

private:
    std::string m_username;
    std::string m_tip;

    Ui::SigninWindow *ui;
    QGroupBox *groupBox;
    QVBoxLayout *groupBoxLayout;
    QVBoxLayout *labelLineEditLayout;
    QLineEdit *lineNume;
    QLineEdit *linePrenume;
    QLineEdit *lineEmail;
    QLineEdit *lineUsername;
    QLineEdit *linePass;
    QLineEdit *lineTelefon;
    QPushButton *button;
};

#endif // SIGNINWINDOW_H
