#pragma once

// Цей файл оголошує клас Library.
// Library — це менеджер-клас, який керує колекцією об'єктів Book,
// відповідає за їх завантаження, збереження та маніпуляції.

#include "Book.h"
#include <vector>
#include <string>

 /**
  * @class Library
  * @brief Менеджер-клас для управління базою даних книг.
  *
  * Відповідає за зберігання, пошук, фільтрацію, сортування,
  * а також завантаження та збереження даних у файл.
  */
class Library
{
public:
    /**
     * @brief Конструктор.
     * @param dataFilePath Шлях до файлу даних (напр., "db.csv").
     */
    Library(const std::string& dataFilePath);

    /**
     * @brief Деструктор.
     * Автоматично викликає SaveToFile() при виході з програми.
     */
    ~Library();

    /**
     * @brief Додає нову книгу до бібліотеки.
     * @param book Об'єкт Book, який потрібно додати.
     * @return true, якщо додавання успішне, false - якщо артикул вже існує.
     */
    bool AddBook(const Book& book);

    /**
     * @brief Видаляє книгу за її артикулом.
     * @param article Артикул книги для видалення.
     * @return true, якщо видалення успішне, false - якщо книга не знайдена.
     */
    bool DeleteBook(const std::string& article);

    /**
     * @brief Оновлює дані існуючої книги.
     * @param article Артикул книги, яку треба оновити.
     * @param newBookData Об'єкт Book з новими даними.
     * @return true, якщо оновлення успішне, false - якщо книга не знайдена.
     */
    bool UpdateBook(const std::string& article, const Book& newBookData);

    /**
     * @brief Знаходить книгу за артикулом.
     * @param article Артикул для пошуку.
     * @return Вказівник на знайдену книгу, або nullptr, якщо не знайдено.
     */
    Book* FindBookByArticle(const std::string& article);

    /**
     * @brief Знаходить книгу за артикулом (const-версія).
     * @param article Артикул для пошуку.
     * @return Вказівник на константну книгу, або nullptr.
     */
    const Book* FindBookByArticle(const std::string& article) const;

    /**
     * @brief Формує список книг за ім'ям автора.
     * @param authorName Ім'я автора для фільтрації.
     * @return std::vector<Book>, що містить лише відповідні книги.
     */
    std::vector<Book> FilterByAuthor(const std::string& authorName) const;

    /**
     * @brief Формує список книг за номером етажерки.
     * @param shelfNumber Номер етажерки для фільтрації.
     * @return std::vector<Book>, що містить лише відповідні книги.
     */
    std::vector<Book> FilterByShelf(int shelfNumber) const;

    void SortByTitle();
    void SortByAuthor();
    void SortByPrice();

    /**
     * @brief Отримує посилання на повний список книг.
     * @return Константне посилання на std::vector<Book>.
     */
    const std::vector<Book>& GetAllBooks() const;

    /**
     * @brief Перевіряє, чи порожня бібліотека.
     * @return true, якщо книг немає.
     */
    bool IsEmpty() const;

private:
    /**
     * @brief Завантажує дані з файлу (вимога 4.1).
     * Викликається конструктором.
     */
    void LoadFromFile();

    /**
     * @brief Зберігає дані у файл (вимога 4.2).
     * Викликається деструктором.
     */
    void SaveToFile();

    std::vector<Book> books;
    std::string dataFilePath;
};

