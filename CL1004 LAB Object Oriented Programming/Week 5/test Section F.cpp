#include<iostream>
#include "pch.h"
#include "../Lab01/Header.h"
using namespace std;


// question 1
TEST(RomanToIntTest, AdditionCases)
{
    EXPECT_EQ(6, romanToInt("VI"));
    EXPECT_EQ(8, romanToInt("VIII"));
    EXPECT_EQ(15, romanToInt("XV"));
    EXPECT_EQ(27, romanToInt("XXVII"));
    EXPECT_EQ(60, romanToInt("LX"));
}


// question 2
TEST(IsValidTest, MultipleValidBrackets)
{
    EXPECT_TRUE(isValid("()[]{}"));
    EXPECT_TRUE(isValid("([])"));
    EXPECT_TRUE(isValid("{[()]}"));
}




// question 3
TEST(LongestConsecutive, array)
{
    int arr[] = { 15,20,21, 1, 2, 13, 17, 16,18,23,24 };
    int val = longestConsecutive(arr, 11, 0, 0);
    EXPECT_EQ(val, 4);

    int arr2[] = { 5,5,6,11,44,8,200,300,9 };
    int val2 = longestConsecutive(arr2, 9, 0, 0);
    EXPECT_EQ(val2, 2);
}


// question 5
TEST(DateTest, DaysBetweenSameDate) {
    Date d1 = { 10, 5, 2024 };
    Date d2 = { 10, 5, 2024 };
    EXPECT_EQ(d1.daysBetween(d1, d2), 0);
}

TEST(DateTest, DaysBetweenDifferentDates) {
    Date d1 = { 1, 1, 2024 };
    Date d2 = { 10, 1, 2024 };
    EXPECT_EQ(d1.daysBetween(d1, d2), 9);
}

TEST(DateTest, DaysBetweenLeapYear) {
    Date d1 = { 28, 2, 2024 };  // Leap year
    Date d2 = { 1, 3, 2024 };
    EXPECT_EQ(d1.daysBetween(d1, d2), 2);
}

TEST(DateTest, IncrementDateSimpleCase) {
    Date d = { 10, 5, 2024 };
    d.incrementDate(d);
    EXPECT_EQ(d.day, 11);
    EXPECT_EQ(d.month, 5);
    EXPECT_EQ(d.year, 2024);
}

TEST(DateTest, IncrementDateMonthOverflow) {
    Date d = { 31, 1, 2024 };  // January 31 → February 1
    d.incrementDate(d);
    EXPECT_EQ(d.day, 1);
    EXPECT_EQ(d.month, 2);
    EXPECT_EQ(d.year, 2024);
}

TEST(DateTest, IncrementDateYearOverflow) {
    Date d = { 31, 12, 2024 };  // December 31 → January 1 (next year)
    d.incrementDate(d);
    EXPECT_EQ(d.day, 1);
    EXPECT_EQ(d.month, 1);
    EXPECT_EQ(d.year, 2025);
}
