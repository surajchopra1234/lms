#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>

using namespace std;

// ==================== Borrower Class ====================>

class Borrower {
    string name;
    string mobile;
    string email;
    time_t borrow_date;
    time_t return_date;

public:
    Borrower(string name, string mobile, string email, time_t borrow_date = time(nullptr),
             time_t return_date = time(nullptr) + 15 * 24 * 60 * 60) {
        this->name = name;
        this->mobile = mobile;
        this->email = email;
        this->borrow_date = borrow_date;
        this->return_date = return_date;
    }

    // Getters
    string getName() const { return name; }
    string getMobile() const { return mobile; }
    string getEmail() const { return email; }
    bool isBookOverdue() const { return time(nullptr) > return_date; }

    string getBorrowDateStr() const {
        tm tm_borrow_date = *localtime(&borrow_date);
        ostringstream oss;
        oss << put_time(&tm_borrow_date, "%d-%m-%Y");
        return oss.str();
    }

    string getReturnDateStr() const {
        tm tm_return_date = *localtime(&return_date);
        ostringstream oss;
        oss << put_time(&tm_return_date, "%d-%m-%Y");
        return oss.str();
    }

    // Converts borrower data to string format
    string toString() const {
        return name + "|" + mobile + "|" + email + "|" + to_string(borrow_date) + "|" + to_string(return_date);
    }

    // Creates a Borrower object from string data
    static Borrower fromString(string str) {
        string name, contact, email, borrow_date_str, return_date_str;

        istringstream ss(str);
        getline(ss, name, '|');
        getline(ss, contact, '|');
        getline(ss, email, '|');
        getline(ss, borrow_date_str, '|');
        getline(ss, return_date_str);

        return Borrower(name, contact, email, stol(borrow_date_str), stol(return_date_str));
    }

    // Compare two borrowers
    bool compare(const Borrower &borrower) const {
        return name == borrower.name && mobile == borrower.mobile && email == borrower.email;
    }
};

// ==================== Book Class ====================>

class Book {
    string title;
    string author;
    string isbn;
    int inventory_count;
    vector<Borrower> borrowers;

public:
    // Constructor
    Book(string title, string author, string isbn, int inventory_count, vector<Borrower> borrowers = {}) {
        this->title = title;
        this->author = author;
        this->isbn = isbn;
        this->inventory_count = inventory_count;
        this->borrowers = borrowers;
    }

    // Getters
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getISBN() const { return isbn; }
    int getInventoryCount() const { return inventory_count; }
    vector<Borrower> getBorrowers() const { return borrowers; }

    // Converts book data to string format
    string toString() const {
        string borrowersStr;
        for (const auto &borrower: borrowers) {
            borrowersStr += borrower.toString() + ";";
        }

        return title + "," + author + "," + isbn + "," + to_string(inventory_count) + "," + borrowersStr;
    }

    // Creates a Book object from string data
    static Book fromString(string str) {
        string title, author, isbn, inventory_count_str, borrowersStr;
        vector<Borrower> borrowers;

        istringstream ss(str);
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, isbn, ',');
        getline(ss, inventory_count_str, ',');
        getline(ss, borrowersStr);

        istringstream ssBorrowers(borrowersStr);
        string borrowerStr;
        while (getline(ssBorrowers, borrowerStr, ';')) {
            borrowers.push_back(Borrower::fromString(borrowerStr));
        }

        return Book(title, author, isbn, stoi(inventory_count_str), borrowers);
    }

    // Borrow a book
    bool borrowBook(const Borrower &borrower) {
        if (inventory_count - borrowers.size() > 0) {
            borrowers.push_back(borrower);
            return true;
        }

        return false;
    }

    // Return a book
    bool returnBook(const Borrower &borrower) {
        for (auto it = borrowers.begin(); it != borrowers.end(); ++it) {
            if (it->compare(borrower)) {
                borrowers.erase(it);
                return true;
            }
        }

        return false;
    }

    // Display book details
    void displayBookDetails() const {
        cout << left
                << setw(20) << title
                << setw(20) << author
                << setw(15) << isbn
                << setw(15) << inventory_count
                << setw(15) << inventory_count - borrowers.size()
                << setw(15) << (inventory_count - borrowers.size() > 0 ? "Available" : "Not Available")
                << endl;
    }

    // Display borrowers details
    void displayBorrowersDetails() const {
        for (const auto &borrower: borrowers) {
            cout << left
                    << setw(15) << borrower.getName()
                    << setw(15) << borrower.getMobile()
                    << setw(35) << borrower.getEmail()
                    << setw(15) << (borrower.isBookOverdue() ? "Overdue" : "Not Overdue")
                    << setw(15) << borrower.getBorrowDateStr()
                    << setw(15) << borrower.getReturnDateStr()
                    << endl;
        }
    }
};

// ==================== Library Class ====================>

class Library {
    vector<Book> books;
    string filename = "library_books.csv";

    // Save books to file
    void saveBooksToFile() const {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error: Unable to open file for writing." << endl;
            return;
        }

        for (const auto &book: books) {
            outFile << book.toString() << endl;
        }

        outFile.close();
    }

    // Load books from file
    void loadBooksFromFile() {
        ifstream inFile(filename);
        if (!inFile) { return; }

        books.clear();
        string line;
        while (getline(inFile, line)) {
            books.push_back(Book::fromString(line));
        }

        inFile.close();
    }

    // Input book ISBN
    string inputISBN(string message) {
        string isbn;
        cout << endl << message;
        getline(cin, isbn);
        return isbn;
    }

public:
    // Constructor
    Library() {
        // Load saved books from file
        loadBooksFromFile();
    }

    // Add a book to library
    void addBook() {
        string title, author, isbn;
        int inventory_count;

        cout << endl << "Enter Book Title:";
        getline(cin, title);

        cout << "Enter Author Name:";
        getline(cin, author);

        cout << "Enter ISBN:";
        getline(cin, isbn);

        cout << "Enter Inventory Count:";
        cin >> inventory_count;
        cin.ignore(); // Clear the input buffer

        // Check if book with same ISBN already exists
        for (const auto &book: books) {
            if (book.getISBN() == isbn) {
                cout << "Book with ISBN " << isbn << " already exists in the library." << endl;
                return;
            }
        }

        books.push_back(Book(title, author, isbn, inventory_count));
        saveBooksToFile();

        cout << endl << "Book '" << title << "' added successfully!" << endl;
    }

    // Delete a book from library
    void deleteBook() {
        string isbn = inputISBN("Enter ISBN of the book to delete:");

        for (auto book = books.begin(); book != books.end(); ++book) {
            if (book->getISBN() == isbn) {
                if (book->getBorrowers().size() > 0) {
                    cout << "Book with ISBN " << isbn << " has been borrowed and cannot be deleted." << endl;
                    return;
                }
                books.erase(book);
                saveBooksToFile();
                cout << "Book with ISBN " << isbn << " deleted successfully." << endl;
                return;
            }
        }

        cout << "Book with ISBN " << isbn << " not found in the library." << endl;
    }

    // Display all books in library
    void displayBooks() {
        if (books.empty()) {
            cout << endl << "No books in the library." << endl;
            return;
        }

        cout << endl << "Library Book Catalog:" << endl << endl;
        cout << left
                << setw(20) << "Title"
                << setw(20) << "Author"
                << setw(15) << "ISBN"
                << setw(15) << "Inventory"
                << setw(15) << "Available"
                << setw(15) << "Status"
                << endl;
        cout << string(100, '-') << endl;

        for (const auto &book: books) {
            book.displayBookDetails();
        }
    }

    // Display total books in library
    void displayTotalBooksCount() {
        cout << endl << "Total books in library: " << books.size() << endl;
    }

    // Borrow a book
    void borrowBook() {
        string isbn = inputISBN("Enter ISBN of the book to borrow:");

        for (auto &book: books) {
            if (book.getISBN() == isbn) {
                string name, mobile, email;

                cout << endl << "Enter Borrower Details:" << endl;

                cout << "Enter Name:";
                getline(cin, name);

                cout << "Enter Mobile:";
                getline(cin, mobile);

                cout << "Enter Email:";
                getline(cin, email);

                Borrower borrower(name, mobile, email);

                if (book.borrowBook(borrower)) {
                    saveBooksToFile();
                    cout << "Book borrowed successfully.";
                    return;
                }
            }
        }

        cout << "Something went wrong. Please try again.";
    }

    // Return a book
    void returnBook() {
        string isbn = inputISBN("Enter ISBN of the book to return:");

        for (auto &book: books) {
            if (book.getISBN() == isbn) {
                string name, mobile, email;

                cout << endl << "Enter Returner Details:" << endl;

                cout << "Enter Name:";
                getline(cin, name);

                cout << "Enter Mobile:";
                getline(cin, mobile);

                cout << "Enter Email:";
                getline(cin, email);

                Borrower borrower(name, mobile, email);

                if (book.returnBook(borrower)) {
                    saveBooksToFile();
                    cout << "Book returned successfully.";
                    return;
                }
            }
        }

        cout << "Something went wrong. Please try again.";
    }

    // Display borrowers of a book
    void displayBookBorrowers() {
        string isbn = inputISBN("Enter ISBN of the book to display borrowers:");

        for (const auto &book: books) {
            if (book.getISBN() == isbn) {
                if (book.getBorrowers().size() == 0) {
                    cout << "No borrowers found for the book with ISBN " << isbn << "." << endl;
                    return;
                }

                cout << endl << "Borrowers of the book with ISBN " << isbn << ":" << endl << endl;
                cout << left
                        << setw(15) << "Name"
                        << setw(15) << "Mobile"
                        << setw(35) << "Email"
                        << setw(15) << "Status"
                        << setw(15) << "Borrow Date"
                        << setw(15) << "Return Date"
                        << endl;
                cout << string(110, '-') << endl;
                book.displayBorrowersDetails();

                return;
            }
        }

        cout << "Book with ISBN " << isbn << " not found in the library." << endl;
    }
};

// ==================== Library Management System ====================>

class LibraryManagementSystem {
    Library library;

    void displayMenu() {
        cout << endl << "*************** Library Management System ***************" << endl << endl;
        cout << "1. Add a Book" << endl;
        cout << "2. Delete a Book" << endl;
        cout << "3. View All Books" << endl;
        cout << "4. Total Books Count in Library" << endl;
        cout << "5. Borrow a Book" << endl;
        cout << "6. Return a Book" << endl;
        cout << "7. View Book Borrowers" << endl;
        cout << "0. Exit" << endl << endl;
    }

public:
    void run() {
        while (true) {
            // Display menu
            displayMenu();

            int choice;
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore(); // Clear the input buffer

            switch (choice) {
                case 1:
                    library.addBook();
                    break;
                case 2:
                    library.deleteBook();
                    break;
                case 3:
                    library.displayBooks();
                    break;
                case 4:
                    library.displayTotalBooksCount();
                    break;
                case 5:
                    library.borrowBook();
                    break;
                case 6:
                    library.returnBook();
                    break;
                case 7:
                    library.displayBookBorrowers();
                    break;
                case 0:
                    cout << endl << "Exiting the Library Management System. Goodbye!" << endl;
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }
};

// ==================== Main Function ====================>

int main() {
    LibraryManagementSystem lms;
    lms.run();

    return 0;
}
