#include <algorithm>
#include <format>
#include <iostream>

#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statsistics.hpp"

using namespace bookdb;

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

    std::cout << "Books: " << db << "\n\n";

    // Sorts
    std::sort(db.begin(), db.end(), comp::LessByAuthor{});
    std::cout << "Books sorted by author: " << db << "\n\n==================\n";

    std::sort(db.begin(), db.end(), comp::LessByPopularity{});
    std::cout << "Books sorted by popularity: " << db << "\n\n==================\n";

    // Author histogram
    auto histogram = buildAuthorHistogramFlat(db);
    std::cout << "Author histogram: " << std::format("{}", histogram) << std::endl;

    // Ratings
    auto genreRatings = calculateGenreRatings(db.begin(), db.end());
    std::cout << "\n\nAverage ratings by genres: " << std::format("{}", genreRatings) << std::endl;

    auto avrRating = calculateAverageRating(db);
    std::cout << "Average books rating in library: " << avrRating << std::endl;

    // Filters
    auto filtered = filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
    std::cout << "\n\nBooks from the 20th century with rating ≥ 4.5:\n";
    std::for_each(filtered.cbegin(), filtered.cend(), [](const auto &v) { std::cout << std::format("{}\n", v.get()); });

    // Top 3 books
    auto topBooks = getTopNBy(db, 3, comp::LessByRating{});
    std::cout << "\n\nTop 3 books by rating:\n";
    std::for_each(topBooks.cbegin(), topBooks.cend(), [](const auto &v) { std::cout << std::format("{}\n", v.get()); });

    auto orwellBookIt = std::find_if(db.begin(), db.end(), [](const auto &v) { return v.author() == "George Orwell"; });
    if (orwellBookIt != db.end()) {
        std::cout << "\n\nTransparent lookup by authors. Found Orwell's book: " << std::format("{}", *orwellBookIt)
                  << std::endl;
    }

    return 0;
}