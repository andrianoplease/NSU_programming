#define _CRT_SECURE_NO_WARNINGS
#define MAX_IDS_AMOUNT 255
#define MAX_EXPR_LENGTH 256

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_number(char *);
int is_id(char *);
int is_sign(char *);
int is_simple(char *);
char *replace_ids(char *, char **, char **, int);
int is_computable(char *);
double compute(char *);
int digits_count(int);

int main() {
	FILE *input, *output;
	input = fopen("input.txt", "r");
	if (input == NULL) {
		printf("Can't open file input.txt");
		return 1;
	}

	char *ids[MAX_IDS_AMOUNT];
	char *values[MAX_IDS_AMOUNT];
	char expr[MAX_EXPR_LENGTH];
	int ids_amount = 0;

	fscanf(input, "%d", &ids_amount);
	for (int i = 0; i < ids_amount; i++) {
		fscanf(input, "%s %s", ids + i, values + i);
	}
	fscanf(input, "%s", expr);
	fclose(input);

	output = fopen("output.txt", "w");
	if (output == NULL) {
		printf("Can't open file output.txt");
		return 1;
	}

	int expr_len = strlen(expr);

	if (is_simple(expr) == expr_len) {
		replace_ids(expr, ids, values, ids_amount);

		if (is_computable(expr)) {
			fprintf(output, "%.1lf", compute(expr));
		}
		else {
			fprintf(output, "%s", expr);
		}
	}
	else {
		fprintf(output, "INCORRECT");
	}

	fclose(output);
	return 0;
}

int is_number(char *expr) {
	int char_count = 0;

	while (*expr && is_sign(expr) == 0 && *expr != ')') {
		if (isdigit(*expr)) {
			expr++;
			char_count++;
			continue;
		}
		else {
			return 0;
		}
	}
	return char_count;
}

int is_id(char *expr) {
	int char_count = 0;

	while (*expr && is_sign(expr) == 0 && *expr != ')') {
		if (isalpha(*expr) || isdigit(*expr) || *expr == '_') {
			expr++;
			char_count++;
			continue;
		}
		else {
			return 0;
		}
	}
	return char_count;
}

int is_sign(char *expr) {
	if (*expr == '+' ||
		*expr == '-' ||
		*expr == '*' ||
		*expr == '/') {
		return 1;
	}
	return 0;
}

int is_simple(char *expr) {
	int shift = 0;

	if (isdigit(*expr)) {
		int shift_number = is_number(expr);
		expr += shift_number;

		if (shift_number == 0) {
			return 0;
		}
		else {
			shift += shift_number;
		}
	}
	else if (isalpha(*expr)) {
		int shift_id = is_id(expr);
		expr += shift_id;

		if (shift_id == 0) {
			return 0;
		}
		else {
			shift += shift_id;
		}
	}
	else if (*expr == '(') {
		expr++;
		int shift_left = is_simple(expr);

		if (shift_left == 0) {
			return 0;
		}

		expr += shift_left;

		if (is_sign(expr)) {
			expr++;
		}
		else {
			return 0;
		}

		int shift_right = is_simple(expr);

		if (shift_right == 0) {
			return 0;
		}

		expr += shift_right;

		shift += shift_left + shift_right + 3;

		if (*expr == ')') {
			return shift;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

char *replace_ids(char *expr, char **ids, char **values, int ids_amount) {
	int expr_len = strlen(expr);

	for (int i = 0; i < ids_amount; i++) {
		char *substr = strstr(expr, ids);

		if (substr == NULL) {
			continue;
		}

		while (1) {
			substr = strstr(substr, ids);

			if (substr == NULL) {
				break;
			}

			int id_len = strlen(ids);
			int value_len = strlen(values);

			if (isdigit(*(substr - 1))      == 0 &&
				isalpha(*(substr - 1))      == 0 &&
				isalpha(*(substr + id_len)) == 0 &&
				isdigit(*(substr + id_len)) == 0) {

				if (id_len != value_len) {
					memcpy(substr + value_len, substr + id_len, strlen(substr));
				}

				memcpy(substr, values, value_len);
			}
			else {
				substr += is_id(substr);
			}
		}

		ids++;
		values++;
	}
	return expr;
}

int is_computable(char *expr) {
	int expr_len = strlen(expr);

	for (int i = 0; i < expr_len; i++) {
		if (isalpha(*(expr + i))) {
			return 0;;
		}
	}
	return 1;
}

double compute(char *computable_expr) {
	double values[MAX_EXPR_LENGTH];
	char operations[MAX_EXPR_LENGTH];
	int op_iter = 0;
	int val_iter = 0;
	char *substr = operations;
	int computable_expr_len = strlen(computable_expr);

	for (int i = 0; i < computable_expr_len; i++) {
		if (isdigit(*(computable_expr + i))) {
			int value = atoi(computable_expr + i);
			int len = digits_count(value);

			*(values + val_iter) = value;
			val_iter++;
			i += len;
		}

		if (is_sign(computable_expr + i)) {

			*(operations + op_iter) = *(computable_expr + i);
			op_iter++;
		}

		if (*(computable_expr + i) == '(') {
			*(operations + op_iter) = '(';
			op_iter++;
		}
		else if (*(computable_expr + i) == ')') {
			while (*(operations + op_iter - 1) != '(') {
				switch (*(operations + op_iter - 1)) {
				case '+':
					*(values + val_iter - 2) += *(values + val_iter - 1);
					val_iter--;
					break;
				case '-':
					*(values + val_iter - 2) -= *(values + val_iter - 1);
					val_iter--;
					break;
				case '*':
					*(values + val_iter - 2) *= *(values + val_iter - 1);
					val_iter--;
					break;
				case '/':
					*(values + val_iter - 2) /= *(values + val_iter - 1);
					val_iter--;
					break;
				}
				op_iter--;
			}

			memcpy(substr + op_iter - 1, substr + op_iter, sizeof(operations));
			op_iter--;
		}
	}
	return *values;
}

int digits_count(int number) {
	int i = 0;
	while (number > 0) {
		number /= 10;
		i++;
	}
	return i;
}