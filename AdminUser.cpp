/*
 * Цей файл містить реалізацію методів класу AdminUser.
 */

#include "AdminUser.h" // Він має включати СВІЙ .h файл

using namespace std;

AdminUser::AdminUser(const string& username, const string& password)
    : username(username), password(password)
{
    // Тіло конструктора
}

string AdminUser::GetUsername() const
{
    return this->username;
}

bool AdminUser::CheckPassword(const string& password) const
{
    return this->password == password;
}

string AdminUser::GetUserType() const
{
    return "Admin";
}

bool AdminUser::IsAdmin() const
{
    // Адмін-користувач Є адміном
    return true;
}

string AdminUser::ToFileString() const
{
    // Формат: Type:Username:Password
    // Наприклад: Admin:admin:admin123
    return this->GetUserType() + ":" + this->username + ":" + this->password;
}