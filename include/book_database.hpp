#pragma once
#include "book.hpp"
#include <algorithm>
#include <format>
#include <initializer_list>
#include <ranges>
#include <set>
#include <string>
#include <vector>

namespace bookdb {

template <typename Container = std::vector<Book>>
class BookDatabase {
public:
    // Псевдонимы типов
    using value_type = typename Container::value_type;
    using container_type = Container;
    using author_container_type = std::set<std::string, std::less<>>;

    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;
    using reverse_iterator = typename Container::reverse_iterator;
    using const_reverse_iterator = typename Container::const_reverse_iterator;
    using size_type = typename Container::size_type;

    // Конструкторы
    BookDatabase() = default;

    BookDatabase(std::initializer_list<Book> books) {
        for (const auto &book : books) {
            PushBack(book);
        }
    }

    // Итераторы
    iterator begin() noexcept { return books_.begin(); }
    const_iterator begin() const noexcept { return books_.begin(); }
    iterator end() noexcept { return books_.end(); }
    const_iterator end() const noexcept { return books_.end(); }
    const_iterator cbegin() const noexcept { return books_.cbegin(); }
    const_iterator cend() const noexcept { return books_.cend(); }
    reverse_iterator rbegin() noexcept { return books_.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return books_.rbegin(); }
    reverse_iterator rend() noexcept { return books_.rend(); }
    const_reverse_iterator rend() const noexcept { return books_.rend(); }

    // Модификаторы
    void PushBack(const Book &book) {
        books_.push_back(book);
        authors_.insert(std::string(book.author()));
    }

    void PushBack(Book &&book) {
        books_.push_back(std::move(book));
        authors_.insert(std::string(books_.back().author()));
    }

    template <typename... Args>
    void EmplaceBack(Args &&...args) {
        books_.emplace_back(std::forward<Args>(args)...);
        authors_.insert(std::string(books_.back().author()));
    }

    // Доступ к данным
    const container_type &GetBooks() const noexcept { return books_; }
    const author_container_type &GetAuthors() const noexcept { return authors_; }

    // Емкость
    bool empty() const noexcept { return books_.empty(); }
    size_type size() const noexcept { return books_.size(); }

    // Поиск
    template <typename T>
    bool containsAuthor(const T &author) const {
        return authors_.find(author) != authors_.end();
    }

    // Вывод в поток
    friend std::ostream &operator<<(std::ostream &os, const BookDatabase &db) {
        os << "BookDatabase with " << db.size() << " books:\n";
        for (const auto &book : db.books_) {
            os << "  " << std::format("{}", book) << "\n";
        }
        return os;
    }

private:
    container_type books_;
    author_container_type authors_;
};

}  // namespace bookdb