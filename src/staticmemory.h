#ifndef _INC_STATICMEMORY_H
#define _INC_STATICMEMORY_H

#include "memorymodule.h"

class StaticMemory : public MemoryModule {
public:
    StaticMemory(SizeType size);
    virtual int readMemory(SizeType offset, void *buffer, SizeType len);
    virtual int writeMemory(SizeType offset, const void *buffer, SizeType len);

private:
    void *buffer;
};

#endif//_INC_STATICMEMORY_H