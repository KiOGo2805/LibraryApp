#pragma once

/*
 * Цей файл оголошує клас AuthManager.
 * AuthManager відповідає за автентифікацію, авторизацію
 * та управління обліковими записами користувачів.
 */

#include <string>
#include <map> // Ефективно для зберігання пар "логін:пароль"

 /**
  * @class AuthManager
  * @brief Керує всіма аспектами облікових записів користувачів.
  *
  * Завантажує користувачів з файлу, перевіряє логін/пароль,
  * керує сесією поточного користувача та надає адмін-функції.
  */
class AuthManager
{
public:
    /**
     * @brief Конструктор.
     * @param usersFilePath Шлях до файлу користувачів (напр., "users.txt").
     */
    AuthManager(const std::string& usersFilePath);

    /**
     * @brief Деструктор.
     * Може викликати SaveUsers(), хоча краще зберігати зміни одразу.
     */
    ~AuthManager();

    /**
     * @brief Спроба входу в систему.
     * @param username Логін користувача.
     * @param password Пароль користувача.
     * @return true, якщо логін та пароль вірні, інакше false.
     */
    bool Login(const std::string& username, const std::string& password);

    /**
     * @brief Вихід із системи.
     */
    void Logout();

    /**
     * @brief Перевіряє, чи є поточний користувач адміністратором.
     * @return true, якщо поточний користувач - адмін, інакше false.
     */
    bool IsAdmin() const;

    /**
     * @brief Перевіряє, чи увійшов користувач у систему.
     * @return true, якщо сесія активна, інакше false.
     */
    bool IsLoggedIn() const;

    /**
     * @brief Отримує ім'я поточного користувача.
     * @return Логін поточного користувача або порожній рядок.
     */
    std::string GetCurrentUser() const;

    // --- Адмін-функції (Виконуються, лише якщо IsAdmin() == true) ---

    /**
     * @brief Створює нового користувача (тільки для адміна).
     * @param username Новий логін.
     * @param password Новий пароль.
     * @return true, якщо успішно, false - якщо користувач існує або немає прав.
     */
    bool CreateUser(const std::string& username, const std::string& password);

    /**
     * @brief Видаляє користувача (тільки для адміна).
     * @param username Логін користувача для видалення.
     * @return true, якщо успішно, false - якщо користувач не знайдений або немає прав.
     */
    bool DeleteUser(const std::string& username);

    /**
     * @brief Показує список всіх зареєстрованих користувачів (тільки для адміна).
     */
    void ListUsers() const;

private:
    /**
     * @brief Завантажує базу користувачів з файлу.
     * Викликається конструктором.
     */
    void LoadUsers();

    /**
     * @brief Зберігає поточну базу користувачів у файл.
     * Викликається після CreateUser або DeleteUser.
     */
    void SaveUsers() const;

    // Приватні поля (camelCase)
    std::string usersFilePath;
    std::map<std::string, std::string> users; // Ключ - логін, Значення - пароль

    std::string currentUser;
    bool isAdmin;
};