#pragma once

#include "pr_lapackpp.hh"
#include "pr_cuda_macros.hh"

#include <cuda.h>
#include <cuda_runtime.h>

#include <cstdint>
#include <vector>
#include <chrono>
#include <numeric>

template <typename T>
void cholqr_nocuda(int64_t m, int64_t k, T* A_device, int64_t lda, T* R_device, T* ldr, lapack::Queue &queue) {
    using lapack::device_info_int;
    device_info_int* d_info = blas::device_malloc< device_info_int >( 1, queue );
    blas::syrk(Layout::ColMajor, Uplo::Upper, Op::Trans, k, m, 1.0, A_device, lda, 0.0, R_device, k, queue);
    lapack::potrf(Uplo::Upper, k, R_device, k, d_info, queue);
    blas::trsm(Layout::ColMajor, Side::Right, Uplo::Upper, Op::NoTrans, Diag::NonUnit, m, k, 1.0, R_device, k, A_device, lda, queue);
    return;
}

template <typename T>
void cholqr_offload_nocuda(int64_t m, int64_t n, T* A, int64_t lda, T* R, int64_t ldr) {
    lapack::Queue queue(0);
    T* A_device, R_device;
    cudaMalloc(&A_device, lda * n * sizeof(T));
    cudaMalloc(&R_device, ldr * n * sizeof(T));
    cudaMemcpy(A_device, A, lda * n * sizeof(T), cudaMemcpyHostToDevice);
    cudaMemcpy(R_device, R, ldr * n * sizeof(T), cudaMemcpyHostToDevice);
    cholqr_nocusolver(m, n, A_device, lda, R_device, ldr, queue);
    queue.sync();
    cudaMemcpy(A_device, A, lda * n * sizeof(T), cudaMemcpyDeviceToHost);
    cudaMemcpy(R_device, R, ldr * n * sizeof(T), cudaMemcpyDeviceToHost);
}
