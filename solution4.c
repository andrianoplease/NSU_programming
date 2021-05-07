#define _CRT_SECURE_NO_WARNINGS
#define AMOUNT_OF_POINTS 10
#define MAXN 100

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

typedef struct Student_ {
    int number;
    int point[AMOUNT_OF_POINTS];
} Student;

typedef struct List_ {
    struct List_ *next;
    Student *student;
} List;

typedef struct Tnode_ {
    struct Tnode_ *left;
    struct Tnode_ *right;
    int key;
    List *val;
} Tnode;

List *find_and_pop_list(List **, int);
Student *find_and_pop_tree(Tnode *, int);
int list_size(List *);
Tnode *add_to_tree1(Tnode *, Student *, int);
Tnode *add_to_tree(Tnode *, Student *);
void destroy_list(List *);
void destroy_tree(Tnode *);
Tnode *cut(Tnode *, int);
int tree_size(Tnode *);

int main() {
    FILE *input, *output;
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

    Tnode *t = NULL;
    char string[MAXN];
    
    fgets(string, MAXN, input);
    fgets(string, MAXN, input);
    while (!isalpha(string[0])) {
        char *token = strtok(string, " ");
        Student *s = (Student *)malloc(sizeof(Student));
        if (!s) exit (-1);
        s->number = atoi(token);
        for (int i = 0; i < AMOUNT_OF_POINTS; i++) {
            token = strtok(NULL, " ");
            s->point[i] = atoi(token);
        }
        t = add_to_tree(t, s);
        fgets(string, MAXN, input);
    }

    fgets(string, MAXN, input);
    while (!isalpha(string[0])) {
        int n = 0;
        int id = 0;
        int p = 0;
        sscanf(string, "%d %d %d", &n, &id, &p);
        Student *s = find_and_pop_tree(t, n);
        s->point[id] = p;
        t = add_to_tree(t, s);
        fgets(string, MAXN, input);
    }

    double k = 0;
    double p = 0;
    int size1 = 0;
    int size2 = 0;

    fscanf(input, "%lf %lf", &k, &p);
    t = cut(t, lround(10 * k));
    size1 = tree_size(t);

    t = cut(t, lround(10 * p));
    size2 = tree_size(t);

    fprintf(output, "%d %d", size1, size2);
    destroy_tree(t);
    fclose(input);
    fclose(output);
    return 0;
}

List *add_to_list(List *l, Student *student) {
    List *new_node = (List *)malloc(sizeof(List));
    if (!new_node) exit(-1);
    new_node->next = l;
    new_node->student = student;
    return new_node;
}

List *find_and_pop_list(List **l, int student_id) {
    List **v = l;
    while (*v) {
        if ((*v)->student->number == student_id) {
            List *res = *v;
            *v = (*v)->next;
            return res;
        }
        v = &((*v)->next);
    }
    return NULL;
}

Student *find_and_pop_tree(Tnode *t, int student_id) {
    if (!t) return NULL;
    List *l = find_and_pop_list(&(t->val), student_id);
    if (l) {
        Student *s = l->student;
        free(l);
        return s;
    }
    Student *s = find_and_pop_tree(t->left, student_id);
    if (s) return s;
    return find_and_pop_tree(t->right, student_id);
}

int list_size(List *l) {
    if (!l) return 0;
    return 1 + list_size(l->next);
}

Tnode *add_to_tree1(Tnode *t, Student *s, int sum) {
    if (!t) {
        Tnode *new_node = (Tnode *)malloc(sizeof(Tnode));
        List *l = add_to_list(NULL, s);
        if (!new_node || !l) exit(-1); 
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->key = sum;
        new_node->val = l;
        return new_node;
    }
    if (sum == t->key) {
        t->val = add_to_list(t->val, s);
    }
    else if (sum < t->key) {
        t->left = add_to_tree1(t->left, s, sum);
    }
    else {
        t->right = add_to_tree1(t->right, s, sum);
    }
    return t;
}

Tnode *add_to_tree(Tnode *t, Student *s) {
    int sum = 0;
    for (int i = 0; i < AMOUNT_OF_POINTS; ++i) {
        sum += s->point[i];
    }
    return add_to_tree1(t, s, sum);
}

void destroy_list(List *l) {
    if (!l) return;
    destroy_list(l->next);
    free(l->student);
    free(l);
}

void destroy_tree(Tnode *t) {
    if (!t) return;
    destroy_tree(t->left);
    destroy_tree(t->right);
    destroy_list(t->val);
}

Tnode *cut(Tnode *t, int slice) {
    if (!t) return NULL;
    if (t->key < slice) {
        Tnode *right = t->right;
        t->right = NULL;
        destroy_tree(t);
        return cut(right, slice);
    }
    t->left = cut(t->left, slice);
    return t;
}

int tree_size(Tnode *t) {
    if (!t) return 0;
    int l = tree_size(t->left);
    int r = tree_size(t->right);
    int m = list_size(t->val);
    return l + r + m;
}