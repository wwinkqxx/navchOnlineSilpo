#include "Header.h"

string statusToString(OrderStatus s) {
    switch (s) {
    case NOT_PAID: return "Not Paid";
    case PAID: return "Paid";
    case CANCELED: return "Canceled";
    case COMPLETED: return "Completed";
    }
    return "";
}

Order::Order() {
    status = NOT_PAID;
    deliveryDate = "";
    deliveryAddress = "";
}

void Order::addProduct(string name, int quantity, double price) {
    for (auto& p : products) {
        if (p.name == name) {
            p.quantity += quantity;
            return;
        }
    }
    products.push_back({ name, quantity, price });
}

void Order::removeByIndex(int i) {
    if (i >= 0 && i < products.size())
        products.erase(products.begin() + i);
}

void Order::setQuantityByIndex(int i, int q) {
    if (i >= 0 && i < products.size())
        products[i].quantity = q;
}

void Order::setStatus(OrderStatus s) {
    status = s;
}

void Order::setDeliveryDate(const string& d) {
    deliveryDate = d;
}

void Order::setDeliveryAddress(const string& addr) {
    deliveryAddress = addr;
}

bool Order::hasDate() { return deliveryDate != ""; }
bool Order::hasAddress() { return deliveryAddress != ""; }

double Order::getTotal() const {
    double sum = 0;
    for (auto& p : products)
        sum += p.price * p.quantity;
    return sum;
}

void Order::printCart() const {
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

bool Order::isEmpty() { return products.empty(); }
int Order::getSize() { return products.size(); }

void Order::saveToFile(const string& fileName) {
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
