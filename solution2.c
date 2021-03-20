#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINES_AMOUNT 255
#define MAX_EXPRESSION_LENGTH 256

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char *has_first_bracket(char *, int);
int is_simple_expression(char *, int, int, int, int);
char *replace_variables(char *, char **, char **, int);
double calculate_expression(char *);
int digits_count(int);

int main() {
	FILE *input, *output;
	input = fopen("input.txt", "r");
	if (input == NULL) {
		printf("Can't open file input.txt");
		return 1;
	}

	int lines_amount = 0;
	fscanf(input, "%d", &lines_amount);

	char *variables[MAX_LINES_AMOUNT];
	char *values[MAX_LINES_AMOUNT];
	for (int i = 0; i < lines_amount; i++) {
		fscanf(input, "%s %s", (char *)(variables + i), (char *)(values + i));
	}

	char expression[MAX_EXPRESSION_LENGTH];
	fscanf(input, "%s", expression);
	fclose(input);

	output = fopen("output.txt", "w");
	if (output == NULL) {
		printf("Can't open file output.txt");
		return 1;
	}

	int expression_length = strlen(expression);

	if (has_first_bracket(expression, expression_length) == "YES") {
		if (is_simple_expression(expression, expression_length, 0, 0, 0) == 1) {
			replace_variables(expression, variables, values, lines_amount);

			int i = 0;
			for (; i < expression_length; i++) {
				if (isalpha(expression[i])) {
					break;
				}
			}

			if (i == expression_length) {
				fprintf(output, "%lf\n", calculate_expression(expression));
				fprintf(output, "%s", expression);
			}
			else {
				fprintf(output, "%s", expression);
			}
		}
		else {
			fprintf(output, "INCORRECT");
		}
	}
	else if (has_first_bracket(expression, expression_length) == "INCORRECT") {
		fprintf(output, "INCORRECT");
	}
	else {
		replace_variables(expression, variables, values, lines_amount);
		fprintf(output, "%s", expression);
	}

	fclose(output);
	return 0;
}

char *has_first_bracket(char *expression, int expression_length) {
	if (*expression == '(') {
		return "YES";
	}
	else if(isdigit(*expression)){
		if (digits_count(atoi(expression)) == expression_length) {
			return expression;
		}
		else {
			return "INCORRECT";
		}
	}
	else if (isalpha(*expression)) {
		int count = 0;
		for (int i = 0; i < expression_length; i++) {
			if (isalpha(expression[i]) || isdigit(expression[i]) || expression[i] == '_') {
				count++;
			}
		}
		if (count == expression_length) {
			return expression;
		}
		else {
			return "INCORRECT";
		}
	}
}

int is_simple_expression(char *expression, int expression_length, int open_brackets_count, int close_brackets_count, int signs_count) {
	for (int i = 0; i < expression_length; i++) {
		if (expression[i] == '(') {	
			open_brackets_count++;
			continue;
		}
		if (isdigit(expression[i])) {
			i += digits_count(atoi(expression + i));
		}
		else if (isalpha(expression[i])) {
			while(isalpha(expression[i]) || isdigit(expression[i]) || expression[i] == '_') {
					i++;
			}
		}
		else {
			return 0;
		}

		if (expression[i] == ')') {
			close_brackets_count++;
				while (i != expression_length - 1 && expression[i] == ')') {
					i++;
					if (expression[i] == ')') {
						close_brackets_count++;
					}
				}
				if (i == expression_length - 1 && open_brackets_count == close_brackets_count && close_brackets_count == signs_count) {
					return 1;
				}
		}
		if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
			signs_count++;
			return is_simple_expression(expression + i + 1, expression_length - 1 - i, open_brackets_count, close_brackets_count, signs_count);
		}
		else {
			return 0;
		}
	}
}

char *replace_variables(char *expression, char **variables, char **values, int lines_amount) {
	int expression_length = strlen(expression);

	if (expression[0] != '(') {
		for (int j = 0; j < lines_amount; j++) {
			char *substr = strstr(expression, (char *)(variables + j));
			int variable_length = strlen((char *)(variables + j));

			if (substr != NULL && expression_length == variable_length) {
				memcpy(expression, (char *)(values + j), sizeof(expression));
				return expression;
			}
			else {
				return expression;
			}
		}
	}

	for (int i = 0; i < lines_amount; i++) {
		char *substr = strstr(expression, (char *)(variables));
		
		while (substr != NULL) {
			substr = strstr(substr, (char *)(variables));
			if (substr == NULL) {
				break;
			}

			int len1 = strlen((char *)(variables));
			int len2 = strlen((char *)(values));

			if (isdigit(*(substr - 1)) == 0 && isalpha(*(substr - 1)) == 0 && isalpha(*(substr + len1)) == 0 && isdigit(*(substr + len1)) == 0 && *(substr + len1) != '_') {
				if (len1 != len2) {
					memcpy(substr + len2, substr + len1, strlen(substr));
				}
				memcpy(substr, values, len2);
			}
			else {
				substr++;
			}
		}
		variables++;
		values++;
	}
	return expression;
}

double calculate_expression(char *computable_expression) {
	double values[MAX_EXPRESSION_LENGTH];
	char operations[MAX_EXPRESSION_LENGTH];
	int operations_iterator = 0;
	int values_iterator = 0;
	char *substr = operations;
	int computable_expression_length = strlen(computable_expression);

	for (int i = 0; i < computable_expression_length; i++) {
		if (isdigit(computable_expression[i])) {	
			int value = atoi(computable_expression + i);
			int len = digits_count(value);

			values[values_iterator] = value;
			values_iterator++;
			i += len;
		}
		
		if (computable_expression[i] == '+' || computable_expression[i] == '-' || computable_expression[i] == '*' || computable_expression[i] == '/') {
			operations[operations_iterator] = computable_expression[i];
			operations_iterator++;
		}

		if (computable_expression[i] == '(') {
			operations[operations_iterator] = '(';
			operations_iterator++;
		} else if (computable_expression[i] == ')') {
			while (operations[operations_iterator - 1] != '(') {
				switch (operations[operations_iterator - 1]) {
				case '+':
					values[values_iterator - 2] += values[values_iterator - 1];
					values_iterator--;
					break;
				case '-':
					values[values_iterator - 2] -= values[values_iterator - 1];
					values_iterator--;
					break;
				case '*':
					values[values_iterator - 2] *= values[values_iterator - 1];
					values_iterator--;
					break;
				case '/':
					values[operations_iterator - 2] /= values[operations_iterator - 1];
					values_iterator--;
					break;
				}
				operations_iterator--;
			}
			memcpy(substr + operations_iterator - 1, substr + operations_iterator, sizeof(operations));
			operations_iterator--;
		}
	}
	return values[0];
}

int digits_count(int number) {
	int i = 0;
	while (number > 0) {
		number /= 10;
		i++;
	}
	return i;
}