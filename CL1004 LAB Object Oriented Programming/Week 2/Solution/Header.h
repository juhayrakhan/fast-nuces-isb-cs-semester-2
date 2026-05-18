#pragma once

#include <iostream>
using namespace std;
/*
Write a C++ program to dynamically allocate memory for a string. The program should find the word
in the string that has the highest number of repeated letters. Ensure you use 1D dynamic memory
allocation for the input string and deallocate the memory properly.

Example:
	Input:		Print a welcome text in a separate line.
	Output:		Word which has the highest number of repeated letters.
*/

int countwords(char* word) {
	if(word == nullptr || word[0] == '\0') return 0;
	int count = 1;
	for(int i = 0; word[i] != '\0'; i++)
		if(word[i] == ' ') count++;
	return count;
}

int getLength(char* str) {
	int len = 0;
	while(str[len] != '\0') len++;
	return len;
}

int countbestrep(char* word) {
	int freq[256] = {0};
	int maxf = 0;

	for(int i = 0; word[i] != '\0'; i++) {
		unsigned char c = word[i];
		freq[c]++;
		if(freq[c] > maxf)
			maxf = freq[c];
	}
	return maxf;
}

char* copyy(char* str, int start, int len) {
	char* word = new char[len + 1];
	for(int i = 0; i < len; i++)
		word[i] = str[start + i];
	word[len] = '\0';
	return word;
}

//char* findendofWord(char* atr){}

char* countRepeatedLetters(char* str) {
	if(str == nullptr || str[0] == '\0') return nullptr;

	int i = 0, bestrep = 0; char* best = 0;

	while(str[i] != '\0') {
		while(str[i] == ' ') i++;
		if(str[i] == '\0') break;

		int start = i;
		while(str[i] != ' ' && str[i] != '\0') i++;
		int len = i - start;

		char* word = copyy(str, start, len);
		int rep = countbestrep(word);

		if(rep > bestrep) {
			delete[] best; best = word;
			bestrep = rep;
		}
		else delete[] word;
		
	}

	return best;
}


/*
Given the string &quot;A string.&quot; Print on one line the letter on the index 0, the pointer position and the
letter t. update the pointer to pointer +2. Then, in another line print the pointer and the letters r and
g of the string (using the pointer).
*/
char printAndUpdatePointer(char* inputStr) {
	int i;
	char* ptr = inputStr;
	cout << *ptr << ptr;
	for(i = 0; i < getLength(inputStr); ptr++, i++) {
		if(*ptr == 't') {
			cout << ptr << *ptr << endl;
			break;
		}
	}
	ptr += 2;
	cout << (ptr - 1) << *(ptr - 1) << (ptr + 2) << *(ptr + 2) << endl;

	return *ptr;
}

/*
Write a function named calculateGardes which takes an array and its size as function parameters.
You are required to apply following grading policy. Note: You have to release the memory using the
delete operator before the end of the program.
*/
char* calculateGrades(int arr[], int size) {
	char* grades = new char[size];
	for(int i = 0; i < size; i++) {
		if(arr[i] > 90) grades[i] = 'A';
		else if(arr[i] > 75) grades[i] = 'B';
		else if(arr[i] > 59) grades[i] = 'C';
		else if(arr[i] > 50) grades[i] = 'D';
		else grades[i] = 'F';
	}
	return grades;
}

/*
Write a function named as sortArray which takes an array and its size as function parameters, you
are required to sort the array such that odd numbers come first and then even numbers. Odd and
even values should be reordered within their interval i-e in the example given below if 6 comes
before 4 in original array, it must be sort in the sorted array. Use pointer notation to solve this
problem.
*/

int* copyyy(int* arr, int size) {
	int* newarr = new int[size];
	for(int i = 0; i < size; i++) {
		newarr[i] = arr[i];
	}
	return newarr;
}

void sortit(int arr[], int start, int end) {
	for(int i = start;i < end-1;i++){
		for(int j = start;j < end - 1;j++){
			if(arr[j] > arr[j + 1]){
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

int* sortArray(int* arr, int size) {
	int oddcount = 0;
	for(int i = 0; i < size; i++) {
		if(arr[i] % 2) oddcount++;
	}
	int odddone = 0;
	for(int i = 0; i < size && odddone < oddcount; i++) {
		if(arr[i] % 2) {
			int temp = arr[i];
			arr[i] = arr[odddone];
			arr[odddone] = temp;
			odddone++;
		}
	}

	sortit(arr, 0, oddcount);
	sortit(arr, oddcount, size);

	return copyyy(arr, size);

}

/*
You are given an array A of N integers.You want to choose some integers from the array subject to
the condition that the number of distinct integers chosen should not exceed K. Your task is to
maximize the sum of chosen numbers. Print the maximum sum you can obtain by choosing some
elements such that the number of distinct integers chosen is at most K. If you cannot choose any
element, output 0.
Note: You need to implement the main for this problem by taking values of the array as input from
the user.
Example
Input
K=2, Array=2, 1, 2, 5
Output: 9
Explanation
we have N=4, K=2, A=[2,1,2,5] . We need to choose atmost 2 distinct integers, we choose 2,2,5. Note
that the condition is choosing atmost distinct integers. So we can choose repeated number as many
times as we want. The sum is 2+2+5=9 and we output it.
*/

void append(int* &arr, int &size, int val) {
	int* newArr = new int[size + 1];
	newArr[size] = val;
	for(int i = 0; i < size; i++)
		newArr[i] = arr[i];
	size++;
	delete[] arr; arr = newArr;
}

int maximizeSumWithKDistinct(int* arr, int size, int k) {

	int *vals = new int[1], *freq = new int[1];
	int distinctvals = 0; int distinctvals2 = 0;

	for(int i = 0; i < size; i++) {
		int found = -1;
		for(int j = 0; j < distinctvals; j++)
			if(vals[j] == arr[i]) {
				found = j;
				break;
			}
		if(found == -1) {
			append(vals, distinctvals, arr[i]);
			append(freq, distinctvals2, 1);
		}
		else  (freq[found])++;
	}

	int* cumulval = new int[distinctvals];
	for(int i = 0; i < distinctvals; i++)
		cumulval[i] = vals[i] * freq[i];

	for(int i = 0; i < distinctvals - 1; i++) {
		for(int j = 0; j < distinctvals - i - 1; j++) {
			if(cumulval[j] < cumulval[j + 1]) {
				int temp = cumulval[j];
				cumulval[j] = cumulval[j + 1];
				cumulval[j + 1] = temp;
			}
		}
	}

	int sum = 0;
	for(int i = 0; i < distinctvals && i < k; i++)
		if(cumulval[i] > 0) sum += cumulval[i];
		else break;
	
	delete[] vals; delete[] freq; delete[] cumulval;
	vals = nullptr; freq = nullptr; cumulval = nullptr;

	return sum;
}
