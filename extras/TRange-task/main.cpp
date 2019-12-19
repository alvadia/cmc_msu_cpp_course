#include <iostream>
#include <unordered_set>
#include <gtest/gtest.h>

#include "TRange.cpp" 

TEST(ExceptionsSet, NO_THROW_TESTS) {
    TRange testee(1, 8, 2);
    EXPECT_NO_THROW(testee.begin());
    EXPECT_NO_THROW(testee.end());
    EXPECT_NO_THROW(testee.end());
    EXPECT_NO_THROW({std::cout << testee;});
    std::unordered_set<TRange> set;
    EXPECT_NO_THROW(set.insert(TRange(0, 88, 3)));
    EXPECT_NO_THROW(set.insert(TRange(0, 88, 1)));
    EXPECT_NO_THROW(set.insert(TRange(88, 0, -3)));
    EXPECT_NO_THROW(set.insert(TRange(20, 108, 3)));
    EXPECT_NO_THROW(set.insert(TRange(-88, 0, 3)));
}

TEST(ExceptionsLogic, THROW_TESTS) {
    EXPECT_THROW(TRange(1, 8, 0);, 
                 std::logic_error);
    EXPECT_THROW(TRange(1, 1, -2);, 
                 std::logic_error);
    EXPECT_THROW(TRange(1, 8, -2);, 
                 std::logic_error);
    EXPECT_THROW(TRange(8, 1, 2);, 
                 std::logic_error);
    auto testee = TRange(1, 8, 2).begin();
    EXPECT_EQ(3, *(++testee));
    EXPECT_EQ(5, *(++testee));
    EXPECT_EQ(7, *(++testee));
    EXPECT_THROW(*(++testee);, 
                 std::out_of_range);
}

TEST(StandartWorkflow, TestResultsOperator) {
    TRange testee(1, 8, 2);
    EXPECT_EQ(1, testee[0]);
    EXPECT_EQ(3, testee[1]);
    EXPECT_EQ(5, testee[2]);
    EXPECT_EQ(7, testee[3]);

    EXPECT_EQ(1, *(testee.begin()));
    EXPECT_EQ(10, *(testee.end()));

    auto iterator = testee.begin();
    EXPECT_EQ(3, *(++iterator));
    EXPECT_EQ(5, *(++iterator));
    EXPECT_EQ(7, *(iterator++));

    TRange testee1(9, 1, -2);
    EXPECT_EQ(7, testee1[1]);
    EXPECT_EQ(5, testee1[2]);

    EXPECT_EQ(4, testee.GetSize());
    EXPECT_EQ(6, testee1.GetSize());

    EXPECT_EQ(9, *(testee1.begin()));
    EXPECT_EQ(-1, *(testee1.end()));

    auto iterator2 = testee1.begin();
    EXPECT_EQ(7, *(iterator2++));
    EXPECT_EQ(5, *(++iterator2));
    EXPECT_EQ(3, *(iterator2++));
    EXPECT_EQ(1, *(++iterator2));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
