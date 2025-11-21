// ÷ей файл м≥стить реал≥зац≥ю метод≥в класу StandardUser.

#include "StandardUser.h"

using namespace std;

StandardUser::StandardUser(const string& username, const string& password)
    : username(username), password(password)
{
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
    return false;
}

string StandardUser::ToFileString() const
{
    return this->GetUserType() + ":" + this->username + ":" + this->password;
}