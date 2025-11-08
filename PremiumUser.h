#ifndef PREMIUMUSER_H
#define PREMIUMUSER_H

#include "User.h"

class PremiumUser : public User {
private:
    int bonusPoints;
    bool unlimitedMessages;
    bool prioritySupport;

public:
    PremiumUser(int i = 0, string u = "", string p = "")
        : User(i, u, p), bonusPoints(100), unlimitedMessages(true), prioritySupport(true) {}

    void addBonusPoints(int points) {
        bonusPoints += points;
        cout << " " << points << " bonus points added! Total: " << bonusPoints << " points\n";
    }
    
    int getBonusPoints() const {
        return bonusPoints;
    }
    
    bool hasUnlimitedMessages() const {
        return unlimitedMessages;
    }
    
    void showPremiumFeatures() {
        cout << "\n === PREMIUM FEATURES === \n";
        cout << " Unlimited Messages\n";
        cout << " Priority Friend Requests\n";
        cout << " Bonus Points: " << bonusPoints << "\n";
        cout << " Advanced Analytics\n";
        cout << " Custom Profile Themes\n";
        cout << " Ad-Free Experience\n";
        cout << "============================\n";
    }

    void showDashboard() override {
        cout << "\n ==============================\n";
        cout << "   Welcome Premium User, " << username << "!\n";
        cout << "   Bonus Points: " << bonusPoints << " \n";
        cout << "================================\n";
    }
    
    string getUserType() const override {
        return "Premium";
    }
    
    // Premium-specific operator overloading
    PremiumUser& operator+=(int points) {
        bonusPoints += points;
        return *this;
    }
    
    PremiumUser& operator-=(int points) {
        bonusPoints = (bonusPoints >= points) ? (bonusPoints - points) : 0;
        return *this;
    }
};

#endif