#pragma once
#include <iostream>
using namespace std;

/*
You are given a chessboard of size N × N and your task is to place N queens on the
board such that no two queens attack each other. A queen is represented by 1 and an
empty cell by 0. For this simplified version, the rule is: if a queen is placed in a cell, no
other queen can be placed in any adjacent cell (including horizontally, vertically, and
diagonally). Your task is to use recursion to explore all possible placements and
determine a valid configuration of the board. The program should output one valid
arrangement of the chessboard showing the positions of all queens. You must implement
the solution using recursive function(s), without using iterative placement strategies, and
the board can be stored using dynamic memory allocation with 2D pointers (int**).A
*/

void printBoard(int** arr, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) cout << *(*(arr+i)+j) << ' ';
		cout << endl;
	}
}

bool checkSafe(int** arr, int row, int col, int n) {

	for (int i = 0; i < row; i++)
		if (arr[i][col] == 1) return false; // vertical values b4 this one |

	for (int i = row, j = col; i >=0 && j >=0 ; i--, j--) // left up 
		if (*(*(arr+i)+j) == 1) return false;

	for (int i = row, j = col; i >=0 && j < n; i--, j++) // left down /
		if (*(*(arr+i)+j) == 1) return false;

	return 1;
}

bool Queens(int** arr, int n, int row=0) {

	if (row == n) {
		printBoard(arr, n);
		cout << "-----------\n";
		return true;
	}

	bool check = false;
	for (int col = 0; col < n; col++) {
		if (checkSafe(arr,row,col,n)) {
			*(*(arr+row)+col) = 1;
			check |= Queens(arr, n, row + 1);
			*(*(arr+row)+col) = 0; // cleanup because need a clean board for next iteration
		}
	}
	return check;
}