#ifndef POSTAREWINDOW_H
#define POSTAREWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "Proprietati.h"

namespace Ui {
class PostareWindow;
}

class PostareWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PostareWindow(Proprietate* proprietate, QWidget *parent = nullptr, std::string apelant = "");
    ~PostareWindow();

    QHBoxLayout* getLayoutPoze() const { return mainLayout; }
    QVBoxLayout* getLayouText() const {return this->proprietateLayout;}


    int getPostareCurenta() const { return postareCurenta; }
    QPushButton* getPostareAnterioaraButton() const { return postareAnterioaraButton; }
    QPushButton* getPostareUrmatoareButton() const { return postareUrmatoareButton; }

    void setPostareCurenta(int nrPostare) { postareCurenta = nrPostare; }
    void updatePostareLabel() { numarPostareLabel->setText("Postare " + QString::number(postareCurenta)); }
    void postareAnterioara();
    void postareUrmatoare();
    void updateImagini();

    void adaugaImagini();
    void adaugaWhitebg();
    void afiseazaText();
    void adaugaButonIntoarcere(QWidget *parentWidget);
    void onIntoarceButtonPressed();

    void adaugaButonCumparare(QWidget *parentWidget);
    void onCumparaButtonPressed();

    void adaugaScrollArea();
    void adaugaFeedbackPostare();
    void adaugaButonFavorite(QWidget *parentWidget);
    void onAdaugaFavoriteButtonPressed();

    void adaugaButonTrimiteMesaj(QWidget* parentWidget);
    void onTrimiteMsjButtonPressed();
    void adaugaLabelIntroducereFeedback(QWidget *parentWidget);

    void adaugaRev();
    void onStergeUtilizatorPressed();
    void onBlocheazaPostarePressed();
    void onVizualizeazaPressed();
    void onDeblocheazaPostarePressed();

private:
    QLineEdit* lineMsg;
    std::string m_apelant;
    Ui::PostareWindow *ui;
    QVBoxLayout* pagina;
    QHBoxLayout* mainLayout;
    QVBoxLayout* proprietateLayout;

    Proprietate* m_proprietate;

    int postareCurenta;
    QLabel *numarPostareLabel;
    QPushButton *postareAnterioaraButton;
    QPushButton *postareUrmatoareButton;

    QLabel *imagineLabel;
    QVBoxLayout* feedbackLayout;
};

#endif // POSTAREWINDOW_H
