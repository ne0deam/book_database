#pragma once
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace bookdb {

enum class Genre { Fiction, NonFiction, Mystery, SciFi, Biography, Poetry, Unknown };

constexpr std::string_view to_string(Genre genre) {
    switch (genre) {
    case Genre::Fiction:
        return "Fiction";
    case Genre::NonFiction:
        return "NonFiction";
    case Genre::Mystery:
        return "Mystery";
    case Genre::SciFi:
        return "SciFi";
    case Genre::Biography:
        return "Biography";
    case Genre::Poetry:
        return "Poetry";
    default:
        return "Unknown";
    }
}

constexpr Genre from_string(std::string_view str) {
    if (str == "Fiction")
        return Genre::Fiction;
    if (str == "NonFiction")
        return Genre::NonFiction;
    if (str == "Mystery")
        return Genre::Mystery;
    if (str == "SciFi")
        return Genre::SciFi;
    if (str == "Biography")
        return Genre::Biography;
    if (str == "Poetry")
        return Genre::Poetry;
    return Genre::Unknown;
}

class Book {
public:
    constexpr Book(std::string_view title, std::string_view author, int year, Genre genre, double rating, int pages = 0)
        : title_(title), author_(author), year_(year), genre_(genre), rating_(rating), pages_(pages) {}

    constexpr Book(std::string_view title, std::string_view author, int year, std::string_view genre, double rating,
                   int pages = 0)
        : title_(title), author_(author), year_(year), genre_(from_string(genre)), rating_(rating), pages_(pages) {}

    // Основные геттеры
    constexpr std::string_view title() const { return title_; }
    constexpr std::string_view author() const { return author_; }
    constexpr int year() const { return year_; }
    constexpr Genre genre() const { return genre_; }
    constexpr double rating() const { return rating_; }
    constexpr int pages() const { return pages_; }

private:
    std::string title_;
    std::string author_;
    int year_;
    Genre genre_;
    double rating_;
    int pages_;
};

}  // namespace bookdb

// Форматтер для Genre
template <>
struct std::formatter<bookdb::Genre> : std::formatter<std::string_view> {
    auto format(bookdb::Genre genre, std::format_context &ctx) const {
        return std::formatter<std::string_view>::format(bookdb::to_string(genre), ctx);
    }
};

// Форматтер для Book
template <>
struct std::formatter<bookdb::Book> : std::formatter<std::string_view> {
    auto format(const bookdb::Book &book, std::format_context &ctx) const {
        std::string str = std::format("{} by {} ({}) [{}] {:.1f} ({} pages)", book.title(), book.author(), book.year(),
                                      std::format("{}", book.genre()), book.rating(), book.pages());
        return std::formatter<std::string_view>::format(str, ctx);
    }
};