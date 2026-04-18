#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <windows.h>

using namespace std;

void loadMatrix(const string& filename, vector<long long>& matrix, size_t& n) {
    ifstream file(filename);
    file >> n;
    matrix.resize(n * n);
    for (size_t i = 0; i < n * n; ++i) {
        file >> matrix[i];
    }
}

void saveResult(const string& filename, const vector<long long>& matrix, size_t n) {
    ofstream file(filename);
    file << n << "\n";
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            file << matrix[i * n + j];
            if (j + 1 < n) file << " ";
        }
        file << "\n";
    }
    file.close();
}

vector<long long> multiplyMatrices(const vector<long long>& A, 
                                   const vector<long long>& B, size_t n) {
    vector<long long> C(n * n, 0);
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            long long sum = 0;
            for (size_t k = 0; k < n; ++k) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
    return C;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    vector<long long> A, B;
    size_t nA, nB;

    loadMatrix("matrix_a.txt", A, nA);
    loadMatrix("matrix_b.txt", B, nB);

    auto t_start = chrono::high_resolution_clock::now();
    vector<long long> result = multiplyMatrices(A, B, nA);
    auto t_end = chrono::high_resolution_clock::now();

    double duration = chrono::duration<double>(t_end - t_start).count();

    saveResult("result_c.txt", result, nA);
    
    cout << "Умножение завершено" << endl;
    cout << "Размер: " << nA << "x" << nA << endl;
    cout << "Время: " << duration << " сек" << endl;
    cout << "Объем задачи: " << nA * nA << " элементов" << endl;

    return 0;
}
