#pragma once

// Цей файл оголошує клас UIManager.

#include <string>

class Library;
class AuthManager;

/**
 * @class UIManager
 * @brief Керує всім консольним інтерфейсом користувача.
 */
class UIManager
{
public:
    /**
     * @brief Конструктор.
     */
    UIManager(Library* library, AuthManager* authManager);

    /**
     * @brief Запускає головний цикл інтерфейсу.
     */
    void StartMainLoop();

private:
    // --- Основні меню ---
    bool HandleLogin();
    void ShowAdminMainMenu();
    void ShowUserMainMenu();
    void ShowAdminMenu();
    void ShowHelpScreen();

    // --- Допоміжні методи вводу ---
    int GetMenuChoice(int maxOption);
    std::string GetStringInput(const std::string& prompt);
    int GetIntInput(const std::string& prompt);
    double GetDoubleInput(const std::string& prompt);
    bool GetYesNoInput(const std::string& prompt);
    void PressEnterToContinue();

    // --- Нові методи для рефакторингу (усунення дублювання) ---

    // Запитує артикул, шукає книгу, обробляє помилки. Повертає книгу або nullptr.
    class Book* PromptAndFindBook(const std::string& actionName);

    // Цикл перевірки нового артикулу (унікальність, символи, довжина).
    std::string GetValidNewArticle();

    // --- Дії з книгами ---
    void DoAddBook();
    void DoListAllBooks();
    void DoFindBookByArticle();
    void DoFilterBooks();
    void DoSortBooks();
    void DoUpdateBook();
    void DoDeleteBook();
    void DoIssueBook();
    void DoReturnBook();

    // --- Дії з користувачами ---
    void DoCreateUser();
    void DoDeleteUser();
    void DoListUsers();

    Library* library;
    AuthManager* authManager;
};