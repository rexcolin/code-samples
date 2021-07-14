#include "stdSum.hpp"
#include "ompCustomSum.hpp"
#include <utility>
#include <chrono>
#include <iostream>


int main (int argc, const char** argv) {

/**
 * setup for tests
 * */

    long double total_time = 0;

    size_t N = atoi(argv[1]);
    int* arr = new int[N];


/**
 * ******************************* TEST 1 ********************************
 * for c++ version
 * */
    for (int i = 0; i < N; i++) {
        arr[i] = 1;
    }

    for(int j = 0; j < atoi(argv[3]); j++) {

        std::pair<int, double> dumb_return = parallel_sum_std<int>(arr, N, atoi(argv[2]));

        total_time += dumb_return.second;

        // make sure the function is adding up the array correctly
        // since the array is full of 1's, it should be equal to N
        assert(dumb_return.first == N);
    }

    std::cout << total_time/atof(argv[3]) << " is the average time it took with parallel_sum_std()\n";


/**
 * ******************************* TEST 2 ********************************
 * for custom omp
 * */

    // reset time for next test
    total_time = 0;

    for(int j = 0; j < atoi(argv[3]); j++) {

        std::pair<int, double> dumb_return = parallel_sum_omp1<int>(arr, N, atoi(argv[2]));

        total_time += dumb_return.second;

        assert(dumb_return.first == N);
    }

    std::cout << total_time/atof(argv[3]) << " is the average time it took with parallel_sum_omp1()\n";


/**
 * ******************************* TEST 3 ********************************
 * for built-in omp
 * */

    // reset time for next test
    total_time = 0;

    for(int j = 0; j < atoi(argv[3]); j++) {

        std::pair<int, double> dumb_return = parallel_sum_omp_builtin<int>(arr, N, atoi(argv[2]));

        total_time += dumb_return.second;

        assert(dumb_return.first == N);
    }

    std::cout << total_time/atof(argv[3]) << " is the average time it took with parallel_sum_omp_builtin()()\n";


    // clean up the heap after tests
    delete[] arr;
}