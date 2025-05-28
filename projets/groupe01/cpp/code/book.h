// book.h
#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QDate>
#include <QUuid> // Nécessaire pour générer des identifiants uniques

/**
 * @brief La structure Book représente une seule copie physique de livre dans la bibliothèque.
 * Chaque copie a un bookId unique.
 */
struct Book
{
    QString bookId;               ///< Identifiant unique pour cette copie physique spécifique (UUID)
    QString title;                ///< Titre du livre
    QString author;               ///< Auteur du livre
    QString isbn;                 ///< L'ISBN identifie l'édition/le titre (peut être le même pour plusieurs copies)
    bool isBorrowed;              ///< Indique si le livre est emprunté
    QString borrowedByUserId;     ///< ID de l'utilisateur qui a emprunté le livre
    QDate borrowDate;             ///< Date d'emprunt
    QDate returnDueDate;          ///< Date de retour prévue
    bool isReserved;              ///< Indique si le livre est réservé
    QString reservedByUserId;     ///< ID de l'utilisateur qui a réservé le livre

    // Constructeur pour initialiser un nouveau livre
    Book(const QString& title, const QString& author, const QString& isbn)
        : title(title), author(author), isbn(isbn),
        isBorrowed(false), borrowedByUserId(""),
        isReserved(false), reservedByUserId("")
    {
        // Générer un ID unique pour chaque copie de livre lors de sa création
        bookId = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }

    // Constructeur par défaut pour la lecture depuis un fichier
    Book()
        : bookId(""), title(""), author(""), isbn(""),
        isBorrowed(false), borrowedByUserId(""),
        isReserved(false), reservedByUserId("") {}


    /**
     * @brief Convertit un objet Book en chaîne de caractères pour la sauvegarde dans un fichier.
     * Format: "bookId|title|author|isbn|isBorrowed|borrowedByUserId|borrowDate|returnDueDate|isReserved|reservedByUserId"
     * @return Représentation QString du livre.
     */
    QString toString() const {
        return bookId + "|" +
               title + "|" +
               author + "|" +
               isbn + "|" +
               QString::number(isBorrowed) + "|" +
               borrowedByUserId + "|" +
               (borrowDate.isValid() ? borrowDate.toString("yyyy-MM-dd") : "") + "|" +
               (returnDueDate.isValid() ? returnDueDate.toString("yyyy-MM-dd") : "") + "|" +
               QString::number(isReserved) + "|" +
               reservedByUserId;
    }

    /**
     * @brief Crée un objet Book à partir d'une chaîne de caractères lue depuis un fichier.
     * @param data La représentation en chaîne de caractères du livre.
     * @return Un objet Book.
     */
    static Book fromString(const QString& data) {
        Book book;
        QStringList parts = data.split('|');
        if (parts.size() == 10) { // S'assurer du bon nombre de parties
            book.bookId = parts.at(0);
            book.title = parts.at(1);
            book.author = parts.at(2);
            book.isbn = parts.at(3);
            book.isBorrowed = parts.at(4).toInt();
            book.borrowedByUserId = parts.at(5);
            book.borrowDate = QDate::fromString(parts.at(6), "yyyy-MM-dd");
            book.returnDueDate = QDate::fromString(parts.at(7), "yyyy-MM-dd");
            book.isReserved = parts.at(8).toInt();
            book.reservedByUserId = parts.at(9);
        } else {
            qWarning() << "Chaîne de données de livre invalide :" << data;
        }
        return book;
    }
};

#endif // BOOK_H
