#include "Application.h"
#include <iostream>

using namespace std;

namespace
{
    const string DB_FILE_PATH = "library_db.csv";
    const string USERS_FILE_PATH = "users.txt";
}

Application::Application()
    : library(DB_FILE_PATH),
    authManager(USERS_FILE_PATH),
    uiManager(&library, &authManager)
{
    cout << "Додаток ініціалізовано.\n";
}

Application::~Application()
{
    cout << "Додаток завершує роботу.\n";
}

void Application::Run()
{
    cout << "Додаток запущено...\n";

    uiManager.StartMainLoop();

    cout << "Робота додатка завершена.\n";
}