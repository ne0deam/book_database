#pragma once
#include "book.hpp"
#include <concepts>
#include <iterator>
#include <type_traits>

namespace bookdb {

template <typename T>
concept BookContainer = requires(T container) {
    typename T::value_type;
    requires std::is_same_v<typename T::value_type, Book>;
    { container.begin() } -> std::input_iterator;
    { container.end() } -> std::input_iterator;
    { container.size() } -> std::convertible_to<std::size_t>;
};

template <typename F>
concept BookPredicate = requires(F f, Book book) {
    { f(book) } -> std::convertible_to<bool>;
};

template <typename T>
concept StringLike = requires(T t) {
    { std::string_view(t) } -> std::convertible_to<std::string_view>;
};

template <typename T>
concept BookLike = requires(T t) {
    { t.title() } -> StringLike;
    { t.author() } -> StringLike;
    { t.year() } -> std::integral;
    { t.genre() } -> std::convertible_to<Genre>;
    { t.rating() } -> std::floating_point;
};

// Концепты для итераторов
template <typename I>
concept BookIterator = std::input_iterator<I> && BookLike<std::iter_value_t<I>>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I> && BookIterator<I>;

// Концепт для flat контейнеров
template <typename T>
concept FlatContainer =
    BookContainer<T> && requires(T container) { requires std::contiguous_iterator<typename T::iterator>; };

}  // namespace bookdb