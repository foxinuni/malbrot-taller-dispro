#include "matrix.h"
#include <stdio.h>

void matn_print(double* matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        printf("| ");
        for (int j = 0; j < m; j++) {
            printf("%6.2f ", matrix[i * m + j]);
        }
        printf("|\n");
    }
}