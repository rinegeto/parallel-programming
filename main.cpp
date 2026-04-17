#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <iomanip>

using namespace std;

int main() {
    ifstream inA("matrix_a.txt"), inB("matrix_b.txt");
    if (!inA || !inB) { 
        cerr << "Error: cannot open input files" << endl; 
        return 1; 
    }

    int N;
    inA >> N;
    vector<double> A(N * N);
    for (int i = 0; i < N * N; ++i) 
        inA >> A[i];

    int N2; 
    inB >> N2;
    if (N != N2) { 
        cerr << "Error: matrix dimensions don't match" << endl; 
        return 1; 
    }
    
    vector<double> B(N * N);
    for (int i = 0; i < N * N; ++i) 
        inB >> B[i];

    vector<double> C(N * N, 0.0);

    auto start = chrono::high_resolution_clock::now();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0.0;
            for (int k = 0; k < N; ++k)
                sum += A[i * N + k] * B[k * N + j];
            C[i * N + j] = sum;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double time_sec = chrono::duration<double>(end - start).count();

    ofstream out("result_c.txt");
    out << fixed << setprecision(6) << N << "\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            out << C[i * N + j] << (j == N - 1 ? "\n" : " ");
    }
    out.close();

    cout << "Size: " << N << "x" << N << endl;
    cout << "Time: " << time_sec << " sec" << endl;
    cout << "Result saved to result_c.txt" << endl;
    
    return 0;
}
