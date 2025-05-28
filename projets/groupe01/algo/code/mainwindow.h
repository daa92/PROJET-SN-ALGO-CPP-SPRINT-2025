// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>         // Nécessaire pour les piles d'historique (retour/avance)
#include <QWebEngineView> // Nécessaire pour la fonctionnalité de navigateur web
#include <QUrl>           // Nécessaire pour la manipulation des URL

// Nécessaire pour la sauvegarde de fichier
#include <QFile>          // Pour les opérations de fichier
#include <QTextStream>    // Pour écrire du texte dans un fichier
#include <QCoreApplication> // Pour obtenir le chemin du répertoire de l'application (applicationDirPath()) lors de la sauvegarde du fichier

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT // Macro requise pour les objets Qt qui utilisent des signaux et des slots

public:
    // Constructeur : initialise la fenêtre principale
    explicit MainWindow(QWidget *parent = nullptr);
    // Destructeur : nettoie les ressources
    ~MainWindow();

private slots:
    // Slots pour les boutons de l'interface utilisateur
    void on_prevButton_clicked();   // Bouton "Retour"
    void on_nextButton_clicked(); // Bouton "Avance"
    void on_setButton_clicked(); // Bouton "Aller/Définir l'URL"

    // Fonction d'aide pour charger une URL dans la vue web et gérer l'historique
    void onLoadUrl(const QString &url);

    // Fonction d'aide pour sauvegarder une chaîne dans un fichier
    void saveStringToFile(const QString &str);

private:
    Ui::MainWindow *ui; // Pointeur vers l'objet UI généré
    QStack<QString> backStack;    // Stocke les URL visitées avant l'actuelle
    QStack<QString> forwardStack; // Stocke les URL visitées après l'actuelle (si navigation arrière)
    QString currentString;        // L'URL actuellement affichée dans la vue web

    // Le widget de la vue web
    QWebEngineView *webView;

    // Fonction d'aide pour mettre à jour l'état activé des boutons de navigation
    void updateButtonStates();
};

#endif // MAINWINDOW_H
