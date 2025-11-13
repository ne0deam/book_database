#pragma once
#include "book.hpp"
#include <string_view>

namespace bookdb {

struct StringViewCompare {
    using is_transparent = void;

    bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs < rhs; }
};

struct BookTitleCompare {
    using is_transparent = void;

    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.title() < rhs.title(); }

    bool operator()(std::string_view lhs, const Book &rhs) const { return lhs < rhs.title(); }

    bool operator()(const Book &lhs, std::string_view rhs) const { return lhs.title() < rhs; }
};

struct BookAuthorCompare {
    using is_transparent = void;

    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.author() < rhs.author(); }

    bool operator()(std::string_view lhs, const Book &rhs) const { return lhs < rhs.author(); }

    bool operator()(const Book &lhs, std::string_view rhs) const { return lhs.author() < rhs; }
};

}  // namespace bookdb