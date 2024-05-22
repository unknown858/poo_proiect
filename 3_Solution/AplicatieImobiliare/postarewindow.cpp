#include "postarewindow.h"
#include "favoritewindow.h"
#include "mesageriewindow.h"
#include "principalwindow.h"
#include "profilwindow.h"
#include "qlineedit.h"
#include "ui_postarewindow.h"
#include "cumparawindow.h"
#include "client.h"
#include "profilwindow.h"

#include <string>
#include <QVBoxLayout>
#include <QMessageBox>

PostareWindow::PostareWindow(Proprietate* proprietate, QWidget *parent, std::string apelant) :
    QMainWindow(parent),
    m_apelant(apelant),
    ui(new Ui::PostareWindow), m_proprietate(proprietate)
{
    ui->setupUi(this);
    showFullScreen();

    adaugaWhitebg();
    adaugaImagini();
    afiseazaText();
    adaugaFeedbackPostare();
    adaugaScrollArea();
}

void PostareWindow::adaugaScrollArea()
{
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget *scrollWidget = new QWidget(scrollArea);

    scrollWidget->setLayout(pagina);
    scrollArea->setWidget(scrollWidget);
    pagina->setContentsMargins(40,40,40,40);
    setCentralWidget(scrollArea);

    adaugaButonCumparare(scrollWidget);
    adaugaButonFavorite(scrollWidget);
    adaugaButonTrimiteMesaj(scrollWidget);
    adaugaLabelIntroducereFeedback(scrollWidget);
    adaugaButonIntoarcere(scrollWidget);
}

void PostareWindow::onVizualizeazaPressed()
{
    ProfilWindow * pw = new ProfilWindow(nullptr, m_proprietate->getNumeVanzator().c_str());
    pw->show();

    this->close();
}

void PostareWindow::onBlocheazaPostarePressed()
{
    Client::getInstance().block_post(std::to_string(m_proprietate->getIdPostare()).c_str());
    if(m_proprietate->getStatusPostare() == true)
    {
        QMessageBox::information(this, "Informare", "Postarea a fost blocată!");
        PostareWindow *postw = new PostareWindow(m_proprietate, this, m_apelant);
        postw->show();
    }
    else
    {
        QMessageBox::critical(this, "Atenție!", "Postarea nu a putut fi blocată!");
    }
}

void PostareWindow::onStergeUtilizatorPressed()
{
    Client::getInstance().delete_user(m_proprietate->getNumeVanzator());

    if(Client::getInstance().getStergereOfertant() == 0)
    {
        QMessageBox::information(this, "Informare", "Ofertantul a fost șters!");
        PrincipalWindow *pw = new PrincipalWindow(nullptr, Client::getInstance().getUsername().c_str());
        pw->show();
    }
    else
    {
        QMessageBox::critical(this, "Atenție!", "Oferantul nu a putut fi șters!");
    }
}

void PostareWindow::adaugaButonFavorite(QWidget *parentWidget)
{
    if(!Client::getInstance().isAdmin())
    {
        QPushButton *button = new QPushButton  (parentWidget);
        button->setIcon(QIcon("C:/imagini/buton_favs.jpg"));
        QSize size(140,60);
        button->setIconSize(size);

        button->setGeometry(1090,580,60,40);
        button->show();
        connect(button, &QPushButton::clicked, this, &PostareWindow::onAdaugaFavoriteButtonPressed);
    }
}

void PostareWindow::onAdaugaFavoriteButtonPressed()
{
    if(Client::getInstance().estePostareInFavorite(m_proprietate))
    {
        Client::getInstance().add_to_remove_from_favorite(std::to_string(m_proprietate->getIdPostare()).c_str(), Client::getInstance().getUsername(), (char *) "1");
        QMessageBox::information(this, "Succes", "Postare a fost eliminată din secțiunea favorite!");
        Client::getInstance().clearVectorFav();
        Client::getInstance().request_feed_fav(Client::getInstance().getUsername());
    }
    else
    {
        Client::getInstance().add_to_remove_from_favorite(std::to_string(m_proprietate->getIdPostare()).c_str(), Client::getInstance().getUsername(), (char *) "0");
        QMessageBox::information(this, "Succes", "Postare adaugată la secțiunea favorite!");
        Client::getInstance().clearVectorFav();
        Client::getInstance().request_feed_fav(Client::getInstance().getUsername());
    }
}

void PostareWindow::adaugaButonTrimiteMesaj(QWidget* parentWidget)
{
    if(!Client::getInstance().isAdmin())
    {
        QPushButton *button = new QPushButton ("Trimite mesaj", parentWidget);

        button->setGeometry(1065,720,120,40);

        QFont font("Gabriola", 15, QFont::Normal);
        button->setFont(font);

        button->show();
        connect(button, &QPushButton::clicked, this, &PostareWindow::onTrimiteMsjButtonPressed);
    }
}

void PostareWindow::onTrimiteMsjButtonPressed()
{
    Client::getInstance().cleanSenderMsg();
    Client::getInstance().request_chat(Client::getInstance().getUsername());
    Client::getInstance().request_specific_chat(Client::getInstance().getUsername(), this->m_proprietate->getNumeVanzator());


    MesagerieWindow *mw =new MesagerieWindow(nullptr, m_proprietate->getNumeVanzator(), "postare");
    mw->show();

    this->close();
}

void PostareWindow::adaugaButonCumparare(QWidget *parentWidget)
{
    if(!Client::getInstance().isAdmin())
    {
        QPushButton *button = new QPushButton ("Cumpără!", parentWidget);

        button->setGeometry(1065,650,120,40);

        QFont font("Gabriola", 15, QFont::Normal);
        button->setFont(font);

        button->show();
        connect(button, &QPushButton::clicked, this, &PostareWindow::onCumparaButtonPressed);
    }
}

void PostareWindow::adaugaLabelIntroducereFeedback(QWidget *parentWidget)
{
    if(!Client::getInstance().isAdmin())
    {
        QHBoxLayout*labelLineEditLayout = new QHBoxLayout();
        QLabel *labelUsername = new QLabel(QString::fromStdString(Client::getInstance().getUsername()) + ":", parentWidget);
        QFont font("Gabriola", 15, QFont::Normal);
        labelUsername->setFont(font);
        labelLineEditLayout->addWidget(labelUsername);
        lineMsg = new QLineEdit(parentWidget);

        labelLineEditLayout->addWidget(lineMsg);
        QPushButton *button = new QPushButton ("Postează!", parentWidget);
        button->setFont(font);
        button->setFixedSize(120,40);
        labelLineEditLayout->addWidget(button);
        labelLineEditLayout->setSpacing(10);

        labelLineEditLayout->setContentsMargins(0,0,35,0);
        pagina->addLayout(labelLineEditLayout);

        connect(button, &QPushButton::clicked, this, &PostareWindow::adaugaRev);
    }
}

void PostareWindow::adaugaRev()
{
    Client::getInstance().add_review(std::to_string(this->m_proprietate->getIdPostare()).c_str(), Client::getInstance().getUsername().c_str(), lineMsg->text().toStdString().c_str());
    QMessageBox::information(this, "Informare", "Feedback adăugat cu succes!");
    lineMsg->clear();
    property_type type;
    if(m_proprietate->getTip() == TipProprietate::casa)
    {
        type = property_type::house;
    }
    else if (m_proprietate->getTip() == TipProprietate::apartament)
    {
        type = property_type::apartment;
    }
    else
    {
        type = property_type::land;
    }

    Client::getInstance().clearFeedback();
    Client::getInstance().request_ad_details(std::to_string(m_proprietate->getIdPostare()).c_str(),type);

    Client::getInstance().clearVectorFav();
    Client::getInstance().request_feed_fav(Client::getInstance().getUsername());

    PostareWindow *postw = new PostareWindow(m_proprietate, this, m_apelant);

    postw->show();
    this->close();
}

void PostareWindow::onCumparaButtonPressed()
{
    CumparaWindow *cw = new CumparaWindow(nullptr, m_proprietate);
    cw->show();

    this->close();
}

void PostareWindow::adaugaButonIntoarcere(QWidget *parentWidget)
{
    QPushButton *button = new QPushButton ("Întoarce-te", parentWidget);
    button->setFixedSize(120,40);

    QVBoxLayout *scrollLayout = qobject_cast<QVBoxLayout*>(parentWidget->layout());
    if (scrollLayout)
        scrollLayout->addWidget(button, 0, Qt::AlignLeft | Qt::AlignBottom); // aliniere la stanga jos


    QFont font("Gabriola", 15, QFont::Normal);
    button->setFont(font);

    button->show();
    connect(button, &QPushButton::clicked, this, &PostareWindow::onIntoarceButtonPressed);
}

void PostareWindow::onIntoarceButtonPressed()
{
    if(m_apelant=="favorite")
    {
        FavoriteWindow *fw = new FavoriteWindow();
        fw->show();

        this->close();
    }
    else if (m_apelant=="profil") {
        ProfilWindow *profw = new ProfilWindow();
        profw->show();

        this->close();
    }
    else
    {
        PrincipalWindow *pw = new PrincipalWindow(nullptr, Client::getInstance().getUsername().c_str());
        pw->show();

        this->close();
    }
}

void PostareWindow::adaugaImagini()
{
    postareCurenta = 0;
    pagina = new QVBoxLayout(this);
    mainLayout = new QHBoxLayout(this);
    centralWidget()->setLayout(pagina);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonLayout);

    postareAnterioaraButton = new QPushButton(this);
    postareAnterioaraButton->setIcon(QIcon("C:/imagini/left_arrow.png"));
    postareAnterioaraButton->setIconSize(QSize(35, 35));
    postareAnterioaraButton->setFixedSize(35, 35);
    postareAnterioaraButton->setFlat(true);
    postareAnterioaraButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mainLayout->addWidget(postareAnterioaraButton);


    imagineLabel = new QLabel(this);
    mainLayout->addWidget(imagineLabel);
    imagineLabel->setScaledContents(true);

    postareUrmatoareButton = new QPushButton(this);
    postareUrmatoareButton->setIcon(QIcon("C:/imagini/right_arrow.png"));
    postareUrmatoareButton->setIconSize(QSize(35, 35));
    postareUrmatoareButton->setFixedSize(35, 35);
    postareUrmatoareButton->setFlat(true);
    postareUrmatoareButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mainLayout->addWidget(postareUrmatoareButton);

    connect(postareAnterioaraButton, &QPushButton::clicked, this, &PostareWindow::postareAnterioara);
    connect(postareUrmatoareButton, &QPushButton::clicked, this, &PostareWindow::postareUrmatoare);

    updateImagini();
    mainLayout->setSpacing(40);
    pagina->addLayout(mainLayout);
}

void PostareWindow::updateImagini()
{
    if (m_proprietate && m_proprietate->getImagini().size() > 0)
    {
        QPixmap pic(m_proprietate->getImagini()[postareCurenta].c_str());

        pic = pic.scaled(800, 500, Qt::KeepAspectRatio);
        imagineLabel->setPixmap(pic);
        imagineLabel->setFixedSize(800, 500);
        imagineLabel->setScaledContents(true);
    }
}

void PostareWindow::postareAnterioara()
{
    if (postareCurenta > 0)
    {
        postareCurenta--;
        updateImagini();
    }
}

void PostareWindow::postareUrmatoare()
{
    if (m_proprietate && postareCurenta < m_proprietate->getImagini().size() - 1)
    {
        postareCurenta++;
        updateImagini();
    }
}

PostareWindow::~PostareWindow()
{
    delete ui;
}

void PostareWindow::adaugaWhitebg()
{
    QLabel *white_bg=new QLabel(this);
    white_bg->setStyleSheet("background-color: white;");
    white_bg->setGeometry(0, 0, width(), height());
    white_bg->lower();
    white_bg->show();
}

void PostareWindow::afiseazaText()
{
    QFont font("Gabriola", 15, QFont::Normal);

    QWidget* widget = new QWidget(this);

    widget->setMinimumSize(200, 50);

    proprietateLayout = new QVBoxLayout();

    QLabel *tipProprLabel = new QLabel("Proprietate de " + QString::fromStdString(m_proprietate->getTipAnunt()), this);
    tipProprLabel->setFont(font);
    proprietateLayout->addWidget(tipProprLabel);

    QWidget *spacer = new QWidget(this);
    spacer->setFixedSize(20, 20);
    proprietateLayout->addWidget(spacer);

    QLabel *descriereLabel = new QLabel ("Descriere proprietate:    ", this);
    descriereLabel->setFont(font);
    proprietateLayout->addWidget(descriereLabel);

    std::string descriere = m_proprietate->getDescriere();

    int maxCaracterePerLabel = 1200;
    int startIndex = 0;

    while (startIndex < descriere.length()) {
        int endIndex = startIndex + maxCaracterePerLabel;

        while (endIndex < descriere.length() && !isspace(descriere[endIndex])) {
            endIndex--;
        }
        if (endIndex >= descriere.length()) {
            endIndex = descriere.length() - 1;
        }

        std::string subDescriere = descriere.substr(startIndex, endIndex - startIndex + 1);

        QLabel *label = new QLabel(QString::fromStdString(subDescriere), this);
        label->setFont(font);
        label->setMaximumWidth(950);
        label->setWordWrap(true);
        proprietateLayout->addWidget(label);
        startIndex = endIndex + 1;
    }


    proprietateLayout->addWidget(spacer);
    QLabel *locLabel = new QLabel("Locație: " + QString::fromStdString(m_proprietate->getLocatie()), this);
    locLabel->setFont(font);
    proprietateLayout->addWidget(locLabel);


    proprietateLayout->addWidget(spacer);

    QLabel *detaliiLabel = new QLabel("Detalii: ", this);
    detaliiLabel->setFont(font);
    detaliiLabel->setFixedWidth(150);
    proprietateLayout->addWidget(detaliiLabel);

    QLabel *pretLabel = new QLabel("    Preț: " + QString::number(m_proprietate->getPret()) + " € ", this);
    pretLabel->setFont(font);
    proprietateLayout->addWidget(pretLabel);

    QLabel *suprafataLabel = new QLabel("    Suprafață: " + QString::number(m_proprietate->getSuprafata()) + " m2 ", this);
    suprafataLabel->setFont(font);
    proprietateLayout->addWidget(suprafataLabel);


    if(this->m_proprietate->getTip() != TipProprietate::teren )
    {
        QLabel *nrCamLabel = new QLabel("    Număr de camere: " + QString::number(m_proprietate->getNrCamere()), this);
        nrCamLabel->setFont(font);
        proprietateLayout->addWidget(nrCamLabel);
    }

    if(this->m_proprietate->getStatusPostare() == true)
    {
        QLabel *postareBlocata = new QLabel("Postarea este blocată", this);
        postareBlocata->setFont(font);
        proprietateLayout->addWidget(postareBlocata);
    }
    else
    {
        QLabel *postareBlocata = new QLabel("Postarea nu este blocată", this);
        postareBlocata->setFont(font);
        proprietateLayout->addWidget(postareBlocata);
    }

    proprietateLayout->addWidget(spacer);
    QLabel *vanzatorLabel = new QLabel("Postat de: " + QString::fromStdString(m_proprietate->getNumeVanzator()), this);
    vanzatorLabel->setFont(font);
    proprietateLayout->addWidget(vanzatorLabel);

    QLabel *dataLabel = new QLabel("Data postării: " + QString::fromStdString(m_proprietate->getData()), this);
    dataLabel->setFont(font);
    proprietateLayout->addWidget(dataLabel);

    proprietateLayout->setSpacing(20);

    QPushButton *profilUrilizator = new QPushButton ("Vizualizează profilul ofertantului!");
    profilUrilizator->setFont(font);
    profilUrilizator->setFixedSize(300,40);
    proprietateLayout->addWidget(profilUrilizator);

    connect(profilUrilizator, &QPushButton::clicked, this, &PostareWindow::onVizualizeazaPressed);


    if (Client::getInstance().isAdmin())
    {
        QFont font("Gabriola", 15, QFont::Normal);

        QPushButton *stergeUtilizator = new QPushButton ("Șterge utilizatorul!");
        stergeUtilizator->setFont(font);
        stergeUtilizator->setFixedSize(180,40);
        proprietateLayout->addWidget(stergeUtilizator);
        connect(stergeUtilizator, &QPushButton::clicked, this, &PostareWindow::onStergeUtilizatorPressed);

        QPushButton *blocheazaPostare = new QPushButton ("Blochează postarea!");
        blocheazaPostare->setFont(font);
        blocheazaPostare->setFixedSize(180,40);
        proprietateLayout->addWidget(blocheazaPostare);
        connect(blocheazaPostare, &QPushButton::clicked, this, &PostareWindow::onBlocheazaPostarePressed);

        QPushButton *deblocheazaPostare = new QPushButton ("Deblochează postarea!");
        deblocheazaPostare->setFont(font);
        deblocheazaPostare->setFixedSize(180,40);
        proprietateLayout->addWidget(deblocheazaPostare);
        connect(deblocheazaPostare, &QPushButton::clicked, this, &PostareWindow::onDeblocheazaPostarePressed);
    }

    proprietateLayout->setSpacing(10);
    proprietateLayout->setContentsMargins(50,0,0,0);
    pagina->addLayout(proprietateLayout);
}

void PostareWindow::onDeblocheazaPostarePressed()
{
    Client::getInstance().unblock_post(std::to_string(m_proprietate->getIdPostare()).c_str());
    if(m_proprietate->getStatusPostare() == true)
    {
        QMessageBox::information(this, "Informare", "Postarea a fost deblocată!");
        PostareWindow *postw = new PostareWindow(m_proprietate, this, m_apelant);
        postw->show();
    }
    else
    {
        QMessageBox::critical(this, "Atenție!", "Postarea nu a putut fi deblocată!");
    }
}

void PostareWindow::adaugaFeedbackPostare()
{
    QFont font("Gabriola", 15, QFont::Normal);
    QWidget* widget = new QWidget(this);

    widget->setMinimumSize(200, 50);

    feedbackLayout = new QVBoxLayout();
    QLabel *feedpLabel = new QLabel("Feedback postare:", this);
    feedpLabel->setFont(font);
    feedbackLayout->addWidget(feedpLabel);
    feedbackLayout->setSpacing(10);

    auto feed = Client::getInstance().getAllFeedback();

    for(auto feedb : feed)
    {
        if(strcmp(feedb->getIdPostare().c_str(), std::to_string(m_proprietate->getIdPostare()).c_str()) == 0)
        {
            QLabel *userLabel = new QLabel(QString::fromStdString(feedb->getUserGiver()) + ":   " + QString::fromStdString(feedb->getFeedback()), this);
            userLabel->setFont(font);
            feedbackLayout->addWidget(userLabel);
        }
    }

    feedbackLayout->setSpacing(10);
    pagina->addLayout(feedbackLayout);
    pagina->setSpacing(50);
}
