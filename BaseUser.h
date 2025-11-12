#pragma once

/*
 * Цей файл оголошує абстрактний клас BaseUser.
 * BaseUser є базовим для всіх типів користувачів у системі
 * (наприклад, StandardUser, AdminUser).
 */

#include <string>

 /**
  * @class BaseUser
  * @brief Абстрактний базовий клас для користувача системи.
  *
  * Визначає загальний інтерфейс для всіх користувачів,
  * включаючи перевірку пароля та визначення типу користувача.
  */
class BaseUser
{
public:
    /**
     * @brief Віртуальний деструктор.
     */
    virtual ~BaseUser() {}

    // --- 1. Основні методи ---

    /**
     * @brief Отримує логін користувача.
     * @return Логін як std::string.
     */
    virtual std::string GetUsername() const = 0;

    /**
     * @brief Перевіряє, чи наданий пароль є вірним.
     * @param password Пароль для перевірки.
     * @return true, якщо пароль вірний, інакше false.
     */
    virtual bool CheckPassword(const std::string& password) const = 0;

    /**
     * @brief Отримує назву типу користувача (напр., "Admin" або "Standard").
     * @return Назва типу як std::string.
     */
    virtual std::string GetUserType() const = 0;

    /**
     * @brief Отримує рядок для збереження у файл (CSV-подібний).
     * @return Рядок для файлу.
     */
    virtual std::string ToFileString() const = 0;

    // --- 2. Права доступу (Поліморфізм) ---

    /**
     * @brief Перевіряє, чи має користувач права адміністратора.
     * @return true, якщо користувач є адміном, інакше false.
     */
    virtual bool IsAdmin() const = 0;
};