#include <iostream>
#include <chrono>
#include <time.h>
#include <string>
#include "myMalloc.hpp"



int main (int argc, const char* argv[]) {

/** ***************
 *  TESTS
 * */

    // instance of class to use with tests
    MyMalloc test;


    /* TEST 1: if I can read from the new pointer, after feeding some data into it
    */

        // first string to allocate
    char* str1 = "1111"; //4 chars long

        // pointer for that string, also allocates space for it.
    char* str1ptr = (char*) test.allocate(sizeof(str1));

    for (int i = 0; i < 4; i++) {
        str1ptr[i] = '1';
    }

    for (int i = 0; i < 4; i++) {
        assert(str1[i] == str1ptr[i]);
    }


    /* TEST 2: that the table is inserting another element when I allocate something new
    */

        //make sure the table has one element after the test above
    assert(test.setElements == 1);
        
        //make a variable to allocate, allocate it, then check to see that the table inserted the variable
    uint8_t x = 2;

    void* deallocatePtr = test.allocate(sizeof(x));

    assert(test.setElements == 2);


    /* TEST 2.5: that deallocating removes a pointer from the table
    */
    test.deallocate(deallocatePtr);

    assert(test.setElements == 1);


    /* TEST 3: that the table will grow when it fills up
    */

        // save old table size in a variable
    size_t oldTableSize = test.tableSize;

        // loop allocating space until it needs to grow
    for (size_t i = test.setElements; i < test.tableSize; i++) {
        uint8_t temp = 32;
        test.allocate(sizeof(temp));
            // the last time it loops is when the setElements is equal to the size, then it allocates again, which should trigger the tabeGrow() method
    }

    uint8_t temp = 32;
    test.allocate(sizeof(temp));

        // make sure that the new tableSize is greater than the old one
    assert(oldTableSize < test.tableSize);



/** **************************
 *  BENCHMARKS
 * */

    // measure an empty loop for use in later benchmarks
    clock_t beforeEmptyLoop = clock();
    for (int i = 0; i < 2000; i++) {};
    clock_t afterEmptyLoop = clock();
    double emptyLoopTime = ((double) (afterEmptyLoop - beforeEmptyLoop)) / CLOCKS_PER_SEC;


    /**
     * time for MyMalloc::allocate();
     * */

    MyMalloc benchmarks1;

    void* ptrArr[2000];

    clock_t beforeMyMallocAllocateLoop = clock();

    for (int i = 0; i < 2000; i++) {
        ptrArr[i] = benchmarks1.allocate(sizeof(i));
    }

    clock_t afterMyMallocAllocateLoop = clock();

    double myMallocAllocateTime = ((double) (afterMyMallocAllocateLoop - beforeMyMallocAllocateLoop) / CLOCKS_PER_SEC - emptyLoopTime) / 2000;

    std::cout << "Time to run MyMalloc::allocate() is: " << myMallocAllocateTime << std::endl;


    /**
     * time for malloc();
     * */

    void* ptrArr2[2000];
    clock_t beforeMallocLoop = clock();

    for (int i = 0; i < 2000; i++) {
        ptrArr2[i] = malloc(sizeof(i));
    }

    clock_t afterMallocLoop = clock();

    double mallocTime = ((double) (afterMallocLoop - beforeMallocLoop) / CLOCKS_PER_SEC - emptyLoopTime) / 2000;

    std::cout << "Time to run malloc() is: " << mallocTime << std::endl;
    std::cout << std::endl;


    /**
     * time for MyMalloc::deallocate()
     * */

    clock_t beforeMyMallocDeallocate = clock();

    for (int i = 0; i < 2000; i++) {
        benchmarks1.deallocate(ptrArr[i]);
    }

    clock_t afterMyMallocDeallocate = clock();

    double myDeallocateTime = ((double) (afterMyMallocDeallocate - beforeMyMallocDeallocate) / CLOCKS_PER_SEC - emptyLoopTime) / 2000;

    std::cout << "Time to run MyMalloc::deallocate() is: " << myDeallocateTime << std::endl;


    /**
     * time for free()
     * */

    clock_t beforeFree = clock();

    for (int i = 0; i < 2000; i++) {
        free(ptrArr2[i]);
    }

    clock_t afterFree = clock();

    double freeTime = ((double) (afterFree - beforeFree) / CLOCKS_PER_SEC - emptyLoopTime) / 2000;

    std::cout << "Time for free() is: " << freeTime << std::endl;


    return 0;
}