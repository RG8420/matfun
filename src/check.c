#include "matrix.h"
#include <stdio.h>

int main() {
    Matrix *mat = matrix_create(2, 2);
    if (mat) {
        printf("Created matrix: rows=%d, cols=%d\n", mat->rows, mat->cols);
        mat->data[0][0] = 1;
        mat->data[0][1] = 2;
        mat->data[1][0] = 3;
        mat->data[1][1] = 4;
        printf("After setting values:\n");
        matrix_print(mat);
        matrix_free(mat);
    }
    return 0;
}
