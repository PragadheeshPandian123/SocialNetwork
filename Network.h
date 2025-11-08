#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <string>
#include "User.h"
#include "PremiumUser.h"
#include "Message.h"
#include "ActivityNode.h"

using namespace std;

class Network {
private:
    vector<User*> users;
    vector<vector<int>> adjMatrix;  // Graph for friendships
    queue<pair<int, int>> friendRequests;  // Queue: (sender_id, receiver_id)
    unordered_map<int, stack<Message>> userMessages;  // Stack of messages per user
    unordered_map<int, ActivityNode*> activityLogs;  // Linked list of activities per user
    unordered_map<string, int> usernameIndex;  // Hash map for quick username lookup
    
    // Helper functions
    void resizeAdjMatrix();
    string getCurrentTime();
    void addActivity(int userId, string activity);
    User* getUserById(int id);
    void saveConnections();
    void loadConnections();
    void saveMessages();
    void loadMessages();
    void saveActivities();
    void loadActivities();
    void saveFriendRequests();
    void loadFriendRequests();

public:
    Network();
    ~Network();

    void loadUsers();
    void saveUsers();

    // Authentication
    void signUp();
    User* signIn();

    // User operations
    void viewAllUsers(User* currentUser);
    void searchUser(User* currentUser);
    void viewProfile(User* currentUser);
    
    // Friend operations
    void sendFriendRequest(User* currentUser);
    void viewFriendRequests(User* currentUser);
    void viewMyFriends(User* currentUser);
    void removeFriend(User* currentUser);
    void suggestFriends(User* currentUser);
    
    // Messaging
    void sendMessage(User* currentUser);
    void viewMessages(User* currentUser);
    void viewMessageCount(User* currentUser);
    
    // Activity & Analytics
    void viewActivityLog(User* currentUser);
    void viewNetworkStats(User* currentUser);
    
    // Graph algorithms
    void findShortestPath(User* currentUser);
    void findMutualFriends(User* currentUser);
    
    // Premium features
    void upgradeToPremium(User* currentUser);
    void viewPremiumFeatures(User* currentUser);
    
    // Account management
    void deleteAccount(User* currentUser);
    
    // Operator overloading demonstration
    void demonstrateOperators(User* currentUser);
};

#endif