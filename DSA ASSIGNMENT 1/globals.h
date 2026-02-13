#pragma once

#include "types.h"
#include "imgui.h"
#include <vector>
#include <string>

// Window dimensions
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
constexpr int TEXT_BUFFER = 256;

// Color scheme
extern ImVec4 COLOR_INCOME;
extern ImVec4 COLOR_EXPENSE;
extern ImVec4 COLOR_TRANSFER;
extern ImVec4 COLOR_POSITIVE;
extern ImVec4 COLOR_NEGATIVE;

// Global transaction storage
extern std::vector<Transaction> g_transactions;

// Category list
extern std::vector<std::string> categories;
extern std::string _currentCategory;
extern char _inputCategoryBuffer[TEXT_BUFFER];
extern int _categoryIndex;
extern bool _inputFieldEmpty;

// Accounts list
extern std::vector<Account> accounts;
extern int _fromAccountIndex;
extern int _toAccountIndex;
extern char _inputAccountBuffer[TEXT_BUFFER];
