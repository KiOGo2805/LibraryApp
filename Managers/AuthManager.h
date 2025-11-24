#pragma once

// Цей файл оголошує клас AuthManager.
// AuthManager відповідає за автентифікацію, авторизацію
// та управління обліковими записами користувачів.

#include "../Entities/BaseUser.h"
#include <string>
#include <map>
#include <memory>

 /**
  * @class AuthManager
  * @brief Керує всіма аспектами облікових записів користувачів.
  *
  * Використовує std::map для зберігання об'єктів-нащадків BaseUser
  * за допомогою розумних вказівників std::unique_ptr.
  */
class AuthManager
{
private:
    std::string usersFilePath;
    std::map<std::string, std::unique_ptr<BaseUser>> users;
    BaseUser* currentUser;

public:
    /**
     * @brief Конструктор.
     * @param usersFilePath Шлях до файлу користувачів (напр., "users.txt").
     */
    AuthManager(const std::string& usersFilePath);

    /**
     * @brief Деструктор.
     * Необхідний для коректного видалення std::unique_ptr.
     */
    ~AuthManager();

    AuthManager(const AuthManager&) = delete;
    AuthManager& operator=(const AuthManager&) = delete;

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

    /**
     * @brief Створює нового користувача (тільки для адміна).
     * @param username Новий логін.
     * @param password Новий пароль.
     * @param isAdmin Чи повинен новий користувач бути адміном.
     * @return true, якщо успішно, false - якщо користувач існує або немає прав.
     */
    bool CreateUser(
        const std::string& username,
        const std::string& password,
        bool isAdmin
    );

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
     */
    void LoadUsers();

    /**
     * @brief Зберігає поточну базу користувачів у файл.
     */
    void SaveUsers() const;
};