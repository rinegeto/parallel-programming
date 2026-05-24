#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <mpi.h>
#include <chrono>

using namespace std;

bool loadMatrix(const string& filename, vector<long long>& matrix, int& n) {
    ifstream file(filename);
    if (!file.is_open()) return false;
    file >> n;
    matrix.assign(n * n, 0);
    for (int i = 0; i < n * n; ++i) file >> matrix[i];
    return true;
}

void saveResult(const string& filename, const vector<long long>& matrix, int n, double duration, int num_procs) {
    ofstream file(filename);
    file << n << "\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file << matrix[i * n + j] << (j + 1 < n ? " " : "");
        }
        file << "\n";
    }
    file << "Time: " << duration << "\n";
    file << "Processes: " << num_procs << "\n";
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_procs);

    vector<int> sizes = {100, 200, 400, 800, 1200, 2000};

    for (int n_size : sizes) {
        string input_a = "matrixA" + to_string(n_size) + ".txt";
        string input_b = "matrixB" + to_string(n_size) + ".txt";
        string output_c = "result_c" + to_string(n_size) + ".txt";

        int n = 0;
        vector<long long> A, B, C;

        bool skip = false;
        if (rank == 0) {
            if (!loadMatrix(input_a, A, n) || !loadMatrix(input_b, B, n)) {
                cerr << "Error loading files for size " << n_size << endl;
                skip = true;
            }
        }

        MPI_Bcast(&skip, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);
        if (skip) continue;
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank != 0) B.resize(n * n);
        MPI_Bcast(B.data(), n * n, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

        vector<int> sendcounts(size_procs), displs(size_procs);
        int sum = 0;
        for (int i = 0; i < size_procs; ++i) {
            int rows = (n / size_procs) + (i < (n % size_procs) ? 1 : 0);
            sendcounts[i] = rows * n;
            displs[i] = sum;
            sum += sendcounts[i];
        }

        int my_rows = sendcounts[rank] / n;
        vector<long long> local_A(sendcounts[rank]);
        vector<long long> local_C(sendcounts[rank], 0);


        MPI_Scatterv(A.data(), sendcounts.data(), displs.data(), MPI_LONG_LONG_INT,
                     local_A.data(), sendcounts[rank], MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

        auto t_start = chrono::high_resolution_clock::now();

        
        for (int i = 0; i < my_rows; ++i) {
            for (int j = 0; j < n; ++j) {
                long long sum_val = 0;
                for (int k = 0; k < n; ++k) {
                    sum_val += local_A[i * n + k] * B[k * n + j];
                }
                local_C[i * n + j] = sum_val;
            }
        }

        auto t_end = chrono::high_resolution_clock::now();
        double local_duration = chrono::duration<double>(t_end - t_start).count();

        if (rank == 0) C.resize(n * n);
        MPI_Gatherv(local_C.data(), sendcounts[rank], MPI_LONG_LONG_INT,
                    C.data(), sendcounts.data(), displs.data(), MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

        double max_duration;
        MPI_Reduce(&local_duration, &max_duration, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            saveResult(output_c, C, n, max_duration, size_procs);
            cout << "Finished size: " << n << "x" << n << " in " << max_duration << "s" << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
