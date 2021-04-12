#define _CRT_SECURE_NO_WARNINGS
#define M 255
#define N 255

#include <stdio.h>

int max_elem(int, int);

int main() {
	FILE *input, *output;
	input = fopen("input.txt", "r");
	if (input == NULL) {
		printf("Can't open file input.txt");
		return 1;
	}

	unsigned int col_amount;
	unsigned int row_amount;
	int matrix[M][N];
	int max;

	fscanf(input, "%d %d", &col_amount, &row_amount);
	unsigned int match_count = col_amount;

	for (int i = 0; i < row_amount; i++) {
		for (int j = 0; j < col_amount; j++) {
			fscanf(input, "%d", &matrix[i][j]);
		}
	}

	fclose(input);

	for (int i = row_amount - 2; i >= 0; i--) {
		for (int j = 0; j < col_amount; j++) {
			max = matrix[i + 1][j];

			if (j != 0) {
				max = max_elem(max, matrix[i + 1][j - 1]);
			}

			if (j != col_amount - 1) {
				max = max_elem(max, matrix[i + 1][j + 1]);
			}

			max = max_elem(max, matrix[i][j]);

			if (max == matrix[i][j]) {
				match_count++;
			}

			matrix[i][j] = max;
		}
	}

	output = fopen("output.txt", "w");
	if (output == NULL) {
		printf("Can't open file output.txt");
		return 1;
	}

	fprintf(output, "%d\n", match_count);
	for (int i = 0; i < row_amount; i++) {
		for (int j = 0; j < col_amount; j++) {
			fprintf(output, "%d ", matrix[i][j]);
		}
		fprintf(output, "\n");
	}

	fclose(output);
	return 0;
}

int max_elem(int max, int expr) {
	return (max < expr) ? expr : max;
}