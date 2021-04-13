#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>

void swap(int *, int *);
void delete_row_and_col(int **, int, int, int, int **);
int matrix_determinant(int **, int, int **);
void sifting(int *, int, int, int *);
void heap_sort(int *, int, int *);

int main() {
    FILE *input, *output;
    input = fopen("input.txt", "r");
    if (input == NULL) {
        printf("Can't open file input.txt");
        return 1;
    }

    int matrix_count = 0;
    fscanf(input, "%d", &matrix_count);

    int ***matrices = (int ***)malloc(matrix_count * sizeof(int **));
    for (int i = 0; i < matrix_count; i++) {
        matrices[i] = (int **)malloc(64 * sizeof(int *));

        for (int j = 0; j < 64; j++) {
            matrices[i][j] = (int *)malloc(64 * sizeof(int));
        }
    }

    int *sizes = (int *)malloc(matrix_count * sizeof(int));
    int element = 0;
    for (int i = 0; i < matrix_count; i++) {
        fscanf(input, "%d", &sizes[i]);

        for (int j = 0; j < sizes[i]; j++) {
            for (int k = 0; k < sizes[i]; k++) {
                fscanf(input, "%d", &element);
                matrices[i][j][k] = element;
            }
        }
    }

    fclose(input);

    int **result_matrix = (int **)malloc(64 * sizeof(int *));
    for (int i = 0; i < 64; i++) {
        result_matrix[i] = (int *)malloc(64 * sizeof(int));
    }

    int *determinants = (int *)malloc(matrix_count * sizeof(int));
    for (int i = 0; i < matrix_count; i++) {
        determinants[i] = matrix_determinant(matrices[i], sizes[i], result_matrix);
    }

    int *indexes = (int *)malloc(matrix_count * sizeof(int));
    for (int i = 0; i < matrix_count; i++) {
        indexes[i] = i;
    }

    heap_sort(determinants, matrix_count, indexes);

    free(determinants);

    for (int i = 0; i < 64; i++) {
        free(result_matrix[i]);
    }
    free(result_matrix);

    output = fopen("output.txt", "w");
    if (output == NULL) {
        printf("Can't open file output.txt");
        return 1;
    }

    for (int i = 0; i < matrix_count; i++) {
        for (int j = 0; j < sizes[indexes[i]]; j++) {
            for (int k = 0; k < sizes[indexes[i]]; k++) {
                fprintf(output, "%d ", matrices[indexes[i]][j][k]);
            }

            fprintf(output, "\n");
        }
    }

    for (int i = 0; i < matrix_count; i++) {
        for (int j = 0; j < 64; j++) {
            free(matrices[i][j]);
        }
        free(matrices[i]);
    }
    free(matrices);

    fclose(output);

    free(sizes);
    free(indexes);

    return 0;
}

void swap(int *pa, int *pb) {
    int c = *pa;
    *pa = *pb;
    *pb = c;
}

void delete_row_and_col(int **matrix, int size, int row, int col, int **result_matrix) {
    int shift_row = 0;
    int shift_col = 0;

    for (int i = 0; i < size - 1; i++) {
        if (i == row) {
            shift_row = 1;
        }

        shift_col = 0;
        for (int j = 0; j < size - 1; j++) {
            if (j == col) {
                shift_col = 1;
            }

            result_matrix[i][j] = matrix[i + shift_row][j + shift_col];
        }
    }
}

int matrix_determinant(int **matrix, int size, int **result_matrix) {
    int determinant = 0;
    int sign = 1;

    if (size == 1) {
        return matrix[0][0];
    }

    if (size == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    for (int i = 0; i < size; i++) {
        delete_row_and_col(matrix, size, 0, i, result_matrix);
        determinant = determinant + (matrix[0][i] * sign * matrix_determinant(result_matrix, size - 1, result_matrix));
        sign *= -1;
    }

    return determinant;
}

void sifting(int *determinants, int root, int end, int *indexes) {
    int max_child;
    int indexes_root = root;

    while (root * 2 + 1 <= end) {
        if (root * 2 + 1 == end) {
            max_child = root * 2 + 1;
        }
        else if (determinants[root * 2 + 1] > determinants[root * 2 + 2]) {
            max_child = root * 2 + 1;
        }
        else {
            max_child = root * 2 + 2;
        }

        if (determinants[root] < determinants[max_child]) {
            swap(determinants + root, determinants + max_child);
            root = max_child;

            swap(indexes + indexes_root, indexes + max_child);
            indexes_root = max_child;
        }
        else {
            return;
        }
    }
}

void heap_sort(int *determinants, int size, int *indexes) {

    for (int i = (size / 2) - 1; i >= 0; i--) {
        sifting(determinants, i, size - 1, indexes);
    }

    for (int i = size - 1; i >= 1; i--) {
        swap(determinants + i, determinants);
        swap(indexes + i, indexes);
        sifting(determinants, 0, i - 1, indexes);
    }
}