#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <mpi.h>
#include <chrono>

using namespace std;

void loadMatrix(const string& filename, vector<long long>& matrix, int& n) {
    ifstream file(filename);
    file >> n;
    matrix.resize(n * n);
    for (int i = 0; i < n * n; ++i) file >> matrix[i];
}

void saveResult(const string& filename, const vector<long long>& matrix, int n, double duration, int num_procs) {
    ofstream file(filename);
    file << n << "\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file << matrix[i * n + j];
            if (j + 1 < n) file << " ";
        }
        file << "\n";
    }
    file << "Time: " << duration << "\n";
    file << "Processes: " << num_procs << "\n";
    file.close();
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_procs);

    int n = 0;
    vector<long long> A, B, C;

    if (rank == 0) {
        loadMatrix("matrix_a.txt", A, n);
        loadMatrix("matrix_b.txt", B, n);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    B.resize(n * n);
    MPI_Bcast(B.data(), n * n, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    int rows_per_proc = n / size_procs;
    int remainder = n % size_procs;
    int my_rows = rows_per_proc + (rank < remainder ? 1 : 0);
    int my_start = rank * rows_per_proc + min(rank, remainder);

    vector<long long> local_A(my_rows * n);
    vector<long long> local_C(my_rows * n, 0);

    if (rank == 0) {
        for (int p = 0; p < size_procs; ++p) {
            int p_rows = rows_per_proc + (p < remainder ? 1 : 0);
            int p_start = p * rows_per_proc + min(p, remainder);
            if (p == 0) {
                for (int r = 0; r < p_rows; ++r)
                    for (int c = 0; c < n; ++c)
                        local_A[r * n + c] = A[(p_start + r) * n + c];
            } else {
                MPI_Send(&A[p_start * n], p_rows * n, MPI_LONG_LONG_INT, p, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(local_A.data(), my_rows * n, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    auto t_start = chrono::high_resolution_clock::now();

    for (int i = 0; i < my_rows; ++i) {
        for (int j = 0; j < n; ++j) {
            long long sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += local_A[i * n + k] * B[k * n + j];
            }
            local_C[i * n + j] = sum;
        }
    }

    if (rank == 0) {
        C.resize(n * n);
        for (int p = 0; p < size_procs; ++p) {
            int p_rows = rows_per_proc + (p < remainder ? 1 : 0);
            int p_start = p * rows_per_proc + min(p, remainder);
            if (p == 0) {
                for (int r = 0; r < p_rows; ++r)
                    for (int c = 0; c < n; ++c)
                        C[(p_start + r) * n + c] = local_C[r * n + c];
            } else {
                MPI_Recv(&C[p_start * n], p_rows * n, MPI_LONG_LONG_INT, p, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    } else {
        MPI_Send(local_C.data(), my_rows * n, MPI_LONG_LONG_INT, 0, 1, MPI_COMM_WORLD);
    }

    auto t_end = chrono::high_resolution_clock::now();
    double duration = chrono::duration<double>(t_end - t_start).count();

    if (rank == 0) {
        saveResult("result_c.txt", C, n, duration, size_procs);
        cout << "Size: " << n << "x" << n << endl;
        cout << "Processes: " << size_procs << endl;
        cout << "Time: " << duration << " sec" << endl;
    }

    MPI_Finalize();
    return 0;
}