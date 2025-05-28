// librarymanager.h
#ifndef LIBRARYMANAGER_H
#define LIBRARYMANAGER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QPair>
#include "book.h" // S'assurer que book.h est inclus
#include "user.h"

/**
 * @brief La classe LibraryManager gère toute la logique principale du système de bibliothèque.
 * Cela inclut la gestion des livres et des utilisateurs, et la persistance des données dans des fichiers.
 */
class LibraryManager : public QObject
{
    Q_OBJECT

public:
    // Constantes pour les règles d'emprunt
    const int MAX_BORROW_DAYS = 14;     ///< Période d'emprunt maximale en jours (2 semaines)
    const double PENALTY_PER_DAY = 100.0; ///< Montant de la pénalité par jour de retard (en FCFA)

    explicit LibraryManager(const QString& booksFile, const QString& usersFile, QObject *parent = nullptr);
    ~LibraryManager();

    // --- Méthodes de gestion des livres ---

    /**
     * @brief Ajoute une ou plusieurs nouvelles copies d'un livre à la bibliothèque.
     * Chaque copie recevra un bookId unique.
     * @param bookTemplate Un objet Book contenant le titre, l'auteur et l'ISBN des nouvelles copies.
     * @param numberOfCopies Le nombre de copies à ajouter.
     * @return True si les livres ont été ajoutés avec succès, false sinon.
     */
    bool addBook(const Book& bookTemplate, int numberOfCopies);

    /**
     * @brief Supprime une copie physique spécifique d'un livre par son bookId unique.
     * @param bookId L'identifiant unique de la copie du livre à supprimer.
     * @return True si la copie du livre a été trouvée et supprimée, false sinon.
     */
    bool removeBook(const QString& bookId);

    /**
     * @brief Permet à un utilisateur d'emprunter une copie physique spécifique d'un livre.
     * @param bookId L'identifiant unique de la copie du livre à emprunter.
     * @param userId L'identifiant de l'utilisateur qui emprunte le livre.
     * @param borrowDate La date à laquelle le livre est emprunté (généralement QDate::currentDate()).
     * @return True si la copie du livre a été empruntée avec succès, false sinon.
     */
    bool borrowBook(const QString& bookId, const QString& userId, const QDate& borrowDate);

    /**
     * @brief Permet de retourner une copie physique spécifique d'un livre, en calculant toute pénalité de retard.
     * @param bookId L'identifiant unique de la copie du livre à retourner.
     * @return Un QPair où le premier élément est true si le retour a réussi,
     * et le second élément est le montant de la pénalité calculée (0.0 si pas de retard).
     * Retourne {false, 0.0} si la copie du livre n'a pas été trouvée ou n'était pas empruntée.
     */
    QPair<bool, double> returnBook(const QString& bookId);

    /**
     * @brief Permet à un utilisateur de réserver une copie physique spécifique d'un livre.
     * @param bookId L'identifiant unique de la copie du livre à réserver.
     * @param userId L'identifiant de l'utilisateur qui réserve le livre.
     * @return True si la copie du livre a été réservée avec succès, false sinon.
     */
    bool reserveBook(const QString& bookId, const QString& userId);

    /**
     * @brief Permet à un utilisateur d'annuler une réservation pour une copie physique spécifique d'un livre.
     * @param bookId L'identifiant unique de la copie du livre pour laquelle annuler la réservation.
     * @return True si la réservation a été trouvée et annulée, false sinon.
     */
    bool cancelReservation(const QString& bookId);

    /**
     * @brief Récupère un vecteur de toutes les copies physiques de livres actuellement dans la bibliothèque.
     * @return Un QVector contenant tous les objets Book (copies physiques).
     */
    QVector<Book> getAllBooks() const;

    // --- Méthodes de gestion des utilisateurs ---

    bool addUser(const User& user);
    User* findUserByDetails(const QString& name, const QString& phone, const QString& gmail);
    QVector<User> getAllUsers() const;

    // --- Méthode de notification par email ---
    void sendUpdateEmails(const QString& subject, const QString& body);

private:
    QString m_booksFilePath;
    QString m_usersFilePath;
    QVector<Book> m_books; // m_books stocke maintenant des copies physiques individuelles
    QVector<User> m_users;

    void loadBooks();
    void saveBooks();
    void loadUsers();
    void saveUsers();

    void sendEmail(const QString& recipientEmail, const QString& subject, const QString& body);
};

#endif // LIBRARYMANAGER_H
