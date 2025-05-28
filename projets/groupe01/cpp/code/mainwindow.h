// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "librarymanager.h" // S'assurer que librarymanager.h est inclus

namespace Ui {
class MainWindow;
}

/**
 * @brief La classe MainWindow gère l'interface utilisateur graphique principale de l'application E-Library.
 * Elle gère les interactions utilisateur, affiche les données et communique avec le LibraryManager.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // --- Slots de l'onglet Bienvenue ---
    void on_librarianRadioButton_toggled(bool checked);
    void on_studentTeacherRadioButton_toggled(bool checked);
    void on_loginButton_clicked();

    // --- Slots de l'onglet Bibliothécaire ---
    void on_addBookButton_clicked();
    void on_removeBookButton_clicked();
    void on_sendUpdatesButton_clicked();

    // --- Slots de l'onglet Étudiant/Enseignant ---
    void on_borrowBookButton_clicked();
    void on_returnBookButton_clicked();
    void on_reserveBookButton_clicked();
    void on_cancelReservationButton_clicked();


private:
    Ui::MainWindow *ui;
    LibraryManager m_libraryManager;
    QString m_currentUserId;
    QString m_currentUserName;

    const QString LIBRARIAN_PASSWORD = "admin123"; // Mot de passe du bibliothécaire

    void updateBooksTableLibrarian(); // Met à jour le tableau des livres pour le bibliothécaire
    void updateBooksTableStudentTeacher(); // Met à jour le tableau des livres pour l'étudiant/enseignant
    void showMessage(const QString& title, const QString& message); // Affiche un message à l'utilisateur
};

#endif // MAINWINDOW_H
