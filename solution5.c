#define MAXN 100
#define MAX 10000000

#include <stdio.h>

void min_dist(int A[][MAXN], int, int);

int main() {
	FILE *input;
	input = fopen("input.txt", "r");
	if (!input) {
		printf("Can't open file input.txt");
		return 1;
	}

	int A[MAXN][MAXN];
	int K = 0;
	int N = 0;
	int M = 0;
	int from = 0;
	int to = 0;
	int time = 0;

	fscanf(input, "%d %d %d", &K, &N, &M);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			A[i][j] = MAX;
		}
	}

	for (int i = 0; i < M; i++) {
		fscanf(input, "%d %d %d", &from, &to, &time);
		A[from][to] = time;
	}

	min_dist(A, N, K);

	fclose(input);
	return 0;
}

void min_dist(int adj_matrix[][MAXN], int vertices_num, int treasure_island) {
	int visited[MAXN] = { 0 };
	int mark[MAXN];
	int p[MAXN];
	int dist = 0;

	mark[0] = 0;
	for (int i = 1; i < vertices_num; i++) {
		mark[i] = MAX;
	}

	int min_mark = 0;
	int min_mark_vertex = 0;

	for (int i = 0; i < vertices_num; i++) {
		min_mark = MAX;
		min_mark_vertex = -1;

		for (int j = 0; j < vertices_num; j++) {
			if (!visited[j] && mark[j] < min_mark) {
				min_mark = mark[j];
				min_mark_vertex = j;
			}
		}

		for (int k = 0; k < vertices_num; k++) {
			dist = mark[min_mark_vertex] + adj_matrix[min_mark_vertex][k];

			if (dist < mark[k]) {
				mark[k] = dist;
				p[k] = min_mark_vertex;
			}
		}

		visited[min_mark_vertex] = 1;
	}

	printf("%d\n", mark[treasure_island]);

	int reverse[MAXN];
	int i = 0;
	int k = treasure_island;

	while (k) {
		reverse[i] = k;
		k = p[k];
		i++;
	}
	reverse[i] = 0;


	while (i >= 0) {
		printf("%d ", reverse[i]);
		i--;
	}
}