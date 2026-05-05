/*
 * ============================================================
 *
 *  BANKING SYSTEM
 *  Apexcify Technologys Internship ” Task 3
 *
 * ============================================================
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <ctime>

using namespace std;

// File paths
const string FILE_CUSTOMERS = "customers.txt";
const string FILE_ACCOUNTS = "accounts.txt";
const string FILE_TRANSACTIONS = "transactions.txt";

// Timestamp helper
string getCurrentTime() {
    time_t t = time(nullptr);
    char buf[20];
    struct tm timeinfo;
    localtime_s(&timeinfo, &t); // Use localtime_s for safety
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return string(buf);
}

// Transaction class
class Transaction {
private:
    string id;
    string accountNo;
    string type;
    double amount;
    double balanceAfter;
    string timestamp;
    string description;

public:
    Transaction() {}

    Transaction(string tid, string accNo, string t, double amt, double bal, string desc)
        : id(tid), accountNo(accNo), type(t), amount(amt), balanceAfter(bal),
        timestamp(getCurrentTime()), description(desc) {
    }

    // Getters
    string getId() const { return id; }
    string getAccountNo() const { return accountNo; }
    string getType() const { return type; }
    double getAmount() const { return amount; }
    double getBalanceAfter() const { return balanceAfter; }
    string getTimestamp() const { return timestamp; }
    string getDescription() const { return description; }

    // For file storage
    string toFileString() const {
        return id + "|" + accountNo + "|" + type + "|" +
            to_string(amount) + "|" + to_string(balanceAfter) + "|" +
            timestamp + "|" + description;
    }

    static Transaction fromFileString(const string& line) {
        stringstream ss(line);
        Transaction t;
        getline(ss, t.id, '|');
        getline(ss, t.accountNo, '|');
        getline(ss, t.type, '|');
        string amtStr, balStr;
        getline(ss, amtStr, '|');
        getline(ss, balStr, '|');
        getline(ss, t.timestamp, '|');
        getline(ss, t.description, '|');
        t.amount = stod(amtStr);
        t.balanceAfter = stod(balStr);
        return t;
    }
};

// Account class
class Account {
private:
    string accountNo;
    string customerId;
    string accountType;
    double balance;
    bool isActive;

public:
    Account() : balance(0), isActive(true) {}

    Account(string accNo, string custId, string type, double initialBalance)
        : accountNo(accNo), customerId(custId), accountType(type),
        balance(initialBalance), isActive(true) {
    }

    // Getters
    string getAccountNo() const { return accountNo; }
    string getCustomerId() const { return customerId; }
    string getAccountType() const { return accountType; }
    double getBalance() const { return balance; }
    bool getIsActive() const { return isActive; }

    // Operations
    bool deposit(double amount) {
        if (amount <= 0) return false;
        balance += amount;
        return true;
    }

    bool withdraw(double amount) {
        if (amount <= 0 || amount > balance) return false;
        balance -= amount;
        return true;
    }

    void setBalance(double bal) { balance = bal; }

    // For file storage
    string toFileString() const {
        return accountNo + "|" + customerId + "|" + accountType + "|" +
            to_string(balance) + "|" + (isActive ? "1" : "0");
    }

    static Account fromFileString(const string& line) {
        stringstream ss(line);
        Account a;
        string activeStr;
        getline(ss, a.accountNo, '|');
        getline(ss, a.customerId, '|');
        getline(ss, a.accountType, '|');
        string balStr;
        getline(ss, balStr, '|');
        getline(ss, activeStr, '|');
        a.balance = stod(balStr);
        a.isActive = (activeStr == "1");
        return a;
    }
};

// Customer class
class Customer {
private:
    string customerId;
    string username;
    string password;
    string fullName;
    string email;
    string phone;

public:
    Customer() {}

    Customer(string id, string uname, string pwd, string name, string mail, string ph)
        : customerId(id), username(uname), password(pwd),
        fullName(name), email(mail), phone(ph) {
    }

    // Getters
    string getCustomerId() const { return customerId; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getFullName() const { return fullName; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }

    // For file storage
    string toFileString() const {
        return customerId + "|" + username + "|" + password + "|" +
            fullName + "|" + email + "|" + phone;
    }

    static Customer fromFileString(const string& line) {
        stringstream ss(line);
        Customer c;
        getline(ss, c.customerId, '|');
        getline(ss, c.username, '|');
        getline(ss, c.password, '|');
        getline(ss, c.fullName, '|');
        getline(ss, c.email, '|');
        getline(ss, c.phone, '|');
        return c;
    }
};

// Global data storage
vector<Customer> customers;
vector<Account> accounts;
vector<Transaction> transactions;

// Helper functions
void loadData() {
    // Load customers
    ifstream custFile(FILE_CUSTOMERS);
    if (custFile.is_open()) {
        string line;
        while (getline(custFile, line)) {
            if (!line.empty())
                customers.push_back(Customer::fromFileString(line));
        }
        custFile.close();
    }

    // Load accounts
    ifstream accFile(FILE_ACCOUNTS);
    if (accFile.is_open()) {
        string line;
        while (getline(accFile, line)) {
            if (!line.empty())
                accounts.push_back(Account::fromFileString(line));
        }
        accFile.close();
    }

    // Load transactions
    ifstream txnFile(FILE_TRANSACTIONS);
    if (txnFile.is_open()) {
        string line;
        while (getline(txnFile, line)) {
            if (!line.empty())
                transactions.push_back(Transaction::fromFileString(line));
        }
        txnFile.close();
    }
}

void saveData() {
    // Save customers
    ofstream custFile(FILE_CUSTOMERS);
    for (const auto& c : customers) {
        custFile << c.toFileString() << "\n";
    }
    custFile.close();

    // Save accounts
    ofstream accFile(FILE_ACCOUNTS);
    for (const auto& a : accounts) {
        accFile << a.toFileString() << "\n";
    }
    accFile.close();

    // Save transactions
    ofstream txnFile(FILE_TRANSACTIONS);
    for (const auto& t : transactions) {
        txnFile << t.toFileString() << "\n";
    }
    txnFile.close();
}

string generateCustomerId() {
    return "CUST" + to_string(customers.size() + 1001);
}

string generateAccountNo() {
    return "ACC" + to_string(accounts.size() + 10001);
}

string generateTransactionId() {
    return "TXN" + to_string(transactions.size() + 9001);
}

bool usernameExists(const string& username) {
    for (const auto& c : customers) {
        if (c.getUsername() == username) return true;
    }
    return false;
}

Customer* findCustomerByUsername(const string& username) {
    for (auto& c : customers) {
        if (c.getUsername() == username) return &c;
    }
    return nullptr;
}

vector<Account*> getCustomerAccounts(const string& customerId) {
    vector<Account*> result;
    for (auto& a : accounts) {
        if (a.getCustomerId() == customerId && a.getIsActive()) {
            result.push_back(&a);
        }
    }
    return result;
}

void addTransaction(const string& accNo, const string& type, double amount, double balanceAfter, const string& desc) {
    string txnId = generateTransactionId();
    Transaction txn(txnId, accNo, type, amount, balanceAfter, desc);
    transactions.push_back(txn);
    saveData();
}

void viewRecentTransactions(const string& accountNo, int limit = 5) {
    cout << "\n========== RECENT TRANSACTIONS ==========\n";
    cout << "Account: " << accountNo << "\n";
    cout << "=========================================\n";

    vector<Transaction*> recent;
    for (auto& t : transactions) {
        if (t.getAccountNo() == accountNo) {
            recent.push_back(&t);
        }
    }

    if (recent.empty()) {
        cout << "No transactions found.\n";
        return;
    }

    // Show recent transactions (last 'limit' entries)
    int start = recent.size() > limit ? recent.size() - limit : 0;
    for (int i = recent.size() - 1; i >= start; i--) {
        cout << "Date: " << recent[i]->getTimestamp() << "\n";
        cout << "Type: " << recent[i]->getType() << "\n";
        cout << "Amount: $" << fixed << setprecision(2) << recent[i]->getAmount() << "\n";
        cout << "Balance: $" << recent[i]->getBalanceAfter() << "\n";
        cout << "Description: " << recent[i]->getDescription() << "\n";
        cout << "-----------------------------------------\n";
    }
}

// Registration
void registerCustomer() {
    cout << "\n========== CUSTOMER REGISTRATION ==========\n";

    string username;
    while (true) {
        cout << "Username: ";
        cin >> username;
        if (usernameExists(username)) {
            cout << "[ERROR] Username already exists.\n";
        }
        else if (username.length() < 3) {
            cout << "[ERROR] Username must be at least 3 characters.\n";
        }
        else {
            break;
        }
    }

    string password;
    cout << "Password: ";
    cin >> password;

    string fullName;
    cout << "Full Name: ";
    cin.ignore();
    getline(cin, fullName);

    string email;
    cout << "Email: ";
    cin >> email;

    string phone;
    cout << "Phone: ";
    cin >> phone;

    string customerId = generateCustomerId();
    Customer newCust(customerId, username, password, fullName, email, phone);
    customers.push_back(newCust);

    saveData();

    cout << "\n[SUCCESS] Registration successful!\n";
    cout << "[INFO] Your Customer ID: " << customerId << "\n\n";
}

// Login
Customer* login() {
    cout << "\n========== LOGIN ==========\n";

    string username, password;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    Customer* cust = findCustomerByUsername(username);
    if (cust && cust->getPassword() == password) {
        cout << "[SUCCESS] Login successful!\n";
        cout << "[INFO] Welcome, " << cust->getFullName() << "!\n\n";
        return cust;
    }

    cout << "[ERROR] Invalid username or password.\n\n";
    return nullptr;
}

// Open account
void openAccount(Customer* customer) {
    cout << "\n========== OPEN ACCOUNT ==========\n";

    cout << "Account Type (Savings/Current): ";
    string type;
    cin >> type;

    double initialDeposit;
    cout << "Initial Deposit (min 500): $";
    cin >> initialDeposit;

    if (initialDeposit < 500) {
        cout << "[ERROR] Minimum deposit is $500.\n";
        return;
    }

    string accountNo = generateAccountNo();
    Account newAcc(accountNo, customer->getCustomerId(), type, initialDeposit);
    accounts.push_back(newAcc);

    // Record initial deposit transaction
    addTransaction(accountNo, "DEPOSIT", initialDeposit, initialDeposit, "Account opening deposit");

    saveData();

    cout << "\n[SUCCESS] Account opened successfully!\n";
    cout << "[INFO] Account Number: " << accountNo << "\n";
    cout << "[INFO] Current Balance: $" << fixed << setprecision(2) << initialDeposit << "\n\n";
}

// Deposit
void deposit(Customer* customer) {
    vector<Account*> custAccounts = getCustomerAccounts(customer->getCustomerId());

    if (custAccounts.empty()) {
        cout << "[ERROR] No accounts found. Please open an account first.\n";
        return;
    }

    cout << "\n========== DEPOSIT ==========\n";
    cout << "Your accounts:\n";
    for (int i = 0; i < custAccounts.size(); i++) {
        cout << "[" << i + 1 << "] " << custAccounts[i]->getAccountNo()
            << " - Balance: $" << fixed << setprecision(2) << custAccounts[i]->getBalance() << "\n";
    }

    int choice;
    cout << "Select account: ";
    cin >> choice;

    if (choice < 1 || choice > custAccounts.size()) {
        cout << "[ERROR] Invalid choice.\n";
        return;
    }

    double amount;
    cout << "Amount to deposit: $";
    cin >> amount;

    if (custAccounts[choice - 1]->deposit(amount)) {
        saveData();
        addTransaction(custAccounts[choice - 1]->getAccountNo(), "DEPOSIT", amount,
            custAccounts[choice - 1]->getBalance(), "Cash deposit");
        cout << "[SUCCESS] Deposited $" << fixed << setprecision(2) << amount << "\n";
        cout << "[INFO] New balance: $" << custAccounts[choice - 1]->getBalance() << "\n";
    }
    else {
        cout << "[ERROR] Invalid amount.\n";
    }
}

// Withdraw
void withdraw(Customer* customer) {
    vector<Account*> custAccounts = getCustomerAccounts(customer->getCustomerId());

    if (custAccounts.empty()) {
        cout << "[ERROR] No accounts found.\n";
        return;
    }

    cout << "\n========== WITHDRAWAL ==========\n";
    cout << "Your accounts:\n";
    for (int i = 0; i < custAccounts.size(); i++) {
        cout << "[" << i + 1 << "] " << custAccounts[i]->getAccountNo()
            << " - Balance: $" << fixed << setprecision(2) << custAccounts[i]->getBalance() << "\n";
    }

    int choice;
    cout << "Select account: ";
    cin >> choice;

    if (choice < 1 || choice > custAccounts.size()) {
        cout << "[ERROR] Invalid choice.\n";
        return;
    }

    double amount;
    cout << "Amount to withdraw: $";
    cin >> amount;

    if (custAccounts[choice - 1]->withdraw(amount)) {
        saveData();
        addTransaction(custAccounts[choice - 1]->getAccountNo(), "WITHDRAWAL", amount,
            custAccounts[choice - 1]->getBalance(), "Cash withdrawal");
        cout << "[SUCCESS] Withdrawn $" << fixed << setprecision(2) << amount << "\n";
        cout << "[INFO] New balance: $" << custAccounts[choice - 1]->getBalance() << "\n";
    }
    else {
        cout << "[ERROR] Insufficient funds or invalid amount.\n";
    }
}

// Fund transfer
void transfer(Customer* customer) {
    vector<Account*> custAccounts = getCustomerAccounts(customer->getCustomerId());

    if (custAccounts.empty()) {
        cout << "[ERROR] No accounts found.\n";
        return;
    }

    cout << "\n========== FUND TRANSFER ==========\n";
    cout << "Your accounts:\n";
    for (int i = 0; i < custAccounts.size(); i++) {
        cout << "[" << i + 1 << "] " << custAccounts[i]->getAccountNo()
            << " - Balance: $" << fixed << setprecision(2) << custAccounts[i]->getBalance() << "\n";
    }

    int fromChoice;
    cout << "Select source account: ";
    cin >> fromChoice;

    if (fromChoice < 1 || fromChoice > custAccounts.size()) {
        cout << "[ERROR] Invalid choice.\n";
        return;
    }

    string toAccount;
    cout << "Enter destination account number: ";
    cin >> toAccount;

    // Find destination account
    Account* destAccount = nullptr;
    for (auto& a : accounts) {
        if (a.getAccountNo() == toAccount && a.getIsActive()) {
            destAccount = &a;
            break;
        }
    }

    if (!destAccount) {
        cout << "[ERROR] Destination account not found.\n";
        return;
    }

    double amount;
    cout << "Amount to transfer: $";
    cin >> amount;

    Account* sourceAccount = custAccounts[fromChoice - 1];

    if (sourceAccount->withdraw(amount)) {
        destAccount->deposit(amount);
        saveData();

        addTransaction(sourceAccount->getAccountNo(), "TRANSFER_OUT", amount,
            sourceAccount->getBalance(), "Transfer to " + toAccount);
        addTransaction(destAccount->getAccountNo(), "TRANSFER_IN", amount,
            destAccount->getBalance(), "Transfer from " + sourceAccount->getAccountNo());

        cout << "[SUCCESS] Transferred $" << fixed << setprecision(2) << amount << "\n";
        cout << "[INFO] Your new balance: $" << sourceAccount->getBalance() << "\n";
    }
    else {
        cout << "[ERROR] Insufficient funds.\n";
    }
}

// View statement
void viewStatement(Customer* customer) {
    vector<Account*> custAccounts = getCustomerAccounts(customer->getCustomerId());

    if (custAccounts.empty()) {
        cout << "[ERROR] No accounts found.\n";
        return;
    }

    cout << "\n========== ACCOUNT STATEMENT ==========\n";
    cout << "Your accounts:\n";
    for (int i = 0; i < custAccounts.size(); i++) {
        cout << "[" << i + 1 << "] " << custAccounts[i]->getAccountNo()
            << " - Balance: $" << fixed << setprecision(2) << custAccounts[i]->getBalance() << "\n";
    }

    int choice;
    cout << "Select account to view statement: ";
    cin >> choice;

    if (choice < 1 || choice > custAccounts.size()) {
        cout << "[ERROR] Invalid choice.\n";
        return;
    }

    viewRecentTransactions(custAccounts[choice - 1]->getAccountNo(), 10);
}

// Customer menu
void customerMenu(Customer* customer) {
    while (true) {
        cout << "\n========================================\n";
        cout << "     BANKING SYSTEM - Customer Menu\n";
        cout << "========================================\n";
        cout << "[1] Open Account\n";
        cout << "[2] Deposit\n";
        cout << "[3] Withdraw\n";
        cout << "[4] Fund Transfer\n";
        cout << "[5] View Statement\n";
        cout << "[0] Logout\n";
        cout << "Choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1: openAccount(customer); break;
        case 2: deposit(customer); break;
        case 3: withdraw(customer); break;
        case 4: transfer(customer); break;
        case 5: viewStatement(customer); break;
        case 0:
            cout << "[INFO] Logged out successfully.\n";
            return;
        default:
            cout << "[ERROR] Invalid choice.\n";
        }
    }
}

// Main
int main() {
    cout << "========================================\n";
    cout << "     BANKING SYSTEM \n";
    cout << "========================================\n";

    loadData();

    while (true) {
        cout << "\n[1] Register\n";
        cout << "[2] Login\n";
        cout << "[0] Exit\n";
        cout << "Choice: ";

        int choice;
        cin >> choice;

        if (choice == 0) {
            cout << "\nThank you for using the Banking System!\n";
            cout << "\n";
            break;
        }
        else if (choice == 1) {
            registerCustomer();
        }
        else if (choice == 2) {
            Customer* customer = login();
            if (customer) {
                customerMenu(customer);
            }
        }
        else {
            cout << "[ERROR] Invalid choice.\n";
        }
    }

    return 0;
}
