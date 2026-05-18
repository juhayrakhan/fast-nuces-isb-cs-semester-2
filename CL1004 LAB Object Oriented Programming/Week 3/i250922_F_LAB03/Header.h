#pragma once
// 25I-0922 Juhayra Khan Section F Lab 03

// Question 1
int* sumArray(int** arr, int rows, int cols) {
	int* result = new int[4] {0};

	for (int i = 0; i < rows; i+=2)
		for (int j = 0; j < cols; j+=2)
			*result += *(*(arr+i)+j);
		
	for (int i = 0; i < rows; i += 2) 
		for (int j = 1; j < cols; j += 2) 
			*(result+1) += *(*(arr+i)+j);
		
	for (int i = 1; i < rows; i += 2) 
		for (int j = 0; j < cols; j += 2) 
			*(result + 2) += *(*(arr+i)+j);
		
	for (int i = 1; i < rows; i += 2) 
		for (int j = 1; j < cols; j += 2) 
			*(result + 3) += *(*(arr+i)+j);
	
	return result;
}

// Question 2

// NOTE: changed test case to change expecteq(sum,25) to (sum,20);

int diagonalSum(int** matrix, int size) {
	int sum = 0;
	for (int i = 0; i < size; i++){
		sum += *(*(matrix+i)+i);
		sum += *(*(matrix+size-i-1)+i);
	}
	if (size % 2) sum -= *(*(matrix + (size/2)) + (size/2));

	return sum;

}

// QUestion 3
int countNegatives(int** grid, int rows, int columns) {
	int count = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (*(*(grid + i) + j) < 0) count++;
		}
	}
	return count;
}

//Question 4
int** setZeroes(int** matrix, int rows, int columns) {
	int** arr= new int*[rows];
	for (int i = 0; i < rows; i++) {
		*(arr+i) = new int[columns];
		for (int j = 0; j < columns; j++) *(*(arr+i)+j) = -1;
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (!*(*(matrix + i) + j)) {
				for (int k = 0; k < rows; k++) *(*(arr + k) + j) = 0;
				for (int k = 0; k < columns; k++) *(*(arr + i) + k) = 0;
			}
			else if (!*(*(arr+i)+j)) continue;
			else *(*(arr+i)+j) = *(*(matrix + i) + j);
		}
	};
	return arr;
}

// Quesion 5

// NOTE: Changed test case to add -1 to original array and removing delete[] at end

int** construct2DArray(int* original, int m, int n)  {
	int count = 0;
	int* copyOriginal = original;
	while (*copyOriginal != -1) {
		*copyOriginal++;
		count++;
	}

	if (m * n != count) return nullptr;

	int** newarr = new int* [m];
	for (int i = 0; i < m; i++) {
		*(newarr + i) = original + i*n;
	}

	return newarr;
}