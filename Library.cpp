#include "Library.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

Library::Library(const string& dataFilePath)
    : dataFilePath(dataFilePath)
{
    try
    {
        this->LoadFromFile();
    }
    catch (const exception& e)
    {
        cerr << "Помилка: Не вдалося завантажити файл даних. " << e.what() << "\n";
    }
}

Library::~Library()
{
    try
    {
        this->SaveToFile();
        cout << "Дані бібліотеки успішно збережено у файл " 
             << this->dataFilePath << "\n";
    }
    catch (const exception& e)
    {
        cerr << "Помилка: Не вдалося зберегти файл даних. " << e.what() << "\n";
    }
}

bool Library::AddBook(const Book& book)
{
    if (this->FindBookByArticle(book.GetId()) != nullptr)
    {
        cerr << "Помилка: Книга з артикулом " 
             << book.GetId() << " вже існує.\n";
        return false;
    }
    
    this->books.push_back(book);
    return true;
}

bool Library::DeleteBook(const string& article)
{
    auto it = find_if(
        this->books.begin(),
        this->books.end(),
        [&article](const Book& b) { return b.GetId() == article; }
    );

    if (it != this->books.end())
    {
        this->books.erase(it);
        return true;
    }

    return false;
}

bool Library::UpdateBook(const string& article, const Book& newBookData)
{
    Book* bookToUpdate = this->FindBookByArticle(article);
    if (bookToUpdate != nullptr)
    {
        *bookToUpdate = newBookData;
        return true;
    }
    return false;
}

Book* Library::FindBookByArticle(const string& article)
{
    auto it = find_if(
        this->books.begin(),
        this->books.end(),
        [&article](const Book& b) { return b.GetId() == article; }
    );

    if (it != this->books.end())
    {
        return &(*it);
    }

    return nullptr;
}

const Book* Library::FindBookByArticle(const string& article) const
{
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

const vector<Book>& Library::GetAllBooks() const
{
    return this->books;
}

bool Library::IsEmpty() const
{
    return this->books.empty();
}

void Library::LoadFromFile()
{
    ifstream file(this->dataFilePath);
    if (!file.is_open())
    {
        cout << "Файл даних не знайдено. Новий файл буде створено при виході.\n";
        return;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty()) continue;

        stringstream ss(line);
        string field;
        
        string article, authorName, bookTitle, readerFullName;
        double price;
        int shelfNumber;

        try
        {
            getline(ss, article, ',');
            getline(ss, authorName, ',');
            getline(ss, bookTitle, ',');
            
            getline(ss, field, ',');
            price = stod(field);
            
            getline(ss, field, ',');
            shelfNumber = stoi(field);

            getline(ss, readerFullName, ',');

            this->books.push_back(Book(
                article, authorName, bookTitle,
                price, shelfNumber, readerFullName
            ));
        }
        catch (const invalid_argument& e)
        {
            cerr << "Попередження: Пропущено пошкоджені дані (невірний формат числа): " 
                 << line << "\n";
        }
        catch (const out_of_range& e)
        {
             cerr << "Попередження: Пропущено пошкоджені дані (число за межами діапазону): " 
                 << line << "\n";
        }
    }
    file.close();
    cout << "Успішно завантажено " << this->books.size() << " книг.\n";
}

void Library::SaveToFile()
{
    ofstream file(this->dataFilePath);
    if (!file.is_open())
    {
        throw runtime_error("Не вдалося відкрити файл для запису: " 
                            + this->dataFilePath);
    }

    for (const Book& book : this->books)
    {
        file << book.ToCsvString() << "\n";
    }

    file.close();
}