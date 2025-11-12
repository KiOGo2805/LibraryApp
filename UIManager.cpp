/*
 * Цей файл містить реалізацію методів класу UIManager.
 * Він керує всією логікою консольного інтерфейсу.
 */

#include "UIManager.h"
#include "Library.h"
#include "AuthManager.h"
#include <iostream>
#include <string>
#include <limits>    // Для std::numeric_limits
#include <stdexcept> // Для std::stod, std::stoi

using namespace std;

UIManager::UIManager(Library* library, AuthManager* authManager)
    : library(library), authManager(authManager)
{
    // Перевірка, чи "мотори" на місці
    if (this->library == nullptr || this->authManager == nullptr)
    {
        // Це критична помилка, програма не може працювати
        throw runtime_error("UIManager: Managers cannot be null.");
    }
}

// --- Головний цикл ---

void UIManager::StartMainLoop()
{
    // 1. Спочатку логін
    bool loggedIn = HandleLogin();

    // 2. Якщо увійшли, показуємо ВІДПОВІДНЕ меню
    if (loggedIn)
    {
        if (authManager->IsAdmin())
        {
            ShowAdminMainMenu(); // Викликаємо велике меню
        }
        else
        {
            ShowUserMainMenu(); // Викликаємо маленьке меню
        }
    }

    cout << "Exiting application. Goodbye!" << endl;
}

bool UIManager::HandleLogin()
{
    while (true)
    {
        cout << "\n--- Library Login ---" << endl;
        cout << "1. Login" << endl;
        cout << "2. Exit" << endl;
        int choice = GetMenuChoice(2);

        if (choice == 2)
        {
            return false; // Користувач вийшов
        }

        string username = GetStringInput("Enter username:");
        string password = GetStringInput("Enter password:");

        if (authManager->Login(username, password))
        {
            cout << "Login successful. Welcome, "
                << authManager->GetCurrentUser() << "!" << endl;
            PressEnterToContinue();
            return true; // Успішний вхід
        }
        else
        {
            // Вимога 3.2: "некоректне введення"
            cout << "Error: Invalid username or password." << endl;
            PressEnterToContinue();
        }
    }
}

void UIManager::ShowAdminMainMenu()
{
    bool running = true;
    while (running)
    {
        cout << "\n--- Main Menu (Admin) ---" << endl;
        cout << "Current User: " << authManager->GetCurrentUser() << endl;
        cout << "--- Book Management ---" << endl;
        cout << "1. List all books" << endl;
        cout << "2. Add a new book" << endl;
        cout << "3. Update a book" << endl;
        cout << "4. Delete a book" << endl;
        cout << "--- Book Actions ---" << endl;
        cout << "5. Find book (by Article)" << endl;
        cout << "6. Filter books" << endl;
        cout << "7. Sort books" << endl;
        cout << "8. Issue book to reader" << endl;
        cout << "9. Return book to library" << endl;
        cout << "--- System ---" << endl;
        cout << "10. Admin (User Management)" << endl;
        cout << "11. Help" << endl;
        cout << "12. Logout & Exit" << endl;

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
            running = false; // Вихід з циклу
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
        cout << "\n--- Main Menu ---" << endl;
        cout << "Current User: " << authManager->GetCurrentUser() << endl;
        cout << "1. List all books" << endl;
        cout << "2. Find book (by Article)" << endl;
        cout << "3. Filter books" << endl;
        cout << "4. Sort books" << endl;
        cout << "5. Issue book (Take)" << endl;
        cout << "6. Return book" << endl;
        cout << "7. Help" << endl;
        cout << "8. Logout & Exit" << endl;

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
            running = false; // Вихід з циклу
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
        cout << "\n--- Admin Menu (User Management) ---" << endl;
        cout << "1. List all users" << endl;
        cout << "2. Create a new user" << endl;
        cout << "3. Delete a user" << endl;
        cout << "4. Back to Main Menu" << endl;

        int choice = GetMenuChoice(4);

        switch (choice)
        {
        case 1: DoListUsers(); break;
        case 2: DoCreateUser(); break;
        case 3: DoDeleteUser(); break;
        case 4: running = false; break; // Вихід з адмін-меню
        }
    }
}

void UIManager::ShowHelpScreen()
{
    // Вимога 5
    cout << "\n--- Help Screen ---" << endl;
    cout << "This is a Library Management System." << endl;

    cout << "\n== Program Purpose ==" << endl;
    cout << "To manage a collection of books, track their status "
        << "(available/issued), and manage user accounts." << endl;

    // Загальне правило, яке бачать всі
    cout << "\n== General Rules ==" << endl;
    cout << "- When asked (y/n), enter 'y' for yes or 'n' for no." << endl;

    cout << "\n== Command Descriptions ==" << endl;
    cout << "List all books: Shows all books currently in the database." << endl;
    cout << "Find book: Searches for one book by its Article ID." << endl;
    cout << "Filter books: Shows all books matching a criteria (e.g., author)." << endl;
    cout << "Sort books: Sorts the entire list by title, author, or price." << endl;
    cout << "Issue book (Take): Mark a book as 'taken' by a reader." << endl;
    cout << "Return book: Mark a book as 'available' in the library." << endl;

    // Блок, який бачить ТІЛЬКИ адмін
    if (authManager->IsAdmin())
    {
        cout << "\n== Admin-Only Data Input Rules ==" << endl;
        cout << "- All inputs (e.g., Article, Title) cannot be empty." << endl;
        cout << "- Article (ID) must be unique for each book." << endl;
        cout << "- Price (e.g., 150.99) and Shelf (e.g., 5) "
            << "must be positive numbers." << endl;

        cout << "\n== Admin Commands ==" << endl;
        cout << "Add/Update/Delete Book: "
            << "Manage the book catalog." << endl;
        cout << "Admin (User Management): "
            << "Create/Delete user accounts." << endl;
    }

    PressEnterToContinue();
}

// --- Функції-дії (для книг) ---

void UIManager::DoListAllBooks()
{
    cout << "\n--- All Books ---" << endl;
    if (library->IsEmpty())
    {
        cout << "The library is currently empty." << endl;
    }
    else
    {
        for (const auto& book : library->GetAllBooks())
        {
            book.Display(); // Використовуємо наш метод Display()
        }
    }
    PressEnterToContinue();
}

void UIManager::DoAddBook()
{
    cout << "\n--- Add New Book ---" << endl;
    try
    {
        string article = GetStringInput("Enter Article (unique):");
        // Перевірка на унікальність
        if (library->FindBookByArticle(article) != nullptr)
        {
            cout << "Error: A book with this article already exists." << endl;
            PressEnterToContinue();
            return;
        }

        string title = GetStringInput("Enter Title:");
        string author = GetStringInput("Enter Author:");
        double price = GetDoubleInput("Enter Price:");
        int shelf = GetIntInput("Enter Shelf Number:");

        Book newBook(article, author, title, price, shelf);
        if (library->AddBook(newBook))
        {
            cout << "Book '" << title << "' added successfully." << endl;
        }
    }
    catch (const exception& e)
    {
        // Вимога 3.2
        cout << "Error adding book: " << e.what() << endl;
    }
    PressEnterToContinue();
}

void UIManager::DoFindBookByArticle()
{
    // Завдання 1
    cout << "\n--- Find Book by Article ---" << endl;
    string article = GetStringInput("Enter Article to find:");

    const Book* book = library->FindBookByArticle(article);
    if (book != nullptr)
    {
        cout << "Book found:" << endl;
        // Завдання: "визначити назву книги, автора і ПІБ читача"
        cout << "Title: " << book->GetBookTitle() << endl;
        cout << "Author: " << book->GetAuthorName() << endl;
        if (book->IsAvailable())
        {
            cout << "Status: Available" << endl;
        }
        else
        {
            cout << "Status: Issued to " << book->GetReaderFullName() << endl;
        }
    }
    else
    {
        cout << "Sorry, no book found with that article." << endl;
    }
    PressEnterToContinue();
}

void UIManager::DoFilterBooks()
{
    // Завдання 2
    cout << "\n--- Filter Books ---" << endl;
    cout << "1. Filter by Author" << endl;
    cout << "2. Filter by Shelf Number" << endl;
    int choice = GetMenuChoice(2);

    vector<Book> results;
    if (choice == 1)
    {
        string author = GetStringInput("Enter Author name:");
        results = library->FilterByAuthor(author);
    }
    else
    {
        int shelf = GetIntInput("Enter Shelf Number:");
        results = library->FilterByShelf(shelf);
    }

    if (results.empty())
    {
        cout << "No books found matching this criteria." << endl;
    }
    else
    {
        cout << "Found " << results.size() << " books:" << endl;
        for (const auto& book : results)
        {
            // Завдання: "сформувати базу даних з відомостей про назви книг"
            cout << "- " << book.GetBookTitle()
                << " (by " << book.GetAuthorName() << ")" << endl;
        }
    }
    PressEnterToContinue();
}

void UIManager::DoSortBooks()
{
    cout << "\n--- Sort Books ---" << endl;
    cout << "1. Sort by Title" << endl;
    cout << "2. Sort by Author" << endl;
    cout << "3. Sort by Price" << endl;
    int choice = GetMenuChoice(3);

    if (choice == 1) library->SortByTitle();
    else if (choice == 2) library->SortByAuthor();
    else if (choice == 3) library->SortByPrice();

    cout << "Book list has been sorted." << endl;
    cout << "Displaying sorted list:" << endl;
    DoListAllBooks(); // Покажемо відсортований список
}

void UIManager::DoUpdateBook()
{
    cout << "\n--- Update Book ---" << endl;
    string article = GetStringInput("Enter Article of book to update:");

    Book* bookToUpdate = library->FindBookByArticle(article);
    if (bookToUpdate == nullptr)
    {
        cout << "Error: Book not found." << endl;
        PressEnterToContinue();
        return;
    }

    cout << "Book found. Current data:" << endl;
    bookToUpdate->Display();
    cout << "Enter new data:" << endl;

    // Створюємо копію старих даних
    Book updatedData = *bookToUpdate;

    try
    {
        // Оновлюємо всі поля (для простоти)
        string title = GetStringInput("Enter new Title:");
        string author = GetStringInput("Enter new Author:");
        double price = GetDoubleInput("Enter new Price:");
        int shelf = GetIntInput("Enter new Shelf Number:");

        updatedData.SetBookTitle(title);
        updatedData.SetAuthorName(author);
        updatedData.SetPrice(price);
        updatedData.SetShelfNumber(shelf);
        // Статус (хто взяв) не чіпаємо

        if (library->UpdateBook(article, updatedData))
        {
            cout << "Book updated successfully." << endl;
        }
    }
    catch (const exception& e)
    {
        cout << "Error updating: " << e.what() << endl;
    }
    PressEnterToContinue();
}

void UIManager::DoDeleteBook()
{
    cout << "\n--- Delete Book ---" << endl;
    string article = GetStringInput("Enter Article of book to delete:");

    const Book* book = library->FindBookByArticle(article);
    if (book == nullptr)
    {
        cout << "Error: Book not found." << endl;
        PressEnterToContinue();
        return;
    }

    cout << "Found book: " << book->GetBookTitle() << endl;
    if (GetYesNoInput("Are you sure you want to delete it? (y/n):"))
    {
        if (library->DeleteBook(article))
        {
            cout << "Book deleted." << endl;
        }
        else
        {
            cout << "Error: Deletion failed." << endl;
        }
    }
    else
    {
        cout << "Deletion cancelled." << endl;
    }
    PressEnterToContinue();
}

void UIManager::DoIssueBook()
{
    cout << "\n--- Issue Book ---" << endl;
    string article = GetStringInput("Enter Article of book to issue:");

    Book* book = library->FindBookByArticle(article);
    if (book == nullptr)
    {
        cout << "Error: Book not found." << endl;
    }
    else if (!book->IsAvailable())
    {
        cout << "Error: Book is already issued to "
            << book->GetReaderFullName() << "." << endl;
    }
    else
    {
        string reader = GetStringInput("Enter Reader's Full Name:");
        book->IssueToReader(reader); // Змінюємо стан об'єкта
        cout << "Book issued to " << reader << "." << endl;
    }
    PressEnterToContinue();
}

void UIManager::DoReturnBook()
{
    cout << "\n--- Return Book ---" << endl;
    string article = GetStringInput("Enter Article of book to return:");

    Book* book = library->FindBookByArticle(article);
    if (book == nullptr)
    {
        cout << "Error: Book not found." << endl;
    }
    else if (book->IsAvailable())
    {
        cout << "Error: This book is already in the library." << endl;
    }
    else
    {
        cout << "Book '" << book->GetBookTitle()
            << "' returned from " << book->GetReaderFullName() << "." << endl;
        book->ReturnToLibrary(); // Змінюємо стан об'єкта
    }
    PressEnterToContinue();
}

// --- Функції-дії (для користувачів) ---

void UIManager::DoListUsers()
{
    cout << "\n--- List All Users ---" << endl;
    authManager->ListUsers(); // AuthManager вже має гарний вивід
    PressEnterToContinue();
}

void UIManager::DoCreateUser()
{
    cout << "\n--- Create New User ---" << endl;
    string user = GetStringInput("Enter new username:");
    string pass = GetStringInput("Enter new password:");
    bool isAdmin = GetYesNoInput("Is this user an admin? (y/n):");

    authManager->CreateUser(user, pass, isAdmin);
    PressEnterToContinue();
}

void UIManager::DoDeleteUser()
{
    cout << "\n--- Delete User ---" << endl;
    string user = GetStringInput("Enter username to delete:");

    if (user == authManager->GetCurrentUser())
    {
        cout << "Error: You cannot delete yourself." << endl;
    }
    else
    {
        authManager->DeleteUser(user);
    }
    PressEnterToContinue();
}


// --- Допоміжні функції (Валідація вводу) ---

int UIManager::GetMenuChoice(int maxOption)
{
    int choice;
    while (true)
    {
        cout << "Enter your choice (1-" << maxOption << "): ";
        if (cin >> choice)
        {
            if (choice >= 1 && choice <= maxOption)
            {
                // Очищення буфера вводу
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }
            else
            {
                cout << "Invalid choice. Please enter a number "
                    << "between 1 and " << maxOption << "." << endl;
            }
        }
        else
        {
            // Вимога 3.1: "коректність даних (формат, діапазон, тип)"
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear(); // Скидання прапорця помилки
            // Очищення буфера вводу
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
        getline(cin, input); // Читаємо цілий рядок
        if (!input.empty())
        {
            return input;
        }
        else
        {
            cout << "Input cannot be empty. Please try again." << endl;
        }
    }
}

int UIManager::GetIntInput(const string& prompt)
{
    while (true)
    {
        string input = GetStringInput(prompt);
        // Вимога 3.2: "try-catch"
        try
        {
            // stoi - string to integer
            int value = stoi(input);
            if (value >= 0)
            {
                return value;
            }
            else
            {
                cout << "Number must be zero or positive." << endl;
            }
        }
        catch (const invalid_argument&)
        {
            cout << "Invalid input. Please enter a valid number." << endl;
        }
        catch (const out_of_range&)
        {
            cout << "Number is too large. Please try again." << endl;
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
            // stod - string to double
            double value = stod(input);
            if (value >= 0.0)
            {
                return value;
            }
            else
            {
                cout << "Number must be zero or positive." << endl;
            }
        }
        catch (const invalid_argument&)
        {
            cout << "Invalid input. Please enter a valid number (e.g., 150.99)." << endl;
        }
        catch (const out_of_range&)
        {
            cout << "Number is too large. Please try again." << endl;
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
        cout << "Please enter 'y' (yes) or 'n' (no)." << endl;
    }
}

void UIManager::PressEnterToContinue()
{
    cout << "\nPress Enter to continue..." << flush;
    // Цей трюк очищує буфер вводу перед тим, як чекати
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}