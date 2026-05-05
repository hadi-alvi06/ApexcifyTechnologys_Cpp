/*
 * ============================================================
 *
 *  Apexcify Technologys Internship - Task 2
 *  Made by : Hadi Ahmed Alvi
 *
 *  ============================================================
*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct User {
    string username;
    string password;
};

const string USERS_FILE = "users.txt";

vector<User> loadUsers() {
    vector<User> users;
    ifstream file(USERS_FILE);
    if (!file.is_open()) return users;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        size_t pos = line.find('|');
        if (pos != string::npos) {
            User u;
            u.username = line.substr(0, pos);
            u.password = line.substr(pos + 1);
            users.push_back(u);
        }
    }
    file.close();
    return users;
}

void saveUsers(const vector<User>& users) {
    ofstream file(USERS_FILE);
    for (const auto& u : users) {
        file << u.username << "|" << u.password << "\n";
    }
    file.close();
}

bool isUsernameTaken(const vector<User>& users, const string& username) {
    for (const auto& u : users) {
        if (u.username == username) return true;
    }
    return false;
}

void registerUser() {
    cout << "\n========== REGISTRATION ==========\n";
    vector<User> users = loadUsers();

    string username;
    while (true) {
        cout << "Enter username: ";
        cin >> username;
        if (isUsernameTaken(users, username)) {
            cout << "[ERROR] Username already exists. Please choose another.\n";
        }
        else if (username.length() < 3) {
            cout << "[ERROR] Username must be at least 3 characters.\n";
        }
        else {
            break;
        }
    }

    string password, confirmPassword;
    while (true) {
        cout << "Enter password: ";
        cin >> password;
        if (password.length() < 4) {
            cout << "[ERROR] Password must be at least 4 characters.\n";
            continue;
        }
        cout << "Confirm password: ";
        cin >> confirmPassword;
        if (password != confirmPassword) {
            cout << "[ERROR] Passwords do not match.\n";
        }
        else {
            break;
        }
    }

    User newUser;
    newUser.username = username;
    newUser.password = password;
    users.push_back(newUser);
    saveUsers(users);

    cout << "[SUCCESS] Registration successful!\n";
    cout << "[INFO] You can now login with your credentials.\n\n";
}

bool loginUser() {
    cout << "\n========== LOGIN ==========\n";
    vector<User> users = loadUsers();

    if (users.empty()) {
        cout << "[ERROR] No users found. Please register first.\n\n";
        return false;
    }

    string username, password;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    for (const auto& u : users) {
        if (u.username == username && u.password == password) {
            cout << "[SUCCESS] Login successful!\n";
            cout << "[INFO] Welcome, " << username << "!\n\n";
            return true;
        }
    }

    cout << "[ERROR] Invalid username or password.\n\n";
    return false;
}

int main() {
    cout << "===============================================\n";
    cout << "     SCHOOL MANAGEMENT SYSTEM \n";
    cout << "===============================================\n";

    while (true) {
        cout << "\n[1] Register\n";
        cout << "[2] Login\n";
        cout << "[0] Exit\n";
        cout << "Choice: ";

        int choice;
        cin >> choice;

        if (choice == 0) {
            cout << "\nThank you for using the School Management System!\n";
            cout << "\n";
            break;
        }
        else if (choice == 1) {
            registerUser();
        }
        else if (choice == 2) {
            loginUser();
        }
        else {
            cout << "[ERROR] Invalid choice. Please try again.\n";
        }
    }

    return 0;
}