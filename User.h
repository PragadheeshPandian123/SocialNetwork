#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
using namespace std;

class User {
protected:
    int id;
    string username;
    string password;

public:
    User(int i = 0, string u = "", string p = "") 
        : id(i), username(u), password(p) {}

    virtual ~User() {}

    int getId() const { return id; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }

    virtual void showDashboard() {
        cout << "\n==============================\n";
        cout << "Welcome, " << username << "!\n";
        cout << "==============================\n";
        cout << "1. View Profile\n";
        cout << "2. Add Friend\n";
        cout << "3. Logout\n";
        cout << "==============================\n";
    }

    void displayProfile() const {
        cout << "\n--- USER PROFILE ---\n";
        cout << "User ID: " << id << "\nUsername: " << username << "\n";
    }

    // Operator overloading examples
    friend ostream& operator<<(ostream& out, const User& u) {
        out << "[User] " << u.username << " (ID: " << u.id << ")";
        return out;
    }
    void operator+(const User& other) {
        cout << username << " and " << other.username << " are now friends!\n";
    }
};

#endif
