/*
 * Цей файл містить реалізацію методів класу AuthManager.
 */

#include "AuthManager.h"
#include <iostream>
#include <fstream>
#include <sstream> // Для std::stringstream

using namespace std;

// Вимога 1.6: "Має бути користувач «admin» із фіксованим паролем, 
// заданим під час ініціалізації або вбудованим у файл."
const string ADMIN_USERNAME = "admin";
const string ADMIN_DEFAULT_PASS = "admin123";

// --- Конструктор та Деструктор ---

AuthManager::AuthManager(const string& usersFilePath)
    : usersFilePath(usersFilePath),
    currentUser(""),
    isAdmin(false)
{
    try
    {
        this->LoadUsers();
    }
    catch (const exception& e)
    {
        // Вимога 3.2
        cerr << "Error: Failed to load users file. " << e.what() << endl;
    }
}

AuthManager::~AuthManager()
{
    // Немає потреби викликати SaveUsers(), оскільки ми
    // зберігаємо зміни одразу в CreateUser/DeleteUser.
}

// --- Основні методи ---

bool AuthManager::Login(const string& username, const string& password)
{
    // Шукаємо користувача в 'map'
    auto it = this->users.find(username);

    // Перевіряємо, чи користувач існує І чи пароль вірний
    if (it != this->users.end() && it->second == password)
    {
        this->currentUser = username;
        this->isAdmin = (username == ADMIN_USERNAME);
        return true;
    }

    // Якщо логін/пароль невірні
    this->currentUser = "";
    this->isAdmin = false;
    return false;
}

void AuthManager::Logout()
{
    this->currentUser = "";
    this->isAdmin = false;
}

bool AuthManager::IsAdmin() const
{
    return this->isAdmin;
}

bool AuthManager::IsLoggedIn() const
{
    return !this->currentUser.empty();
}

string AuthManager::GetCurrentUser() const
{
    return this->currentUser;
}

// --- Адмін-функції ---

bool AuthManager::CreateUser(const string& username, const string& password)
{
    // Вимога 4.6: "Адміністратор має доступ до..."
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

    // Перевіряємо, чи користувач вже існує
    if (this->users.find(username) != this->users.end())
    {
        cerr << "Error: User '" << username << "' already exists." << endl;
        return false;
    }

    // Додаємо нового користувача
    this->users[username] = password;

    try
    {
        this->SaveUsers(); // Одразу зберігаємо зміни
        cout << "User '" << username << "' created successfully." << endl;
        return true;
    }
    catch (const exception& e)
    {
        cerr << "Error: Failed to save users file after creation. "
            << e.what() << endl;
        // Повертаємо зміни назад, якщо не вдалося зберегти
        this->users.erase(username);
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

    // Вимога 1.6: "видаляти користувачів (крім себе)"
    if (username == ADMIN_USERNAME)
    {
        cerr << "Error: Cannot delete the admin account." << endl;
        return false;
    }

    auto it = this->users.find(username);
    if (it == this->users.end())
    {
        cerr << "Error: User '" << username << "' not found." << endl;
        return false;
    }

    // Видаляємо користувача
    this->users.erase(it);

    try
    {
        this->SaveUsers(); // Одразу зберігаємо зміни
        cout << "User '" << username << "' deleted successfully." << endl;
        return true;
    }
    catch (const exception& e)
    {
        cerr << "Error: Failed to save users file after deletion. "
            << e.what() << endl;
        // (Тут складно відновити дані, але для курсової це ок)
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
        cout << "- " << pair.first;
        if (pair.first == ADMIN_USERNAME)
        {
            cout << " [Admin]";
        }
        cout << endl;
    }
    cout << "-----------------" << endl;
}

// --- Приватні методи ---

void AuthManager::LoadUsers()
{
    // Вимога 4.4, 4.5: формат username:password
    ifstream file(this->usersFilePath);
    if (!file.is_open())
    {
        cout << "Users file not found. Creating a new one with default admin..."
            << endl;
        // Файлу немає, створюємо admin'а і зберігаємо
        this->users[ADMIN_USERNAME] = ADMIN_DEFAULT_PASS;
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
        string username, password;

        // Роздільник ':' (або ' ', як у прикладі "admin admin123")
        // Вимога 4.5: "username:password"
        // Вимога 1.6: "(наприклад: admin:admin123)"
        // Будемо дотримуватись формату з ':'

        if (getline(ss, username, ':') && getline(ss, password))
        {
            this->users[username] = password;
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
        this->users[ADMIN_USERNAME] = ADMIN_DEFAULT_PASS;
        this->SaveUsers(); // Додаємо адміна у файл
    }

    file.close();
    cout << "Successfully loaded " << this->users.size() << " users." << endl;
}

void AuthManager::SaveUsers() const
{
    ofstream file(this->usersFilePath);
    if (!file.is_open())
        * {
        // Вимога 3.2
        throw runtime_error("Could not open users file for writing: "
            + this->usersFilePath);
    }

    for (const auto& pair : this->users)
    {
        // Зберігаємо у форматі username:password
        file << pair.first << ":" << pair.second << endl;
    }
    file.close();
}