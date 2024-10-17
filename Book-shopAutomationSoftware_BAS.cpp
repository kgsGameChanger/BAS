// Contributors:-
//    Killada Gowtham Sai - 19CS10040
//    Deepak Nayak - 19CS30017
//    Pratham Nigam - 19CS10047


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Enum for User Roles
enum class Role {
    Customer,
    SalesClerk,
    Manager
};

// Function to convert string to Role enum
Role stringToRole(const string& roleStr) {
    if (roleStr == "Customer")
        return Role::Customer;
    else if (roleStr == "SalesClerk")
        return Role::SalesClerk;
    else
        return Role::Manager;
}

// Function to convert Role enum to string
string roleToString(Role role) {
    switch (role) {
        case Role::Customer: return "Customer";
        case Role::SalesClerk: return "SalesClerk";
        case Role::Manager: return "Manager";
        default: return "Unknown";
    }
}

// Class to represent a Book
class Book {
public:
    string title;
    string author;
    string ISBN;
    string publisher;
    string stockist;
    int copies;
    double price;
    int rackNumber;
    int requestCount;
    int soldCopies;
    double salesRevenue;

    Book() : copies(0), price(0.0), rackNumber(0), requestCount(0), soldCopies(0), salesRevenue(0.0) {}

    Book(string t, string a, string i, string p, string s, int c, double pr, int r,
         int req = 0, int sold = 0, double revenue = 0.0)
        : title(t), author(a), ISBN(i), publisher(p), stockist(s), copies(c), price(pr),
          rackNumber(r), requestCount(req), soldCopies(sold), salesRevenue(revenue) {}

    // Convert Book object to a string for file storage
    string toFileString() const {
        stringstream ss;
        ss << title << "|" << author << "|" << ISBN << "|" << publisher << "|"
           << stockist << "|" << copies << "|" << fixed << setprecision(2) << price << "|"
           << rackNumber << "|" << requestCount << "|" << soldCopies << "|"
           << fixed << setprecision(2) << salesRevenue;
        return ss.str();
    }

    // Display book details based on user role
    void display(Role userRole) const {
        cout << "\nTitle: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "ISBN: " << ISBN << endl;
        cout << "Publisher: " << publisher << endl;
        cout << "Price: $" << fixed << setprecision(2) << price << endl;
        if (userRole != Role::Customer) {
            cout << "Stockist: " << stockist << endl;
            cout << "Rack Number: " << rackNumber << endl;
            cout << "Copies Available: " << copies << endl;
            cout << "Request Count: " << requestCount << endl;
            cout << "Copies Sold: " << soldCopies << endl;
            cout << "Sales Revenue: $" << fixed << setprecision(2) << salesRevenue << endl;
        }
    }
};

// Class to represent a User
class User {
public:
    string username;
    string password;
    Role role;

    User() : role(Role::Customer) {}

    User(string u, string p, string r)
        : username(u), password(p), role(stringToRole(r)) {}

    // Convert User object to a string for file storage
    string toFileString() const {
        stringstream ss;
        ss << username << "|" << password << "|" << roleToString(role);
        return ss.str();
    }
};

// Class to represent the Book-shop Automation System
class BAS {
    vector<Book> inventory;
    vector<User> users;
    double totalRevenue;

public:
    BAS() : totalRevenue(0.0) {}

    // Load books from file
    void loadBooks(const string& filename = "books.txt") {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "books.txt not found. Starting with empty inventory." << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            vector<string> tokens;
            while (getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            if (tokens.size() < 11) continue; // Ensure all fields are present
            Book book;
            book.title = tokens[0];
            book.author = tokens[1];
            book.ISBN = tokens[2];
            book.publisher = tokens[3];
            book.stockist = tokens[4];
            book.copies = stoi(tokens[5]);
            book.price = stod(tokens[6]);
            book.rackNumber = stoi(tokens[7]);
            book.requestCount = stoi(tokens[8]);
            book.soldCopies = stoi(tokens[9]);
            book.salesRevenue = stod(tokens[10]);
            inventory.push_back(book);
        }
        file.close();
    }

    // Save books to file
    void saveBooks(const string& filename = "books.txt") {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening books.txt for writing." << endl;
            return;
        }
        for (const auto& book : inventory) {
            file << book.toFileString() << "\n";
        }
        file.close();
    }

    // Load users from file
    void loadUsers(const string& filename = "users.txt") {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "users.txt not found. Starting with no users." << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            vector<string> tokens;
            while (getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            if (tokens.size() < 3) continue; // Ensure all fields are present
            User user(tokens[0], tokens[1], tokens[2]);
            users.push_back(user);
        }
        file.close();
    }

    // Authenticate user and return pointer to User object
    User* authenticateUser() {
        string username, password;
        cout << "=== Login ===\n";
        cout << "Username (or type 'exit' to quit): ";
        cin >> username;
        if (username == "exit") {
            return nullptr;
        }
        cout << "Password: ";
        cin >> password;

        for (auto& user : users) {
            if (user.username == username && user.password == password) {
                cout << "Login successful as " << roleToString(user.role) << "!\n";
                return &user;
            }
        }
        cout << "Invalid credentials. Please try again.\n";
        return nullptr;
    }

    // Add a book to the inventory
    void addBook(const Book& book) {
        inventory.push_back(book);
    }

    // Search books by title or author
    void searchBook(Role userRole) {
        string query;
        cout << "Enter book title or author: ";
        cin.ignore();
        getline(cin, query);
        bool found = false;

        for (Book& book : inventory) {
            if (book.title == query || book.author == query) {
                cout << "\nBook found!" << endl;
                book.display(userRole);
                found = true;
            }
        }

        if (!found) {
            cout << "Book not found. ";
            if (userRole == Role::Customer) {
                cout << "Would you like to request it for future procurement? (yes/no): ";
                string choice;
                cin >> choice;
                if (choice == "yes") {
                    requestBook(query);
                }
            } else {
                cout << "You can add this book to the inventory if needed.\n";
            }
        }
    }

    // Handle book requests
    void requestBook(const string& query) {
        for (Book& book : inventory) {
            if (book.title == query || book.author == query) {
                book.requestCount++;
                cout << "Request for book '" << query << "' has been incremented." << endl;
                saveBooks();
                return;
            }
        }

        // If book doesn't exist, prompt to add details
        cout << "Book not found in system.\n";
        cout << "Enter details to add the book for future procurement.\n";
        string title, author, ISBN, publisher, stockist;
        int copies, rackNumber;
        double price;

        cout << "Title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Author: ";
        getline(cin, author);
        cout << "ISBN: ";
        getline(cin, ISBN);
        cout << "Publisher: ";
        getline(cin, publisher);
        cout << "Stockist: ";
        getline(cin, stockist);
        cout << "Number of copies: ";
        cin >> copies;
        cout << "Price: ";
        cin >> price;
        cout << "Rack Number: ";
        cin >> rackNumber;

        Book newBook(title, author, ISBN, publisher, stockist, copies, price, rackNumber, 1, 0, 0.0);
        inventory.push_back(newBook);
        cout << "Book added to inventory with a request count of 1." << endl;
        saveBooks();
    }

    // Purchase a book (for Customers)
    void purchaseBook() {
        string isbn;
        int cps;
        cout << "Enter ISBN of the book to purchase: ";
        cin >> isbn;
        cout << "No.of copies want to purchase: ";
        cin >> cps;

        for (Book& book : inventory) {
            if (book.ISBN == isbn) {
                if (book.copies > cps) {
                    book.copies -= cps;
                    book.soldCopies += cps;
                    book.salesRevenue += (book.price * cps);
                    totalRevenue += (book.price * cps);

                    cout << "Book purchased successfully!" << endl;
                    cout << "Sales receipt: " << endl;
                    cout << "Title: " << book.title << ", ISBN: " << book.ISBN << endl;
                    cout << "Price: $" << fixed << setprecision(2) << (book.price * cps) << endl;
                    saveBooks();
                } else {
                    cout << "Sorry, book is out of stock. Only " << book.copies << " copies available." << endl;
                }
                return;
            }
            
        }

        cout << "Book with ISBN " << isbn << " not found in inventory." << endl;
    }

    // Update inventory (for SalesClerks and Managers)
    void updateInventory() {
        string isbn;
        int newCopies;
        cout << "Enter ISBN of the book to restock: ";
        cin >> isbn;
        cout << "Enter number of new copies: ";
        cin >> newCopies;

        for (Book& book : inventory) {
            if (book.ISBN == isbn) {
                book.copies += newCopies;
                cout << "Inventory updated. New copies added: " << newCopies << endl;
                saveBooks();
                return;
            }
        }

        cout << "Book with ISBN " << isbn << " not found in inventory." << endl;
    }

    // Generate sales statistics (for Managers)
    void generateSalesStatistics() {
        cout << "\n=== Sales Statistics ===\n";
        cout << left << setw(30) << "Title" 
             << setw(20) << "Publisher" 
             << setw(15) << "ISBN" 
             << setw(15) << "Copies Sold" 
             << setw(15) << "Sales Revenue" << endl;
        cout << string(95, '-') << endl;

        for (const Book& book : inventory) {
            if (book.soldCopies > 0) {
                cout << left << setw(30) << book.title
                     << setw(20) << book.publisher
                     << setw(15) << book.ISBN
                     << setw(15) << book.soldCopies
                     << "$" << fixed << setprecision(2) << setw(14) << book.salesRevenue
                     << endl;
            }
        }
        cout << "\nTotal Revenue: $" << fixed << setprecision(2) << totalRevenue << endl;
    }

    // Print low stock books (for SalesClerks and Managers)
    void printLowStockBooks() {
        int threshold;
        cout << "Enter stock threshold: ";
        cin >> threshold;

        cout << "\n=== Books Below Threshold ===\n";
        cout << left << setw(30) << "Title" 
             << setw(25) << "Stockist" 
             << setw(15) << "Copies in Stock" << endl;
        cout << string(70, '-') << endl;

        for (const Book& book : inventory) {
            if (book.copies < threshold) {
                cout << left << setw(30) << book.title
                     << setw(25) << book.stockist
                     << setw(15) << book.copies
                     << endl;
            }
        }
    }

    // Display all available books with varying details based on role
    void showAllBooks(Role userRole) const {
        if (inventory.empty()) {
            cout << "No books available in the inventory." << endl;
            return;
        }

        cout << "\n=== All Available Books ===" << endl;
        for (const Book& book : inventory) {
            book.display(userRole);
        }
    }

    // Save all data before exiting
    void saveAll() {
        saveBooks();
        // Users are typically not modified in this system, but if needed, implement saving users.
    }

    // Main menu handler based on user role
    void handleMenu(User* user) {
        if (user == nullptr) return;

        Role userRole = user->role;
        int choice;

        do {
            cout << "\n--- Book-shop Automation Software (BAS) ---\n";
            cout << "Logged in as: " << roleToString(userRole) << "\n";
            cout << "1. Search for a Book\n";

            if (userRole == Role::Customer) {
                cout << "2. Purchase a Book\n";
                cout << "3. Show All Available Books\n";
                cout << "4. Logout\n";
            }
            else if (userRole == Role::SalesClerk) {
                cout << "2. Update Inventory\n";
                cout << "3. Print Low Stock Books\n";
                cout << "4. Show All Available Books\n";
                cout << "5. Logout\n";
            }
            else if (userRole == Role::Manager) {
                cout << "2. Update Inventory\n";
                cout << "3. Generate Sales Statistics\n";
                cout << "4. Print Low Stock Books\n";
                cout << "5. Show All Available Books\n";
                cout << "6. Logout\n";
            }

            cout << "Enter your choice: ";
            cin >> choice;

            if (userRole == Role::Customer) {
                switch (choice) {
                    case 1:
                        searchBook(userRole);
                        break;
                    case 2:
                        purchaseBook();
                        break;
                    case 3:
                        showAllBooks(userRole);
                        break;
                    case 4:
                        cout << "Logging out...\n";
                        return;
                    default:
                        cout << "Invalid choice. Try again." << endl;
                }
            }
            else if (userRole == Role::SalesClerk) {
                switch (choice) {
                    case 1:
                        searchBook(userRole);
                        break;
                    case 2:
                        updateInventory();
                        break;
                    case 3:
                        printLowStockBooks();
                        break;
                    case 4:
                        showAllBooks(userRole);
                        break;
                    case 5:
                        cout << "Logging out...\n";
                        return;
                    default:
                        cout << "Invalid choice. Try again." << endl;
                }
            }
            else if (userRole == Role::Manager) {
                switch (choice) {
                    case 1:
                        searchBook(userRole);
                        break;
                    case 2:
                        updateInventory();
                        break;
                    case 3:
                        generateSalesStatistics();
                        break;
                    case 4:
                        printLowStockBooks();
                        break;
                    case 5:
                        showAllBooks(userRole);
                        break;
                    case 6:
                        cout << "Logging out...\n";
                        return;
                    default:
                        cout << "Invalid choice. Try again." << endl;
                }
            }

        } while (true);
    }

    // Initialize the system (load data)
    void initialize() {
        loadUsers();
        loadBooks();
    }

    // Finalize the system (save data)
    void finalize() {
        saveAll();
    }
};

int main() {
    BAS bookShop;
    bookShop.initialize();

    while (true) {
        User* loggedInUser = bookShop.authenticateUser();
        if (loggedInUser == nullptr) {
            cout << "Exiting application. Goodbye!\n";
            break;
        }
        bookShop.handleMenu(loggedInUser);
    }

    bookShop.finalize();
    return 0;
}


