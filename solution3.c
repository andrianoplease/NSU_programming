#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <intrin.h>
#include <math.h>

void swap(int *, int *);
void delete_row_and_col(int **, int, int, int, int **);
int matrix_determinant(int **, int, int **);
void sifting(int *, int, int, int *);
void heap_sort(int *, int, int *);
long int sum(int *, int);
double sample_mean(int *, int);
double sample_variance(int *, int);
void selection_sort(int *, int);
void test(FILE *, FILE *, FILE *, FILE *);

int main() {
    FILE *input, *output, *initial, *sorted;

    input = fopen("input.txt", "r");
    if (input == NULL) {
        printf("Can't open file input.txt");
        return 1;
    }

    output = fopen("output.txt", "w");
    if (output == NULL) {
        printf("Can't open file output.txt");
        return 1;
    }

    initial = fopen("initial.txt", "w");
    if (input == NULL) {
        printf("Can't open file initial.txt");
        return 1;
    }

    sorted = fopen("sorted.txt", "w");
    if (input == NULL) {
        printf("Can't open file sorted.txt");
        return 1;
    }

    test(input, output, initial, sorted);

    fclose(input);
    fclose(output);
    fclose(initial);
    fclose(sorted);

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

void heap_sort(int *array, int size, int *indexes) {
    for (int i = (size / 2) - 1; i >= 0; i--) {
        sifting(array, i, size - 1, indexes);
    }

    for (int i = size - 1; i >= 1; i--) {
        swap(array + i, array);
        swap(indexes + i, indexes);
        sifting(array, 0, i - 1, indexes);
    }
}

long int sum(int *selection, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += selection[i];
    }
    return sum;
}

double sample_mean(int *selection, int size) {
    return (double)sum(selection, size) / (double)size;
}

double sample_variance(int *selection, int size) {
    int sum_squares = 0;
    int mean = sample_mean(selection, size);
    for (int i = 0; i < size; i++) {
        sum_squares += pow(selection[i] - mean, 2);
    }
    return sqrt(sum_squares / size, 2);
}

void selection_sort(int *array, int size) {
    for (int i = 0; i < size; i++) {
        int min_index = i;
        for (int j = i + 1; j < size; j++) {
            if (array[min_index] > array[j]) {
                min_index = j;
            }
        }
        swap(array, array + min_index);
    }
}

void test(FILE *input, FILE *output, FILE *initial, FILE *sorted) {
    srand(time(NULL));

    int matrices_amount = 1 + rand() % 100;

    int ***matrices = (int ***)malloc(matrices_amount * sizeof(int **));
    for (int i = 0; i < matrices_amount; i++) {
        matrices[i] = (int **)malloc(64 * sizeof(int *));
        for (int j = 0; j < 64; j++) {
            matrices[i][j] = (int *)malloc(64 * sizeof(int));
        }
    }

    int *sizes = (int *)malloc(matrices_amount * sizeof(int));
    for (int i = 0; i < matrices_amount; i++) {
        sizes[i] = 1 + rand() % 10;
        for (int j = 0; j < sizes[i]; j++) {
            for (int k = 0; k < sizes[i]; k++) {
                matrices[i][j][k] = 1 + rand() % 100;
            }
        }
    }

    int **result_matrix = (int **)malloc(64 * sizeof(int *));
    for (int i = 0; i < 64; i++) {
        result_matrix[i] = (int *)malloc(64 * sizeof(int));
    }

    int *determinants = (int *)malloc(matrices_amount * sizeof(int));
    for (int i = 0; i < matrices_amount; i++) {
        determinants[i] = matrix_determinant(matrices[i], sizes[i], result_matrix);
    }

    int *determinants_copy = (int *)malloc(matrices_amount * sizeof(int));
    for (int i = 0; i < matrices_amount; i++) {
        determinants_copy[i] = determinants[i];
    }

    int *indexes = (int *)malloc(matrices_amount * sizeof(int));
    for (int i = 0; i < matrices_amount; i++) {
        indexes[i] = i;
    }

    for (int i = 0; i < matrices_amount; i++) {
        for (int j = 0; j < sizes[i]; j++) {
            for (int k = 0; k < sizes[i]; k++) {
                fprintf(initial, "%d ", matrices[i][j][k]);
            }
            fprintf(initial, "\n");
        }
        fprintf(initial, "\n");
    }

    int choice = 0;
    printf("1. Heap sort\n2. Selection sort\nChoose sort: ");
    scanf("%d", &choice);

    int runs_amount = 0;
    printf("Enter the number of runs: ");
    scanf("%d", &runs_amount);
    printf("\n");

    __int64 start, end;
    int *tics = (__int64 *)malloc(runs_amount * sizeof(__int64));

    for (int i = 0; i < runs_amount; i++) {
        start = __rdtsc();

        switch (choice) {
        case 1: 
            heap_sort(determinants, matrices_amount, indexes);
            break;
        case 2:
            selection_sort(determinants, matrices_amount);
            break;
        default:
            printf("Missing sort");
            return 0;
        }
  
        end = __rdtsc();

        tics[i] = end - start;
        fprintf(output, "%d. %d\n", i + 1, tics[i]);

        for (int j = 0; j < matrices_amount; j++) {
            determinants[j] = determinants_copy[j];
        }
    }
    fprintf(output, "\n");

    int mean = sample_mean(tics, runs_amount);
    int best_deviation = abs(mean - tics[0]);
    int worst_deviation = abs(mean - tics[0]);
    int best_index = 0;
    int worst_index = 0;

    for (int i = 0; i < runs_amount; i++) {
        int deviation = abs(mean - tics[i]);

        if (deviation < best_deviation) {
            best_deviation = deviation;
            best_index = i;
        }

        if (deviation > worst_deviation) {
            worst_deviation = deviation;
            worst_index = i;
        }
    }

    fprintf(output, "Sample mean: %lf\n", sample_mean(tics, runs_amount));
    fprintf(output, "Sample variance: %lf\n", sample_variance(tics, runs_amount));
    fprintf(output, "\n");
    fprintf(output, "Best run: %d. %d\n", best_index + 1, tics[best_index]);
    fprintf(output, "Worst run: %d. %d", worst_index + 1, tics[worst_index]);

    switch (choice) {
    case 1:
        heap_sort(determinants, matrices_amount, indexes);
        break;
    case 2:
        selection_sort(determinants, matrices_amount);
        break;
    }

    for (int i = 0; i < matrices_amount; i++) {
        for (int j = 0; j < sizes[indexes[i]]; j++) {
            for (int k = 0; k < sizes[indexes[i]]; k++) {
                fprintf(sorted, "%d ", matrices[indexes[i]][j][k]);
            }
            fprintf(sorted, "\n");
        }
        fprintf(sorted, "\n");
    }

    for (int i = 0; i < 64; i++) {
        free(result_matrix[i]);
    }
    free(result_matrix);

    for (int i = 0; i < matrices_amount; i++) {
        for (int j = 0; j < 64; j++) {
            free(matrices[i][j]);
        }
        free(matrices[i]);
    }
    free(matrices);

    free(determinants);
    free(determinants_copy);
    free(tics);
    free(sizes);
    free(indexes);
}