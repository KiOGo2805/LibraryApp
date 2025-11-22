#include "Book.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

Book::Book()
    : article("N/A"),
    authorName("Unknown"),
    bookTitle("Untitled"),
    price(0.0),
    shelfNumber(0),
    readerFullName("")
{
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
}

Book::Book(const Book& other)
    : article(other.article),
    authorName(other.authorName),
    bookTitle(other.bookTitle),
    price(other.price),
    shelfNumber(other.shelfNumber),
    readerFullName(other.readerFullName)
{
}

Book::Book(Book&& other) noexcept
    : article(std::move(other.article)),
    authorName(std::move(other.authorName)),
    bookTitle(std::move(other.bookTitle)),
    price(other.price),
    shelfNumber(other.shelfNumber),
    readerFullName(std::move(other.readerFullName))
{
    other.price = 0.0;
    other.shelfNumber = 0;
}

Book& Book::operator=(Book&& other) noexcept
{
    if (this == &other)
        return *this;

    this->article = std::move(other.article);
    this->authorName = std::move(other.authorName);
    this->bookTitle = std::move(other.bookTitle);
    this->price = other.price;
    this->shelfNumber = other.shelfNumber;
    this->readerFullName = std::move(other.readerFullName);

    other.price = 0.0;
    other.shelfNumber = 0;

    return *this;
}

Book::~Book()
{
    //cout << "Об'єкт книги '" << this->bookTitle
    //    << "' (Арт: " << this->article
    //    << ") знищується.\n";
}

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

string Book::GetId() const
{
    return this->article;
}

string Book::GetTypeName() const
{
    return "Book";
}

bool Book::IsAvailable() const
{
    return this->readerFullName.empty();
}

void Book::IssueToReader(const string& readerName)
{
    this->readerFullName = readerName;
}

void Book::ReturnToLibrary()
{
    this->readerFullName = "";
}

void Book::Display() const
{
    cout << "----------------------------------------\n";
    cout << "Назва:    " << this->bookTitle << "\n";
    cout << "Автор:    " << this->authorName << "\n";
    cout << "Артикул:  " << this->article << "\n";
    cout << "Ціна:     " << fixed << setprecision(2) << this->price << " грн\n";
    cout << "Полиця:   " << this->shelfNumber << "\n";

    if (IsAvailable())
    {
        cout << "Статус:   Доступна\n";
    }
    else
    {
        cout << "Статус:   Видана читачу " << this->readerFullName << "\n";
    }
    cout << "----------------------------------------\n";
}

string Book::ToCsvString() const
{
    stringstream ss;

    ss << fixed << setprecision(2) << this->price;
    string formattedPrice = ss.str();

    string csvRow = this->article + "," +
        this->authorName + "," +
        this->bookTitle + "," +
        formattedPrice + "," +
        to_string(this->shelfNumber) + "," +
        this->readerFullName;

    return csvRow;
}

Book& Book::operator=(const Book& other)
{
    if (this == &other)
    {
        return *this;
    }

    this->article = other.article;
    this->authorName = other.authorName;
    this->bookTitle = other.bookTitle;
    this->price = other.price;
    this->shelfNumber = other.shelfNumber;
    this->readerFullName = other.readerFullName;

    return *this;
}