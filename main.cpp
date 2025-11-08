#include "Network.h"
#include <iostream>
#include <limits>

using namespace std;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void displayMainMenu() {
    cout << "\n==============================================\n";
    cout << "                                               \n";
    cout << "          SOCIAL NETWORK SYSTEM                \n";
    cout << "        C++ Project - Enhanced                \n";
    cout << "                                               \n";
    cout << "================================================\n";
    cout << "\n";
    cout << "  1.  Sign Up (Create New Account)\n";
    cout << "  2.  Sign In (Login to Account)\n";
    cout << "  3.  Exit Program\n";
    cout << "\n===============================================\n";
    cout << "Enter your choice: ";
}

void displayUserDashboard(User* currentUser) {
    cout << "\n===============================================\n";
    cout << "                                               \n";
    cout << "           USER DASHBOARD                      \n";
    cout << "   Welcome, " << currentUser->getUsername();
    int spaces = 32 - currentUser->getUsername().length();
    for (int i = 0; i < spaces; i++) cout << " ";
    cout << "\n";
    cout << "                                               \n";
    cout << "================================================\n";
    cout << "\n";
    cout << "   USER MANAGEMENT\n";
    cout << "================================================\n";
    cout << "   1.  View My Profile\n";
    cout << "   2.  View All Users\n";
    cout << "   3.  Search User\n";
    cout << "\n";
    cout << "   FRIEND OPERATIONS\n";
    cout << "=================================================\n";
    cout << "   4.  Send Friend Request\n";
    cout << "   5.  View Friend Requests\n";
    cout << "   6.  View My Friends\n";
    cout << "   7.  Remove Friend\n";
    cout << "   8.  Friend Suggestions\n";
    cout << "\n";
    cout << "   MESSAGING\n";
    cout << "==================================================\n";
    cout << "   9.  Send Message\n";
    cout << "  10.  View My Messages\n";
    cout << "  11.  View Message Count\n";
    cout << "\n";
    cout << "   ANALYTICS & ALGORITHMS\n";
    cout << "===================================================\n";
    cout << "  12.  View Activity Log\n";
    cout << "  13.  Network Statistics\n";
    cout << "  14.  Find Shortest Path (Dijkstra)\n";
    cout << "  15.  Find Mutual Friends\n";
    cout << "\n";
    cout << "   PREMIUM & ADVANCED\n";
    cout << "==================================================\n";
    cout << "  16.  Upgrade to Premium\n";
    cout << "  17.  View Premium Features\n";
    cout << "  18.  Operator Overloading Demo\n";
    cout << "\n";
    cout << "    ACCOUNT SETTINGS\n";
    cout << "==================================================\n";
    cout << "  19.  Delete Account\n";
    cout << "  20.  Log Out\n";
    cout << "\n================================================\n";
    cout << "Enter your choice: ";
}

int main() {
    Network network;
    int choice;
    User* currentUser = nullptr;

    do {
        if (currentUser == nullptr) {
            displayMainMenu();
            cin >> choice;

            switch (choice) {
                case 1:
                    network.signUp();
                    pauseScreen();
                    break;

                case 2:
                    currentUser = network.signIn();
                    if (currentUser) {
                        currentUser->showDashboard();
                        pauseScreen();
                    } else {
                        pauseScreen();
                    }
                    break;

                case 3:
                    cout << "\n Thank you for using Social Network System!\n";
                    cout << "Goodbye! \n\n";
                    break;

                default:
                    cout << " Invalid choice! Please try again.\n";
                    pauseScreen();
            }
        } else {
            // User is logged in
            displayUserDashboard(currentUser);
            cin >> choice;

            switch (choice) {
                case 1:
                    network.viewProfile(currentUser);
                    pauseScreen();
                    break;

                case 2:
                    network.viewAllUsers(currentUser);
                    pauseScreen();
                    break;

                case 3:
                    network.searchUser(currentUser);
                    pauseScreen();
                    break;

                case 4:
                    network.sendFriendRequest(currentUser);
                    pauseScreen();
                    break;

                case 5:
                    network.viewFriendRequests(currentUser);
                    pauseScreen();
                    break;

                case 6:
                    network.viewMyFriends(currentUser);
                    pauseScreen();
                    break;

                case 7:
                    network.removeFriend(currentUser);
                    pauseScreen();
                    break;

                case 8:
                    network.suggestFriends(currentUser);
                    pauseScreen();
                    break;

                case 9:
                    network.sendMessage(currentUser);
                    pauseScreen();
                    break;

                case 10:
                    network.viewMessages(currentUser);
                    pauseScreen();
                    break;

                case 11:
                    network.viewMessageCount(currentUser);
                    pauseScreen();
                    break;

                case 12:
                    network.viewActivityLog(currentUser);
                    pauseScreen();
                    break;

                case 13:
                    network.viewNetworkStats(currentUser);
                    pauseScreen();
                    break;

                case 14:
                    network.findShortestPath(currentUser);
                    pauseScreen();
                    break;

                case 15:
                    network.findMutualFriends(currentUser);
                    pauseScreen();
                    break;

                case 16:
                    network.upgradeToPremium(currentUser);
                    // Need to refresh currentUser pointer after upgrade
                    currentUser = network.signIn();
                    pauseScreen();
                    break;

                case 17:
                    network.viewPremiumFeatures(currentUser);
                    pauseScreen();
                    break;

                case 18:
                    network.demonstrateOperators(currentUser);
                    pauseScreen();
                    break;

                case 19:
                    network.deleteAccount(currentUser);
                    currentUser = nullptr; // Log out after deletion
                    pauseScreen();
                    break;

                case 20:
                    cout << "\n Logged out successfully!\n";
                    currentUser = nullptr;
                    pauseScreen();
                    break;

                default:
                    cout << " Invalid choice! Please try again.\n";
                    pauseScreen();
            }
        }
    } while (choice != 3 || currentUser != nullptr);

    return 0;
}