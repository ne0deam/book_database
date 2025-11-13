#pragma once
#include "book_database.hpp"
#include <algorithm>
#include <format>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <ranges>

namespace bookdb {

template <typename Container>
auto buildAuthorHistogramFlat(const BookDatabase<Container> &db) {
    std::map<std::string, std::size_t> histogram;

    for (const auto &book : db.GetBooks()) {
        histogram[std::string(book.author())]++;
    }

    return histogram;
}

template <typename Iterator>
auto calculateGenreRatings(Iterator begin, Iterator end) {
    std::map<Genre, std::pair<double, std::size_t>> genreStats;

    for (auto it = begin; it != end; ++it) {
        const auto &book = *it;
        auto &stats = genreStats[book.genre()];
        stats.first += book.rating();
        stats.second++;
    }

    std::map<Genre, double> result;
    for (auto &[genre, stats] : genreStats) {
        result[genre] = stats.first / stats.second;
    }

    return result;
}

template <typename Container>
double calculateAverageRating(const BookDatabase<Container> &db) {
    const auto &books = db.GetBooks();
    if (books.empty())
        return 0.0;

    double sum = std::accumulate(books.begin(), books.end(), 0.0,
                                 [](double acc, const Book &book) { return acc + book.rating(); });

    return sum / books.size();
}

template <typename Container, typename Compare>
auto getTopNBy(BookDatabase<Container> &db, std::size_t n, Compare comp) {
    std::vector<std::reference_wrapper<const Book>> result;
    auto &books = const_cast<typename BookDatabase<Container>::container_type &>(db.GetBooks());

    if (books.empty() || n == 0)
        return result;

    std::size_t actualN = std::min(n, books.size());
    result.reserve(actualN);

    // Создаем копию для сортировки, чтобы не нарушать оригинальный порядок
    std::vector<std::reference_wrapper<const Book>> allBooks;
    allBooks.reserve(books.size());
    for (const auto &book : books) {
        allBooks.emplace_back(book);
    }

    // Сортируем по компаратору
    std::partial_sort(allBooks.begin(), allBooks.begin() + actualN, allBooks.end(),
                      [&comp](const Book &lhs, const Book &rhs) { return comp(lhs, rhs); });

    // Возвращаем топ-N
    result.assign(allBooks.begin(), allBooks.begin() + actualN);
    return result;
}

}  // namespace bookdb

// Форматтер для вывода map
template <typename K, typename V>
struct std::formatter<std::map<K, V>> : std::formatter<std::string_view> {
    auto format(const std::map<K, V> &map, std::format_context &ctx) const {
        std::string result = "{";
        bool first = true;
        for (const auto &[key, value] : map) {
            if (!first)
                result += ", ";
            result += std::format("{}: {}", key, value);
            first = false;
        }
        result += "}";
        return std::formatter<std::string_view>::format(result, ctx);
    }
};