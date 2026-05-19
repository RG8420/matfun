#include "matrix.h"
#include <ctype.h>
#include <string.h>

static int is_number_char(char c) {
    return (c >= '0' && c <= '9') || c == '-' || c == '.' || c == 'e' || c == 'E';
}

static int count_values(const char *str, int max) {
    int count = 0;
    int in_num = 0;
    for (int i = 0; str[i] && count < max; i++) {
        if (is_number_char(str[i])) {
            if (!in_num) { count++; in_num = 1; }
        } else {
            in_num = 0;
        }
    }
    return count;
}

static double parse_value(const char **s) {
    while (**s && !is_number_char(**s)) (*s)++;
    double val = atof(*s);
    while (is_number_char(**s)) (*s)++;
    return val;
}

static int parse_row(const char *row_str, double *values, int max_cols) {
    int count = 0;
    const char *p = row_str;
    while (*p && count < max_cols) {
        while (*p && !is_number_char(*p)) p++;
        if (!*p) break;
        values[count++] = parse_value(&p);
    }
    return count;
}

Matrix *parser_parse_matrix(const char *input) {
    // Skip leading whitespace
    while (*input == ' ') input++;
    
    // Find the matrix content between [ and ]
    const char *start = strchr(input, '[');
    const char *end = strrchr(input, ']');
    if (!start || !end || start >= end) return NULL;
    
    start++;
    size_t len = end - start;
    if (len == 0) return NULL;
    
    char *content = (char *)malloc(len + 1);
    strncpy(content, start, len);
    content[len] = '\0';
    
    // Split by semicolon for rows
    char *rows[64];
    int row_count = 0;
    char *temp = content;
    
    while (*temp && row_count < 64) {
        char *semicolon = strchr(temp, ';');
        if (semicolon) {
            *semicolon = '\0';
            rows[row_count++] = temp;
            temp = semicolon + 1;
        } else {
            rows[row_count++] = temp;
            break;
        }
    }
    
    if (row_count == 0) { free(content); return NULL; }
    
    // Count columns from first row
    int cols = count_values(rows[0], 1000);
    if (cols == 0) { free(content); return NULL; }
    
    // Validate all rows have same columns
    for (int i = 1; i < row_count; i++) {
        int c = count_values(rows[i], 1000);
        if (c != cols) { free(content); return NULL; }
    }
    
    // Create matrix
    Matrix *mat = matrix_create(row_count, cols);
    if (!mat) { free(content); return NULL; }
    
    // Fill matrix
    for (int i = 0; i < row_count; i++) {
        parse_row(rows[i], mat->data[i], cols);
    }
    
    free(content);
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
    
    // Vectors have no semicolons (single row)
    if (strchr(str, ';')) return 0;
    
    start++;
    while (start < end) {
        if (*start == '[' || *start == ']') return 0;
        start++;
    }
    return 1;
}