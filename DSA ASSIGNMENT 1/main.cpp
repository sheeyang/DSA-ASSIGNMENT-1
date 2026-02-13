// Finance Tracker App
// Using OpenGL + ImGui

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

#include "types.h"
#include "globals.h"
#include "calculations.h"
#include "callbacks.h"
#include "ui.h"

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
    SetupImGuiStyle();
    // ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();
    // ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Application state
    ImVec4 clear_color = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);
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
            renderSummarySection();

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
                renderTransactionsByType();
            }
            // View Mode: All transactions in one table
            else
            {
                renderAllTransactions();
            }
            ImGui::Text("\nTotal Transactions: %zu", g_transactions.size());

            ImGui::Separator();

            renderAccountsBreakdown(show_delete_account, deleteBufferAccountIndex, transferBufferAccountIndex, show_add_account, new_account);

            ImGui::End();
        }

        //  add account window
        if (show_add_account)
        {
            renderAddAccountWindow(show_add_account, new_account, new_transaction);
        }

        //  delete account window
        if (show_delete_account)
        {
            renderDeleteAccountWindow(show_delete_account, deleteBufferAccountIndex, transferBufferAccountIndex);
        }

        // category window
        if (show_category_window)
        {
            renderCategoryWindow(show_category_window);
        }

        // Add Transaction Window
        if (show_add_transaction_window)
        {
            renderAddTransactionWindow(show_add_transaction_window, new_transaction, selected_tab_for_new);
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
