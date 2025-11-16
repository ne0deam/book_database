#pragma once
#include "book.hpp"
#include "concepts.hpp"
#include <concepts>
#include <functional>
#include <vector>

namespace bookdb {

// Фабрики предикатов
auto YearBetween(int start, int end) {
    return [start, end](const Book &book) { return book.year() >= start && book.year() <= end; };
}

auto RatingAbove(double minRating) {
    return [minRating](const Book &book) { return book.rating() >= minRating; };
}

auto GenreIs(Genre genre) {
    return [genre](const Book &book) { return book.genre() == genre; };
}

// Композиция предикатов
template <typename... Preds>
auto all_of(Preds... preds) {
    return [... preds = std::move(preds)](const Book &book) { return (preds(book) && ...); };
}

template <typename... Preds>
auto any_of(Preds... preds) {
    return [... preds = std::move(preds)](const Book &book) { return (preds(book) || ...); };
}

// Алгоритм фильтрации с использованием концептов
template <BookIterator Iterator, BookSentinel<Iterator> Sentinel>
auto filterBooks(Iterator begin, Sentinel end, BookPredicate auto pred) {
    std::vector<std::reference_wrapper<const Book>> result;

    for (auto it = begin; it != end; ++it) {
        if (pred(*it)) {
            result.emplace_back(*it);
        }
    }

    return result;
}

// Перегрузка для контейнеров
template <BookContainer Container>
auto filterBooks(const Container &container, BookPredicate auto pred) {
    return filterBooks(container.begin(), container.end(), pred);
}

}  // namespace bookdb