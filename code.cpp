#include<bits/stdc++.h>
using namespace std;

// Book class
class Book {
public:
    int bookID;
    string title;
    string author;
    bool isAvailable;

    Book(int id,string t, std::string a){
        this->bookID=id;
        this->title=t;
        this->author=a;
        this->isAvailable=true;
    }

    void displayDetails() {
    cout << "Book ID: " << bookID << "\nTitle: " << title << "\nAuthor: " << author << "\nAvailability: " << (isAvailable ? "Available" : "Not Available") <<endl;
    }
};

// User class
class User {
protected:
    int userID;
    string name;
public:
    User(int id,string n) {
        this->userID=id;
        this->name=n;
    }

    virtual void displayInfo() {
        cout << "User ID: " << userID << "\nName: " << name <<endl;
    }
    string getname(){
        return name;
    }
};

// Member class
class Member : public User {
    vector<Book> borrowedBooks;
public:
    Member(int id, std::string n) : User(id, n) {}

    void borrowBook(Book &book) {
        if (book.isAvailable) {
            borrowedBooks.push_back(book);
            book.isAvailable = false;
            std::cout << name << " borrowed " << book.title << std::endl;
        } else {
            throw runtime_error("Book is not available.");
        }
    }

    void returnBook(Book &book) {
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it) {
            if (it->bookID == book.bookID) {
                borrowedBooks.erase(it);
                book.isAvailable = true;
                cout << name << " returned " << book.title << std::endl;
                return;
            }
        }
        throw runtime_error("Book not found in borrowed list.");
    }
};

// Administrator class
class Administrator : public User {
public:
    Administrator(int id,string n) : User(id, n) {}

    void manageUser() {
        cout << "Managing users." << endl;
    }
    void manageBooks() {
        cout << "Managing books." << endl;
    }
};

// Library class
class Library {
    vector<Book> books;
    vector<User*> users;
public:
    void addBook(Book book) {
        books.push_back(book);
        cout << "Book added: " << book.title << endl;
    }

    void addUser(User *user) {
        users.push_back(user);
        cout << "User added: " << user->getname() << endl;
    }

    Book& findBookByID(int bookID) {
        for (auto &book : books) {
            if (book.bookID == bookID) {
                return book;
            }
        }
        throw runtime_error("Book not found.");
    }

    void displayBooks() {
        for (auto &book : books) {
            book.displayDetails();
        }
    }

    void displayUsers() {
        for (const auto &user : users) {
            user->displayInfo();
        }
    }
};

int main() {
    Library library;

    Book book1(1, "1984", "George Orwell");
    Book book2(2, "To Kill a Mockingbird", "Harper Lee");

    library.addBook(book1);
    library.addBook(book2);

    Member member1(1, "Alice");
    Administrator admin1(2, "Bob");

    library.addUser(&member1);
    library.addUser(&admin1);

    library.displayBooks();
    library.displayUsers();

    try {
        Book &bookToBorrow = library.findBookByID(1);
        member1.borrowBook(bookToBorrow);
        library.displayBooks();
    } catch (const exception &e) {
        cerr << e.what() << endl;
    }

    try {
        Book &bookToReturn = library.findBookByID(1);
        member1.returnBook(bookToReturn);
        library.displayBooks();
    } catch (const exception &e) {
        cerr << e.what() << endl;
    }

    try {
        Book &bookNotExist = library.findBookByID(3);
        member1.borrowBook(bookNotExist);
    } catch (const exception &e) {
        cerr << e.what() <<endl;
    }

    return 0;
}
