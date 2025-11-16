#include <algorithm>
#include <format>
#include <iostream>
#include <random>

#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statsistics.hpp"

using namespace bookdb;

// Вспомогательная функция для вывода map с std::format
template <typename Map>
void printMap(const Map &map, const std::string &title) {
    std::cout << std::format("{}:\n", title);
    for (const auto &[key, value] : map) {
        std::cout << std::format("  {}: {}\n", key, value);
    }
    std::cout << "\n";
}

int main() {
    // Create a book database
    BookDatabase<std::vector<Book>> db;

    // Add some books
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
    db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    db.EmplaceBack("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
    db.EmplaceBack("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
    db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);

    std::cout << std::format("BookDatabase with {} books:\n", db.size());
    for (const auto &book : db.GetBooks()) {
        std::cout << std::format("  {}\n", book);
    }
    std::cout << "\n";

    // Демонстрация гетерогенного поиска
    std::cout << "Heterogeneous lookup examples:\n";
    std::cout << std::format("Contains author 'George Orwell': {}\n", db.containsAuthor("George Orwell"));
    std::cout << std::format("Contains author 'Unknown': {}\n", db.containsAuthor("Unknown"));
    std::cout << "\n";

    // Author histogram с flat контейнером
    auto histogram = buildAuthorHistogramFlat(db);
    printMap(histogram, "Author histogram (flat)");

    // Остальной код остается аналогичным...
    auto genreRatings = calculateGenreRatings(db.begin(), db.end());
    printMap(genreRatings, "Average ratings by genres");

    auto avrRating = calculateAverageRating(db);
    std::cout << std::format("Average books rating in library: {:.2f}\n\n", avrRating);

    // Filters
    auto filtered = filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
    std::cout << "Books from the 20th century with rating ≥ 4.5:\n";
    for (const auto &book_ref : filtered) {
        std::cout << std::format("  {}\n", book_ref.get());
    }
    std::cout << "\n";

    // Random sample
    auto randomBooks = sampleRandomBooks(db, 3);
    std::cout << "Random sample of 3 books:\n";
    for (const auto &book_ref : randomBooks) {
        std::cout << std::format("  {}\n", book_ref.get());
    }

    return 0;
}