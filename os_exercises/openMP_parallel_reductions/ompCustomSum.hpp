#ifndef CUSTOM_OMP
#define CUSTOM_OMP

#include "omp.h"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <utility>


template<typename T>
auto parallel_sum_omp1(T a[], size_t N, size_t num_threads) {

    //start timer
    auto t1 = std::chrono::high_resolution_clock::now();

    T total_sum = 0;

    // set number of threads for omp to use
    omp_set_num_threads(num_threads);

    #pragma omp parallel
    {
        
        int id = omp_get_thread_num();
        size_t Nthrds = omp_get_num_threads();
        // if (id == 0)
        //     std::cout << Nthrds;
        
        size_t istart = id * N/Nthrds;
        size_t iend = (id + 1) * N/Nthrds;

        if (id == Nthrds-1)
            iend = N;

        T chunk_sum = 0;
        for (int i = istart; i < iend; i++) {
            chunk_sum += a[i];
        }

        #pragma omp critical
            total_sum += chunk_sum;

    }    

    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    return std::make_pair(total_sum, time_span.count());

}


template<typename T>
auto parallel_sum_omp_builtin(T a[], size_t N, size_t num_threads) {

    // start timer
    auto t1 = std::chrono::high_resolution_clock::now();

    T total_sum = 0;

    // set number of threads for omp to use
    omp_set_num_threads(num_threads);

    // omp built-in reduction, specifies that 
    #pragma omp parallel for reduction( + : total_sum )
    {
        for(int i = 0; i < N; i++) {
            total_sum += a[i];
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    return std::make_pair(total_sum, time_span.count());
}

#endif