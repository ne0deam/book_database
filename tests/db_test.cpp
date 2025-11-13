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

    // Проверка итераторов
    EXPECT_EQ(std::distance(db.begin(), db.end()), 5);
    EXPECT_EQ(std::distance(db.cbegin(), db.cend()), 5);
}

TEST_F(BookDatabaseTest, AuthorsCollection) {
    const auto &authors = db.GetAuthors();
    EXPECT_EQ(authors.size(), 4);  // Tolkien, Hawking, Martin, Herbert
    EXPECT_TRUE(db.containsAuthor("J.R.R. Tolkien"));
    EXPECT_TRUE(db.containsAuthor("Stephen Hawking"));
    EXPECT_TRUE(db.containsAuthor("Robert Martin"));
    EXPECT_TRUE(db.containsAuthor("Frank Herbert"));
    EXPECT_FALSE(db.containsAuthor("Unknown Author"));
}

TEST_F(BookDatabaseTest, FilterBooks) {
    // Фильтр по автору - исправленная версия с итераторами
    auto tolkienBooks =
        filterBooks(db.begin(), db.end(), [](const Book &book) { return book.author() == "J.R.R. Tolkien"; });

    EXPECT_EQ(tolkienBooks.size(), 2);

    // Комплексный фильтр - исправленная версия с итераторами
    auto highRatedFiction = filterBooks(db.begin(), db.end(), all_of(RatingAbove(4.8), GenreIs(Genre::Fiction)));

    EXPECT_EQ(highRatedFiction.size(), 1);
    EXPECT_EQ(highRatedFiction[0].get().title(), "The Lord of the Rings");
}

TEST_F(BookDatabaseTest, StatisticsFunctions) {
    // Гистограмма авторов
    auto histogram = buildAuthorHistogramFlat(db);
    EXPECT_EQ(histogram["J.R.R. Tolkien"], 2);
    EXPECT_EQ(histogram["Stephen Hawking"], 1);
    EXPECT_EQ(histogram["Robert Martin"], 1);
    EXPECT_EQ(histogram["Frank Herbert"], 1);

    // Средний рейтинг
    auto avgRating = calculateAverageRating(db);
    EXPECT_GT(avgRating, 4.5);
    EXPECT_LT(avgRating, 5.0);

    // Рейтинги по жанрам
    auto genreRatings = calculateGenreRatings(db.begin(), db.end());
    EXPECT_TRUE(genreRatings.contains(Genre::Fiction));
    EXPECT_TRUE(genreRatings.contains(Genre::NonFiction));
    EXPECT_TRUE(genreRatings.contains(Genre::SciFi));

    // Топ-N книг
    auto topBooks = getTopNBy(db, 2, comp::LessByRating{});
    EXPECT_EQ(topBooks.size(), 2);
    // Проверяем что книги отсортированы по рейтингу
    EXPECT_GE(topBooks[0].get().rating(), topBooks[1].get().rating());
}

TEST_F(BookDatabaseTest, EmptyDatabase) {
    BookDatabase<std::vector<Book>> emptyDb;

    EXPECT_TRUE(emptyDb.empty());
    EXPECT_EQ(emptyDb.size(), 0);

    // Фильтрация пустой БД - исправленная версия с итераторами
    auto result = filterBooks(emptyDb.begin(), emptyDb.end(), RatingAbove(3.0));
    EXPECT_TRUE(result.empty());

    // Статистика пустой БД
    auto avgRating = calculateAverageRating(emptyDb);
    EXPECT_DOUBLE_EQ(avgRating, 0.0);

    auto histogram = buildAuthorHistogramFlat(emptyDb);
    EXPECT_TRUE(histogram.empty());

    auto topBooks = getTopNBy(emptyDb, 5, comp::LessByRating{});
    EXPECT_TRUE(topBooks.empty());
}
