#include <bits/stdc++.h>
#include <fstream>
#include <filesystem>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>

using namespace std;
namespace fs = std::filesystem;

// string k lowercase newar jonnno
string toLower(const string &str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

class ChildCompany {
public:
    string id;
    string password;

    ChildCompany(const string& id, const string& password) : id(id), password(password) {}

    bool isValid() {
        return id.length() == 5 && password.length() == 10;
    }

    string getFolderName() {
        return id + password; // id and pass eksath kore unique ekta folder banaor jonno
    }
};

class Employee {
public:
    string id;
    int age;
    string phoneNumber;
    string address;
    string password;

    Employee(const string& id, int age, const string& phoneNumber, const string& address, const string& password)
        : id(id), age(age), phoneNumber(phoneNumber), address(address), password(password) {}

    bool isValid() {
        return id.length() == 5 && password.length() == 10;
    }

    string getFolderName() {
        return id + password; // Unique folder name based on ID and password
    }
};

class CompanyManager {
public:
    static int childCompanyCount; // child comapny er count rakhar jonno

public:
    void registerChildCompany() {
        string id, password;

        cout << "Enter Child Company ID (5 characters): ";
        cin >> id;
        cout << "Enter Password (10 characters): ";
        cin >> password;

        ChildCompany company(id, password);
        if (!company.isValid()) {
            cout << "Invalid ID or Password length. Please try again.\n";
            sleep(3);
            system("cls");
            return;
        }

        string folderName = company.getFolderName();
        if (fs::exists(folderName)) {
            cout << "Company ID already exists. Choose another name.\n";
            sleep(3);
            system("cls");
            return;
        }

        fs::create_directory(folderName);
        ofstream profitFile(folderName + "/profit.txt");
        profitFile << ""; // profit txt file bananor por initial profit zero bananor jonno
        profitFile.close();

        childCompanyCount++; // ekta company bananor por jate company count increamate hoi
        cout << "Child Company registered successfully! Total Child Companies: " << childCompanyCount << "\n";
        sleep(3);
        system("cls");
    }

    void loginAsChildCompany() {
        string id, password;

        cout << "Enter Child Company ID: ";
        cin >> id;
        cout << "Enter Password: ";
        cin >> password;

        string folderName = id + password;
        if (!fs::exists(folderName)) {
            cout << "Incorrect ID or Password. Returning to main menu.\n";
            sleep(3);
            system("cls");
            return;
        }

        system("cls"); // Clear the console
        cout << "Login successful!\n";
        sleep(3);
        system("cls");
        int choice;
        do {
            cout << "1. Enter Investment and Return Value\n";
            cout << "2. Register Employee\n";
            cout << "3. Logout\n";
            cout << "Choose an option: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    system("cls");
                    enterInvestment(folderName);
                    break;
                case 2:
                    system("cls");
                    registerEmployee(folderName);
                    break;
                case 3:
                    system("cls");
                    cout << "Logging out...\n";
                    break;
                default:
                    system("cls");
                    cout << "Invalid option. Please try again.\n";
                    sleep(5);
            }
        } while (choice != 3);
    }

    void enterInvestment(const string& folderName) {
        double investment, returnValue;
        cout << "Enter Investment: ";
        cin >> investment;
        cout << "Enter Return Value: ";
        cin >> returnValue;

        double profit = returnValue - investment;

        ofstream profitFile(folderName + "/profit.txt", ios::app);
        if (profitFile.is_open()) {
            profitFile << profit << "\n"; // Append profit to the file
            profitFile.close();
            cout << "Profit recorded successfully!\n";
        } else {
            cerr << "Error: Could not open profit file for writing.\n";
        }
    }

    void registerEmployee(const string& folderName) {
        string id, password, phoneNumber, address;
        int age;

        cout << "Enter Employee ID (5 characters): ";
        cin >> id;

        // Validate ID length
        if (id.length() != 5) {
            cout << "Invalid ID length. Please enter exactly 5 characters.\n";
            return;
        }

        cout << "Enter Age: ";
        while (!(cin >> age) || age <= 0) {
            cout << "Invalid age. Please enter a positive integer: ";
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        }

        cout << "Enter Phone Number: ";
        cin >> phoneNumber;
        cout << "Enter Address: ";
        cin.ignore(); // Clear the newline character from the input buffer
        getline(cin, address);
        cout << "Enter Password (10 characters): ";
        cin >> password;

        // Validate password length
        if (password.length() != 10) {
            cout << "Invalid Password length. Please enter exactly 10 characters.\n";
            return;
        }

        Employee employee(id, age, phoneNumber, address, password);
        if (!employee.isValid()) {
            cout << "Invalid ID or Password length. Please try again.\n";
            return;
        }

        string employeeFolderName = employee.getFolderName();
        if (fs::exists(employeeFolderName)) {
            cout << "Employee ID already exists. Choose another name.\n";
            return;
        }

        // Create directory for the employee
        if (!fs::create_directory(employeeFolderName)) {
            cerr << "Error: Could not create directory for employee.\n";
            return;
        }

        // Write employee details to a file
        ofstream employeeFile(employeeFolderName + "/details.txt");
        if (employeeFile.is_open()) {
            employeeFile << age << "\n" << phoneNumber << "\n" << address << "\n" << password << "\n"; // Save employee details
            employeeFile.close();
            cout << "Employee registered successfully!\n";
        } else {
            cerr << "Error: Could not open employee file for writing.\n";
        }
    }

    void loginAsEmployee() {
        string id, password;

        cout << "Enter Employee ID: ";
        cin >> id;
        cout << "Enter Password: ";
        cin >> password;

        bool found = false;
        for (const auto& entry : fs::directory_iterator(".")) {
            if (entry.is_directory()) {
                string folderName = entry.path().filename().string();
                if (folderName.substr(0, 5) == id && folderName.substr(5, 10) == password) {
                    found = true;
                    cout << "Employee login successful!\n";
                    sleep(4);
                    system("cls");
                    modifyEmployeeDetails(folderName);
                    break;
                }
            }
        }

        if (!found) {
            cout << "Incorrect Employee ID or Password. Returning to main menu.\n";
            sleep(4);
            system("cls");
        }
    }

    void modifyEmployeeDetails(const string& folderName) {
        string phoneNumber, address;
        int age;

        cout << "Modify Employee Details:\n";
        cout << "Enter new Age: ";
        cin >> age;
        cout << "Enter new Phone Number: ";
        cin >> phoneNumber;
        cout << "Enter new Address: ";
        cin.ignore(); // Clear the newline character from the input buffer
        getline(cin, address);

        ofstream employeeFile(folderName + "/details.txt");
        if (employeeFile.is_open()) {
            employeeFile << age << "\n" << phoneNumber << "\n" << address << "\n"; // Save updated employee details
            employeeFile.close();
            cout << "Employee details updated successfully!\n";
        } else {
            cerr << "Error: Could not open employee file for writing.\n";
        }
    }

    void seeProfit() {
        double totalProfit = 0.0;

        for (const auto& entry : fs::directory_iterator(".")) {
            if (entry.is_directory()) {
                string folderName = entry.path().filename().string();
                ifstream profitFile(entry.path() / "profit.txt");
                double profit;
                while (profitFile >> profit) {
                    totalProfit += profit;
                }
                profitFile.close();
            }
        }

        cout << "Total Profit from all Child Companies: " << totalProfit << "\n";
    }
};

void welcome()

{



    printf("\n\n\n\n\n");

    printf("\n\t\t\t        *---------------------------------------------*");

    printf("\n\t\t\t        ||==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==||");

    printf("\n\t\t\t        ||                 WELCOME                   ||");

    printf("\n\t\t\t        ||              To Our Project               ||");

    printf("\n\t\t\t        ||        MultiCorp-Management-System        ||");

    printf("\n\t\t\t        ||                                           ||");

    printf("\n\t\t\t        ||==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==||");

    printf("\n\t\t\t        *---------------------------------------------*");

    printf("\n\n\t\t\t      C241080 HASAN || C241042 ALVIR || C241048 AMZAD");



    printf("\n\n\n\t\t\t                 Enter Any Key To Continue\n");

    getch();

    system("cls");





}



// Initialize static variable
int CompanyManager::childCompanyCount = 0;

int main() {
    CompanyManager manager;
    int mainChoice;

    do {
        welcome();

        // Display the count of child companies
        cout << "Total Child Companies: " << CompanyManager::childCompanyCount << "\n";
        cout << "1. Register a Child Company\n";
        cout << "2. Log in as Child Company\n";
        cout << "3. Log in as Employee\n";
        cout << "4. See Total Profit\n";
        cout << "5. Exit\n";
        cout << "Choose an option: ";
        cin >> mainChoice;

        switch (mainChoice) {
            case 1:
                manager.registerChildCompany();
                break;
            case 2:
                manager.loginAsChildCompany();
                break;
            case 3:
                manager.loginAsEmployee();
                break;
            case 4:
                manager.seeProfit();
                break;
            case 5:
                cout << "Exiting the program.\n";
                break;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    } while (mainChoice != 5);

    return 0;
}