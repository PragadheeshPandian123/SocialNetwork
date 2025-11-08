#include "Network.h"
#include <fstream>
#include <iostream>
using namespace std;

Network::Network() {
    loadUsers();
}

Network::~Network() {
    for (auto u : users)
        delete u;
}

void Network::loadUsers() {
    ifstream fin("users.txt");
    if (!fin) return;

    int id, type;
    string username, password;
    while (fin >> id >> type >> username >> password) {
        if (type == 1)
            users.push_back(new PremiumUser(id, username, password));
        else
            users.push_back(new User(id, username, password));
    }
    fin.close();
}

void Network::saveUsers() {
    ofstream fout("users.txt");
    for (auto &u : users) {
        int type = (dynamic_cast<PremiumUser*>(u)) ? 1 : 0;
        fout << u->getId() << " " << type << " "
             << u->getUsername() << " " << u->getPassword() << "\n";
    }
    fout.close();
}

void Network::signUp() {
    int amt;
    string username, password;
    int type;
    cout << "\n--- SIGN UP ---\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    cout << "Account type (0 = Normal, 1 = Premium): ";
    cin >> type;
    cout<< "Enter Subscription Amount : (Rs . 200) : ";
    cin>>amt;
    for (auto &u : users) {
        if (u->getUsername() == username) {
            cout << "❌ Username already exists.\n";
            return;
        }
    }

    int id = users.size() + 1;
    if (type == 1 && amt==200)
        users.push_back(new PremiumUser(id, username, password));
    
    else if(type==1)
    {
        cout<<"Invalid subcription Amount \n Exiting Program ..";
        exit(0);
    }
    else
        users.push_back(new User(id, username, password));

    saveUsers();
    cout << "✅ Account created successfully! Your ID: " << id << "\n";
}

User* Network::signIn() {
    string username, password;
    cout << "\n--- SIGN IN ---\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    for (auto &u : users) {
        if (u->getUsername() == username && u->getPassword() == password) {
            cout << "✅ Login successful!\n";
            return u;
        }
    }

    cout << "❌ Invalid username or password.\n";
    return nullptr;
}
