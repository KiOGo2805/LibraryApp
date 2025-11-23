#include "UIManager.h"
#include "Library.h"
#include "AuthManager.h"
#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>

using namespace std;

UIManager::UIManager(Library* library, AuthManager* authManager)
    : library(library), authManager(authManager)
{
    if (this->library == nullptr || this->authManager == nullptr)
    {
        throw runtime_error("UIManager: Менеджери не ініціалізовані (null).");
    }
}

void UIManager::StartMainLoop()
{
    while (true)
    {
        bool loggedIn = HandleLogin();

        if (!loggedIn)
        {
            break;
        }

        if (authManager->IsAdmin())
        {
            ShowAdminMainMenu();
        }
        else
        {
            ShowUserMainMenu();
        }
    }

    cout << "Завершення роботи програми. До побачення!\n";
}

bool UIManager::HandleLogin()
{
    while (true)
    {
        cout << "\n--- Вхід до Системи ---\n";
        cout << "1. Увійти\n";
        cout << "2. Вийти\n";
        int choice = GetMenuChoice(2);

        if (choice == 2)
        {
            return false;
        }

        string username = GetStringInput("Введіть логін:");
        string password = GetStringInput("Введіть пароль:");

        if (authManager->Login(username, password))
        {
            cout << "Вхід успішний. Вітаємо, "
                << authManager->GetCurrentUser() << "!\n";
            PressEnterToContinue();
            return true;
        }
        else
        {
            cout << "Помилка: Невірний логін або пароль.\n";
            PressEnterToContinue();
        }
    }
}

void UIManager::ShowAdminMainMenu()
{
    bool running = true;
    while (running)
    {
        cout << "\n--- Головне Меню (Адміністратор) ---\n";
        cout << "Користувач: " << authManager->GetCurrentUser() << "\n";

        cout << "--- Керування Книгами ---\n";
        cout << "1. Список усіх книг\n";
        cout << "2. Додати нову книгу\n";
        cout << "3. Оновити книгу\n";
        cout << "4. Видалити книгу\n";

        cout << "--- Дії з Книгами ---\n";
        cout << "5. Пошук книги (за Артикулом)\n";
        cout << "6. Фільтрація книг\n";
        cout << "7. Сортування книг\n";
        cout << "8. Видати книгу читачу\n";
        cout << "9. Повернути книгу в бібліотеку\n";

        cout << "--- Система ---\n";
        cout << "10. Адміністрування (Користувачі)\n";
        cout << "11. Допомога\n";
        cout << "12. Вийти з акаунту\n";

        int choice = GetMenuChoice(12);

        switch (choice)
        {
        case 1: DoListAllBooks(); break;
        case 2: DoAddBook(); break;
        case 3: DoUpdateBook(); break;
        case 4: DoDeleteBook(); break;
        case 5: DoFindBookByArticle(); break;
        case 6: DoFilterBooks(); break;
        case 7: DoSortBooks(); break;
        case 8: DoIssueBook(); break;
        case 9: DoReturnBook(); break;
        case 10: ShowAdminMenu(); break;
        case 11: ShowHelpScreen(); break;
        case 12:
            running = false;
            authManager->Logout();
            break;
        }
    }
}

void UIManager::ShowUserMainMenu()
{
    bool running = true;
    while (running)
    {
        cout << "\n--- Головне Меню ---\n";
        cout << "Користувач: " << authManager->GetCurrentUser() << "\n";
        cout << "1. Список усіх книг\n";
        cout << "2. Пошук книги (за Артикулом)\n";
        cout << "3. Фільтрація книг\n";
        cout << "4. Сортування книг\n";
        cout << "5. Взяти книгу (Видача)\n";
        cout << "6. Повернути книгу\n";
        cout << "7. Допомога\n";
        cout << "8. Вийти з акаунту\n";

        int choice = GetMenuChoice(8);

        switch (choice)
        {
        case 1: DoListAllBooks(); break;
        case 2: DoFindBookByArticle(); break;
        case 3: DoFilterBooks(); break;
        case 4: DoSortBooks(); break;
        case 5: DoIssueBook(); break;
        case 6: DoReturnBook(); break;
        case 7: ShowHelpScreen(); break;
        case 8:
            running = false;
            authManager->Logout();
            break;
        }
    }
}

void UIManager::ShowAdminMenu()
{
    bool running = true;
    while (running)
    {
        cout << "\n--- Меню Адміністратора (Користувачі) ---\n";
        cout << "1. Список усіх користувачів\n";
        cout << "2. Створити нового користувача\n";
        cout << "3. Видалити користувача\n";
        cout << "4. Назад до Головного Меню\n";

        int choice = GetMenuChoice(4);

        switch (choice)
        {
        case 1: DoListUsers(); break;
        case 2: DoCreateUser(); break;
        case 3: DoDeleteUser(); break;
        case 4: running = false; break;
        }
    }
}

void UIManager::ShowHelpScreen()
{
    cout << "\n--- Довідка ---\n";
    cout << "Це автоматизована система управління бібліотекою.\n";

    cout << "\n== Призначення програми ==\n";
    cout << "Керування колекцією книг, відстеження їх статусу "
        << "(доступна/видана) та управління користувачами.\n";

    cout << "\n== Загальні правила ==\n";
    cout << "- Коли система запитує (y/n), введіть 'y' (так) або 'n' (ні).\n";

    cout << "\n== Опис команд ==\n";
    cout << "Список усіх книг: Показати всі книги, що є в базі.\n";
    cout << "Пошук книги: Знайти одну книгу за унікальним Артикулом.\n";
    cout << "Фільтрація: Показати книги за критерієм (автор або полиця).\n";
    cout << "Сортування: Впорядкувати список за назвою, автором або ціною.\n";
    cout << "Взяти книгу: Позначити книгу як видану читачу.\n";
    cout << "Повернути книгу: Позначити книгу як доступну в бібліотеці.\n";

    if (authManager->IsAdmin())
    {
        cout << "\n== Правила вводу даних (Тільки Адмін) ==\n";
        cout << "- Поля не можуть бути порожніми.\n";
        cout << "- Артикул має бути унікальним.\n";
        cout << "- Ціна та номер полиці мають бути позитивними числами.\n";

        cout << "\n== Команди Адміністратора ==\n";
        cout << "Додати/Оновити/Видалити книгу: "
            << "Повне керування каталогом.\n";
        cout << "Адміністрування (Користувачі): "
            << "Створення та видалення акаунтів.\n";
    }

    PressEnterToContinue();
}

void UIManager::DoListAllBooks()
{
    cout << "\n--- Список Усіх Книг ---\n";
    if (library->IsEmpty())
    {
        cout << "Бібліотека наразі порожня.\n";
    }
    else
    {
        for (const auto& book : library->GetAllBooks())
        {
            book.Display();
        }
    }
    PressEnterToContinue();
}

void UIManager::DoAddBook()
{
    cout << "\n--- Додавання Нової Книги ---\n";
    try
    {
        string article = GetStringInput("Введіть Артикул (унікальний):");
        if (library->FindBookByArticle(article) != nullptr)
        {
            cout << "Помилка: Книга з таким артикулом вже існує.\n";
            PressEnterToContinue();
            return;
        }

        string title = GetStringInput("Введіть Назву:");
        string author = GetStringInput("Введіть Автора:");
        double price = GetDoubleInput("Введіть Ціну (грн):");
        int shelf = GetIntInput("Введіть Номер Полиці:");

        Book newBook(article, author, title, price, shelf);
        if (library->AddBook(newBook))
        {
            cout << "Книгу '" << title << "' успішно додано.\n";
        }
    }
    catch (const exception& e)
    {
        cout << "Помилка при додаванні: " << e.what() << "\n";
    }
    PressEnterToContinue();
}

void UIManager::DoFindBookByArticle()
{
    cout << "\n--- Пошук Книги за Артикулом ---\n";
    string article = GetStringInput("Введіть Артикул для пошуку:");

    const Book* book = library->FindBookByArticle(article);
    if (book != nullptr)
    {
        cout << "Книгу знайдено:\n";
        cout << "Назва: " << book->GetBookTitle() << "\n";
        cout << "Автор: " << book->GetAuthorName() << "\n";
        if (book->IsAvailable())
        {
            cout << "Статус: Доступна\n";
        }
        else
        {
            cout << "Статус: Видана читачу " << book->GetReaderFullName() << "\n";
        }
    }
    else
    {
        cout << "На жаль, книги з таким артикулом не знайдено.\n";
    }
    PressEnterToContinue();
}

void UIManager::DoFilterBooks()
{
    cout << "\n--- Фільтрація Книг ---\n";
    cout << "1. За Автором\n";
    cout << "2. За Номером Полиці\n";
    int choice = GetMenuChoice(2);

    vector<Book> results;
    if (choice == 1)
    {
        string author = GetStringInput("Введіть ім'я Автора:");
        results = library->FilterByAuthor(author);
    }
    else
    {
        int shelf = GetIntInput("Введіть Номер Полиці:");
        results = library->FilterByShelf(shelf);
    }

    if (results.empty())
    {
        cout << "Книг за цим критерієм не знайдено.\n";
    }
    else
    {
        cout << "Знайдено " << results.size() << " книг:\n";
        for (const auto& book : results)
        {
            cout << "- " << book.GetBookTitle()
                << " (Автор: " << book.GetAuthorName() << ")\n";
        }
    }
    PressEnterToContinue();
}

void UIManager::DoSortBooks()
{
    cout << "\n--- Сортування Книг ---\n";
    cout << "1. За Назвою\n";
    cout << "2. За Автором\n";
    cout << "3. За Ціною\n";
    int choice = GetMenuChoice(3);

    if (choice == 1) library->SortByTitle();
    else if (choice == 2) library->SortByAuthor();
    else if (choice == 3) library->SortByPrice();

    cout << "Список книг відсортовано.\n";
    cout << "Відобразити список зараз?\n";
    if (GetYesNoInput("Відобразити? (y/n):"))
    {
        DoListAllBooks();
    }
}

void UIManager::DoUpdateBook()
{
    cout << "\n--- Оновлення Книги ---\n";

    //cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Введіть Артикул книги для оновлення: ";
    string article;
    getline(cin, article);

    Book* bookToUpdate = library->FindBookByArticle(article);
    if (bookToUpdate == nullptr)
    {
        cout << "Помилка: Книгу не знайдено.\n";
        PressEnterToContinue();
        return;
    }

    cout << "Книгу знайдено. Поточні дані:\n";
    bookToUpdate->Display();

    cout << "Введіть нові дані (залиште порожнім, щоб зберегти старі):\n";

    Book updatedData = *bookToUpdate;
    string input;

    try
    {
        cout << "Назва [" << bookToUpdate->GetBookTitle() << "]: ";
        getline(cin, input);
        if (!input.empty()) updatedData.SetBookTitle(input);

        cout << "Автор [" << bookToUpdate->GetAuthorName() << "]: ";
        getline(cin, input);
        if (!input.empty()) updatedData.SetAuthorName(input);

        cout << "Ціна [" << bookToUpdate->GetPrice() << "]: ";
        getline(cin, input);
        if (!input.empty())
        {
            double newPrice = stod(input);
            if (newPrice >= 0) updatedData.SetPrice(newPrice);
            else cout << "Попередження: Ціна не може бути від'ємною. Залишено старе значення.\n";
        }

        cout << "Полиця [" << bookToUpdate->GetShelfNumber() << "]: ";
        getline(cin, input);
        if (!input.empty())
        {
            int newShelf = stoi(input);
            if (newShelf >= 0) updatedData.SetShelfNumber(newShelf);
            else cout << "Попередження: Номер полиці має бути позитивним. Залишено старе значення.\n";
        }

        if (library->UpdateBook(article, updatedData))
        {
            cout << "Книгу оновлено успішно.\n";
        }
    }
    catch (const exception& e)
    {
        cout << "Помилка: Невірний формат числа. Оновлення скасовано.\n";
    }
    PressEnterToContinue();
}

void UIManager::DoDeleteBook()
{
    cout << "\n--- Видалення Книги ---\n";
    string article = GetStringInput("Введіть Артикул книги для видалення:");

    const Book* book = library->FindBookByArticle(article);
    if (book == nullptr)
    {
        cout << "Помилка: Книгу не знайдено.\n";
        PressEnterToContinue();
        return;
    }

    cout << "Знайдено книгу: " << book->GetBookTitle() << "\n";
    if (GetYesNoInput("Ви впевнені, що хочете видалити її? (y/n):"))
    {
        if (library->DeleteBook(article))
        {
            cout << "Книгу видалено.\n";
        }
        else
        {
            cout << "Помилка видалення.\n";
        }
    }
    else
    {
        cout << "Видалення скасовано.\n";
    }
    PressEnterToContinue();
}

void UIManager::DoIssueBook()
{
    cout << "\n--- Видача Книги ---\n";
    string article = GetStringInput("Введіть Артикул книги для видачі:");

    Book* book = library->FindBookByArticle(article);

    if (book == nullptr)
    {
        cout << "Помилка: Книгу не знайдено.\n";
    }
    else if (!book->IsAvailable())
    {
        cout << "Помилка: Ця книга вже видана читачу "
            << book->GetReaderFullName() << ".\n";
    }
    else
    {
        string readerName;

        if (authManager->IsAdmin())
        {
            readerName = GetStringInput("Введіть ПІБ читача:");
        }
        else
        {
            readerName = authManager->GetCurrentUser();
            cout << "Видача книги на ваш акаунт (" << readerName << ")...\n";
        }

        book->IssueToReader(readerName);
        cout << "Книгу успішно видано.\n";
    }
    PressEnterToContinue();
}

void UIManager::DoReturnBook()
{
    cout << "\n--- Повернення Книги ---\n";
    string article = GetStringInput("Введіть Артикул книги для повернення:");

    Book* book = library->FindBookByArticle(article);
    if (book == nullptr)
    {
        cout << "Помилка: Книгу не знайдено.\n";
    }
    else if (book->IsAvailable())
    {
        cout << "Помилка: Ця книга вже знаходиться в бібліотеці.\n";
    }
    else
    {
        cout << "Книгу '" << book->GetBookTitle()
            << "' повернуто від читача " << book->GetReaderFullName() << ".\n";
        book->ReturnToLibrary();
    }
    PressEnterToContinue();
}

void UIManager::DoListUsers()
{
    cout << "\n--- Список Користувачів ---\n";
    authManager->ListUsers();
    PressEnterToContinue();
}

void UIManager::DoCreateUser()
{
    cout << "\n--- Створення Користувача ---\n";
    string user = GetStringInput("Введіть новий логін:");
    string pass = GetStringInput("Введіть новий пароль:");

    bool isAdmin = false;

    authManager->CreateUser(user, pass, isAdmin);
    PressEnterToContinue();
}

void UIManager::DoDeleteUser()
{
    cout << "\n--- Видалення Користувача ---\n";
    string user = GetStringInput("Введіть логін для видалення:");

    if (user == authManager->GetCurrentUser())
    {
        cout << "Помилка: Ви не можете видалити самі себе.\n";
    }
    else
    {
        authManager->DeleteUser(user);
    }
    PressEnterToContinue();
}

int UIManager::GetMenuChoice(int maxOption)
{
    int choice;
    while (true)
    {
        cout << "Ваш вибір (1-" << maxOption << "): ";
        if (cin >> choice)
        {
            if (choice >= 1 && choice <= maxOption)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }
            else
            {
                cout << "Невірний вибір. Будь ласка, введіть число від 1 до "
                    << maxOption << ".\n";
            }
        }
        else
        {
            cout << "Некоректне введення. Введіть число.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string UIManager::GetStringInput(const string& prompt)
{
    string input;
    while (true)
    {
        cout << prompt << " ";
        getline(cin, input);
        if (!input.empty())
        {
            return input;
        }
        else
        {
            cout << "Введення не може бути порожнім.\n";
        }
    }
}

int UIManager::GetIntInput(const string& prompt)
{
    while (true)
    {
        string input = GetStringInput(prompt);
        try
        {
            int value = stoi(input);
            if (value >= 0) return value;
            else cout << "Число має бути позитивним.\n";
        }
        catch (const exception&)
        {
            cout << "Некоректне введення. Введіть ціле число.\n";
        }
    }
}

double UIManager::GetDoubleInput(const string& prompt)
{
    while (true)
    {
        string input = GetStringInput(prompt);
        try
        {
            double value = stod(input);
            if (value >= 0.0) return value;
            else cout << "Число має бути позитивним.\n";
        }
        catch (const exception&)
        {
            cout << "Некоректне введення. Введіть число (напр. 150.50).\n";
        }
    }
}

bool UIManager::GetYesNoInput(const string& prompt)
{
    while (true)
    {
        string input = GetStringInput(prompt);
        if (input == "y" || input == "Y") return true;
        if (input == "n" || input == "N") return false;
        cout << "Будь ласка, введіть 'y' (так) або 'n' (ні).\n";
    }
}

void UIManager::PressEnterToContinue()
{
    cout << "\nНатисніть Enter щоб продовжити..." << flush;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}