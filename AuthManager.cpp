#include "AuthManager.h"
#include "StandardUser.h"
#include "AdminUser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

using namespace std;

namespace
{
    const string ADMIN_USERNAME = "admin";
    const string ADMIN_DEFAULT_PASS = "admin123";
}

AuthManager::AuthManager(const string& usersFilePath)
    : usersFilePath(usersFilePath),
    currentUser(nullptr)
{
    try
    {
        this->LoadUsers();
    }
    catch (const exception& e)
    {
        cerr << "Помилка: Не вдалося завантажити файл користувачів. " << e.what() << "\n";
    }
}

AuthManager::~AuthManager()
{
}

bool AuthManager::Login(const string& username, const string& password)
{
    auto it = this->users.find(username);

    if (it != this->users.end())
    {
        if (it->second->CheckPassword(password))
        {
            this->currentUser = it->second.get();
            return true;
        }
    }

    this->currentUser = nullptr;
    return false;
}

void AuthManager::Logout()
{
    this->currentUser = nullptr;
}

bool AuthManager::IsAdmin() const
{
    return this->IsLoggedIn() && this->currentUser->IsAdmin();
}

bool AuthManager::IsLoggedIn() const
{
    return this->currentUser != nullptr;
}

string AuthManager::GetCurrentUser() const
{
    if (this->IsLoggedIn())
    {
        return this->currentUser->GetUsername();
    }
    return "";
}

bool AuthManager::CreateUser(
    const string& username,
    const string& password,
    bool isAdmin)
{
    if (!this->IsAdmin())
    {
        cerr << "Помилка: Тільки адміністратор може створювати користувачів.\n";
        return false;
    }

    if (username.empty() || password.empty())
    {
        cerr << "Помилка: Логін та пароль не можуть бути порожніми.\n";
        return false;
    }

    if (this->users.find(username) != this->users.end())
    {
        cerr << "Помилка: Користувач '" << username << "' вже існує.\n";
        return false;
    }

    unique_ptr<BaseUser> newUser;
    if (isAdmin)
    {
        newUser = make_unique<AdminUser>(username, password);
    }
    else
    {
        newUser = make_unique<StandardUser>(username, password);
    }

    this->users[username] = move(newUser);

    try
    {
        this->SaveUsers();
        cout << "Користувача '" << username << "' успішно створено.\n";
        return true;
    }
    catch (const exception& e)
    {
        cerr << "Помилка: Не вдалося зберегти файл користувачів. " << e.what() << "\n";
        this->users.erase(username);
        return false;
    }
}

bool AuthManager::DeleteUser(const string& username)
{
    if (!this->IsAdmin())
    {
        cerr << "Помилка: Тільки адміністратор може видаляти користувачів.\n";
        return false;
    }

    if (username == ADMIN_USERNAME)
    {
        cerr << "Помилка: Неможливо видалити головний акаунт адміністратора.\n";
        return false;
    }

    auto it = this->users.find(username);
    if (it == this->users.end())
    {
        cerr << "Помилка: Користувача '" << username << "' не знайдено.\n";
        return false;
    }

    this->users.erase(it);

    try
    {
        this->SaveUsers();
        cout << "Користувача '" << username << "' успішно видалено.\n";
        return true;
    }
    catch (const exception& e)
    {
        cerr << "Помилка: Не вдалося зберегти файл користувачів після видалення. " << e.what() << "\n";
        return false;
    }
}

void AuthManager::ListUsers() const
{
    if (!this->IsAdmin())
    {
        cerr << "Помилка: Тільки адміністратор може переглядати список.\n";
        return;
    }

    for (const auto& pair : this->users)
    {
        string type = pair.second->GetUserType();
        string displayType = (type == "Admin") ? "Адміністратор" : "Читач";

        cout << "- " << pair.second->GetUsername()
            << " [" << displayType << "]\n";
    }
}

void AuthManager::LoadUsers()
{
    ifstream file(this->usersFilePath);
    if (!file.is_open())
    {
        cout << "Файл користувачів не знайдено. Створення нового з адміністратором за замовчуванням...\n";

        this->users[ADMIN_USERNAME] =
            make_unique<AdminUser>(ADMIN_USERNAME, ADMIN_DEFAULT_PASS);

        this->SaveUsers();
        return;
    }

    string line;
    int lineCount = 0;
    while (getline(file, line))
    {
        lineCount++;
        if (line.empty()) continue;

        stringstream ss(line);
        string userType, username, password;

        if (getline(ss, userType, ':') &&
            getline(ss, username, ':') &&
            getline(ss, password))
        {
            if (userType == "Admin")
            {
                this->users[username] =
                    make_unique<AdminUser>(username, password);
            }
            else if (userType == "User" || userType == "Standard")
            {
                this->users[username] =
                    make_unique<StandardUser>(username, password);
            }
            else
            {
                cerr << "Попередження: Пропущено рядок " << lineCount
                    << " (невідомий тип користувача: " << userType << ")\n";
            }
        }
        else
        {
            cerr << "Попередження: Пропущено пошкоджені дані у рядку "
                << lineCount << "\n";
        }
    }

    if (this->users.find(ADMIN_USERNAME) == this->users.end())
    {
        cout << "Адміністратора не знайдено. Додавання адміністратора за замовчуванням...\n";
        this->users[ADMIN_USERNAME] =
            make_unique<AdminUser>(ADMIN_USERNAME, ADMIN_DEFAULT_PASS);
        this->SaveUsers();
    }

    file.close();
    cout << "Успішно завантажено " << this->users.size() << " користувачів.\n";
}

void AuthManager::SaveUsers() const
{
    ofstream file(this->usersFilePath);
    if (!file.is_open())
    {
        throw runtime_error("Не вдалося відкрити файл користувачів для запису: "
            + this->usersFilePath);
    }

    for (const auto& pair : this->users)
    {
        file << pair.second->ToFileString() << "\n";
    }
    file.close();
}