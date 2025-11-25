#pragma once
#include <string>

using namespace std;

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

    /**
     * @brief Отримує логін користувача.
     * @return Логін як string.
     */
    virtual string GetUsername() const = 0;

    /**
     * @brief Перевіряє, чи наданий пароль є вірним.
     * @param password Пароль для перевірки.
     * @return true, якщо пароль вірний, інакше false.
     */
    virtual bool CheckPassword(const string& password) const = 0;

    /**
     * @brief Отримує назву типу користувача (напр., "Admin" або "Standard").
     * @return Назва типу як string.
     */
    virtual string GetUserType() const = 0;

    /**
     * @brief Отримує рядок для збереження у файл (CSV-подібний).
     * @return Рядок для файлу.
     */
    virtual string ToFileString() const = 0;

    /**
     * @brief Перевіряє, чи має користувач права адміністратора.
     * @return true, якщо користувач є адміном, інакше false.
     */
    virtual bool IsAdmin() const = 0;
};