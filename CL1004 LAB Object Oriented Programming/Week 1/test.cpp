#include<iostream>
#include "pch.h"
#include "../Lab01/Header.h"
using namespace std;
// Question 1

TEST(sumArray, Sum1) { 
    int arr[]={7,2,8,4,5,9};
    ASSERT_EQ(35, sumArray(arr,6));
}

TEST(sumArray, sum2) {
    int arr[]={-1,-2,-7,3,-11,4,0};
    ASSERT_EQ(-14, sumArray(arr,7));
}
// Question 2
TEST(reverseArray, t1) { 
    int arr[] = {1, -2, 6, 3, -9, 4, 10};
    int a[]={10,4,-9,3,6,-2,1};
    int *arr1=reverseArray(arr,7);

    for(int i=0; i<7; i++){
        ASSERT_EQ(a[i], arr1[i]);
    }

}