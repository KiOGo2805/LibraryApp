/*
 * Цей файл містить реалізацію методів класу Book.
 */

#include "Book.h"
#include <iostream>  // Для std::cout (у Display та деструкторі)
#include <sstream>   // Для ToCsvString (для форматування рядка)
#include <iomanip>   // Для std::setprecision (для форматування ціни)

 // Тут безпечно використовувати "using namespace std",
 // оскільки це .cpp файл і він не вплине на інші файли.
using namespace std;

// --- 2. Конструктори та Деструктор ---

Book::Book()
    : article("N/A"),
    authorName("Unknown"),
    bookTitle("Untitled"),
    price(0.0),
    shelfNumber(0),
    readerFullName("")
{
    // Тіло конструктора за замовчуванням
}

Book::Book(
    const string& article,
    const string& authorName,
    const string& bookTitle,
    double price,
    int shelfNumber,
    const string& readerFullName
)
    : article(article),
    authorName(authorName),
    bookTitle(bookTitle),
    price(price),
    shelfNumber(shelfNumber),
    readerFullName(readerFullName)
{
    // Тіло конструктора з параметрами
    // Використовуємо список ініціалізації для присвоєння полів
}

Book::Book(const Book& other)
    : article(other.article),
    authorName(other.authorName),
    bookTitle(other.bookTitle),
    price(other.price),
    shelfNumber(other.shelfNumber),
    readerFullName(other.readerFullName)
{
    // Тіло конструктора копіювання
}

Book::~Book()
{
    // Вимога 2.3: "деструктор з повідомленням про знищення"
    cout << "Book object '" << this->bookTitle
        << "' (Art: " << this->article
        << ") is being destroyed." << endl;
}

// --- 3. Властивості (Методи Get/Set) ---

void Book::SetArticle(const string& article)
{
    this->article = article;
}

string Book::GetArticle() const
{
    return this->article;
}

void Book::SetAuthorName(const string& authorName)
{
    this->authorName = authorName;
}

string Book::GetAuthorName() const
{
    return this->authorName;
}

void Book::SetBookTitle(const string& bookTitle)
{
    this->bookTitle = bookTitle;
}

string Book::GetBookTitle() const
{
    return this->bookTitle;
}

void Book::SetPrice(double price)
{
    this->price = price;
}

double Book::GetPrice() const
{
    return this->price;
}

void Book::SetShelfNumber(int shelfNumber)
{
    this->shelfNumber = shelfNumber;
}

int Book::GetShelfNumber() const
{
    return this->shelfNumber;
}

void Book::SetReaderFullName(const string& readerFullName)
{
    this->readerFullName = readerFullName;
}

string Book::GetReaderFullName() const
{
    return this->readerFullName;
}

// --- 4. Методи ---

// Реалізація методів інтерфейсу IStorable
string Book::GetId() const
{
    return this->article;
}

string Book::GetTypeName() const
{
    return "Book";
}

// 5 власних методів (вимога 2.3)
bool Book::IsAvailable() const
{
    // Якщо ім'я читача порожнє, книга доступна
    return this->readerFullName.empty();
}

void Book::IssueToReader(const string& readerName)
{
    this->readerFullName = readerName;
}

void Book::ReturnToLibrary()
{
    this->readerFullName = ""; // Очищуємо ім'я читача
}

void Book::Display() const
{
    cout << "----------------------------------------" << endl;
    cout << "Title:    " << this->bookTitle << endl;
    cout << "Author:   " << this->authorName << endl;
    cout << "Article:  " << this->article << endl;
    cout << "Price:    " << fixed << setprecision(2) << this->price << " UAH" << endl;
    cout << "Shelf:    " << this->shelfNumber << endl;

    if (IsAvailable())
    {
        cout << "Status:   Available" << endl;
    }
    else
    {
        cout << "Status:   Issued to " << this->readerFullName << endl;
    }
    cout << "----------------------------------------" << endl;
}

string Book::ToCsvString() const
{
    // Використовуємо stringstream для простої конвертації
    stringstream ss;

    // Форматуємо ціну з 2 знаками після коми
    ss << fixed << setprecision(2) << this->price;
    string formattedPrice = ss.str();

    // Формуємо CSV рядок (вимога 4.3)
    // Увага: цей простий CSV не обробляє коми всередині полів.
    string csvRow = this->article + "," +
        this->authorName + "," +
        this->bookTitle + "," +
        formattedPrice + "," +
        to_string(this->shelfNumber) + "," +
        this->readerFullName;

    return csvRow;
}

// --- 5. Оператори ---

Book& Book::operator=(const Book& other)
{
    // 1. Перевірка на самоприсвоєння (важливо!)
    if (this == &other)
    {
        return *this;
    }

    // 2. Копіюємо дані
    this->article = other.article;
    this->authorName = other.authorName;
    this->bookTitle = other.bookTitle;
    this->price = other.price;
    this->shelfNumber = other.shelfNumber;
    this->readerFullName = other.readerFullName;

    // 3. Повертаємо *this
    return *this;
}