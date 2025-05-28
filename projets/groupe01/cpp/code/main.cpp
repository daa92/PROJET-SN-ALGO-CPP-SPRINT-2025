// main.cpp
#include "mainwindow.h"
#include <QApplication>

/**
 * @brief La fonction main est le point d'entrée de l'application E-Library.
 * @param argc Le nombre d'arguments en ligne de commande.
 * @param argv Un tableau d'arguments en ligne de commande.
 * @return Le code de sortie de l'application.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Crée une instance de QApplication
    MainWindow w;               // Crée une instance de votre MainWindow
    w.show();                   // Affiche la fenêtre principale
    return a.exec();            // Démarre la boucle d'événements de Qt
}
