#include<iostream>
#include "pch.h"
#include "../Lab01/Header.h"
using namespace std;

// Question 1
TEST(sumof2Darray, case1) {
    int* a1 = new int[5] {10, 9, 8, 7, 6};
    int* a2 = new int[5] {9, 8, 7, 6, 5};
    int* a3 = new int[5] {8, 7, 6, 5, 4};
    int* a4 = new int[5] {7, 6, 5, 4, 3};
    int* a5 = new int[5] {6, 5, 4, 3, 2};
    int* A[5] = { a1,a2,a3,a4,a5 };
    int* sum = sumArray(A, 5, 5);
    ASSERT_EQ(sum[0], 54);
    ASSERT_EQ(sum[1], 36);
    ASSERT_EQ(sum[2], 36);
    ASSERT_EQ(sum[3], 24);

}
TEST(sumof2Darray, case2) {

    int* a1 = new int[4] {10, 9, 7, 12};
    int* a2 = new int[4] {7, 9, 6, 11};
    int* a3 = new int[4] {19, 2, 21, 3};
    int* A[3] = { a1,a2,a3 };
    int* sum = sumArray(A, 3, 4);

    ASSERT_EQ(sum[0], 57);
    ASSERT_EQ(sum[1], 26);
    ASSERT_EQ(sum[2], 13);
    ASSERT_EQ(sum[3], 20);
}
// Question 2
TEST(DiagonalSumTest, SquareMatrix)
{
    int size = 3;

    int** matrix = new int* [size];
    for (int i = 0; i < size; ++i)
    {
        matrix[i] = new int[size];
        for (int j = 0; j < size; ++j)
        {
            matrix[i][j] = i * size + j; 
        }
    }

    int sum = diagonalSum(matrix, size);

    EXPECT_EQ(sum, 25); 
}
// Question 3
TEST(CountNegativesTest, EmptyMatrix)
{
    int* a1 = new int[5] {4, 3, 2, -1};
    int* a2 = new int[5] {3, 2, 1, -1};
    int* a3 = new int[5] {1, 1, -1, -2};
    int* a4 = new int[5] {-1,-1,-2,-3};

    int* A[5] = { a1,a2,a3,a4};

    int result = countNegatives(A, 4, 4);

    EXPECT_EQ(result, 8); 
}
// Question 4
TEST(SetZeroesTest, ExampleMatrix)
{
    int m = 3;
    int n = 4;
    int** matrix = new int* [m]
        {
            new int[4] {1, 2, 3, 0},
                new int[4] {4, 5, 0, 2},
                new int[4] { 1, 3, 1, 5 }
        };

    int** p=setZeroes(matrix, m, n);
    EXPECT_EQ(p[0][0], 0);
    EXPECT_EQ(p[0][1], 0);
    EXPECT_EQ(p[0][2], 0);
    EXPECT_EQ(p[0][3], 0);
    EXPECT_EQ(p[1][0], 0);
    EXPECT_EQ(p[1][1], 0);
    EXPECT_EQ(p[1][2], 0);
    EXPECT_EQ(p[1][3], 0);
    EXPECT_EQ(p[2][0], 1);
    EXPECT_EQ(p[2][1], 3);
    EXPECT_EQ(p[2][2], 0);
    EXPECT_EQ(p[2][3], 0);

    // Deallocate memory
     for (int i = 0; i < m; ++i) {
         delete[] matrix[i];
     }
     delete[] matrix;
}

// Question 5
TEST(Construct2DArrayTest, PossibleConstruction)
{
    int m = 2;
    int n = 3;
    int original[] = { 1, 2, 3, 4, 5, 6 };
    int expected[][3] = { {1, 2, 3}, {4, 5, 6} };

    int** result = construct2DArray(original, m, n);

    ASSERT_NE(result, nullptr);
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            EXPECT_EQ(result[i][j], expected[i][j]);
        }
    }

    for (int i = 0; i < m; ++i)
    {
        delete[] result[i];
    }
    delete[] result;
}
