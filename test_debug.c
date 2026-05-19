#include <stdio.h>
#include <stdlib.h>
#include "src/matrix.h"

int main() {
    Matrix *mat = parser_parse_matrix("[1 2; 3 4]");
    if (mat) {
        printf("rows=%d, cols=%d\n", mat->rows, mat->cols);
        for (int i = 0; i < mat->rows; i++) {
            for (int j = 0; j < mat->cols; j++) {
                printf("data[%d][%d] = %f\n", i, j, mat->data[i][j]);
            }
        }
        matrix_free(mat);
    } else {
        printf("Failed to parse\n");
    }
    return 0;
}
