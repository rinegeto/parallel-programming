#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

using namespace std;

__global__ void matMulKernel(const long long* A, const long long* B, long long* C, int n) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < n && col < n) {
        long long sum = 0;
        for (int k = 0; k < n; ++k) {
            sum += A[row * n + k] * B[k * n + col];
        }
        C[row * n + col] = sum;
    }
}

void loadMatrix(const string& filename, vector<long long>& matrix, int& n) {
    ifstream file(filename);
    file >> n;
    matrix.resize(n * n);
    for (int i = 0; i < n * n; ++i) file >> matrix[i];
}

void saveResult(const string& filename, const vector<long long>& matrix, int n, double time, int blockSize) {
    ofstream file(filename);
    file << n << "\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file << matrix[i * n + j];
            if (j + 1 < n) file << " ";
        }
        file << "\n";
    }
    file << "Time: " << time << "\n";
    file << "BlockSize: " << blockSize << "\n";
    file.close();
}

int main(int argc, char* argv[]) {
    int blockSize = 16; 
    if (argc > 1) blockSize = atoi(argv[1]);

    if (blockSize * blockSize > 1024) {
        cerr << "Error: Block size too large (max 1024 threads per block)" << endl;
        return 1;
    }

    int n;
    vector<long long> h_A, h_B, h_C;
    loadMatrix("matrix_a.txt", h_A, n);
    loadMatrix("matrix_b.txt", h_B, n);
    h_C.resize(n * n);

    long long *d_A, *d_B, *d_C;
    size_t bytes = n * n * sizeof(long long);
    cudaMalloc(&d_A, bytes);
    cudaMalloc(&d_B, bytes);
    cudaMalloc(&d_C, bytes);

    cudaMemcpy(d_A, h_A.data(), bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B.data(), bytes, cudaMemcpyHostToDevice);

    dim3 threadsPerBlock(blockSize, blockSize);
    dim3 numBlocks((n + blockSize - 1) / blockSize, (n + blockSize - 1) / blockSize);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);
    matMulKernel<<<numBlocks, threadsPerBlock>>>(d_A, d_B, d_C, n);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);
    double seconds = milliseconds / 1000.0;

    cudaMemcpy(h_C.data(), d_C, bytes, cudaMemcpyDeviceToHost);

    saveResult("result_c.txt", h_C, n, seconds, blockSize);

    cout << "CUDA multiplication completed" << endl;
    cout << "Size: " << n << "x" << n << endl;
    cout << "Block size: " << blockSize << "x" << blockSize << endl;
    cout << "Time: " << seconds << " sec" << endl;

    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
    cudaEventDestroy(start); cudaEventDestroy(stop);

    return 0;
}