#include "bank_account.h"

BankAccount::BankAccount() : current_balance(0) {}

double BankAccount::balance() {
    return current_balance;
}

void BankAccount::deposit(double amount) {
    if (amount > 0) {
        current_balance += amount;
    }
}

void BankAccount::withdraw(double amount) {
    if (amount > 0 && amount <= current_balance) {
        current_balance -= amount;
    }
}