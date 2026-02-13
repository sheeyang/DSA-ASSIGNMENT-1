#include "globals.h"

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
