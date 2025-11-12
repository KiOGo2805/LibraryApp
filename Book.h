#pragma once

/*
 * Цей файл оголошує клас Book.
 * Book представляє сутність "книга" у бібліотеці, містить її дані
 * та реалізує інтерфейс IStorable.
 */

#include "IStorable.h" // Включаємо наш абстрактний базовий клас
#include <string>
#include <iostream> // Для деструктора з повідомленням

 // Згідно з Додатком А (1.2), використовуємо простір імен
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
    // Згідно з Додатком А (1.3), порядок: 1) поля, 2) конструктори...
    // Але також вимога (3): "Імена закритих полів... нотації «верблюд»"
    // І вимога (1.3): "властивості... public, notim protected, notim private"
    // Це трохи суперечливо. Зробимо за стандартом C++: поля в private.

public:
    // --- 2. Конструктори ---

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
     * @brief Конструктор копіювання (вимога 2.3).
     * @param other Інший об'єкт Book для копіювання.
     */
    Book(const Book& other);

    /**
     * @brief Деструктор (з повідомленням, вимога 2.3).
     */
    ~Book();

    // --- 3. Властивості (Методи Get/Set) ---

    /**
     * @brief Встановлює артикул книги.
     * @param article Новий артикул.
     */
    void SetArticle(const string& article);

    /**
     * @brief Отримує артикул книги.
     * @return Артикул як std::string.
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

    // --- 4. Методи ---

    // Реалізація віртуальних методів з IStorable
    string GetId() const override;
    string GetTypeName() const override;

    // 5 власних методів (вимога 2.3)

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
     * @return Рядок std::string у форматі CSV.
     */
    string ToCsvString() const;

    // --- 5. Оператори ---

    /**
     * @brief Копіювальний оператор присвоєння (вимога 2.3).
     * @param other Інший об'єкт Book для копіювання.
     * @return Посилання на цей об'єкт (*this).
     */
    Book& operator=(const Book& other);

protected:
    // Немає захищених членів

private:
    // --- 1. Поля ---
    // (Імена в camelCase, як вимагає пункт 3 для приватних полів)

    string article;
    string authorName;
    string bookTitle;
    double price;
    int shelfNumber;
    string readerFullName;
};