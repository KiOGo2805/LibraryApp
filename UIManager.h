#pragma once

/*
 * Цей файл оголошує клас UIManager.
 * UIManager відповідає за всю взаємодію з користувачем:
 * показ меню, обробку вводу та виклик відповідних
 * методів у Library та AuthManager.
 */

#include <string> // Потрібен для GetStringInput

 // Пряме оголошення, щоб уникнути циклічних залежностей у .h
 // Ми не можемо включати Library.h чи AuthManager.h тут.
class Library;
class AuthManager;

/**
 * @class UIManager
 * @brief Керує всім консольним інтерфейсом користувача.
 *
 * Не володіє менеджерами, а отримує вказівники на них
 * для виконання операцій.
 */
class UIManager
{
public:
    /**
     * @brief Конструктор.
     * @param library Вказівник на екземпляр Library.
     * @param authManager Вказівник на екземпляр AuthManager.
     */
    UIManager(Library* library, AuthManager* authManager);

    /**
     * @brief Запускає головний цикл інтерфейсу.
     * Починається з екрану логіна.
     */
    void StartMainLoop();

private:
    // --- Головні цикли меню ---

    /**
     * @brief Показує екран логіна, доки користувач не увійде.
     * @return true, якщо вхід успішний, false - якщо користувач вийшов.
     */
    bool HandleLogin();

    /**
     * @brief Показує ГОЛОВНЕ меню для АДМІНІСТРАТОРА.
     */
    void ShowAdminMainMenu();

    /**
     * @brief Показує ОБМЕЖЕНЕ меню для ЗВИЧАЙНОГО КОРИСТУВАЧА.
     */
    void ShowUserMainMenu();

    /**
     * @brief Показує меню адміністратора (для керування користувачами).
     */
    void ShowAdminMenu();

    /**
     * @brief Показує екран допомоги (вимога 5).
     */
    void ShowHelpScreen();

    // --- Допоміжні функції (Отримання вводу) ---

    /**
     * @brief Отримує та валідує вибір користувача в меню.
     * @param maxOption Максимальний номер опції.
     * @return Вибір користувача (число).
     */
    int GetMenuChoice(int maxOption);

    /**
     * @brief Отримує рядок від користувача (з валідацією).
     * @param prompt Повідомлення для користувача (напр., "Enter title:").
     * @return Введений рядок (не порожній).
     */
    std::string GetStringInput(const std::string& prompt);

    /**
     * @brief Отримує ціле число від користувача (з валідацією).
     * @param prompt Повідомлення для користувача.
     * @return Введене число.
     */
    int GetIntInput(const std::string& prompt);

    /**
     * @brief Отримує число з плаваючою комою (з валідацією).
     * @param prompt Повідомлення для користувача.
     * @return Введене число (double).
     */
    double GetDoubleInput(const std::string& prompt);

    /**
     * @brief Отримує відповідь Так/Ні.
     * @param prompt Повідомлення (напр., "Are you sure? (y/n):").
     * @return true, якщо 'y', false - якщо 'n'.
     */
    bool GetYesNoInput(const std::string& prompt);

    /**
     * @brief Очікує натискання Enter від користувача.
     */
    void PressEnterToContinue();

    // --- Функції-дії (для книг) ---
    void DoAddBook();
    void DoListAllBooks();
    void DoFindBookByArticle();
    void DoFilterBooks();
    void DoSortBooks();
    void DoUpdateBook();
    void DoDeleteBook();
    void DoIssueBook();
    void DoReturnBook();

    // --- Функції-дії (для користувачів) ---
    void DoCreateUser();
    void DoDeleteUser();
    void DoListUsers();

    // Вказівники на наші "мотори"
    Library* library;
    AuthManager* authManager;
};