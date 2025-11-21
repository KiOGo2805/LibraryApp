// Цей файл містить реалізацію методів класу AdminUser.

#include "AdminUser.h"

using namespace std;

AdminUser::AdminUser(const string& username, const string& password)
    : username(username), password(password)
{
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
    return "Адміністратор";
}

bool AdminUser::IsAdmin() const
{
    return true;
}

string AdminUser::ToFileString() const
{
    return this->GetUserType() + ":" + this->username + ":" + this->password;
}