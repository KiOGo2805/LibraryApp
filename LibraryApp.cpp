// Створює та запускає об'єкт Application.

#include "Application.h"
#include <iostream>
#include <exception>
#include <Windows.h>

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    try
    {
        Application app;
        app.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "КРИТИЧНА ПОМИЛКА: " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        std::cerr << "Виникла невідома критична помилка.\n";
        return 2;
    }

    return 0;
}