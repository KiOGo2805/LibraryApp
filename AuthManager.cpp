/*
 * Цей файл містить реалізацію методів класу AuthManager.
 * Ця версія використовує поліморфізм для роботи з BaseUser.
 */

#include "AuthManager.h"
#include "StandardUser.h" // Потрібна реалізація для StandardUser
#include "AdminUser.h"    // Потрібна реалізація для AdminUser
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>         // Для std::make_unique

using namespace std;

// Константи для адміна
namespace
{
    const string ADMIN_USERNAME = "admin";
    const string ADMIN_DEFAULT_PASS = "admin123";
}

// --- Конструктор та Деструктор ---

AuthManager::AuthManager(const string& usersFilePath)
    : usersFilePath(usersFilePath),
    currentUser(nullptr) // На початку ніхто не залогований
{
    try
    {
        this->LoadUsers();
    }
    catch (const exception& e)
    {
        cerr << "Error: Failed to load users file. " << e.what() << endl;
    }
}

// Оголошення деструктора в .cpp файлі є ОБОВ'ЯЗКОВИМ
// для коректної роботи std::unique_ptr з неповними типами (PImpl)
AuthManager::~AuthManager()
{
    // Тіло може бути порожнім, але його наявність тут
    // дозволяє компілятору коректно видалити unique_ptr
}

// --- Основні методи ---

bool AuthManager::Login(const string& username, const string& password)
{
    // Шукаємо користувача в 'map'
    auto it = this->users.find(username);

    if (it != this->users.end())
    {
        // Якщо знайдено, викликаємо поліморфний метод CheckPassword
        if (it->second->CheckPassword(password))
        {
            // Успіх. Зберігаємо вказівник на об'єкт користувача
            this->currentUser = it->second.get();
            return true;
        }
    }

    // Якщо логін/пароль невірні або користувач не знайдений
    this->currentUser = nullptr;
    return false;
}

void AuthManager::Logout()
{
    this->currentUser = nullptr;
}

bool AuthManager::IsAdmin() const
{
    // Перевіряємо, чи ми залоговані І чи поточний користувач
    // повертає true з поліморфного методу IsAdmin()
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

// --- Адмін-функції ---

bool AuthManager::CreateUser(
    const string& username,
    const string& password,
    bool isAdmin)
{
    if (!this->IsAdmin())
    {
        cerr << "Error: Only admin can create users." << endl;
        return false;
    }

    if (username.empty() || password.empty())
    {
        cerr << "Error: Username and password cannot be empty." << endl;
        return false;
    }

    if (this->users.find(username) != this->users.end())
    {
        cerr << "Error: User '" << username << "' already exists." << endl;
        return false;
    }

    // Створюємо нового користувача потрібного типу
    unique_ptr<BaseUser> newUser;
    if (isAdmin)
    {
        newUser = make_unique<AdminUser>(username, password);
    }
    else
    {
        newUser = make_unique<StandardUser>(username, password);
    }

    // Додаємо нового користувача в map
    this->users[username] = move(newUser);

    try
    {
        this->SaveUsers(); // Зберігаємо зміни у файл
        cout << "User '" << username << "' created successfully." << endl;
        return true;
    }
    catch (const exception& e)
    {
        cerr << "Error: Failed to save users file. " << e.what() << endl;
        this->users.erase(username); // Повертаємо зміни
        return false;
    }
}

bool AuthManager::DeleteUser(const string& username)
{
    if (!this->IsAdmin())
    {
        cerr << "Error: Only admin can delete users." << endl;
        return false;
    }

    if (username == ADMIN_USERNAME)
    {
        cerr << "Error: Cannot delete the primary admin account." << endl;
        return false;
    }

    auto it = this->users.find(username);
    if (it == this->users.end())
    {
        cerr << "Error: User '" << username << "' not found." << endl;
        return false;
    }

    this->users.erase(it); // Видаляємо з map

    try
    {
        this->SaveUsers(); // Зберігаємо зміни
        cout << "User '" << username << "' deleted successfully." << endl;
        return true;
    }
    catch (const exception& e)
    {
        cerr << "Error: Failed to save users file after deletion." << e.what() << endl;
        // (Відновити видалене складно, але для курсової це ок)
        return false;
    }
}

void AuthManager::ListUsers() const
{
    if (!this->IsAdmin())
    {
        cerr << "Error: Only admin can list users." << endl;
        return;
    }

    cout << "--- User List ---" << endl;
    for (const auto& pair : this->users)
    {
        // pair.second - це unique_ptr<BaseUser>
        cout << "- " << pair.second->GetUsername()
            << " [" << pair.second->GetUserType() << "]" << endl;
    }
    cout << "-----------------" << endl;
}

// --- Приватні методи (Завантаження/Збереження) ---

void AuthManager::LoadUsers()
{
    ifstream file(this->usersFilePath);
    if (!file.is_open())
    {
        cout << "Users file not found. Creating a new one with default admin..."
            << endl;

        // Створюємо default адміна
        this->users[ADMIN_USERNAME] =
            make_unique<AdminUser>(ADMIN_USERNAME, ADMIN_DEFAULT_PASS);

        this->SaveUsers(); // Зберігаємо новий файл
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

        // Новий формат: Type:Username:Password (вимога 4.5, 1.6)
        if (getline(ss, userType, ':') &&
            getline(ss, username, ':') &&
            getline(ss, password))
        {
            if (userType == "Admin")
            {
                this->users[username] =
                    make_unique<AdminUser>(username, password);
            }
            else if (userType == "Standard")
            {
                this->users[username] =
                    make_unique<StandardUser>(username, password);
            }
            else
            {
                cerr << "Warning: Skipping line " << lineCount
                    << " (unknown user type: " << userType << ")" << endl;
            }
        }
        else
        {
            cerr << "Warning: Skipping corrupted user data on line "
                << lineCount << endl;
        }
    }

    // Переконуємося, що адмін існує (вимога 1.6)
    if (this->users.find(ADMIN_USERNAME) == this->users.end())
    {
        cout << "Admin user not found. Adding default admin..." << endl;
        this->users[ADMIN_USERNAME] =
            make_unique<AdminUser>(ADMIN_USERNAME, ADMIN_DEFAULT_PASS);
        this->SaveUsers(); // Додаємо адміна у файл
    }

    file.close();
    cout << "Successfully loaded " << this->users.size() << " users." << endl;
}

void AuthManager::SaveUsers() const
{
    ofstream file(this->usersFilePath);
    if (!file.is_open())
    {
        throw runtime_error("Could not open users file for writing: "
            + this->usersFilePath);
    }

    for (const auto& pair : this->users)
    {
        // Викликаємо поліморфний метод ToFileString()
        file << pair.second->ToFileString() << endl;
    }
    file.close();
}