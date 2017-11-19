#ifndef _INC_MEMORYMODULE_H
#define _INC_MEMORYMODULE_H

#include "baseTypes.h"

// Memory backend interface
class MemoryModule {
// All methods return 0 on success and non-zero on error
public:
    // Read memory from (module) offset into buffer
    virtual int readMemory(SizeType offset, void *buffer, SizeType len) = 0;

    // Write buffer into memory at (moudle) offset
    virtual int writeMemory(SizeType offset, const void *buffer, SizeType len) = 0;

    // Allow for proper polymorphic destruction
    virtual ~MemoryModule() { }
};

#endif//_INC_MEMORYMODULE_H