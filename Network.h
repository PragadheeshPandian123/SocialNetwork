#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <string>
#include "User.h"
#include "PremiumUser.h"
using namespace std;

class Network {
private:
    vector<User*> users;

public:
    Network();
    ~Network();

    void loadUsers();
    void saveUsers();

    void signUp();
    User* signIn();
};

#endif
