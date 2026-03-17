#include <gtest/gtest.h>
#include <containers/bor.hpp>
#include <vector>
#include <algorithm>
#include <random>

using Vec = std::vector<int>;
using VecVec = std::vector<std::vector<int>>;

// ── insert / find ────────────────────────────────────────────────────────────

TEST(BorInsertFind, FindAfterInsert) {
    Bor<int> b;
    b.insert(Vec{1, 2, 3});
    EXPECT_TRUE(b.find(Vec{1, 2, 3}));
}

TEST(BorInsertFind, PrefixNotFound) {
    Bor<int> b;
    b.insert(Vec{1, 2, 3});
    EXPECT_FALSE(b.find(Vec{1, 2}));
}

TEST(BorInsertFind, EmptyKeyInsertFind) {
    Bor<int> b;
    b.insert(Vec{});
    EXPECT_TRUE(b.find(Vec{}));
}

TEST(BorInsertFind, MultipleKeys) {
    Bor<int> b;
    b.insert(Vec{1, 2});
    b.insert(Vec{1, 3});
    b.insert(Vec{2});
    EXPECT_TRUE(b.find(Vec{1, 2}));
    EXPECT_TRUE(b.find(Vec{1, 3}));
    EXPECT_TRUE(b.find(Vec{2}));
    EXPECT_FALSE(b.find(Vec{1}));
}

TEST(BorInsertFind, DuplicateInsert) {
    Bor<int> b;
    b.insert(Vec{1, 2, 3});
    b.insert(Vec{1, 2, 3});
    EXPECT_EQ(b.size(), 1u);
}

// ── erase ────────────────────────────────────────────────────────────────────

TEST(BorErase, EraseExisting) {
    Bor<int> b;
    b.insert(Vec{1, 2, 3});
    b.erase(Vec{1, 2, 3});
    EXPECT_FALSE(b.find(Vec{1, 2, 3}));
}

TEST(BorErase, EraseNonExisting) {
    Bor<int> b;
    b.insert(Vec{1, 2, 3});
    b.erase(Vec{9, 9, 9});
    EXPECT_TRUE(b.find(Vec{1, 2, 3}));
}

TEST(BorErase, ErasePrefixDoesNotRemoveChild) {
    Bor<int> b;
    b.insert(Vec{1, 2});
    b.insert(Vec{1, 2, 3});
    b.erase(Vec{1, 2});
    EXPECT_FALSE(b.find(Vec{1, 2}));
    EXPECT_TRUE(b.find(Vec{1, 2, 3}));
}

// ── size / empty ─────────────────────────────────────────────────────────────

TEST(BorSize, EmptyOnConstruct) {
    Bor<int> b;
    EXPECT_TRUE(b.empty());
    EXPECT_EQ(b.size(), 0u);
}

TEST(BorSize, SizeAfterInserts) {
    Bor<int> b;
    b.insert(Vec{1});
    b.insert(Vec{2});
    b.insert(Vec{1, 2});
    EXPECT_EQ(b.size(), 3u);
}

TEST(BorSize, SizeAfterErase) {
    Bor<int> b;
    b.insert(Vec{1});
    b.insert(Vec{2});
    b.erase(Vec{1});
    EXPECT_EQ(b.size(), 1u);
    EXPECT_FALSE(b.empty());
}

// ── find_full ────────────────────────────────────────────────────────────────

TEST(BorFindFull, ExactMatch) {
    Bor<int> b;
    b.insert(Vec{1, 2});
    auto res = b.find_full(Vec{1, 2});
    ASSERT_EQ(res.size(), 1u);
    EXPECT_EQ(res[0], (Vec{1, 2}));
}

TEST(BorFindFull, PrefixReturnsAllSuffixes) {
    Bor<int> b;
    b.insert(Vec{1, 2});
    b.insert(Vec{1, 2, 3});
    b.insert(Vec{1, 2, 4});
    auto res = b.find_full(Vec{1, 2});
    EXPECT_EQ(res.size(), 3u);
    EXPECT_NE(std::find(res.begin(), res.end(), Vec{1, 2}),     res.end());
    EXPECT_NE(std::find(res.begin(), res.end(), Vec{1, 2, 3}),  res.end());
    EXPECT_NE(std::find(res.begin(), res.end(), Vec{1, 2, 4}),  res.end());
}

TEST(BorFindFull, EmptyPrefixReturnsAll) {
    Bor<int> b;
    b.insert(Vec{1});
    b.insert(Vec{2});
    b.insert(Vec{1, 2});
    auto res = b.find_full(Vec{});
    EXPECT_EQ(res.size(), 3u);
}

TEST(BorFindFull, MissingPrefixReturnsEmpty) {
    Bor<int> b;
    b.insert(Vec{1, 2, 3});
    auto res = b.find_full(Vec{9});
    EXPECT_TRUE(res.empty());
}

// ── iterators ────────────────────────────────────────────────────────────────

TEST(BorIterator, BeginEqualsEndOnEmpty) {
    Bor<int> b;
    EXPECT_EQ(b.begin(), b.end());
}

TEST(BorIterator, IteratesAllElements) {
    Bor<int> b;
    b.insert(Vec{1});
    b.insert(Vec{2});
    b.insert(Vec{1, 2});

    VecVec collected;
    for (auto it = b.begin(); it != b.end(); ++it) {
        collected.push_back(*it);
    }
    EXPECT_EQ(collected.size(), 3u);
    EXPECT_NE(std::find(collected.begin(), collected.end(), Vec{1}),    collected.end());
    EXPECT_NE(std::find(collected.begin(), collected.end(), Vec{2}),    collected.end());
    EXPECT_NE(std::find(collected.begin(), collected.end(), Vec{1, 2}), collected.end());
}

TEST(BorIterator, PostfixIncrement) {
    Bor<int> b;
    b.insert(Vec{1});
    b.insert(Vec{2});

    auto it = b.begin();
    auto old = it++;
    EXPECT_NE(old, it);
}

TEST(BorIterator, CbeginCend) {
    Bor<int> b;
    b.insert(Vec{1, 2});

    VecVec collected;
    for (auto it = b.cbegin(); it != b.cend(); ++it) {
        collected.push_back(*it);
    }
    EXPECT_EQ(collected.size(), 1u);
    EXPECT_EQ(collected[0], (Vec{1, 2}));
}

TEST(BorIterator, ConstBorIterable) {
    Bor<int> b;
    b.insert(Vec{1});
    b.insert(Vec{2});

    const Bor<int>& cb = b;
    VecVec collected;
    for (auto it = cb.begin(); it != cb.end(); ++it) {
        collected.push_back(*it);
    }
    EXPECT_EQ(collected.size(), 2u);
}

constexpr int seed_max = 400;
constexpr int max_keys = std::max(100, seed_max);


TEST(BorStress, InsertFindSize) {
    for (size_t seed = 0; seed < seed_max; ++seed) {
        std::mt19937 gen(seed);
        std::uniform_int_distribution<> dist(1, max_keys);
        std::vector<std::vector<int>> keys(max_keys);
        for (size_t i = 0; i < max_keys; ++i) {
            keys[i].resize(dist(gen));
            for (size_t j = 0; j < keys[i].size(); ++j) {
                keys[i][j] = dist(gen);
            }
        }
        Bor<int, std::map> b;
        for (const auto& key : keys) {
            b.insert(key);
        }
        for (const auto& key : keys) {
            EXPECT_TRUE(b.find(key)) << "seed=" << seed;
        }
        std::sort(keys.begin(), keys.end());
        keys.erase(std::unique(keys.begin(), keys.end()), keys.end());
        auto it2 = keys.begin();
        for (auto it = b.begin(); it != b.end(); ++it) {
            EXPECT_EQ(*it, *it2) << "seed=" << seed;
            ++it2;
        }
        EXPECT_TRUE(it2 == keys.end()) << "seed=" << seed;
    }
}
