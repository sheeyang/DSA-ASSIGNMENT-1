#pragma once

#include "imgui.h"
#include "types.h"
#include "globals.h"

// Setup custom ImGui theme
void SetupImGuiStyle();

// UI rendering functions
void renderSummarySection();
void renderTransactionsByType();
void renderAllTransactions();
void renderAccountsBreakdown(bool &show_delete_account, int &deleteBufferAccountIndex, int &transferBufferAccountIndex, bool &show_add_account, Account &new_account);
void renderAddAccountWindow(bool &show_add_account, Account &new_account, Transaction &new_transaction);
void renderDeleteAccountWindow(bool &show_delete_account, int &deleteBufferAccountIndex, int &transferBufferAccountIndex);
void renderCategoryWindow(bool &show_category_window);
void renderAddTransactionWindow(bool &show_add_transaction_window, Transaction &new_transaction, int &selected_tab_for_new);
