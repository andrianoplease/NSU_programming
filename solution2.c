#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINES_AMOUNT 255
#define MAX_EXPRESSION_LENGTH 256

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_simple_expression(char*, int, int, int);
char *replace_identifiers(char*, char**, char**, int);

int main() {
	FILE *input, *output;
	input = fopen("input.txt", "r");
	if (input== NULL) {
		printf("Can't open file input.txt");
		return 1;
	}

	int lines_amount = 0;
	fscanf(input, "%d", &lines_amount);

	char *identifiers[MAX_LINES_AMOUNT];
	char *values[MAX_LINES_AMOUNT];
	for (int i = 0; i < lines_amount; i++) {
		fscanf(input, "%s %s", (char*)(identifiers + i), (char*)(values + i));
	}

	char expression[MAX_EXPRESSION_LENGTH];
	fscanf(input, "%s", expression);
	fclose(input);

	output = fopen("output.txt", "w");
	if (output == NULL) {
		printf("Can't open file output.txt");
		return 1;
	}

	if (is_simple_expression(expression, 0, 0, 0) == 1) {
		fprintf(output, "%s", replace_identifiers(expression, identifiers, values, lines_amount));
	}
	else {
		fprintf(output, "INCORRECT");
	}
	fclose(output);
	return 0;
}

int is_simple_expression(char *expression, int open_brackets_count, int close_brackets_count, int signs_count) {
	int tail_length = 0;
	
	if(expression[0] == '(') {
		open_brackets_count++;
		tail_length = strlen(expression);

		if (tail_length == 1) {
			return 0;
		}
		else {
			for (int i = 1; i < tail_length; i++) {
				if (expression[i] == '(') {
					open_brackets_count++;
					if (i == tail_length - 1) {
						return 0;
					}
					continue;
				}
				else {
					return is_simple_expression(expression + i, open_brackets_count, close_brackets_count, signs_count);
				}
			}
		}
	}
	else if (expression[0] == ')') {
		close_brackets_count++;
		tail_length = strlen(expression);

		if (tail_length == 1) {
			if (open_brackets_count == close_brackets_count && open_brackets_count == signs_count) {
				return 1;
			}
			else {
				return 0;
			}
		}
		else {
			for (int i = 1; i < tail_length; i++) {
				if (expression[i] == ')') {
					close_brackets_count++;
					if (i == tail_length - 1) {
						if (open_brackets_count == close_brackets_count &&  open_brackets_count == signs_count) {
							return 1;
						}
						else {
							return 0;
						}
					}
					continue;
				}
				else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
					signs_count++;
					if (tail_length - i == 1) {
						return 0;
					}
					else {
						return is_simple_expression(expression + 1 + i, open_brackets_count, close_brackets_count, signs_count);
					}
				}
				else {
					return 0;
				}
			}
		}
	}
	else if (isdigit(expression[0])) {
		tail_length = strlen(expression);

		if (tail_length == 1) {
			return 0;
		}
		else {
			for (int i = 1; i < tail_length; i++) {
				if (isdigit(expression[i])) {
					if (i == tail_length - 1) {
						if (open_brackets_count == 0 && close_brackets_count == 0 && signs_count == 0) {
							return 1;
						}
						else {
							return 0;
						}
					}
					continue;
				}
				else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
					signs_count++;
					if (tail_length - i == 1 || expression[i + 1] == ')') {
						return 0;
					}
					else {
						return is_simple_expression(expression + 1 + i, open_brackets_count, close_brackets_count, signs_count);
					}
				}
				else {
					return is_simple_expression(expression + i, open_brackets_count, close_brackets_count, signs_count);
				}
			}
		}
	}
	else if (isalpha(expression[0])) {
		tail_length = strlen(expression);

		if (tail_length == 1) {
			return 0;
		}
		else {
			for (int i = 1; i < tail_length; i++) {
				if (isalpha(expression[i]) || isdigit(expression[i]) || expression[i] == '_') {				
					if (i == tail_length - 1) {
						if (open_brackets_count == 0 && close_brackets_count == 0 && signs_count == 0) {
							return 1;
						}
						else {
							return 0;
						}
					}
					continue;
				}
				else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
					signs_count++;
					if (tail_length - i == 1) {
						return 0;
					}
					else {
						return is_simple_expression(expression + 1 + i, open_brackets_count, close_brackets_count, signs_count);
					}
				}
				else if (expression[i] == ')'){
					return is_simple_expression(expression + i, open_brackets_count, close_brackets_count, signs_count);
				}
				else {
					return 0;
				}
			}
		}
	}
	else {
		return 0;
	}
	return 0;
}

char *replace_identifiers(char *expression, char **identifiers, char **values, int lines_amount) {
	for (int i = 0; i < lines_amount; i++) {
		if(expression[0] != '('){
			break;
		}

		char* substr = strstr(expression, (char*)(identifiers + i));
		while (substr != NULL) {
			if (isdigit(*(substr - 1)) == 0 && isalpha(*(substr - 1)) == 0) {
				int len1 = strlen((char*)(identifiers));
				int len2 = strlen((char*)(values + i));
				if (len1 != len2) {
					memmove(substr + len2, substr + len1, strlen(substr + len1) + 1);
				}
				memcpy(substr, values + i, len2);
				substr = strstr(expression, (char*)(identifiers + i));
			}
		}
	}
	return expression;
}