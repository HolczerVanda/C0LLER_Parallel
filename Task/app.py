N = 3

def luDecomposition(A):
    L = [[0] * N for _ in range(N)]
    U = [[0] * N for _ in range(N)]
    det = 1.0

    for i in range(N):

        for j in range(i, N):
            U[i][j] = A[i][j]
            for k in range(i):
                U[i][j] -= L[i][k] * U[k][j]

        for j in range(i + 1, N):
            L[j][i] = A[j][i]
            for k in range(i):
                L[j][i] -= L[j][k] * U[k][i]
            L[j][i] /= U[i][i]

        det *= U[i][i]

    return L, U, det


if __name__ == "__main__":
    A = [[13, -41, 10], [0, -4, 1], [3, 11, 9]]

    L, U, det = luDecomposition(A)

    print("Lower triangular matrix (L):")
    for row in L:
        print("\t".join(f"{val:.2f}" for val in row))

    print("\nUpper triangular matrix (U):")
    for row in U:
        print("\t".join(f"{val:.2f}" for val in row))

    print("\nDeterminant of the matrix:", det)
