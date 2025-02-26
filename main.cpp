//
// Created by Siew Yahuai on 24/02/2025.
//
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Product {
    int product_id;
    string product_name;
    double unit_price;
    int quantity;
    string supplier;
};

void parseLine(const string &line, Product &prod) {
    string temp;
    stringstream ss(line);

    //get product id
    getline(ss, temp, ',');
    prod.product_id = stoi(temp);

    //get product name
    getline(ss, prod.product_name, ',');

    //get product unit price
    getline(ss, temp, ',');
    prod.unit_price = stod(temp);

    //get product quantity
    getline(ss, temp, ',');
    prod.quantity = stoi(temp);

    //get product supplier
    getline(ss,prod.supplier, ',');
}

void load(const string &fname, vector<Product> &data) {
    ifstream fin(fname);
    if (fin) {
        string line;
        while (getline(fin, line)) {
            Product prod;
            parseLine(line, prod);
            data.push_back(prod);
        }
        fin.close();
    } else {
        cout << "Unable to open file " << fname << endl;
    }
}

void display(const Product &prod) {
    cout << left
         << setw(10) << prod.product_id
         << setw(30) << prod.product_name
         << setw(12) << fixed << setprecision(2) << prod.unit_price
         << setw(10) << prod.quantity
         << setw(20) << prod.supplier
         << endl;
}

int findProductByName(vector<Product> &data, string &name) {
    for (int i = 0; i < data.size(); i++) {
        if (data[i].product_name == name) {
            return i;
        }
    } return -1;
}

int main() {
    vector<Product> v;
    load("data.csv", v);

    // Print table header
    cout << left
         << setw(10) << "ID"
         << setw(30) << "Product Name"
         << setw(12) << "Unit Price"
         << setw(10) << "Quantity"
         << setw(20) << "Supplier"
         << endl;

    // for (const Product &p : v) {
    //     display(p);
    // }

    string searchName;
    cout << "Enter search name: ";
    getline(cin, searchName);

    int index = findProductByName(v, searchName);
    if (index == -1) {
        cout << "Product not found!" << endl;
    } else {
        cout << "Product found at index: " << index+1 << endl;
    }
    return 0;
 }
