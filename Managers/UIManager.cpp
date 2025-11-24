/*
 * Цей файл містить реалізацію методів класу UIManager.
 */

#include "UIManager.h"
#include "Library.h"
#include "AuthManager.h"
#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include <algorithm>

using namespace std;

// --- КОНСТАНТИ (Вимога: Заміна літералів) ---
namespace
{
    // Налаштування меню
    const int LOGIN_MENU_COUNT = 2;
    const int ADMIN_MAIN_MENU_COUNT = 12;
    const int USER_MAIN_MENU_COUNT = 8;
    const int ADMIN_USER_MENU_COUNT = 4;

    // Повідомлення (Prompts)
    const string PROMPT_CHOICE = "Ваш вибір:";
    const string PROMPT_LOGIN = "Введіть логін:";
    const string PROMPT_PASSWORD = "Введіть пароль:";
    const string PROMPT_ARTICLE = "Введіть Артикул:";
    const string PROMPT_TITLE = "Введіть Назву:";
    const string PROMPT_AUTHOR = "Введіть Автора:";
    const string PROMPT_PRICE = "Введіть Ціну (грн):";
    const string PROMPT_SHELF = "Введіть Номер Полиці:";
    const string PROMPT_READER_NAME = "Введіть ПІБ читача:";
    const string PROMPT_CONTINUE = "\nНатисніть Enter щоб продовжити...";

    const string MSG_EXIT = "Завершення роботи програми. До побачення!";
    const string MSG_LOGIN_SUCCESS = "Вхід успішний. Вітаємо, ";
    const string MSG_SUCCESS = "Операцію виконано успішно.";
    const string MSG_CANCELLED = "Операцію скасовано.";
    const string MSG_EMPTY_LIB = "Бібліотека наразі порожня.";
    const string MSG_NOT_FOUND_SEARCH = "Нічого не знайдено.";

    // Помилки (Errors) - Узагальнені
    const string ERR_INVALID_INPUT = "Помилка: Некоректне введення. Перевірте формат даних.";
    const string ERR_LOGIN_FAILED = "Помилка: Невірний логін або пароль.";
    const string ERR_NOT_FOUND = "Помилка: Об'єкт не знайдено.";
    const string ERR_ALREADY_EXISTS = "Помилка: Такий об'єкт вже існує.";
    const string ERR_ACCESS_DENIED = "Помилка: Доступ заборонено.";
    const string ERR_ITEM_BUSY = "Помилка: Книга вже видана або недоступна.";
    const string ERR_ITEM_AVAILABLE = "Помилка: Книга вже в бібліотеці (не видана).";
    const string ERR_SELF_DELETE = "Помилка: Ви не можете видалити самі себе.";
    const string ERR_VALIDATION = "Помилка: Дані не відповідають вимогам (див. Допомогу).";
}

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

        if (!loggedIn) break;

        if (authManager->IsAdmin())
        {
            ShowAdminMainMenu();
        }
        else
        {
            ShowUserMainMenu();
        }
    }
    cout << MSG_EXIT << "\n";
}

bool UIManager::HandleLogin()
{
    while (true)
    {
        cout << "\n--- Вхід до Системи ---\n";
        cout << "1. Увійти\n";
        cout << "2. Вийти\n";
        int choice = GetMenuChoice(LOGIN_MENU_COUNT);

        if (choice == 2) return false;

        string username = GetStringInput(PROMPT_LOGIN);
        string password = GetStringInput(PROMPT_PASSWORD);

        if (authManager->Login(username, password))
        {
            cout << MSG_LOGIN_SUCCESS << authManager->GetCurrentUser() << "!\n";
            PressEnterToContinue();
            return true;
        }
        else
        {
            cout << ERR_LOGIN_FAILED << "\n";
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

        int choice = GetMenuChoice(ADMIN_MAIN_MENU_COUNT);

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

        int choice = GetMenuChoice(USER_MAIN_MENU_COUNT);

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

        int choice = GetMenuChoice(ADMIN_USER_MENU_COUNT);

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
    cout << "Керування колекцією книг, відстеження їх статусу та управління користувачами.\n";

    cout << "\n== Загальні правила ==\n";
    cout << "- Коли система запитує (y/n), введіть 'y' (так) або 'n' (ні).\n";
    cout << "- Всі поля повинні бути заповнені.\n";

    if (authManager->IsAdmin())
    {
        cout << "\n== Правила вводу даних (Тільки Адмін) ==\n";
        cout << "- Артикул: макс. 6 символів, латиниця/цифри.\n";
        cout << "- Назва/Автор: макс. 16 символів.\n";
        cout << "- Полиця: макс. 5 цифр.\n";
        cout << "- Ціна: число, більше 0.\n";

        cout << "\n== Команди Адміністратора ==\n";
        cout << "Додати/Оновити/Видалити книгу: Повне керування каталогом.\n";
        cout << "Адміністрування: Створення та видалення акаунтів.\n";
    }
    PressEnterToContinue();
}

// --- Допоміжні функції ---

Book* UIManager::PromptAndFindBook(const string& actionName)
{
    cout << "\n--- " << actionName << " ---\n";
    string article = GetStringInput("Введіть Артикул:");

    Book* book = library->FindBookByArticle(article);
    if (book == nullptr)
    {
        cout << ERR_NOT_FOUND << "\n";
        PressEnterToContinue();
    }
    return book;
}

string UIManager::GetValidNewArticle()
{
    while (true)
    {
        string article = GetStringInput("Введіть Артикул:");

        if (article.length() > 6)
        {
            cout << ERR_VALIDATION << "\n";
            continue;
        }

        // Перевірка на ASCII
        bool isValidChars = true;
        for (char c : article)
        {
            if (c < 0 || c > 127 || isspace(c))
            {
                isValidChars = false;
                break;
            }
        }

        if (!isValidChars)
        {
            cout << ERR_VALIDATION << "\n";
            continue;
        }

        if (library->FindBookByArticle(article) != nullptr)
        {
            cout << ERR_ALREADY_EXISTS << "\n";
            continue;
        }

        return article;
    }
}

// --- Дії з книгами ---

void UIManager::DoListAllBooks()
{
    cout << "\n--- Список Усіх Книг ---\n";
    if (library->IsEmpty())
    {
        cout << MSG_EMPTY_LIB << "\n";
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
        string article = GetValidNewArticle();
        if (article.empty()) return;

        string title = GetStringInput(PROMPT_TITLE);
        string author = GetStringInput(PROMPT_AUTHOR);
        double price = GetDoubleInput(PROMPT_PRICE);
        int shelf = GetIntInput(PROMPT_SHELF);

        Book newBook(article, author, title, price, shelf);
        if (library->AddBook(newBook))
        {
            cout << MSG_SUCCESS << "\n";
        }
    }
    catch (const exception&)
    {
        cout << ERR_INVALID_INPUT << "\n";
    }
    PressEnterToContinue();
}

void UIManager::DoFindBookByArticle()
{
    Book* book = PromptAndFindBook("Пошук Книги");
    if (book != nullptr)
    {
        cout << "Книгу знайдено:\n";
        book->Display();
        PressEnterToContinue();
    }
}

void UIManager::DoDeleteBook()
{
    Book* book = PromptAndFindBook("Видалення Книги");
    if (book != nullptr)
    {
        if (GetYesNoInput("Видалити цю книгу? (y/n):"))
        {
            if (library->DeleteBook(book->GetArticle()))
                cout << MSG_SUCCESS << "\n";
        }
        else
        {
            cout << MSG_CANCELLED << "\n";
        }
        PressEnterToContinue();
    }
}

void UIManager::DoIssueBook()
{
    Book* book = PromptAndFindBook("Видача Книги");
    if (book != nullptr)
    {
        if (!book->IsAvailable())
        {
            cout << ERR_ITEM_BUSY << "\n";
        }
        else
        {
            string readerName;
            if (authManager->IsAdmin())
                readerName = GetStringInput(PROMPT_READER_NAME);
            else
                readerName = authManager->GetCurrentUser();

            book->IssueToReader(readerName);
            cout << MSG_SUCCESS << "\n";
        }
        PressEnterToContinue();
    }
}

void UIManager::DoReturnBook()
{
    Book* book = PromptAndFindBook("Повернення Книги");
    if (book != nullptr)
    {
        if (book->IsAvailable())
        {
            cout << ERR_ITEM_AVAILABLE << "\n";
        }
        else
        {
            book->ReturnToLibrary();
            cout << MSG_SUCCESS << "\n";
        }
        PressEnterToContinue();
    }
}

void UIManager::DoUpdateBook()
{
    cout << "\n--- Оновлення Книги ---\n";

    cout << "Введіть Артикул книги: ";
    string article;
    getline(cin, article);

    Book* bookToUpdate = library->FindBookByArticle(article);
    if (bookToUpdate == nullptr)
    {
        cout << ERR_NOT_FOUND << "\n";
        PressEnterToContinue();
        return;
    }

    cout << "Поточні дані:\n";
    bookToUpdate->Display();
    cout << "Введіть нові дані (Enter - залишити старі):\n";

    Book updatedData = *bookToUpdate;
    string input;

    try
    {
        cout << "Назва: "; getline(cin, input);
        if (!input.empty()) updatedData.SetBookTitle(input);

        cout << "Автор: "; getline(cin, input);
        if (!input.empty()) updatedData.SetAuthorName(input);

        cout << "Ціна: "; getline(cin, input);
        if (!input.empty()) updatedData.SetPrice(stod(input));

        cout << "Полиця: "; getline(cin, input);
        if (!input.empty()) updatedData.SetShelfNumber(stoi(input));

        if (library->UpdateBook(article, updatedData))
        {
            cout << MSG_SUCCESS << "\n";
        }
    }
    catch (...)
    {
        cout << ERR_INVALID_INPUT << "\n";
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
        results = library->FilterByAuthor(GetStringInput(PROMPT_AUTHOR));
    else
        results = library->FilterByShelf(GetIntInput(PROMPT_SHELF));

    if (results.empty())
        cout << MSG_NOT_FOUND_SEARCH << "\n";
    else
        for (const auto& book : results) book.Display();

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

    cout << "Список відсортовано. ";
    if (GetYesNoInput("Показати? (y/n):"))
        DoListAllBooks();
}

// --- Дії з користувачами ---

void UIManager::DoListUsers()
{
    cout << "\n--- Список Користувачів ---\n";
    authManager->ListUsers();
    PressEnterToContinue();
}

void UIManager::DoCreateUser()
{
    cout << "\n--- Створення Користувача ---\n";
    string user = GetStringInput(PROMPT_LOGIN);
    string pass = GetStringInput(PROMPT_PASSWORD);

    authManager->CreateUser(user, pass, false); // Адмін створює тільки звичайних юзерів
    PressEnterToContinue();
}

void UIManager::DoDeleteUser()
{
    cout << "\n--- Видалення Користувача ---\n";
    string user = GetStringInput("Логін для видалення:");

    if (user == authManager->GetCurrentUser())
        cout << ERR_SELF_DELETE << "\n";
    else
        authManager->DeleteUser(user);

    PressEnterToContinue();
}

// --- Допоміжні функції ---

int UIManager::GetMenuChoice(int maxOption)
{
    int choice;
    while (true)
    {
        cout << PROMPT_CHOICE << " (1-" << maxOption << "): ";
        if (cin >> choice && choice >= 1 && choice <= maxOption)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
        cout << ERR_INVALID_INPUT << "\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string UIManager::GetStringInput(const string& prompt)
{
    string input;
    while (true)
    {
        cout << prompt << " ";
        getline(cin, input);
        if (!input.empty()) return input;
        cout << ERR_INVALID_INPUT << "\n";
    }
}

int UIManager::GetIntInput(const string& prompt)
{
    while (true)
    {
        try {
            string s = GetStringInput(prompt);
            size_t pos;
            int val = stoi(s, &pos);
            if (pos == s.length() && val >= 0) return val;
        }
        catch (...) {}
        cout << ERR_INVALID_INPUT << "\n";
    }
}

double UIManager::GetDoubleInput(const string& prompt)
{
    while (true)
    {
        try {
            string s = GetStringInput(prompt);
            size_t pos;
            double val = stod(s, &pos);
            if (pos == s.length() && val >= 0) return val;
        }
        catch (...) {}
        cout << ERR_INVALID_INPUT << "\n";
    }
}

bool UIManager::GetYesNoInput(const string& prompt)
{
    while (true)
    {
        string s = GetStringInput(prompt);
        if (s == "y" || s == "Y") return true;
        if (s == "n" || s == "N") return false;
        cout << "Будь ласка, введіть 'y' або 'n'.\n";
    }
}

void UIManager::PressEnterToContinue()
{
    cout << PROMPT_CONTINUE << flush;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    system("cls");
}