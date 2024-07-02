# Library-Management-System

This project is a console-based Library Management System implemented in C++. It uses object-oriented programming principles and file I/O for persistence. The system allows users to log in as either administrators or members, manage books, and perform various actions based on their roles.

## Features

- **Admin Functions:**
  - Add books to the library.
  - Remove books from the library.
  - Change password.

- **Member Functions:**
  - Borrow books from the library.
  - Return borrowed books.
  - View borrowed books.
  - Change password.

- **General Functions:**
  - Login system for administrators and members.
  - Create new member accounts.
  - Persist data using files (`books.txt` and `users.txt`).



### Installation



1. **Create Data Files:**

    Create `users.txt` and `books.txt` files in the same directory as your executable.

    **users.txt:**

    ```plaintext
    1,Admin,adminpass,Administrator
    ```

    **books.txt:**

    ```plaintext
    1,1984,George Orwell,1
    2,To Kill a Mockingbird,Harper Lee,1
    ```

2. **Compile the Program:**

    ```bash
    g++ -o library_system main.cpp
    ```

3. **Run the Program:**

    ```bash
    ./library_system
    ```

## Usage

1. **Initial Menu:**

    - Admin Login
    - Member Login
    - Exit

2. **Admin Login:**

    - Use the ID `1` and password `adminpass` to log in as an admin.

3. **Member Login:**

    - Choose to log in as an existing member or create a new account.

4. **Admin Functions:**

    - Add a book: Enter book ID, title, and author.
    - Remove a book: Enter book ID to remove.
    - Change password: Enter a new password.

5. **Member Functions:**

    - Borrow a book: Enter book ID to borrow.
    - Return a book: Enter book ID to return.
    - View borrowed books: Display a list of borrowed books.
    - Change password: Enter a new password.

## Code Structure

- **Book Class:**
  - Manages book information and availability.

- **User Class:**
  - Base class for common user attributes and methods.
  - Abstract class with virtual functions.

- **Member Class:**
  - Inherits from User.
  - Manages borrowing and returning of books.

- **Administrator Class:**
  - Inherits from User.
  - Manages adding and removing books.

- **Library Class:**
  - Manages users and books.
  - Handles file I/O operations for persistence.
  - Provides login functionality and user interface.
