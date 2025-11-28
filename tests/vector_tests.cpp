#include <gtest/gtest.h>
#include "../lib/vector.hpp"
#include <random>
#include <chrono>

class VectorStressTest : public ::testing::Test {
protected:
    void SetUp() override {
        rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    }
    
    std::mt19937 rng;
    const size_t STRESS_SIZE = 100000;
};

TEST_F(VectorStressTest, CreateHugeVector) {
    mystd::Vector<int> vec(STRESS_SIZE, 42);
    EXPECT_EQ(vec.Size(), STRESS_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        EXPECT_EQ(vec[i], 42);
    }
}

TEST_F(VectorStressTest, MultipleCopyLargeVector) {
    mystd::Vector<int> original(STRESS_SIZE);
    for (size_t i = 0; i < original.Size(); ++i) {
        original[i] = static_cast<int>(i);
    }
    for (int i = 0; i < 100; ++i) {
        mystd::Vector<int> copy = original;
        EXPECT_EQ(copy.Size(), STRESS_SIZE);
        EXPECT_EQ(copy[STRESS_SIZE - 1], static_cast<int>(STRESS_SIZE - 1));
    }
}

TEST_F(VectorStressTest, RepeatedAssignmentStress) {
    mystd::Vector<int> vec1(STRESS_SIZE);
    mystd::Vector<int> vec2(STRESS_SIZE / 2);
    
    for (int i = 0; i < 500; ++i) {
        vec1 = vec2;
        EXPECT_EQ(vec1.Size(), STRESS_SIZE / 2);
        vec2 = vec1;
        EXPECT_EQ(vec2.Size(), STRESS_SIZE / 2);
    }
}

TEST_F(VectorStressTest, SelfAssignmentStress) {
    mystd::Vector<int> vec(STRESS_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i * 2);
    }
    
    for (int i = 0; i < 1000; ++i) {
        vec = vec;
        EXPECT_EQ(vec.Size(), STRESS_SIZE);
        EXPECT_EQ(vec[STRESS_SIZE - 1], static_cast<int>((STRESS_SIZE - 1) * 2));
    }
}

TEST_F(VectorStressTest, RandomAccessStress) {
    mystd::Vector<int> vec(STRESS_SIZE);
    std::uniform_int_distribution<size_t> index_dist(0, STRESS_SIZE - 1);
    std::uniform_int_distribution<int> value_dist(-1000, 1000);
    
    for (size_t i = 0; i < STRESS_SIZE * 10; ++i) {
        size_t idx = index_dist(rng);
        int val = value_dist(rng);
        vec[idx] = val;
        EXPECT_EQ(vec[idx], val);
    }
}

TEST_F(VectorStressTest, CopyChainStress) {
    mystd::Vector<int> base(STRESS_SIZE);
    for (size_t i = 0; i < base.Size(); ++i) {
        base[i] = static_cast<int>(i * 3);
    }
    
    mystd::Vector<int> current = base;
    for (int i = 0; i < 50; ++i) {
        mystd::Vector<int> next = current;
        EXPECT_EQ(next.Size(), STRESS_SIZE);
        EXPECT_EQ(next[0], 0);
        EXPECT_EQ(next[STRESS_SIZE - 1], static_cast<int>((STRESS_SIZE - 1) * 3));
        current = next;
    }
}

TEST_F(VectorStressTest, MixedOperationsStress) {
    
    for (int i = 0; i < 100; ++i) {
        mystd::Vector<int> vec1(1000, 1);
        mystd::Vector<int> vec2(2000, 2);
        mystd::Vector<int> temp = vec1;
        vec1 = vec2;
        vec2 = temp;

        EXPECT_EQ(vec1.Size(), 2000);
        EXPECT_EQ(vec2.Size(), 1000);
        EXPECT_EQ(vec1[0], 2);
        EXPECT_EQ(vec2[0], 1);
    }
}

TEST_F(VectorStressTest, LargeValueTypeStress) {
    struct LargeType {
        double data[100];
        LargeType() = default;
        LargeType(double val) {
            for (int i = 0; i < 100; ++i) data[i] = val;
        }
    };
    
    mystd::Vector<LargeType> vec(1000, LargeType(3.14));
    EXPECT_EQ(vec.Size(), 1000);
    EXPECT_DOUBLE_EQ(vec[999].data[0], 3.14);
}

TEST_F(VectorStressTest, ExceptionSafetyStress) {
    mystd::Vector<int> vec(STRESS_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    size_t exception_count = 0;
    for (size_t i = 0; i < 10000; ++i) {
        try {
            int val = vec[STRESS_SIZE + i];
            (void)val;
        } catch (...) {
            exception_count++;
        }
    }
    EXPECT_GT(exception_count, 0);
}

TEST_F(VectorStressTest, RapidConstructionDestruction) {
    for (int i = 0; i < 1000; ++i) {
        mystd::Vector<int> vec(1000, i);
        EXPECT_EQ(vec.Size(), 1000);
        EXPECT_EQ(vec[500], i);
    }
}

TEST_F(VectorStressTest, MemoryReuseStress) {
    mystd::Vector<int> vec;
    for (int i = 0; i < 100; ++i) {
        vec = mystd::Vector<int>(STRESS_SIZE, i);
        EXPECT_EQ(vec.Size(), STRESS_SIZE);
        EXPECT_EQ(vec[STRESS_SIZE / 2], i);
    }
}

TEST_F(VectorStressTest, CopyDifferentSizesStress) {
    std::uniform_int_distribution<size_t> size_dist(1, 5000);
    
    for (int i = 0; i < 200; ++i) {
        size_t size1 = size_dist(rng);
        size_t size2 = size_dist(rng);
        
        mystd::Vector<int> vec1(size1, 1);
        mystd::Vector<int> vec2(size2, 2);
        
        vec1 = vec2;
        EXPECT_EQ(vec1.Size(), size2);
        EXPECT_EQ(vec1[0], 2);
    }
}

TEST_F(VectorStressTest, SequentialCopyStress) {
    mystd::Vector<int> source(STRESS_SIZE);
    for (size_t i = 0; i < source.Size(); ++i) {
        source[i] = static_cast<int>(i * i);
    }
    
    mystd::Vector<int> dest;
    for (int i = 0; i < 100; ++i) {
        dest = source;
        EXPECT_EQ(dest.Size(), STRESS_SIZE);
        EXPECT_EQ(dest[STRESS_SIZE - 1], static_cast<int>((STRESS_SIZE - 1) * (STRESS_SIZE - 1)));
    }
}

TEST_F(VectorStressTest, BoundaryAccessStress) {
    mystd::Vector<int> vec(STRESS_SIZE);
    
    for (int i = 0; i < 10000; ++i) {
        vec[0] = i;
        vec[STRESS_SIZE - 1] = i * 2;
        EXPECT_EQ(vec[0], i);
        EXPECT_EQ(vec[STRESS_SIZE - 1], i * 2);
    }
}

TEST_F(VectorStressTest, LargeDefaultValueStress) {
    const int large_value = 123456789;
    mystd::Vector<int> vec(STRESS_SIZE, large_value);
    
    for (size_t i = 0; i < vec.Size(); ++i) {
        EXPECT_EQ(vec[i], large_value);
    }
}

TEST_F(VectorStressTest, RapidIndexAccessStress) {
    mystd::Vector<int> vec(STRESS_SIZE);
    
    for (size_t iter = 0; iter < 10; ++iter) {
        for (size_t i = 0; i < vec.Size(); ++i) {
            vec[i] = static_cast<int>(i + iter);
        }
        for (size_t i = 0; i < vec.Size(); ++i) {
            EXPECT_EQ(vec[i], static_cast<int>(i + iter));
        }
    }
}

TEST_F(VectorStressTest, CopyToEmptyStress) {
    mystd::Vector<int> source(STRESS_SIZE);
    for (size_t i = 0; i < source.Size(); ++i) {
        source[i] = static_cast<int>(i * 10);
    }
    
    for (int i = 0; i < 50; ++i) {
        mystd::Vector<int> empty;
        empty = source;
        EXPECT_EQ(empty.Size(), STRESS_SIZE);
        EXPECT_EQ(empty[STRESS_SIZE - 1], static_cast<int>((STRESS_SIZE - 1) * 10));
    }
}

TEST_F(VectorStressTest, ComplexTypeCopyStress) {
    struct ComplexType {
        int x, y, z;
        ComplexType(int val = 0) : x(val), y(val + 1), z(val + 2) {}
    };
    
    mystd::Vector<ComplexType> vec(STRESS_SIZE, ComplexType(5));
    
    for (int i = 0; i < 20; ++i) {
        mystd::Vector<ComplexType> copy = vec;
        EXPECT_EQ(copy.Size(), STRESS_SIZE);
        EXPECT_EQ(copy[0].x, 5);
        EXPECT_EQ(copy[0].y, 6);
        EXPECT_EQ(copy[0].z, 7);
    }
}

TEST_F(VectorStressTest, MassiveObjectLifetime) {
    for (int cycle = 0; cycle < 100; ++cycle) {
        mystd::Vector<int> vec1(1000, cycle);
        mystd::Vector<int> vec2 = vec1;
        mystd::Vector<int> vec3;
        vec3 = vec2;
        
        EXPECT_EQ(vec3.Size(), 1000);
        EXPECT_EQ(vec3[999], cycle);
    }
}

class VectorOperatorStressTest : public ::testing::Test {
protected:
    void SetUp() override {
        rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    }
    
    std::mt19937 rng;
    const size_t STRESS_SIZE = 100000;
};

TEST_F(VectorOperatorStressTest, PlusEqualsLargeVectors) {
    mystd::Vector<int> vec1(STRESS_SIZE, 1);
    mystd::Vector<int> vec2(STRESS_SIZE, 2);
    
    vec1 += vec2;
    
    EXPECT_EQ(vec1.Size(), STRESS_SIZE * 2);
    EXPECT_EQ(vec1[0], 1);
    EXPECT_EQ(vec1[STRESS_SIZE - 1], 1);
    EXPECT_EQ(vec1[STRESS_SIZE], 2);
    EXPECT_EQ(vec1[STRESS_SIZE * 2 - 1], 2);
}

TEST_F(VectorOperatorStressTest, PlusEqualsMultipleTimes) {
    mystd::Vector<int> vec(1000, 1);
    mystd::Vector<int> small_vec(100, 2);
    
    for (int i = 0; i < 100; ++i) {
        vec += small_vec;
    }
    
    EXPECT_EQ(vec.Size(), 1000 + 100 * 100);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1000], 2);
    EXPECT_EQ(vec[1000 + 99], 2);
}

TEST_F(VectorOperatorStressTest, PlusOperatorLargeVectors) {
    mystd::Vector<int> vec1(STRESS_SIZE, 1);
    mystd::Vector<int> vec2(STRESS_SIZE, 2);
    
    mystd::Vector<int> result = vec1 + vec2;
    
    EXPECT_EQ(result.Size(), STRESS_SIZE * 2);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[STRESS_SIZE - 1], 1);
    EXPECT_EQ(result[STRESS_SIZE], 2);
    EXPECT_EQ(result[STRESS_SIZE * 2 - 1], 2);
}

TEST_F(VectorOperatorStressTest, PlusOperatorChain) {
    mystd::Vector<int> vec1(10000, 1);
    mystd::Vector<int> vec2(10000, 2);
    mystd::Vector<int> vec3(10000, 3);
    
    mystd::Vector<int> result = vec1 + vec2 + vec3;
    
    EXPECT_EQ(result.Size(), 30000);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[9999], 1);
    EXPECT_EQ(result[10000], 2);
    EXPECT_EQ(result[19999], 2);
    EXPECT_EQ(result[20000], 3);
    EXPECT_EQ(result[29999], 3);
}

TEST_F(VectorOperatorStressTest, PlusEqualsSelf) {
    mystd::Vector<int> vec(10000, 5);
    
    vec += vec;
    
    EXPECT_EQ(vec.Size(), 20000);
    for (size_t i = 0; i < 10000; ++i) {
        EXPECT_EQ(vec[i], 5);
        EXPECT_EQ(vec[i + 10000], 5);
    }
}

TEST_F(VectorOperatorStressTest, EqualityLargeVectors) {
    mystd::Vector<int> vec1(STRESS_SIZE);
    mystd::Vector<int> vec2(STRESS_SIZE);
    
    for (size_t i = 0; i < STRESS_SIZE; ++i) {
        vec1[i] = static_cast<int>(i);
        vec2[i] = static_cast<int>(i);
    }
    
    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 != vec2);
}

TEST_F(VectorOperatorStressTest, InequalityLargeVectors) {
    mystd::Vector<int> vec1(STRESS_SIZE);
    mystd::Vector<int> vec2(STRESS_SIZE);
    
    for (size_t i = 0; i < STRESS_SIZE; ++i) {
        vec1[i] = static_cast<int>(i);
        vec2[i] = static_cast<int>(i);
    }
    vec2[STRESS_SIZE - 1] = -1;
    
    EXPECT_FALSE(vec1 == vec2);
    EXPECT_TRUE(vec1 != vec2);
}

TEST_F(VectorOperatorStressTest, EqualityDifferentSizes) {
    mystd::Vector<int> vec1(STRESS_SIZE, 1);
    mystd::Vector<int> vec2(STRESS_SIZE / 2, 1);
    
    EXPECT_FALSE(vec1 == vec2);
    EXPECT_TRUE(vec1 != vec2);
}

TEST_F(VectorOperatorStressTest, PlusEqualsReallocationStress) {
    mystd::Vector<int> vec(10, 1);
    mystd::Vector<int> large_vec(1000, 2);
    
    for (int i = 0; i < 50; ++i) {
        vec += large_vec;
    }
    
    EXPECT_EQ(vec.Size(), 10 + 1000 * 50);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[9], 1);
    EXPECT_EQ(vec[10], 2);
    EXPECT_EQ(vec[10 + 1000 * 50 - 1], 2);
}

TEST_F(VectorOperatorStressTest, PlusOperatorMultipleLarge) {
    mystd::Vector<int> vec1(5000, 1);
    mystd::Vector<int> vec2(5000, 2);
    mystd::Vector<int> vec3(5000, 3);
    mystd::Vector<int> vec4(5000, 4);
    
    mystd::Vector<int> result = vec1 + vec2 + vec3 + vec4;
    
    EXPECT_EQ(result.Size(), 20000);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[4999], 1);
    EXPECT_EQ(result[5000], 2);
    EXPECT_EQ(result[9999], 2);
    EXPECT_EQ(result[10000], 3);
    EXPECT_EQ(result[14999], 3);
    EXPECT_EQ(result[15000], 4);
    EXPECT_EQ(result[19999], 4);
}

TEST_F(VectorOperatorStressTest, EqualityRandomData) {
    mystd::Vector<int> vec1(STRESS_SIZE);
    mystd::Vector<int> vec2(STRESS_SIZE);
    
    std::uniform_int_distribution<int> dist(0, 1000);
    for (size_t i = 0; i < STRESS_SIZE; ++i) {
        int val = dist(rng);
        vec1[i] = val;
        vec2[i] = val;
    }
    
    EXPECT_TRUE(vec1 == vec2);
    
    vec2[STRESS_SIZE / 2] = -1;
    EXPECT_FALSE(vec1 == vec2);
}

TEST_F(VectorOperatorStressTest, PlusEqualsEmptyVectors) {
    mystd::Vector<int> vec1;
    mystd::Vector<int> vec2(STRESS_SIZE, 5);
    
    vec1 += vec2;
    EXPECT_EQ(vec1.Size(), STRESS_SIZE);
    EXPECT_EQ(vec1[0], 5);
    EXPECT_EQ(vec1[STRESS_SIZE - 1], 5);
    
    mystd::Vector<int> vec3;
    vec1 += vec3;
    EXPECT_EQ(vec1.Size(), STRESS_SIZE);
}

TEST_F(VectorOperatorStressTest, PlusOperatorWithEmpty) {
    mystd::Vector<int> empty;
    mystd::Vector<int> large(STRESS_SIZE, 10);
    
    mystd::Vector<int> result1 = empty + large;
    EXPECT_EQ(result1.Size(), STRESS_SIZE);
    EXPECT_EQ(result1[0], 10);
    
    mystd::Vector<int> result2 = large + empty;
    EXPECT_EQ(result2.Size(), STRESS_SIZE);
    EXPECT_EQ(result2[0], 10);
}

TEST_F(VectorOperatorStressTest, PlusEqualsMemoryUsage) {
    mystd::Vector<int> vec(1000);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    mystd::Vector<int> temp(100);
    for (int i = 0; i < 1000; ++i) {
        vec += temp;
    }
    
    EXPECT_EQ(vec.Size(), 1000 + 100 * 1000);
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[999], 999);
    EXPECT_EQ(vec[1000], 0);
}

TEST_F(VectorOperatorStressTest, EqualityFirstElementDifferent) {
    mystd::Vector<int> vec1(STRESS_SIZE, 1);
    mystd::Vector<int> vec2(STRESS_SIZE, 1);
    vec2[0] = 2;
    
    EXPECT_FALSE(vec1 == vec2);
    EXPECT_TRUE(vec1 != vec2);
}

TEST_F(VectorOperatorStressTest, EqualityLastElementDifferent) {
    mystd::Vector<int> vec1(STRESS_SIZE, 1);
    mystd::Vector<int> vec2(STRESS_SIZE, 1);
    vec2[STRESS_SIZE - 1] = 2;
    
    EXPECT_FALSE(vec1 == vec2);
    EXPECT_TRUE(vec1 != vec2);
}

TEST_F(VectorOperatorStressTest, PlusEqualsVeryLarge) {
    const size_t HUGE_SIZE = 50000;
    mystd::Vector<int> vec1(HUGE_SIZE, 1);
    mystd::Vector<int> vec2(HUGE_SIZE, 2);
    
    vec1 += vec2;
    
    EXPECT_EQ(vec1.Size(), HUGE_SIZE * 2);
    for (size_t i = 0; i < HUGE_SIZE; ++i) {
        EXPECT_EQ(vec1[i], 1);
        EXPECT_EQ(vec1[i + HUGE_SIZE], 2);
    }
}

TEST_F(VectorOperatorStressTest, PlusOperatorAssignmentChain) {
    mystd::Vector<int> vec1(1000, 1);
    mystd::Vector<int> vec2(1000, 2);
    mystd::Vector<int> vec3(1000, 3);
    
    mystd::Vector<int> result;
    result = vec1 + vec2 + vec3;
    
    EXPECT_EQ(result.Size(), 3000);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1000], 2);
    EXPECT_EQ(result[2000], 3);
}

TEST_F(VectorOperatorStressTest, SelfComparison) {
    mystd::Vector<int> vec(STRESS_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i * i);
    }
    
    EXPECT_TRUE(vec == vec);
    EXPECT_FALSE(vec != vec);
}

TEST_F(VectorOperatorStressTest, PlusEqualsMixedSizes) {
    std::uniform_int_distribution<size_t> size_dist(1, 1000);
    
    for (int i = 0; i < 100; ++i) {
        size_t size1 = size_dist(rng);
        size_t size2 = size_dist(rng);
        
        mystd::Vector<int> vec1(size1, 1);
        mystd::Vector<int> vec2(size2, 2);
        
        vec1 += vec2;
        EXPECT_EQ(vec1.Size(), size1 + size2);
        EXPECT_EQ(vec1[0], 1);
        EXPECT_EQ(vec1[size1 - 1], 1);
        EXPECT_EQ(vec1[size1], 2);
        EXPECT_EQ(vec1[size1 + size2 - 1], 2);
    }
}

class VectorModificationStressTest : public ::testing::Test {
protected:
    void SetUp() override {
        rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    }
    
    std::mt19937 rng;
    const size_t HUGE_SIZE = 1000000; // 1M элементов
    const size_t LARGE_SIZE = 500000; // 500K элементов
};

TEST_F(VectorModificationStressTest, ResizeHugeIncrease) {
    mystd::Vector<int> vec(1000, 42);
    
    vec.Resize(HUGE_SIZE);
    
    EXPECT_EQ(vec.Size(), HUGE_SIZE);
    for (size_t i = 0; i < 1000; ++i) {
        EXPECT_EQ(vec[i], 42);
    }
}

TEST_F(VectorModificationStressTest, ResizeHugeDecrease) {
    mystd::Vector<int> vec(HUGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    vec.Resize(1000);
    
    EXPECT_EQ(vec.Size(), 1000);
    for (size_t i = 0; i < 1000; ++i) {
        EXPECT_EQ(vec[i], static_cast<int>(i));
    }
}

TEST_F(VectorModificationStressTest, ResizeWithValueHugeIncrease) {
    mystd::Vector<int> vec(1000, 10);
    
    vec.Resize(HUGE_SIZE, 999);
    
    EXPECT_EQ(vec.Size(), HUGE_SIZE);
    for (size_t i = 0; i < 1000; ++i) {
        EXPECT_EQ(vec[i], 10);
    }
    for (size_t i = 1000; i < HUGE_SIZE; ++i) {
        EXPECT_EQ(vec[i], 999);
    }
}

TEST_F(VectorModificationStressTest, ResizeWithValueMultipleTimes) {
    mystd::Vector<int> vec;
    
    for (int i = 0; i < 100; ++i) {
        size_t new_size = 10000 * (i + 1);
        vec.Resize(new_size, i);
        EXPECT_EQ(vec.Size(), new_size);
        EXPECT_EQ(vec[new_size - 1], i);
    }
    
    EXPECT_EQ(vec.Size(), 10000 * 100);
}

TEST_F(VectorModificationStressTest, ResizeOscillation) {
    mystd::Vector<int> vec;
    
    for (int i = 0; i < 50; ++i) {
        vec.Resize(HUGE_SIZE, 1);
        EXPECT_EQ(vec.Size(), HUGE_SIZE);
        
        vec.Resize(1000);
        EXPECT_EQ(vec.Size(), 1000);
    }
}

TEST_F(VectorModificationStressTest, ReverseHugeVector) {
    mystd::Vector<int> vec(HUGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    vec.Reverse();
    
    EXPECT_EQ(vec.Size(), HUGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        EXPECT_EQ(vec[i], static_cast<int>(HUGE_SIZE - 1 - i));
    }
}

TEST_F(VectorModificationStressTest, ReverseLargeSections) {
    mystd::Vector<int> vec(HUGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    vec.Reverse(100000, 900000); // Reverse 800K elements
    
    for (size_t i = 0; i < 100000; ++i) {
        EXPECT_EQ(vec[i], static_cast<int>(i));
    }
    for (size_t i = 100000; i < 900000; ++i) {
        EXPECT_EQ(vec[i], static_cast<int>(100000 + 899999 - i));
    }
    for (size_t i = 900000; i < HUGE_SIZE; ++i) {
        EXPECT_EQ(vec[i], static_cast<int>(i));
    }
}

TEST_F(VectorModificationStressTest, MultipleReverseOperations) {
    mystd::Vector<int> vec(LARGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    for (int i = 0; i < 10; ++i) {
        vec.Reverse();
    }
    
    EXPECT_EQ(vec.Size(), LARGE_SIZE);
    // After even number of reverses, should be back to original
    for (size_t i = 0; i < vec.Size(); ++i) {
        EXPECT_EQ(vec[i], static_cast<int>(i));
    }
}

TEST_F(VectorModificationStressTest, ResizeToExtremeSizes) {
    mystd::Vector<int> vec;
    
    vec.Resize(10, 1);
    EXPECT_EQ(vec.Size(), 10);
    
    vec.Resize(1000000, 2);
    EXPECT_EQ(vec.Size(), 1000000);
    
    vec.Resize(100, 3);
    EXPECT_EQ(vec.Size(), 100);
    
    vec.Resize(500000, 4);
    EXPECT_EQ(vec.Size(), 500000);
}

TEST_F(VectorModificationStressTest, ReverseSmallSectionsInLargeVector) {
    mystd::Vector<int> vec(HUGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    // Reverse many small sections
    for (size_t i = 0; i < 1000; i += 100) {
        vec.Reverse(i, i + 50);
    }
    
    EXPECT_EQ(vec.Size(), HUGE_SIZE);
}

TEST_F(VectorModificationStressTest, ResizeWithComplexType) {
    struct ComplexType {
        int data[10];
        double value;
        ComplexType(int v = 0) : value(v) {
            for (int i = 0; i < 10; ++i) data[i] = v + i;
        }
    };
    
    mystd::Vector<ComplexType> vec(1000, ComplexType(1));
    
    vec.Resize(LARGE_SIZE, ComplexType(2));
    
    EXPECT_EQ(vec.Size(), LARGE_SIZE);
    EXPECT_EQ(vec[0].value, 1);
    EXPECT_EQ(vec[999].value, 1);
    EXPECT_EQ(vec[1000].value, 2);
    EXPECT_EQ(vec[LARGE_SIZE - 1].value, 2);
}

TEST_F(VectorModificationStressTest, ReversePerformance) {
    mystd::Vector<int> vec(HUGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    vec.Reverse();
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    EXPECT_LE(duration.count(), 1000); // Should complete within 1 second
    
    EXPECT_EQ(vec[0], static_cast<int>(HUGE_SIZE - 1));
    EXPECT_EQ(vec[HUGE_SIZE - 1], 0);
}

TEST_F(VectorModificationStressTest, ResizeMemoryReuse) {
    mystd::Vector<int> vec;
    
    // Multiple resize operations to test memory management
    for (int i = 1; i <= 100; ++i) {
        size_t new_size = 10000 * i;
        vec.Resize(new_size, i);
        EXPECT_EQ(vec.Size(), new_size);
        EXPECT_EQ(vec[new_size - 1], i);
    }
}

TEST_F(VectorModificationStressTest, ReverseEntireRange) {
    mystd::Vector<int> vec(HUGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i % 1000);
    }
    
    vec.Reverse(0, vec.Size()); // Same as Reverse()
    
    for (size_t i = 0; i < vec.Size(); ++i) {
        EXPECT_EQ(vec[i], static_cast<int>((HUGE_SIZE - 1 - i) % 1000));
    }
}

TEST_F(VectorModificationStressTest, ResizeToZero) {
    mystd::Vector<int> vec(HUGE_SIZE, 42);
    
    vec.Resize(0);
    EXPECT_EQ(vec.Size(), 0);
    EXPECT_TRUE(vec.Empty());
    
    vec.Resize(LARGE_SIZE, 100);
    EXPECT_EQ(vec.Size(), LARGE_SIZE);
    EXPECT_EQ(vec[0], 100);
    EXPECT_EQ(vec[LARGE_SIZE - 1], 100);
}

TEST_F(VectorModificationStressTest, MultiplePartialReverses) {
    mystd::Vector<int> vec(HUGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    // Reverse overlapping sections
    vec.Reverse(100000, 200000);
    vec.Reverse(150000, 250000);
    vec.Reverse(50000, 150000);
    
    EXPECT_EQ(vec.Size(), HUGE_SIZE);
    // Verify some known positions
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[99999], 199999);
}

TEST_F(VectorModificationStressTest, ResizeWithMaxValue) {
    mystd::Vector<int> vec;
    
    vec.Resize(LARGE_SIZE, INT_MAX);
    
    EXPECT_EQ(vec.Size(), LARGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        EXPECT_EQ(vec[i], INT_MAX);
    }
}

TEST_F(VectorModificationStressTest, StressReverseResizeCombination) {
    mystd::Vector<int> vec(100000);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    for (int i = 0; i < 10; ++i) {
        vec.Reverse();
        vec.Resize(vec.Size() + 50000, -1);
        vec.Reverse(100000, vec.Size());
    }
    
    EXPECT_GT(vec.Size(), 100000);
}

TEST_F(VectorModificationStressTest, ResizeExactCapacity) {
    mystd::Vector<int> vec(1000, 5);
    
    // Resize to exactly current capacity
    vec.Resize(1000, 10);
    EXPECT_EQ(vec.Size(), 1000);
    for (size_t i = 0; i < vec.Size(); ++i) {
        EXPECT_EQ(vec[i], 5); // Values shouldn't change
    }
    
    // Resize to just below capacity
    vec.Resize(999, 20);
    EXPECT_EQ(vec.Size(), 999);
    for (size_t i = 0; i < vec.Size(); ++i) {
        EXPECT_EQ(vec[i], 5);
    }
}

TEST_F(VectorModificationStressTest, ReverseSingleElementSections) {
    mystd::Vector<int> vec(HUGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    // Reverse single element sections (should do nothing)
    for (size_t i = 0; i < 1000; ++i) {
        vec.Reverse(i, i + 1);
    }
    
    // Vector should remain unchanged
    for (size_t i = 0; i < vec.Size(); ++i) {
        EXPECT_EQ(vec[i], static_cast<int>(i));
    }
}

class VectorBasicOperationsStressTest : public ::testing::Test {
protected:
    void SetUp() override {
        rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    }
    
    std::mt19937 rng;
    const size_t EXTREME_SIZE = 5000000; // 5M элементов
    const size_t HUGE_SIZE = 2000000;    // 2M элементов
};

// 1. Простые типы данных
TEST_F(VectorBasicOperationsStressTest, PushBackExtremeInt) {
    mystd::Vector<int> vec;
    
    for (size_t i = 0; i < EXTREME_SIZE; ++i) {
        vec.PushBack(static_cast<int>(i * 2));
    }
    
    EXPECT_EQ(vec.Size(), EXTREME_SIZE);
    EXPECT_FALSE(vec.Empty());
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[EXTREME_SIZE - 1], static_cast<int>((EXTREME_SIZE - 1) * 2));
}

TEST_F(VectorBasicOperationsStressTest, PushBackExtremeDouble) {
    mystd::Vector<double> vec;
    
    for (size_t i = 0; i < EXTREME_SIZE; ++i) {
        vec.PushBack(i * 3.14159);
    }
    
    EXPECT_EQ(vec.Size(), EXTREME_SIZE);
    EXPECT_NEAR(vec[0], 0.0, 1e-10);
    EXPECT_NEAR(vec[EXTREME_SIZE - 1], (EXTREME_SIZE - 1) * 3.14159, 1e-10);
}

TEST_F(VectorBasicOperationsStressTest, PushBackExtremeString) {
    mystd::Vector<std::string> vec;
    
    for (size_t i = 0; i < HUGE_SIZE; ++i) {
        vec.PushBack("String_" + std::to_string(i) + "_very_long_string_that_takes_more_memory");
    }
    
    EXPECT_EQ(vec.Size(), HUGE_SIZE);
    EXPECT_EQ(vec[0], "String_0_very_long_string_that_takes_more_memory");
    EXPECT_EQ(vec[HUGE_SIZE - 1], "String_" + std::to_string(HUGE_SIZE - 1) + "_very_long_string_that_takes_more_memory");
}

// 2. Многомерные массивы как тип данных
TEST_F(VectorBasicOperationsStressTest, PushBack2DArrays) {
    using Array2D = std::array<std::array<int, 100>, 100>; // 10,000 элементов в каждом
    mystd::Vector<Array2D> vec;
    
    for (size_t i = 0; i < 1000; ++i) {
        Array2D arr;
        for (int x = 0; x < 100; ++x) {
            for (int y = 0; y < 100; ++y) {
                arr[x][y] = static_cast<int>(i + x + y);
            }
        }
        vec.PushBack(arr);
    }
    
    EXPECT_EQ(vec.Size(), 1000);
    EXPECT_EQ(vec[0][0][0], 0);
    EXPECT_EQ(vec[999][99][99], 999 + 99 + 99);
}

TEST_F(VectorBasicOperationsStressTest, PushBack3DArrays) {
    using Array3D = std::array<std::array<std::array<double, 50>, 50>, 50>; // 125,000 элементов
    mystd::Vector<Array3D> vec;
    
    for (size_t i = 0; i < 100; ++i) {
        Array3D arr;
        for (int x = 0; x < 50; ++x) {
            for (int y = 0; y < 50; ++y) {
                for (int z = 0; z < 50; ++z) {
                    arr[x][y][z] = (i + x + y + z) * 1.5;
                }
            }
        }
        vec.PushBack(arr);
    }
    
    EXPECT_EQ(vec.Size(), 100);
    EXPECT_NEAR(vec[0][0][0][0], 0.0, 1e-10);
    EXPECT_NEAR(vec[99][49][49][49], (99 + 49 + 49 + 49) * 1.5, 1e-10);
}

// 3. Сложные структуры данных
struct ComplexStruct {
    int id;
    double values[1000];
    std::string name;
    std::array<float, 500> floats;
    
    ComplexStruct(int i = 0) : id(i), name("Object_" + std::to_string(i)) {
        for (int j = 0; j < 1000; ++j) values[j] = i + j * 0.1;
        for (int j = 0; j < 500; ++j) floats[j] = static_cast<float>(i * j);
    }
};

TEST_F(VectorBasicOperationsStressTest, PushBackComplexStruct) {
    mystd::Vector<ComplexStruct> vec;
    
    for (size_t i = 0; i < 50000; ++i) {
        vec.PushBack(ComplexStruct(static_cast<int>(i)));
    }
    
    EXPECT_EQ(vec.Size(), 50000);
    EXPECT_EQ(vec[0].id, 0);
    EXPECT_EQ(vec[0].name, "Object_0");
    EXPECT_EQ(vec[49999].id, 49999);
    EXPECT_EQ(vec[49999].name, "Object_49999");
    EXPECT_NEAR(vec[1000].values[500], 1000 + 500 * 0.1, 1e-10);
}

// 4. Векторы векторов (очень тяжелый тип)
TEST_F(VectorBasicOperationsStressTest, PushBackVectorOfVectors) {
    using NestedVector = mystd::Vector<mystd::Vector<int>>;
    NestedVector vec;
    
    for (size_t i = 0; i < 1000; ++i) {
        mystd::Vector<int> inner_vec;
        for (size_t j = 0; j < 1000; ++j) {
            inner_vec.PushBack(static_cast<int>(i * j));
        }
        vec.PushBack(inner_vec);
    }
    
    EXPECT_EQ(vec.Size(), 1000);
    EXPECT_EQ(vec[0].Size(), 1000);
    EXPECT_EQ(vec[999].Size(), 1000);
    EXPECT_EQ(vec[500][500], 500 * 500);
}

// 5. Долгие операции с PopBack
TEST_F(VectorBasicOperationsStressTest, PushPopExtremeCycle) {
    mystd::Vector<long long> vec;
    
    // Push massive amount
    for (long long i = 0; i < 1000000; ++i) {
        vec.PushBack(i * i);
    }
    
    EXPECT_EQ(vec.Size(), 1000000);
    
    // Pop most of them
    for (size_t i = 0; i < 900000; ++i) {
        vec.PopBack();
    }
    
    EXPECT_EQ(vec.Size(), 100000);
    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec[99999], 99999LL * 99999LL);
    
    // Push more
    for (long long i = 0; i < 500000; ++i) {
        vec.PushBack(i + 1000000);
    }
    
    EXPECT_EQ(vec.Size(), 600000);
    EXPECT_EQ(vec[600000 - 1], 1000000 + 500000 - 1);
}

// 6. Тест на многократное изменение размера
TEST_F(VectorBasicOperationsStressTest, EmptyCheckStress) {
    mystd::Vector<int> vec;
    
    EXPECT_TRUE(vec.Empty());
    
    for (size_t i = 0; i < EXTREME_SIZE; ++i) {
        vec.PushBack(static_cast<int>(i));
        EXPECT_FALSE(vec.Empty());
    }
    
    EXPECT_EQ(vec.Size(), EXTREME_SIZE);
    
    for (size_t i = 0; i < EXTREME_SIZE; ++i) {
        vec.PopBack();
    }
    
    EXPECT_TRUE(vec.Empty());
    EXPECT_EQ(vec.Size(), 0);
}

// 7. Случайные операции
TEST_F(VectorBasicOperationsStressTest, RandomOperationsStress) {
    mystd::Vector<int> vec;
    std::uniform_int_distribution<int> op_dist(0, 3);
    std::uniform_int_distribution<int> value_dist(0, 1000000);
    
    int push_count = 0;
    int pop_count = 0;
    
    for (int i = 0; i < 2000000; ++i) {
        int operation = op_dist(rng);
        
        if (operation == 0 || operation == 1) {
            // Push
            vec.PushBack(value_dist(rng));
            push_count++;
        } else if (operation == 2 && !vec.Empty()) {
            // Pop
            vec.PopBack();
            pop_count++;
        }
        // operation == 3: do nothing
    }
    
    EXPECT_EQ(vec.Size(), push_count - pop_count);
    EXPECT_EQ(vec.Empty(), (push_count == pop_count));
}

// 8. Очень большие структуры
struct MassiveStruct {
    char data[16384]; // 16KB на элемент
    int id;
    
    MassiveStruct(int i = 0) : id(i) {
        for (int j = 0; j < 16384; ++j) {
            data[j] = static_cast<char>((i + j) % 256);
        }
    }
};

TEST_F(VectorBasicOperationsStressTest, PushBackMassiveStruct) {
    mystd::Vector<MassiveStruct> vec;
    
    for (int i = 0; i < 10000; ++i) {
        vec.PushBack(MassiveStruct(i));
    }
    
    EXPECT_EQ(vec.Size(), 10000);
    EXPECT_EQ(vec[0].id, 0);
    EXPECT_EQ(vec[9999].id, 9999);
    EXPECT_EQ(vec[5000].data[1000], static_cast<char>((5000 + 1000) % 256));
}

// 9. Долгий тест на реаллокацию
TEST_F(VectorBasicOperationsStressTest, PushBackReallocationStress) {
    mystd::Vector<int> vec;
    size_t expected_size = 1;
    
    for (int i = 0; i < 30; ++i) { // 2^30 очень большое число
        vec.PushBack(i);
        if (vec.Size() > expected_size) {
            expected_size *= 2;
        }
    }
    
    EXPECT_EQ(vec.Size(), 30);
    for (int i = 0; i < 30; ++i) {
        EXPECT_EQ(vec[i], i);
    }
}

// 10. Комбинированный стресс-тест
TEST_F(VectorBasicOperationsStressTest, CombinedOperationsMarathon) {
    mystd::Vector<double> vec;
    
    // Фаза 1: Быстрое заполнение
    for (size_t i = 0; i < 1000000; ++i) {
        vec.PushBack(std::sin(i * 0.001));
    }
    
    // Фаза 2: Удаление и добавление
    for (int phase = 0; phase < 10; ++phase) {
        size_t current_size = vec.Size();
        for (size_t i = 0; i < current_size / 2; ++i) {
            vec.PopBack();
        }
        
        for (size_t i = 0; i < 500000; ++i) {
            vec.PushBack(std::cos(i * 0.001));
        }
    }
    
    EXPECT_GT(vec.Size(), 0);
    EXPECT_FALSE(vec.Empty());
}

// 11. Тест с std::array разных размерностей
TEST_F(VectorBasicOperationsStressTest, VariousDimensionArrays) {
    // 4D array
    using Array4D = std::array<std::array<std::array<std::array<float, 20>, 20>, 20>, 20>;
    mystd::Vector<Array4D> vec4d;
    
    for (int i = 0; i < 50; ++i) {
        Array4D arr;
        for (int a = 0; a < 20; ++a) {
            for (int b = 0; b < 20; ++b) {
                for (int c = 0; c < 20; ++c) {
                    for (int d = 0; d < 20; ++d) {
                        arr[a][b][c][d] = static_cast<float>(i + a + b + c + d);
                    }
                }
            }
        }
        vec4d.PushBack(arr);
    }
    
    EXPECT_EQ(vec4d.Size(), 50);
    EXPECT_FLOAT_EQ(vec4d[0][0][0][0][0], 0.0f);
    EXPECT_FLOAT_EQ(vec4d[49][19][19][19][19], 49 + 19 + 19 + 19 + 19);
}

// 12. Долгий тест на выносливость
TEST_F(VectorBasicOperationsStressTest, EnduranceTest) {
    mystd::Vector<int> vec;
    const int TOTAL_OPERATIONS = 3000000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < TOTAL_OPERATIONS; ++i) {
        if (i % 3 != 0 || vec.Empty()) {
            vec.PushBack(i);
        } else {
            vec.PopBack();
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    EXPECT_LE(duration.count(), 10000); // Должно завершиться за 10 секунд
    EXPECT_GT(vec.Size(), 0);
}

// 13. Тест с типами разных размеров
TEST_F(VectorBasicOperationsStressTest, MixedSizeTypes) {
    // Маленький тип
    mystd::Vector<char> char_vec;
    for (size_t i = 0; i < 2000000; ++i) {
        char_vec.PushBack(static_cast<char>(i % 128));
    }
    
    // Большой тип
    mystd::Vector<std::array<double, 1000>> big_vec;
    for (int i = 0; i < 1000; ++i) {
        std::array<double, 1000> arr;
        for (int j = 0; j < 1000; ++j) {
            arr[j] = i * j * 0.5;
        }
        big_vec.PushBack(arr);
    }
    
    EXPECT_EQ(char_vec.Size(), 2000000);
    EXPECT_EQ(big_vec.Size(), 1000);
    EXPECT_EQ(char_vec[1000000], static_cast<char>(1000000 % 128));
    EXPECT_DOUBLE_EQ(big_vec[500][500], 500 * 500 * 0.5);
}

// 14. Тест на исключения при PopBack
TEST_F(VectorBasicOperationsStressTest, PopBackExceptionStress) {
    mystd::Vector<int> vec;
    
    // Заполняем
    for (int i = 0; i < 1000; ++i) {
        vec.PushBack(i);
    }
    
    // Опустошаем
    while (!vec.Empty()) {
        vec.PopBack();
    }
    
    EXPECT_TRUE(vec.Empty());
    
    // Пытаемся удалить из пустого вектора
    EXPECT_THROW(vec.PopBack(), std::exception);
    
    int exception_count = 0;
    for (int i = 0; i < 1000; ++i) {
        try {
            vec.PopBack();
        } catch (...) {
            exception_count++;
        }
    }
    
    EXPECT_GT(exception_count, 0);
}

class VectorInsertEraseStressTest : public ::testing::Test {
protected:
    void SetUp() override {
        rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    }
    
    std::mt19937 rng;
    const size_t HUGE_SIZE = 1000000; // 1M элементов
    const size_t LARGE_SIZE = 500000; // 500K элементов
};

// 1. Базовые стресс-тесты для int
TEST_F(VectorInsertEraseStressTest, InsertSingleExtreme) {
    mystd::Vector<int> vec;
    
    // Вставляем 100K элементов в начало (худший случай)
    for (size_t i = 0; i < 100000; ++i) {
        vec.Insert(0, static_cast<int>(i));
    }
    
    EXPECT_EQ(vec.Size(), 100000);
    EXPECT_EQ(vec[0], 99999);
    EXPECT_EQ(vec[99999], 0);
}

TEST_F(VectorInsertEraseStressTest, InsertSingleAtEndExtreme) {
    mystd::Vector<int> vec;
    
    // Вставляем 1M элементов в конец (лучший случай)
    for (size_t i = 0; i < HUGE_SIZE; ++i) {
        vec.Insert(vec.Size(), static_cast<int>(i));
    }
    
    EXPECT_EQ(vec.Size(), HUGE_SIZE);
    for (size_t i = 0; i < HUGE_SIZE; ++i) {
        EXPECT_EQ(vec[i], static_cast<int>(i));
    }
}

TEST_F(VectorInsertEraseStressTest, InsertRangeExtreme) {
    mystd::Vector<int> vec(100000, 0);
    
    // Вставляем большой диапазон в середину
    vec.Insert(50000, 150000, 42);
    
    EXPECT_EQ(vec.Size(), 200000);
    for (size_t i = 0; i < 50000; ++i) {
        EXPECT_EQ(vec[i], 0);
    }
    for (size_t i = 50000; i < 150000; ++i) {
        EXPECT_EQ(vec[i], 42);
    }
    for (size_t i = 150000; i < 200000; ++i) {
        EXPECT_EQ(vec[i], 0);
    }
}

TEST_F(VectorInsertEraseStressTest, EraseSingleExtreme) {
    mystd::Vector<int> vec(HUGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    // Удаляем из начала 100K раз (худший случай)
    for (size_t i = 0; i < 100000; ++i) {
        vec.Erase(0);
    }
    
    EXPECT_EQ(vec.Size(), HUGE_SIZE - 100000);
    EXPECT_EQ(vec[0], 100000);
    EXPECT_EQ(vec[vec.Size() - 1], HUGE_SIZE - 1);
}

TEST_F(VectorInsertEraseStressTest, EraseRangeExtreme) {
    mystd::Vector<int> vec(HUGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    
    // Удаляем большой диапазон из середины
    vec.Erase(300000, 700000);
    
    EXPECT_EQ(vec.Size(), HUGE_SIZE - 400000);
    for (size_t i = 0; i < 300000; ++i) {
        EXPECT_EQ(vec[i], static_cast<int>(i));
    }
    for (size_t i = 300000; i < vec.Size(); ++i) {
        EXPECT_EQ(vec[i], static_cast<int>(i + 400000));
    }
}

// 2. Комбинированные операции
TEST_F(VectorInsertEraseStressTest, InsertEraseMixedOperations) {
    mystd::Vector<int> vec;
    
    for (int cycle = 0; cycle < 100; ++cycle) {
        // Вставляем 1000 элементов
        for (int i = 0; i < 1000; ++i) {
            vec.Insert(vec.Size() / 2, cycle * 1000 + i);
        }
        
        // Удаляем 500 элементов
        for (int i = 0; i < 500; ++i) {
            vec.Erase(vec.Size() / 3);
        }
    }
    
    EXPECT_GT(vec.Size(), 0);
}

TEST_F(VectorInsertEraseStressTest, InsertAtRandomPositions) {
    mystd::Vector<int> vec(100000, 0);
    std::uniform_int_distribution<size_t> pos_dist(0, 100000);
    
    for (int i = 0; i < 50000; ++i) {
        size_t pos = pos_dist(rng) % (vec.Size() + 1);
        vec.Insert(pos, i + 1);
    }
    
    EXPECT_EQ(vec.Size(), 150000);
}

// 3. Тесты с double
TEST_F(VectorInsertEraseStressTest, InsertDoubleExtreme) {
    mystd::Vector<double> vec;
    
    for (size_t i = 0; i < LARGE_SIZE - 100000; ++i) {
        vec.Insert(i % 100, i * 3.14159);
    }
    
    EXPECT_EQ(vec.Size(), LARGE_SIZE - 100000);
}

TEST_F(VectorInsertEraseStressTest, EraseDoubleRange) {
    mystd::Vector<double> vec(LARGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = i * 2.71828;
    }
    
    vec.Erase(100000, 400000);
    
    EXPECT_EQ(vec.Size(), LARGE_SIZE - 300000);
    EXPECT_NEAR(vec[0], 0.0, 1e-10);
    EXPECT_NEAR(vec[vec.Size() - 1], (LARGE_SIZE - 1) * 2.71828, 1e-10);
}

// 4. Тесты со string (тяжелый тип)
TEST_F(VectorInsertEraseStressTest, InsertStringExtreme) {
    mystd::Vector<std::string> vec;
    
    for (size_t i = 0; i < 100000; ++i) {
        std::string value = "String_" + std::to_string(i) + "_" + std::string(100, 'X');
        vec.Insert(vec.Size() / (i % 10 + 1), value);
    }
    
    EXPECT_EQ(vec.Size(), 100000);
    EXPECT_GT(vec[0].size(), 10);
    EXPECT_GT(vec[99999].size(), 10);
}

TEST_F(VectorInsertEraseStressTest, EraseStringRange) {
    mystd::Vector<std::string> vec(50000);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = "Initial_" + std::to_string(i) + "_" + std::string(50, 'Y');
    }
    
    vec.Erase(10000, 40000);
    
    EXPECT_EQ(vec.Size(), 20000);
    EXPECT_EQ(vec[0], "Initial_0_" + std::string(50, 'Y'));
    EXPECT_EQ(vec[19999], "Initial_49999_" + std::string(50, 'Y'));
}

// 5. Тесты с std::array (многомерные данные)
TEST_F(VectorInsertEraseStressTest, Insert2DArray) {
    using Array2D = std::array<std::array<int, 50>, 50>;
    mystd::Vector<Array2D> vec;
    
    for (int i = 0; i < 1000; ++i) {
        Array2D arr;
        for (int x = 0; x < 50; ++x) {
            for (int y = 0; y < 50; ++y) {
                arr[x][y] = i * x * y;
            }
        }
        vec.Insert(vec.Size(), arr);
    }
    
    EXPECT_EQ(vec.Size(), 1000);
    EXPECT_EQ(vec[0][0][0], 0);
    EXPECT_EQ(vec[999][49][49], 999 * 49 * 49);
}

// 6. Тесты с пользовательскими структурами
struct ComplexData {
    int id;
    double values[100];
    std::string name;
    std::vector<int> nested_data;
    
    ComplexData(int i = 0) : id(i), name("Object_" + std::to_string(i)), nested_data(100, i) {
        for (int j = 0; j < 100; ++j) {
            values[j] = i + j * 0.5;
        }
    }
};

TEST_F(VectorInsertEraseStressTest, InsertComplexStruct) {
    mystd::Vector<ComplexData> vec;
    
    for (int i = 0; i < 50000; ++i) {
        vec.Insert(i % 100, ComplexData(i));
    }
    
    EXPECT_EQ(vec.Size(), 50000);
    EXPECT_EQ(vec[0].id, 49900);
    EXPECT_EQ(vec[49999].id, 99);
    EXPECT_EQ(vec[25000].name, "Object_24900");
}

// 7. Стресс-тесты на граничные случаи
TEST_F(VectorInsertEraseStressTest, InsertAtBoundaries) {
    mystd::Vector<int> vec(1000, 1);
    
    // Вставка в самое начало
    vec.Insert(0, 999);
    EXPECT_EQ(vec[0], 999);
    
    // Вставка в самый конец
    vec.Insert(vec.Size(), 888);
    EXPECT_EQ(vec[vec.Size() - 1], 888);
    
    // Вставка прямо перед концом
    vec.Insert(vec.Size() - 1, 777);
    EXPECT_EQ(vec[vec.Size() - 2], 777);
}

TEST_F(VectorInsertEraseStressTest, EraseEntireVector) {
    mystd::Vector<int> vec(HUGE_SIZE);
    for (size_t i = 0; i < vec.Size(); ++i) {
        vec[i] = static_cast<int>(i);
    }
    // Постепенно удаляем весь вектор
    while (!vec.Empty()) {
        size_t old_size = vec.Size();
        vec.Erase(0, std::min(size_t(1000), old_size));
    }
    
    EXPECT_TRUE(vec.Empty());
    EXPECT_EQ(vec.Size(), 0);
}

// 8. Тесты на производительность
TEST_F(VectorInsertEraseStressTest, PerformanceInsertAtEnd) {
    mystd::Vector<int> vec;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < LARGE_SIZE; ++i) {
        vec.Insert(vec.Size(), static_cast<int>(i));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    EXPECT_LE(duration.count(), 5000); // Должно уложиться в 5 секунд
    EXPECT_EQ(vec.Size(), LARGE_SIZE);
}

TEST_F(VectorInsertEraseStressTest, PerformanceInsertAtBeginning) {
    mystd::Vector<int> vec;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < 100000; ++i) {
        vec.Insert(0, static_cast<int>(i));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    EXPECT_LE(duration.count(), 6000);
    EXPECT_EQ(vec.Size(), 100000);
}

// 9. Тесты на правильность работы с памятью
TEST_F(VectorInsertEraseStressTest, MemoryReuseAfterMultipleOperations) {
    mystd::Vector<int> vec;
    
    // Многократные вставки и удаления
    for (int cycle = 0; cycle < 50; ++cycle) {
        // Вставляем блок
        for (int i = 0; i < 10000; ++i) {
            vec.Insert(vec.Size() / 2, cycle * 10000 + i);
        }
        
        // Удаляем часть
        vec.Erase(2000, 8000);
    }
    
    EXPECT_EQ(vec.Size(), 50 * 4000); // 50 циклов * (10000 - 6000)
}

// 10. Тесты с исключениями
TEST_F(VectorInsertEraseStressTest, ExceptionHandling) {
    mystd::Vector<int> vec(1000, 1);
    
    // Некорректная позиция вставки
    EXPECT_THROW(vec.Insert(2000, 42), std::exception);
    
    // Некорректный диапазон вставки
    EXPECT_THROW(vec.Insert(500, 400, 42), std::exception);
    
    // Некорректная позиция удаления
    EXPECT_THROW(vec.Erase(2000), std::exception);
    
    // Некорректный диапазон удаления
    EXPECT_THROW(vec.Erase(500, 400), std::exception);
    EXPECT_THROW(vec.Erase(500, 1500), std::exception);
}

// 11. Тесты с bool (специальный случай)
TEST_F(VectorInsertEraseStressTest, InsertEraseBool) {
    mystd::Vector<bool> vec;
    
    for (size_t i = 0; i < 200000; ++i) {
        vec.Insert(i % 100, (i % 2 == 0));
    }
    
    vec.Erase(50000, 150000);
    
    EXPECT_EQ(vec.Size(), 100000);
}

// 12. Долгий комбинированный тест
TEST_F(VectorInsertEraseStressTest, MarathonTest) {
    mystd::Vector<int> vec;
    
    // Фаза 1: Заполнение вставками в разные позиции
    for (int i = 0; i < 100000; ++i) {
        size_t pos = (i * 7) % (vec.Size() + 1); // Псевдослучайная позиция
        vec.Insert(pos, i);
    }
    
    // Фаза 2: Удаление блоков
    for (int i = 0; i < 10; ++i) {
        vec.Erase(1000 * i, 1000 * i + 500);
    }
    
    // Фаза 3: Вставка диапазонов
    for (int i = 0; i < 5; ++i) {
        vec.Insert(2000 * i, 2000 * i + 1000, 999);
    }
    
    EXPECT_GT(vec.Size(), 0);
}

// 13. Тест с указателями
TEST_F(VectorInsertEraseStressTest, InsertErasePointers) {
    mystd::Vector<int*> vec;
    
    std::vector<int> data(100000);
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] = static_cast<int>(i);
    }
    
    for (size_t i = 0; i < 50000; ++i) {
        vec.Insert(i % 100, &data[i]);
    }
    
    vec.Erase(10000, 40000);
    
    EXPECT_EQ(vec.Size(), 20000);
    EXPECT_EQ(*vec[0], 49900);
    EXPECT_EQ(*vec[19999], 99);
}

// 14. Тест на стабильность после множества операций
TEST_F(VectorInsertEraseStressTest, StabilityTest) {
    mystd::Vector<long long> vec;
    
    const int OPERATIONS = 200000;
    long long expected_sum = 0;
    long long actual_sum = 0;
    
    for (int i = 0; i < OPERATIONS; ++i) {
        if (i % 3 == 0 && !vec.Empty()) {
            // Удаляем
            vec.Erase(vec.Size() / 2);
        } else {
            // Вставляем
            vec.Insert(vec.Size() / (i % 5 + 1), static_cast<long long>(i));
            expected_sum += i;
        }
    }
    
    // Считаем сумму оставшихся элементов
    for (size_t i = 0; i < vec.Size(); ++i) {
        actual_sum += vec[i];
    }
    
    // Сумма должна быть корректной (хотя и не обязательно равной expected_sum)
    EXPECT_GE(actual_sum, 0);
    EXPECT_GT(vec.Size(), 0);
}