# Library Management System

This is a Program written in C++ that manages the working of a library. The system supports three types of users: **Librarians**,**Students**, and **Faculty**. Students and Faculty can borrow, return, and view books, while Libarians can manage books and users.

### Compilation & Execution Instructions

#### Compilation Command
```bash
g++ libraryManagementSystem.cpp -o libraryManagementSystem
```
#### Execution Command
```bash
.\libraryManagementSystem 
```

### Features

1. **User Roles**:
- **Students**: Can borrow up to 3 books for 15 days. Fines are applied for overdue books.
- **Faculty**: Can borrow up to 5 books for 30 days. No fines are applied for overdue books.
- **Librarians**: Can manage books and users (add, remove, update).
2. **Book Management**:
- Add new books to the library.
- Remove existing books from the library.
- Update book details such as title, author, publisher, year, ISBN, and status.
3. **Borrowing and Returning Books**:
- Users can borrow available books.
- Books can be returned with automatic status updates.
4. **Fine Management**:
- Automatically calculates fines for overdue books (for student only).
- Students can view and pay their fines.
5. **Data Persistence**:
- Save all changes (books and user data) to files (books.txt and users.txt) for persistent storage.

### Requirements

#### Software Requirements
- A C++ compiler like g++ (part of GCC).
- Visual Studio Code (or any text editor/IDE).
#### Files Required
- books.txt : Stores the list of the books in the library.
- users.txt : Stores the list of registered users.

### How to Use
1. When you run the program, you will see a menu like this:
```text
~ LIBRARY MANAGEMENT SYSTEM ~
1. Student
2. Faculty
3. Librarian
4. Exit
Enter your choice:
```
2. Select your role by entering the corresponding number.

#### For Students:
- Log in with your name and user ID.
- After logging in, you can perform these actions:
 - View available books.
 - Borrow a book (up to 3 books at a time).
 - Return a borrowed book.
 - View borrowed books.
 - Check fine amount for overdue books.
 - Pay fines to clear outstanding dues.
 - Exit.

#### For Faculty:
- Log in with your name and user ID.
- After logging in, you can perform these actions:
 - View available books.
 - Borrow a book (up to 5 books at a time).
 - Return a borrowed book.
 - View borrowed books.
 - Exit.

#### For Librarians:
- Log in with your name and user ID.
- After logging in, you can perform these actions:
 - View available books.
 - Add new books to the library.
 - Remove existing books from the library.
 - Update book details (title, author, publisher, etc.).
 - Add new users (students or faculty).
 - Remove existing users.
 - Exit.


