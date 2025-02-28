//
// Created by Siew Yahuai on 24/02/2025.
//
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

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

void header() {
    // Print table header
    cout << left
         << setw(10) << "ID"
         << setw(30) << "Product Name"
         << setw(12) << "Unit Price"
         << setw(10) << "Quantity"
         << setw(20) << "Supplier"
         << endl;
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

map<string, int> countBySupplier(vector<Product> &data) {
    map<string, int> supplierCount;
    for (const Product &prod : data) {
        supplierCount[prod.supplier] += 1;
    }
    return supplierCount;
}

void displayBySupplier(const vector<Product> &data, const string &supplierName) {
    bool found = false;
    cout << "\nProducts from supplier: " << supplierName << endl;
    cout << left
          << setw(10) << "ID"
          << setw(30) << "Product Name"
          << setw(12) << "Unit Price"
          << setw(10) << "Quantity"
          << setw(20) << "Supplier"
          << endl;

    for (const Product &prod : data) {
        if (prod.supplier == supplierName) {
            display(prod);
            found = true;
        }
    }

    if (!found) {
        cout << "No supplier found" << endl;
    }
}

int findQuantityStats(vector<Product> &data, Product &highest, Product &lowest) {
    if (data.empty()) {
        cout << "No data found." << endl;
        return 0;
    }

    highest = data[0];
    lowest = data[0];

    int totalQuantity = 0;

    for (const Product &p : data) {
        totalQuantity += p.quantity;

        if (p.quantity > highest.quantity) {
            highest = p;
        }
        if (p.quantity < lowest.quantity) {
            lowest = p;
        }
    }
    return totalQuantity / static_cast<int>(data.size());
}

string toLower(const string &str) {
    string lowerStr = str;
    for (size_t i = 0; i < lowerStr.length(); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            lowerStr[i] = str[i] + ('a' - 'A');
        } else {
            lowerStr[i] = str[i];
        }
    }
    return lowerStr;
}

vector<Product> searchProductByName(vector<Product> &data, string &input) {
    vector<Product> result;
    string lowerStr = toLower(input);
    for (vector<Product>::const_iterator it = data.begin(); it != data.end(); ++it) {\
        string productName = toLower(it->product_name);
        if (productName.find(lowerStr) != string::npos) {
            result.push_back(*it);
        }
    }

    return result;
}

void sortPriceDesc(vector<Product> &data) {
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = i + 1; j < data.size(); j++) {
            if (data[i].unit_price < data[j].unit_price) {
                const Product temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}

void menu(vector<Product> &data) {
    int choice;
    int index;
    string userInput;
    map<string, int> supplierCount = countBySupplier(data);
    Product highest, lowest;
    int avgQuantity;
    vector<Product> matches;
    do {
        cout << "\n======================== Product Management System ========================" << endl;
        cout << "1. Display All Products" << endl;
        cout << "2. Search Products by Name" << endl;
        cout << "3. Count Supplier" << endl;
        cout << "4. Search Product by Supplier" << endl;
        cout << "5. Find Quantity Statistic" << endl;
        cout << "6. Filter Product by Input" << endl;
        cout << "7. Sort Product Descending by Unit Price" << endl;
        cout << "8. Exit" << endl;

        while (true) {
            cout << "\nPlease enter your choice: ";
            cin >> choice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\nInvalid input. Please enter a number between 1 and 8." << endl;
            } else if (choice < 1 || choice > 8) {
                cout << "\nInvalid choice. Please enter a number between 1 and 8." << endl;
            } else {
                cin.ignore();
                break;
            }
        }

        switch (choice) {
            case 1:
                cout << "\n============================== All Products ==============================" << endl;
                header();
                for (const Product &prod : data) {
                    display(prod);
                }
            break;
            case 2:
                cout << "Enter search name: ";
                getline(cin, userInput);
                index = findProductByName(data, userInput);
                if (index == -1) {
                    cout << "\nProduct not found!" << endl;
                } else {
                    cout << "\nProduct found at index: " << index+1 << endl;
                }
            break;
            case 3:
                cout << "\n============================= Supplier Count =============================" << endl;
                for (map<string, int>::iterator it = supplierCount.begin(); it != supplierCount.end(); ++it) {
                    cout << left
                         << setw(20)
                         << it->first
                         << ": "
                         << it->second
                         << " products"
                         << endl;
                }
            break;
            case 4:
                cout << "Enter supplier name to filter: ";
                getline(cin, userInput);

                displayBySupplier(data, userInput);
            break;
            case 5:
                avgQuantity = findQuantityStats(data, highest, lowest);
                cout << "\nHighest Quantity: " << endl;
                header();
                display(highest);
                cout << "\nLowest Quantity: " << endl;
                header();
                display(lowest);
                cout << "\nAverage Quantity of All Products: " << avgQuantity << endl;
            break;
            case 6:
                cout << "Please enter a product name: ";
                getline(cin, userInput);
                matches = searchProductByName(data, userInput);
                if (matches.empty()) {
                    cout << "\nNo matching products found." << endl;
                } else {
                    cout << "\nMatching Products:" << endl;
                    header();
                    for (const Product &p : matches) {
                        display(p);
                    }
                }
            break;
            case 7:
                sortPriceDesc(data);
                cout << "\n=================== Sorted Product Descending by Price ===================" << endl;
                header();
                for (const Product &prod : data) {
                    display(prod);
                }
            break;
            case 8:
                cout << "\nExiting Program..." << endl;
            break;
            default:
                cout << "\nInvalid choice. Please enter a valid choice." << endl;
        }

    } while (choice != 8);
}
int main() {
    vector<Product> v;
    load("data.csv", v);
    menu(v);
    return 0;


 }
