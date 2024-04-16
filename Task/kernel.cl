__kernel void luDecomposition(__global double* A, __global double* L, __global double* U, const int N) {
    int gid = get_global_id(0);
    int i, j;

    for (i = 0; i < N; i++) {

        if (gid >= i) {
            U[i * N + gid] = A[i * N + gid];
            for (j = 0; j < i; j++) {
                U[i * N + gid] -= L[i * N + j] * U[j * N + gid];
            }
        }

        if (gid >= i) {
            if (gid == i)
                L[i * N + i] = 1;
            else {
                L[gid * N + i] = A[gid * N + i];
                for (j = 0; j < i; j++) {
                    L[gid * N + i] -= L[gid * N + j] * U[j * N + i];
                }
                if (U[i * N + i] == 0.0) {
                    L[gid * N + i] = -1.0;
                } else {
                    L[gid * N + i] /= U[i * N + i];
                }
            }
        }
    }
}
