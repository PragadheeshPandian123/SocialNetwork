#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class User {
protected:
    int id;
    string username;
    string password;
    vector<int> friends;  // Store friend IDs

public:
    User(int i = 0, string u = "", string p = "") 
        : id(i), username(u), password(p) {}

    virtual ~User() {}

    int getId() const { return id; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    vector<int> getFriends() const { return friends; }
    
    void addFriend(int friendId) {
        friends.push_back(friendId);
    }
    
    void removeFriend(int friendId) {
        for (auto it = friends.begin(); it != friends.end(); ++it) {
            if (*it == friendId) {
                friends.erase(it);
                break;
            }
        }
    }
    
    bool isFriend(int friendId) const {
        for (int fid : friends) {
            if (fid == friendId) return true;
        }
        return false;
    }

    virtual void showDashboard() {
        cout << "\n==============================\n";
        cout << "Welcome, " << username << "!\n";
        cout << "==============================\n";
    }

    virtual string getUserType() const {
        return "Normal";
    }

    void displayProfile() const {
        cout << "\n--- USER PROFILE ---\n";
        cout << "User ID: " << id << "\n";
        cout << "Username: " << username << "\n";
        cout << "Account Type: " << getUserType() << "\n";
        cout << "Total Friends: " << friends.size() << "\n";
    }

    // Operator overloading
    friend ostream& operator<<(ostream& out, const User& u) {
        out << "[" << u.getUserType() << "] " << u.username << " (ID: " << u.id << ")";
        return out;
    }
    
    void operator+(const User& other) {
        cout << "✅ " << username << " and " << other.username << " are now friends!\n";
    }
    
    bool operator==(const User& other) const {
        return this->id == other.id;
    }
    
    bool operator!=(const User& other) const {
        return this->id != other.id;
    }
};

#endif