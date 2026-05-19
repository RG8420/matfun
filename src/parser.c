#include "matrix.h"
#include <ctype.h>

Matrix *parser_parse_matrix(const char *input) {
    const char *p = input;
    
    // Find first '['
    while (*p && *p != '[') p++;
    if (!*p) return NULL;
    
    // Count rows and columns
    int rows = 1, cols = 0, in_num = 0;
    const char *temp = p;
    int current_row_cols = 0;
    
    while (*temp && *temp != ']') {
        if (*temp == '[') {
            if (current_row_cols > cols) cols = current_row_cols;
            current_row_cols = 0;
            rows++;
        }
        if (isdigit(*temp) || *temp == '-' || *temp == '.') {
            if (!in_num) {
                current_row_cols++;
                in_num = 1;
            }
        } else {
            in_num = 0;
        }
        temp++;
    }
    if (current_row_cols > cols) cols = current_row_cols;
    
    if (rows == 0 || cols == 0) return NULL;
    
    Matrix *mat = matrix_create(rows - 1, cols);
    if (!mat) return NULL;
    
    // Extract numbers
    double *values = (double *)malloc(rows * cols * sizeof(double));
    int value_count = 0;
    
    temp = p;
    in_num = 0;
    char num_buf[64];
    int buf_idx = 0;
    
    while (*temp && value_count < rows * cols) {
        if (isdigit(*temp) || *temp == '-' || *temp == '.' ) {
            num_buf[buf_idx++] = *temp;
            in_num = 1;
        } else if (in_num) {
            num_buf[buf_idx] = '\0';
            values[value_count++] = atof(num_buf);
            buf_idx = 0;
            in_num = 0;
        }
        
        // Skip to next number
        if (*temp == ']') {
            temp++;
            while (*temp && *temp != '[' && !isdigit(*temp) && *temp != '-' ) temp++;
            continue;
        }
        temp++;
    }
    
    // Fill matrix
    for (int i = 0; i < value_count && i < rows * cols; i++) {
        mat->data[i / cols][i % cols] = values[i];
    }
    
    free(values);
    return mat;
}

int parser_parse_assignment(const char *input, char *var_name, char *expr) {
    const char *eq = strchr(input, '=');
    if (!eq) return 0;
    
    int j = 0;
    for (int i = 0; i < eq - input && j < MAX_NAME_LEN - 1; i++) {
        if (input[i] != ' ' && input[i] != '\t') {
            var_name[j++] = input[i];
        }
    }
    var_name[j] = '\0';
    
    strcpy(expr, eq + 1);
    return strlen(var_name) > 0;
}

int parser_is_vector(const char *str) {
    const char *start = strchr(str, '[');
    const char *end = strrchr(str, ']');
    if (!start || !end) return 0;
    
    start++;
    while (start < end) {
        if (*start == '[' || *start == ']') return 0;
        start++;
    }
    return 1;
}