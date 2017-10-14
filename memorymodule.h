#ifndef _INC_MEMORYMODULE_H
#define _INC_MEMORYMODULE_H

typedef unsigned long long SizeType;

// Memory backend interface
class MemoryModule {
public:
    virtual int readMemory(SizeType offset, void *buffer, SizeType len, void *user_data) = 0;
    virtual int writeMemory(SizeType offset, const void *buffer, SizeType len, void *user_data) = 0;
};

#endif//_INC_MEMORYMODULE_H