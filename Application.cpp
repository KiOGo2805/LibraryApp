/*
 * Цей файл містить реалізацію методів класу Application.
 */

#include "Application.h"
#include <iostream>

namespace
{
    const std::string DB_FILE_PATH = "library_db.csv";
    const std::string USERS_FILE_PATH = "users.txt";
}

// 3. ОНОВИТИ СПИСОК ІНІЦІАЛІЗАЦІЇ
Application::Application()
    : library(DB_FILE_PATH),
    authManager(USERS_FILE_PATH),
    uiManager(&library, &authManager) // <-- Передаємо вказівники
{
    std::cout << "Application initialized." << std::endl;
}

Application::~Application()
{
    std::cout << "Application shutting down." << std::endl;
}

// 4. ОНОВИТИ МЕТОД RUN
void Application::Run()
{
    std::cout << "Application is running..." << std::endl;

    // Запускаємо головний цикл інтерфейсу
    uiManager.StartMainLoop();

    std::cout << "Application finished." << endl;
}