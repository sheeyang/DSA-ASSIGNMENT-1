#include "ui.h"
#include "calculations.h"
#include "imgui.h"
#include <cstring>

void renderSummarySection()
{
    ImGui::Text("=== Account Summary ===");
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "Total Income:  $%.2f", calculateTotalIncome());
    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Total Expense: $%.2f", calculateTotalExpense());
    float balance = calculateBalance();
    ImVec4 balanceColor = balance >= 0 ? ImVec4(0.4f, 1.0f, 0.4f, 1.0f) : ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
    ImGui::TextColored(balanceColor, "Balance:       $%.2f", balance);
    ImGui::Separator();
    ImGui::Spacing();
}

void renderTransactionsByType()
{
    if (ImGui::BeginTabBar("TransactionTabs"))
    {
        // Income Tab
        if (ImGui::BeginTabItem("Income"))
        {
            ImGui::Text("Income Transactions");
            ImGui::Separator();

            if (ImGui::BeginTable("IncomeTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
            {
                ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Category", ImGuiTableColumnFlags_WidthFixed, 120.0f);
                ImGui::TableSetupColumn("Amount", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableHeadersRow();

                for (size_t i = 0; i < g_transactions.size(); i++)
                {
                    const auto &t = g_transactions[i];
                    if (t.type == TransactionType::Income)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text("%s", t.date);
                        ImGui::TableNextColumn();
                        ImGui::Text("%s", t.description);
                        ImGui::TableNextColumn();
                        ImGui::Text(t.category.c_str());
                        ImGui::TableNextColumn();
                        ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "+$%.2f", t.amount);
                    }
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        // Expense Tab
        if (ImGui::BeginTabItem("Expense"))
        {
            ImGui::Text("Expense Transactions");
            ImGui::Separator();

            if (ImGui::BeginTable("ExpenseTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
            {
                ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Category", ImGuiTableColumnFlags_WidthFixed, 120.0f);
                ImGui::TableSetupColumn("Amount", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableHeadersRow();

                for (size_t i = 0; i < g_transactions.size(); i++)
                {
                    const auto &t = g_transactions[i];
                    if (t.type == TransactionType::Expense)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text("%s", t.date);
                        ImGui::TableNextColumn();
                        ImGui::Text("%s", t.description);
                        ImGui::TableNextColumn();
                        ImGui::Text(t.category.c_str());
                        ImGui::TableNextColumn();
                        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "-$%.2f", t.amount);
                    }
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        // Transfer Tab
        if (ImGui::BeginTabItem("Transfer"))
        {
            ImGui::Text("Transfer Transactions");
            ImGui::Separator();

            if (ImGui::BeginTable("TransferTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
            {
                ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("From", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("To", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Amount", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableHeadersRow();

                for (size_t i = 0; i < g_transactions.size(); i++)
                {
                    const auto &t = g_transactions[i];
                    if (t.type == TransactionType::Transfer)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text("%s", t.date);
                        ImGui::TableNextColumn();
                        ImGui::Text("%s", t.description);
                        ImGui::TableNextColumn();
                        ImGui::Text(t.fromAccount->accountName.c_str());
                        ImGui::TableNextColumn();
                        ImGui::Text(t.toAccount->accountName.c_str());
                        ImGui::TableNextColumn();
                        ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "$%.2f", t.amount);
                    }
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}

void renderAllTransactions()
{
    ImGui::Text("All Transactions");
    ImGui::Separator();

    if (ImGui::BeginTable("AllTransactionsTable", 8, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable))
    {
        ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Account", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Category", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableSetupColumn("Details", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Amount", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Modify", ImGuiTableColumnFlags_WidthFixed, 50.0f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < g_transactions.size(); i++)
        {
            ImGui::PushID(i);

            const auto &t = g_transactions[i];
            ImGui::TableNextRow();

            // Date
            ImGui::TableNextColumn();
            ImGui::Text("%s", t.date);

            // Type
            ImGui::TableNextColumn();
            if (t.type == TransactionType::Income)
                ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "Income");
            else if (t.type == TransactionType::Expense)
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Expense");
            else
                ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "Transfer");

            // Description
            ImGui::TableNextColumn();
            ImGui::Text("%s", t.description);

            // Account
            ImGui::TableNextColumn();
            ImGui::Text(t.fromAccount->accountName.c_str());

            // Category
            ImGui::TableNextColumn();
            ImGui::Text(t.category.c_str());

            // Details (To account for transfers, empty for others)
            ImGui::TableNextColumn();
            if (t.type == TransactionType::Transfer)
            {
                std::string toAccountString = "-> " + t.toAccount->accountName;
                ImGui::Text(toAccountString.c_str());
            }
            else
                ImGui::Text("-");

            // Amount
            ImGui::TableNextColumn();
            if (t.type == TransactionType::Income)
                ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "+$%.2f", t.amount);
            else if (t.type == TransactionType::Expense)
                ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "-$%.2f", t.amount);
            else
                ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "$%.2f", t.amount);

            ImGui::TableNextColumn();

            if (ImGui::Button("Delete"))
            {
                // take the money from account if its income and vice versa
                if (g_transactions[i].type == TransactionType::Income)
                {
                    g_transactions[i].fromAccount->withdraw(g_transactions[i].amount);
                }
                else if (g_transactions[i].type == TransactionType::Expense)
                {
                    g_transactions[i].fromAccount->deposit(g_transactions[i].amount);
                }
                else
                {
                    // basically undos the transfer
                    g_transactions[i].fromAccount->deposit(g_transactions[i].amount);
                    g_transactions[i].toAccount->withdraw(g_transactions[i].amount);
                }

                g_transactions.erase(g_transactions.begin() + i);
            }

            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}

void renderAccountsBreakdown(bool &show_delete_account, int &deleteBufferAccountIndex, int &transferBufferAccountIndex, bool &show_add_account, Account &new_account)
{
    ImGui::Text("=== Accounts Breakdown ===");

    if (ImGui::BeginTable("AccountsTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Account", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Amount", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Modify", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < accounts.size(); i++)
        {
            if (accounts[i].deleted)
                continue; // skip if deleted

            ImGui::PushID(i);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text(accounts[i].accountName.c_str());
            ImGui::TableNextColumn();

            // tried to to ternary here but idk why it didnt work so i did this instead
            ImVec4 green = {0.4f, 1.0f, 0.4f, 1.0f};
            ImVec4 red = {1.0f, 0.4f, 0.4f, 1.0f};

            ImVec4 textColour = accounts[i].amount > 0.0f ? green : red;
            ImGui::TextColored(textColour, "$%.2f", accounts[i].amount);

            ImGui::TableNextColumn();
            if (ImGui::Button("Delete"))
            {
                if (accounts.size() > 1)
                {

                    // there is balance inside
                    if (accounts[i].amount != 0)
                    {
                        deleteBufferAccountIndex = i;
                        show_delete_account = true;
                    }
                    else
                    {
                        accounts.erase(accounts.begin() + i);
                    }
                }
            }

            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    if (ImGui::Button("Add Account"))
    {
        new_account = Account();
        show_add_account = true;
    }
}

void renderAddAccountWindow(bool &show_add_account, Account &new_account, Transaction &new_transaction)
{
    ImGui::SetWindowSize(ImVec2(400, 350), ImGuiCond_FirstUseEver);
    ImGui::Begin("Add Account", &show_add_account);

    ImGui::SetNextItemWidth(200);
    if (ImGui::InputText("##AccountName", _inputAccountBuffer, sizeof(_inputAccountBuffer)))
    {
    }
    ImGui::SameLine();
    ImGui::Text("Account Name");

    ImGui::InputFloat("Amount ($)", &new_account.amount, 1.0f, 100.0f, "%.2f");

    _inputFieldEmpty = _inputAccountBuffer[0] == '\0';
    if (_inputFieldEmpty)
        ImGui::BeginDisabled();

    if (ImGui::Button("Add"))
    {
        new_account.accountName = _inputAccountBuffer;

        // make a new account and set add to list
        accounts.emplace_back(new_account);

        if (new_account.amount)
        {
            new_transaction = Transaction();

            new_transaction.amount = new_account.amount;
            new_transaction.fromAccount = &accounts[accounts.size() - 1]; // sets to the last one since u place it at the back
            new_transaction.category = "NEW ACCOUNT";
            new_transaction.type = TransactionType::Income;
            g_transactions.emplace_back(new_transaction);

            new_transaction = Transaction();
        }

        // need to clear the string
        _inputAccountBuffer[0] = '\0';
        show_add_account = false;
        new_account = Account(); // reset
    }

    if (_inputFieldEmpty)
        ImGui::EndDisabled();

    ImGui::End();
}

void renderDeleteAccountWindow(bool &show_delete_account, int &deleteBufferAccountIndex, int &transferBufferAccountIndex)
{
    ImGui::SetWindowSize(ImVec2(400, 350), ImGuiCond_FirstUseEver);
    ImGui::Begin("Delete Account", &show_delete_account);

    ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f),
                       "There is an outstanding balance of $%.2f in %s", accounts[deleteBufferAccountIndex].amount, accounts[deleteBufferAccountIndex].accountName.c_str());
    ImGui::Text("Select an Account to transfer the balance to:");

    if (ImGui::BeginCombo("##Account", accounts[_fromAccountIndex].accountName.c_str()))
    {
        for (int i = 0; i < accounts.size(); ++i)
        {
            if (accounts[i].deleted)
                continue; // skip if deleted
            // skip the one being deleted
            if (i == deleteBufferAccountIndex)
                continue;

            const bool isSelected = (_fromAccountIndex == i);
            if (ImGui::Selectable(accounts[i].accountName.c_str(), isSelected))
                _fromAccountIndex = i;
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    transferBufferAccountIndex = _fromAccountIndex;
    ImGui::SameLine();
    ImGui::Text("Account");

    if (ImGui::Button("Transfer"))
    {
        // check that the selected account is not the same
        if (accounts[transferBufferAccountIndex] != accounts[deleteBufferAccountIndex])
        {

            std::string deletedAccountName = accounts[deleteBufferAccountIndex].accountName;
            accounts[deleteBufferAccountIndex].transfer(&accounts[transferBufferAccountIndex], accounts[deleteBufferAccountIndex].amount);
            accounts[deleteBufferAccountIndex].deleted = true;

            for (Transaction &t : g_transactions)
            {
                if (t.fromAccount->accountName == deletedAccountName)
                {
                    t.fromAccount = &accounts[transferBufferAccountIndex];
                }
                if (t.toAccount && t.toAccount->accountName == deletedAccountName)
                {
                    t.toAccount = &accounts[transferBufferAccountIndex];
                }
            }

            show_delete_account = false;

            // reset
            transferBufferAccountIndex = -1;
            deleteBufferAccountIndex = -1;

            for (int i = 0; i < accounts.size(); ++i)
            {
                if (accounts[i].deleted)
                    continue;

                // finds the first none deleted account
                _fromAccountIndex = i;
                _toAccountIndex = i;
                break;
            }
        }
    }
    ImGui::End();
}

void renderCategoryWindow(bool &show_category_window)
{
    ImGui::SetWindowSize(ImVec2(400, 350), ImGuiCond_FirstUseEver);
    ImGui::Begin("Categories", &show_category_window);

    ImGui::Text("=== Categories ===");
    ImGui::SetNextItemWidth(200);
    if (ImGui::InputText("##CategoryName", _inputCategoryBuffer, sizeof(_inputCategoryBuffer)))
    {
    }
    ImGui::SameLine();

    _inputFieldEmpty = _inputCategoryBuffer[0] == '\0';
    if (_inputFieldEmpty)
        ImGui::BeginDisabled();

    if (ImGui::Button("Add"))
    {
        categories.emplace_back(_inputCategoryBuffer);
        // need to clear the string
        _inputCategoryBuffer[0] = '\0';
    }

    if (_inputFieldEmpty)
        ImGui::EndDisabled();

    if (ImGui::BeginTable("CategoryTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Category", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Modify", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableHeadersRow();

        for (int i = 0; i < categories.size(); i++)
        {
            ImGui::PushID(i);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text(categories[i].c_str());
            ImGui::TableNextColumn();

            if (ImGui::Button("Delete"))
            {
                if (categories.size() > 1)
                {
                    categories.erase(categories.begin() + i); // delete currently selected category
                }
            }

            ImGui::PopID();
        }
        ImGui::EndTable();
    }

    ImGui::End();
}

void renderAddTransactionWindow(bool &show_add_transaction_window, Transaction &new_transaction, int &selected_tab_for_new)
{
    ImGui::SetNextWindowSize(ImVec2(400, 350), ImGuiCond_FirstUseEver);
    ImGui::Begin("Add New Transaction", &show_add_transaction_window);

    if (ImGui::BeginTabBar("NewTransactionTabs"))
    {
        // Income Tab
        if (ImGui::BeginTabItem("Income"))
        {
            selected_tab_for_new = 0;
            new_transaction.type = TransactionType::Income;

            ImGui::InputFloat("Amount ($)", &new_transaction.amount, 1.0f, 100.0f, "%.2f");

            ImGui::InputText("Description", new_transaction.description, sizeof(new_transaction.description));

            if (ImGui::BeginCombo("##Account", accounts[_fromAccountIndex].accountName.c_str()))
            {
                for (int i = 0; i < accounts.size(); ++i)
                {
                    if (accounts[i].deleted)
                        continue; // skip if deleted

                    const bool isSelected = (_fromAccountIndex == i);
                    if (ImGui::Selectable(accounts[i].accountName.c_str(), isSelected))
                        _fromAccountIndex = i;
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            new_transaction.fromAccount = &accounts[_fromAccountIndex];
            ImGui::SameLine();
            ImGui::Text("Account");

            // new category using strings
            if (ImGui::BeginCombo("##Category", categories[_categoryIndex].c_str()))
            {
                for (int i = 0; i < categories.size(); ++i)
                {
                    const bool isSelected = (_categoryIndex == i);
                    if (ImGui::Selectable(categories[i].c_str(), isSelected))
                        _categoryIndex = i;
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            new_transaction.category = categories[_categoryIndex].c_str();
            ImGui::SameLine();
            ImGui::Text("Category");

            ImGui::InputText("Date", new_transaction.date, sizeof(new_transaction.date));

            ImGui::Spacing();
            if (ImGui::Button("Add Income", ImVec2(120, 30)))
            {
                if (new_transaction.amount > 0)
                {
                    g_transactions.push_back(new_transaction);
                    new_transaction.fromAccount->deposit(new_transaction.amount);

                    new_transaction = Transaction();
                    show_add_transaction_window = false;
                }
            }
            ImGui::EndTabItem();
        }

        // Expense Tab
        if (ImGui::BeginTabItem("Expense"))
        {
            selected_tab_for_new = 1;
            new_transaction.type = TransactionType::Expense;

            ImGui::InputFloat("Amount ($)", &new_transaction.amount, 1.0f, 100.0f, "%.2f");
            ImGui::InputText("Description", new_transaction.description, sizeof(new_transaction.description));

            if (ImGui::BeginCombo("##Account", accounts[_fromAccountIndex].accountName.c_str()))
            {
                for (int i = 0; i < accounts.size(); ++i)
                {
                    if (accounts[i].deleted)
                        continue; // skip if deleted

                    const bool isSelected = (_fromAccountIndex == i);
                    if (ImGui::Selectable(accounts[i].accountName.c_str(), isSelected))
                        _fromAccountIndex = i;
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            new_transaction.fromAccount = &accounts[_fromAccountIndex];
            ImGui::SameLine();
            ImGui::Text("Account");

            // new category using strings
            if (ImGui::BeginCombo("##Category", categories[_categoryIndex].c_str()))
            {
                for (int i = 0; i < categories.size(); ++i)
                {
                    const bool isSelected = (_categoryIndex == i);
                    if (ImGui::Selectable(categories[i].c_str(), isSelected))
                        _categoryIndex = i;
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            new_transaction.category = categories[_categoryIndex].c_str();
            ImGui::SameLine();
            ImGui::Text("Category");

            ImGui::InputText("Date", new_transaction.date, sizeof(new_transaction.date));

            ImGui::Spacing();
            if (ImGui::Button("Add Expense", ImVec2(120, 30)))
            {
                if (new_transaction.amount > 0)
                {
                    g_transactions.push_back(new_transaction);
                    new_transaction.fromAccount->withdraw(new_transaction.amount);

                    new_transaction = Transaction();
                    show_add_transaction_window = false;
                }
            }
            ImGui::EndTabItem();
        }

        // Transfer Tab
        if (ImGui::BeginTabItem("Transfer"))
        {
            selected_tab_for_new = 2;
            new_transaction.type = TransactionType::Transfer;

            ImGui::InputFloat("Amount ($)", &new_transaction.amount, 1.0f, 100.0f, "%.2f");
            ImGui::InputText("Description", new_transaction.description, sizeof(new_transaction.description));

            if (ImGui::BeginCombo("##FromAccount", accounts[_fromAccountIndex].accountName.c_str()))
            {
                for (int i = 0; i < accounts.size(); ++i)
                {
                    if (accounts[i].deleted)
                        continue; // skip if deleted

                    const bool isSelected = (_fromAccountIndex == i);
                    if (ImGui::Selectable(accounts[i].accountName.c_str(), isSelected))
                        _fromAccountIndex = i;
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            ImGui::Text("From Account");
            new_transaction.fromAccount = &accounts[_fromAccountIndex];

            if (ImGui::BeginCombo("##ToAccount", accounts[_toAccountIndex].accountName.c_str()))
            {
                for (int i = 0; i < accounts.size(); ++i)
                {
                    if (accounts[i].deleted)
                        continue; // skip if deleted

                    const bool isSelected = (_toAccountIndex == i);
                    if (ImGui::Selectable(accounts[i].accountName.c_str(), isSelected))
                        _toAccountIndex = i;
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            ImGui::Text("To Account");
            new_transaction.toAccount = &accounts[_toAccountIndex];

            ImGui::InputText("Date", new_transaction.date, sizeof(new_transaction.date));

            ImGui::Spacing();
            if (ImGui::Button("Add Transfer", ImVec2(120, 30)))
            {
                if (new_transaction.amount > 0)
                {
                    g_transactions.push_back(new_transaction);
                    new_transaction.fromAccount->transfer(new_transaction.toAccount, new_transaction.amount);

                    new_transaction = Transaction();
                    show_add_transaction_window = false;
                }
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Spacing();
    if (ImGui::Button("Cancel", ImVec2(80, 25)))
    {
        show_add_transaction_window = false;
        new_transaction = Transaction();
    }

    ImGui::End();
}
