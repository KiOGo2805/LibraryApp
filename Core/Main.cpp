#include "Application.h"
#include <iostream>
#include <exception>
#include <Windows.h>

using namespace std;

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
        cerr << "ÊÐÈÒÈ×ÍÀ ÏÎÌÈËÊÀ: " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        cerr << "Âèíèêëà íåâ³äîìà êðèòè÷íà ïîìèëêà.\n";
        return 2;
    }

    return 0;
}