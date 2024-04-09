import numpy as np
import pyopencl as cl

N = 3

def luDecomposition(A, queue):
    mf = cl.mem_flags
    ctx = queue.get_info(cl.command_queue_info.CONTEXT)
    A_buf = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=A)
    L = np.zeros((N, N), dtype=float)
    U = np.zeros((N, N), dtype=float)

    L_buf = cl.Buffer(ctx, mf.WRITE_ONLY, L.nbytes)
    U_buf = cl.Buffer(ctx, mf.WRITE_ONLY, U.nbytes)

    prg = cl.Program(ctx, """
        __kernel void luDecomposition(__global double* A, __global double* L, __global double* U, const int N) {
            int i = get_global_id(0);
            int j, k;

            // Initialize L and U matrices
            for (j = 0; j < N; j++) {
                L[i * N + j] = 0;
                U[i * N + j] = 0;
            }

            // Compute U matrix
            for(j = i; j < N; j++) {
                U[i * N + j] = A[i * N + j];
                for(k = 0; k < i; k++) {
                    U[i * N + j] -= L[i * N + k] * U[k * N + j];
                }
            }

            // Compute L matrix
            for(j = i; j < N; j++) {
                if (i == j)
                    L[i * N + i] = 1;  // Diagonal elements of L are 1
                else {
                    L[j * N + i] = A[j * N + i];
                    for(k = 0; k < i; k++) {
                        L[j * N + i] -= L[j * N + k] * U[k * N + i];
                    }
                    L[j * N + i] /= U[i * N + i];  // Division to get correct value
                }
            }
        }
    """).build()

    prg.luDecomposition(queue, (N,), None, A_buf, L_buf, U_buf, np.int32(N))
    cl.enqueue_copy(queue, L, L_buf)
    cl.enqueue_copy(queue, U, U_buf)
    queue.finish()

    return L, U

def calculate_determinant(U):
    det = 1.0
    for i in range(N):
        det *= U[i, i]
    return det

if __name__ == "__main__":
    A = np.array([[13, -41, 10], [0, -4, 1], [3, 11, 9]], dtype=float)

    platform = cl.get_platforms()[0]
    device = platform.get_devices()[0]
    ctx = cl.Context([device])
    queue = cl.CommandQueue(ctx)

    L, U = luDecomposition(A.flatten(), queue)
    det = calculate_determinant(U)

    print("\nLower triangular matrix (L):")
    print(L)

    print("\nUpper triangular matrix (U):")
    print(U)

    print("\nDeterminant of the matrix:", det)
