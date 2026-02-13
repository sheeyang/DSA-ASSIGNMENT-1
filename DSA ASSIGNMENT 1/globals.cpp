#include "globals.h"

// Color scheme (darker colors for better readability)
ImVec4 COLOR_INCOME = ImVec4(0.0f, 0.6f, 0.0f, 1.0f);   // Dark green
ImVec4 COLOR_EXPENSE = ImVec4(0.7f, 0.0f, 0.0f, 1.0f);  // Dark red
ImVec4 COLOR_TRANSFER = ImVec4(0.0f, 0.4f, 0.8f, 1.0f); // Dark blue
ImVec4 COLOR_POSITIVE = ImVec4(0.0f, 0.6f, 0.0f, 1.0f); // Dark green
ImVec4 COLOR_NEGATIVE = ImVec4(0.7f, 0.0f, 0.0f, 1.0f); // Dark red

// Global transaction storage
std::vector<Transaction> g_transactions;

// Category list
std::vector<std::string> categories{"FOOD", "SOCIAL", "SHOPPING", "TRANSPORT", "EDUCATION", "SALARY", "INTEREST"};
std::string _currentCategory;
char _inputCategoryBuffer[TEXT_BUFFER]{'\0'};
int _categoryIndex = 0;
bool _inputFieldEmpty{true};

// Accounts list
std::vector<Account> accounts;
int _fromAccountIndex = 0;
int _toAccountIndex = 0;
char _inputAccountBuffer[TEXT_BUFFER]{'\0'};
