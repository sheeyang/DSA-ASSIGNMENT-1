#include "ui.h"
#include "calculations.h"
#include "imgui.h"
#include <cstring>

void SetupImGuiStyle()
{
	// Light style by dougbinks from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();
	
	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 0.0f;
	//style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
	
	style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.9372549f, 0.9372549f, 0.9372549f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.98f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.3f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.4f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.95686275f, 0.95686275f, 0.95686275f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.81960785f, 0.81960785f, 0.81960785f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 1.0f, 1.0f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.85882354f, 0.85882354f, 0.85882354f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.9764706f, 0.9764706f, 0.9764706f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.6862745f, 0.6862745f, 0.6862745f, 0.8f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4862745f, 0.4862745f, 0.4862745f, 0.8f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4862745f, 0.4862745f, 0.4862745f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.78f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.45882353f, 0.5372549f, 0.8f, 0.6f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.4f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05882353f, 0.5294118f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.8f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.3882353f, 0.3882353f, 0.3882353f, 0.62f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.13725491f, 0.4392157f, 0.8f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.13725491f, 0.4392157f, 0.8f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.34901962f, 0.34901962f, 0.34901962f, 0.17f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.95f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.7607843f, 0.79607844f, 0.8352941f, 0.931f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.8f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.5921569f, 0.7254902f, 0.88235295f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.91764706f, 0.9254902f, 0.93333334f, 0.9862f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.7411765f, 0.81960785f, 0.9137255f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.3882353f, 0.3882353f, 0.3882353f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.42745098f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392f, 0.69803923f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.44705883f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.7764706f, 0.8666667f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.5686275f, 0.5686275f, 0.6392157f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.6784314f, 0.6784314f, 0.7372549f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.29803923f, 0.29803923f, 0.29803923f, 0.09f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.95f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.8f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.69803923f, 0.69803923f, 0.69803923f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.35f);
}

void renderSummarySection()
{
    ImGui::Text("=== Account Summary ===");
    ImGui::Separator();
    ImGui::TextColored(COLOR_INCOME, "Total Income:  $%.2f", calculateTotalIncome());
    ImGui::TextColored(COLOR_EXPENSE, "Total Expense: $%.2f", calculateTotalExpense());
    float balance = calculateBalance();
    ImVec4 balanceColor = balance >= 0 ? COLOR_POSITIVE : COLOR_NEGATIVE;
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
                        ImGui::TextColored(COLOR_INCOME, "+$%.2f", t.amount);
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
                        ImGui::TextColored(COLOR_EXPENSE, "-$%.2f", t.amount);
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
                        ImGui::TextColored(COLOR_TRANSFER, "$%.2f", t.amount);
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
                ImGui::TextColored(COLOR_INCOME, "Income");
            else if (t.type == TransactionType::Expense)
                ImGui::TextColored(COLOR_EXPENSE, "Expense");
            else
                ImGui::TextColored(COLOR_TRANSFER, "Transfer");

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
                ImGui::TextColored(COLOR_INCOME, "+$%.2f", t.amount);
            else if (t.type == TransactionType::Expense)
                ImGui::TextColored(COLOR_EXPENSE, "-$%.2f", t.amount);
            else
                ImGui::TextColored(COLOR_TRANSFER, "$%.2f", t.amount);

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

            ImVec4 textColour = accounts[i].amount >= 0.0f ? COLOR_POSITIVE : COLOR_NEGATIVE;
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

    ImGui::TextColored(COLOR_EXPENSE,
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
