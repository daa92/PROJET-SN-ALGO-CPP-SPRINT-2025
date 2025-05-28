// librarymanager.cpp
#include "librarymanager.h"
#include <QDebug>
#include <QDate>
#include <QUuid> // Ensure QUuid is included for generating unique IDs

// Constructor: Initializes file paths and loads existing data
LibraryManager::LibraryManager(const QString& booksFile, const QString& usersFile, QObject *parent)
    : QObject(parent), m_booksFilePath(booksFile), m_usersFilePath(usersFile)
{
    loadBooks();
    loadUsers();
    qDebug() << "LibraryManager initialized. Books loaded:" << m_books.size() << ", Users loaded:" << m_users.size();
}

// Destructor: Saves all data when the manager is destroyed
LibraryManager::~LibraryManager()
{
    saveBooks();
    saveUsers();
    qDebug() << "LibraryManager destroyed. Data saved.";
}

// --- Private Data Persistence Methods ---

// Loads book data from file
void LibraryManager::loadBooks()
{
    QFile file(m_booksFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open books file for reading:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    m_books.clear();
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.isEmpty()) {
            m_books.append(Book::fromString(line));
        }
    }
    file.close();
    qDebug() << "Books loaded from" << m_booksFilePath << ":" << m_books.size();
}

// Saves book data to file
void LibraryManager::saveBooks()
{
    QFile file(m_booksFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Could not open books file for writing:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    for (const Book& book : m_books) {
        out << book.toString() << "\n";
    }
    file.close();
    qDebug() << "Books saved to" << m_booksFilePath << ":" << m_books.size();
}

// Loads user data from file
void LibraryManager::loadUsers()
{
    QFile file(m_usersFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open users file for reading:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    m_users.clear();
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.isEmpty()) {
            m_users.append(User::fromString(line));
        }
    }
    file.close();
    qDebug() << "Users loaded from" << m_usersFilePath << ":" << m_users.size();
}

// Saves user data to file
void LibraryManager::saveUsers()
{
    QFile file(m_usersFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Could not open users file for writing:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    for (const User& user : m_users) {
        out << user.toString() << "\n";
    }
    file.close();
    qDebug() << "Users saved to" << m_usersFilePath << ":" << m_users.size();
}

// --- Public Book Management Methods ---

// Adds one or more copies of a book
bool LibraryManager::addBook(const Book& bookTemplate, int numberOfCopies)
{
    if (numberOfCopies <= 0) {
        qDebug() << "Cannot add 0 or negative copies.";
        return false;
    }

    for (int i = 0; i < numberOfCopies; ++i) {
        // Create a new Book object for each copy, letting its constructor generate a unique bookId
        Book newCopy(bookTemplate.title, bookTemplate.author, bookTemplate.isbn);
        m_books.append(newCopy);
        qDebug() << "Added copy of book: Title:" << newCopy.title << "ISBN:" << newCopy.isbn << "Book ID:" << newCopy.bookId;
    }
    saveBooks();
    qDebug() << numberOfCopies << "copies of book '" << bookTemplate.title << "' (ISBN:" << bookTemplate.isbn << ") added successfully.";
    return true;
}

// Removes a specific physical book copy by its unique bookId
bool LibraryManager::removeBook(const QString& bookId)
{
    for (int i = 0; i < m_books.size(); ++i) {
        if (m_books.at(i).bookId == bookId) {
            if (m_books.at(i).isBorrowed || m_books.at(i).isReserved) {
                qDebug() << "Cannot remove book ID" << bookId << ": it is currently borrowed or reserved.";
                return false;
            }
            m_books.removeAt(i);
            saveBooks();
            qDebug() << "Book ID" << bookId << "removed successfully.";
            return true;
        }
    }
    qDebug() << "Book ID" << bookId << "not found for removal.";
    return false;
}

// Borrows a specific physical book copy by its unique bookId
bool LibraryManager::borrowBook(const QString& bookId, const QString& userId, const QDate& borrowDate)
{
    for (Book& book : m_books) {
        if (book.bookId == bookId) {
            if (!book.isBorrowed && !book.isReserved) {
                book.isBorrowed = true;
                book.borrowedByUserId = userId;
                book.borrowDate = borrowDate;
                book.returnDueDate = borrowDate.addDays(MAX_BORROW_DAYS); // Set due date
                saveBooks();
                qDebug() << "Book ID" << bookId << "borrowed by" << userId << "on" << book.borrowDate.toString("yyyy-MM-dd")
                         << ", due by" << book.returnDueDate.toString("yyyy-MM-dd");
                return true;
            } else {
                qDebug() << "Book ID" << bookId << "is not available to borrow (Borrowed:" << book.isBorrowed << ", Reserved:" << book.isReserved << ")";
                return false;
            }
        }
    }
    qDebug() << "Book ID" << bookId << "not found for borrowing.";
    return false;
}

// Returns a specific physical book copy by its unique bookId, calculating penalty
QPair<bool, double> LibraryManager::returnBook(const QString& bookId)
{
    QDate currentDate = QDate::currentDate(); // Get current date for penalty calculation
    for (Book& book : m_books) {
        if (book.bookId == bookId) {
            if (book.isBorrowed) {
                double penalty = 0.0;
                if (currentDate > book.returnDueDate) {
                    int overdueDays = book.returnDueDate.daysTo(currentDate);
                    penalty = overdueDays * PENALTY_PER_DAY;
                    qDebug() << "Book ID" << bookId << "is overdue by" << overdueDays << "days. Penalty:" << penalty << "FCFA.";
                }

                book.isBorrowed = false;
                book.borrowedByUserId = ""; // Clear borrower ID
                book.borrowDate = QDate(); // Clear borrow date (invalid date)
                book.returnDueDate = QDate(); // Clear due date (invalid date)
                saveBooks();
                qDebug() << "Book ID" << bookId << "returned.";
                return {true, penalty}; // Return success and calculated penalty
            } else {
                qDebug() << "Book ID" << bookId << "was not borrowed.";
                return {false, 0.0}; // Not borrowed, no penalty
            }
        }
    }
    qDebug() << "Book ID" << bookId << "not found for returning.";
    return {false, 0.0}; // Book not found
}

// Reserves a specific physical book copy by its unique bookId
bool LibraryManager::reserveBook(const QString& bookId, const QString& userId)
{
    for (Book& book : m_books) {
        if (book.bookId == bookId) {
            if (!book.isBorrowed && !book.isReserved) {
                book.isReserved = true;
                book.reservedByUserId = userId;
                saveBooks();
                qDebug() << "Book ID" << bookId << "reserved by" << userId;
                return true;
            } else {
                qDebug() << "Book ID" << bookId << "is not available to reserve (Borrowed:" << book.isBorrowed << ", Reserved:" << book.isReserved << ")";
                return false;
            }
        }
    }
    qDebug() << "Book ID" << bookId << "not found for reserving.";
    return false;
}

// Cancels a reservation for a specific physical book copy by its unique bookId
bool LibraryManager::cancelReservation(const QString& bookId)
{
    for (Book& book : m_books) {
        if (book.bookId == bookId) {
            if (book.isReserved) {
                book.isReserved = false;
                book.reservedByUserId = "";
                saveBooks();
                qDebug() << "Reservation for Book ID" << bookId << "cancelled.";
                return true;
            } else {
                qDebug() << "Book ID" << bookId << "was not reserved.";
                return false;
            }
        }
    }
    qDebug() << "Book ID" << bookId << "not found for cancelling reservation.";
    return false;
}

// Gets all books (all physical copies)
QVector<Book> LibraryManager::getAllBooks() const
{
    return m_books;
}

// --- Public User Management Methods ---

// Adds a user
bool LibraryManager::addUser(const User& user)
{
    for (const User& existingUser : m_users) {
        if (existingUser.name == user.name &&
            existingUser.phoneNumber == user.phoneNumber &&
            existingUser.gmailAddress == user.gmailAddress)
        {
            qDebug() << "User with details already exists.";
            return false;
        }
    }
    m_users.append(user);
    saveUsers();
    qDebug() << "User added:" << user.name;
    return true;
}

// Finds a user by details
User* LibraryManager::findUserByDetails(const QString& name, const QString& phone, const QString& gmail)
{
    for (User& user : m_users) {
        if (user.name == name &&
            user.phoneNumber == phone &&
            user.gmailAddress == gmail) {
            qDebug() << "User found:" << user.name;
            return &user;
        }
    }
    qDebug() << "User not found with details Name:" << name << "Phone:" << phone << "Gmail:" << gmail;
    return nullptr;
}

// Gets all users
QVector<User> LibraryManager::getAllUsers() const
{
    return m_users;
}

// --- Email Notification Logic (Simulated) ---

// Simulates sending an update email to all registered users.
void LibraryManager::sendUpdateEmails(const QString& subject, const QString& body)
{
    qDebug() << "Attempting to send update emails to all registered users...";
    if (m_users.isEmpty()) {
        qDebug() << "No registered users to send emails to.";
        return;
    }

    for (const User& user : m_users) {
        if (!user.gmailAddress.isEmpty()) {
            sendEmail(user.gmailAddress, subject, body);
        } else {
            qDebug() << "Skipping user" << user.name << "due to missing Gmail address.";
        }
    }
    qDebug() << "Finished simulating sending update emails.";
}

// Simulates sending a single email.
void LibraryManager::sendEmail(const QString& recipientEmail, const QString& subject, const QString& body)
{
    // --- IMPORTANT: This is a SIMULATED email sending function. ---
    qDebug() << "--- SIMULATED EMAIL SENT ---";
    qDebug() << "To: " << recipientEmail;
    qDebug() << "Subject: " << subject;
    qDebug() << "Body: " << body;
    qDebug() << "---------------------------";
}
