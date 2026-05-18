#pragma once
#include <iostream>
using namespace std;
 /*
 You are given a matrix of natural numbers of size N x M.
Let MAX and MIN. Any cell containing MAX or MIN is considered
unsafe, and an unsafe cell corrodes its entire row and column, making all other cells in
the same row and column unsafe as well. Your task is to use dynamic memory
allocation with 2D pointers (int**) to create and store the matrix, identify all unsafe
cells according to these rules, count the number of remaining safe cells, and display the
matrix with unsafe cells marked as X while safe cells retain their original values. The

input consists of two integers N and M followed by N lines each containing M integers
representing the matrix elements. The output should first display the number of safe cells,
followed by the updated matrix with unsafe cells marked.
 */

void countUnsafe(int** arr, int row, int column) {
	int max = **arr;  int min = **arr;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			if (max < *(*(arr+i)+j)) max = *(*(arr+i)+j);
			if (min > *(*(arr+i)+j)) min = *(*(arr+i)+j);
		}
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			if (*(*(arr+i)+j) == max || *(*(arr+i)+j) == min) {
				for (int k = 0; k < row; k++) *(*(arr + k) + j) = -1;
				for (int k = 0; k < column; k++) *(*(arr + i) + k) = -1;
			}
		}
	}

	int count = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			if (*(*(arr+i)+j)==-1) count++;
		}
	}

	cout << (row*column)-count << endl;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			if (*(*(arr+i)+j)!=-1) cout << *(*(arr+i)+j) << ' ';
			else cout << "X ";
		}
		cout << endl;
	}
}