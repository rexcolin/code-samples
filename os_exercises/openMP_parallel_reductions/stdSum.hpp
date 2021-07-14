#ifndef STD_SUM
#define STD_SUM

#include <cstdlib>
#include <chrono>
#include <atomic>
#include <vector>
#include <utility>
#include <thread>


template<typename T>
void parallel_sum_std_helper(T a[], size_t start, size_t end, std::atomic<T> &ret) {

    T chunk_sum = 0;
    for (size_t i = start; i < end; i++) {
        chunk_sum += a[i];
    }

    ret += chunk_sum;

};


template<typename T>
auto parallel_sum_std(T a[], size_t N, size_t num_threads) {

    auto t1 = std::chrono::high_resolution_clock::now();

    std::atomic<T> total_sum(0);

    std::vector<std::thread> threads;
    for(int i = 0; i < num_threads; i++) {

        size_t step_size = N / num_threads;
        size_t start = step_size * i;
        size_t end;
        if (i == num_threads - 1)
            end = N;
        else
            end = start + step_size;

        threads.push_back( std::thread(parallel_sum_std_helper<T>, a, start, end, std::ref(total_sum) ) );
    }

    for (int i = 0; i < num_threads; i++) {
        threads.at(i).join();
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    return std::make_pair(total_sum.load(), time_span.count());
};



#endif