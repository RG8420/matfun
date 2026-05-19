#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_NAME_LEN 32
#define MAX_MATRICES 52
#define MAX_PRINT_WIDTH 8

typedef struct {
    int rows;
    int cols;
    double **data;
    char name[MAX_NAME_LEN];
} Matrix;

typedef struct {
    Matrix matrices[MAX_MATRICES];
    int count;
} MatrixStore;

void ms_init(MatrixStore *store);
Matrix *ms_get(MatrixStore *store, const char *name);
int ms_set(MatrixStore *store, const char *name, Matrix *mat);
void ms_list(MatrixStore *store);
void ms_clear(MatrixStore *store);

Matrix *matrix_create(int rows, int cols);
void matrix_free(Matrix *mat);
Matrix *matrix_copy(Matrix *mat);
void matrix_print(Matrix *mat);
Matrix *matrix_from_array(double *arr, int rows, int cols);

int matrix_rows(Matrix *mat);
int matrix_cols(Matrix *mat);
double matrix_get(Matrix *mat, int row, int col);
void matrix_set(Matrix *mat, int row, int col, double val);

Matrix *matrix_add(Matrix *a, Matrix *b);
Matrix *matrix_sub(Matrix *a, Matrix *b);
Matrix *matrix_mult(Matrix *a, Matrix *b);
Matrix *matrix_emult(Matrix *a, Matrix *b);
Matrix *matrix_scale(Matrix *mat, double scalar);
Matrix *matrix_sadd(Matrix *mat, double scalar);

Matrix *matrix_transpose(Matrix *mat);
Matrix *matrix_inverse(Matrix *mat);
double matrix_det(Matrix *mat);
double matrix_trace(Matrix *mat);
int matrix_rank(Matrix *mat);
Matrix *matrix_adjugate(Matrix *mat);

Matrix *matrix_mvmult(Matrix *mat, Matrix *vec);
Matrix *matrix_vmult(Matrix *vec, Matrix *mat);

double matrix_dot(Matrix *v1, Matrix *v2);
Matrix *matrix_cross(Matrix *v1, Matrix *v2);
double matrix_magnitude(Matrix *v);
Matrix *matrix_normalize(Matrix *v);

Matrix *matrix_apply_func(Matrix *mat, double (*func)(double));

Matrix *matrix_identity(int n);
Matrix *matrix_zeros(int rows, int cols);
Matrix *matrix_ones(int rows, int cols);
Matrix *matrix_diag(Matrix *vec);

double matrix_frobenius(Matrix *mat);
double matrix_norm1(Matrix *mat);
double matrix_norminf(Matrix *mat);

int matrix_lu(Matrix *mat, Matrix **L, Matrix **U);
int matrix_qr(Matrix *mat, Matrix **Q, Matrix **R);
int matrix_cholesky(Matrix *mat, Matrix **L);
int matrix_eig(Matrix *mat, double **eigenvalues, Matrix **eigenvectors);
int matrix_svd(Matrix *mat, Matrix **U, Matrix **S, Matrix **V);

char *format_number(double val);

#endif