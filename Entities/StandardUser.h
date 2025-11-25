#pragma once
#include "BaseUser.h"
#include <string>

using namespace std;

 /**
  * @class StandardUser
  * @brief Клас для звичайного користувача (не адміна).
  *
  * Успадковує BaseUser та реалізує його функціонал
  * для користувача зі стандартними правами.
  */
class StandardUser : public BaseUser
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
    StandardUser(const string& username, const string& password);

    string GetUsername() const override;
    bool CheckPassword(const string& password) const override;
    string GetUserType() const override;
    string ToFileString() const override;
    bool IsAdmin() const override;
};