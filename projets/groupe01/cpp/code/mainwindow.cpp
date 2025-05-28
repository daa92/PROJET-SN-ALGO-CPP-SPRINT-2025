// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidget>
#include <QDate>
#include <QDebug>

// Constructor for the MainWindow class.
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_libraryManager("books.txt", "users.txt")
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("C:/Users/Lenovo/OneDrive/Documenten/ELibraryApp/Blackvariant-Button-Ui-System-Folders-Drives-Library.ico"));

    // --- Initial UI Setup for Welcome Tab ---
    ui->librarianLoginGroup->setVisible(false);
    ui->studentTeacherLoginGroup->setVisible(false);

    connect(ui->librarianRadioButton, &QRadioButton::toggled, this, &MainWindow::on_librarianRadioButton_toggled);
    connect(ui->studentTeacherRadioButton, &QRadioButton::toggled, this, &MainWindow::on_studentTeacherRadioButton_toggled);
    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::on_loginButton_clicked);

    // --- Initial UI Setup for Librarian Tab ---
    connect(ui->addBookButton, &QPushButton::clicked, this, &MainWindow::on_addBookButton_clicked);
    connect(ui->removeBookButton, &QPushButton::clicked, this, &MainWindow::on_removeBookButton_clicked);
    connect(ui->sendUpdatesButton, &QPushButton::clicked, this, &MainWindow::on_sendUpdatesButton_clicked);


    // Set up the table widget for displaying books in the Librarian tab.
    // Added 'Book ID' column (was ISBN before, now ISBN is separate from unique ID)
    ui->librarianBooksTable->setColumnCount(10);
    ui->librarianBooksTable->setHorizontalHeaderLabels({"Title", "Author", "ISBN", "Book ID", "Borrowed", "Borrowed By", "Borrow Date", "Due Date", "Reserved", "Reserved By"});
    ui->librarianBooksTable->horizontalHeader()->setStretchLastSection(true);
    ui->librarianBooksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->librarianBooksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // --- Initial UI Setup for Student/Teacher Tab ---
    connect(ui->borrowBookButton, &QPushButton::clicked, this, &MainWindow::on_borrowBookButton_clicked);
    connect(ui->returnBookButton, &QPushButton::clicked, this, &MainWindow::on_returnBookButton_clicked);
    connect(ui->reserveBookButton, &QPushButton::clicked, this, &MainWindow::on_reserveBookButton_clicked);
    connect(ui->cancelReservationButton, &QPushButton::clicked, this, &MainWindow::on_cancelReservationButton_clicked);

    // Set up the table widget for displaying books in the Student/Teacher tab.
    // Added 'Book ID' column
    ui->studentTeacherBooksTable->setColumnCount(7);
    ui->studentTeacherBooksTable->setHorizontalHeaderLabels({"Title", "Author", "ISBN", "Book ID", "Status", "Borrow Date", "Due Date"});
    ui->studentTeacherBooksTable->horizontalHeader()->setStretchLastSection(true);
    ui->studentTeacherBooksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->studentTeacherBooksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
}

// Destructor for MainWindow.
MainWindow::~MainWindow()
{
    delete ui;
}

// --- Welcome Tab Slots ---

void MainWindow::on_librarianRadioButton_toggled(bool checked)
{
    ui->librarianLoginGroup->setVisible(checked);
    ui->studentTeacherLoginGroup->setVisible(false);
}

void MainWindow::on_studentTeacherRadioButton_toggled(bool checked)
{
    ui->studentTeacherLoginGroup->setVisible(checked);
    ui->librarianLoginGroup->setVisible(false);
}

void MainWindow::on_loginButton_clicked()
{
    if (ui->librarianRadioButton->isChecked()) {
        QString password = ui->librarianPasswordLineEdit->text();
        if (password == LIBRARIAN_PASSWORD) {
            showMessage("Login Success", "Welcome, Librarian!");
            ui->tabWidget->setTabEnabled(1, true);
            ui->tabWidget->setCurrentIndex(1);
            updateBooksTableLibrarian();
        } else {
            showMessage("Login Failed", "Incorrect password for Librarian.");
            ui->librarianPasswordLineEdit->clear();
        }
    } else if (ui->studentTeacherRadioButton->isChecked()) {
        QString name = ui->studentTeacherNameLineEdit->text().trimmed();
        QString phone = ui->studentTeacherPhoneLineEdit->text().trimmed();
        QString gmail = ui->studentTeacherGmailLineEdit->text().trimmed();

        if (name.isEmpty() || phone.isEmpty() || gmail.isEmpty()) {
            showMessage("Login Failed", "Please fill in all student/teacher information.");
            return;
        }

        User* user = m_libraryManager.findUserByDetails(name, phone, gmail);
        if (user) {
            m_currentUserId = user->id;
            m_currentUserName = user->name;
            showMessage("Login Success", "Welcome back, " + m_currentUserName + "!");
        } else {
            User newUser(name, phone, gmail);
            if (m_libraryManager.addUser(newUser)) {
                m_currentUserId = newUser.id;
                m_currentUserName = newUser.name;
                showMessage("Registration Success", "Welcome, " + m_currentUserName + "! Your account has been created.");
            } else {
                showMessage("Registration Failed", "Could not create new user account.");
                return;
            }
        }

        ui->studentTeacherNameDisplayLabel->setText("Name: " + m_currentUserName);
        ui->studentTeacherPhoneDisplayLabel->setText("Phone: " + phone);
        ui->studentTeacherGmailDisplayLabel->setText("Gmail: " + gmail);
        ui->studentTeacherIdDisplayLabel->setText("User ID: " + m_currentUserId);

        ui->tabWidget->setTabEnabled(2, true);
        ui->tabWidget->setCurrentIndex(2);
        updateBooksTableStudentTeacher();
    } else {
        showMessage("Login Required", "Please select if you are a Librarian or a Student/Teacher.");
    }
}

// --- Librarian Tab Slots ---

void MainWindow::on_addBookButton_clicked()
{
    QString title = ui->addBookTitleLineEdit->text().trimmed();
    QString author = ui->addBookAuthorLineEdit->text().trimmed();
    QString isbn = ui->addBookISBNLineEdit->text().trimmed();
    int numberOfCopies = ui->addBookCopiesSpinBox->value(); // Get number of copies

    if (title.isEmpty() || author.isEmpty() || isbn.isEmpty()) {
        showMessage("Input Error", "Please fill in all fields (Title, Author, ISBN) to add a book.");
        return;
    }
    if (numberOfCopies <= 0) {
        showMessage("Input Error", "Number of copies must be at least 1.");
        return;
    }

    // Create a dummy Book object to pass title, author, ISBN
    Book bookTemplate(title, author, isbn);
    if (m_libraryManager.addBook(bookTemplate, numberOfCopies)) {
        showMessage("Success", QString::number(numberOfCopies) + " copies of '" + title + "' added successfully!");
        ui->addBookTitleLineEdit->clear();
        ui->addBookAuthorLineEdit->clear();
        ui->addBookISBNLineEdit->clear();
        ui->addBookCopiesSpinBox->setValue(1); // Reset spinbox
        updateBooksTableLibrarian();
    } else {
        showMessage("Error", "Failed to add books. Check debug console for details.");
    }
}

void MainWindow::on_removeBookButton_clicked()
{
    // Now removing by Book ID, not ISBN
    QString bookId = ui->removeBookIdLineEdit->text().trimmed();

    if (bookId.isEmpty()) {
        showMessage("Input Error", "Please enter the Book ID of the specific copy to remove.");
        return;
    }

    if (m_libraryManager.removeBook(bookId)) {
        showMessage("Success", "Book copy with ID '" + bookId + "' removed successfully!");
        ui->removeBookIdLineEdit->clear();
        updateBooksTableLibrarian();
    } else {
        showMessage("Error", "Failed to remove book copy. Book ID '" + bookId + "' not found or currently borrowed/reserved.");
    }
}

void MainWindow::on_sendUpdatesButton_clicked()
{
    QString subject = "E-Library App Update!";
    QString body = "Dear User,\n\nThis is an important update from your E-Library App.\n\n"
                   "We have new books available and some exciting changes!\n\n"
                   "Best regards,\nYour Library Team";

    m_libraryManager.sendUpdateEmails(subject, body);
    showMessage("Updates Sent", "Simulated sending update emails to all registered users. Check debug console for details.");
}

// --- Student/Teacher Tab Slots ---

void MainWindow::on_borrowBookButton_clicked()
{
    // Now borrowing by Book ID, not ISBN
    QString bookId = ui->borrowBookIdLineEdit->text().trimmed();
    if (bookId.isEmpty()) {
        showMessage("Input Error", "Please enter the Book ID of the specific copy to borrow.");
        return;
    }

    if (m_currentUserId.isEmpty()) {
        showMessage("Error", "No user logged in. Please log in first.");
        return;
    }

    if (m_libraryManager.borrowBook(bookId, m_currentUserId, QDate::currentDate())) {
        showMessage("Success", "Book copy with ID '" + bookId + "' borrowed successfully!");
        ui->borrowBookIdLineEdit->clear();
        updateBooksTableStudentTeacher();
        updateBooksTableLibrarian();
    } else {
        showMessage("Error", "Failed to borrow book copy. It might be unavailable or reserved by another user, or Book ID not found.");
    }
}

void MainWindow::on_returnBookButton_clicked()
{
    // Now returning by Book ID, not ISBN
    QString bookId = ui->returnBookIdLineEdit->text().trimmed();
    if (bookId.isEmpty()) {
        showMessage("Input Error", "Please enter the Book ID of the specific copy to return.");
        return;
    }

    QPair<bool, double> result = m_libraryManager.returnBook(bookId);

    if (result.first) {
        QString message = "Book copy with ID '" + bookId + "' returned successfully!";
        if (result.second > 0.0) {
            message += QString("\n\nPenalty incurred: %1 FCFA.").arg(result.second);
            showMessage("Book Returned - With Penalty", message);
        } else {
            showMessage("Book Returned", message);
        }
        ui->returnBookIdLineEdit->clear();
        updateBooksTableStudentTeacher();
        updateBooksTableLibrarian();
    } else {
        showMessage("Error", "Failed to return book copy. Book ID '" + bookId + "' not found or not borrowed.");
    }
}

void MainWindow::on_reserveBookButton_clicked()
{
    // Now reserving by Book ID, not ISBN
    QString bookId = ui->reserveBookIdLineEdit->text().trimmed();
    if (bookId.isEmpty()) {
        showMessage("Input Error", "Please enter the Book ID of the specific copy to reserve.");
        return;
    }

    if (m_currentUserId.isEmpty()) {
        showMessage("Error", "No user logged in. Please log in first.");
        return;
    }

    if (m_libraryManager.reserveBook(bookId, m_currentUserId)) {
        showMessage("Success", "Book copy with ID '" + bookId + "' reserved successfully!");
        ui->reserveBookIdLineEdit->clear();
        updateBooksTableStudentTeacher();
        updateBooksTableLibrarian();
    } else {
        showMessage("Error", "Failed to reserve book copy. It might be unavailable or already reserved, or Book ID not found.");
    }
}

void MainWindow::on_cancelReservationButton_clicked()
{
    // Now cancelling reservation by Book ID, not ISBN
    QString bookId = ui->cancelReservationIdLineEdit->text().trimmed();
    if (bookId.isEmpty()) {
        showMessage("Input Error", "Please enter the Book ID of the specific copy to cancel reservation for.");
        return;
    }

    if (m_libraryManager.cancelReservation(bookId)) {
        showMessage("Success", "Reservation for book copy with ID '" + bookId + "' cancelled successfully!");
        ui->cancelReservationIdLineEdit->clear();
        updateBooksTableStudentTeacher();
        updateBooksTableLibrarian();
    } else {
        showMessage("Error", "Failed to cancel reservation for book copy. Book ID '" + bookId + "' not found or not reserved.");
    }
}

// --- Helper Functions ---

// Refreshes the book display table in the Librarian tab.
void MainWindow::updateBooksTableLibrarian()
{
    ui->librarianBooksTable->setRowCount(0);
    QVector<Book> allBooks = m_libraryManager.getAllBooks(); // all physical copies
    QDate currentDate = QDate::currentDate();

    ui->librarianBooksTable->setRowCount(allBooks.size());

    for (int i = 0; i < allBooks.size(); ++i) {
        const Book& book = allBooks.at(i);
        ui->librarianBooksTable->setItem(i, 0, new QTableWidgetItem(book.title));
        ui->librarianBooksTable->setItem(i, 1, new QTableWidgetItem(book.author));
        ui->librarianBooksTable->setItem(i, 2, new QTableWidgetItem(book.isbn)); // ISBN
        ui->librarianBooksTable->setItem(i, 3, new QTableWidgetItem(book.bookId)); // Book ID
        ui->librarianBooksTable->setItem(i, 4, new QTableWidgetItem(book.isBorrowed ? "Yes" : "No"));
        ui->librarianBooksTable->setItem(i, 5, new QTableWidgetItem(book.isBorrowed ? book.borrowedByUserId : ""));

        QString borrowDateText = book.borrowDate.isValid() ? book.borrowDate.toString("yyyy-MM-dd") : "";
        ui->librarianBooksTable->setItem(i, 6, new QTableWidgetItem(borrowDateText));

        QString dueDateText = "";
        if (book.returnDueDate.isValid()) {
            dueDateText = book.returnDueDate.toString("yyyy-MM-dd");
            if (book.isBorrowed && currentDate > book.returnDueDate) {
                int overdueDays = book.returnDueDate.daysTo(currentDate);
                double penalty = overdueDays * m_libraryManager.PENALTY_PER_DAY;
                dueDateText += QString(" (OVERDUE: %1 FCFA)").arg(penalty);
            }
        }
        ui->librarianBooksTable->setItem(i, 7, new QTableWidgetItem(dueDateText));


        ui->librarianBooksTable->setItem(i, 8, new QTableWidgetItem(book.isReserved ? "Yes" : "No"));
        ui->librarianBooksTable->setItem(i, 9, new QTableWidgetItem(book.isReserved ? book.reservedByUserId : ""));
    }
}

// Refreshes the book display table in the Student/Teacher tab.
void MainWindow::updateBooksTableStudentTeacher()
{
    ui->studentTeacherBooksTable->setRowCount(0);
    QVector<Book> allBooks = m_libraryManager.getAllBooks(); // all physical copies
    QDate currentDate = QDate::currentDate();

    ui->studentTeacherBooksTable->setRowCount(allBooks.size());

    for (int i = 0; i < allBooks.size(); ++i) {
        const Book& book = allBooks.at(i);
        ui->studentTeacherBooksTable->setItem(i, 0, new QTableWidgetItem(book.title));
        ui->studentTeacherBooksTable->setItem(i, 1, new QTableWidgetItem(book.author));
        ui->studentTeacherBooksTable->setItem(i, 2, new QTableWidgetItem(book.isbn)); // ISBN
        ui->studentTeacherBooksTable->setItem(i, 3, new QTableWidgetItem(book.bookId)); // Book ID

        QString status;
        if (book.isBorrowed) {
            status = "Borrowed";
            if (currentDate > book.returnDueDate) {
                int overdueDays = book.returnDueDate.daysTo(currentDate);
                double penalty = overdueDays * m_libraryManager.PENALTY_PER_DAY;
                status += QString(" (OVERDUE: %1 FCFA)").arg(penalty);
            }
        } else if (book.isReserved) {
            status = "Reserved";
        } else {
            status = "Available";
        }
        ui->studentTeacherBooksTable->setItem(i, 4, new QTableWidgetItem(status));

        QString borrowDateText = book.borrowDate.isValid() ? book.borrowDate.toString("yyyy-MM-dd") : "";
        ui->studentTeacherBooksTable->setItem(i, 5, new QTableWidgetItem(borrowDateText));

        QString dueDateText = book.returnDueDate.isValid() ? book.returnDueDate.toString("yyyy-MM-dd") : "";
        ui->studentTeacherBooksTable->setItem(i, 6, new QTableWidgetItem(dueDateText));
    }
}

// Custom message box function.
void MainWindow::showMessage(const QString& title, const QString& message) {
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
