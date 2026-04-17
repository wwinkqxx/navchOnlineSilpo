#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;

enum OrderStatus {
    NOT_PAID,
    PAID,
    CANCELED,
    COMPLETED
};

string statusToString(OrderStatus s) {
    switch (s) {
    case NOT_PAID: return "Not Paid";
    case PAID: return "Paid";
    case CANCELED: return "Canceled";
    case COMPLETED: return "Completed";
    }
    return "";
}

struct Product {
    string name;
    int quantity;
    double price;
};

class Order {
private:
    vector<Product> products;
    OrderStatus status;
    string deliveryDate;
    string deliveryAddress;

public:
    Order() {
        status = NOT_PAID;
        deliveryDate = "";
        deliveryAddress = "";
    }

    void addProduct(string name, int quantity, double price) {
        for (auto& p : products) {
            if (p.name == name) {
                p.quantity += quantity;
                return;
            }
        }
        products.push_back({ name, quantity, price });
    }

    void removeByIndex(int i) {
        if (i >= 0 && i < products.size())
            products.erase(products.begin() + i);
    }

    void setQuantityByIndex(int i, int q) {
        if (i >= 0 && i < products.size())
            products[i].quantity = q;
    }

    void setStatus(OrderStatus s) {
        status = s;
    }

    void setDeliveryDate(const string& d) {
        deliveryDate = d;
    }

    void setDeliveryAddress(const string& addr) {
        deliveryAddress = addr;
    }

    bool hasDate() { return deliveryDate != ""; }
    bool hasAddress() { return deliveryAddress != ""; }

    double getTotal() const {
        double sum = 0;
        for (auto& p : products)
            sum += p.price * p.quantity;
        return sum;
    }

    void printCart() const {
        cout << "\n~~~ CART ~~~\n";
        if (products.empty()) {
            cout << "Cart is empty!\n";
            return;
        }
        for (int i = 0; i < products.size(); i++) {
            cout << i + 1 << ". " << products[i].name
                << " x" << products[i].quantity
                << " = " << products[i].price * products[i].quantity
                << " UAH\n";
        }
        cout << "TOTAL: " << getTotal() << " UAH\n";
    }

    bool isEmpty() { return products.empty(); }
    int getSize() { return products.size(); }

    void saveToFile(const string& fileName) {
        ofstream file(fileName);
        file << "STATUS: " << statusToString(status) << endl;
        file << "DATE & TIME: " << deliveryDate << endl;
        file << "ADDRESS: " << deliveryAddress << endl;
        file << "~~~~~~~~~~~~~~~~~~~~\n";
        for (auto& p : products) {
            file << p.name << " x" << p.quantity
                << " = " << p.price * p.quantity << " UAH\n";
        }
        file << "TOTAL: " << getTotal() << " UAH\n";
    }
};

bool inputInt(int& x) {
    cin >> x;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

tm getCurrentDate() {
    time_t t = time(0);
    tm now;
    localtime_s(&now, &t);
    return now;
}

bool isFutureDateTime(int d, int m, int y, int h, int min) {
    tm now = getCurrentDate();
    int cd = now.tm_mday, cm = now.tm_mon + 1, cy = now.tm_year + 1900;
    int ch = now.tm_hour, cmin = now.tm_min + 30;

    if (y < cy) return false;
    if (y > cy) return true;
    if (m < cm) return false;
    if (m > cm) return true;
    if (d < cd) return false;
    if (d > cd) return true;
    if (h < ch) return false;
    if (h > ch) return true;
    return min >= cmin;
}

bool validDate(int d, int m, int y, int h, int min) {
    if (m < 1 || m > 12) return false;
    int days = (m == 2 ? 28 : (m == 4 || m == 6 || m == 9 || m == 11 ? 30 : 31));
    if (d < 1 || d > days) return false;
    if (h < 0 || h > 23) return false;
    if (min < 0 || min > 59) return false;
    return isFutureDateTime(d, m, y, h, min);
}

string generateFileName() {
    time_t t = time(0);
    tm now;
    localtime_s(&now, &t);

    char buffer[50];
    strftime(buffer, sizeof(buffer),
        "order_%Y-%m-%d_%H-%M-%S.txt", &now);
    return string(buffer);
}

int main() {
    srand(time(0));

    vector<Product> store = {
    {"Milk",0,50},
    {"Bread",0,30},
    {"Apple",0,40},
    {"Cheese",0,120},
    {"Banana",0,35},
    {"Orange",0,45},
    {"Chicken",0,150},
    {"Beef",0,220},
    {"Fish",0,180},
    {"Eggs",0,70},
    {"Butter",0,90},
    {"Yogurt",0,60},
    {"Tomato",0,55},
    {"Cucumber",0,50},
    {"Potato",0,25},
    {"Onion",0,20},
    {"Rice",0,80},
    {"Pasta",0,75},
    {"Juice",0,65},
    {"Water",0,25},
    {"Chocolate",0,95}
    };

    Order order;
    int choice;

    while (true) {
        cout << "\n~~~ SILPO CASH REGISTER ~~~\n";
        cout << "1. Add product\n2. Remove product\n3. Change quantity\n4. Set delivery date/time\n5. Pay\n6. Cancel\n7. Show cart\n0. Exit\nChoice: ";
        if (!inputInt(choice)) { cout << "Invalid input!\n"; continue; }

        if (choice == 1) {
            cout << "\nPRODUCTS:\n";
            for (int i = 0;i < store.size();i++)
                cout << i + 1 << ". " << store[i].name << " - " << store[i].price << " UAH\n";

            int id; cout << "Choose product: ";
            if (!inputInt(id) || id<1 || id>store.size()) { cout << "Invalid choice!\n"; continue; }

            int q;
            while (true) {
                cout << "Enter quantity: ";
                if (!inputInt(q) || q <= 0) cout << "Invalid quantity!\n"; else break;
            }
            order.addProduct(store[id - 1].name, q, store[id - 1].price);
        }
        else if (choice == 2) {
            order.printCart();
            if (order.isEmpty()) continue;
            int i; cout << "Select item number: ";
            if (!inputInt(i) || i<1 || i>order.getSize()) { cout << "Invalid choice!\n"; continue; }
            order.removeByIndex(i - 1);
        }
        else if (choice == 3) {
            order.printCart();
            if (order.isEmpty()) continue;
            int i; cout << "Select item number: ";
            if (!inputInt(i) || i<1 || i>order.getSize()) { cout << "Invalid choice!\n"; continue; }
            int q; while (true) {
                cout << "Enter new quantity: ";
                if (!inputInt(q) || q <= 0) cout << "Invalid quantity!\n"; else break;
            }
            order.setQuantityByIndex(i - 1, q);
        }
        else if (choice == 4) {
            if (order.isEmpty()) {
                cout << "Cart is empty!\n";
                continue;
            }
            int d, m, y, h, min;
            while (true) {
                cout << "Enter delivery date and time (day month year hour minute): ";
                if (!inputInt(d) || !inputInt(m) || !inputInt(y) || !inputInt(h) || !inputInt(min) ||
                    !validDate(d, m, y, h, min))
                    cout << "Invalid date/time! Only future allowed.\n";
                else break;
            }
            order.setDeliveryDate(to_string(d) + "." + to_string(m) + "." + to_string(y) + " " +
                to_string(h) + ":" + (min < 10 ? "0" : "") + to_string(min));
            cin.ignore();
            string addr;
            cout << "Enter delivery address: ";
            getline(cin, addr);
            order.setDeliveryAddress(addr);
        }
        else if (choice == 5) {
            if (order.isEmpty()) {
                cout << "Cart is empty!\n";
                continue;
            }
            if (!order.hasDate()) { cout << "Set delivery date first!\n"; continue; }
            if (!order.hasAddress()) { cout << "Set delivery address first!\n"; continue; }

            cout << "Processing payment...\n";
            int r = rand() % 100;
            if (r < 70) {
                cout << "Payment successful!\n";
                order.setStatus(PAID);
                order.saveToFile(generateFileName());

                cout << "Delivery in progress"; cout.flush();
                for (int i = 0;i < 5;i++) { this_thread::sleep_for(chrono::seconds(1)); cout << "."; cout.flush(); }
                cout << "\nDelivery completed!\n";
                order.setStatus(COMPLETED);
                order.saveToFile(generateFileName());
            }
            else {
                cout << "Insufficient funds!\n";
                order.setStatus(NOT_PAID);
                order.saveToFile(generateFileName());
            }
        }
        else if (choice == 6) {
            if (order.isEmpty()) {
                cout << "Cart is empty!\n";
                continue;
            }
            order.setStatus(CANCELED);
            cout << "Order canceled!\n";
            order.saveToFile(generateFileName());
            break;
        }
        else if (choice == 7) order.printCart();
        else if (choice == 0) break;
        else cout << "Invalid choice!\n";
    }

    return 0;
}
