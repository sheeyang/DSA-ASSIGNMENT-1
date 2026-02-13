#pragma once

#include <string>
#include <cstring>

// Transaction types
enum class TransactionType
{
    Income,
    Expense,
    Transfer
};

// View modes
enum class ViewMode
{
    ByType,
    All
};

struct Account
{
    std::string accountName;
    float amount;
    bool deleted;

    Account()
    {
        accountName = "";
        amount = 0.0f;
        deleted = false;
    }

    void withdraw(float amt) { amount -= amt; }
    void deposit(float amt) { amount += amt; }
    void transfer(Account *to, float amt)
    {
        amount -= amt;
        to->amount += amt;
    }

    bool operator==(const Account &other) const
    {
        return (accountName == other.accountName);
    }
};

// Transaction structure
struct Transaction
{
    TransactionType type;
    char description[256];
    float amount;
    std::string category;
    char date[32];

    Account *fromAccount; // For transfers
    Account *toAccount;   // For transfers

    Transaction()
    {
        type = TransactionType::Income;
        memset(description, 0, sizeof(description));
        category = "";
        memset(date, 0, sizeof(date));
        fromAccount = nullptr;
        toAccount = nullptr;
        amount = 0.0f;

        // Set default date to today
        time_t now = time(nullptr);
        struct tm timeinfo;
        localtime_s(&timeinfo, &now);
        strftime(date, sizeof(date), "%Y-%m-%d", &timeinfo);
    }
};
