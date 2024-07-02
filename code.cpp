#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

// Book class
class Book {
public:
    int bookID;
    std::string title;
    std::string author;
    bool isAvailable;

    Book(int id, std::string t, std::string a, bool available = true)
        : bookID(id), title(t), author(a), isAvailable(available) {}

    void displayDetails() const {
        std::cout << "Book ID: " << bookID << "\nTitle: " << title << "\nAuthor: " << author << "\nAvailability: " << (isAvailable ? "Available" : "Not Available") << std::endl;
    }

    std::string toString() const {
        return std::to_string(bookID) + "," + title + "," + author + "," + (isAvailable ? "1" : "0");
    }

    static Book fromString(const std::string& str) {
        std::istringstream iss(str);
        std::string token;
        std::getline(iss, token, ',');
        int id = std::stoi(token);
        std::getline(iss, token, ',');
        std::string title = token;
        std::getline(iss, token, ',');
        std::string author = token;
        std::getline(iss, token, ',');
        bool isAvailable = token == "1";
        return Book(id, title, author, isAvailable);
    }
};

// User class
class User {
protected:
    int userID;
    std::string name;
    std::string password;
public:
    User(int id, std::string n, std::string p) : userID(id), name(n), password(p) {}

    virtual ~User() = default;

    virtual void displayInfo() const {
        std::cout << "User ID: " << userID << "\nName: " << name << std::endl;
    }
    int getID(){
        return this->userID;
    }

    std::string getName() const {
        return name;
    }

    std::string getPassword() const {
        return password;
    }

    void setPassword(const std::string& newPassword) {
        password = newPassword;
    }

    virtual void displayMenu() const = 0;

    virtual std::string toString() const {
        return std::to_string(userID) + "," + name + "," + password + "," + getType();
    }

    static User* fromString(const std::string& str);

    virtual std::string getType() const = 0;
};

// Member class
class Member : public User {
    std::vector<Book> borrowedBooks;
public:
    Member(int id, std::string n, std::string p) : User(id, n, p) {}

    void borrowBook(Book &book) {
        if (book.isAvailable) {
            borrowedBooks.push_back(book);
            book.isAvailable = false;
            std::cout << name << " borrowed " << book.title << std::endl;
        } else {
            throw std::runtime_error("Book is not available.");
        }
    }

    void returnBook(Book &book) {
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it) {
            if (it->bookID == book.bookID) {
                borrowedBooks.erase(it);
                book.isAvailable = true;
                std::cout << name << " returned " << book.title << std::endl;
                return;
            }
        }
        throw std::runtime_error("Book not found in borrowed list.");
    }

    void displayMenu() const override {
        std::cout << "1. Borrow a book\n2. Return a book\n3. View borrowed books\n4. Change password\n5. Exit" << std::endl;
    }

    void viewBorrowedBooks() const {
        std::cout << name << "'s borrowed books:" << std::endl;
        for (const auto &book : borrowedBooks) {
            book.displayDetails();
        }
    }

    std::string getType() const override {
        return "Member";
    }
};

// Administrator class
class Administrator : public User {
public:
    Administrator(int id, std::string n, std::string p) : User(id, n, p) {}

    void displayMenu() const override {
        std::cout << "1. Add a book\n2. Remove a book\n3. Change password\n4. Exit" << std::endl;
    }

    std::string getType() const override {
        return "Administrator";
    }
};

// Library class
class Library {
    std::vector<Book> books;
    std::vector<User*> users;
public:
    Library() {
        loadBooks();
        loadUsers();
    }

    ~Library() {
        saveBooks();
        saveUsers();
        for (auto user : users) {
            delete user;
        }
    }

    void loadBooks() {
        std::ifstream file("books.txt");
        std::string line;
        while (std::getline(file, line)) {
            books.push_back(Book::fromString(line));
        }
        file.close();
    }

    void saveBooks() const {
        std::ofstream file("books.txt");
        for (const auto& book : books) {
            file << book.toString() << std::endl;
        }
        file.close();
    }

    void loadUsers() {
        std::ifstream file("users.txt");
        std::string line;
        while (std::getline(file, line)) {
            users.push_back(User::fromString(line));
        }
        file.close();
    }

    void saveUsers() const {
        std::ofstream file("users.txt");
        for (const auto& user : users) {
            file << user->toString() << std::endl;
        }
        file.close();
    }

    void addBook(const Book &book) {
        books.push_back(book);
        std::cout << "Book added: " << book.title << std::endl;
    }

    void removeBook(int bookID) {
        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->bookID == bookID) {
                std::cout << "Book removed: " << it->title << std::endl;
                books.erase(it);
                return;
            }
        }
        throw std::runtime_error("Book not found.");
    }

    void addUser(User *user) {
        users.push_back(user);
        std::cout << "User added: " << user->getName() << std::endl;
    }

    Book& findBookByID(int bookID) {
        for (auto &book : books) {
            if (book.bookID == bookID) {
                return book;
            }
        }
        throw std::runtime_error("Book not found.");
    }

    User* findUserByID(int userID) {
        for (auto &user : users) {
            if (user->getID() == userID) {
                return user;
            }
        }
        return nullptr;
    }

    void displayBooks() const {
        for (const auto &book : books) {
            book.displayDetails();
        }
    }

    void displayUsers() const {
        for (const auto &user : users) {
            user->displayInfo();
        }
    }

    void adminLogin() {
        int userID;
        std::string password;
        std::cout << "Enter admin ID: ";
        std::cin >> userID;
        std::cout << "Enter password: ";
        std::cin >> password;

        User* user = findUserByID(userID);
        if (user && user->getPassword() == password && dynamic_cast<Administrator*>(user)) {
            userInterface(*dynamic_cast<Administrator*>(user));
        } else {
            std::cout << "Invalid ID or password." << std::endl;
        }
    }

    void memberLogin() {
        int choice;
        std::cout << "1. Existing member\n2. New member\nEnter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            int userID;
            std::string password;
            std::cout << "Enter member ID: ";
            std::cin >> userID;
            std::cout << "Enter password: ";
            std::cin >> password;

            User* user = findUserByID(userID);
            if (user && user->getPassword() == password && dynamic_cast<Member*>(user)) {
                userInterface(*dynamic_cast<Member*>(user));
            } else {
                std::cout << "Invalid ID or password." << std::endl;
            }
        } else if (choice == 2) {
            int userID;
            std::string name, password;
            std::cout << "Enter new member ID: ";
            std::cin >> userID;
            std::cout << "Enter name: ";
            std::cin >> name;
            std::cout << "Enter password: ";
            std::cin >> password;

            User* user = findUserByID(userID);
            if (!user) {
                addUser(new Member(userID, name, password));
                std::cout << "Member account created." << std::endl;
            } else {
                std::cout << "User ID already exists." << std::endl;
            }
        }
    }

    void userInterface(User &user) {
        int choice;
        bool exit = false;
        do {
            user.displayMenu();
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            if (Member *member = dynamic_cast<Member*>(&user)) {
                switch (choice) {
                    case 1:
                        borrowBookInterface(*member);
                        break;
                    case 2:
                        returnBookInterface(*member);
                        break;
                    case 3:
                        member->viewBorrowedBooks();
                        break;
                    case 4:
                        changePassword(*member);
                        break;
                    case 5:
                        exit = true;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                }
            } else if (Administrator *admin = dynamic_cast<Administrator*>(&user)) {
                switch (choice) {
                    case 1:
                        addBookInterface();
                        break;
                    case 2:
                        removeBookInterface();
                        break;
                    case 3:
                        changePassword(*admin);
                        break;
                    case 4:
                        exit = true;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                }
            }
        } while (!exit);
    }

private:
    void borrowBookInterface(Member &member) {
        int bookID;
        std::cout << "Enter book ID to borrow: ";
        std::cin >> bookID;
        try {
            Book &book = findBookByID(bookID);
            member.borrowBook(book);
            saveBooks();
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void returnBookInterface(Member &member) {
        int bookID;
        std::cout << "Enter book ID to return: ";
        std::cin >> bookID;
        try {
            Book &book = findBookByID(bookID);
            member.returnBook(book);
            saveBooks();
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void addBookInterface() {
        int bookID;
        std::string title, author;
        std::cout << "Enter book ID: ";
        std::cin >> bookID;
        std::cout << "Enter title: ";
        std::cin.ignore(); // To ignore the newline character left by previous input
        std::getline(std::cin, title);
        std::cout << "Enter author: ";
        std::getline(std::cin, author);
        Book book(bookID, title, author);
        addBook(book);
        saveBooks();
    }

    void removeBookInterface() {
        int bookID;
        std::cout << "Enter book ID to remove: ";
        std::cin >> bookID;
        try {
            removeBook(bookID);
            saveBooks();
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void changePassword(User &user) {
        std::string newPassword;
        std::cout << "Enter new password: ";
        std::cin >> newPassword;
        user.setPassword(newPassword);
        saveUsers();
        std::cout << "Password changed successfully." << std::endl;
    }
};

User* User::fromString(const std::string& str) {
    std::istringstream iss(str);
    std::string token;
    std::getline(iss, token, ',');
    int id = std::stoi(token);
    std::getline(iss, token, ',');
    std::string name = token;
    std::getline(iss, token, ',');
    std::string password = token;
    std::getline(iss, token, ',');
    std::string type = token;

    if (type == "Member") {
        return new Member(id, name, password);
    } else if (type == "Administrator") {
        return new Administrator(id, name, password);
    } else {
        throw std::runtime_error("Unknown user type.");
    }
}

int main() {
    Library library;
    int choice;

    std::cout << "Welcome to the Library Management System" << std::endl;
    do {
        std::cout << "1. Admin Login\n2. Member Login\n3. Exit\nEnter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                library.adminLogin();
                break;
            case 2:
                library.memberLogin();
                break;
            case 3:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 3);

    return 0;
}
