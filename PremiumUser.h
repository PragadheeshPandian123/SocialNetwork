#ifndef PREMIUMUSER_H
#define PREMIUMUSER_H

#include "User.h"

class PremiumUser : public User {
private:
    int bonusPoints;

public:
    PremiumUser(int i = 0, string u = "", string p = "")
        : User(i, u, p), bonusPoints(0) {}

    void addBonusPoints(int points) {
        bonusPoints += points;
        cout << points << " bonus points added! Total: " << bonusPoints << endl;
    }

    void unlockPremiumFeature() {
        cout << "🎉 Premium feature unlocked! You can send unlimited messages!\n";
    }

    void showDashboard() override {
        cout << "\n==============================\n";
        cout << "Welcome Premium User, " << username << "!\n";
        cout << "==============================\n";
        cout << "1. View Profile\n";
        cout << "2. Add Friend\n";
        cout << "3. View Bonus Points\n";
        cout << "4. Unlock Premium Feature\n";
        cout << "5. Logout\n";
        cout << "==============================\n";
    }
};

#endif
