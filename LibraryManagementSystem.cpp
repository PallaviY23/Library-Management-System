#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <limits>

using namespace std;

class Book {
public:
    string title;
    string author;
    string publisher;
    int year;
    string ISBN;
    string status;

    Book ( string title, string author, string publisher, int year, string ISBN, string status = "Available")
        : title(title), author(author), publisher(publisher), year(year), ISBN(ISBN), status(status) {
    }

    void displayBookInfo() {
        cout << "Title: " << title << ", Author: " << author << ", Publisher: " << publisher << ", Year: " << year << ", ISBN: " << ISBN << ", Status: " << status << endl;
    }
};

class Account {
public:
    vector<pair<Book, time_t>> borrowedBooks;
    int fine;
    
    Account() : fine(0) {
    }
    int getFine() { 
        return fine; 
    }
    void addFine(int amount) { 
        fine += amount; 
    }
    void clearFine() { 
        fine = 0; 
    }
    vector<pair<Book, time_t>>& getBorrowedBooks() { 
        return borrowedBooks; 
    }
};
    

class User {
public:
    string name, userID;
    Account account;

    User(string n, string id) : name(n), userID(id) {
    }
    
    virtual void borrowBook(Book* book, time_t borrowDate) = 0;
    virtual void returnBook(Book& book, time_t returnDate) = 0;

    string getUserID() {
        return userID;
    }
};

class Student : public User {
public:
    int maxBooks = 3;
    int maxDays = 15;
    int finePerDay = 10;
    int Fine = 0;

    int calculateFine() {
        int finex = 0;
        time_t now = time(0);
        for (const auto& b : account.getBorrowedBooks()) {
            int daysBorrowed = (now - b.second);
            if (daysBorrowed > maxDays) {
                finex += (daysBorrowed - maxDays) * finePerDay;
            }
        }
        account.addFine(finex);
        int x=account.getFine();
        finex = 0;
        return x;
    }

public:
    Student(string n, string id) : User(n, id) {
    }
    
    void borrowBook(Book *book, time_t borrowDate) override {
        if (Fine > 0) {
            cout << "Cannot borrow books until fine is paid." << endl;
            return;
        }
        if (account.borrowedBooks.size() >= maxBooks) {
            cout << "Cannot borrow more than " << maxBooks << " books." << endl;
            return;
        }
        if (book->status != "Available") {
            cout << "Book is not available." << endl;
            return;
        }
        account.borrowedBooks.push_back({*book, borrowDate});
        book->status = "Borrowed";
        cout << name << " borrowed " << book->title << endl;
    }
    
    void returnBook(Book& book, time_t returnDate) override {
        for (auto it = account.borrowedBooks.begin(); it != account.borrowedBooks.end(); ++it) {
            if (it->first.ISBN == book.ISBN) {
                time_t borrowTime = it->second;
                int daysBorrowed = (returnDate - borrowTime);
                if (daysBorrowed > maxDays) {
                    Fine += (daysBorrowed - maxDays) * finePerDay;
                    cout << "Overdue fine: " << (daysBorrowed - maxDays) * finePerDay << " rupees." << endl;
                }
                account.borrowedBooks.erase(it);
                book.status = "Available";
                cout << name << " returned " << book.title << endl;
                return;
            }
        }
        cout << "Book not found in borrowed list." << endl;
    }
    
    void veiwfine(){
        cout << calculateFine() << endl; 
    }

    void payFine() {
        cout << "Amount paid = " << calculateFine() << endl;
        cout << "Fine paid successfully!" << endl;
        account.clearFine();
    }
};

class Faculty : public User {
public:
    int maxBooks = 5;
    int maxDays = 30;
public:
    Faculty(string n, string id) : User(n, id) {
    }
    
    void borrowBook(Book* book, time_t borrowDate) override {
        int canBorrow = 1;
        for (auto it = account.borrowedBooks.begin(); it != account.borrowedBooks.end(); ++it){
            if (it->first.ISBN == book->ISBN){
                time_t borrowTime = it->second;
                time_t now = time(0);
                int daysBorrowed = (now - borrowTime) / (60 * 60 * 24);
                if (daysBorrowed > 60){
                    canBorrow = 0;
                }
            }
        }
        if (canBorrow == 1) {
            if (account.borrowedBooks.size() >= maxBooks) {
                cout << "Cannot borrow more than " << maxBooks << " books." << endl;
                return;
            }
            if (book->status != "Available") {
                cout << "Book is not available." << endl;
                return;
            }
            account.borrowedBooks.push_back({*book, borrowDate});
            book->status = "Borrowed";
            cout << name << " borrowed " << book->title << endl;
        }
        else {
            cout << "You are not allowed to Borrow a Book. Since you exceeded your overdue" << endl; 
        }
    }
    
    void returnBook(Book& book, time_t returnDate) override {
        for (auto it = account.borrowedBooks.begin(); it != account.borrowedBooks.end(); ++it) {
            if (it->first.ISBN == book.ISBN) {
                time_t borrowTime = it->second;
                int daysBorrowed = (returnDate - borrowTime) / (60 * 60 * 24); // Corrected here
                if (daysBorrowed > maxDays) {
                    cout << "Book returned late, but no fine for faculty." << endl;
                }
                account.borrowedBooks.erase(it);
                book.status = "Available";
                cout << name << " returned " << book.title << endl;
                return;
            }
        }
        cout << "Book not found in borrowed list." << endl;
    }
};

class Librarian : public User {
public:
    Librarian(string n, string id) : User(n, id) {
    }
    
    void addBook(vector<Book>* books, Book book) {
        books->push_back(book);
        cout << "Book added: " << book.title << endl;
    }
    
    void removeBook(vector<Book>& books, string isbn) {
        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->ISBN == isbn) {
                cout << "Book removed: " << it->title << endl;
                books.erase(it);
                return;
            }
        }
        cout << "Book not found." << endl;
    }

    void updateBook(vector<Book>& books, string isbn) {
        for (auto& book : books) {  // Iterate through the books
            if (book.ISBN == isbn) {
                cout << "Updating details for book: " << book.title << endl;
    
                cout << "Enter new title (leave empty to keep current): ";
                string newTitle;
                getline(cin, newTitle);
                if (!newTitle.empty()) {
                    book.title = newTitle;
                }
    
                cout << "Enter new author (leave empty to keep current): ";
                string newAuthor;
                getline(cin, newAuthor);
                if (!newAuthor.empty()) {
                    book.author = newAuthor;
                }
    
                cout << "Enter new publisher (leave empty to keep current): ";
                string newPublisher;
                getline(cin, newPublisher);
                if (!newPublisher.empty()) {
                    book.publisher = newPublisher;
                }
    
                cout << "Enter new year (0 to keep current): ";
                int newYear;
                cin >> newYear;
                cin.ignore();
                if (newYear > 0) {
                    book.year = newYear;
                }
    
                cout << "Enter new status (Available/Borrowed/Reserved): ";
                string newStatus;
                getline(cin, newStatus);
                if (!newStatus.empty()) {
                    book.status = newStatus;
                }
    
                cout << "Book information updated successfully!" << endl;
                return;
            }
        }
        cout << "Book not found." << endl;
    }
    
    void addUser(vector<User*>& users, User* user) {
        users.push_back(user);
        cout << "User added: " << user->getUserID() << endl;
    }
    
    void removeUser(vector<User*>& users, string userID) {
        for (auto it = users.begin(); it != users.end(); ++it) {
            if ((*it)->getUserID() == userID) {
                cout << "User removed: " << (*it)->getUserID() << endl;
                delete *it;
                users.erase(it);
                return;
            }
        }
        cout << "User not found." << endl;
    }
    
    void borrowBook(Book*, time_t) override {
    }
    void returnBook(Book&, time_t) override {
    }
};

class Library
{
public:
    vector<Book> books;
    vector<User *> users;

    Library() {}
    ~Library()
    {
        for (auto u : users)
            delete u;
    }

    void loadData()
    {
        ifstream infile("./data/books.txt");
        if (infile){
            string line;
            while (getline(infile, line)){
                if (line.empty())
                    continue;
                istringstream iss(line);
                vector<string> tokens;
                string token;
                while (getline(iss, token, ','))
                {
                    tokens.push_back(token);
                }
                if (tokens.size() != 6)
                    continue;
                int year = stoi(tokens[3]);
                books.push_back(Book(tokens[0], tokens[1], tokens[2], year, tokens[4], tokens[5]));
            }
            infile.close();
        }
        else{
            cout << "books.txt not found. Starting with empty book list.\n";
        }

    ifstream ufile("./data/users.txt");
    if (ufile){
        string line;
        while (getline(ufile, line))
        {
            if (line.empty())
                continue;
            istringstream iss(line);
            vector<string> tokens;
            string token;
            while (getline(iss, token, ','))
            {
                tokens.push_back(token);
            }
            if (tokens.size() != 3)
                continue;

            string name = tokens[0];
            string userID = tokens[1];
            string role = tokens[2];

            if (role == "Student")
                users.push_back(new Student(name, userID));
            else if (role == "Faculty")
                users.push_back(new Faculty(name, userID));
            else if (role == "Librarian")
                users.push_back(new Librarian(name, userID));
        }
        ufile.close();
    }
    else{
        cout << "users.txt not found. Starting with empty user list.\n";
    }
}


void saveData()
{
    ofstream outfile("./data/books.txt");
    for (auto &book : books)
    {
        outfile << book.title << "," << book.author << ","
                << book.publisher << "," << book.year << "," << book.ISBN << ","
                << book.status << "\n";
    }
    outfile.close();

    ofstream uoutfile("./data/users.txt");
    for (auto user : users)
    {
        string role;
        if (dynamic_cast<Student *>(user))
            role = "Student";
        else if (dynamic_cast<Faculty *>(user))
            role = "Faculty";
        else if (dynamic_cast<Librarian *>(user))
            role = "Librarian";
        
        uoutfile << user->name << "," << user->userID << "," << role << "\n";
    }
    uoutfile.close();
}


void displayAvailableBooks()
{
    cout << "\nAvailable Books:\n";
    for (auto &book : books)
    {
        if (book.status == "Available")
            book.displayBookInfo();
    }
}

Book *findBookByISBN(string isbn)
{
    for (auto &book : books)
    {
        if (book.ISBN == isbn)
            return &book;
    }
    return nullptr;
}
};

int main() {
    Library library;
    library.loadData();

    int choice;
    do {
        cout << "\n~ LIBRARY MANAGEMENT SYSTEM ~\n";
        cout << "1. Student\n";
        cout << "2. Faculty\n";
        cout << "3. Librarian\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            string userID;
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter userID: ";
            cin >> userID;
            int isStudent = 0;
            User *currentUser = nullptr;
            for (auto user : library.users) {
                if (user->name == name && user->userID == userID) {
                    currentUser = user;
                    isStudent = 1;
                    break;
                }
            }
            do {
                if (currentUser) {
                    cout << "Welcome " << currentUser->name << endl;
                    cout << "1. View Available Books\n";
                    cout << "2. Borrow a Book\n";
                    cout << "3. Return a Book\n";
                    cout << "4. View Borrowed Books\n";
                    cout << "5. Check Fine Amount\n";
                    cout << "6. Pay Fine\n";
                    cout << "7. Exit\n";
                    cout << "Enter your choice: ";
                    cin >> choice;

                    if (choice == 1) {
                        library.displayAvailableBooks();
                    } 
                    else if (choice == 2) {
                        string isbn;
                        cout << "Enter ISBN: ";
                        cin >> isbn;
                        Book *book = library.findBookByISBN(isbn);
                        Student *student = dynamic_cast<Student *>(currentUser);
                        if (book) {
                            if (book->status == "Available") {
                                if (student->account.borrowedBooks.size() < student->maxBooks) {
                                    student->borrowBook(book, time(0));
                                    library.saveData(); // Save changes to file
                                } else {
                                    cout << "Sorry!! you are not allowed to Borrow more than " << student->maxBooks << " books.\n";
                                }
                            } else {
                                cout << "Book is not available. It is currently borrowed.\n";
                            }
                        } else {
                            cout << "Book not found!\n";
                        }
                    }
                    
                    else if (choice == 3) {
                        string isbn;
                        cout << "Enter ISBN: ";
                        cin >> isbn;
                        Book *book = library.findBookByISBN(isbn);
                        if (book) {
                            Student *student = dynamic_cast<Student *>(currentUser);
                            if (student) {
                                student->returnBook(*book, time(0));
                                library.saveData();
                            }
                        } else {
                            cout << "Book not found!\n";
                        }
                    } 
                    else if (choice == 4) { // View Borrowed Books
                        cout << "Your Borrowed Books:\n";
                        for (auto &rec : currentUser->account.borrowedBooks) {
                            cout << "Book ISBN: " << rec.first.ISBN << " Borrowed on: " << ctime(&rec.second);
                        }
                    } 
                    else if (choice == 5) {
                        Student *student = dynamic_cast<Student *>(currentUser);
                        student->veiwfine();
                    } else if (choice == 6) {
                        Student *student = dynamic_cast<Student *>(currentUser);
                        student->payFine(); }
                    else if (choice == 7) {
                        break;
                    }
                    else {
                        cout << "Invalid choice...\n";
                    }
                } 
                if (isStudent == 0) {
                    cout << "Invalid credentials.\n";
                    break;
                }
            } while (true);
        }
        else if (choice == 2) {
            string name;
            string userID;
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter userID: ";
            cin >> userID;
            int isFaculty = 0;
            User *currentUser = nullptr;
            for (auto user : library.users) {
                if (user->name == name && user->userID == userID) {
                    currentUser = user;
                    isFaculty = 1;
                    break;
                }
            }
            do {
                if (currentUser) {
                    cout << "Welcome " << currentUser->name << endl;
                    cout << "1. View Available Books\n";
                    cout << "2. Borrow a Book\n";
                    cout << "3. Return a Book\n";
                    cout << "4. View Borrowed Books\n";
                    cout << "5. Exit\n";
                    cout << "Enter your choice: ";
                    cin >> choice;

                    if (choice == 1) {
                        library.displayAvailableBooks();
                    } 
                    else if (choice == 2) {
                        string isbn;
                        cout << "Enter ISBN: ";
                        cin >> isbn;
                        Book *book = library.findBookByISBN(isbn);
                        Faculty *faculty = dynamic_cast<Faculty *>(currentUser);
                        if (book) {
                            if (book->status == "Available") {
                                if (faculty->account.borrowedBooks.size() < faculty->maxBooks) {
                                    faculty->borrowBook(book, time(0));
                                    library.saveData(); // Save changes to file
                                } else {
                                    cout << "Cannot borrow more than " << faculty->maxBooks << " books.\n";
                                }
                            } else {
                                cout << "Book is not available. It is currently borrowed.\n";
                            }
                        } else {
                            cout << "Book not found!\n";
                        }
                    }
                    
                    else if (choice == 3) {
                        string isbn;
                        cout << "Enter ISBN: ";
                        cin >> isbn;
                        Book *book = library.findBookByISBN(isbn);
                        if (book) {
                            Faculty *faculty = dynamic_cast<Faculty *>(currentUser);
                            if (faculty) {
                                faculty->returnBook(*book, time(0));
                                library.saveData();
                            }
                        } else {
                            cout << "Book not found!\n";
                            cin.ignore(); 
                            continue; 
                        }
                    } 
                    else if (choice == 4) { 
                        cout << "Your Borrowed Books:\n";
                        for (auto &rec : currentUser->account.borrowedBooks) {
                            cout << "Book ISBN: " << rec.first.ISBN << " Borrowed on: " << ctime(&rec.second);
                        }
                    }
                    else if (choice == 5) {
                        break;
                    }
                    else {
                        cout << "Invalid choice...\n";
                    }
                } 
                if (isFaculty == 0) {
                    cout << "Invalid credentials.\n";
                    break;
                }
            } while (true);
        }
        else if (choice == 3) {
            string name;
            string userID;
            cout << "Enter name: ";
            cin.ignore(); 
            getline(cin, name);
            cout << "Enter userID: ";
            cin >> userID;
            int isLibrarian = 0;
            User *currentUser = nullptr;
            for (auto user : library.users) {
                if (user->name == name && user->userID == userID) {
                    currentUser = user;
                    isLibrarian = 1;
                    break;
                }
            }
            do {
                if (currentUser) {
                    cout << "Welcome " << currentUser->name << endl;
                    cout << "1. View Available Books\n";
                    cout << "2. Add a Book\n";
                    cout << "3. Remove a Book\n";
                    cout << "4. Update a Book\n";
                    cout << "5. Add a User\n";
                    cout << "6. Remove a User\n";
                    cout << "7. Exit\n";
                    cout << "Enter your choice: ";
                    cin >> choice;

                    if (choice == 1) {
                        library.displayAvailableBooks();
                    } 
                    else if (choice == 2) {
                        string title, author, publisher, isbn;
                        int year;
                        cout << "Enter title: ";
                        cin.ignore();
                        getline(cin, title);
                        cout << "Enter author: ";
                        getline(cin, author);
                        cout << "Enter publisher: ";
                        getline(cin, publisher);
                        cout << "Enter ISBN: ";
                        cin >> isbn;
                        cout << "Enter year: ";
                        cin >> year;
                        Book newBook(title, author, publisher, year, isbn);
                        Librarian *librarian = dynamic_cast<Librarian *>(currentUser);
                        if (librarian) {
                            librarian->addBook(&library.books, newBook);
                        }
                    } 
                    else if (choice == 3) {
                        string isbn;
                        cout << "Enter ISBN: ";
                        cin >> isbn;
                        Librarian *librarian = dynamic_cast<Librarian *>(currentUser);
                        if (librarian) {
                            librarian->removeBook(library.books, isbn);
                        }
                    } 
                    else if (choice == 4) {
                        string isbn;
                        cout << "Enter ISBN: ";
                        cin >> isbn;
                        Librarian *librarian = dynamic_cast<Librarian *>(currentUser);
                        if (librarian) {
                            librarian->updateBook(library.books, isbn);
                        }
                    }
                    else if (choice == 5) {
                        string name, userID, role;
                        cout << "Enter name: ";
                        cin.ignore();
                        getline(cin, name);
                        cout << "Enter userID: ";
                        cin >> userID;
                        cout << "Enter role (Student/Faculty): ";
                        cin >> role;
                        User *newUser;
                        if (role == "Student")
                            newUser = new Student(name, userID);
                        else if (role == "Faculty")
                            newUser = new Faculty(name, userID);
                        else {
                            cout << "Invalid role.\n";
                            continue;
                        }
                        Librarian *librarian = dynamic_cast<Librarian *>(currentUser);
                        if (librarian) {
                            librarian->addUser(library.users, newUser);
                        }
                    }
                    else if (choice == 6) {
                        string userID;
                        cout << "Enter userID: ";
                        cin >> userID;
                        Librarian *librarian = dynamic_cast<Librarian *>(currentUser);
                        if (librarian) {
                            librarian->removeUser(library.users, userID);
                        }
                    }
                    else if (choice == 7) {
                        break;
                    }
                    else {
                        cout << "Invalid choice...\n";
                    }
                } 
                if (isLibrarian == 0) {
                    cout << "Invalid credentials.\n";
                    break;
                }
            } while (true);
        }
        else if (choice == 4) {
            library.saveData();
            break;
        }
        else {
            cout << "Invalid choice...\n";
        }
    } while (true);

    library.saveData();

    return 0;
}
