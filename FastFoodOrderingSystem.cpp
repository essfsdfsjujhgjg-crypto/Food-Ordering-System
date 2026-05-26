#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <limits>
#include <iostream>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cctype>  
#include <locale>

using namespace std;

//* Admin Module *//

struct Product {
    int id;
    string name;
    double price;
    int quantity;
    Product* next;
};

//LinkedQueue for admin ( product adding , product display, product editing, deleting product)

class ProductQueue {
public:
    Product* front;
    Product* rear;

    ProductQueue() : front(NULL), rear(NULL) {}
    
    Product* findProductById(int id) {
        Product* current = front;
        while (current != NULL) {
            if (current->id == id) {
                return current;
            }
            current = current->next;
        }
        return NULL;
    }

    void enqueue(int id, const string& name, double price, int quantity);
    bool dequeue(string& name, double& price);
   
};

class Admin {
public:
    static void registerAdmin(string& username);
    friend string login();
    static void addBurgerRecords(ProductQueue& productQueue);
    static void addBeverageRecords(ProductQueue& productQueue);
    static void editProduct(ProductQueue& burgerQueue, ProductQueue& beverageQueue);
    static void sortCustomerOrders();
    static void deleteProduct(ProductQueue& productQueue);
    static void binarySearchProduct(const std::string& filename, const std::string& productName);
    
private:
    static Product* findProductByName(Product* front, const string& productName);
};

void loadRecordsFromFile(const string& filename, ProductQueue& productQueue);
void saveRecordsToFile(const string& filename, ProductQueue& productQueue);
void displayProducts(const ProductQueue& productQueue);




void toLowerCase(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::tolower(c); });
}

int compareIgnoreCase(const std::string& str1, const std::string& str2) {
    size_t len1 = str1.length();
    size_t len2 = str2.length();
    size_t minLen = std::min(len1, len2);

    for (size_t i = 0; i < minLen; ++i) {
        char c1 = std::tolower(str1[i]);
        char c2 = std::tolower(str2[i]);

        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
    }

    if (len1 < len2) return -1;
    if (len1 > len2) return 1;
    return 0;  
}

// Binary Search for admin to search product 

void Admin::binarySearchProduct(const std::string& filename, const std::string& productName) {
    ProductQueue productQueue;
    loadRecordsFromFile(filename, productQueue);

    Product* current = productQueue.front;
    int count = 0;

    
    while (current != nullptr) {
        count++;
        current = current->next;
    }

    
    current = productQueue.front;

    int first = 0;
    int last = count - 1;

    while (first <= last) {
    	
        int mid = (first + last) / 2;

        
        Product* midNode = current;
        for (int i = 0; i < mid; ++i) {
            midNode = midNode->next;
        }

        std::cout << "Comparing: '" << midNode->name << "' with '" << productName << "'\n";

        if (compareIgnoreCase(midNode->name, productName) == 0) {
            count = 1;
            current = midNode;
            break;
        } else if (compareIgnoreCase(productName, midNode->name) < 0) {
            last = mid - 1;
        } else {
            first = mid + 1;
        }
    }

    if (count == 1) {
        std::cout << "Search results in " << filename << " for query " << productName << ": ";
        std::cout << "\nThe fast food item is successfully found :";
        std::cout << "\nFast food product details:\n";
        std::cout << "ID: " << current->id << ", Name: " << current->name
                  << ", Price: RM" << current->price << ", Quantity: " << current->quantity << std::endl;

        std::cout << "\n\nPlease press any key to return to the main menu\n";
        system("pause");
        system("cls");
    } else {
        std::cout << "No matching fast food item is found in " << filename << " for query '" << productName << "'" << std::endl;
        std::cout << "\n\n\nPlease press any key to return to the main menu";
        system("pause");
        system("cls");
    }
}

	/*
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::vector<std::string> products;
    std::string product;
    while (getline(file, product)) {
        size_t spacePos = product.find(' ');  
        if (spacePos != std::string::npos) {
            products.push_back(product.substr(0, spacePos));  
        }
    }
    file.close();

    
    std::sort(products.begin(), products.end());

    
    std::cout << "Products in " << filename << ": ";
    for (std::vector<std::string>::iterator it = products.begin(); it != products.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    
    std::vector<std::string> matches;
    for (std::vector<std::string>::iterator it = products.begin(); it != products.end(); ++it) {
        if (it->find(searchQuery) != std::string::npos) {
            matches.push_back(*it);
        }
    }

    
    if (!matches.empty()) {
        std::cout << "Search results in " << filename << " for query '" << searchQuery << "': ";
        for (std::vector<std::string>::iterator it = matches.begin(); it != matches.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No matching products found in " << filename << " for query '" << searchQuery << "'" << std::endl;
    }
    */


// Deleting product by admin using linked queue

void Admin::deleteProduct(ProductQueue& productQueue) {
    string productName;
    cout << "Enter the name of the product to delete: ";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(cin, productName);

    Product* current = productQueue.front;
    Product* prev = NULL;

    while (current != NULL && current->name != productName) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        
        if (prev != NULL) {
            prev->next = current->next;
        } else {
            productQueue.front = current->next;
        }

        delete current;
        cout << "Product deleted successfully!" << endl;
    } else {
        cout << "Product not found. Unable to delete." << endl;
    }
}

// Insertion sort to sort customers based on their name

void Admin::sortCustomerOrders() {
    
    ifstream ordersFile("user_orders.txt");
    vector<string> orders;

    if (ordersFile.is_open()) {
        string order;
        while (getline(ordersFile, order)) {
            orders.push_back(order);
        }
        ordersFile.close();

       
        for (size_t i = 1; i < orders.size(); ++i) {
            string key = orders[i];
            int j = static_cast<int>(i) - 1;

            while (j >= 0 && key < orders[static_cast<size_t>(j)]) {
                orders[static_cast<size_t>(j) + 1] = orders[static_cast<size_t>(j)];
                --j;
            }

            orders[static_cast<size_t>(j) + 1] = key;
        }
        
        ofstream sortedOrdersFile("user_orders_sorted.txt");
        if (sortedOrdersFile.is_open()) {
            for (size_t i = 0; i < orders.size(); ++i) {
                sortedOrdersFile << orders[i] << endl;
            }
            sortedOrdersFile.close();
            cout << "Orders sorted using insertion sort and saved to user_orders_sorted.txt" << endl;
        } else {
            cerr << "Error opening file: user_orders_sorted.txt" << endl;
        }
    } else {
        cerr << "Error opening file: user_orders.txt" << endl;
    }
}

// linked queue enqueue

void ProductQueue::enqueue(int id, const string& name, double price, int quantity) {
    Product* newNode = new Product;
    newNode->id = id;
    newNode->name = name;
    newNode->price = price;
    newNode->quantity = quantity;
    newNode->next = NULL;

    if (rear == NULL) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

// linked queue dequeue

bool ProductQueue::dequeue(string& name, double& price) {
    if (front == NULL) {
        return false;  
    }

    Product* temp = front;
    name = temp->name;
    price = temp->price;

    front = front->next;
    delete temp;

    if (front == NULL) {
        rear = NULL;  
    }

    return true;
}

//Admin registration

void Admin::registerAdmin(string& username) {
    system("cls");
    string enteredUsername, userpass, un, up;
    cout << "===================";
    cout << "\nEnter your name: ";
    cin>>enteredUsername;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    cout << "\nEnter password: ";
    cin>>userpass;
    cout << "===================" << endl;

    ifstream check("admin.txt");
    // Validation for admin registration
    while (check >> un >> up) {
        if (un == enteredUsername) {
            cout << "Please change to another username as it is already used by another admin. " << endl;
            check.close();
		    //system("pause");
		    getch();
			system("cls");
            return ;
        } else if (up == userpass) {
            cout << "Please change to another password as it is already used by another admin." << endl;
            check.close();
            system("pause");
            getch();
			system("cls");
            return;
        }
    }
    check.close();

    ofstream registerFile("admin.txt", ios::app);
    registerFile << enteredUsername << ' ' << userpass << endl;
    
    
    system("pause");
	system("cls");

    username = enteredUsername;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
}

// Admin login

string login() {
    system("cls");
    string loginname, loginpass, x, y;
    int count = 0;
    cout << "Enter Name : ";
    cin>>loginname;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    cout << "\nEnter Password : ";
    cin>>loginpass;

    ifstream input("admin.txt");
    while (input >> x >> y) {
        if (x == loginname && y == loginpass) {
            count = 1;
            system("cls");
        }
    }
    input.close();
    if (count == 1) {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        cin.get();
        return loginname;
    } else {
        cout << "\nWrong username or password\n"; //validation for admin
        return "";
    }
}

// Adding product by admin using linked queue

void Admin::addBurgerRecords(ProductQueue& productQueue) {
    int burgerId;
    string burgerName;
    double burgerPrice;
    int burgerQuantity;

    while (true) {
        cout << "        Enter Burger ID: ";
        cin >> burgerId;

        if (productQueue.findProductById(burgerId) != NULL) {
            cout << "  This ID is already in use. Please enter a different ID." << endl;
        } else {
            break;
        }
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "        Enter Burger name: ";
    getline(cin, burgerName);

    cout << "        Enter Burger price(RM): ";
    cin >> burgerPrice;

    cout << "        Enter Burger quantity: ";
    cin >> burgerQuantity;

    productQueue.enqueue(burgerId, burgerName, burgerPrice, burgerQuantity);
    cout << "        Burger record added successfully!" << endl;

    displayProducts(productQueue);

    system("pause");
    system("cls");
}

// Adding product by admin using linked queue

void Admin::addBeverageRecords(ProductQueue& productQueue) {
    int beverageId;
    string beverageName;
    double beveragePrice;
    int beverageQuantity;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (true) {
        cout << "       Enter Beverage ID: ";
    	cin >> beverageId;

        if (productQueue.findProductById(beverageId) !=NULL) {
        cout << "    This ID is already in use. Please enter a different ID." << endl;
        } else {
            break;
        }
    }

    cout << "       Enter Beverage name: ";
    cin >> beverageName;

    cout << "       Enter Beverage price(RM - Please input integers only): ";
    cin >> beveragePrice;

    cout << "       Enter Beverage quantity: ";
    cin >> beverageQuantity;

    productQueue.enqueue(beverageId, beverageName, beveragePrice, beverageQuantity);
    cout << "       Beverage record added successfully!" << endl;

    // Display the beverage list
    displayProducts(productQueue);

    system("pause");
    system("cls");
}

// Editing product by admin using linked queue

void Admin::editProduct(ProductQueue& burgerQueue, ProductQueue& beverageQueue) {
    int productType;
    cout << "\n\n\tEnter the type of the product to edit (Choose 1 for Burger, 2 for Beverage): ";
    cin >> productType;
    
    ProductQueue* queueToEdit = nullptr;
    string filename;
    if (productType == 1) {
        queueToEdit = &burgerQueue;
        filename = "burger.txt";
    } else if (productType == 2) {
        queueToEdit = &beverageQueue;
        filename = "beverage.txt";
    } else {
        cout << "Invalid product type selected." << endl;
        return;
    }

    
    cout << "Available IDs in " << (productType == 1 ? "Burgers" : "Beverages") << ":\n";
    Product* current = queueToEdit->front;
    if (current == nullptr) {
        cout << "ID list is empty." << endl;
       
        
        cout << "\n\nPress any key to return to Admin's main menu." << endl;
        getch();
        system("cls");
        return; 
    }

    while (current != nullptr) {
        cout << "ID: " << current->id << ", Name: " << current->name << endl;
        current = current->next;
    }

    
    int productId;
    cout << "Enter the ID of the product to edit: ";
    cin >> productId;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Product* productToEdit = queueToEdit->findProductById(productId);
    if (productToEdit == nullptr) {
        cout << "Product with ID " << productId << " not found." << endl;
        return;
    }

    if (productToEdit != nullptr) {
        string newName;
        double newPrice;
        int newQuantity;
        int editChoice;

        cout << "Current product details:\n";
        cout << "Name: " << productToEdit->name << ", Price: RM" << productToEdit->price << ", Quantity: " << productToEdit->quantity << endl;
        cout << "What would you like to edit?\n1. Name\n2. Price\n3. Quantity\n4. All\nEnter your choice: ";
        cin >> editChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        switch (editChoice) {
            case 1:
                cout << "Enter the new name: ";
                getline(cin, newName);
                productToEdit->name = newName;
                break;
            case 2:
                cout << "Enter the new price (RM): ";
                cin >> newPrice;
                productToEdit->price = newPrice;
                break;
            case 3:
                cout << "Enter the new quantity: ";
                cin >> newQuantity;
                productToEdit->quantity = newQuantity;
                break;
            case 4:
                cout << "Enter the new name: ";
                getline(cin, newName);
                productToEdit->name = newName;
                cout << "Enter the new price (RM): ";
                cin >> newPrice;
                productToEdit->price = newPrice;
                cout << "Enter the new quantity: ";
                cin >> newQuantity;
                productToEdit->quantity = newQuantity;
                break;
            default:
                cout << "Invalid choice." << endl;
                return;
        }

        cout << "Product updated successfully!\n";
        system("pause");
        system("cls");

        saveRecordsToFile(filename, *queueToEdit);
    } else {
        cout << "Product not found with ID: " << productId << endl;
    }
}


void writeRecordsToFile(const string& filename, Product* front) {
    std::ofstream outFile(filename, std::ios::trunc); // Use trunc mode to overwrite existing content

    if (!outFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    Product* current = front;
    while (current != NULL) {
        // Assuming the desired file format includes the ID and quantity
        outFile << current->id << " " << current->name << " RM" << std::fixed << std::setprecision(2) << current->price << " " << current->quantity << std::endl;
        current = current->next;
    }

    outFile.close();
}

void clearQueue(Product*& front, Product*& rear) {
    while (front != NULL) {
        Product* temp = front;
        front = front->next;
        delete temp;
    }
    rear = NULL;
}


void loadRecordsFromFile(const string& filename, ProductQueue& productQueue) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    vector<Product> products;

    int id;
    string name;
    double price;
    int quantity;

    while (inFile >> id >> name >> price >> quantity) {
        products.push_back({id, name, price, quantity});
    }

    inFile.close();

    // Sort the products based on name before enqueueing
    sort(products.begin(), products.end(), [](const Product& a, const Product& b) {
        return compareIgnoreCase(a.name, b.name) < 0;
    });

    for (const Product& p : products) {
        productQueue.enqueue(p.id, p.name, p.price, p.quantity);
    }
}

void saveRecordsToFile(const string& filename, ProductQueue& productQueue) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    Product* current = productQueue.front;
    while (current != nullptr) {
        outFile << current->id << " " << current->name << " " << current->price << " " << current->quantity << endl;
        current = current->next;
    }

    outFile.close();
}

void displayProducts(const ProductQueue& productQueue) {
    Product* current = productQueue.front;
    cout << "\nCurrent Products:\n";
    cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(15) << "Price(RM)" << "Quantity\n";
    while (current != nullptr) {
        cout << left << setw(5) << current->id << setw(20) << current->name
             << "RM" << fixed << setprecision(2) << setw(15) << current->price
             << setw(10) << current->quantity << '\n';
        current = current->next;
    }
}


    
//*****User Module 
 
struct OrderNode {
	int orderID; 
    string username;
    string burgername;
    char size;
    int quantity;
    string bevname;
    int bevquantity;
    float price, bevprice, totalprice, totalbev;
    OrderNode* next;

    OrderNode(string user, string burger, char size, int quantity, string beverage, int bevQuantity, float totalPrice, OrderNode* nextNode)
        : username(user), burgername(burger), size(size), quantity(quantity), bevname(beverage), bevquantity(bevQuantity), totalprice(totalPrice), next(nextNode) {}
};

// Linked queue for user to add orders
 
class OrderQueue {
private:
    OrderNode* front;
    OrderNode* rear;

public:
    OrderQueue() : front(NULL), rear(NULL) {}

    void enqueue(string user, string burger, char size, int quantity, string beverage, int bevQuantity, float totalPrice) {
        OrderNode* newNode = new OrderNode(user, burger, size, quantity, beverage, bevQuantity, totalPrice, NULL);

        if (!front) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }
    
 

    // Displaying records
    void displayOrders() {
    OrderNode* current = front;
    int orderCount = 1;
    
    
    cout <<"\n===============================================Displaying Orders==================================================="<<endl;

    cout << "--------------------------------------------------------------------------------------------------------------------"<<endl;
    cout << setw(10) << "Order ID" << setw(15) << "Username" << setw(15) << "Burger" << setw(10)
         << "Size" << setw(10) << "Quantity" << setw(15) << "Beverage" << setw(20) << "Bev Quantity" << setw(15) << "Total Price" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------"<<endl;

    
    while (current) {
        cout << setw(10) << orderCount
             << setw(15) << current->username
             << setw(15) << current->burgername
             << setw(10) << current->size
             << setw(10) << current->quantity
             << setw(15) << current->bevname
             << setw(20) << current->bevquantity
             << setw(15) << current->totalprice << endl;

        
        current = current->next;

       
        orderCount++;
    }



    cout << "--------------------------------------------------------------------------------------------------------------------"<<endl;
}

    void saveOrdersToFile() {
        ofstream outFile("user_orders.txt");
        if (!outFile) {
            cerr << "Error opening file for writing." << endl;
            return;
        }

        OrderNode* current = front;
        while (current) {
            outFile << current->username << ' ' << current->burgername << ' ' << current->size << ' ' << current->quantity << ' '
                    << current->bevname << ' ' << current->bevquantity << ' ' << current->totalprice << endl;
            current = current->next;
        }

        outFile.close();
    }
    
// Editing of orders by users using linked queue
 
  void editOrder() {
    	
		string username;
        cout << "Enter your username: ";
        cin >> username;

        OrderNode* current = front;
        while (current) {
        	if (current->username == username) {
                
                cout << "Order Details:" << endl;
                cout << "Burger: " << current->burgername << ", Size: " << current->size << ", Quantity: "
                     << current->quantity << ", Beverage: " << current->bevname << ", Beverage Quantity: "
                     << current->bevquantity << ", Total Price: " << current->totalprice << endl;

                
                cout << "\nEdit Order:" << endl;
                cout << "1. Change Quantity" << endl;
                cout << "2. Change Size" << endl;
                cout << "3. Change Beverage Quantity" << endl;
                cout << "4. Cancel" << endl;

                int editChoice;
                cout << "Enter your choice: ";
                cin >> editChoice;

                switch (editChoice) {
                    case 1:
                        cout << "Enter new quantity: ";
                        cin >> current->quantity;
                        break;
                    case 2:
                        cout << "Enter new size [S/L]: ";
                        cin >> current->size;
                        break;
                    case 3:
                        cout << "Enter new beverage quantity: ";
                        cin >> current->bevquantity;
                        break;
                    case 4:
                        cout << "Order edit canceled." << endl;
                        return;
                    default:
                        cout << "Invalid choice. Order edit canceled." << endl;
                        return;
                }

                current->totalprice = (current->price * current->quantity) + (current->bevprice * current->bevquantity);

                cout << "Order successfully edited!" << endl;
                return;
            }
            current = current->next;
        }

        cout << "Order not found for username: " << username << endl;
    }
    ~OrderQueue() {
        OrderNode* current = front;
        while (current) {
            OrderNode* next = current->next;
            delete current;
            current = next;
        }
    }
    
    // Selection of orders based on total price
    
    void SelectionSortTotalp() {
    OrderNode* current = front;

    int orderCount = 1; 

    while (current) {
        OrderNode* minNode = current;
        OrderNode* temp = current->next;

        while (temp) {
            if (temp->totalprice < minNode->totalprice) {
                minNode = temp;
            }
            temp = temp->next;
        }

        
        swap(current->username, minNode->username);
        swap(current->burgername, minNode->burgername);
        swap(current->size, minNode->size);
        swap(current->quantity, minNode->quantity);
        swap(current->bevname, minNode->bevname);
        swap(current->bevquantity, minNode->bevquantity);
        swap(current->totalprice, minNode->totalprice);

      
        current->orderID = orderCount;

        
        current = current->next;

       
        orderCount++;
    }
}
    

    
    void displaySortedOrders() {
    SelectionSortTotalp();
    displayOrders();
    }
    
    // Deleting of orders by users using linked queue
    
    void cancelOrder(const string& username) {
    OrderNode* current = front;
    OrderNode* prev = NULL;

    while (current) {
        if (current->username == username) {
            if (prev) {
                prev->next = current->next;
            } else {
                front = current->next;
            }

            delete current;
            cout << "Order for user " << username << " has been canceled." << endl;
            saveOrdersToFile();
            return;
        }

        prev = current;
        current = current->next;
    }

    cout << "Order not found for user " << username << "." << endl;
    }

    	
};

struct User {
    static void registerUser(string& username);
    static void binarySearchProduct(const std::string& filename, const std::string& productName);
    friend string login();
};

// Binary search for user to search product

void User::binarySearchProduct(const std::string& filename, const std::string& productName) {
    ProductQueue productQueue;
    loadRecordsFromFile(filename, productQueue);

    Product* current = productQueue.front;
    int count = 0;

    
    while (current != nullptr) {
        count++;
        current = current->next;
    }

    
    current = productQueue.front;

    int first = 0;
    int last = count - 1;

    while (first <= last) {
    	
        int mid = (first + last) / 2;

        
        Product* midNode = current;
        for (int i = 0; i < mid; ++i) {
            midNode = midNode->next;
        }

        std::cout << "Comparing: '" << midNode->name << "' with '" << productName << "'\n";

        if (compareIgnoreCase(midNode->name, productName) == 0) {
            count = 1;
            current = midNode;
            break;
        } else if (compareIgnoreCase(productName, midNode->name) < 0) {
            last = mid - 1;
        } else {
            first = mid + 1;
        }
    }

    if (count == 1) {
        std::cout << "Search results in " << filename << " for query " << productName << ": ";
        std::cout << "\nThe fast food item is successfully found :";
        std::cout << "\nFast food product details:\n";
        std::cout << "ID: " << current->id << ", Name: " << current->name
                  << ", Price: RM" << current->price << ", Quantity: " << current->quantity << std::endl;

        std::cout << "\n\nPlease press any key to return to the main menu\n";
        system("pause");
        system("cls");
    } else {
        std::cout << "No matching fast food item is found in " << filename << " for query '" << productName << "'" << std::endl;
        std::cout << "\n\n\nPlease press any key to return to the main menu";
        system("pause");
        system("cls");
    }
}



//User registration

void User::registerUser(string& username) {
    system("cls");
    string enteredUsername, userpass, un, up;
    cout << "===================";
    cout << "\nEnter your name: ";
    cin >> enteredUsername;
    cout << "Enter password: ";
    cin >> userpass;
    cout << "===================" << endl;

    ifstream check("user.txt");
    while (check >> un >> up) {
        if (un == enteredUsername) {
            cout << "Please change to another username as it is already used by another user. " << endl;
            check.close();
            return;
        } else if (up == userpass) {
            cout << "Please change to another password as it is already used by another user." << endl;
            check.close();
            return;
        }
    }
    check.close();

    ofstream registerFile("user.txt", ios::app);
    registerFile << enteredUsername << ' ' << userpass << endl;
    system("cls");
    cout << "\nYou have successfully registered\n";

    username = enteredUsername;
}

//User login
string userlogin() {
    system("cls");
    string loginname, loginpass, x, y;
    int count = 0;
    cout << "Enter Name : ";
    cin >> loginname;
    cout << "Enter Password : ";
    cin >> loginpass;

    ifstream input("user.txt");
    while (input >> x >> y) {
        if (x == loginname && y == loginpass) {
            count = 1;
            system("cls");
        }
    }
    input.close();
    if (count == 1) {
        cout << "Login Successful. Welcome, " << loginname << endl;
        cin.ignore();
        cin.get();
        return loginname;
    } else {
        cout << "\nWrong username or password\n";
        return "";
    }
}


class Burger {
private:
    string username;
    string burgername, bevname;
    char size;
    int quantity;
    int bevquantity;
    float price, bevprice, totalprice, totalbev;

public:
    void menu() {
        system("cls");
        cout << "==NEW BURGER ORDER==       ============Price==========" << endl;
        cout << "1. Chicken Burger          S -> RM 5.50 , L -> RM 9.50" << endl;
        cout << "2. Fish Burger             S -> RM 5.50 , L -> RM 9.50" << endl;
    }
    
    
    void Select() {
    int select;

    do {
        cout << "Enter your choice: ";

       
        if (cin >> select) {
            switch (select) {
                case 1:
                    burgername = "Chicken Burger";
                    break;
                case 2:
                    burgername = "Fish Burger";
                    break;
                default:
                    cout << "Invalid input. Please enter 1 or 2." << endl;
            }
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 1 or 2." << endl;
        }

    } while (select != 1 && select != 2);
}

     void SizeQuantity() {
    	while (true) {
        cout << "Please select the size [S/L]: ";
        cin >> size;

        if (size == 'S' || size == 'L') {
            // Exit the loop if the input is valid
            break;
        } else {
            cout << "Invalid input. Please enter 'S' or 'L': ";
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
        }
    }
    
     while (true) {
        cout << "Please enter quantity: ";
        if (cin >> quantity && quantity > 0) {
            break;  // Valid input, exit the loop
        } else {
            cout << "Invalid input. Please enter a valid positive integer for quantity." << endl;
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
        }
    }

    // Calculate total price based on burgername, size, and quantity
    if (burgername == "Chicken Burger" && size == 'S') {
        price = 5.50;
    } else if (burgername == "Chicken Burger" && size == 'L') {
        price = 9.50;
    } else if (burgername == "Fish Burger" && size == 'S') {
        price = 5.50;
    } else if (burgername == "Fish Burger" && size == 'L') {
        price = 9.50;
    }

    totalprice = price * quantity;
    
 }
    

        
    void Beverage() {
        cout << "\n==BEVERAGE==                 ========Price=========" << endl;
        cout << "1.  Cola Cola                         RM 3.20 " << endl;
        cout << "2.  Pepsi                             RM 2.40 " << endl;
        cout << "3.  Mountain Dew                      RM 3.40" << endl;
        cout << "4.  7up                               RM 3.40" << endl;
    } 
   
   void BeverageSelect() {
    int select;

    while (true) {
        cout << "Enter your choice based on the number (1-4): ";

        if (cin >> select) {
            if (select >= 1 && select <= 4) {
                // Valid input, set bevname and exit the loop
                switch (select) {
                    case 1:
                        bevname = "Cola Cola";
                        break;
                    case 2:
                        bevname = "Pepsi";
                        break;
                    case 3:
                        bevname = "Mountain Dew";
                        break;
                    case 4:
                        bevname = "7up";
                        break;
                }
                break;
            } else {
                cout << "Invalid input. Please enter a valid choice (1-4)." << endl;
            }
        } else {
            cout << "Invalid input. Please enter a valid numeric choice." << endl;
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
        }
    }
}



   void setbevQuantity() {
    while (true) {
        cout << "Please enter beverage quantity: ";

        if (cin >> bevquantity && bevquantity > 0) {
            // Valid input, set bevprice and exit the loop
            if (bevname == "Cola Cola") {
                bevprice = 3.20;
            } else if (bevname == "Pepsi") {
                bevprice = 2.40;
            } else if (bevname == "Mountain Dew" || bevname == "7up") {
                bevprice = 3.40;
            }

            totalbev = bevprice * bevquantity;
            cout << "Total Beverage Price: RM" << totalbev << endl;
            break;
        } else {
            cout << "Invalid input. Please enter a valid positive integer for beverage quantity." << endl;
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
        }
    }
}

    void setUsername(const string& user) {
        username = user;
    }

    void display(OrderQueue& orderQueue) {
    	cout << "\n===================Order Details=====================";
        cout << "\nThank you for ordering, " << username << "!" << endl;
        cout << "Your burger details" << endl;
        cout << "Burger: " << burgername << endl;
        cout << "Size: " << size << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Beverage: " << bevname << endl;
        cout << "Beverage Quantity: " << bevquantity << endl;
        cout << "Total payment: " << totalprice + totalbev << endl;

        orderQueue.enqueue(username, burgername, size, quantity, bevname, bevquantity, totalprice + totalbev);
        orderQueue.saveOrdersToFile();
    }
    
    void displayProducts(const ProductQueue& productQueue) {
    Product* current = productQueue.front;
    cout << "\nCurrent Products:\n";
    cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(15) << "Price(RM)" << "Quantity\n";
    while (current != nullptr) {
        cout << left << setw(5) << current->id << setw(20) << current->name
             << "RM" << fixed << setprecision(2) << setw(15) << current->price
             << setw(10) << current->quantity << '\n';
        current = current->next;
    }
}

};

int main() {
	int choice; 
    User u;
    Burger b;
    OrderQueue orderQueue;
    Admin adminObject;
    string loggedInUser,searchProduct;;
    
    ProductQueue burgerQueue;
    ProductQueue beverageQueue;
    ProductQueue PQueue;
    
    loadRecordsFromFile("burger.txt", burgerQueue);
    loadRecordsFromFile("beverage.txt", beverageQueue);

    
        while (1) {
        cout << "\n**************************** BURGER ORDERING SYSTEM *****************************"<<endl;
        cout << "\n                               1. Admin"<<endl;
        cout << "\n                               2. Customer"<<endl;
        cout << "\n                               3. Exit"<<endl;
        cout << "\n                               Enter your choice: ";
        cin >> choice;
        cout << "\n*********************************************************************************"<<endl;
        

    switch (choice) {
    case 1 : 
    do {
        cout << "\n---------------------------------------------------------------------------------";
        cout << "\n                               ==============";
        cout << "\n                                *ADMIN MENU*";
        cout << "\n                               ==============";
     
        
        cout << "\n=>                             1. Register                   ----------"<<endl;
        cout << "\n=>                             2. Login                      -WELCOME!-"<<endl;
        cout << "\n=>                             3. Add Burger Records         - ADMIN  -"<<endl;
        cout << "\n=>                             4. Add Beverage Records       ----------"<<endl;
        cout << "\n=>                             5. Edit Product"<<endl;
        cout << "\n=>                             6. Sort Customer Orders"<<endl;
        cout << "\n=>                             7. Search Product"<<endl;
        cout << "\n=>                             8. Delete Product"<<endl;
        cout << "\n=>                             9. Logout";

        cout << "\n--------------------------------------------------------------------------------";
        cout << "\n\n\tEnter Your Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                Admin::registerAdmin(loggedInUser);
                if (!loggedInUser.empty()) {
                    cout << "You have successfully registered as: " << loggedInUser << endl;
                }
                break;
            case 2:
                loggedInUser = login();
                if (!loggedInUser.empty()) {
                    cout << "Login Successful. Welcome, " << loggedInUser << endl;
                }
                break;
            case 3:
                if (!loggedInUser.empty()) {
                    adminObject.addBurgerRecords(burgerQueue);
                    writeRecordsToFile("burger.txt", burgerQueue.front);
                    saveRecordsToFile("burger.txt", burgerQueue);
                } else {
                    cout << "Please login first." << endl;
                }
                break;
            case 4:
                if (!loggedInUser.empty()) {
                    adminObject.addBeverageRecords(beverageQueue);
                    writeRecordsToFile("beverage.txt", beverageQueue.front);
                    saveRecordsToFile("beverage.txt", beverageQueue);
                } else {
                    cout << "Please login first." << endl;
                }
                break;
            case 5:
                if (!loggedInUser.empty()) {
                    adminObject.editProduct(burgerQueue, beverageQueue);
                } else {
                    cout << "Please login first." << endl;
                }
                break;
            case 6:
                if (!loggedInUser.empty()) {
                 Admin::sortCustomerOrders();
                 } else {
                cout << "Please login first." << endl;
                  }
                 break;
            case 7:
            	    if (!loggedInUser.empty()) {
                    // Display the burger list
                    cout << "\n======================";
                    displayProducts(burgerQueue);
                    cout << "\n======================";

                    cout << "\n\nEnter the name of the product to search: ";
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                     getline(cin, searchProduct);

                    Admin::binarySearchProduct("burger.txt", searchProduct);
                    } else {
                    std::cout << "Please login first." << std::endl;
                    }
                    break;

            case 8:
                if (!loggedInUser.empty()) {
                adminObject.deleteProduct(burgerQueue);
                writeRecordsToFile("burger.txt", burgerQueue.front);
                } else {
                cout << "Please login first." << endl;
                 }
                 break;
  
            case 9:
            	exit(0);
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    } while (choice != 9);
    
    saveRecordsToFile("burger.txt", burgerQueue);
    saveRecordsToFile("beverage.txt", beverageQueue);

    clearQueue(burgerQueue.front, burgerQueue.rear);
    clearQueue(beverageQueue.front, beverageQueue.rear);

  	
    case 2 :
    do {
    	cout << "\n Welcome , user ";
        cout << "\n---------------------------------------------------------------------------------";
        cout << "\n                               ==============";
        cout << "\n                                 *USER MENU*";
        cout << "\n                               ==============";

        cout << "\n=>                             1. Register                                     ------------"<<endl;
        cout << "\n=>                             2. Login                                        -  WELCOME !"<<endl;
        cout << "\n=>                             3. Order Burger                                 -   USER   -"<<endl;
        cout << "\n=>                             4. Display Orders                               ------------"<<endl;
        cout << "\n=>                             5. Edit Order"<<endl;
        cout << "\n=>                             6. Sort Orders by total price"<<endl;
        cout << "\n=>                             7. Display Sorted Orders"<<endl;
        cout << "\n=>                             8. Cancel Order"<<endl;
        cout << "\n=>                             9. Search Product"<<endl;
        cout << "\n=>                            10. Logout";

        cout << "\n---------------------------------------------------------------------------------";
        cout << "\n\n\tEnter Your Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                u.registerUser(loggedInUser);
                cout << "You have successfully registered as: " << loggedInUser << endl;
                break;
            case 2:
                loggedInUser = userlogin();
                if (!loggedInUser.empty()) {
                    cout << "Login Successful. Welcome, " << loggedInUser << endl;
                }
                cin.ignore();
                cin.get();
                break;
            case 3:
                if (!loggedInUser.empty()) {
                    b.menu();
                    b.Select();
                    b.SizeQuantity();
                    b.Beverage();
                    b.BeverageSelect();
                    b.setbevQuantity();
                    b.setUsername(loggedInUser);
                    b.display(orderQueue);
                } else {
                    cout << "Please log in first before placing an order." << endl;
                }
                break;
            case 4:
                orderQueue.displayOrders();
                break;
            case 5:
                if (!loggedInUser.empty()) {
                    orderQueue.editOrder();
                } else {
                    cout << "Please log in first before editing an order." << endl;
                }
                break;
            
            case 6:
            	if (!loggedInUser.empty()) {
                    orderQueue.SelectionSortTotalp();
                    cout << "Orders sorted by total price in ascending order." << endl;
                } else {
                    cout << "Please log in first before sorting orders." << endl;
                }
                
                break;
                
            case 7:
                  if (!loggedInUser.empty()) {
                  orderQueue.displaySortedOrders();
                   } else {
                   cout << "Please log in first before displaying sorted orders." << endl;
                   }
                  break;
                  
            case 8: 
                  if (!loggedInUser.empty()) {
                  orderQueue.cancelOrder(loggedInUser);
                  } else {
                 cout << "Please log in first before canceling an order." << endl;
                   }
                  break;
                  
                  
            case 9:
            	    if (!loggedInUser.empty()) {
                    // Display the burger list
                    cout << "\n======================";
                    displayProducts(burgerQueue);
                    cout << "\n======================";

                    cout << "\n\nEnter the name of the product to search: ";
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    getline(cin, searchProduct);

                    User::binarySearchProduct("burger.txt", searchProduct);
                    } else {
                    std::cout << "Please login first." << std::endl;
                    }
                    break;
         

            case 10:
            	exit(0);
            	cout << "Exiting program. Goodbye!" << endl;
                break;
            	
            	    
            default:
                cout << "\n\n\tWrong choice!!!";
                cout << "\n\tPress any key to continue!!";
                cin.ignore();
                cin.get();
                break;
        }

    } while (choice != 10);
    
    break;
}
    
    return 0;
}
}




