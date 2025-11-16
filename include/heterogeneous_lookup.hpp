#pragma once
#include <functional>
#include <string>
#include <string_view>


namespace bookdb {

// Хеш-функция, работающая со всеми типами строк
struct StringViewHash {
    using is_transparent = void;

    std::size_t operator()(std::string_view str) const { return std::hash<std::string>{}(std::string(str)); }

    std::size_t operator()(const std::string &str) const { return std::hash<std::string>{}(str); }

    std::size_t operator()(const char *str) const { return operator()(std::string_view(str)); }
};

// Компаратор для прозрачного сравнения всех типов строк
struct StringViewEqual {
    using is_transparent = void;

    bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs == rhs; }

    bool operator()(const std::string &lhs, std::string_view rhs) const { return lhs == rhs; }

    bool operator()(std::string_view lhs, const std::string &rhs) const { return lhs == rhs; }

    bool operator()(const std::string &lhs, const std::string &rhs) const { return lhs == rhs; }

    bool operator()(const char *lhs, std::string_view rhs) const { return std::string_view(lhs) == rhs; }

    bool operator()(std::string_view lhs, const char *rhs) const { return lhs == std::string_view(rhs); }

    bool operator()(const char *lhs, const std::string &rhs) const { return std::string_view(lhs) == rhs; }

    bool operator()(const std::string &lhs, const char *rhs) const { return lhs == std::string_view(rhs); }

    bool operator()(const char *lhs, const char *rhs) const { return std::string_view(lhs) == std::string_view(rhs); }
};

// Компаратор для упорядочивания
struct StringViewCompare {
    using is_transparent = void;

    bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs < rhs; }

    bool operator()(const std::string &lhs, std::string_view rhs) const { return lhs < rhs; }

    bool operator()(std::string_view lhs, const std::string &rhs) const { return lhs < rhs; }

    bool operator()(const std::string &lhs, const std::string &rhs) const { return lhs < rhs; }

    bool operator()(const char *lhs, std::string_view rhs) const { return std::string_view(lhs) < rhs; }

    bool operator()(std::string_view lhs, const char *rhs) const { return lhs < std::string_view(rhs); }
};

}  // namespace bookdb