// Finance Tracker App
// Using OpenGL + ImGui

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstring>
#include <algorithm>

// Window dimensions
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
constexpr int TEXT_BUFFER = 256;

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

// Global transaction storage
std::vector<Transaction> g_transactions;

// category list
std::vector<std::string> categories{"FOOD", "SOCIAL", "SHOPPING", "TRANSPORT", "EDUCATION", "SALARY", "INTEREST"};
std::string _currentCategory;
char _inputCategoryBuffer[TEXT_BUFFER]{'\0'};
static int _categoryIndex = 0;
bool _inputFieldEmpty{true};

// accounts list
std::vector<Account> accounts;
static int _fromAccountIndex = 0;
static int _toAccountIndex = 0;
char _inputAccountBuffer[TEXT_BUFFER]{'\0'};

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

// Callback for window resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Process input
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Finance Tracker", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewError) << std::endl;
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Application state
    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    bool show_add_transaction_window = false;
    Transaction new_transaction;
    int selected_tab_for_new = 0; // 0=Income, 1=Expense, 2=Transfer
    ViewMode view_mode = ViewMode::All;

    // manage categories
    bool show_category_window = false;

    // manage deleting account with balance
    Account new_account;
    bool show_add_account = false;
    bool show_delete_account = false;
    int deleteBufferAccountIndex{-1};
    int transferBufferAccountIndex{-1};

    // add sample accounts
    {
        Account a1;
        a1.accountName = "SAVINGS";
        a1.amount = 0.0f;
        accounts.push_back(a1);

        Account a2;
        a2.accountName = "EXPENSES";
        a2.amount = 0.0f;
        accounts.push_back(a2);

        Account a3;
        a3.accountName = "EMERGENCY";
        a3.amount = 0.0f;
        accounts.push_back(a3);

        accounts.reserve(20);
    }

    // Add some sample transactions
    {
        Transaction t1;
        t1.type = TransactionType::Income;
        strcpy_s(t1.description, "Monthly Salary");
        t1.category = "SALARY";
        t1.amount = 5000.0f;
        t1.fromAccount = &accounts[0];
        t1.fromAccount->deposit(t1.amount);
        strcpy_s(t1.date, "2026-01-15");
        g_transactions.push_back(t1);

        Transaction t2;
        t2.type = TransactionType::Expense;
        strcpy_s(t2.description, "Grocery Shopping");
        t2.category = "FOOD";
        t2.amount = 150.0f;
        t2.fromAccount = &accounts[1];
        t2.fromAccount->withdraw(t2.amount);
        strcpy_s(t2.date, "2026-01-20");
        g_transactions.push_back(t2);

        Transaction t3;
        t3.type = TransactionType::Transfer;
        strcpy_s(t3.description, "Savings Transfer");

        t3.fromAccount = &accounts[0];
        t3.toAccount = &accounts[1];

        t3.amount = 500.0f;
        t3.fromAccount->transfer(t3.toAccount, t3.amount);
        strcpy_s(t3.date, "2026-01-25");
        g_transactions.push_back(t3);
    }

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);
        glfwPollEvents();

        // get window size
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main Finance Tracker Window
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));

            // remove padding
            ImGui::Begin("Finance Tracker", nullptr,
                         ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_NoBringToFrontOnFocus |
                             ImGuiWindowFlags_MenuBar);

            // Menu bar
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Categories"))
                        show_category_window = true;
                    if (ImGui::MenuItem("Exit"))
                        glfwSetWindowShouldClose(window, true);
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            // Summary section
            ImGui::Text("=== Account Summary ===");
            ImGui::Separator();
            ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "Total Income:  $%.2f", calculateTotalIncome());
            ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Total Expense: $%.2f", calculateTotalExpense());
            float balance = calculateBalance();
            ImVec4 balanceColor = balance >= 0 ? ImVec4(0.4f, 1.0f, 0.4f, 1.0f) : ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
            ImGui::TextColored(balanceColor, "Balance:       $%.2f", balance);
            ImGui::Separator();
            ImGui::Spacing();

            // Add transaction button
            if (ImGui::Button("+ Add New Transaction", ImVec2(200, 30)))
            {
                show_add_transaction_window = true;
                new_transaction = Transaction(); // Reset
            }

            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 300);
            ImGui::Text("View Mode:");
            ImGui::SameLine();
            if (ImGui::RadioButton("All", view_mode == ViewMode::All))
                view_mode = ViewMode::All;
            ImGui::SameLine();
            if (ImGui::RadioButton("By Type", view_mode == ViewMode::ByType))
                view_mode = ViewMode::ByType;

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // View Mode: Separate tabs for each transaction type
            if (view_mode == ViewMode::ByType)
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
            // View Mode: All transactions in one table
            else
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
            ImGui::Text("\nTotal Transactions: %zu", g_transactions.size());

            ImGui::Separator();

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

            ImGui::End();
        }

        //  add account indow
        if (show_add_account)
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

        //  delete account window
        if (show_delete_account)
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

        // category window
        if (show_category_window)
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

        // Add Transaction Window
        if (show_add_transaction_window)
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

        // Rendering
        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
