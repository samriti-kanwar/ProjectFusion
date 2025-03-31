#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#define MAX_PRODUCTS 10
#define MAX_CART_SIZE 10

class Product {
protected:
    int id;
    string name;
    double price;

public:
    Product(){
        id=0;
        name="";
        price=0.0;
    } 

    Product(int pid, string pname, double pprice){
        id=pid;
        name=pname;
        price=pprice;
    }

    virtual void display() { 
        cout << "ID: " << id << " | Name: " << name << " | Price: Rs." << price << endl;
    }

    int getId() { return id; }
    string getName() { return name; }
    double getPrice() { return price; }
};

class Electronics : public Product {
public:
    Electronics(int pid, string pname, double pprice) : Product(pid, pname, pprice) {}

    void display(){
        cout << "[Electronics] ";
        Product::display();
    }
};

class Clothing : public Product {
public:
    Clothing(int pid, string pname, double pprice) : Product(pid, pname, pprice) {}

    void display(){
        cout << "[Clothing] ";
        Product::display();
    }
};

class Cart {
private:
    Product* items[MAX_CART_SIZE]; 
    int itemCount; 

public:
    Cart() : itemCount(0) {}

    void addProduct(Product* p) {
        if (itemCount < MAX_CART_SIZE) {
            items[itemCount++] = p;
            cout << p->getName() << " added to cart.\n";
            cout << "Total Bill: Rs." << getTotalBill() << endl; 
        } else {
            cout << "Cart is full!\n";
        }
    }

    void removeProduct(int pid) {
        for (int i = 0; i < itemCount; i++) {
            if (items[i]->getId() == pid) {
                cout << items[i]->getName() << " removed from cart.\n";
                for (int j = i; j < itemCount - 1; j++) {
                    items[j] = items[j + 1]; 
                }
                itemCount--;
                return;
            }
        }
        cout << "Product not found in cart!\n";
    }

    double getTotalBill() {
        double total = 0;
        for (int i = 0; i < itemCount; i++) {
            total += items[i]->getPrice();
        }
        return total;
    }

    friend void displayCart(Cart& c);
};

void displayCart(Cart& c) {
    cout << "\n--- Cart Items ---\n";
    if (c.itemCount == 0) {
        cout << "Cart is empty!\n";
        return;
    }
    for (int i = 0; i < c.itemCount; i++) {
        c.items[i]->display();
    }
    cout << "Total Bill: Rs." << c.getTotalBill() << endl; 
}

class User {
private:
    string username, password;

public:
    User(string uname, string pass) : username(uname), password(pass) {}

    void registerUser() {
        ofstream file("users.txt", ios::app);
        file << username << " " << password << "\n";
        file.close();
        cout << "Registration successful!\n";
    }

    bool loginUser() {
        ifstream file("users.txt");
        string uname, pass;
        while (file >> uname >> pass) {
            if (uname == username && pass == password) {
                cout << "Login successful!\n";
                return true;
            }
        }
        file.close();
        cout << "Invalid credentials!\n";
        return false;
    }
};

int main() {
    Product* products[MAX_PRODUCTS] = {
        new Electronics(1, "Laptop", 80000),
        new Electronics(2, "Smartphone", 15000),
        new Electronics(3, "Headphone", 1000),
        new Electronics(4, "Smartwatch", 2000),
        new Electronics(5, "Tablet", 25000),
        new Electronics(6, "Speakers", 4000),
        new Clothing(7, "Saree", 4500),
        new Clothing(8, "Trousers", 900),
        new Clothing(9, "Shorts", 700),
        new Clothing(10, "Hoodie", 2000)
    };

    Cart cart;
    int choice;

    while (true) {
        cout << "\n1. Register\n2. Login\n3. Browse Products\n4. Add to Cart\n5. View Cart\n6. Remove from Cart\n7. Place Order\n8. Exit\nEnter choice: ";
        cin >> choice;

        try {
            if (choice < 1 || choice > 8)
                throw invalid_argument("Invalid menu choice! Please enter between 1 and 8.");

            if (choice == 1) {
                string uname, pass;
                cout << "Enter username: ";
                cin >> uname;
                cout << "Enter password: ";
                cin >> pass;
                User u(uname, pass);
                u.registerUser();
            } else if (choice == 2) {
                string uname, pass;
                cout << "Enter username: ";
                cin >> uname;
                cout << "Enter password: ";
                cin >> pass;
                User u(uname, pass);
                if (!u.loginUser()) continue;
            } else if (choice == 3) {
                cout << "\n--- Available Products ---\n";
                for (int i = 0; i < 10; i++) {
                    products[i]->display();
                }
            } else if (choice == 4) {
                int pid;
                cout << "Enter Product ID to add to cart: ";
                cin >> pid;
                bool found = false;
                for (int i = 0; i < 10; i++) {
                    if (products[i]->getId() == pid) {
                        cart.addProduct(products[i]);
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Invalid Product ID!\n";
            } else if (choice == 5) {
                displayCart(cart);
            } else if (choice == 6) {
                int pid;
                cout << "Enter Product ID to remove from cart: ";
                cin >> pid;
                cart.removeProduct(pid);
            } else if (choice == 7) {
                displayCart(cart);
                if (cart.getTotalBill() == 0) {
                    cout << "Cart is empty. Cannot place an order!\n";
                } else {
                    string address;
                    cout << "Enter your shipping address: ";
                    cin.ignore();  
                    getline(cin, address);
                    cout << "Order placed successfully!\n";
                    cout << "Shipping to: " << address << endl;
                    cout << "Total Amount Paid: $" << cart.getTotalBill() << endl;
                    cart = Cart(); 
                }
            } else if (choice == 8) {
                cout << "Exiting program...\n";
                break;
            }
        } catch (const invalid_argument& e) {
            cout << "Exception: " << e.what() << endl;
        }
    }

    for (int i = 0; i < 4; i++) {
        delete products[i];
    }

    return 0;
}