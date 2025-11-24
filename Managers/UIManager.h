#pragma once
#include <string>

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
    /**
     * @brief Показує екран логіна, доки користувач не увійде.
     * @return true, якщо вхід успішний, false - якщо користувач вийшов.
     */
    bool HandleLogin();

    /**
     * @brief Відображає головне меню для адміністратора.
     */
    void ShowAdminMainMenu();

    /**
     * @brief Відображає обмежене меню для звичайного користувача.
     */
    void ShowUserMainMenu();

    /**
     * @brief Відображає меню адміністратора (для керування користувачами).
     */
    void ShowAdminMenu();

    /**
     * @brief Показує екран допомоги.
     */
    void ShowHelpScreen();

    /**
     * @brief Отримує та валідує вибір користувача в меню.
     * @param maxOption Максимальний номер опції.
     * @return Вибір користувача (число).
     */
    int GetMenuChoice(int maxOption);

    /**
     * @brief Отримує рядок від користувача (з валідацією на порожній ввід).
     * @param prompt Повідомлення для користувача.
     * @return Введений рядок.
     */
    std::string GetStringInput(const std::string& prompt);

    /**
     * @brief Отримує ціле число від користувача.
     * @param prompt Повідомлення для користувача.
     * @return Введене число.
     */
    int GetIntInput(const std::string& prompt);

    /**
     * @brief Отримує число з плаваючою комою.
     * @param prompt Повідомлення для користувача.
     * @return Введене число.
     */
    double GetDoubleInput(const std::string& prompt);

    /**
     * @brief Отримує відповідь Так/Ні.
     * @param prompt Повідомлення для користувача.
     * @return true, якщо 'y', false - якщо 'n'.
     */
    bool GetYesNoInput(const std::string& prompt);

    /**
     * @brief Очікує натискання Enter від користувача та очищує екран.
     */
    void PressEnterToContinue();

    /**
     * @brief Запитує артикул, шукає книгу та обробляє помилки.
     * @param actionName Назва поточної дії (для заголовка).
     * @return Вказівник на книгу або nullptr, якщо не знайдено/скасовано.
     */
    class Book* PromptAndFindBook(const std::string& actionName);

    /**
     * @brief Отримує та валідує новий артикул (унікальність, формат).
     * @return Коректний артикул або порожній рядок (скасування).
     */
    std::string GetValidNewArticle();

    /**
     * @brief Виконує сценарій додавання нової книги.
     */
    void DoAddBook();

    /**
     * @brief Виводить список усіх книг.
     */
    void DoListAllBooks();

    /**
     * @brief Виконує пошук книги за артикулом.
     */
    void DoFindBookByArticle();

    /**
     * @brief Виконує фільтрацію книг за критеріями.
     */
    void DoFilterBooks();

    /**
     * @brief Виконує сортування книг.
     */
    void DoSortBooks();

    /**
     * @brief Виконує оновлення даних книги.
     */
    void DoUpdateBook();

    /**
     * @brief Виконує видалення книги.
     */
    void DoDeleteBook();

    /**
     * @brief Виконує видачу книги читачу.
     */
    void DoIssueBook();

    /**
     * @brief Виконує повернення книги в бібліотеку.
     */
    void DoReturnBook();

    /**
     * @brief Створює нового користувача.
     */
    void DoCreateUser();

    /**
     * @brief Видаляє користувача.
     */
    void DoDeleteUser();

    /**
     * @brief Виводить список усіх користувачів.
     */
    void DoListUsers();

    Library* library;
    AuthManager* authManager;
};