
__kernel void luDecomposition(__global double* A, __global double* L, __global double* U, const int N) {
    int gid = get_global_id(0);
    int i, j, k;

    for (i = 0; i < N; i++) {

        if (gid >= i) {
            U[i * N + gid] = A[i * N + gid];
            for (k = 0; k < i; k++) {
                U[i * N + gid] -= L[i * N + k] * U[k * N + gid];
            }
        }

        if (gid >= i) {
            if (gid == i)
                L[i * N + i] = 1;
            else {
                L[gid * N + i] = A[gid * N + i];
                for (k = 0; k < i; k++) {
                    L[gid * N + i] -= L[gid * N + k] * U[k * N + i];
                }
                L[gid * N + i] /= U[i * N + i];
            }
        }
    }
}
