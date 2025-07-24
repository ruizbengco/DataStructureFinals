//Ruiz Christian V. Bengco
//Abelardo Pascual IV
//Exzikel Pangan
#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <cstdlib>
#include <vector>
namespace fs = std::filesystem;

using namespace std;

const string ACCOUNTS_FILE = "bank_data/bank_accounts.txt";
const string TEMP_FILE = "bank_data/temp.txt";
const string TRANSACTIONS_FILE = "bank_data/transactions.txt";

struct Account {
    string id, name, password, pin, address, phoneNumber;
    float balance;
    int currentUserIndex = -1;

    string to_line() const {
        stringstream ss;
        ss << id << "|" << name << "|" << password << "|" << pin << "|" << address << "|" << phoneNumber << "|" << fixed << setprecision(2) << balance;
        return ss.str();
    }
};

class bank {

private:
    string id, name, password, pin, address, phoneNumber;
    float balance;
    int currentUserIndex = -1;
    std::vector<Account> accounts;

public:
    void menu(); // Main menu

    void bank_system(); // Bank functions
    void user_registration(); // Register new user
    void user_login(); // Login For users
    void user_menu(); // User Menu
    void deposit(); // Under User Menu
    void withdraw(); // Under User Menu
    void transfer(); // Under User Menu
    void payment(); // Under User Menu

    void atm_system(); // ATM functions

    void bank_management(); // Bank Management Functions
    void search_accounts();
    void show_all_accounts();
    void show_all_transactions();
    void account_management_menu();
    void edit_account_details();
    void close_account();


    string getHiddenInput(); // For password masking
};
// Function to mask password input using '*'
string bank::getHiddenInput() {
    string password;
    char ch;
    while ((ch = _getch()) != 13) { // Enter key
        if (ch == 8 && !password.empty()) { // Backspace
            password.pop_back();
            cout << "\b \b";
        } else if (isprint(ch)) {
            password += ch;
            cout << '*';
        }
    }
    return password;
}
// Main Menu
void bank::menu() {
    int choice;
    string bankAccount, bankPassword, bankPincode;

    while (true) {
        cout << "\n\n\t\t\t--- CONTROL PANEL ---";
        cout << "\n 1. Bank System";
        cout << "\n 2. ATM System";
        cout << "\n 3. Bank Management";
        cout << "\n 4. Exit";
        cout << "\n Please Enter Your Choice: ";
        // Input validation
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n\n Invalid input! Please enter a number.";
            cout << "\n Press Enter to try again...";
            cin.ignore();
            continue;
        }

        switch (choice) {
            case 1:
                bank_system();
                break;

            case 2:
                atm_system();
                break;

            case 3:
                bank_management();
                break;

            case 4:
                cout << "\n\n Exiting program...";
                Sleep(1000);
                exit(0);

            default:
                cout << "\n\n Invalid Choice!";
                cout << "\n Press Enter to try again...";
                cin.ignore();
                cin.get();
        }
    }
}
// Bank-side menu
void bank::bank_system() {
    int choice;
    while (true) {
        cout << "\n\n\t\t\t--- BANK SYSTEM ---\n";
        cout << "\n 1. User Registration";
        cout << "\n 2. User Login";
        cout << "\n 3. Back";
        cout << "\n Please Enter Your Choice: ";
        // Input validation
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n\n Invalid input! Please enter a number.";
            cout << "\n Press Enter to try again...";
            cin.ignore();
            cin.get();
            continue;
        }

        switch (choice) {
            case 1:
                user_registration();
                break;
            case 2:
                user_login();
                break;
            case 3:
                menu();
                break;
            default:
                cout << "\n\n Invalid Choice!";
        }
        cout << "\n\n Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
}
// Register a new bank user
void bank::user_registration() {
    vector<Account> accounts;
    Account newAccount;
    bool exists = false;

    cout << "\n\n--- User Registration ---";

    // Input user details
    cout << "\n User ID: ";
    cin >> newAccount.id;

    cout << " Name: ";
    cin.ignore(); // Clear leftover newline
    getline(cin, newAccount.name);

    cout << " Password: ";
    cin >> newAccount.password;

    while (true) {
        cout << " Pincode (MAX 4 Digits): ";
        cin >> newAccount.pin;
        if (newAccount.pin.length() == 4 && all_of(newAccount.pin.begin(), newAccount.pin.end(), ::isdigit)) {
            break;
        } else {
            cout << " Invalid PIN. Please enter exactly 4 digits.\n";
        }
    }

    cout << " Address: ";
    cin.ignore();
    getline(cin, newAccount.address);

    cout << " Phone Number: ";
    cin >> newAccount.phoneNumber;

    // Read existing users
    fstream file(ACCOUNTS_FILE, ios::in);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            Account acc;
            string balanceStr;

            getline(ss, acc.id, '|');
            getline(ss, acc.name, '|');
            getline(ss, acc.password, '|');
            getline(ss, acc.pin, '|');
            getline(ss, acc.address, '|');
            getline(ss, acc.phoneNumber, '|');
            getline(ss, balanceStr, '|');

            acc.balance = stof(balanceStr);

            if (acc.id == newAccount.id) {
                exists = true;
                break;
            }

            accounts.push_back(acc);
        }
        file.close();
    }

    if (exists) {
        cout << "\n\n User ID already exists! Please try again with a different ID.\n";
        return;
    }

    // Add new account to vector
    newAccount.balance = 0.00f;
    accounts.push_back(newAccount);

    // Save all accounts to file
    ofstream outFile(ACCOUNTS_FILE);
    if (outFile.is_open()) {
        for (const auto& acc : accounts) {
            outFile << acc.id << "|" << acc.name << "|" << acc.password << "|" << acc.pin << "|" << acc.address << "|" << acc.phoneNumber << "|" << fixed << setprecision(2) << acc.balance << endl;
        }
        outFile.close();
        cout << "\n\n User Registered Successfully.\n";
    } else {
        cout << "\n\n Error opening file for writing.\n";
    }
}

void bank::user_login() {
    cout << "\n\n--- User Login ---";

    string l_id, inputName, inputPassword, inputPin;
    bool found = false;

    ifstream file(ACCOUNTS_FILE);
    if (!file.is_open()) {
        cout << "\n\n Error opening file.\n";
        return;
    }

    // Load all accounts into the class-level vector
    accounts.clear();
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Account acc;
        string balanceStr;
        getline(ss, acc.id, '|');
        getline(ss, acc.name, '|');
        getline(ss, acc.password, '|');
        getline(ss, acc.pin, '|');
        getline(ss, acc.address, '|');
        getline(ss, acc.phoneNumber, '|');
        getline(ss, balanceStr, '|');
        acc.balance = stof(balanceStr);
        accounts.push_back(acc);
    }
    file.close();

    // Prompt for login
    cout << "\n\n Enter User ID: ";
    cin >> l_id;
    cin.ignore();

    for (size_t i = 0; i < accounts.size(); ++i) {
        const auto& acc = accounts[i];
        if (acc.id == l_id) {
            found = true;

            cout << " Enter User Name: ";
            getline(cin, inputName);

            cout << " Enter User Password: ";
            inputPassword = getHiddenInput();
            cout << endl;

            cout << " Enter User PIN: ";
            inputPin = getHiddenInput();
            cout << endl;

            // Check credentials
            if (inputName == acc.name &&
                inputPassword == acc.password &&
                inputPin == acc.pin) {
                cout << "\n\n Login Successful!";

                // Save index of logged-in user
                currentUserIndex = static_cast<int>(i);

                // Proceed using currentUserIndex
                user_menu();
            } else {
                cout << "\n\n Login Failed! Incorrect credentials.\n";
            }

            break;
        }
    }

    if (!found) {
        cout << "\n\n User ID not found.\n";
    }
}

void bank::user_menu() {
    if (currentUserIndex == -1 || currentUserIndex >= accounts.size()) {
        cout << "\n\n No user is currently logged in.";
        return;
    }

    const Account& current = accounts[currentUserIndex];

    cout << "\n\n--- User Menu ---";
    int choice;

    while (true) {
        // Use current account
        cout << "\n\n Account ID    : " << current.id;
        cout << "\n Name            : " << current.name;
        cout << "\n Address         : " << current.address;
        cout << "\n Phone Number    : " << current.phoneNumber;
        cout << "\n Current Balance : PHP" << fixed << setprecision(2) << current.balance;

        cout << "\n\n--- Actions ---";
        cout << "\n 1. Deposit";
        cout << "\n 2. Withdraw";
        cout << "\n 3. Transfer";
        cout << "\n 4. Payment";
        cout << "\n 5. Back";
        cout << "\n\n Please Enter Your Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n\n Invalid input! Please enter a number.";
            cout << "\n Press Enter to try again...";
            cin.get();
            continue;
        }

        cin.ignore(); // Flush newline

        switch (choice) {
            case 1:
                deposit(); break;
            case 2:
                withdraw(); break;
            case 3:
                transfer(); break;
            case 4:
                payment(); break;
            case 5:
                bank_system(); break;
            default:
                cout << "\n\n Invalid Choice!";
        }
    }
}

void bank::deposit() {
    cout << "\n\n--- Deposit ---";

    if (currentUserIndex == -1) {
        cout << "\n\n No user is currently logged in.";
        return;
    }

    float depositAmount;
    Account& user = accounts[currentUserIndex];

    cout << "\n Current Balance: PHP " << fixed << setprecision(2) << user.balance;
    cout << "\n Enter amount to deposit: PHP ";

    while (!(cin >> depositAmount) || depositAmount <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\n Invalid amount. Please enter a positive number: PHP ";
    }

    user.balance += depositAmount;

    // Save all accounts to file
    ofstream out(ACCOUNTS_FILE);
    for (const auto& acc : accounts) {
        out << acc.to_line() << endl;
    }

    // Log transaction
    ofstream logFile(TRANSACTIONS_FILE, ios::app);
    if (logFile.is_open()) {
        // Get current time
        time_t now = time(0);
        string timeStr = ctime(&now);
        timeStr.pop_back(); // Remove newline character
        logFile << "[" << timeStr << "] " << "Deposit|" << user.id << "|" << user.name << "|+" << fixed << setprecision(2) << depositAmount << "|Balance: " << fixed << setprecision(2) << user.balance << endl;
        logFile.close();
    }
    cout << "\n Deposit Successful!";
    cout << "\n New Balance: PHP " << fixed << setprecision(2) << user.balance;
}

void bank::withdraw() {
    cout << "\n\n--- Withdraw ---";

    if (currentUserIndex == -1) {
        cout << "\n\n No user is currently logged in.";
        return;
    }

    float withdrawAmount;
    Account& user = accounts[currentUserIndex];

    cout << "\n Current Balance: PHP " << fixed << setprecision(2) << user.balance;
    cout << "\n Enter amount to withdraw: PHP ";

    while (!(cin >> withdrawAmount) || withdrawAmount <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\n Invalid amount. Please enter a positive number: PHP ";
    }

    if (withdrawAmount > user.balance) {
        cout << "\n Insufficient balance. Withdrawal failed.";
        return;
    }

    user.balance -= withdrawAmount;

    // Save updated accounts to file
    ofstream out(ACCOUNTS_FILE);
    for (const auto& acc : accounts) {
        out << acc.to_line() << endl;
    }

    // Log transaction
    ofstream logFile(TRANSACTIONS_FILE, ios::app);
    if (logFile.is_open()) {
        // Get current time
        time_t now = time(0);
        string timeStr = ctime(&now);
        timeStr.pop_back(); // Remove the newline character at the end
        logFile << "[" << timeStr << "] " << "Withdraw|" << user.id << "|" << user.name  << "|-" << withdrawAmount << "|Balance: " << fixed << setprecision(2) << user.balance << endl;
        logFile.close();
    }

    cout << "\n Withdrawal Successful!";
    cout << "\n New Balance: PHP" << fixed << setprecision(2) << user.balance;
}

void bank::transfer() {
    cout << "\n\n--- Transfer to Another Account ---";

    if (currentUserIndex == -1 || currentUserIndex >= accounts.size()) {
        cout << "\n\n No user is currently logged in.";
        return;
    }

    Account& sender = accounts[currentUserIndex];
    string receiverNameInput;
    float transferAmount;

    cout << "\n Enter Receiver's Name: ";
    getline(cin, receiverNameInput);

    // Prevent transferring to self
    if (receiverNameInput == sender.name) {
        cout << "\n Cannot transfer to your own account.";
        return;
    }

    string inputLower = receiverNameInput;
    transform(inputLower.begin(), inputLower.end(), inputLower.begin(), ::tolower);

    int receiverIndex = -1;
    for (size_t i = 0; i < accounts.size(); ++i) {
        if (i == currentUserIndex) continue; // skip self

        string accountNameLower = accounts[i].name;
        transform(accountNameLower.begin(), accountNameLower.end(), accountNameLower.begin(), ::tolower);

        if (accountNameLower == inputLower) {
            if (receiverIndex != -1) {
                cout << "\n Multiple users found with that name. Please use a unique name.";
                return;
            }
            receiverIndex = i;
        }
    }

    if (receiverIndex == -1) {
        cout << "\n No user found with that name.";
        return;
    }

    Account& receiver = accounts[receiverIndex];

    // Get transfer amount
    cout << "\n Enter Transfer Amount: PHP";
    while (!(cin >> transferAmount) || transferAmount <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\n Invalid amount. Please enter a positive number: PHP";
    }

    if (transferAmount > sender.balance) {
        cout << "\n Insufficient balance. Transfer failed.";
        return;
    }

    // Confirm transfer
    cout << "\n Confirm transfer of PHP" << fixed << setprecision(2) << transferAmount << " to " << receiver.name << " (User ID: " << receiver.id << ")? (Y/N): ";
    char confirm;
    cin >> confirm;
    if (tolower(confirm) != 'y') {
        cout << "\n Transfer cancelled.";
        return;
    }

    // Perform transfer
    sender.balance -= transferAmount;
    receiver.balance += transferAmount;

    // Save updated accounts to file
    ofstream outFile("bank_data/bank_accounts.txt");
    if (outFile.is_open()) {
        for (const Account& acc : accounts) {
            outFile << acc.to_line() << endl;
        }
        outFile.close();
    }

    // Log the transaction
    ofstream logFile("bank_data/transactions.txt", ios::app);
    if (logFile.is_open()) {
        time_t now = time(0);
        string timeStr = ctime(&now);
        timeStr.pop_back(); // remove newline
        logFile << "[" << timeStr << "] Transfer PHP" << fixed << setprecision(2) << transferAmount << " from " << sender.name << " (ID: " << sender.id << ") to " << receiver.name << " (ID: " << receiver.id << ")\n";
        logFile.close();
    }

    cout << "\n Transfer successful!";
    cout << "\n Your new balance is: PHP" << fixed << setprecision(2) << sender.balance;
}

void bank::payment() {
    if (currentUserIndex == -1 || currentUserIndex >= accounts.size()) {
        cout << "\n\n No user is currently logged in.";
        return;
    }

    Account& user = accounts[currentUserIndex];
    string billName;
    float billAmount;

    cout << "\n\n--- Payment ---";

    cout << "\n Enter Bill Name: ";
    getline(cin, billName);

    cout << "\n Enter Bill Amount: PHP";
    while (!(cin >> billAmount) || billAmount <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\n Invalid amount. Please enter a positive number: PHP";
    }

    if (billAmount > user.balance) {
        cout << "\n Insufficient balance.";
        return;
    }

    user.balance -= billAmount;

    // Save updated accounts to file
    ofstream outFile("bank_data/bank_accounts.txt");
    for (const Account& acc : accounts) {
        outFile << acc.to_line() << endl;
    }
    outFile.close();

    // Log transaction
    ofstream logFile("bank_data/transactions.txt", ios::app);
    if (logFile.is_open()) {
        time_t now = time(0);
        string timeStr = ctime(&now);
        timeStr.pop_back();

        logFile << "[" << timeStr << "] "
                << "Payment|" << user.id << "|" << user.name
                << "|-" << fixed << setprecision(2) << billAmount
                << "|Bill: " << billName
                << "|Balance: " << fixed << setprecision(2) << user.balance << endl;
        logFile.close();
    }
    cout << "\n Payment successful!";
    cout << "\n Your new balance is: PHP" << fixed << setprecision(2) << user.balance;
}
// ATM-side menu
void bank::atm_system() {
    const string ACCOUNTS_FILE = "bank_data/bank_accounts.txt";
    string inputId, inputPin;
    bool loggedIn = false;

    string id, name, password, pin, phoneNumber, address;
    double balance = 0.0;

    // Login loop
    while (!loggedIn) {
        cout << "\n\n\t\t--- ATM SYSTEM LOGIN ---\n";
        cout << "\n Enter Account ID: ";
        cin >> ws;
        getline(cin, inputId);
        cout << " Enter PIN: ";
        getline(cin, inputPin);

        ifstream file(ACCOUNTS_FILE);
        if (!file.is_open()) {
            cout << "\n\n Error opening file.\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, password, '|');
            getline(ss, pin, '|');
            getline(ss, phoneNumber, '|');
            getline(ss, address, '|');
            ss >> balance;

            if (id == inputId && pin == inputPin) {
                loggedIn = true;
                break;
            }
        }

        file.close();

        if (!loggedIn) {
            cout << "\n Invalid ID or PIN. Try again.\n";
        }
    }

    // Menu for logged-in user
    int choice;
    while (true) {
        cout << "\n\n\t\t--- ATM SYSTEM ---\n";
        cout << "\n 1. View Balance";
        cout << "\n 2. Withdraw Amount";
        cout << "\n 3. Account Details";
        cout << "\n 4. Back (Logout)";
        cout << "\n Please Enter Your Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n\n Invalid input! Please enter a number.";
            cout << "\n Press Enter to try again...";
            cin.ignore();
            cin.get();
            continue;
        }

        cin.ignore();

        switch (choice) {
            case 1:
                cout << "\n\n--- Balance ---";
                cout << "\n Hello, " << name;
                cout << "\n Your current balance is: PHP " << fixed << setprecision(2) << balance << "\n";
                break;

            case 2:
                cout << "\n\n--- Withdraw Amount ---";
                double amount;
                cout << "\n Enter amount to withdraw: PHP ";
                cin >> amount;

                if (cin.fail() || amount <= 0) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\n Invalid amount.";
                } else if (amount > balance) {
                    cout << "\n Insufficient balance.";
                } else {
                    balance -= amount;
                    cout << "\n Withdrawal successful.";
                    cout << "\n New balance: PHP " << fixed << setprecision(2) << balance;

                    // Update the account in the file
                    ifstream inFile(ACCOUNTS_FILE);
                    vector<string> lines;
                    string tempLine;
                    while (getline(inFile, tempLine)) {
                        if (tempLine.rfind(id + "|", 0) == 0) {
                            stringstream updated;
                            updated << id << "|" << name << "|" << password << "|" << pin << "|"
                                    << phoneNumber << "|" << address << "|" << fixed << setprecision(2) << balance;
                            lines.push_back(updated.str());
                        } else {
                            lines.push_back(tempLine);
                        }
                    }
                    inFile.close();

                    ofstream outFile(ACCOUNTS_FILE, ios::trunc);
                    for (const string& l : lines)
                        outFile << l << '\n';
                    outFile.close();

                    // 📝 Log the withdrawal
                    ofstream logFile("bank_data/account_logs.txt", ios::app);
                    if (logFile.is_open()) {
                        time_t now = time(0);
                        string timeStr = ctime(&now);
                        timeStr.pop_back(); // remove newline
                        logFile << "[" << timeStr << "] "
                                << "Withdrawal | ID: " << id << " | Name: " << name
                                << " | Amount: PHP " << fixed << setprecision(2) << amount
                                << " | New Balance: PHP " << balance << "\n";
                        logFile.close();
                    }
                }
                break;

            case 3:
                cout << "\n\n--- Account Details ---";
                cout << "\n ID: " << id;
                cout << "\n Name: " << name;
                cout << "\n Phone Number: " << phoneNumber;
                cout << "\n Address: " << address;
                cout << "\n Balance: PHP " << fixed << setprecision(2) << balance << "\n";
                break;

            case 4:
                cout << "\n Logging out. Goodbye, " << name << "!\n";
                return;

            default:
                cout << "\n Invalid Choice!";
        }

        cout << "\n\n Press Enter to continue...";
        cin.get();
    }
}


void bank::bank_management() {
    int choice;

    while (true) {
        cout << "\n\n\t\t\t=== BANK MANAGEMENT MENU ===";
        cout << "\n 1. Search Account";
        cout << "\n 2. Show All Accounts";
        cout << "\n 3. Show All Transactions";
        cout << "\n 4. Back to Main Menu";
        cout << "\n Please Enter Your Choice: ";

        // Input validation
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n Invalid input! Please enter a valid number.";
            cout << "\n Press Enter to try again...";
            cin.ignore();
            cin.get();
            continue;
        }

        switch (choice) {
            case 1:
                search_accounts();
                break;

            case 2:
                show_all_accounts();
                break;

            case 3:
                show_all_transactions();
                break;

            case 4:
                menu();
                break;

            default:
                cout << "\n Invalid Choice!";
        }

        cout << "\n\n Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
}

void bank::search_accounts() {
    cout << "\n\n--- Search Account ---";

    string searchId, searchName;
    bool found = false;

    cout << "\n Enter User ID to search: ";
    cin >> searchId;
    cin.ignore();  // Clear newline

    cout << " Enter User Name to search: ";
    getline(cin, searchName);

    ifstream file("bank_data/bank_accounts.txt");
    if (!file.is_open()) {
        cout << "\n\n Error: Unable to open accounts file.\n";
        return;
    }

    string line;
    Account matchedAccount;

    while (getline(file, line)) {
        stringstream ss(line);
        Account acc;
        string balanceStr;

        getline(ss, acc.id, '|');
        getline(ss, acc.name, '|');
        getline(ss, acc.password, '|');
        getline(ss, acc.pin, '|');
        getline(ss, acc.address, '|');
        getline(ss, acc.phoneNumber, '|');
        getline(ss, balanceStr, '|');
        acc.balance = stof(balanceStr);

        if (acc.id == searchId && acc.name == searchName) {
            matchedAccount = acc;
            found = true;
            cout << "\n\n--- Account Found ---";
            cout << "\n ID: " << acc.id;
            cout << "\n Name: " << acc.name;
            cout << "\n Address: " << acc.address;
            cout << "\n Phone Number: " << acc.phoneNumber;
            cout << "\n Balance: php" << fixed << setprecision(2) << acc.balance;
            break;
        }
    }

    file.close();

    if (!found) {
        cout << "\n\n No matching account found.\n";
        return;
    }

    // Match to account loaded in memory
    currentUserIndex = -1;
    for (int i = 0; i < accounts.size(); ++i) {
        if (accounts[i].id == matchedAccount.id && accounts[i].name == matchedAccount.name) {
            currentUserIndex = i;
            break;
        }
    }

    if (currentUserIndex == -1) {
        accounts.push_back(matchedAccount);
        currentUserIndex = accounts.size() - 1;
    }
    account_management_menu();
}


void bank::account_management_menu() {
    if (currentUserIndex == -1 || currentUserIndex >= accounts.size()) {
        cout << "\n\n Invalid or no user selected.";
        return;
    }

    Account& account = accounts[currentUserIndex];  // Use currentUserIndex

    int choice;
    while (true) {
        cout << "\n\n--- ACCOUNT MANAGEMENT MENU ---";
        cout << "\n\n Account ID      : " << account.id;
        cout << "\n Name            : " << account.name;
        cout << "\n Address         : " << account.address;
        cout << "\n Phone Number    : " << account.phoneNumber;
        cout << "\n Current Balance : PHP" << fixed << setprecision(2) << account.balance;

        cout << "\n\n--- Actions ---";
        cout << "\n 1. Edit Account Details";
        cout << "\n 2. Close (Delete) Account";
        cout << "\n 3. Back";
        cout << "\n\n Please Enter Your Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n\n Invalid input! Please enter a number.";
            cout << "\n Press Enter to try again...";
            cin.get();
            continue;
        }

        cin.ignore();
        switch (choice) {
            case 1:
                edit_account_details();
                break;

            case 2:
                close_account();
                break;

            case 3:
                bank_management();
                break;
            default:
                cout << "\n\n Invalid Choice!";
        }

        cout << "\n\n Press Enter to continue...";
        cin.get();
    }
}

void bank::show_all_accounts() {
    cout << "\n\n--- All Bank Accounts ---\n";

    accounts.clear(); // Clear old data

    ifstream file("bank_data/bank_accounts.txt");
    if (!file.is_open()) {
        cout << "\n Error: Unable to open accounts file.\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Account acc;
        string balanceStr;

        getline(ss, acc.id, '|');
        getline(ss, acc.name, '|');
        getline(ss, acc.password, '|');
        getline(ss, acc.pin, '|');
        getline(ss, acc.address, '|');
        getline(ss, acc.phoneNumber, '|');
        getline(ss, balanceStr, '|');
        acc.balance = stof(balanceStr);

        accounts.push_back(acc);
    }

    file.close();

    if (accounts.empty()) {
        cout << "\n No accounts available.\n";
        return;
    }

    // Display all accounts
    for (const Account& acc : accounts) {
        cout << "\n Account ID      : " << acc.id;
        cout << "\n Name            : " << acc.name;
        cout << "\n Address         : " << acc.address;
        cout << "\n Phone Number    : " << acc.phoneNumber;
        cout << "\n Current Balance : PHP" << fixed << setprecision(2) << acc.balance;
        cout << "\n-------------------------------";
    }

    char choice;
    cout << "\n\n Do you want to select an account? (y/n): ";
    cin >> choice;
    cin.ignore();

    if (tolower(choice) == 'n') {
        return;
    } else if (tolower(choice) == 'y') {
        string inputId;
        cout << "\n Enter Account ID to select: ";
        getline(cin, inputId);

        currentUserIndex = -1;
        for (int i = 0; i < accounts.size(); ++i) {
            if (accounts[i].id == inputId) {
                currentUserIndex = i;
                break;
            }
        }

        if (currentUserIndex == -1 || currentUserIndex >= accounts.size()) {
            cout << "\n\n Invalid or no user selected.";
            return;
        }

        account_management_menu();
    } else {
        cout << "\n Invalid input. Returning...";
    }
}

void bank::edit_account_details() {
if (currentUserIndex == -1 || currentUserIndex >= accounts.size()) {
        cout << "\n\nInvalid or no user selected.\n";
        return;
    }

    Account& account = accounts[currentUserIndex];

    cout << "\nEditing account (ID: " << account.id << ", Name: " << account.name << ")\n";
    cout << "\n\n--- Actions ---";
    cout << "\n 1. Edit Name";
    cout << "\n 2. Edit Password";
    cout << "\n 3. Edit PIN";
    cout << "\n 4. Edit Address";
    cout << "\n 5. Edit Phone Number";
    cout << "\n 6. Cancel";
    cout << "\n\n Please Enter Your Choice: ";


    int choice;
    cin >> choice;
    cin.ignore(); // Clear newline

    switch (choice) {
        case 1:
            cout << "Enter new name: ";
            getline(cin, account.name);
            break;
        case 2:
            cout << "Enter new password: ";
            getline(cin, account.password);
            break;
        case 3:
            while (true) {
                cout << " Pincode (MAX 4 Digits): ";
                cin >> account.pin;
                if (account.pin.length() == 4 && all_of(account.pin.begin(), account.pin.end(), ::isdigit)) {
                    break;
                } else {
                    cout << " Invalid PIN. Please enter exactly 4 digits.\n";
                }
            }
        case 4:
            cout << "Enter new address: ";
            getline(cin, account.address);
            break;
        case 5:
            cout << "Enter new phone number: ";
            getline(cin, account.phoneNumber);
            break;


        default:
            cout << "Edit cancelled.\n";
            return;
    }


    ifstream file("bank_data/bank_accounts.txt");
    if (!file.is_open()) {
        cout << "Error opening account file.\n";
        return;
    }

    vector<string> updatedLines;
    string line;

    while (getline(file, line)) {
        if (line.rfind(account.id + "|", 0) == 0) {
            ostringstream updatedLine;
            updatedLine << account.id << "|" << account.name << "|" << account.password << "|" << account.pin << "|" << account.address << "|" << account.phoneNumber << "|" << fixed << setprecision(2) << account.balance;
            updatedLines.push_back(updatedLine.str());
        } else {
            updatedLines.push_back(line);
        }
    }

    file.close();

    ofstream outFile("bank_data/bank_accounts.txt", ios::trunc);
    if (!outFile.is_open()) {
        cout << "Error writing to account file.\n";
        return;
    }

    for (const string& accLine : updatedLines) {
        outFile << accLine << '\n';
    }

    outFile.close();

    // Log the edit
    ofstream logFile("bank_data/account_logs.txt", ios::app);
    if (logFile.is_open()) {
        time_t now = time(0);
        string timeStr = ctime(&now);
        timeStr.pop_back();

        logFile << "[" << timeStr << "] " << "Account Edited | ID: " << account.id << " | New Name: " << account.name << "\n";
        logFile.close();
    }

    cout << "Account updated successfully.\n";
    account_management_menu();
}

void bank::close_account() {
    if (currentUserIndex == -1 || currentUserIndex >= accounts.size()) {
        cout << "\n\nInvalid or no user selected.\n";
        return;
    }

    Account& account = accounts[currentUserIndex];

    cout << "\nAre you sure you want to delete your account (ID: " << account.id << ", Name: " << account.name << ")? (y/n): ";
    char confirm;
    cin >> confirm;

    if (tolower(confirm) != 'y') {
        cout << "Account deletion cancelled.\n";
        return;
    }

    // Remove from file
    ifstream file("bank_data/bank_accounts.txt");
    if (!file.is_open()) {
        cout << "Error opening account file.\n";
        return;
    }

    vector<string> updatedLines;
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.rfind(account.id + "|", 0) == 0) {
            found = true;
        } else {
            updatedLines.push_back(line);
        }
    }

    file.close();

    if (!found) {
        cout << "Error: Account ID '" << account.id << "' not found in file.\n";
        return;
    }

    ofstream outFile("bank_data/bank_accounts.txt", ios::trunc);
    if (!outFile.is_open()) {
        cout << "Error writing to account file.\n";
        return;
    }

    for (const string& accLine : updatedLines) {
        outFile << accLine << '\n';
    }

    outFile.close();

    // Log deletion
    ofstream logFile("bank_data/account_logs.txt", ios::app);
    if (logFile.is_open()) {
        time_t now = time(0);
        string timeStr = ctime(&now);
        timeStr.pop_back();
        logFile << "[" << timeStr << "] "
                << "Account Deleted | ID: " << account.id
                << " | Name: " << account.name << "\n";
        logFile.close();
    }

    accounts.erase(accounts.begin() + currentUserIndex);
    currentUserIndex = -1;

    cout << "Account '" << account.name << "' (ID: " << account.id << ") has been successfully deleted.\n";
    account_management_menu();
}



void bank::show_all_transactions() {
    cout << "\n\n--- Transaction History ---\n";

    ifstream file("bank_data/transactions.txt");
    if (!file.is_open()) {
        cout << "\n Error: Unable to open transactions file.\n";
        return;
    }

    string line;
    bool empty = true;

    while (getline(file, line)) {
        if (!line.empty()) {
            cout << " " << line << "\n";
            empty = false;
        }
    }

    file.close();

    if (empty) {
        cout << "\n No transactions found.\n";
    }

    int choice;
    while (true) {
        cout << "\n\n Press 1 to go back: ";
        if (cin >> choice && choice == 1) {
            break;
        } else {
            cin.clear();
            cout << " Invalid input. Please enter 1 to go back.\n";
        }
    }
}


int main() {
    fs::create_directories("bank_data");
    bank obj;
    obj.menu(); // Show control panel
    return 0;
}