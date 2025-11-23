#include "Network.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <climits>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <utility>
using namespace std;

Network::Network() {
    loadUsers();
    loadConnections();
    loadMessages();
    loadActivities();
    loadFriendRequests();
    resizeAdjMatrix();
}

Network::~Network() {
    saveUsers();
    saveConnections();
    saveMessages();
    saveActivities();
    saveFriendRequests();
    
    for (auto u : users)
        delete u;
    
    for (auto& pair : activityLogs) {
        ActivityNode* current = pair.second;
        while (current) {
            ActivityNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

string Network::getCurrentTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string timeStr(dt);
    timeStr.pop_back(); 
    return timeStr;
}

void Network::resizeAdjMatrix() {
    int n = users.size();
    adjMatrix.resize(n, vector<int>(n, 0));
}

User* Network::getUserById(int id) {
    for (auto u : users) {
        if (u->getId() == id)
            return u;
    }
    return nullptr;
}

void Network::addActivity(int userId, string activity) {
    string timestamp = getCurrentTime();
    ActivityNode* newNode = new ActivityNode(activity, timestamp);
    
    if (activityLogs.find(userId) == activityLogs.end()) {
        activityLogs[userId] = newNode;
    } else {
        newNode->next = activityLogs[userId];
        activityLogs[userId] = newNode;
    }
}

// ==================== FILE HANDLING ====================

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
        
        usernameIndex[username] = id;
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

void Network::loadConnections() {
    ifstream fin("connections.txt");
    if (!fin) return;
    
    int userId, friendId;
    while (fin >> userId >> friendId) {
        User* user = getUserById(userId);
        if (user && !user->isFriend(friendId)) {
            user->addFriend(friendId);
        }
    }
    fin.close();
}

void Network::saveConnections() {
    ofstream fout("connections.txt");
    for (auto &u : users) {
        int userId = u->getId();
        for (int friendId : u->getFriends()) {
            if (userId < friendId) { // Avoid duplicates
                fout << userId << " " << friendId << "\n";
            }
        }
    }
    fout.close();
}

void Network::loadMessages() {
    ifstream fin("messages.txt");
    if (!fin) return;
    
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        int senderId, receiverId;
        string senderName, content, timestamp;
        
        ss >> senderId >> receiverId;
        ss >> senderName;
        ss.ignore();
        getline(ss, content, '|');
        getline(ss, timestamp);
        
        Message msg(senderId, receiverId, senderName, content, timestamp);
        userMessages[receiverId].push(msg);
    }
    fin.close();
}

void Network::saveMessages() {
    ofstream fout("messages.txt");
    
    for (auto& pair : userMessages) {
        stack<Message> tempStack = pair.second;
        vector<Message> messages;
        
        while (!tempStack.empty()) {
            messages.push_back(tempStack.top());
            tempStack.pop();
        }
        
        for (auto it = messages.rbegin(); it != messages.rend(); ++it) {
            fout << it->senderId << " " << it->receiverId << " "
                 << it->senderName << " " << it->content << "|"
                 << it->timestamp << "\n";
        }
    }
    fout.close();
}

void Network::loadActivities() {
    ifstream fin("activities.txt");
    if (!fin) return;
    
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        int userId;
        string activity, timestamp;
        
        ss >> userId;
        ss.ignore();
        getline(ss, activity, '|');
        getline(ss, timestamp);
        
        addActivity(userId, activity);
    }
    fin.close();
}

void Network::saveActivities() {
    ofstream fout("activities.txt");
    
    for (auto& pair : activityLogs) {
        int userId = pair.first;
        ActivityNode* current = pair.second;
        
        // Collect activities in a vector to reverse order
        vector<std::pair<string, string > > activities;
        while (current) {
            activities.push_back({current->activity, current->timestamp});
            current = current->next;
        }
        
        // Write activities in reverse order (oldest first in file)
        for (int i = activities.size() - 1; i >= 0; i--) {
            // Escape special characters in activity string
            string escapedActivity = activities[i].first;
            // Replace any newlines or pipes that might break formatting
            for (size_t pos = 0; pos < escapedActivity.length(); pos++) {
                if (escapedActivity[pos] == '\n') escapedActivity[pos] = ' ';
            }
            fout << userId << " " << escapedActivity << "|" << activities[i].second << "\n";
        }
    }
    fout.close();
}


void Network::loadFriendRequests() {
    ifstream fin("requests.txt");
    if (!fin) return;
    
    int senderId, receiverId;
    while (fin >> senderId >> receiverId) {
        friendRequests.push({senderId, receiverId});
    }
    fin.close();
}

void Network::saveFriendRequests() {
    ofstream fout("requests.txt");
    
    queue<pair<int, int>> tempQueue = friendRequests;
    while (!tempQueue.empty()) {
        auto req = tempQueue.front();
        tempQueue.pop();
        fout << req.first << " " << req.second << "\n";
    }
    fout.close();
}

// ==================== AUTHENTICATION ====================

void Network::signUp() {
    int amt;
    string username, password;
    int type;
    
    cout << "\n================================\n";
    cout << "        SIGN UP - NEW USER      \n";
    cout << "==================================\n";
    
    cout << "Enter username: ";
    cin >> username;
    
    // Check if username exists (using hash map)
    if (usernameIndex.find(username) != usernameIndex.end()) {
        cout << " Username already exists.\n";
        return;
    }
    
    cout << "Enter password: ";
    cin >> password;
    cout << "\nAccount Types:\n";
    cout << "  0 = Normal User (Free)\n";
    cout << "  1 = Premium User (Rs. 200)\n";
    cout << "Choose account type: ";
    cin >> type;
    
    int id = users.size() + 1;
    
    if (type == 1) {
        cout << "Enter Subscription Amount (Rs. 200): ";
        cin >> amt;
        
        if (amt != 200) {
            cout << " Invalid subscription amount!\n";
            return;
        }
        users.push_back(new PremiumUser(id, username, password));
        cout << " Premium account created successfully!\n";
    } else {
        users.push_back(new User(id, username, password));
        cout << " Normal account created successfully!\n";
    }
    
    usernameIndex[username] = id;
    resizeAdjMatrix();
    saveUsers();
    
    cout << "Your User ID: " << id << "\n";
    addActivity(id, "Account created");
}

User* Network::signIn() {
    string username, password;
    
    cout << "\n===============================\n";
    cout << "          SIGN IN              \n";
    cout << "================================\n";
    
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Use hash map for quick lookup
    if (usernameIndex.find(username) != usernameIndex.end()) {
        int userId = usernameIndex[username];
        User* user = getUserById(userId);
        
        if (user && user->getPassword() == password) {
            cout << " Login successful! Welcome, " << username << "!\n";
            addActivity(userId, "Logged in");
            return user;
        }
    }

    cout << " Invalid username or password.\n";
    return nullptr;
}

// ==================== USER OPERATIONS ====================

void Network::viewAllUsers(User* currentUser) {
    cout << "\n===============================================\n";
    cout << "            ALL REGISTERED USERS               \n";
    cout << "=================================================\n";
    
    if (users.empty()) {
        cout << "No users found.\n";
        return;
    }
    
    cout << left << setw(5) << "ID" << setw(20) << "Username" 
         << setw(15) << "Type" << "Status\n";
    cout << string(60, '-') << "\n";
    
    for (auto &u : users) {
        if (u->getId() == currentUser->getId()) continue;
        
        cout << left << setw(5) << u->getId() 
             << setw(20) << u->getUsername()
             << setw(15) << u->getUserType();
        
        if (currentUser->isFriend(u->getId())) {
            cout << " Friend";
        } else {
            cout << " Add";
        }
        cout << "\n";
    }
    
    addActivity(currentUser->getId(), "Viewed all users");
}

void Network::searchUser(User* currentUser) {
    string searchName;
    cout << "\nEnter username to search: ";
    cin >> searchName;
    
    // Using hash map for O(1) lookup
    if (usernameIndex.find(searchName) != usernameIndex.end()) {
        int userId = usernameIndex[searchName];
        User* user = getUserById(userId);
        
        if (user) {
            cout << "\n User Found!\n";
            cout << *user << "\n";  // Operator overloading
            cout << "Account Type: " << user->getUserType() << "\n";
            cout << "Friends: " << user->getFriends().size() << "\n";
            
            if (currentUser->isFriend(userId)) {
                cout << "Status: Already friends \n";
            } else {
                cout << "Status: Not friends yet\n";
            }
        }
    } else {
        cout << " User not found.\n";
    }
    
    addActivity(currentUser->getId(), "Searched for user: " + searchName);
}

void Network::viewProfile(User* currentUser) {
    currentUser->displayProfile();
    
    PremiumUser* premiumUser = dynamic_cast<PremiumUser*>(currentUser);
    if (premiumUser) {
        cout << "Bonus Points: " << premiumUser->getBonusPoints() << " \n";
    }
    
    addActivity(currentUser->getId(), "Viewed own profile");
}

// ==================== FRIEND OPERATIONS ====================

void Network::sendFriendRequest(User* currentUser) {
    int targetId;
    cout << "\nEnter User ID to send friend request: ";
    cin >> targetId;
    
    if (targetId == currentUser->getId()) {
        cout << " You cannot send a friend request to yourself!\n";
        return;
    }
    
    User* targetUser = getUserById(targetId);
    if (!targetUser) {
        cout << " User not found.\n";
        return;
    }
    
    if (currentUser->isFriend(targetId)) {
        cout << " You are already friends with " << targetUser->getUsername() << "!\n";
        return;
    }
    
    // Check if request already exists
    queue<pair<int, int>> tempQueue = friendRequests;
    while (!tempQueue.empty()) {
        auto req = tempQueue.front();
        tempQueue.pop();
        if (req.first == currentUser->getId() && req.second == targetId) {
            cout << " Friend request already sent!\n";
            return;
        }
    }
    
    friendRequests.push({currentUser->getId(), targetId});
    cout << " Friend request sent to " << targetUser->getUsername() << "!\n";
    
    addActivity(currentUser->getId(), "Sent friend request to " + targetUser->getUsername());
    addActivity(targetId, "Received friend request from " + currentUser->getUsername());
}

void Network::viewFriendRequests(User* currentUser) {
    cout << "\n===============================================\n";
    cout << "           PENDING FRIEND REQUESTS             \n";
    cout << "=================================================\n";
    
    queue<pair<int, int>> tempQueue = friendRequests;
    vector<pair<int, int>> myRequests;
    vector<pair<int, int>> otherRequests;
    
    while (!tempQueue.empty()) {
        auto req = tempQueue.front();
        tempQueue.pop();
        
        if (req.second == currentUser->getId()) {
            myRequests.push_back(req);
        } else {
            otherRequests.push_back(req);
        }
    }
    
    if (myRequests.empty()) {
        cout << "No pending friend requests.\n";
        return;
    }
    
    for (size_t i = 0; i < myRequests.size(); i++) {
        User* sender = getUserById(myRequests[i].first);
        if (sender) {
            cout << (i+1) << ". " << *sender << "\n";  // Operator overloading
        }
    }
    
    cout << "\nEnter request number to accept (0 to cancel): ";
    int choice;
    cin >> choice;
    
    if (choice > 0 && choice <= (int)myRequests.size()) {
        int senderId = myRequests[choice-1].first;
        User* sender = getUserById(senderId);
        
        // Add friendship (both ways)
        currentUser->addFriend(senderId);
        sender->addFriend(currentUser->getId());
        
        // Operator overloading demonstration
        *currentUser + *sender;
        
        // Remove from queue
        friendRequests = queue<pair<int, int>>();
        for (auto req : otherRequests) {
            friendRequests.push(req);
        }
        for (size_t i = 0; i < myRequests.size(); i++) {
            if (i != (size_t)(choice-1)) {
                friendRequests.push(myRequests[i]);
            }
        }
        
        // Premium bonus
        PremiumUser* premiumSender = dynamic_cast<PremiumUser*>(sender);
        if (premiumSender) {
            premiumSender->addBonusPoints(10);
        }
        
        PremiumUser* premiumReceiver = dynamic_cast<PremiumUser*>(currentUser);
        if (premiumReceiver) {
            premiumReceiver->addBonusPoints(10);
        }
        
        addActivity(currentUser->getId(), "Accepted friend request from " + sender->getUsername());
        addActivity(senderId, "Friend request accepted by " + currentUser->getUsername());
        
        saveConnections();
    }
}

void Network::viewMyFriends(User* currentUser) {
    cout << "\n===============================================\n";
    cout << "              MY FRIENDS LIST                  \n";
    cout << "==================================================\n";
    
    vector<int> friends = currentUser->getFriends();
    
    if (friends.empty()) {
        cout << "You have no friends yet. Start connecting!\n";
        return;
    }
    
    cout << "Total Friends: " << friends.size() << "\n\n";
    
    for (size_t i = 0; i < friends.size(); i++) {
        User* friendUser = getUserById(friends[i]);
        if (friendUser) {
            cout << (i+1) << ". " << *friendUser << "\n";  // Operator overloading
        }
    }
    
    addActivity(currentUser->getId(), "Viewed friends list");
}

void Network::removeFriend(User* currentUser) {
    vector<int> friends = currentUser->getFriends();
    
    if (friends.empty()) {
        cout << " You have no friends to remove.\n";
        return;
    }
    
    cout << "\n--- Your Friends ---\n";
    for (size_t i = 0; i < friends.size(); i++) {
        User* friendUser = getUserById(friends[i]);
        if (friendUser) {
            cout << (i+1) << ". " << friendUser->getUsername() << " (ID: " << friends[i] << ")\n";
        }
    }
    
    cout << "\nEnter friend number to remove (0 to cancel): ";
    int choice;
    cin >> choice;
    
    if (choice > 0 && choice <= (int)friends.size()) {
        int friendId = friends[choice-1];
        User* friendUser = getUserById(friendId);
        
        currentUser->removeFriend(friendId);
        friendUser->removeFriend(currentUser->getId());
        
        cout << " " << friendUser->getUsername() << " removed from your friends list.\n";
        
        addActivity(currentUser->getId(), "Removed friend: " + friendUser->getUsername());
        addActivity(friendId, "Removed by: " + currentUser->getUsername());
        
        saveConnections();
    }
}

void Network::suggestFriends(User* currentUser) {
    cout << "\n===============================================\n";
    cout << "          FRIEND SUGGESTIONS                   \n";
    cout << "==================================================\n";
    
    unordered_map<int, int> mutualCount;
    vector<int> myFriends = currentUser->getFriends();
    
    // Find users with mutual friends
    for (int friendId : myFriends) {
        User* friendUser = getUserById(friendId);
        if (friendUser) {
            for (int friendOfFriend : friendUser->getFriends()) {
                if (friendOfFriend != currentUser->getId() && 
                    !currentUser->isFriend(friendOfFriend)) {
                    mutualCount[friendOfFriend]++;
                }
            }
        }
    }
    
    if (mutualCount.empty()) {
        cout << "No suggestions available at the moment.\n";
        return;
    }
    
    vector<pair<int, int>> suggestions(mutualCount.begin(), mutualCount.end());
    sort(suggestions.begin(), suggestions.end(), 
         [](const pair<int,int>& a, const pair<int,int>& b) {
             return a.second > b.second;
         });
    
    cout << "People you may know:\n\n";
    int count = 0;
    for (auto& sug : suggestions) {
        if (count >= 5) break;
        User* suggestedUser = getUserById(sug.first);
        if (suggestedUser) {
            cout << ++count << ". " << *suggestedUser 
                 << " (" << sug.second << " mutual friends)\n";
        }
    }
    
    addActivity(currentUser->getId(), "Viewed friend suggestions");
}

// ==================== MESSAGING ====================

void Network::sendMessage(User* currentUser) {
    int receiverId;
    string content;
    
    cout << "\nEnter recipient User ID: ";
    cin >> receiverId;
    
    if (receiverId == currentUser->getId()) {
        cout << " You cannot send a message to yourself!\n";
        return;
    }
    
    User* receiver = getUserById(receiverId);
    if (!receiver) {
        cout << " User not found.\n";
        return;
    }
    
    // Check message limit for non-premium users
    PremiumUser* premiumUser = dynamic_cast<PremiumUser*>(currentUser);
    if (!premiumUser) {
        // Non-premium users have a limit
        int messageCount = 0;
        stack<Message> tempStack = userMessages[currentUser->getId()];
        while (!tempStack.empty()) {
            messageCount++;
            tempStack.pop();
        }
        
        if (messageCount >= 10) {
            cout << " Message limit reached! Upgrade to Premium for unlimited messages.\n";
            return;
        }
    }
    
    cout << "Enter your message: ";
    cin.ignore();
    getline(cin, content);
    
    Message msg(currentUser->getId(), receiverId, 
                currentUser->getUsername(), content, getCurrentTime());
    
    userMessages[receiverId].push(msg);
    
    cout << " Message sent to " << receiver->getUsername() << "!\n";
    
    addActivity(currentUser->getId(), "Sent message to " + receiver->getUsername());
    addActivity(receiverId, "Received message from " + currentUser->getUsername());
}

void Network::viewMessages(User* currentUser) {
    cout << "\n===============================================\n";
    cout << "              YOUR MESSAGES                    \n";
    cout << "==================================================\n";
    
    if (userMessages[currentUser->getId()].empty()) {
        cout << "No messages yet.\n";
        return;
    }
    
    stack<Message> tempStack = userMessages[currentUser->getId()];
    vector<Message> messages;
    
    while (!tempStack.empty()) {
        messages.push_back(tempStack.top());
        tempStack.pop();
    }
    
    cout << "Total Messages: " << messages.size() << "\n\n";
    
    for (size_t i = 0; i < messages.size(); i++) {
        cout << "=======================================\n";
        cout << "From: " << messages[i].senderName << " (ID: " << messages[i].senderId << ")\n";
        cout << "Time: " << messages[i].timestamp << "\n";
        cout << "Message: " << messages[i].content << "\n";
    }
    cout << "==========================================\n";
    
    addActivity(currentUser->getId(), "Viewed messages");
}

void Network::viewMessageCount(User* currentUser) {
    int count = userMessages[currentUser->getId()].size();
    cout << "\n You have " << count << " message(s).\n";
    
    PremiumUser* premiumUser = dynamic_cast<PremiumUser*>(currentUser);
    if (premiumUser) {
        cout << " As a Premium user, you can send unlimited messages!\n";
    } else {
        cout << " Free users can send up to 10 messages. Upgrade to Premium for unlimited!\n";
    }
}

// ==================== ACTIVITY & ANALYTICS ====================

void Network::viewActivityLog(User* currentUser) {
    cout << "\n===============================================\n";
    cout << "           RECENT ACTIVITY LOG                 \n";
    cout << "==================================================\n";
    
    if (activityLogs.find(currentUser->getId()) == activityLogs.end()) {
        cout << "No activities recorded yet.\n";
        return;
    }
    
    ActivityNode* current = activityLogs[currentUser->getId()];
    int count = 0;
    
    while (current && count < 15) {
        cout << "=> " << current->activity << "\n";
        cout << "   " << current->timestamp << "\n\n";
        current = current->next;
        count++;
    }
}

void Network::viewNetworkStats(User* currentUser) {
    cout << "\n===============================================\n";
    cout << "          NETWORK STATISTICS                   \n";
    cout << "==================================================\n";
    
    int totalUsers = users.size();
    int premiumUsers = 0;
    int totalConnections = 0;
    
    for (auto u : users) {
        if (dynamic_cast<PremiumUser*>(u)) {
            premiumUsers++;
        }
        totalConnections += u->getFriends().size();
    }
    totalConnections /= 2; // Each connection counted twice
    
    cout << " Total Users: " << totalUsers << "\n";
    cout << " Premium Users: " << premiumUsers << "\n";
    cout << " Total Friendships: " << totalConnections << "\n";
    cout << " Your Friends: " << currentUser->getFriends().size() << "\n";
    cout << " Your Messages: " << userMessages[currentUser->getId()].size() << "\n";
    
    // Calculate average friends
    double avgFriends = totalUsers > 0 ? (double)totalConnections * 2 / totalUsers : 0;
    cout << " Average Friends per User: " << fixed << setprecision(1) << avgFriends << "\n";
    
    addActivity(currentUser->getId(), "Viewed network statistics");
}

// ==================== GRAPH ALGORITHMS ====================

void Network::findShortestPath(User* currentUser) {
    int targetId;
    cout << "\nEnter target User ID to find shortest path: ";
    cin >> targetId;
    
    if (targetId == currentUser->getId()) {
        cout << " You are already at your own profile!\n";
        return;
    }
    
    User* targetUser = getUserById(targetId);
    if (!targetUser) {
        cout << " User not found.\n";
        return;
    }
    
    int n = users.size();
    vector<int> dist(n+1, INT_MAX);
    vector<int> parent(n+1, -1);
    vector<bool> visited(n+1, false);
    
    dist[currentUser->getId()] = 0;
    
    // Dijkstra's Algorithm
    for (int i = 0; i < n; i++) {
        int minDist = INT_MAX;
        int u = -1;
        
        for (int j = 1; j <= n; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }
        
        if (u == -1 || minDist == INT_MAX) break;
        
        visited[u] = true;
        
        User* currentNode = getUserById(u);
        if (!currentNode) continue;
        
        for (int friendId : currentNode->getFriends()) {
            if (dist[u] + 1 < dist[friendId]) {
                dist[friendId] = dist[u] + 1;
                parent[friendId] = u;
            }
        }
    }
    
    if (dist[targetId] == INT_MAX) {
        cout << " No connection path found to " << targetUser->getUsername() << ".\n";
        return;
    }
    
    // Reconstruct path
    vector<int> path;
    int current = targetId;
    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }
    reverse(path.begin(), path.end());
    
    cout << "\n Shortest Connection Path (Distance: " << dist[targetId] << " hops):\n\n";
    
    for (size_t i = 0; i < path.size(); i++) {
        User* user = getUserById(path[i]);
        if (user) {
            cout << user->getUsername() << " (ID: " << path[i] << ")";
            if (i < path.size() - 1) {
                cout << " → ";
            }
        }
    }
    cout << "\n";
    
    addActivity(currentUser->getId(), "Found shortest path to " + targetUser->getUsername());
}

void Network::findMutualFriends(User* currentUser) {
    int otherId;
    cout << "\nEnter User ID to find mutual friends: ";
    cin >> otherId;
    
    if (otherId == currentUser->getId()) {
        cout << " Cannot find mutual friends with yourself!\n";
        return;
    }
    
    User* otherUser = getUserById(otherId);
    if (!otherUser) {
        cout << " User not found.\n";
        return;
    }
    
    vector<int> myFriends = currentUser->getFriends();
    vector<int> theirFriends = otherUser->getFriends();
    vector<int> mutualFriends;
    
    // Find intersection using sets
    for (int friendId : myFriends) {
        if (find(theirFriends.begin(), theirFriends.end(), friendId) != theirFriends.end()) {
            mutualFriends.push_back(friendId);
        }
    }
    
    cout << "\n================================================\n";
    cout << "         MUTUAL FRIENDS WITH " << otherUser->getUsername() << "\n";
    cout << "==================================================\n";
    
    if (mutualFriends.empty()) {
        cout << "No mutual friends found.\n";
        return;
    }
    
    cout << "Total Mutual Friends: " << mutualFriends.size() << "\n\n";
    
    for (size_t i = 0; i < mutualFriends.size(); i++) {
        User* mutualUser = getUserById(mutualFriends[i]);
        if (mutualUser) {
            cout << (i+1) << ". " << *mutualUser << "\n";  // Operator overloading
        }
    }
    
    addActivity(currentUser->getId(), "Found mutual friends with " + otherUser->getUsername());
}

// ==================== PREMIUM FEATURES ====================

void Network::upgradeToPremium(User* currentUser) {
    PremiumUser* premiumUser = dynamic_cast<PremiumUser*>(currentUser);
    
    if (premiumUser) {
        cout << " You are already a Premium user!\n";
        premiumUser->showPremiumFeatures();
        return;
    }
    
    cout << "\n===============================================\n";
    cout << "        UPGRADE TO PREMIUM                     \n";
    cout << "=================================================\n";
    
    cout << "\n Premium Benefits:\n";
    cout << "   Unlimited Messages\n";
    cout << "   Priority Friend Requests\n";
    cout << "   Bonus Points Rewards\n";
    cout << "   Advanced Network Analytics\n";
    cout << "   Ad-Free Experience\n";
    cout << "   Custom Profile Themes\n";
    
    cout << "\n Subscription Cost: Rs. 200\n";
    cout << "\nDo you want to upgrade? (1 = Yes, 0 = No): ";
    
    int choice;
    cin >> choice;
    
    if (choice != 1) {
        cout << "Upgrade cancelled.\n";
        return;
    }
    
    int amount;
    cout << "Enter payment amount: Rs. ";
    cin >> amount;
    
    if (amount != 200) {
        cout << " Invalid amount! Upgrade cancelled.\n";
        return;
    }
    
    // Create new premium user
    PremiumUser* newPremiumUser = new PremiumUser(
        currentUser->getId(),
        currentUser->getUsername(),
        currentUser->getPassword()
    );
    
    // Transfer friends
    for (int friendId : currentUser->getFriends()) {
        newPremiumUser->addFriend(friendId);
    }
    
    // Replace in users vector
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i]->getId() == currentUser->getId()) {
            delete users[i];
            users[i] = newPremiumUser;
            break;
        }
    }
    
    cout << " Successfully upgraded to Premium! Welcome to the Premium club! \n";
    newPremiumUser->addBonusPoints(50); // Welcome bonus
    
    saveUsers();
    addActivity(newPremiumUser->getId(), "Upgraded to Premium account");
}

void Network::viewPremiumFeatures(User* currentUser) {
    PremiumUser* premiumUser = dynamic_cast<PremiumUser*>(currentUser);
    
    if (!premiumUser) {
        cout << "\n This feature is only available for Premium users.\n";
        cout << " Upgrade to Premium to unlock all features!\n";
        return;
    }
    
    premiumUser->showPremiumFeatures();
    addActivity(currentUser->getId(), "Viewed premium features");
}

// ==================== ACCOUNT MANAGEMENT ====================

void Network::deleteAccount(User* currentUser) {
    cout << "\n  WARNING: This action cannot be undone!\n";
    cout << "Are you sure you want to delete your account? (1 = Yes, 0 = No): ";
    
    int choice;
    cin >> choice;
    
    if (choice != 1) {
        cout << "Account deletion cancelled.\n";
        return;
    }
    
    int userId = currentUser->getId();
    
    // Remove from all friends' friend lists
    for (auto u : users) {
        if (u->isFriend(userId)) {
            u->removeFriend(userId);
        }
    }
    
    // Remove from users vector
    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it)->getId() == userId) {
            delete *it;
            users.erase(it);
            break;
        }
    }
    
    // Remove from username index
    usernameIndex.erase(currentUser->getUsername());
    
    // Remove messages
    userMessages.erase(userId);
    
    // Remove activities
    if (activityLogs.find(userId) != activityLogs.end()) {
        ActivityNode* current = activityLogs[userId];
        while (current) {
            ActivityNode* temp = current;
            current = current->next;
            delete temp;
        }
        activityLogs.erase(userId);
    }
    
    cout << " Account deleted successfully. Goodbye!\n";
    
    saveUsers();
    saveConnections();
    saveMessages();
    saveActivities();
}

// ==================== OPERATOR OVERLOADING DEMO ====================

void Network::demonstrateOperators(User* currentUser) {
    cout << "\n===============================================\n";
    cout << "     OPERATOR OVERLOADING DEMONSTRATION        \n";
    cout << "==================================================\n";
    
    cout << "\n1. Stream Insertion Operator (<<):\n";
    cout << "   " << *currentUser << "\n";
    
    if (!users.empty() && users[0]->getId() != currentUser->getId()) {
        cout << "\n2. Addition Operator (+) for friendship:\n";
        cout << "   ";
        *currentUser + *users[0];
    }
    
    cout << "\n3. Equality Operator (==):\n";
    User* sameUser = currentUser;
    if (*currentUser == *sameUser) {
        cout << "    Same user detected!\n";
    }
    
    cout << "\n4. Inequality Operator (!=):\n";
    if (!users.empty() && users[0]->getId() != currentUser->getId()) {
        if (*currentUser != *users[0]) {
            cout << "    Different users detected!\n";
        }
    }
    
    PremiumUser* premiumUser = dynamic_cast<PremiumUser*>(currentUser);
    if (premiumUser) {
        cout << "\n5. Premium User Compound Assignment (+=, -=):\n";
        cout << "   Current Bonus Points: " << premiumUser->getBonusPoints() << "\n";
        *premiumUser += 50;
        cout << "   After += 50: " << premiumUser->getBonusPoints() << "\n";
        *premiumUser -= 20;
        cout << "   After -= 20: " << premiumUser->getBonusPoints() << "\n";
    }
    
    addActivity(currentUser->getId(), "Viewed operator overloading demo");
}