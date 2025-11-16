#pragma once
#include "book_database.hpp"
#include "concepts.hpp"
#include <algorithm>
#include <format>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <ranges>
#include <unordered_map>

namespace bookdb {

// Используем flat контейнер для гистограммы
template <FlatContainer Container>
auto buildAuthorHistogramFlat(const BookDatabase<Container> &db) {
    std::unordered_map<std::string_view, std::size_t, StringViewHash, StringViewEqual> histogram;

    for (const auto &book : db.GetBooks()) {
        histogram[book.author()]++;
    }

    return histogram;
}

template <BookIterator Iterator>
auto calculateGenreRatings(Iterator begin, Iterator end) {
    std::map<Genre, std::pair<double, std::size_t>> genreStats;

    for (auto it = begin; it != end; ++it) {
        const auto &book = *it;
        auto &stats = genreStats[book.genre()];
        stats.first += book.rating();
        stats.second++;
    }

    std::map<Genre, double> result;
    for (const auto &[genre, stats] : genreStats) {
        result[genre] = stats.first / stats.second;
    }

    return result;
}

template <FlatContainer Container>
double calculateAverageRating(const BookDatabase<Container> &db) {
    const auto &books = db.GetBooks();
    if (books.empty())
        return 0.0;

    // Используем transform_reduce как рекомендовано
    double sum = std::transform_reduce(books.begin(), books.end(), 0.0, std::plus<>(),
                                       [](const Book &book) { return book.rating(); });

    return sum / books.size();
}

template <FlatContainer Container>
auto sampleRandomBooks(const BookDatabase<Container> &db, std::size_t count) {
    std::vector<std::reference_wrapper<const Book>> result;
    const auto &books = db.GetBooks();

    if (books.empty() || count == 0)
        return result;

    std::size_t actualCount = std::min(count, books.size());
    result.reserve(actualCount);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::reference_wrapper<const Book>> allBooks;
    allBooks.reserve(books.size());
    for (const auto &book : books) {
        allBooks.push_back(std::cref(book));
    }

    std::sample(allBooks.begin(), allBooks.end(), std::back_inserter(result), actualCount, gen);

    return result;
}

template <FlatContainer Container, typename Compare>
auto getTopNBy(const BookDatabase<Container> &db, std::size_t n, Compare comp) {
    std::vector<std::reference_wrapper<const Book>> result;
    const auto &books = db.GetBooks();

    if (books.empty() || n == 0)
        return result;

    std::size_t actualN = std::min(n, books.size());
    result.reserve(actualN);

    // Создаем вектор ссылок для сортировки
    std::vector<std::reference_wrapper<const Book>> allBooks;
    allBooks.reserve(books.size());
    for (const auto &book : books) {
        allBooks.emplace_back(book);
    }

    // Сортируем по компаратору
    std::partial_sort(allBooks.begin(), allBooks.begin() + actualN, allBooks.end(),
                      [&comp](const auto &lhs, const auto &rhs) { return comp(lhs.get(), rhs.get()); });

    result.assign(allBooks.begin(), allBooks.begin() + actualN);
    return result;
}

}  // namespace bookdb
// Форматтер для вывода std::map
template <typename K, typename V>
struct std::formatter<std::map<K, V>> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const std::map<K, V> &map, FormatContext &ctx) const {
        auto out = ctx.out();
        format_to(out, "{{");
        bool first = true;
        for (const auto &[key, value] : map) {
            if (!first)
                format_to(out, ", ");
            format_to(out, "{}: {}", key, value);
            first = false;
        }
        return format_to(out, "}}");
    }
};

// Форматтер для вывода std::unordered_map
template <typename K, typename V, typename H, typename E>
struct std::formatter<std::unordered_map<K, V, H, E>> {
    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const std::unordered_map<K, V, H, E> &map, FormatContext &ctx) const {
        auto out = ctx.out();
        format_to(out, "{{");
        bool first = true;
        for (const auto &[key, value] : map) {
            if (!first)
                format_to(out, ", ");
            format_to(out, "{}: {}", key, value);
            first = false;
        }
        return format_to(out, "}}");
    }
};