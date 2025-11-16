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
    Book(std::string_view title, std::string_view author, int year, Genre genre, double rating, int pages = 0)
        : title_(title), author_(author), year_(year), genre_(genre), rating_(rating), pages_(pages) {}

    Book(std::string_view title, std::string_view author, int year, std::string_view genre, double rating,
         int pages = 0)
        : title_(title), author_(author), year_(year), genre_(from_string(genre)), rating_(rating), pages_(pages) {}

    // Основные геттеры
    std::string_view title() const { return title_; }
    std::string_view author() const { return author_; }
    int year() const { return year_; }
    Genre genre() const { return genre_; }
    double rating() const { return rating_; }
    int pages() const { return pages_; }

    // Сеттер для author
    void setAuthor(std::string_view author) { author_ = author; }

private:
    std::string title_;
    std::string_view author_;  // Оставляем string_view как требуется
    int year_;
    Genre genre_;
    double rating_;
    int pages_;
};

}  // namespace bookdb

// Упрощенный форматтер для Book
template <>
struct std::formatter<bookdb::Book> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const bookdb::Book &book, FormatContext &ctx) const {
        return format_to(ctx.out(), "{} by {} ({}) [{}] {:.1f} ({} pages)", book.title(), book.author(), book.year(),
                         bookdb::to_string(book.genre()), book.rating(), book.pages());
    }
};

// Упрощенный форматтер для Genre
template <>
struct std::formatter<bookdb::Genre> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(bookdb::Genre genre, FormatContext &ctx) const {
        return format_to(ctx.out(), "{}", bookdb::to_string(genre));
    }
};