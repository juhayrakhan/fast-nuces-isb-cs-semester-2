#pragma once
#pragma once
/**
Garden represented as a 2D grid of size N x M where each cell in the grid correspond to a pot, 
which can either be intact or broken. pots that are broken cannot hold plants.
Broken pots are  0 and intact pots are 1 in the grid (user input).
Your task is to determine how to plant strategically under the following rules:
You can plant a maximum of one plant per pot.
No two plants in the same row can be planted in adjacent columns
(consecutiveness rule). Means you cannot plant two plants in pots that are next to
each other in the same row.
You cannot plant in broken pots.
Compute the maximum number of plants while representing the rules. Also compute the
minimum number of plants needed to cover the garden such as each contiguous segment
of intact pots in a row has at least one plant. No additional plants can be added without
violating the non-consective rule.
Output should look like this
Enter total number of rows: 3
Enter total number of columns: 5
[	[1, 1, 0, 1, 1],
	[1, 0, 1, 1, 0],
	[1, 1, 1, 1, 1] ]
Maximum plants: 7
minimum covering plants: 5
 */

void placePlants(int** arr, int row, int column) {

	// 2 means it has a plant

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			if (*(*(arr+i)+j) == 0) continue;
			if (arr[i][(j==0?0:j-1)]== 2) continue;


			j++;
		}
	}
	

}


