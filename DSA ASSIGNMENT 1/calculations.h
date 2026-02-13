#pragma once

#include "types.h"
#include "globals.h"
#include <string>
#include <ctime>

// Get current date as string
std::string getCurrentDate()
{
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
    return std::string(buffer);
}

// Calculate totals
float calculateTotalIncome()
{
    float total = 0.0f;
    for (const auto &t : g_transactions)
    {
        if (t.type == TransactionType::Income)
            total += t.amount;
    }
    return total;
}

float calculateTotalExpense()
{
    float total = 0.0f;
    for (const auto &t : g_transactions)
    {
        if (t.type == TransactionType::Expense)
            total += t.amount;
    }
    return total;
}

float calculateBalance()
{
    return calculateTotalIncome() - calculateTotalExpense();
}
