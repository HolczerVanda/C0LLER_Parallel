#include <stdio.h>

#define N 3

void luDecomposition(double A[N][N], double L[N][N], double U[N][N], double *det) {
    int i, j, k;

    // Initializing L and U matrices
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (i == j)
                L[i][j] = 1;
            else
                L[i][j] = 0;
            U[i][j] = 0;
        }
    }

    *det = 1.0;

    // LU decomposition
    for (i = 0; i < N; i++) {
        
        for (j = i; j < N; j++) {
            U[i][j] = A[i][j];
            for (k = 0; k < i; k++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }

        for (j = i + 1; j < N; j++) {
            L[j][i] = A[j][i];
            for (k = 0; k < i; k++) {
                L[j][i] -= L[j][k] * U[k][i];
            }
            L[j][i] /= U[i][i];
        }

        *det *= U[i][i];
    }
}

int main() {
    double A[N][N] = {{13, -41, 10}, {0, -4, 1}, {3, 11, 9}};
    double L[N][N], U[N][N], det;
    int i, j;

    luDecomposition(A, L, U, &det);

    printf("Lower triangular matrix (L):\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%f\t", L[i][j]);
        }
        printf("\n");
    }

    printf("\nUpper triangular matrix (U):\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%f\t", U[i][j]);
        }
        printf("\n");
    }

    printf("\nDeterminant of the matrix: %f\n", det);

    return 0;
}
