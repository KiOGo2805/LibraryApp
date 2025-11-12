/*
 * Цей файл містить реалізацію методів класу Application.
 */

#include "Application.h"
#include <iostream>

 // Вимога 4: Заміна числових/рядкових літералів символічними константами
namespace
{
    // Анонімний простір імен для констант,
    // видимих лише у цьому файлі
    const std::string DB_FILE_PATH = "library_db.csv";
    const std::string USERS_FILE_PATH = "users.txt";
}

// --- Конструктор та Деструктор ---

Application::Application()
    : library(DB_FILE_PATH),       // Ініціалізуємо Library
    authManager(USERS_FILE_PATH) // Ініціалізуємо AuthManager
{
    // Коли цей конструктор відпрацює,
    // 'library' та 'authManager' вже будуть створені
    // і автоматично завантажать свої дані з файлів
    // (як ми написали у їхніх конструкторах).

    std::cout << "Application initialized." << std::endl;
}

Application::~Application()
{
    // Деструктори 'library' та 'authManager' будуть викликані
    // АВТОМАТИЧНО, як тільки 'Application' знищиться.
    // Вони, в свою чергу, автоматично збережуть дані у файли
    // (як ми написали у їхніх деструкторах).

    std::cout << "Application shutting down." << std::endl;
}

// --- Головний метод ---

void Application::Run()
{
    std::cout << "Application is running..." << std::endl;

    //
    // *** ТУТ БУДЕ ГОЛОВНИЙ ЦИКЛ ПРОГРАМИ ***
    //
    // На наступному кроці ми створимо клас UIManager.
    // Ми ініціалізуємо його тут, передавши йому
    // вказівники або посилання на 'library' та 'authManager'.
    //
    // UIManager uiManager(&this->library, &this->authManager);
    // uiManager.StartMainLoop();
    //

    std::cout << "Application finished." << std::endl;
}