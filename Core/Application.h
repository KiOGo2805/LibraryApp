#pragma once
#include "../Managers/Library.h"
#include "../Managers/AuthManager.h"
#include "../Managers/UIManager.h"

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