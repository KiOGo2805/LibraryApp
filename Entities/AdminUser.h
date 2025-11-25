#pragma once
#include "BaseUser.h"
#include <string>

using namespace std;

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
    string username;
    string password;

public:
    /**
     * @brief Конструктор.
     * @param username Логін.
     * @param password Пароль.
     */
    AdminUser(const string& username, const string& password);

    string GetUsername() const override;
    bool CheckPassword(const string& password) const override;
    string GetUserType() const override;
    string ToFileString() const override;
    bool IsAdmin() const override;
};