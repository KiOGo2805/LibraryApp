/*
 * Цей файл містить реалізацію методів класу StandardUser.
 */

#include "StandardUser.h" // Він має включати СВІЙ .h файл

using namespace std; // Можна додати для зручності

StandardUser::StandardUser(const string& username, const string& password)
    : username(username), password(password)
{
    // Тіло конструктора
}

string StandardUser::GetUsername() const
{
    return this->username;
}

bool StandardUser::CheckPassword(const string& password) const
{
    return this->password == password;
}

string StandardUser::GetUserType() const
{
    return "Standard";
}

bool StandardUser::IsAdmin() const
{
    // Звичайний користувач НЕ є адміном
    return false;
}

string StandardUser::ToFileString() const
{
    // Формат: Type:Username:Password
    // Наприклад: Standard:someuser:pass123
    return this->GetUserType() + ":" + this->username + ":" + this->password;
}