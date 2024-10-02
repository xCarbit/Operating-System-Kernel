//
// Created by os on 7/22/23.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_H
#define PROJECT_BASE_MEMORYALLOCATOR_H


#include "../lib/hw.h"

struct FreeMem{
    FreeMem* next;
    FreeMem* prev;
    size_t size;

};


class MemoryAllocator {
private:

    FreeMem* head;


public:

    void* mem_alloc (size_t _size);
    int mem_free (void* adr);

  static  MemoryAllocator& Singleton();


private:

    MemoryAllocator(){}

    void tryToJoin (FreeMem* cur);
};


#endif //PROJECT_BASE_MEMORYALLOCATOR_H

