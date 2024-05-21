#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class SecondWindow;
}
QT_END_NAMESPACE

class SecondWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);
    ~SecondWindow();
    void loadResources();

private slots:
    void adaugaWhitebg();
    void adaugaImagine();
    void adaugaButonLogin();
    void adaugaButonSignin();
    void onLoginButtonPressed();
    void onSigninButtonPressed();

private:
    Ui::SecondWindow *ui;
    QLabel *white_bg;
    QLabel *imagine;
    QPushButton *signin;
    QPushButton *login;
};

#endif // SECONDWINDOW_H
