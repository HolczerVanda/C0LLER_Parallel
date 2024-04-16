import time
import random
import matplotlib.pyplot as plt

def luDecomposition(A):
    N = len(A)
    L = [[0] * N for _ in range(N)]
    U = [[0] * N for _ in range(N)]

    for i in range(N):
        L[i][i] = 1

        for j in range(i, N):
            U[i][j] = A[i][j]
            for k in range(i):
                U[i][j] -= L[i][k] * U[k][j]

        for j in range(i + 1, N):
            L[j][i] = A[j][i]
            for k in range(i):
                L[j][i] -= L[j][k] * U[k][i]
            if U[i][i] == 0:
                raise ValueError("A zero pivot encountered. LU decomposition cannot proceed.")
            else:
                L[j][i] /= U[i][i]
    return L, U


if __name__ == "__main__":
    execution_times = []
    sizes = range(100, 501, 50)

    for N in sizes:
        A = [[random.randint(1, 10) for _ in range(N)] for _ in range(N)]

        start_time = time.time()
        L, U = luDecomposition(A)
        end_time = time.time()

        execution_time_seconds = end_time - start_time
        execution_times.append(execution_time_seconds)

        print(f"N = {N}, Execution time: {execution_time_seconds:.6f} seconds")

    plt.plot(sizes, execution_times, marker='o')
    plt.title('Execution Time vs. Matrix Size')
    plt.xlabel('Matrix Size (N)')
    plt.ylabel('Execution Time (seconds)')
    plt.grid(True)
    plt.show()
