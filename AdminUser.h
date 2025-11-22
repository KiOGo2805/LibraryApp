#pragma once

// Цей файл оголошує клас AdminUser.
// Це конкретний клас-нащадок від BaseUser для адміністратора.

#include "BaseUser.h"
#include <string>

 /**
  * @class AdminUser
  * @brief Клас для користувача-адміністратора.
  *
  * Успадковує BaseUser та реалізує його функціонал
  * для користувача з розширеними правами (адміна).
  */
class AdminUser : public BaseUser
{
private:
    std::string username;
    std::string password;

public:
    /**
     * @brief Конструктор.
     * @param username Логін.
     * @param password Пароль.
     */
    AdminUser(const std::string& username, const std::string& password);

    std::string GetUsername() const override;
    bool CheckPassword(const std::string& password) const override;
    std::string GetUserType() const override;
    std::string ToFileString() const override;
    bool IsAdmin() const override;
};