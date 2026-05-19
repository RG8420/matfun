#include "matrix.h"
#include <stdio.h>
#include <string.h>

int main() {
    MatrixStore store;
    ms_init(&store);
    
    const char *input = "A = [[1,2],[3,4]]";
    
    char var_name[32], expr[256];
    
    // Simulate parser_parse_assignment
    const char *eq = strchr(input, '=');
    if (eq) {
        int j = 0;
        for (int i = 0; i < eq - input && j < 31; i++) {
            if (input[i] != ' ' && input[i] != '\t') {
                var_name[j++] = input[i];
            }
        }
        var_name[j] = '\0';
        
        strcpy(expr, eq + 1);
        
        printf("var_name: '%s'\n", var_name);
        printf("expr: '%s'\n", expr);
        
        // Try parsing the matrix
        Matrix *mat = parser_parse_matrix(expr);
        if (mat) {
            printf("Parsed OK: %dx%d\n", mat->rows, mat->cols);
            matrix_print(mat);
            ms_set(&store, var_name, mat);
        } else {
            printf("FAILED to parse\n");
        }
    }
    
    return 0;
}
