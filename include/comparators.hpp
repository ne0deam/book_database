#pragma once
#include "book.hpp"

namespace bookdb { namespace comp {

struct LessByTitle {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.title() < rhs.title(); }
};

struct LessByAuthor {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.author() < rhs.author(); }
};

struct LessByYear {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.year() < rhs.year(); }
};

struct LessByGenre {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.genre() < rhs.genre(); }
};

struct LessByRating {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.rating() < rhs.rating(); }
};

struct LessByPopularity {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.rating() < rhs.rating(); }
};

}}  // namespace bookdb::comp