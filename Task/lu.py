import numpy as np
import pyopencl as cl
import random
import time
import matplotlib.pyplot as plt

def luDecomposition(A, queue, N):
    mf = cl.mem_flags
    ctx = queue.get_info(cl.command_queue_info.CONTEXT)
    A_buf = cl.Buffer(ctx, mf.READ_ONLY | mf.COPY_HOST_PTR, hostbuf=A)
    L = np.zeros((N, N), dtype=np.float64)
    U = np.zeros((N, N), dtype=np.float64)

    L_buf = cl.Buffer(ctx, mf.WRITE_ONLY, L.nbytes)
    U_buf = cl.Buffer(ctx, mf.WRITE_ONLY, U.nbytes)

    with open('kernel.cl', 'r') as f:
        kernel_src = f.read()

    prg = cl.Program(ctx, kernel_src).build()

    event = prg.luDecomposition(queue, (N,), None, A_buf, L_buf, U_buf, np.int32(N))
    event.wait()

    cl.enqueue_copy(queue, L, L_buf)
    cl.enqueue_copy(queue, U, U_buf)
    queue.finish()

    return L, U, event

def calculate_determinant(U, N):
    det = 1.0
    for i in range(N):
        det *= U[i, i]
    return det

if __name__ == "__main__":
    execution_times = []
    for N in range(600, 2001, 200):
        A = np.array([[random.randint(1, 10) for _ in range(N)] for _ in range(N)], dtype=np.float64)

        platform = cl.get_platforms()[0]
        device = platform.get_devices()[0]
        ctx = cl.Context([device])
        queue = cl.CommandQueue(ctx, properties=cl.command_queue_properties.PROFILING_ENABLE)

        start_time = time.time()
        L, U, event = luDecomposition(A.flatten(), queue, N)
        end_time = time.time()

        execution_time_seconds = end_time - start_time
        execution_times.append(execution_time_seconds)

        print(f"N = {N}, Execution time: {execution_time_seconds:.6f} seconds")

        with open('results.txt', 'a') as results_file:
            results_file.write(f"{N};{execution_time_seconds}\n")

    plt.plot(range(600, 2001, 200), execution_times, marker='o')
    plt.title('Execution Time vs. Matrix Size')
    plt.xlabel('Matrix Size')
    plt.ylabel('Execution Time (seconds)')
    plt.grid(True)
    plt.show()
