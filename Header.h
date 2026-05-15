#pragma once
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

string statusToString(OrderStatus s);

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
    Order();

    void addProduct(string name, int quantity, double price);
    void removeByIndex(int i);
    void setQuantityByIndex(int i, int q);
    void setStatus(OrderStatus s);
    void setDeliveryDate(const string& d);
    void setDeliveryAddress(const string& addr);

    bool hasDate();
    bool hasAddress();
    double getTotal() const;
    void printCart() const;
    bool isEmpty();
    int getSize();
    void saveToFile(const string& fileName);
};

bool inputInt(int& x);
tm getCurrentDate();
bool isFutureDateTime(int d, int m, int y, int h, int min);
bool validDate(int d, int m, int y, int h, int min);
string generateFileName();
