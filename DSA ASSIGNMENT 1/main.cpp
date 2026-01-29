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

// Window dimensions
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

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

// Transaction structure
struct Transaction
{
    TransactionType type;
    char description[256];
    float amount;
    char category[64];
    char date[32];
    char fromAccount[64]; // For transfers
    char toAccount[64];   // For transfers

    Transaction()
    {
        type = TransactionType::Income;
        memset(description, 0, sizeof(description));
        memset(category, 0, sizeof(category));
        memset(date, 0, sizeof(date));
        memset(fromAccount, 0, sizeof(fromAccount));
        memset(toAccount, 0, sizeof(toAccount));
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

    // Add some sample transactions
    {
        Transaction t1;
        t1.type = TransactionType::Income;
        strcpy_s(t1.description, "Monthly Salary");
        strcpy_s(t1.category, "Salary");
        t1.amount = 5000.0f;
        strcpy_s(t1.date, "2026-01-15");
        g_transactions.push_back(t1);

        Transaction t2;
        t2.type = TransactionType::Expense;
        strcpy_s(t2.description, "Grocery Shopping");
        strcpy_s(t2.category, "Food");
        t2.amount = 150.0f;
        strcpy_s(t2.date, "2026-01-20");
        g_transactions.push_back(t2);

        Transaction t3;
        t3.type = TransactionType::Transfer;
        strcpy_s(t3.description, "Savings Transfer");
        strcpy_s(t3.fromAccount, "Checking");
        strcpy_s(t3.toAccount, "Savings");
        t3.amount = 500.0f;
        strcpy_s(t3.date, "2026-01-25");
        g_transactions.push_back(t3);
    }

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Main Finance Tracker Window
        {
            ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
            ImGui::Begin("Finance Tracker", nullptr, ImGuiWindowFlags_MenuBar);

            // Menu bar
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
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
                                    ImGui::Text("%s", t.category);
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
                                    ImGui::Text("%s", t.category);
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
                                    ImGui::Text("%s", t.fromAccount);
                                    ImGui::TableNextColumn();
                                    ImGui::Text("%s", t.toAccount);
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

                if (ImGui::BeginTable("AllTransactionsTable", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable))
                {
                    ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                    ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Category/Account", ImGuiTableColumnFlags_WidthFixed, 150.0f);
                    ImGui::TableSetupColumn("Details", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableSetupColumn("Amount", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableHeadersRow();

                    for (size_t i = 0; i < g_transactions.size(); i++)
                    {
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

                        // Category/Account
                        ImGui::TableNextColumn();
                        if (t.type == TransactionType::Transfer)
                            ImGui::Text("%s", t.fromAccount);
                        else
                            ImGui::Text("%s", t.category);

                        // Details (To account for transfers, empty for others)
                        ImGui::TableNextColumn();
                        if (t.type == TransactionType::Transfer)
                            ImGui::Text("-> %s", t.toAccount);
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
                    }
                    ImGui::EndTable();
                }
            }

            ImGui::Text("\nTotal Transactions: %zu", g_transactions.size());
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

                    ImGui::InputText("Date", new_transaction.date, sizeof(new_transaction.date));
                    ImGui::InputText("Description", new_transaction.description, sizeof(new_transaction.description));
                    ImGui::InputText("Category", new_transaction.category, sizeof(new_transaction.category));
                    ImGui::InputFloat("Amount ($)", &new_transaction.amount, 1.0f, 100.0f, "%.2f");

                    ImGui::Spacing();
                    if (ImGui::Button("Add Income", ImVec2(120, 30)))
                    {
                        if (new_transaction.amount > 0)
                        {
                            g_transactions.push_back(new_transaction);
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

                    ImGui::InputText("Date", new_transaction.date, sizeof(new_transaction.date));
                    ImGui::InputText("Description", new_transaction.description, sizeof(new_transaction.description));
                    ImGui::InputText("Category", new_transaction.category, sizeof(new_transaction.category));
                    ImGui::InputFloat("Amount ($)", &new_transaction.amount, 1.0f, 100.0f, "%.2f");

                    ImGui::Spacing();
                    if (ImGui::Button("Add Expense", ImVec2(120, 30)))
                    {
                        if (new_transaction.amount > 0)
                        {
                            g_transactions.push_back(new_transaction);
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

                    ImGui::InputText("Date", new_transaction.date, sizeof(new_transaction.date));
                    ImGui::InputText("Description", new_transaction.description, sizeof(new_transaction.description));
                    ImGui::InputText("From Account", new_transaction.fromAccount, sizeof(new_transaction.fromAccount));
                    ImGui::InputText("To Account", new_transaction.toAccount, sizeof(new_transaction.toAccount));
                    ImGui::InputFloat("Amount ($)", &new_transaction.amount, 1.0f, 100.0f, "%.2f");

                    ImGui::Spacing();
                    if (ImGui::Button("Add Transfer", ImVec2(120, 30)))
                    {
                        if (new_transaction.amount > 0)
                        {
                            g_transactions.push_back(new_transaction);
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
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
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
