#include "matrix.h"

void ms_init(MatrixStore *store) {
    store->count = 0;
    for (int i = 0; i < MAX_MATRICES; i++) {
        store->matrices[i].data = NULL;
        store->matrices[i].rows = 0;
        store->matrices[i].cols = 0;
    }
}

Matrix *ms_get(MatrixStore *store, const char *name) {
    for (int i = 0; i < store->count; i++) {
        if (strcmp(store->matrices[i].name, name) == 0) {
            return &store->matrices[i];
        }
    }
    return NULL;
}

int ms_set(MatrixStore *store, const char *name, Matrix *mat) {
    for (int i = 0; i < store->count; i++) {
        if (strcmp(store->matrices[i].name, name) == 0) {
            if (store->matrices[i].data) {
                for (int r = 0; r < store->matrices[i].rows; r++) {
                    free(store->matrices[i].data[r]);
                }
                free(store->matrices[i].data);
            }
            store->matrices[i].rows = mat->rows;
            store->matrices[i].cols = mat->cols;
            store->matrices[i].data = mat->data;
            return 1;
        }
    }
    
    if (store->count >= MAX_MATRICES) {
        matrix_free(mat);
        return 0;
    }
    
    store->matrices[store->count].rows = mat->rows;
    store->matrices[store->count].cols = mat->cols;
    store->matrices[store->count].data = mat->data;
    strncpy(store->matrices[store->count].name, name, MAX_NAME_LEN - 1);
    store->count++;
    return 1;
}

void ms_list(MatrixStore *store) {
    if (store->count == 0) {
        printf("  No matrices stored.\n");
        return;
    }
    for (int i = 0; i < store->count; i++) {
        printf("  %s: %dx%d\n", store->matrices[i].name, 
               store->matrices[i].rows, store->matrices[i].cols);
    }
}

void ms_clear(MatrixStore *store) {
    for (int i = 0; i < store->count; i++) {
        if (store->matrices[i].data) {
            for (int j = 0; j < store->matrices[i].rows; j++) {
                free(store->matrices[i].data[j]);
            }
            free(store->matrices[i].data);
        }
    }
    store->count = 0;
}

Matrix *matrix_create(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return NULL;
    
    Matrix *mat = (Matrix *)malloc(sizeof(Matrix));
    if (!mat) return NULL;
    
    mat->rows = rows;
    mat->cols = cols;
    mat->data = (double **)malloc(rows * sizeof(double *));
    if (!mat->data) {
        free(mat);
        return NULL;
    }
    
    for (int i = 0; i < rows; i++) {
        mat->data[i] = (double *)calloc(cols, sizeof(double));
        if (!mat->data[i]) {
            for (int j = 0; j < i; j++) free(mat->data[j]);
            free(mat->data);
            free(mat);
            return NULL;
        }
    }
    
    mat->name[0] = '\0';
    return mat;
}

void matrix_free(Matrix *mat) {
    if (!mat) return;
    if (mat->data) {
        for (int i = 0; i < mat->rows; i++) {
            free(mat->data[i]);
        }
        free(mat->data);
    }
    free(mat);
}

Matrix *matrix_copy(Matrix *mat) {
    if (!mat) return NULL;
    
    Matrix *copy = matrix_create(mat->rows, mat->cols);
    if (!copy) return NULL;
    
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            copy->data[i][j] = mat->data[i][j];
        }
    }
    
    return copy;
}

void matrix_print(Matrix *mat) {
    if (!mat) {
        printf("  [NULL]\n");
        return;
    }
    
    for (int i = 0; i < mat->rows; i++) {
        printf("  [");
        for (int j = 0; j < mat->cols; j++) {
            printf("%s%.4f", j > 0 ? ", " : "", mat->data[i][j]);
        }
        printf("]\n");
    }
}

Matrix *matrix_from_array(double *arr, int rows, int cols) {
    Matrix *mat = matrix_create(rows, cols);
    if (!mat) return NULL;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat->data[i][j] = arr[i * cols + j];
        }
    }
    
    return mat;
}

int matrix_rows(Matrix *mat) {
    return mat ? mat->rows : 0;
}

int matrix_cols(Matrix *mat) {
    return mat ? mat->cols : 0;
}

double matrix_get(Matrix *mat, int row, int col) {
    if (!mat || row < 0 || row >= mat->rows || col < 0 || col >= mat->cols) return 0;
    return mat->data[row][col];
}

void matrix_set(Matrix *mat, int row, int col, double val) {
    if (!mat || row < 0 || row >= mat->rows || col < 0 || col >= mat->cols) return;
    mat->data[row][col] = val;
}

char *format_number(double val) {
    static char buf[32];
    if (fabs(val) < 0.0001 && val != 0) {
        snprintf(buf, sizeof(buf), "%.2e", val);
    } else if (val == (int)val && fabs(val) < 10000) {
        snprintf(buf, sizeof(buf), "%.0f", val);
    } else {
        snprintf(buf, sizeof(buf), "%.4f", val);
    }
    return buf;
}