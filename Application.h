#pragma once

/*
 * Цей файл оголошує клас Application.
 * Application - це головний клас програми, який "володіє"
 * всіма основними менеджерами (Library, AuthManager)
 * та керує головним циклом програми.
 */

#include "Library.h"
#include "AuthManager.h"
 // #include "UIManager.h" // Ми додамо це на наступному кроці

 /**
  * @class Application
  * @brief Головний клас програми, що все об'єднує.
  *
  * Створює та зберігає екземпляри Library та AuthManager
  * і запускає головний цикл програми.
  */
class Application
{
public:
    /**
     * @brief Конструктор.
     * Ініціалізує всі менеджери.
     */
    Application();

    /**
     * @brief Деструктор.
     */
    ~Application();

    /**
     * @brief Запускає головний цикл програми.
     * Це основна точка входу в логіку програми.
     */
    void Run();

private:
    // --- Головні менеджери ---

    // "Мотор" для роботи з книгами
    Library library;

    // "Мотор" для роботи з користувачами
    AuthManager authManager;

    // "Панель приладів" (інтерфейс)
    // UIManager uiManager; // Ми додамо це на фінальному кроці
};