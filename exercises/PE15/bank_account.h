#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

class BankAccount {
    public:
        BankAccount();
        double balance();
        void deposit(double amount);
        void withdraw(double amount);
    private:
        double current_balance;
};

#endif