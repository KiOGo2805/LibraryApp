/*
 * Цей файл містить реалізацію методів класу Library.
 */

#include "Library.h"
#include <iostream>   // Для cout, cerr
#include <fstream>    // Для ifstream, ofstream (робота з файлами)
#include <sstream>    // Для stringstream (парсинг CSV)
#include <algorithm>  // Для std::find_if, std::sort
#include <stdexcept>  // Для std::invalid_argument, std::out_of_range

using namespace std;

// --- Конструктор та Деструктор ---

Library::Library(const string& dataFilePath)
    : dataFilePath(dataFilePath)
{
    // Вимога 4.1: "Програма зчитує дані з файлів при запуску."
    try
    {
        this->LoadFromFile();
    }
    catch (const exception& e)
    {
        // Вимога 3.2: "робота з файлами"
        cerr << "Error: Failed to load data file. " << e.what() << endl;
    }
}

Library::~Library()
{
    // Вимога 4.2: "При виході – зберігає оновлені дані."
    try
    {
        this->SaveToFile();
        cout << "Library data successfully saved to "
            << this->dataFilePath << endl;
    }
    catch (const exception& e)
    {
        cerr << "Error: Failed to save data file. " << e.what() << endl;
    }
}

// --- 1. Основні операції (CRUD) ---

bool Library::AddBook(const Book& book)
{
    // Перевірка на дублікат артикулу
    if (this->FindBookByArticle(book.GetId()) != nullptr)
    {
        cerr << "Error: Book with article "
            << book.GetId() << " already exists." << endl;
        return false;
    }

    this->books.push_back(book);
    return true;
}

bool Library::DeleteBook(const string& article)
{
    // Використовуємо find_if з лямбда-виразом для пошуку
    auto it = find_if(
        this->books.begin(),
        this->books.end(),
        [&article](const Book& b) { return b.GetId() == article; }
    );

    if (it != this->books.end())
    {
        this->books.erase(it); // Видаляємо елемент
        return true;
    }

    return false; // Не знайдено
}

bool Library::UpdateBook(const string& article, const Book& newBookData)
{
    Book* bookToUpdate = this->FindBookByArticle(article);
    if (bookToUpdate != nullptr)
    {
        *bookToUpdate = newBookData; // Використовуємо оператор =
        return true;
    }
    return false; // Не знайдено
}

// --- 2. Пошук (Завдання 1) ---

Book* Library::FindBookByArticle(const string& article)
{
    auto it = find_if(
        this->books.begin(),
        this->books.end(),
        [&article](const Book& b) { return b.GetId() == article; }
    );

    if (it != this->books.end())
    {
        return &(*it); // Повертаємо вказівник на знайдений об'єкт
    }

    return nullptr; // Не знайдено
}

const Book* Library::FindBookByArticle(const string& article) const
{
    // Const-версія методу
    auto it = find_if(
        this->books.cbegin(),
        this->books.cend(),
        [&article](const Book& b) { return b.GetId() == article; }
    );

    if (it != this->books.cend())
    {
        return &(*it);
    }

    return nullptr;
}

// --- 3. Фільтрація (Завдання 2) ---

vector<Book> Library::FilterByAuthor(const string& authorName) const
{
    vector<Book> results;
    for (const Book& book : this->books)
    {
        if (book.GetAuthorName() == authorName)
        {
            results.push_back(book);
        }
    }
    return results;
}

vector<Book> Library::FilterByShelf(int shelfNumber) const
{
    vector<Book> results;
    for (const Book& book : this->books)
    {
        if (book.GetShelfNumber() == shelfNumber)
        {
            results.push_back(book);
        }
    }
    return results;
}

// --- 4. Сортування (Вимога 2.5) ---

void Library::SortByTitle()
{
    sort(this->books.begin(), this->books.end(),
        [](const Book& a, const Book& b)
        {
            return a.GetBookTitle() < b.GetBookTitle();
        }
    );
}

void Library::SortByAuthor()
{
    sort(this->books.begin(), this->books.end(),
        [](const Book& a, const Book& b)
        {
            return a.GetAuthorName() < b.GetAuthorName();
        }
    );
}

void Library::SortByPrice()
{
    sort(this->books.begin(), this->books.end(),
        [](const Book& a, const Book& b)
        {
            return a.GetPrice() < b.GetPrice();
        }
    );
}

// --- 5. Допоміжні методи ---

const vector<Book>& Library::GetAllBooks() const
{
    return this->books;
}

bool Library::IsEmpty() const
{
    return this->books.empty();
}

// --- Приватні методи ---

void Library::LoadFromFile()
{
    // Вимога 4.3: формат CSV
    ifstream file(this->dataFilePath);
    if (!file.is_open())
    {
        cout << "Data file not found. A new one will be created on exit." << endl;
        return; // Файлу ще немає, це не помилка
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue; // Пропускаємо порожні рядки

        stringstream ss(line);
        string field;

        string article, authorName, bookTitle, readerFullName;
        double price;
        int shelfNumber;

        // Вимога 3.1, 3.2: Обробка помилок коректності даних
        try
        {
            // Розбираємо рядок CSV, який ми створили у Book::ToCsvString()
            // Формат: article,authorName,bookTitle,price,shelfNumber,readerFullName

            getline(ss, article, ',');
            getline(ss, authorName, ',');
            getline(ss, bookTitle, ',');

            getline(ss, field, ',');
            price = stod(field); // Може кинути виняток

            getline(ss, field, ',');
            shelfNumber = stoi(field); // Може кинути виняток

            getline(ss, readerFullName, ','); // Читаємо залишок рядка

            // Додаємо в колекцію
            this->books.push_back(Book(
                article, authorName, bookTitle,
                price, shelfNumber, readerFullName
            ));
        }
        catch (const invalid_argument& e)
        {
            cerr << "Warning: Skipping corrupted data line (invalid number format): "
                << line << endl;
        }
        catch (const out_of_range& e)
        {
            cerr << "Warning: Skipping corrupted data line (number out of range): "
                << line << endl;
        }
    }
    file.close();
    cout << "Successfully loaded " << this->books.size() << " books." << endl;
}

void Library::SaveToFile()
{
    ofstream file(this->dataFilePath);
    if (!file.is_open())
    {
        // Вимога 3.2
        throw runtime_error("Could not open file for writing: "
            + this->dataFilePath);
    }

    for (const Book& book : this->books)
    {
        file << book.ToCsvString() << endl;
    }

    file.close();
}