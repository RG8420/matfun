#include "matrix.h"
#include <ctype.h>

static int is_number_char(char c) {
    return (c >= '0' && c <= '9') || c == '-' || c == '.' || c == 'e' || c == 'E';
}

static int count_matrix_dims(const char *str, int *rows, int *cols) {
    *rows = 0;
    *cols = 0;
    
    // Skip leading whitespace
    while (*str == ' ') str++;
    
    // Count rows - each "],[ " separates rows
    const char *p = str;
    while (*p) {
        if (*p == ']' && p[1] == ',') (*rows)++;
        p++;
    }
    (*rows)++; // last row
    
    // Find first row content between first [ and matching ]
    p = strchr(str, '[');
    if (!p) return 0;
    
    // Skip outer [, find inner row
    p++;
    // Find inner [
    while (*p && *p != '[') p++;
    if (!*p) return 0;
    
    // Find matching ] for inner row
    const char *row_start = p + 1;
    int depth = 1;
    while (*p && depth > 0) {
        p++;
        if (*p == '[') depth++;
        if (*p == ']') depth--;
    }
    const char *row_end = p;
    
    // Count numbers in first row
    int in_num = 0;
    while (row_start < row_end) {
        if (is_number_char(*row_start)) {
            if (!in_num) { (*cols)++; in_num = 1; }
        } else {
            in_num = 0;
        }
        row_start++;
    }
    
    return (*rows > 0 && *cols > 0) ? 1 : 0;
}

static int parse_numbers(const char *str, double *values, int max_count) {
    int count = 0;
    char buffer[64];
    int buf_idx = 0;
    int in_number = 0;
    
    for (int i = 0; str[i] && count < max_count; i++) {
        if (is_number_char(str[i])) {
            buffer[buf_idx++] = str[i];
            in_number = 1;
        } else if (in_number) {
            buffer[buf_idx] = '\0';
            values[count++] = atof(buffer);
            buf_idx = 0;
            in_number = 0;
        }
    }
    
    if (in_number && buf_idx > 0 && count < max_count) {
        buffer[buf_idx] = '\0';
        values[count++] = atof(buffer);
    }
    
    return count;
}

Matrix *parser_parse_matrix(const char *input) {
    int rows, cols;
    if (!count_matrix_dims(input, &rows, &cols) || rows < 1 || cols < 1) {
        return NULL;
    }
    
    Matrix *mat = matrix_create(rows, cols);
    if (!mat) return NULL;
    
    // Remove brackets and parse numbers
    char clean[4096];
    int ci = 0;
    for (int i = 0; input[i] && ci < 4095; i++) {
        if (input[i] != '[' && input[i] != ']') {
            clean[ci++] = input[i];
        }
    }
    clean[ci] = '\0';
    
    double *values = (double *)malloc(rows * cols * sizeof(double));
    if (!values) {
        matrix_free(mat);
        return NULL;
    }
    
    int total = parse_numbers(clean, values, rows * cols);
    
    for (int i = 0; i < total && i < rows * cols; i++) {
        int row = i / cols;
        int col = i % cols;
        mat->data[row][col] = values[i];
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