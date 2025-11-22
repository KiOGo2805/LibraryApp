#pragma once

// Цей файл оголошує клас Application.
// Application - це головний клас програми, який "володіє"
// всіма основними менеджерами (Library, AuthManager)
// та керує головним циклом програми.

#include "Library.h"
#include "AuthManager.h"
#include "UIManager.h"

 /**
  * @class Application
  * @brief Головний клас програми, що все об'єднує.
  *
  * Створює та зберігає екземпляри Library та AuthManager
  * і запускає головний цикл програми.
  */
class Application
{
private:
    Library library;
    AuthManager authManager;
    UIManager uiManager;

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
};