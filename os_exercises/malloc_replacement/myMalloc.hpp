#include <iostream>
#define PAGE_SIZE 4096

struct keyPair {
    
    void* key;      //pointer to the memory
    size_t value;   //size of the chunk of memory used

};

class MyMalloc {

public:

    keyPair* allocationSizeTable;
    int pages;
    size_t tableSize;
    int setElements;

    
    MyMalloc();

    ~MyMalloc();

    void tableInsert(keyPair pair);

    size_t tableDelete(void* key);

    void tableGrow();

    void* allocate(size_t bytesToAllocate);

    void deallocate(void* ptr);

    size_t hashHelper(size_t table, void* key);

};