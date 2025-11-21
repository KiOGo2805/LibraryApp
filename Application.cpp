#include "Application.h"
#include <iostream>

namespace
{
    const std::string DB_FILE_PATH = "library_db.csv";
    const std::string USERS_FILE_PATH = "users.txt";
}

Application::Application()
    : library(DB_FILE_PATH),
    authManager(USERS_FILE_PATH),
    uiManager(&library, &authManager)
{
    std::cout << "Додаток ініціалізовано.\n";
}

Application::~Application()
{
    std::cout << "Додаток завершує роботу.\n";
}

void Application::Run()
{
    std::cout << "Додаток запущено...\n";

    uiManager.StartMainLoop();

    std::cout << "Робота додатка завершена.\n";
}