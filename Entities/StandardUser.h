#pragma once
#include "BaseUser.h"
#include <string>

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
    std::string username;
    std::string password;

public:
    /**
     * @brief Конструктор.
     * @param username Логін.
     * @param password Пароль.
     */
    StandardUser(const std::string& username, const std::string& password);

    std::string GetUsername() const override;
    bool CheckPassword(const std::string& password) const override;
    std::string GetUserType() const override;
    std::string ToFileString() const override;
    bool IsAdmin() const override;
};