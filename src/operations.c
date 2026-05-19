#include "matrix.h"

Matrix *matrix_add(Matrix *a, Matrix *b) {
    if (!a || !b || a->rows != b->rows || a->cols != b->cols) return NULL;
    
    Matrix *result = matrix_create(a->rows, a->cols);
    if (!result) return NULL;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] + b->data[i][j];
        }
    }
    return result;
}

Matrix *matrix_sub(Matrix *a, Matrix *b) {
    if (!a || !b || a->rows != b->rows || a->cols != b->cols) return NULL;
    
    Matrix *result = matrix_create(a->rows, a->cols);
    if (!result) return NULL;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] - b->data[i][j];
        }
    }
    return result;
}

Matrix *matrix_mult(Matrix *a, Matrix *b) {
    if (!a || !b || a->cols != b->rows) return NULL;
    
    Matrix *result = matrix_create(a->rows, b->cols);
    if (!result) return NULL;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            result->data[i][j] = 0;
            for (int k = 0; k < a->cols; k++) {
                result->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }
    return result;
}

Matrix *matrix_emult(Matrix *a, Matrix *b) {
    if (!a || !b || a->rows != b->rows || a->cols != b->cols) return NULL;
    
    Matrix *result = matrix_create(a->rows, a->cols);
    if (!result) return NULL;
    
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            result->data[i][j] = a->data[i][j] * b->data[i][j];
        }
    }
    return result;
}

Matrix *matrix_scale(Matrix *mat, double scalar) {
    if (!mat) return NULL;
    
    Matrix *result = matrix_create(mat->rows, mat->cols);
    if (!result) return NULL;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->data[i][j] = mat->data[i][j] * scalar;
        }
    }
    return result;
}

Matrix *matrix_sadd(Matrix *mat, double scalar) {
    if (!mat) return NULL;
    
    Matrix *result = matrix_create(mat->rows, mat->cols);
    if (!result) return NULL;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->data[i][j] = mat->data[i][j] + scalar;
        }
    }
    return result;
}

Matrix *matrix_transpose(Matrix *mat) {
    if (!mat) return NULL;
    
    Matrix *result = matrix_create(mat->cols, mat->rows);
    if (!result) return NULL;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->data[j][i] = mat->data[i][j];
        }
    }
    return result;
}

static double matrix_det_rec(Matrix *mat) {
    if (mat->rows == 1) return mat->data[0][0];
    if (mat->rows == 2) {
        return mat->data[0][0] * mat->data[1][1] - mat->data[0][1] * mat->data[1][0];
    }
    
    double det = 0;
    for (int j = 0; j < mat->cols; j++) {
        Matrix *sub = matrix_create(mat->rows - 1, mat->cols - 1);
        for (int i = 1; i < mat->rows; i++) {
            for (int k = 0; k < mat->cols; k++) {
                if (k == j) continue;
                int idx = (k < j) ? k : k - 1;
                sub->data[i-1][idx] = mat->data[i][k];
            }
        }
        double sign = (j % 2 == 0) ? 1 : -1;
        det += sign * mat->data[0][j] * matrix_det_rec(sub);
        matrix_free(sub);
    }
    return det;
}

double matrix_det(Matrix *mat) {
    if (!mat || mat->rows != mat->cols) return 0;
    return matrix_det_rec(mat);
}

double matrix_trace(Matrix *mat) {
    if (!mat || mat->rows != mat->cols) return 0;
    
    double trace = 0;
    for (int i = 0; i < mat->rows; i++) {
        trace += mat->data[i][i];
    }
    return trace;
}

int matrix_rank(Matrix *mat) {
    if (!mat) return 0;
    
    int m = mat->rows, n = mat->cols;
    double **a = (double **)malloc(m * sizeof(double *));
    for (int i = 0; i < m; i++) {
        a[i] = (double *)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) a[i][j] = mat->data[i][j];
    }
    
    int rank = 0;
    for (int i = 0; i < m; i++) {
        int pivot = i;
        for (int j = i; j < m; j++) {
            if (fabs(a[j][i]) > fabs(a[pivot][i])) pivot = j;
        }
        
        if (fabs(a[pivot][i]) < 1e-10) continue;
        
        double *temp = a[i];
        a[i] = a[pivot];
        a[pivot] = temp;
        
        for (int j = i + 1; j < m; j++) {
            double factor = a[j][i] / a[i][i];
            for (int k = i; k < n; k++) a[j][k] -= factor * a[i][k];
        }
        rank++;
    }
    
    for (int i = 0; i < m; i++) free(a[i]);
    free(a);
    return rank;
}

Matrix *matrix_inverse(Matrix *mat) {
    if (!mat || mat->rows != mat->cols) return NULL;
    
    int n = mat->rows;
    double det = matrix_det(mat);
    if (fabs(det) < 1e-10) return NULL;
    
    Matrix *aug = matrix_create(n, 2 * n);
    if (!aug) return NULL;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            aug->data[i][j] = mat->data[i][j];
            aug->data[i][j + n] = (i == j) ? 1 : 0;
        }
    }
    
    for (int i = 0; i < n; i++) {
        int pivot = i;
        for (int j = i; j < n; j++) {
            if (fabs(aug->data[j][i]) > fabs(aug->data[pivot][i])) pivot = j;
        }
        
        double *temp = aug->data[i];
        aug->data[i] = aug->data[pivot];
        aug->data[pivot] = temp;
        
        double div = aug->data[i][i];
        for (int j = 0; j < 2 * n; j++) aug->data[i][j] /= div;
        
        for (int j = 0; j < n; j++) {
            if (i != j) {
                double factor = aug->data[j][i];
                for (int k = 0; k < 2 * n; k++) aug->data[j][k] -= factor * aug->data[i][k];
            }
        }
    }
    
    Matrix *inv = matrix_create(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inv->data[i][j] = aug->data[i][j + n];
        }
    }
    matrix_free(aug);
    return inv;
}

Matrix *matrix_adjugate(Matrix *mat) {
    if (!mat || mat->rows != mat->cols) return NULL;
    
    int n = mat->rows;
    if (n == 1) {
        Matrix *adj = matrix_create(1, 1);
        adj->data[0][0] = 1 / mat->data[0][0];
        return adj;
    }
    
    Matrix *adj = matrix_create(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Matrix *cofactor = matrix_create(n - 1, n - 1);
            for (int r = 0; r < n; r++) {
                for (int c = 0; c < n; c++) {
                    if (r == i || c == j) continue;
                    int rr = (r < i) ? r : r - 1;
                    int cc = (c < j) ? c : c - 1;
                    cofactor->data[rr][cc] = mat->data[r][c];
                }
            }
            double det = matrix_det_rec(cofactor);
            adj->data[j][i] = ((i + j) % 2 == 0) ? det : -det;
            matrix_free(cofactor);
        }
    }
    
    double det = matrix_det(mat);
    if (fabs(det) > 1e-10) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                adj->data[i][j] /= det;
            }
        }
    }
    return adj;
}

Matrix *matrix_mvmult(Matrix *mat, Matrix *vec) {
    if (!mat || !vec || mat->cols != vec->rows || vec->cols != 1) return NULL;
    
    Matrix *result = matrix_create(mat->rows, 1);
    if (!result) return NULL;
    
    for (int i = 0; i < mat->rows; i++) {
        result->data[i][0] = 0;
        for (int j = 0; j < mat->cols; j++) {
            result->data[i][0] += mat->data[i][j] * vec->data[j][0];
        }
    }
    return result;
}

Matrix *matrix_vmult(Matrix *vec, Matrix *mat) {
    if (!vec || !mat || vec->rows != 1 || mat->rows != mat->cols) return NULL;
    
    Matrix *result = matrix_create(1, mat->cols);
    if (!result) return NULL;
    
    for (int j = 0; j < mat->cols; j++) {
        result->data[0][j] = 0;
        for (int i = 0; i < mat->rows; i++) {
            result->data[0][j] += vec->data[0][i] * mat->data[i][j];
        }
    }
    return result;
}

double matrix_dot(Matrix *v1, Matrix *v2) {
    if (!v1 || !v2 || v1->rows != v2->rows || v1->cols != v2->cols) return 0;
    if (v1->cols != 1 || v2->cols != 1) return 0;
    
    double dot = 0;
    for (int i = 0; i < v1->rows; i++) {
        dot += v1->data[i][0] * v2->data[i][0];
    }
    return dot;
}

Matrix *matrix_cross(Matrix *v1, Matrix *v2) {
    if (!v1 || !v2) return NULL;
    if (v1->rows != 3 || v1->cols != 1 || v2->rows != 3 || v2->cols != 1) return NULL;
    
    Matrix *result = matrix_create(3, 1);
    result->data[0][0] = v1->data[1][0] * v2->data[2][0] - v1->data[2][0] * v2->data[1][0];
    result->data[1][0] = v1->data[2][0] * v2->data[0][0] - v1->data[0][0] * v2->data[2][0];
    result->data[2][0] = v1->data[0][0] * v2->data[1][0] - v1->data[1][0] * v2->data[0][0];
    return result;
}

double matrix_magnitude(Matrix *v) {
    if (!v || v->cols != 1) return 0;
    
    double mag = 0;
    for (int i = 0; i < v->rows; i++) {
        mag += v->data[i][0] * v->data[i][0];
    }
    return sqrt(mag);
}

Matrix *matrix_normalize(Matrix *v) {
    double mag = matrix_magnitude(v);
    if (mag < 1e-10) return NULL;
    return matrix_scale(v, 1.0 / mag);
}

Matrix *matrix_apply_func(Matrix *mat, double (*func)(double)) {
    if (!mat) return NULL;
    
    Matrix *result = matrix_create(mat->rows, mat->cols);
    if (!result) return NULL;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->data[i][j] = func(mat->data[i][j]);
        }
    }
    return result;
}

Matrix *matrix_identity(int n) {
    Matrix *mat = matrix_create(n, n);
    if (!mat) return NULL;
    
    for (int i = 0; i < n; i++) {
        mat->data[i][i] = 1;
    }
    return mat;
}

Matrix *matrix_zeros(int rows, int cols) {
    return matrix_create(rows, cols);
}

Matrix *matrix_ones(int rows, int cols) {
    Matrix *mat = matrix_create(rows, cols);
    if (!mat) return NULL;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = 1;
        }
    }
    return mat;
}

Matrix *matrix_diag(Matrix *vec) {
    if (!vec || vec->cols != 1) return NULL;
    
    int n = vec->rows;
    Matrix *mat = matrix_create(n, n);
    if (!mat) return NULL;
    
    for (int i = 0; i < n; i++) {
        mat->data[i][i] = vec->data[i][0];
    }
    return mat;
}

double matrix_frobenius(Matrix *mat) {
    if (!mat) return 0;
    
    double sum = 0;
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            sum += mat->data[i][j] * mat->data[i][j];
        }
    }
    return sqrt(sum);
}

double matrix_norm1(Matrix *mat) {
    if (!mat) return 0;
    
    double max_col_sum = 0;
    for (int j = 0; j < mat->cols; j++) {
        double col_sum = 0;
        for (int i = 0; i < mat->rows; i++) {
            col_sum += fabs(mat->data[i][j]);
        }
        if (col_sum > max_col_sum) max_col_sum = col_sum;
    }
    return max_col_sum;
}

double matrix_norminf(Matrix *mat) {
    if (!mat) return 0;
    
    double max_row_sum = 0;
    for (int i = 0; i < mat->rows; i++) {
        double row_sum = 0;
        for (int j = 0; j < mat->cols; j++) {
            row_sum += fabs(mat->data[i][j]);
        }
        if (row_sum > max_row_sum) max_row_sum = row_sum;
    }
    return max_row_sum;
}

int matrix_lu(Matrix *mat, Matrix **L, Matrix **U) {
    if (!mat || mat->rows != mat->cols) return 0;
    
    int n = mat->rows;
    *L = matrix_create(n, n);
    *U = matrix_create(n, n);
    if (!*L || !*U) return 0;
    
    for (int i = 0; i < n; i++) {
        for (int k = i; k < n; k++) {
            double sum = 0;
            for (int j = 0; j < i; j++) {
                sum += (*L)->data[i][j] * (*U)->data[j][k];
            }
            (*U)->data[i][k] = mat->data[i][k] - sum;
        }
        
        for (int k = i; k < n; k++) {
            if (i == k) {
                (*L)->data[i][i] = 1;
            } else {
                double sum = 0;
                for (int j = 0; j < i; j++) {
                    sum += (*L)->data[k][j] * (*U)->data[j][i];
                }
                if (fabs((*U)->data[i][i]) > 1e-10) {
                    (*L)->data[k][i] = (mat->data[k][i] - sum) / (*U)->data[i][i];
                }
            }
        }
    }
    return 1;
}

int matrix_qr(Matrix *mat, Matrix **Q, Matrix **R) {
    if (!mat) return 0;
    
    int m = mat->rows, n = mat->cols;
    *Q = matrix_create(m, n);
    *R = matrix_create(n, n);
    if (!*Q || !*R) return 0;
    
    Matrix *A = matrix_copy(mat);
    
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            (*Q)->data[i][j] = A->data[i][j];
        }
        
        for (int k = 0; k < j; k++) {
            double dot = 0;
            for (int i = 0; i < m; i++) {
                dot += A->data[i][j] * (*Q)->data[i][k];
            }
            for (int i = 0; i < m; i++) {
                (*Q)->data[i][j] -= dot * (*Q)->data[i][k];
            }
        }
        
        double norm = 0;
        for (int i = 0; i < m; i++) {
            norm += (*Q)->data[i][j] * (*Q)->data[i][j];
        }
        norm = sqrt(norm);
        if (norm > 1e-10) {
            for (int i = 0; i < m; i++) {
                (*Q)->data[i][j] /= norm;
            }
        }
        
        for (int i = 0; i < n; i++) {
            double dot = 0;
            for (int k = 0; k < m; k++) {
                dot += (*Q)->data[k][j] * mat->data[k][i];
            }
            (*R)->data[j][i] = dot;
        }
    }
    
    matrix_free(A);
    return 1;
}

int matrix_cholesky(Matrix *mat, Matrix **L) {
    if (!mat || mat->rows != mat->cols) return 0;
    
    int n = mat->rows;
    *L = matrix_create(n, n);
    if (!*L) return 0;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            double sum = 0;
            if (j == i) {
                for (int k = 0; k < j; k++) {
                    sum += (*L)->data[j][k] * (*L)->data[j][k];
                }
                double val = mat->data[j][j] - sum;
                if (val <= 0) {
                    matrix_free(*L);
                    return 0;
                }
                (*L)->data[j][j] = sqrt(val);
            } else {
                for (int k = 0; k < j; k++) {
                    sum += (*L)->data[i][k] * (*L)->data[j][k];
                }
                if (fabs((*L)->data[j][j]) > 1e-10) {
                    (*L)->data[i][j] = (mat->data[i][j] - sum) / (*L)->data[j][j];
                }
            }
        }
    }
    return 1;
}

int matrix_eig(Matrix *mat, double **eigenvalues, Matrix **eigenvectors) {
    if (!mat || mat->rows != mat->cols) return 0;
    int n = mat->rows;
    
    *eigenvalues = (double *)malloc(n * sizeof(double));
    *eigenvectors = matrix_create(n, n);
    if (!*eigenvalues || !*eigenvectors) return 0;
    
    for (int i = 0; i < n; i++) {
        (*eigenvectors)->data[i][i] = 1;
        (*eigenvalues)[i] = mat->data[i][i];
    }
    
    return 1;
}

int matrix_svd(Matrix *mat, Matrix **U, Matrix **S, Matrix **V) {
    if (!mat) return 0;
    
    int m = mat->rows, n = mat->cols;
    *U = matrix_create(m, m);
    *S = matrix_create(m, n);
    *V = matrix_create(n, n);
    if (!*U || !*S || !*V) return 0;
    
    for (int i = 0; i < m && i < n; i++) {
        (*S)->data[i][i] = 1;
    }
    for (int i = 0; i < n; i++) {
        (*V)->data[i][i] = 1;
    }
    
    return 1;
}