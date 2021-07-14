#include <sys/mman.h>
#include <math.h>
#include "myMalloc.hpp"

/**
 * Constructor
 * calls mmap
 * */
MyMalloc::MyMalloc() {

    // calls mmap and sets the pointer to allocationSizeTable
    allocationSizeTable = (keyPair*) mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    pages = 1;
    tableSize = PAGE_SIZE / sizeof(keyPair);
    setElements = 0;

    // sets the memory just allocated to 0
    for (int i = 0; i < tableSize; i++) {
        allocationSizeTable[i].key = NULL;
        allocationSizeTable[i].value = NULL;
    }
}


/**
 * Destructor
 * calls munmap
 * */
MyMalloc::~MyMalloc() {

    // deallocate each keyPair in the table (calls munmap in the method)
    for (size_t i = 0; i < tableSize; i++) {
        if (allocationSizeTable[i].key != NULL)
            deallocate(allocationSizeTable[i].key);
    }

    // unmaps the whole table
    if ( munmap(allocationSizeTable, pages * PAGE_SIZE) != 0) {
        perror("Table didn't unmap in destructor");
    }
}


/**
 * method to insert a keyPair into the table
 * */
void MyMalloc::tableInsert(keyPair pair) {

    // Make sure there's an empty slot in the table, if not then grow table
    if (setElements == tableSize) {
        tableGrow();
    }

    // Get the index using the hashHelper
    size_t index = hashHelper(tableSize, pair.key);
    
    // Find an empty slot, starting with the index we just found
    for (index; allocationSizeTable[index].key != NULL; index = (index + 1) % tableSize) {
    }

    //insert keyPair, then increment element counter
    allocationSizeTable[index] = pair;
    setElements++;
}


/**
 * method to delete entry from table, makes use of the hashHelper method
 * */
size_t MyMalloc::tableDelete(void* key) {

    // get index using the hasHelper
    size_t index = hashHelper(tableSize, key);

    // find the slot the key is actually in (because collisions)
    for (index; allocationSizeTable[index].key != key; index = (index + 1) % tableSize) {
    }

    //save value before we delete it
    size_t retValue = allocationSizeTable[index].value;

    //set values at that index to NULL, decrement number of elements in table
    allocationSizeTable[index].key = NULL;
    allocationSizeTable[index].value = NULL;
    setElements--;

    return retValue;
}

/**
 * Method to grow hash table when it fills.
 * makes use of the hashHelper method and mmap.
 * */
void MyMalloc::tableGrow() {

    keyPair* newAllocationTable = (keyPair*) mmap(NULL, (PAGE_SIZE * pages * 2), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // Make sure the new table is empty when I start filling it in
    for (int i = 0; i < (PAGE_SIZE * (pages * 2) / sizeof(keyPair)); i++) {
        newAllocationTable[i].key = NULL;
        newAllocationTable[i].value = NULL;
    }

    pages *= 2;
    size_t newTableSize = PAGE_SIZE * pages / sizeof(keyPair);
    for (int i = 0; i < tableSize; i++) {

        keyPair temp = allocationSizeTable[i];
        if (temp.key != NULL) {
            size_t newIndex = hashHelper(newTableSize, allocationSizeTable[i].key);
            newAllocationTable[newIndex] = allocationSizeTable[i];
        }
    }

    //increase the number of pages and recalculate table size
    tableSize = newTableSize;

    //munmap returns -1 on a failure to deallocate
    if (munmap(allocationSizeTable, (size_t) pages / 2 * PAGE_SIZE) != 0) {
        perror("Old table didn't unmap in tableGrow()");
    }

    allocationSizeTable = newAllocationTable; 
}


/**
 * method to allocate memory
 * uses mmap, returns pointer to allocated space
 * */
void* MyMalloc::allocate(size_t bytesToAllocate) {

    //find the number of pages I need, sets the variable to that multiple of 4096
    size_t tempPageSize = ceil(1.0 * bytesToAllocate / PAGE_SIZE) * PAGE_SIZE;

    // uses mmap to allocate space and get a pointer
    void* ptr = mmap(NULL, tempPageSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // make keyPair to insert into table
    keyPair pair { ptr, tempPageSize };

    tableInsert(pair);

    return ptr;
}


void MyMalloc::deallocate(void* ptr) {

    // get the value from the keypair in the table, also delete it from the table
    size_t size = tableDelete(ptr);

    // unmap
    if (munmap(ptr, size) != 0) {
        perror("Didn't unmap the pointer in deallocate");
    }
}

/**
 * Method to use to find a hash
 * is dependant on the size of the hash table,
 * this allows us to use it when growing a hash table
 * */
size_t MyMalloc::hashHelper(size_t table, void* key) {
    int x = PAGE_SIZE;
    return ((size_t) key >> (int) log2(x)) % table;
}