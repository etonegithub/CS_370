#include <string>
#include <iostream>
#include "bank_account.h"
using namespace std;

int main(int argc, char *argv[]) {
    string input;
    double num;

    BankAccount account;

    while (cin >> input && input != "exit") {
        if (input == "balance") {
            cout << account.balance() << endl;
        } else if (input == "deposit") {
            cin >> num;
            account.deposit(num);
        } else if (input == "withdraw") {
            cin >> num;
            account.withdraw(num);
        }
    }
}