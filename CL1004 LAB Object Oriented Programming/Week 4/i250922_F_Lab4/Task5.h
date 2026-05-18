#pragma once
#include <iostream>
using namespace std;

/*
Create a recursive function to determine whether there exists a subset of a given
array of integers that sums up to a specified target value. if yes it returns 1 else 0.
Example: Given the array 3,34,4,12,5,2 and target sum 9, the function should return 1
(since 4 + 5 = 9).
*/

void checkSum(int* arr, int size, int val, int& sum, int i=0) {
	if (i == size) return;
	sum += arr[i];
	checkSum(arr, size, val, sum, i + 1);
}

void checkDigit(int* arr, int size, int val, bool& check, int i = 0) {
	if (i == size) return;
	if (arr[i] == val) check = true;
	checkDigit(arr, size, val, check, i + 1);
}

void creatingSubset(int* arr, int size, int val, int* sub, bool&found, int subsize=0, int index=0) {

	if (found) return;

	int sum = 0;
	checkSum(sub, subsize, val, sum);
	if (sum == val) {
		found = true;
		return;
	}

	for (int i = index; i < size; i++) {
		sub[subsize] = arr[i];
		creatingSubset(arr, size, val, sub, found, subsize + 1, i + 1);
	}

}

bool sumExists(int* arr, int size, int val) {

	int sum = 0; 
	checkSum(arr, size, val, sum);
	if (sum < val) return 0;
	else if (sum == val) return 1;

	bool check = false;
	checkDigit(arr, size, val, check);
	if (check) return 1;

	sum = 0; int subsize = size;
	int* sub = new int[subsize];

	bool found = false;
	creatingSubset(arr, size, val, sub, found);
	return found;

}