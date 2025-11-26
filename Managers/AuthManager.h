#pragma once
#include "../Entities/BaseUser.h"
#include <string>
#include <map>
#include <memory>

using namespace std;

 /**
  * @class AuthManager
  * @brief Керує всіма аспектами облікових записів користувачів.
  *
  * Використовує map для зберігання об'єктів-нащадків BaseUser
  * за допомогою розумних вказівників unique_ptr.
  */
class AuthManager
{
private:
    string usersFilePath;
    map<string, unique_ptr<BaseUser>> users;
    BaseUser* currentUser;

public:
    /**
     * @brief Конструктор.
     * @param usersFilePath Шлях до файлу користувачів (напр., "users.txt").
     */
    AuthManager(const string& usersFilePath);

    /**
     * @brief Деструктор.
     * Необхідний для коректного видалення unique_ptr.
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
    bool Login(const string& username, const string& password);

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
    string GetCurrentUser() const;

    /**
     * @brief Створює нового користувача (тільки для адміна).
     * @param username Новий логін.
     * @param password Новий пароль.
     * @param isAdmin Чи повинен новий користувач бути адміном.
     * @return true, якщо успішно, false - якщо користувач існує або немає прав.
     */
    bool CreateUser(
        const string& username,
        const string& password,
        bool isAdmin
    );

    /**
     * @brief Видаляє користувача (тільки для адміна).
     * @param username Логін користувача для видалення.
     * @return true, якщо успішно, false - якщо користувач не знайдений або немає прав.
     */
    bool DeleteUser(const string& username);

    /**
     * @brief Показує список всіх зареєстрованих користувачів (тільки для адміна).
     */
    void ListUsers() const;

private:
    /**
     * @brief Завантажує базу користувачів з файлу.
     */
    void loadUsers();

    /**
     * @brief Зберігає поточну базу користувачів у файл.
     */
    void saveUsers() const;
};