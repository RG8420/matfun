#define _POSIX_C_SOURCE 200809L

#include "matrix.h"
#include <ctype.h>
#include <unistd.h>

#define HELP_TEXT \
"===========================================================\n" \
"              MATRIX CALCULATOR HELP\n" \
"===========================================================\n" \
"  Input Syntax: (use semicolon for rows)\n" \
"    A = [1 2; 3 4]        # 2x2 matrix\n" \
"    v = [1 2 3]           # 1x3 vector\n" \
"\n" \
"  Variables:\n" \
"    list              # List all matrices\n" \
"    clear             # Clear all matrices\n" \
"\n" \
"  Matrix-Matrix Operations:\n" \
"    add A B            # A + B\n" \
"    sub A B            # A - B\n" \
"    mult A B           # Matrix multiplication\n" \
"    emult A B          # Element-wise multiply\n" \
"\n" \
"  Matrix-Vector Operations:\n" \
"    mvmult A v         # Matrix * vector\n" \
"    vmult v A          # Vector * matrix\n" \
"\n" \
"  Scalar Operations:\n" \
"    scale A 5          # k * A\n" \
"    sadd A 5           # A + k\n" \
"\n" \
"  Unary Operations:\n" \
"    transpose A        # A^T\n" \
"    inverse A          # A^-1\n" \
"    det A              # Determinant\n" \
"    trace A            # Trace\n" \
"    rank A             # Rank\n" \
"    adjugate A         # Adjugate\n" \
"\n" \
"  Factorization:\n" \
"    lu A               # LU decomposition\n" \
"    qr A               # QR decomposition\n" \
"    cholesky A         # Cholesky (SPD)\n" \
"    eig A              # Eigenvalues\n" \
"    svd A              # SVD\n" \
"\n" \
"  Vector Operations:\n" \
"    dot v1 v2          # Dot product\n" \
"    cross v1 v2        # Cross product\n" \
"    magnitude v        # Vector magnitude\n" \
"    normalize v        # Unit vector\n" \
"\n" \
"  Element-wise Math:\n" \
"    sin A, cos A, exp A, log A, abs A, sqrt A\n" \
"\n" \
"  Matrix Construction:\n" \
"    identity n         # Identity matrix\n" \
"    zeros m n          # Zero matrix\n" \
"    ones m n           # Matrix of ones\n" \
"    diag [1,2,3]       # Diagonal matrix\n" \
"\n" \
"  Norms:\n" \
"    frobenius A        # Frobenius norm\n" \
"    norm1 A            # L1 norm\n" \
"    norminf A          # Infinity norm\n" \
"\n" \
"  Other:\n" \
"    help               # Show this help\n" \
"    quit               # Exit calculator\n" \
"===========================================================\n"

extern Matrix *parser_parse_matrix(const char *input);
extern int parser_parse_assignment(const char *input, char *var_name, char *expr);
extern int parser_is_vector(const char *str);

static MatrixStore store;

static void print_prompt() {
    printf("matfun> ");
    fflush(stdout);
}

static Matrix *get_matrix_arg(char **args, int arg_count, int idx) {
    if (idx >= arg_count) return NULL;
    return ms_get(&store, args[idx]);
}

static double get_scalar_arg(char **args, int arg_count, int idx) {
    if (idx >= arg_count) return 0;
    return atof(args[idx]);
}

static void handle_command(char *input) {
    char var_name[MAX_NAME_LEN];
    char expr[1024];
    
    if (parser_parse_assignment(input, var_name, expr)) {
        char *expr_trimmed = expr;
        while (*expr_trimmed == ' ') expr_trimmed++;
        
        if (strncmp(expr_trimmed, "[", 1) == 0) {
            Matrix *mat = parser_parse_matrix(expr_trimmed);
            if (mat) {
                ms_set(&store, var_name, mat);
                printf("Stored as %s:\n", var_name);
                matrix_print(mat);
            } else {
                printf("Error: Invalid matrix syntax\n");
            }
        } else if (expr_trimmed[0] == '[' && parser_is_vector(expr_trimmed)) {
            Matrix *mat = parser_parse_matrix(expr_trimmed);
            if (mat) {
                ms_set(&store, var_name, mat);
                printf("Stored as %s:\n", var_name);
                matrix_print(mat);
            } else {
                printf("Error: Invalid vector syntax\n");
            }
        } else {
            char cmd[64], arg1[64], arg2[64], arg3[64];
            int args = sscanf(expr_trimmed, "%63s %63s %63s %63s", cmd, arg1, arg2, arg3);
            
            Matrix *result = NULL;
            double scalar_result = 0;
            Matrix *m1 = NULL, *m2 = NULL;
            
            if (strcmp(cmd, "add") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_add(m1, m2);
            } else if (strcmp(cmd, "sub") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_sub(m1, m2);
            } else if (strcmp(cmd, "mult") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_mult(m1, m2);
            } else if (strcmp(cmd, "emult") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_emult(m1, m2);
            } else if (strcmp(cmd, "scale") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                double s = atof(arg2);
                if (m1) result = matrix_scale(m1, s);
            } else if (strcmp(cmd, "sadd") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                double s = atof(arg2);
                if (m1) result = matrix_sadd(m1, s);
            } else if (strcmp(cmd, "transpose") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_transpose(m1);
            } else if (strcmp(cmd, "inverse") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_inverse(m1);
            } else if (strcmp(cmd, "det") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_det(m1);
            } else if (strcmp(cmd, "trace") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_trace(m1);
            } else if (strcmp(cmd, "rank") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_rank(m1);
            } else if (strcmp(cmd, "adjugate") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_adjugate(m1);
            } else if (strcmp(cmd, "mvmult") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_mvmult(m1, m2);
            } else if (strcmp(cmd, "vmult") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_vmult(m1, m2);
            } else if (strcmp(cmd, "dot") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) scalar_result = matrix_dot(m1, m2);
            } else if (strcmp(cmd, "cross") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_cross(m1, m2);
            } else if (strcmp(cmd, "magnitude") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_magnitude(m1);
            } else if (strcmp(cmd, "normalize") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_normalize(m1);
            } else if (strcmp(cmd, "sin") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_apply_func(m1, sin);
            } else if (strcmp(cmd, "cos") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_apply_func(m1, cos);
            } else if (strcmp(cmd, "exp") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_apply_func(m1, exp);
            } else if (strcmp(cmd, "log") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_apply_func(m1, log);
            } else if (strcmp(cmd, "abs") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_apply_func(m1, fabs);
            } else if (strcmp(cmd, "sqrt") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_apply_func(m1, sqrt);
            } else if (strcmp(cmd, "identity") == 0 && args >= 2) {
                int n = atoi(arg1);
                if (n > 0) result = matrix_identity(n);
            } else if (strcmp(cmd, "zeros") == 0 && args >= 3) {
                int m = atoi(arg1), n = atoi(arg2);
                if (m > 0 && n > 0) result = matrix_zeros(m, n);
            } else if (strcmp(cmd, "ones") == 0 && args >= 3) {
                int m = atoi(arg1), n = atoi(arg2);
                if (m > 0 && n > 0) result = matrix_ones(m, n);
            } else if (strcmp(cmd, "diag") == 0 && args >= 2) {
                Matrix *vec = parser_parse_matrix(arg1);
                if (vec) {
                    result = matrix_diag(vec);
                    matrix_free(vec);
                }
            } else if (strcmp(cmd, "frobenius") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_frobenius(m1);
            } else if (strcmp(cmd, "norm1") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_norm1(m1);
            } else if (strcmp(cmd, "norminf") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_norminf(m1);
            } else if (strcmp(cmd, "lu") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) {
                    Matrix *L, *U;
                    if (matrix_lu(m1, &L, &U)) {
                        printf("L:\n");
                        matrix_print(L);
                        printf("U:\n");
                        matrix_print(U);
                        matrix_free(L);
                        matrix_free(U);
                    } else {
                        printf("Error: LU decomposition failed\n");
                    }
                    return;
                }
            } else if (strcmp(cmd, "qr") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) {
                    Matrix *Q, *R;
                    if (matrix_qr(m1, &Q, &R)) {
                        printf("Q:\n");
                        matrix_print(Q);
                        printf("R:\n");
                        matrix_print(R);
                        matrix_free(Q);
                        matrix_free(R);
                    } else {
                        printf("Error: QR decomposition failed\n");
                    }
                    return;
                }
            } else if (strcmp(cmd, "cholesky") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) {
                    Matrix *L;
                    if (matrix_cholesky(m1, &L)) {
                        printf("L:\n");
                        matrix_print(L);
                        matrix_free(L);
                    } else {
                        printf("Error: Cholesky decomposition failed (matrix not SPD)\n");
                    }
                    return;
                }
            } else if (strcmp(cmd, "eig") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) {
                    double *evals;
                    Matrix *evectors;
                    if (matrix_eig(m1, &evals, &evectors)) {
                        printf("Eigenvalues:\n");
                        for (int i = 0; i < m1->rows; i++) {
                            printf("  %.4f\n", evals[i]);
                        }
                        printf("Eigenvectors:\n");
                        matrix_print(evectors);
                        free(evals);
                        matrix_free(evectors);
                    }
                    return;
                }
            } else if (strcmp(cmd, "svd") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) {
                    Matrix *U, *S, *V;
                    if (matrix_svd(m1, &U, &S, &V)) {
                        printf("U:\n");
                        matrix_print(U);
                        printf("S:\n");
                        matrix_print(S);
                        printf("V:\n");
                        matrix_print(V);
                        matrix_free(U);
                        matrix_free(S);
                        matrix_free(V);
                    }
                    return;
                }
            }
            
            if (result) {
                ms_set(&store, var_name, result);
                printf("Stored as %s:\n", var_name);
                matrix_print(result);
            } else if (scalar_result != 0 || (args >= 2 && (strstr(cmd, "det") || strstr(cmd, "trace") || strstr(cmd, "rank") || strstr(cmd, "magnitude") || strstr(cmd, "frobenius") || strstr(cmd, "norm")))) {
                printf("  = %.6f\n", scalar_result);
            } else {
                printf("Error: Unknown command or invalid arguments\n");
            }
        }
    } else {
        if (strncmp(input, "list", 4) == 0) {
            ms_list(&store);
        } else if (strcmp(input, "clear") == 0) {
            ms_clear(&store);
            printf("  Cleared all matrices.\n");
        } else if (strcmp(input, "help") == 0) {
            printf("%s", HELP_TEXT);
        } else if (strncmp(input, "[", 1) == 0) {
            Matrix *mat = parser_parse_matrix(input);
            if (mat) {
                matrix_print(mat);
                matrix_free(mat);
            } else {
                printf("Error: Invalid matrix syntax\n");
            }
        } else {
            char cmd[64], arg1[64], arg2[64], arg3[64];
            int args = sscanf(input, "%63s %63s %63s %63s", cmd, arg1, arg2, arg3);
            
            Matrix *result = NULL;
            double scalar_result = 0;
            Matrix *m1 = NULL, *m2 = NULL;
            
            if (strcmp(cmd, "add") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_add(m1, m2);
            } else if (strcmp(cmd, "sub") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_sub(m1, m2);
            } else if (strcmp(cmd, "mult") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_mult(m1, m2);
            } else if (strcmp(cmd, "emult") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_emult(m1, m2);
            } else if (strcmp(cmd, "scale") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                double k = atof(arg2);
                if (m1) result = matrix_scale(m1, k);
            } else if (strcmp(cmd, "sadd") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                double k = atof(arg2);
                if (m1) result = matrix_sadd(m1, k);
            } else if (strcmp(cmd, "transpose") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_transpose(m1);
            } else if (strcmp(cmd, "inverse") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_inverse(m1);
            } else if (strcmp(cmd, "det") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_det(m1);
            } else if (strcmp(cmd, "trace") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_trace(m1);
            } else if (strcmp(cmd, "rank") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_rank(m1);
            } else if (strcmp(cmd, "adjugate") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_adjugate(m1);
            } else if (strcmp(cmd, "mvmult") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_vmult(m1, m2);
            } else if (strcmp(cmd, "vmult") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_vmult(m2, m1);
            } else if (strcmp(cmd, "dot") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) scalar_result = matrix_dot(m1, m2);
            } else if (strcmp(cmd, "cross") == 0 && args >= 3) {
                m1 = ms_get(&store, arg1);
                m2 = ms_get(&store, arg2);
                if (m1 && m2) result = matrix_cross(m1, m2);
            } else if (strcmp(cmd, "magnitude") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_magnitude(m1);
            } else if (strcmp(cmd, "normalize") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_normalize(m1);
            } else if (strcmp(cmd, "sin") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_apply_func(m1, sin);
            } else if (strcmp(cmd, "cos") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_apply_func(m1, cos);
            } else if (strcmp(cmd, "exp") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_apply_func(m1, exp);
            } else if (strcmp(cmd, "log") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_apply_func(m1, log);
            } else if (strcmp(cmd, "abs") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_apply_func(m1, fabs);
            } else if (strcmp(cmd, "sqrt") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) result = matrix_apply_func(m1, sqrt);
            } else if (strcmp(cmd, "identity") == 0 && args >= 2) {
                int n = atoi(arg1);
                if (n > 0) result = matrix_identity(n);
            } else if (strcmp(cmd, "zeros") == 0 && args >= 3) {
                int m = atoi(arg1);
                int n = atoi(arg2);
                if (m > 0 && n > 0) result = matrix_create(m, n);
            } else if (strcmp(cmd, "ones") == 0 && args >= 3) {
                int m = atoi(arg1);
                int n = atoi(arg2);
                if (m > 0 && n > 0) {
                    result = matrix_create(m, n);
                    for (int i = 0; i < m; i++)
                        for (int j = 0; j < n; j++)
                            result->data[i][j] = 1.0;
                }
            } else if (strcmp(cmd, "diag") == 0 && args >= 2) {
                Matrix *v = ms_get(&store, arg1);
                if (v) result = matrix_diag(v);
            } else if (strcmp(cmd, "frobenius") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_frobenius(m1);
            } else if (strcmp(cmd, "norm1") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_norm1(m1);
            } else if (strcmp(cmd, "norminf") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) scalar_result = matrix_norminf(m1);
            } else if (strcmp(cmd, "lu") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) {
                    Matrix *L = NULL, *U = NULL;
                    matrix_lu(m1, &L, &U);
                    if (L && U) {
                        printf("L:\n");
                        matrix_print(L);
                        printf("U:\n");
                        matrix_print(U);
                        matrix_free(L);
                        matrix_free(U);
                    }
                }
            } else if (strcmp(cmd, "qr") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) {
                    Matrix *Q = NULL, *R = NULL;
                    matrix_qr(m1, &Q, &R);
                    if (Q && R) {
                        printf("Q:\n");
                        matrix_print(Q);
                        printf("R:\n");
                        matrix_print(R);
                        matrix_free(Q);
                        matrix_free(R);
                    }
                }
            } else if (strcmp(cmd, "cholesky") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) {
                    Matrix *L = NULL;
                    if (matrix_cholesky(m1, &L) && L) {
                        printf("L:\n");
                        matrix_print(L);
                        matrix_free(L);
                    }
                }
            } else if (strcmp(cmd, "eig") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) {
                    double *real = NULL, *imag = NULL;
                    int n = matrix_eig(m1, &real, &imag);
                    if (n > 0) {
                        printf("Eigenvalues:\n");
                        for (int i = 0; i < n; i++) {
                            printf("  %.4f + %.4fi\n", real[i], imag[i]);
                        }
                        free(real);
                        free(imag);
                    }
                }
            } else if (strcmp(cmd, "svd") == 0 && args >= 2) {
                m1 = ms_get(&store, arg1);
                if (m1) {
                    Matrix *U = NULL, *S = NULL, *V = NULL;
                    int n = matrix_svd(m1, &U, &S, &V);
                    if (n > 0) {
                        printf("U:\n");
                        matrix_print(U);
                        printf("Singular values:\n");
                        matrix_print(S);
                        printf("V:\n");
                        matrix_print(V);
                        matrix_free(U);
                        matrix_free(S);
                        matrix_free(V);
                    }
                }
            }
            
            if (result) {
                matrix_print(result);
                matrix_free(result);
            } else if (scalar_result != 0 || (args >= 2 && (strstr(cmd, "det") || strstr(cmd, "trace") || strstr(cmd, "rank") || strstr(cmd, "magnitude") || strstr(cmd, "frobenius") || strstr(cmd, "norm")))) {
                printf("  = %.6f\n", scalar_result);
            } else {
                Matrix *mat = ms_get(&store, input);
                if (mat) {
                    matrix_print(mat);
                } else {
                    printf("Error: Unknown command. Type 'help' for options.\n");
                }
            }
        }
    }
}

int main() {
    ms_init(&store);
    
    printf("=== Matrix Calculator (matfun) ===\n");
    printf("Type 'help' for operations, 'quit' to exit.\n\n");
    
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    while (1) {
        print_prompt();
        
        read = getline(&line, &len, stdin);
        if (read == -1) {
            printf("\nGoodbye!\n");
            break;
        }
        
        size_t line_len = strlen(line);
        while (line_len > 0 && (line[line_len - 1] == '\n' || line[line_len - 1] == '\r')) {
            line[--line_len] = '\0';
        }
        
        if (line_len == 0) continue;
        
        if (strcmp(line, "quit") == 0 || strcmp(line, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }
        
        handle_command(line);
    }
    
    free(line);
    ms_clear(&store);
    return 0;
}