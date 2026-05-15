#include "../Header.h"
#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>

using namespace std;

TEST(Order_Add, AddProductTest) {
    Order order;

    order.addProduct("Milk", 2, 50);

    EXPECT_EQ(order.getSize(), 1);
    EXPECT_DOUBLE_EQ(order.getTotal(), 100);
}

TEST(Order_Add, AddSameProductTest) {
    Order order;

    order.addProduct("Milk", 2, 50);
    order.addProduct("Milk", 3, 50);

    EXPECT_EQ(order.getSize(), 1);
    EXPECT_DOUBLE_EQ(order.getTotal(), 250);
}

TEST(Order_Remove, RemoveProductTest) {
    Order order;

    order.addProduct("Milk", 2, 50);
    order.addProduct("Bread", 1, 30);

    order.removeByIndex(0);

    EXPECT_EQ(order.getSize(), 1);
    EXPECT_DOUBLE_EQ(order.getTotal(), 30);
}

TEST(Order_Edit, ChangeQuantityTest) {
    Order order;

    order.addProduct("Apple", 2, 40);

    order.setQuantityByIndex(0, 5);

    EXPECT_DOUBLE_EQ(order.getTotal(), 200);
}

TEST(Order_Check, EmptyCartTest) {
    Order order;

    EXPECT_TRUE(order.isEmpty());
}

TEST(Order_Check, HasDateTest) {
    Order order;

    order.setDeliveryDate("20.05.2026 14:30");

    EXPECT_TRUE(order.hasDate());
}

TEST(Order_Check, HasAddressTest) {
    Order order;

    order.setDeliveryAddress("Lviv");

    EXPECT_TRUE(order.hasAddress());
}

TEST(Order_Status, StatusToStringTest) {
    EXPECT_EQ(statusToString(PAID), "Paid");
    EXPECT_EQ(statusToString(CANCELED), "Canceled");
}

TEST(Order_Date, InvalidDateTest) {
    EXPECT_FALSE(validDate(32, 15, 2020, 30, 90));
}

TEST(Order_Date, FutureDateTest) {
    tm now = getCurrentDate();

    int d = now.tm_mday;
    int m = now.tm_mon + 1;
    int y = now.tm_year + 1900;
    int h = now.tm_hour + 1;

    EXPECT_TRUE(validDate(d, m, y, h, 30));
}

TEST(Order_File, GenerateFileNameTest) {
    string fileName = generateFileName();

    EXPECT_NE(fileName.find("order_"), string::npos);
    EXPECT_NE(fileName.find(".txt"), string::npos);
}

TEST(Order_File, SaveToFileTest) {
    Order order;

    order.addProduct("Milk", 2, 50);
    order.setDeliveryDate("20.05.2026 14:30");
    order.setDeliveryAddress("Lviv");
    order.setStatus(PAID);

    string filename = "test_order.txt";

    order.saveToFile(filename);

    ifstream fin(filename);

    ASSERT_TRUE(fin.good());

    string content((istreambuf_iterator<char>(fin)),
        istreambuf_iterator<char>());

    EXPECT_NE(content.find("Milk"), string::npos);
    EXPECT_NE(content.find("Paid"), string::npos);
    EXPECT_NE(content.find("Lviv"), string::npos);

    fin.close();

    remove(filename.c_str());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}