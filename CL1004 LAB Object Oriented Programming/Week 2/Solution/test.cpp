#include "pch.h"
#include "Header.h"
// Question 1
TEST(Q1, Best_repeat) {
    char* arr = "hello hi yes";
    char* exp = "hello";
    char* result = countRepeatedLetters(arr);
    for(int i = 0; i < 5; i++) {
        ASSERT_EQ(exp[i], result[i]);
    }
}

// ques22
TEST(Q3, printAndUpdatePointer) {
    char str[] = "A string";
    char result = printAndUpdatePointer(str);
    EXPECT_EQ(result, 'i');
}

//ques 3
TEST(calculateGrades, grade) {
    int arr[] = {95, 63, 76, 40, 85};
    char a[] = {'A','C','B','F','B'};
    char* arr1 = calculateGrades(arr, 5);

    for(int i = 0; i < 5; i++) {
        ASSERT_EQ(a[i], arr1[i]);
    }

    int arr2[] = {35, 72, 59, 88, 100, 43};
    char a1[] = {'F','C','D','B','A','F'};
    char* arr3 = calculateGrades(arr2, 6);

    for(int i = 0; i < 6; i++) {
        ASSERT_EQ(a1[i], arr3[i]);
    }

}

//ques 4
TEST(sortArray, sort) {
    int sort_arr[] = {6, 3, 1, 2, 7, 5, 9, 4};
    char sort_a[] = {1,3,5,7,9,2,4,6};
    int* sort_arr1 = sortArray(sort_arr, 8);

    for(int i = 0; i < 8; i++) {
        ASSERT_EQ(sort_a[i], sort_arr1[i]);
    }

    int arr[] = {7, 42, 27, 2, 12, 55, 9, 50, 8, 15};
    char a[] = {7,9,15,27,55,2,8,12,42,50};
    int* arr1 = sortArray(arr, 10);

    for(int i = 0; i < 10; i++) {
        ASSERT_EQ(a[i], arr1[i]);
    }

}

//ques 5
TEST(MaximizetheSum, case1) {
    int arr1[] = { 2, 1, 2, 5 };
    int k = 2; // Choose 2, 2, 5 (distinct integers = 2)
    ASSERT_EQ(maximizeSumWithKDistinct(arr1, 4, 2), 9);
    int arr2[] = { 3, -1, 2, 5 };
    int k1 = 1;
    ASSERT_EQ(maximizeSumWithKDistinct(arr2, 4, 1), 5);

}