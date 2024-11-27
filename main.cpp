#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// ==================== Book Class ====================>

class Book {
    string title;
    string author;
    string isbn;
    bool is_available;

public:
    // Constructor
    Book(string title, string author, string isbn) {
        this->title = title;
        this->author = author;
        this->isbn = isbn;
        this->is_available = true;
    }

    // Getters
    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    string getISBN() const {
        return isbn;
    }

    bool isAvailable() const {
        return is_available;
    }

    // Display book details
    void displayDetails() const {
        cout << left
                << setw(30) << title
                << setw(25) << author
                << setw(20) << isbn
                << setw(15) << (is_available ? "Available" : "Checked Out")
                << endl;
    }

    // Converts book data to CSV format: title,author,isbn
    string toCSVString() const {
        return title + "," + author + "," + isbn;
    }

    // Creates a Book object from CSV string
    static Book fromCSVString(const string &csvString) {
        istringstream ss(csvString);
        string title, author, isbn;

        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, isbn);

        return Book(title, author, isbn);
    }
};

// ==================== Library Class ====================>

class Library {
    vector<Book> books;
    string filename = "library_books.csv";

    // Save books to file
    void saveBooksToFile() {
        ofstream outFile(filename);

        if (!outFile) {
            cerr << "Error: Unable to open file for writing." << endl;
            return;
        }

        for (const auto &book: books) {
            outFile << book.toCSVString() << endl;
        }

        outFile.close();
    }

    // Load books from file
    void loadBooksFromFile() {
        ifstream inFile(filename);

        if (!inFile) {
            return;
        }

        books.clear();
        string line;
        while (getline(inFile, line)) {
            books.push_back(Book::fromCSVString(line));
        }

        inFile.close();
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

        cout << endl << "Enter Book Title:";
        getline(cin, title);

        cout << "Enter Author Name:";
        getline(cin, author);

        cout << "Enter ISBN:";
        getline(cin, isbn);

        // Check if book with same ISBN already exists
        for (const auto &book: books) {
            if (book.getISBN() == isbn) {
                cout << "Book with ISBN " << isbn << " already exists in the library." << endl;
                return;
            }
        }

        books.push_back(Book(title, author, isbn));
        saveBooksToFile();

        cout << endl << "Book '" << title << "' added successfully!" << endl;
    }

    // Delete a book from library
    void deleteBook() {
        string isbn;

        cout << endl << "Enter ISBN of the book to delete:";
        getline(cin, isbn);

        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->getISBN() == isbn) {
                books.erase(it);
                saveBooksToFile();
                cout << "Book with ISBN " << isbn << " deleted successfully." << endl;
                return;
            }
        }

        cout << "Book with ISBN " << isbn << " not found in the library." << endl;
    }

    // View all books in library
    void viewAllBooks() {
        if (books.empty()) {
            cout << endl << "No books in the library." << endl;
            return;
        }

        cout << endl << "Library Book Catalog:" << endl << endl;

        cout << left
                << setw(30) << "Title"
                << setw(25) << "Author"
                << setw(20) << "ISBN"
                << setw(15) << "Status"
                << endl;
        cout << string(90, '-') << endl;

        for (const auto &book: books) {
            book.displayDetails();
        }
    }

    // Get total number of books in library
    int getTotalBooks() {
        return books.size();
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
        cout << "4. Total Books in Library" << endl;
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
                    library.viewAllBooks();
                    break;
                case 4:
                    cout << endl << "Total books in library: " << library.getTotalBooks() << endl;
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
