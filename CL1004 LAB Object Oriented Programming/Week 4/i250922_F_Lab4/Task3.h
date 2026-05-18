#pragma once

/*
logo is a int matrix of size N × N,
1 is a color and 0 is no color.
A logo is symmetric if colored pixels are symmetric about both the X-axis and Y-axis. 
print Yes if it is symmetric or No otherwise.
The input consists of the integer N followed by N lines of N integers
representing the pixels of the logo. The solution must efficiently handle the matrix using
dynamic memory, without using static arrays or extra
libraries.
For instance: Take a 5x5 matrix as follows
[	[0, 1, 1, 1, 0],
	[0, 1, 0, 1, 0],
	[1, 0, 0, 0, 1],
	[0, 1, 0, 1, 0],
	[0, 1, 1, 1, 0] ]
It is symmetric around x and y axis.
*/

bool isSymmetric(int** arr, int n) {
	for (int i = 0; i < n/2; i++) {
		for (int j = 0; j < n/2; j++) {
			if (*(*(arr+i)+j) != *(*(arr+n - i - 1)+j)) return 0;
			if (*(*(arr+i)+j) != *(*(arr+i)+n - j - 1)) return 0;
		}
	}
	return 1;
}