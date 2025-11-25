#pragma once
#include "IStorable.h"
#include <string>
#include <iostream>

using namespace std;

/**
 * @class Book
 * @brief Клас, що представляє книгу в бібліотеці.
 *
 * Успадковує IStorable і зберігає всю інформацію про книгу,
 * таку як артикул, автор, назва, а також хто її взяв.
 */
class Book : public IStorable
{
private:
    string article;
    string authorName;
    string bookTitle;
    double price;
    int shelfNumber;
    string readerFullName;

public:
    /**
     * @brief Конструктор за замовчуванням.
     */
    Book();

    /**
     * @brief Конструктор з параметрами для створення книги.
     * @param article Артикул (унікальний ID).
     * @param authorName Ім'я автора.
     * @param bookTitle Назва книги.
     * @param price Вартість.
     * @param shelfNumber Номер етажерки.
     * @param readerFullName ПІБ читача (за замовчуванням порожньо).
     */
    Book(
        const string& article,
        const string& authorName,
        const string& bookTitle,
        double price,
        int shelfNumber,
        const string& readerFullName = ""
    );

    /**
     * @brief Конструктор копіювання.
     * @param other Інший об'єкт Book для копіювання.
     */
    Book(const Book& other);

    /**
     * @brief Конструктор переміщення (Move Constructor).
     */
    Book(Book&& other) noexcept;

    /**
     * @brief Оператор присвоєння з переміщенням (Move Assignment).
     */
    Book& operator=(Book&& other) noexcept;

    /**
     * @brief Деструктор.
     */
    ~Book();

    /**
     * @brief Встановлює артикул книги.
     * @param article Новий артикул.
     */
    void SetArticle(const string& article);

    /**
     * @brief Отримує артикул книги.
     * @return Артикул як string.
     */
    string GetArticle() const;

    void SetAuthorName(const string& authorName);
    string GetAuthorName() const;

    void SetBookTitle(const string& bookTitle);
    string GetBookTitle() const;

    void SetPrice(double price);
    double GetPrice() const;

    void SetShelfNumber(int shelfNumber);
    int GetShelfNumber() const;

    void SetReaderFullName(const string& readerFullName);
    string GetReaderFullName() const;

    string GetId() const override;
    string GetTypeName() const override;

    /**
     * @brief Перевіряє, чи книга доступна (не на руках у читача).
     * @return true, якщо доступна, false - якщо видана.
     */
    bool IsAvailable() const;

    /**
     * @brief Видає книгу читачу.
     * @param readerName ПІБ читача, якому видається книга.
     */
    void IssueToReader(const string& readerName);

    /**
     * @brief Повертає книгу до бібліотеки (робить доступною).
     */
    void ReturnToLibrary();

    /**
     * @brief Виводить інформацію про книгу в консоль.
     */
    void Display() const;

    /**
     * @brief Конвертує дані книги в рядок формату CSV.
     * @return Рядок string у форматі CSV.
     */
    string ToCsvString() const;

    /**
     * @brief Копіювальний оператор присвоєння.
     * @param other Інший об'єкт Book для копіювання.
     * @return Посилання на цей об'єкт (*this).
     */
    Book& operator=(const Book& other);
};