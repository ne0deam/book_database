#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statsistics.hpp"
#include <algorithm>
#include <gtest/gtest.h>

using namespace bookdb;

class BookDatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        db.EmplaceBack("The Lord of the Rings", "J.R.R. Tolkien", 1954, Genre::Fiction, 4.9, 1178);
        db.EmplaceBack("A Brief History of Time", "Stephen Hawking", 1988, Genre::NonFiction, 4.7, 256);
        db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.8, 310);
        db.EmplaceBack("Clean Code", "Robert Martin", 2008, Genre::NonFiction, 4.5, 464);
        db.EmplaceBack("Dune", "Frank Herbert", 1965, Genre::SciFi, 4.6, 412);
    }

    BookDatabase<std::vector<Book>> db;
};

TEST_F(BookDatabaseTest, InitializationAndBasicOperations) {
    EXPECT_FALSE(db.empty());
    EXPECT_EQ(db.size(), 5);
    EXPECT_EQ(std::distance(db.begin(), db.end()), 5);
}

TEST_F(BookDatabaseTest, AuthorsCollection) {
    const auto &authors = db.GetAuthors();
    EXPECT_EQ(authors.size(), 4);
    EXPECT_TRUE(db.containsAuthor("J.R.R. Tolkien"));
    EXPECT_TRUE(db.containsAuthor("Stephen Hawking"));
    EXPECT_FALSE(db.containsAuthor("Unknown Author"));
}

TEST_F(BookDatabaseTest, FilterBooksByAuthor) {
    auto tolkienBooks = filterBooks(db.GetBooks(), [](const Book &book) { return book.author() == "J.R.R. Tolkien"; });
    EXPECT_EQ(tolkienBooks.size(), 2);
}

TEST_F(BookDatabaseTest, FilterBooksByMultipleConditions) {
    auto highRatedFiction = filterBooks(db.GetBooks(), all_of(RatingAbove(4.8), GenreIs(Genre::Fiction)));
    EXPECT_EQ(highRatedFiction.size(), 1);
    EXPECT_EQ(highRatedFiction[0].get().title(), "The Lord of the Rings");
}

TEST_F(BookDatabaseTest, AuthorHistogram) {
    auto histogram = buildAuthorHistogramFlat(db);
    EXPECT_EQ(histogram["J.R.R. Tolkien"], 2);
    EXPECT_EQ(histogram["Stephen Hawking"], 1);
    EXPECT_EQ(histogram["Robert Martin"], 1);
    EXPECT_EQ(histogram["Frank Herbert"], 1);
}

TEST_F(BookDatabaseTest, AverageRating) {
    auto avgRating = calculateAverageRating(db);
    EXPECT_GT(avgRating, 4.5);
    EXPECT_LT(avgRating, 5.0);
}

TEST_F(BookDatabaseTest, GenreRatings) {
    auto genreRatings = calculateGenreRatings(db.begin(), db.end());
    EXPECT_TRUE(genreRatings.contains(Genre::Fiction));
    EXPECT_TRUE(genreRatings.contains(Genre::NonFiction));
    EXPECT_TRUE(genreRatings.contains(Genre::SciFi));
}

TEST_F(BookDatabaseTest, TopNBooks) {
    auto topBooks = getTopNBy(db, 2, comp::LessByRating{});
    EXPECT_EQ(topBooks.size(), 2);
    EXPECT_GE(topBooks[0].get().rating(), topBooks[1].get().rating());
}

TEST_F(BookDatabaseTest, SampleRandomBooks) {
    auto samples = sampleRandomBooks(db, 3);
    EXPECT_EQ(samples.size(), 3);

    for (const auto &sample : samples) {
        bool found = std::any_of(db.begin(), db.end(),
                                 [&sample](const Book &book) { return book.title() == sample.get().title(); });
        EXPECT_TRUE(found);
    }
}

TEST_F(BookDatabaseTest, EmptyDatabase) {
    BookDatabase<std::vector<Book>> emptyDb;
    EXPECT_TRUE(emptyDb.empty());

    auto result = filterBooks(emptyDb.GetBooks(), RatingAbove(3.0));
    EXPECT_TRUE(result.empty());

    auto avgRating = calculateAverageRating(emptyDb);
    EXPECT_DOUBLE_EQ(avgRating, 0.0);

    auto histogram = buildAuthorHistogramFlat(emptyDb);
    EXPECT_TRUE(histogram.empty());

    auto topBooks = getTopNBy(emptyDb, 5, comp::LessByRating{});
    EXPECT_TRUE(topBooks.empty());
}