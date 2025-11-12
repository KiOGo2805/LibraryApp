#pragma once

/*
 * Цей файл оголошує клас StandardUser.
 * Це конкретний клас-нащадок від BaseUser для звичайного користувача.
 */

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
public:
    /**
     * @brief Конструктор.
     * @param username Логін.
     * @param password Пароль.
     */
    StandardUser(const std::string& username, const std::string& password);

    // --- Реалізація віртуальних методів ---

    std::string GetUsername() const override;
    bool CheckPassword(const std::string& password) const override;
    std::string GetUserType() const override;
    std::string ToFileString() const override;
    bool IsAdmin() const override;

private:
    std::string username;
    std::string password; // У реальному проекті тут був би хеш
};