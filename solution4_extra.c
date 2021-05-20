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
    int points[AMOUNT_OF_POINTS];
} Student;

typedef struct Node_ {
    struct Node_ *next;
    Student *student;
} Node;

typedef struct Tnode_ {
    struct Tnode_ *left;
    struct Tnode_ *right;
    struct Tnode_ *parent;
    int height;
    int key;
    Node *val;
} Tnode;

Node *find_and_pop_from_list(Node **, int);
Student *find_and_pop_from_tree(Tnode *, int);
int list_size(Node *);
Tnode *add_to_tree1(Tnode *, Student *, int);
Tnode *add_to_tree(Tnode *, Student *);
void destroy_list(Node *);
void destroy_tree(Tnode *);
Tnode *cut(Tnode *, int);
int tree_size(Tnode *);
int height(Tnode *);
int balance_factor(Tnode *);
void fix_height(Tnode *);
Tnode *rotate_right(Tnode *);
Tnode *rotate_left(Tnode *);
Tnode *balance(Tnode *);
void delete_Tnode(Tnode **);
Tnode *find_max_Tnode(Tnode *);

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
        if (!s) exit(-1);
        s->number = atoi(token);
        for (int i = 0; i < AMOUNT_OF_POINTS; i++) {
            token = strtok(NULL, " ");
            s->points[i] = atoi(token);
        }
        t = add_to_tree(t, s);
        fgets(string, MAXN, input);
    }

    fgets(string, MAXN, input);
    while (!isalpha(string[0])) {
        int n = 0;
        int id = 0;
        int p = 0;
        sscanf(string, "%d %d %d", &id, &n, &p);
        Student *s = find_and_pop_from_tree(t, id);
        s->points[n] = p;
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

int height(Tnode *t) {
    return t ? t->height : 0;
}

int balance_factor(Tnode *t) {
    return height(t->right) - height(t->left);
}

void fix_height(Tnode *t) {
    int height_left = height(t->left);
    int height_right = height(t->right);
    t->height = (height_left > height_right ? height_left : height_right) + 1;
}

Tnode *rotate_right(Tnode *t) {
    Tnode *left = t->left;
    t->left = left->right;
    left->right = t;
    fix_height(t);
    fix_height(left);
    return left;
}

Tnode *rotate_left(Tnode *t) {
    Tnode *right = t->right;
    t->right = right->left;
    right->left = t;
    fix_height(t);
    fix_height(right);
    return right;
}

Tnode *balance(Tnode *t) {
    fix_height(t);
    if (balance_factor(t) == 2) {
        if (balance_factor(t->right) < 0) {
            t->right = rotate_right(t->right);
        }
        return rotate_left(t);
    }
    if (balance_factor(t) == -2) {
        if (balance_factor(t->left) > 0) {
            t->left = rotate_left(t->left);
        }
        return rotate_right(t);
    }
    return t;
}

void delete_Tnode(Tnode **t) {
    Tnode *left = (*t)->left;
    Tnode *right = (*t)->right;
    Tnode *parent = (*t)->parent;

    if (left && right) {
        Tnode *local_max = find_max_Tnode(left);
        (*t)->key = local_max->key;
        delete_Tnode(&local_max);
    }
    else if (left) {
        if (*t == parent->left) {
            balance(*t);
            free(*t);
            parent->left = left;
            left->parent = parent;
        }
        else {
            balance(*t);
            free(*t);
            parent->right = left;
            left->parent = parent;
        }
    }
    else if (right) {
        if ((*t) == parent->left) {
            balance(*t);
            free(*t);
            parent->left = right;
            right->parent = parent;
        }
        else {
            if ((*t) == parent->left) {
                balance(*t);
                free(*t);
                parent->left = NULL;
            }
            else {
                balance(*t);
                free(*t);
                parent->right = NULL;
            }
        }
    }
    else {
        if ((*t) == parent->left) {
            balance(*t);
            free(*t);
            parent->left = NULL;
        }
        else {
            balance(*t);
            free(*t);
            parent->right = NULL;
        }
    }
}

Tnode *find_max_Tnode(Tnode *t) {
    while (t->right) {
        t = t->right;
    }
    return t;
}

Node *add_to_list(Node *l, Student *student) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) exit(-1);
    new_node->next = l;
    new_node->student = student;
    return new_node;
}

Node *find_and_pop_from_list(Node **l, int student_id) {
    Node **v = l;
    while (*v) {
        if ((*v)->student->number == student_id) {
            Node *res = *v;
            *v = (*v)->next;
            return res;
        }
        v = &((*v)->next);
    }
    return NULL;
}

Student *find_and_pop_from_tree(Tnode *t, int student_id) {
    if (!t) return NULL;
    Node *l = find_and_pop_from_list(&(t->val), student_id);
    if (l) {
        if (t->val == NULL) {
            delete_Tnode(&t);
        }
        Student *s = l->student;
        free(l);
        return s;
    }
    Student *s = find_and_pop_from_tree(t->left, student_id);
    if (s) return s;
    return find_and_pop_from_tree(t->right, student_id);
}

int list_size(Node *l) {
    if (!l) return 0;
    return 1 + list_size(l->next);
}

Tnode *add_to_tree1(Tnode *t, Student *s, int sum) {
    if (!t) {
        Tnode *new_node = (Tnode *)malloc(sizeof(Tnode));
        Node *l = add_to_list(NULL, s);
        if (!new_node || !l) exit(-1);
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->parent = NULL;
        new_node->key = sum;
        new_node->val = l;
        new_node->height = 0;
        return new_node;
    }
    if (sum == t->key) {
        t->val = add_to_list(t->val, s);
    }
    else if (sum < t->key) {
        t->left = add_to_tree1(t->left, s, sum);
        t->left->parent = t;
    }
    else {
        t->right = add_to_tree1(t->right, s, sum);
        t->right->parent = t;
    }
    return balance(t);
}

Tnode *add_to_tree(Tnode *t, Student *s) {
    int sum = 0;
    for (int i = 0; i < AMOUNT_OF_POINTS; ++i) {
        sum += s->points[i];
    }
    return add_to_tree1(t, s, sum);
}

void destroy_list(Node *l) {
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