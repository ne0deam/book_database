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

}  // namespace bookdb