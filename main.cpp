#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// Structure to store room information
struct Room {
    int roomNo;
    string type;
    double pricePerDay;
    bool isBooked;
};

// Structure to store customer and booking details
struct Customer {
    string name;
    string phone;
    int roomNo;
    string checkIn;
    string checkOut;
    int days;
    double totalBill;
};

// Initialize default rooms
void initializeRooms(vector<Room>& rooms) {
    rooms = {
        {101, "Single", 1500.0, false},
        {102, "Double", 2500.0, false},
        {103, "Deluxe", 4000.0, false},
        {104, "Suite", 7000.0, false}
    };
}

// 2. View Available Rooms
void viewAvailableRooms(const vector<Room>& rooms) {
    cout << "\n==================================================\n";
    cout << "                 AVAILABLE ROOMS                  \n";
    cout << "==================================================\n";
    cout << left << setw(10) << "ROOM NO." << setw(15) << "TYPE" << setw(15) << "PRICE/DAY" << setw(10) << "STATUS" << endl;
    cout << "--------------------------------------------------\n";
    for (const auto& room : rooms) {
        cout << left << setw(10) << room.roomNo 
             << setw(15) << room.type 
             << "₹" << setw(14) << room.pricePerDay 
             << setw(10) << (room.isBooked ? "Booked" : "Available") << endl;
    }
    cout << "==================================================\n";
}

// 1. Book Room
void bookRoom(vector<Room>& rooms) {
    int roomNo;
    cout << "\nEnter Room Number to Book: ";
    cin >> roomNo;

    int index = -1;
    for (size_t i = 0; i < rooms.size(); ++i) {
        if (rooms[i].roomNo == roomNo) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "[Error] Room number not found!\n";
        return;
    }

    if (rooms[index].isBooked) {
        cout << "[Error] Room is already booked!\n";
        return;
    }

    Customer cust;
    cust.roomNo = roomNo;
    
    cout << "Enter Customer Name: ";
    cin.ignore();
    getline(cin, cust.name);
    
    cout << "Enter Phone Number: ";
    cin >> cust.phone;
    
    cout << "Enter Check-in Date (DD/MM/YYYY): ";
    cin >> cust.checkIn;
    
    cout << "Enter Check-out Date (DD/MM/YYYY): ";
    cin >> cust.checkOut;
    
    cout << "Enter Number of Days: ";
    cin >> cust.days;

    cust.totalBill = cust.days * rooms[index].pricePerDay;
    rooms[index].isBooked = true;

    // Save booking and customer details to file (File Handling)
    ofstream outFile("customers.txt", ios::app);
    if (outFile.is_open()) {
        outFile << cust.roomNo << "|" << cust.name << "|" << cust.phone << "|" 
                << cust.checkIn << "|" << cust.checkOut << "|" << cust.days << "|" << cust.totalBill << "\n";
        outFile.close();
        cout << "\n[Success] Room booked successfully!\n";
    } else {
        cout << "[Error] Unable to save booking details to file.\n";
    }
}

// 3. Customer Details
void viewCustomerDetails() {
    ifstream inFile("customers.txt");
    if (!inFile.is_open()) {
        cout << "\n[Info] No customer records found yet.\n";
        return;
    }

    cout << "\n==================================================\n";
    cout << "                 CUSTOMER DETAILS                 \n";
    cout << "==================================================\n";

    string line;
    bool found = false;
    while (getline(inFile, line)) {
        found = true;
        size_t pos = 0;
        string token;
        vector<string> fields;
        while ((pos = line.find('|')) != string::npos) {
            token = line.substr(0, pos);
            fields.push_back(token);
            line.erase(0, pos + 1);
        }
        fields.push_back(line); // Remaining part is totalBill

        if (fields.size() >= 7) {
            cout << "Room No     : " << fields[0] << endl;
            cout << "Name        : " << fields[1] << endl;
            cout << "Phone       : " << fields[2] << endl;
            cout << "Check-in    : " << fields[3] << endl;
            cout << "Check-out   : " << fields[4] << endl;
            cout << "Days Stayed : " << fields[5] << endl;
            cout << "Total Bill  : ₹" << fields[6] << endl;
            cout << "--------------------------------------------------\n";
        }
    }
    inFile.close();

    if (!found) {
        cout << "[Info] No customer records available.\n";
    }
    cout << "==================================================\n";
}

// 4. Generate Bill
void generateBill() {
    int roomNo;
    cout << "\nEnter Room Number to Generate Bill: ";
    cin >> roomNo;

    ifstream inFile("customers.txt");
    if (!inFile.is_open()) {
        cout << "\n[Error] No customer records found!\n";
        return;
    }

    string line;
    bool found = false;
    while (getline(inFile, line)) {
        size_t pos = line.find('|');
        if (pos != string::npos) {
            int rNo = stoi(line.substr(0, pos));
            if (rNo == roomNo) {
                found = true;
                vector<string> fields;
                string tempLine = line;
                while ((pos = tempLine.find('|')) != string::npos) {
                    fields.push_back(tempLine.substr(0, pos));
                    tempLine.erase(0, pos + 1);
                }
                fields.push_back(tempLine);

                cout << "\n==================================================\n";
                cout << "                   BILL SUMMARY                   \n";
                cout << "==================================================\n";
                cout << "Room Number   : " << fields[0] << endl;
                cout << "Customer Name : " << fields[1] << endl;
                cout << "Phone Number  : " << fields[2] << endl;
                cout << "Check-in      : " << fields[3] << endl;
                cout << "Check-out     : " << fields[4] << endl;
                cout << "Days Stayed   : " << fields[5] << endl;
                cout << "Total Amount  : ₹" << fields[6] << endl;
                cout << "==================================================\n";
                break;
            }
        }
    }
    inFile.close();

    if (!found) {
        cout << "\n[Error] No active booking found for Room No " << roomNo << "!\n";
    }
}

// Main Menu Loop
int main() {
    vector<Room> rooms;
    initializeRooms(rooms);

    int choice;
    do {
        cout << "\n==================================================\n";
        cout << "             HOTEL MANAGEMENT SYSTEM              \n";
        cout << "==================================================\n";
        cout << " 1. Book Room\n";
        cout << " 2. View Available Rooms\n";
        cout << " 3. Customer Details\n";
        cout << " 4. Generate Bill\n";
        cout << " 5. Exit System\n";
        cout << "==================================================\n";
        cout << "Enter your choice (1-5): ";
        cin >> choice;

        switch (choice) {
            case 1:
                bookRoom(rooms);
                break;
            case 2:
                viewAvailableRooms(rooms);
                break;
            case 3:
                viewCustomerDetails();
                break;
            case 4:
                generateBill();
                break;
            case 5:
                cout << "\nThank you for using the Hotel Management System!\n";
                break;
            default:
                cout << "\n[Error] Invalid choice! Please enter a number between 1 and 5.\n";
        }
    } while (choice != 5);

    return 0;
}