// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h" // En-tête de l'interface utilisateur générée
#include <QMessageBox>     // Optionnel : pour afficher les messages d'erreur

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) // Appelle le constructeur de la classe de base
    , ui(new Ui::MainWindow) // Initialise l'objet UI
{
    ui->setupUi(this); // Configure les éléments de l'interface utilisateur définis dans mainwindow.ui
    this->setStyleSheet("background-image: url(C:/Users/Lenovo/OneDrive/Documenten/NAVIGATEUR/360_F_1440411675_8EUXKOnS3deAKGXMbqLob9ZWzvZVzHHp.jpg); background-repeat no-repeat; background-position: center;");
    this->setWindowIcon(QIcon("C:/Users/Lenovo/OneDrive/Documenten/NAVIGATEUR/apps_web_browser_15742.ico"));
    this->showMaximized(); // Affiche la fenêtre maximisée

    // 1. Créer et ajouter le QWebEngineView
    webView = new QWebEngineView(this);
    // En supposant que ui->verticalLayout est la disposition principale de votre widget central
    // Ajoute la vue web à la disposition, de sorte qu'elle occupe la zone de contenu principale.
    ui->verticalLayout->addWidget(webView);
    this->setWindowTitle("ICTU SEARCH"); // Définit le titre initial de la fenêtre
    // Faire en sorte que la vue web s'étende pour remplir l'espace disponible
    webView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // Supprimer les marges et l'espacement pour la disposition contenant la vue web
    ui->verticalLayout->setContentsMargins(0, 0, 0, 0);
    ui->verticalLayout->setSpacing(0);

    // 2. Configuration initiale de la fenêtre (à partir de votre code de projet b2)
    // Vous voudrez peut-être supprimer ces chemins codés en dur et utiliser les ressources Qt à la place pour le déploiement
    // this->setStyleSheet("background-image: url(C:/Users/Lenovo/Desktop/b2/360_F_1440411675_8EUXKOnS3deAKGXMbqLob9ZWzvZVzHHp.jpg); background-repeat no-repeat; background-position: center;");

    // this->setWindowIcon(QIcon("C:/Users/Lenovo/Desktop/b2/apps_web_browser_15742.ico"));

    // Initialiser currentString à une URL vide
    currentString = "";

    // 3. Connecter les signaux de l'interface utilisateur aux slots
    // Connecter la touche Entrée dans lineEdit au slot du bouton "Go"
    connect(ui->inputLineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_setButton_clicked);
    // Connecter le bouton Retour à son slot
    connect(ui->prevButton, &QPushButton::clicked, this, &MainWindow::on_prevButton_clicked);
    // Connecter le bouton Avance à son slot
    connect(ui->nextButton, &QPushButton::clicked, this, &MainWindow::on_nextButton_clicked);
    // Connecter le bouton Go à son slot (s'il n'est pas géré par lineEdit returnPressed)
    connect(ui->setButton, &QPushButton::clicked, this, &MainWindow::on_setButton_clicked);


    // 4. Connecter les signaux de QWebEngineView aux mises à jour de l'interface utilisateur
    // Lorsque l'URL de la vue web change, mettez à jour l'étiquette d'affichage de l'URL
    connect(webView, &QWebEngineView::urlChanged, this, [this](const QUrl &url){
        ui->displayLabel->setText(url.toString()); // Met à jour l'étiquette avec la nouvelle URL
        // Si l'URL change PAS à cause d'une action de l'utilisateur, mettez à jour currentString et l'historique
        // Cela gère les redirections ou les liens cliqués dans la vue web.
        if (url.toString() != currentString) {
            if (!currentString.isEmpty()) {
                backStack.push(currentString); // Ajoute l'URL actuelle à la pile "arrière"
            }
            currentString = url.toString(); // Définit la nouvelle URL comme l'URL actuelle
            forwardStack.clear(); // Toute nouvelle navigation efface l'historique "avant"
            updateButtonStates(); // Met à jour l'état des boutons
            saveStringToFile(currentString); // Sauvegarde la nouvelle URL dans le fichier
        }
    });

    // Lorsque le titre de la vue web change, mettez à jour le titre de la fenêtre
    connect(webView, &QWebEngineView::titleChanged, this, &QMainWindow::setWindowTitle);

    // 5. Définir les états initiaux des boutons
    updateButtonStates();

    // 6. Charger une page par défaut au démarrage (optionnel)
    onLoadUrl("https://www.google.com");
}

MainWindow::~MainWindow()
{
    delete ui; // Supprime l'objet UI (ce qui supprime également webView car c'est un enfant)
}

// Slot pour le bouton "Retour" (prevButton)
void MainWindow::on_prevButton_clicked()
{
    if (!backStack.isEmpty()) { // S'il y a des URL dans la pile "arrière"
        // Pousser l'URL actuelle dans forwardStack avant de revenir en arrière
        forwardStack.push(currentString);
        QString previousUrl = backStack.pop(); // Récupère l'URL précédente
        onLoadUrl(previousUrl); // Charge l'URL précédente
    }
}

// Slot pour le bouton "Avance" (nextButton)
void MainWindow::on_nextButton_clicked()
{
    if (!forwardStack.isEmpty()) { // S'il y a des URL dans la pile "avant"
        // Pousser l'URL actuelle dans backStack avant d'aller en avant
        backStack.push(currentString);
        QString nextUrl = forwardStack.pop(); // Récupère l'URL suivante
        onLoadUrl(nextUrl); // Charge l'URL suivante
    }
}

// Slot pour le bouton "Go/Définir l'URL" (setButton) ou Entrée dans lineEdit
void MainWindow::on_setButton_clicked()
{
    QString url = ui->inputLineEdit->text().trimmed(); // Récupère l'URL du champ de saisie

    if (!url.isEmpty()) { // Si l'URL n'est pas vide
        // S'il y a une URL actuelle, poussez-la dans backStack pour l'historique
        if (!currentString.isEmpty()) {
            backStack.push(currentString);
        }
        currentString = url; // Définir la nouvelle URL comme actuelle
        onLoadUrl(currentString); // Charger l'URL
        forwardStack.clear(); // Effacer l'historique "avant" car nous commençons un nouveau chemin
    }
    ui->inputLineEdit->clear(); // Effacer le champ de saisie
}

// Fonction d'aide pour charger une URL dans la vue web et mettre à jour l'historique
void MainWindow::onLoadUrl(const QString &url)
{
    QString finalUrl = url; // Copie l'URL pour la manipulation
    // Préfixer "https://" si l'URL ne commence pas par un schéma
    if (!finalUrl.contains("://") && !finalUrl.startsWith("file:///")) {
        finalUrl = "https://" + finalUrl;
    }

    webView->load(QUrl(finalUrl)); // Charge l'URL dans le QWebEngineView
    ui->displayLabel->setText(finalUrl);   // Met à jour l'étiquette avec l'URL

    // Nous ne poussons pas directement dans les piles d'historique ici.
    // Le signal webView->urlChanged gérera cela pour toutes les navigations
    // (saisie utilisateur, liens internes, redirections, retour/avant).

    saveStringToFile(finalUrl);     // Sauvegarde l'URL chargée dans le fichier
    updateButtonStates();           // Met à jour l'état des boutons
}


// Fonction d'aide pour sauvegarder une chaîne (URL) dans un fichier dans le répertoire de l'EXE
void MainWindow::saveStringToFile(const QString &str)
{
    QString appDirPath = QCoreApplication::applicationDirPath(); // Obtient le répertoire de l'application
    QString filePath = appDirPath + "/entered_strings.txt"; // Construit le chemin complet du fichier

    QFile file(filePath); // Crée un objet QFile
    if (file.open(QFile::Append | QFile::Text)) { // Ouvre le fichier en mode ajout de texte
        QTextStream out(&file); // Crée un flux de texte pour écrire
        out << str << "\n"; // Écrit la chaîne suivie d'un retour à la ligne
        file.close(); // Ferme le fichier
    } else {
        QMessageBox::warning(this, "Erreur de fichier", "Impossible d'ouvrir le fichier en écriture : " + file.errorString()); // Affiche un message d'erreur si l'ouverture échoue
    }
}

// Fonction d'aide pour mettre à jour l'état activé des boutons de navigation
void MainWindow::updateButtonStates()
{
    ui->prevButton->setEnabled(!backStack.isEmpty());   // Active le bouton "Retour" si la pile "arrière" n'est pas vide
    ui->nextButton->setEnabled(!forwardStack.isEmpty()); // Active le bouton "Avance" si la pile "avant" n'est pas vide
}
