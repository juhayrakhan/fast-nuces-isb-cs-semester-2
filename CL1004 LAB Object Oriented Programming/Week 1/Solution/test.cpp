// Juhayra Khan 25i-0922

#include<iostream>
#include "pch.h"
#include "Header.h"
using namespace std;

//Question 1
    TEST(sumArray, Sum1) {
        int arr[] = { 7,2,8,4,5,9 };
        ASSERT_EQ(35, sumArray(arr, 6));
    }

    TEST(sumArray, sum2) {
        int arr[] = { -1,-2,-7,3,-11,4,0 };
        ASSERT_EQ(-14, sumArray(arr, 7));
    }

//Question 2
    TEST(reverseArray, t1) {
        int arr[] = { 1, -2, 6, 3, -9, 4, 10 };
        int a[] = { 10,4,-9,3,6,-2,1 };
        int* arr1 = reverseArray(arr, 7);

        for (int i = 0; i < 7; i++) {
            ASSERT_EQ(a[i], arr1[i]);
        }

    }

//Question 4
    TEST(findMinMax, t1) {
        int minVal, maxVal;
        const int size = 5;
        int arr1[size] = { 1,2,3,4,5 };
        findMinMax(arr1, size, minVal, maxVal);
        EXPECT_EQ(maxVal, 5);
        EXPECT_EQ(minVal , 1);
    }

    TEST(findMinMax, t2) {
        int minVal, maxVal;
        const int size = 7;
        int arr2[size] = {0,0,-1,0,0,1,0};
        findMinMax(arr2, size, minVal, maxVal);
        EXPECT_EQ(maxVal, 1);
        EXPECT_EQ(minVal, -1);
    }

    TEST(findMinMax, t3) {
        int minVal, maxVal;
        const int size = 5;
        int arr2[size] = { 0,0,0,0,0 };
        findMinMax(arr2, size, minVal, maxVal);
        EXPECT_EQ(maxVal, 0);
        EXPECT_EQ(minVal, 0);
    }

//QUestion 5
    TEST(isPrime, t1) {
        EXPECT_FALSE(isPrime(5));
    }

    TEST(isPrime, t2) {
        EXPECT_FALSE(isPrime(1));
    }

    TEST(isPrime, t3) {
        EXPECT_TRUE(isPrime(4));
    }

    TEST(isPrime, t4) {
        EXPECT_FALSE(isPrime(0));
    }

//Question 6
    TEST(factorial, t1) {
        EXPECT_EQ(factorial(0),0);
    }

    TEST(factorial, t2) {
        EXPECT_EQ(factorial(4), 24);
    }

    TEST(factorial, t3) {
        EXPECT_EQ(factorial(1), 1);
    }