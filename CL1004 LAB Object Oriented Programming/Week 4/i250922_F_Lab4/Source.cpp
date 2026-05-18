// Juhayra Khan 25I-0922 Section BCS F

// #include "Task1.h"
#include "Task2.h"
#include "Task3.h"
#include "Task4.h"
#include "Task5.h"
#include <iostream>

using namespace std;

int main() {
	int n;
	cout << "Question N0. "; cin >> n;
	switch (n) {
	case 2: {
		int row, col; row = col = 4;
		int** arr = new int* [row];
		for (int i = 0; i < row; i++) {
			*(arr+i) = new int[col];
			for (int j = 0; j < col; j++) cin >> *(*(arr+i)+j);
		}

		countUnsafe(arr, row, col);

		delete[] arr; arr = nullptr;
		break;
	}
	case 3: {
		int n; cin >> n;
		int** arr = new int* [n];
		for (int i = 0; i < n; i++) {
			*(arr+i) = new int[n];
			for (int j = 0; j < n; j++) cin >> *(*(arr+i)+j);
		}

		if (isSymmetric(arr, n)) cout << "Yes";
		else cout << "No";

		delete[] arr; arr = nullptr;
		break;
	}
	case 4: {
		int n; cin >> n;
		int** arr = new int*[n];
		for (int i = 0; i < n; i++) *(arr+i) = new int[n] {0};
		Queens(arr, n);
		delete[] arr; arr = nullptr;
		break;
	}
	case 5: {
		int size; cin >> size;
		int* arr = new int[size];
		for (int i = 0; i < size; i++) cin >> *(arr+i);
		int val; cin >> val;

		int* sub = new int[size];
		bool found = false;

		cout << (sumExists(arr, size, val) ? "\nExists!" : "\nDoes Not Exist!");

		delete[] arr; arr = nullptr;
		delete[] sub; sub = nullptr;
		break;
		}
	}
}