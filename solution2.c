#define _CRT_SECURE_NO_WARNINGS
#define MAX_IDS_AMOUNT 255
#define MAX_EXPR_LENGTH 256

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_single_id(char *);
int is_single_number(char *);
int is_sign(char *);
int is_simple(char *, int, int, int);
char *replace_ids(char *, char **, char **, int);
double calculate(char *);
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

	int expr_length = strlen(expr);
	
	if (is_single_number(expr)) {
		fprintf(output, "%s", expr);
	}
	else if (is_single_id(expr)) {
		fprintf(output, "%s", replace_ids(expr, ids, values, ids_amount));
	}
	else if (is_simple(expr, 0, 0, 0) == 1) {
		replace_ids(expr, ids, values, ids_amount);

		int i = 0;
		for (; i < expr_length; i++) {
			if (isalpha(*(expr + i))) {
				break;
			}
		}

		if (i == expr_length) {
			fprintf(output, "%s = %.1lf", expr, calculate(expr));
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


int is_sign(char *expr) {
	if (*expr == '+' || *expr == '-' || *expr == '*' || *expr == '/') {
		return 1;
	}
	return 0;
}


int is_single_id(char *expr) {
	if (isalpha(*expr)) {
		while (*expr) {
			if (isalpha(*expr) || isdigit(*expr) || *expr == '_') {
				expr++;
				continue;
			}
			else {
				return 0;
			}
		}
		return 1;
	}
	else {
		return 0;
	}
}


int is_single_number(char *expr) {
	while (*expr) {
		if (isdigit(*expr)) {
			expr++;
			continue;
		}
		else {
			return 0;
		}
	}
	return 1;
}


int is_simple(char *expr, int opens, int closes, int signs) {
	int current_length = strlen(expr);

	if (*expr == '(') {
		opens++;

		if (current_length == 1) {
			return 0;
		}
		else {
			for (int i = 1; i < current_length; i++) {
				if (*(expr + i) == '(') {
					opens++;

					if (i == current_length - 1) {
						return 0;
					}

					continue;
				}
				else {
					return is_simple(expr + i, opens, closes, signs);
				}
			}
		}
	}
	else if (*expr == ')') {
		closes++;

		if (current_length == 1) {
			if (opens == closes && opens == signs) {
				return 1;
			}
			else {
				return 0;
			}
		}
		else {
			for (int i = 1; i < current_length; i++) {
				if (*(expr + i) == ')') {
					closes++;

					if (i == current_length - 1) {
						if (opens == closes && opens == signs) {
							return 1;
						}
						else {
							return 0;
						}
					}

					continue;
				}
				else if (is_sign((expr + i))) {
					signs++;

					if (current_length - i == 1) {
						return 0;
					}
					else {
						return is_simple(expr + 1 + i, opens, closes, signs);
					}
				}
				else {
					return 0;
				}
			}
		}
	}
	else if (isdigit(*expr)) {
		if (current_length == 1) {
			return 0;
		}
		else {
			for (int i = 1; i < current_length; i++) {
				if (isdigit(*(expr + i))) {
					if (i == current_length - 1) {
						if (opens == 0 && closes == 0 && signs == 0) {
							return 1;
						}
						else {
							return 0;
						}
					}

					continue;
				}
				else if (is_sign((expr + i))) {
					signs++;

					if (current_length - i == 1 || expr[i + 1] == ')') {
						return 0;
					}
					else {
						return is_simple(expr + 1 + i, opens, closes, signs);
					}
				}
				else if(*(expr + i) == ')') {
					return is_simple(expr + i, opens, closes, signs);
				}
				else {
					return 0;
				}
			}
		}
	}
	else if (isalpha(*expr)) {
		if (current_length == 1) {
			return 0;
		}
		else {
			for (int i = 1; i < current_length; i++) {
				if (isalpha(*(expr + i)) || isdigit(*(expr + i)) || *(expr + i) == '_') {
					if (i == current_length - 1) {
						if (opens == 0 && closes == 0 && signs == 0) {
							return 1;
						}
						else {
							return 0;
						}
					}

					continue;
				}
				else if (is_sign((expr + i))) {
					signs++;

					if (current_length - i == 1) {
						return 0;
					}
					else {
						return is_simple(expr + 1 + i, opens, closes, signs);
					}
				}
				else if (*(expr + i) == ')') {
					return is_simple(expr + i, opens, closes, signs);
				}
				return 0;
			}
		}
	}
	return 0;
}


char *replace_ids(char *expr, char **ids, char **values, int ids_amount) {
	int expr_length = strlen(expr);

	if (isalpha(*expr)) {
		for (int i = 0; i < ids_amount; i++) {
			char *substr = strstr(expr, ids);

			if (substr == NULL) {
				continue;
			}
		
			substr = strstr(substr, ids);

			int id_length = strlen(ids);
			int value_length = strlen(values);

			if (id_length == expr_length) {

				if (id_length != value_length) {
					memcpy(substr + value_length, substr + id_length, strlen(substr));
				}

				memcpy(substr, values, value_length);
			}
			

			ids++;
			values++;;
		}
	}
	else {
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

				int id_length = strlen(ids);
				int value_length = strlen(values);

				if (isdigit(*(substr - 1)) == 0 &&
					isalpha(*(substr - 1)) == 0 &&
					isalpha(*(substr + id_length)) == 0 &&
					isdigit(*(substr + id_length)) == 0) {

					if (id_length != value_length) {
						memcpy(substr + value_length, substr + id_length, strlen(substr));
					}

					memcpy(substr, values, value_length);
				}
				else {
					substr++;
				}
			}

			ids++;
			values++;
		}
	}
	return expr;
}

double calculate(char *computable_expr) {
	double values[MAX_EXPR_LENGTH];
	char operations[MAX_EXPR_LENGTH];
	int op_iter = 0;
	int val_iter = 0;
	char *substr = operations;
	int computable_expr_length = strlen(computable_expr);

	for (int i = 0; i < computable_expr_length; i++) {
		if (isdigit(*(computable_expr + i))) {
			int value = atoi(computable_expr + i);
			int len = digits_count(value);

			*(values + val_iter) = value;
			val_iter++;
			i += len;
		}

		if (*(computable_expr + i) == '+' ||
			*(computable_expr + i) == '-' ||
			*(computable_expr + i) == '*' ||
			*(computable_expr + i) == '/'   ) {

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