#include "Network.h"
#include <iostream>
using namespace std;

int main() {
    Network network;
    int choice;
    User* currentUser = nullptr;

    do {
        cout << "\n=========== MAIN MENU ===========\n";
        cout << "1. Sign Up\n";
        cout << "2. Sign In\n";
        cout << "3. Exit\n";
        cout << "=================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                network.signUp();
                break;

            case 2:
                currentUser = network.signIn();
                if (currentUser) {
                    currentUser->showDashboard();  // Polymorphic call
                    cout << "\n(Returning to main menu...)\n";
                }
                break;

            case 3:
                cout << "Exiting program. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 3);

    return 0;
}
