// Juhayra Khan 25i-0922 F

#pragma once

/* 1.
Write a program that has an integer array having n elements. The program should have a
function that can receive the array and then return the sum of all the elements of the
array.
Function Prototype: int sumArray(int arr[ ],int size)
*/

int sumArray(int arr[], int size) {
	int sum = 0;
	for (int i = 0; i < size; i++)
		sum += arr[i];
	return sum;
}

/* 2.
Write a function that reverses the array using a single loop. The program should have a
function that can receive the array and then return the reverse array.
Function Prototype: int *reverseArray(int arr[ ], int size)
*/

int* reverseArray(int arr[], int size) {
	int* arr2 = new int[size];

	for (int i = 0; i < size; i++) {
		arr2[size-i-1] = arr[i];
	}

	return arr2;
}

/* 3.
Write a function that finds the minimum and maximum values in an integer array.
Function Prototype: void findMinMax(int arr[ ], int size, int &amp;minVal, int &amp;maxVal)
*/

void findMinMax(int arr[], int size, int& minVal, int& maxVal) {
	minVal = arr[0], maxVal = arr[0];
	for (int i = 0; i < size; i++) {
		if (arr[i] < minVal) minVal = arr[i];
		if (arr[i] > maxVal) maxVal = arr[i];
	}
}

/* 4.
Write a function that checks whether a given integer is a prime number.
Function Prototype: bool isPrime(int number)
*/

bool isPrime(int number) {
	if (number <= 1) return false;
	for (int i = 2; i < number; i++)
		if (number % i == 0) return true;
	return false;
}

/* 5.
Write a function that calculates the factorial of a given non-negative integer using recursion.
Function Prototype: int factorial(int n)
*/

int factorial(int n) {
	if (n <= 0) return 0;
	if (n == 1) return 1;
	return n * factorial(n - 1);
}